#include <stdio.h>
#include <string.h>
#include "asset_name.h"
#include "audio_port.h"
#include "audio_thread.h"
#include "clock.h"
#include "crc32.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "music_engine.h"
#include "music_source.h"
#include "pcm_ramp.h"
#include "run_log.h"

#define FRAMES_PER_TICK (MUSIC_RATE / 60)
#define SAMPLES_PER_FRAME 2
#define GRAIN_BYTES (MUSIC_GRAIN_FRAMES * SAMPLES_PER_FRAME * (unsigned int)sizeof(short))
#define GRAIN_US 92880ULL
#define GAP_LIMIT_US GRAIN_US
#define BLOCK_LIMIT_US (2 * GRAIN_US)
#define RAMP_IN_FRAMES 512
#define WINDOW_GRAINS 11
#define IDLE_SLEEP_US 10000
#define QUIT_WAIT_US 1000000
#define LOOP_TEXT_MAX 64
#define NO_LOOP_POINT -1
#define LOG_FOR_TOOLS_MUSIC_THREAD "music thread priority=%d mask=0x%08x cpu=%d arm_mhz=%d fpscr=0x%08x"
#define LOG_FOR_TOOLS_MUSIC_PORT "music port result=0x%08x rate=%d grain=%d mode=%d adopt=%d system_volume=%d"
#define LOG_FOR_TOOLS_MUSIC_LOADED "music loaded name=%s bytes=%u read_us=%llu open_us=%llu frames=%u rate=%u channels=%u loop_start=%d loop_last=%d decoder_bytes=%u"
#define LOG_FOR_TOOLS_MUSIC_STARTED "music started name=%s looping=%d loop_start=%d loop_last=%d"
#define LOG_FOR_TOOLS_MUSIC_RAMP "music ramp at=%llu frames=%u from=%d to=%d"
#define LOG_FOR_TOOLS_MUSIC_WINDOW "music window name=%s index=%u at=%llu source_frame=%u grains=%u frames=%u crc32=%08x crc_total=%08x rest=%d wall_us=%llu underruns=%u late=%u loops=%u decode_us_avg=%llu decode_us_max=%llu block_us_avg=%llu block_us_max=%llu gap_us_max=%llu"
#define LOG_FOR_TOOLS_MUSIC_STOPPED "music stopped name=%s reason=%s submitted=%llu grains=%u crc_total=%08x underruns=%u late=%u"

struct music_wish {
    unsigned int generation;
    char track_name[ASSET_NAME_MAX];
    int should_play;
    int is_looping;
    int fade_ticks;
    int should_shut_down;
    int starve_ms;
};

struct playback_totals {
    unsigned long long frames_submitted;
    unsigned long long started_us;
    unsigned long long last_submit_ended_us;
    unsigned long long last_block_us;
    unsigned int grains_submitted;
    unsigned int crc_total;
    unsigned int underruns;
    unsigned int late;
};

struct window_totals {
    unsigned int index;
    unsigned long long first_frame;
    unsigned int first_source_frame;
    unsigned int grains;
    unsigned int crc;
    int rest;
    unsigned long long decode_us_total;
    unsigned long long decode_us_max;
    unsigned long long block_us_total;
    unsigned long long block_us_max;
    unsigned long long gap_us_max;
};

static char folder[FILE_PATH_MAX];
static int worker_is_running;

static struct music_wish mailbox;
static char prepared_name[ASSET_NAME_MAX];

static struct music_source source;
static struct pcm_ramp ramp;
static struct playback_totals playback;
static struct window_totals window;
static short grain[MUSIC_GRAIN_FRAMES * SAMPLES_PER_FRAME] __attribute__((aligned(AUDIO_PORT_BUFFER_ALIGNMENT)));
static unsigned char *track_data;
static char loaded_name[ASSET_NAME_MAX];
static int track_is_loaded;
static int is_playing;
static int is_stopping;
static unsigned int handled_generation;
static unsigned int fade_generation;
static int thread_line_is_written;

static void log_main_thread_facts(void)
{
    struct audio_thread_facts facts;

    audio_thread_read_facts(&facts);
    run_log_write("music main_thread priority=%d mask=0x%08x cpu=%d arm_mhz=%d", facts.priority, facts.cpu_mask, facts.cpu, facts.arm_mhz);
}

void music_engine_init(const char *music_folder)
{
    int result;

    snprintf(folder, sizeof(folder), "%s", music_folder);
    memset(&mailbox, 0, sizeof(mailbox));
    prepared_name[0] = '\0';
    track_data = NULL;
    loaded_name[0] = '\0';
    track_is_loaded = 0;
    is_playing = 0;
    is_stopping = 0;
    handled_generation = 0;
    fade_generation = 0;
    thread_line_is_written = 0;
    log_main_thread_facts();
    result = audio_thread_start(AUDIO_THREAD_MUSIC, music_engine_step);
    worker_is_running = result >= 0;
    if (!worker_is_running) {
        run_log_write("music thread_failed code=0x%08x", (unsigned int)result);
    }
}

static void post_wish(const char *name, int should_play, int is_looping, int fade_ticks)
{
    audio_mailbox_lock();
    mailbox.generation = mailbox.generation + 1;
    snprintf(mailbox.track_name, sizeof(mailbox.track_name), "%s", name);
    mailbox.should_play = should_play;
    mailbox.is_looping = is_looping;
    mailbox.fade_ticks = fade_ticks;
    prepared_name[0] = '\0';
    audio_mailbox_unlock();
}

void music_request_prepare(const char *name)
{
    post_wish(name, 0, 0, MUSIC_QUICK_FADE_TICKS);
}

int music_prepare_is_done(const char *name)
{
    int is_done;

    if (!worker_is_running) {
        return 1;
    }
    audio_mailbox_lock();
    is_done = strcmp(prepared_name, name) == 0;
    audio_mailbox_unlock();
    return is_done;
}

void music_request_play(const char *name, int is_looping)
{
    post_wish(name, 1, is_looping, MUSIC_QUICK_FADE_TICKS);
}

void music_request_stop(int fade_ticks)
{
    post_wish("", 0, 0, fade_ticks);
}

void music_request_starve(int milliseconds)
{
    audio_mailbox_lock();
    mailbox.starve_ms = milliseconds;
    audio_mailbox_unlock();
}

void music_shutdown(void)
{
    if (!worker_is_running) {
        return;
    }
    post_wish("", 0, 0, MUSIC_QUICK_FADE_TICKS);
    audio_mailbox_lock();
    mailbox.should_shut_down = 1;
    audio_mailbox_unlock();
    audio_thread_wait_for_end(AUDIO_THREAD_MUSIC, QUIT_WAIT_US);
    worker_is_running = 0;
}

static void read_mailbox(struct music_wish *wish)
{
    audio_mailbox_lock();
    *wish = mailbox;
    mailbox.starve_ms = 0;
    audio_mailbox_unlock();
}

static void publish_prepared_name(const char *name)
{
    audio_mailbox_lock();
    snprintf(prepared_name, sizeof(prepared_name), "%s", name);
    audio_mailbox_unlock();
}

static void release_track(void)
{
    if (track_is_loaded) {
        music_source_close(&source);
    }
    mem_free(track_data);
    track_data = NULL;
    track_is_loaded = 0;
    loaded_name[0] = '\0';
}

static void read_loop_points(const char *name)
{
    char path[FILE_PATH_MAX];
    char text[LOOP_TEXT_MAX];
    unsigned char *data;
    unsigned int size;
    unsigned int loop_start;
    unsigned int loop_last;

    if (!file_path_join(path, folder, name, ".loop") || file_read_whole(path, &data, &size) != FILE_OK) {
        return;
    }
    if (size >= sizeof(text)) {
        size = sizeof(text) - 1;
    }
    memcpy(text, data, size);
    text[size] = '\0';
    mem_free(data);
    if (sscanf(text, "%u %u", &loop_start, &loop_last) != 2 || loop_start > loop_last || loop_last >= source.stream.frame_count) {
        run_log_write("music bad_loop_file name=%s", name);
        return;
    }
    music_source_set_loop_points(&source, loop_start, loop_last);
}

static int logged_loop_start(void)
{
    return source.has_loop_points ? (int)source.loop_start : NO_LOOP_POINT;
}

static int logged_loop_last(void)
{
    return source.has_loop_points ? (int)source.loop_last : NO_LOOP_POINT;
}

static int read_track_file(const char *name, unsigned int *size)
{
    char path[FILE_PATH_MAX];
    int result;

    if (!file_path_join(path, folder, name, ".ogg")) {
        run_log_write("music path_too_long name=%s", name);
        return 0;
    }
    result = file_read_whole(path, &track_data, size);
    if (result == FILE_ERR_MEMORY) {
        run_log_write("music out_of_memory name=%s", name);
    } else if (result != FILE_OK) {
        run_log_write("music file_missing name=%s code=%d", name, result);
    }
    if (result != FILE_OK) {
        track_data = NULL;
        return 0;
    }
    return 1;
}

static void load_track(const char *name)
{
    unsigned long long started_us;
    unsigned long long read_ended_us;
    unsigned int size;
    int error;

    started_us = clock_now_us();
    if (!read_track_file(name, &size)) {
        return;
    }
    read_ended_us = clock_now_us();
    error = music_source_open(&source, track_data, size);
    if (error != VORBIS_STREAM_OK) {
        run_log_write("music decoder_failed name=%s error=%d", name, error);
        release_track();
        return;
    }
    track_is_loaded = 1;
    if (source.stream.sample_rate != MUSIC_RATE) {
        run_log_write("music wrong_rate name=%s rate=%u", name, source.stream.sample_rate);
        release_track();
        return;
    }
    read_loop_points(name);
    snprintf(loaded_name, sizeof(loaded_name), "%s", name);
    run_log_write(LOG_FOR_TOOLS_MUSIC_LOADED, name, size, read_ended_us - started_us, clock_now_us() - read_ended_us, source.stream.frame_count, source.stream.sample_rate, source.stream.channel_count, logged_loop_start(), logged_loop_last(), source.stream.decoder_bytes);
}

static void begin_ramp(int start_gain, int target_gain, unsigned int frames)
{
    pcm_ramp_begin(&ramp, start_gain, target_gain, frames);
    run_log_write(LOG_FOR_TOOLS_MUSIC_RAMP, playback.frames_submitted, frames, start_gain, target_gain);
}

static void log_window_if_any(void)
{
    unsigned int next_index;

    if (window.grains == 0) {
        return;
    }
    run_log_write(LOG_FOR_TOOLS_MUSIC_WINDOW, loaded_name, window.index, window.first_frame, window.first_source_frame, window.grains, window.grains * MUSIC_GRAIN_FRAMES, window.crc, playback.crc_total, window.rest, clock_now_us() - playback.started_us, playback.underruns, playback.late, source.loop_count, window.decode_us_total / window.grains, window.decode_us_max, window.block_us_total / window.grains, window.block_us_max, window.gap_us_max);
    next_index = window.index + 1;
    memset(&window, 0, sizeof(window));
    window.index = next_index;
}

static void note_window_start(void)
{
    if (window.grains == 0) {
        window.first_frame = playback.frames_submitted;
        window.first_source_frame = source.next_frame;
    }
}

static unsigned long long larger_of(unsigned long long first, unsigned long long second)
{
    return first > second ? first : second;
}

static int submit_grain(unsigned long long decode_us)
{
    unsigned long long submit_started_us;
    unsigned long long block_us;
    int rest;
    int result;

    rest = audio_port_frames_waiting(AUDIO_PORT_MUSIC);
    if (rest == 0 && playback.grains_submitted > 0) {
        playback.underruns = playback.underruns + 1;
    }
    window.crc = crc32_add(window.crc, (const unsigned char *)grain, GRAIN_BYTES);
    playback.crc_total = crc32_add(playback.crc_total, (const unsigned char *)grain, GRAIN_BYTES);
    submit_started_us = clock_now_us();
    result = audio_port_submit(AUDIO_PORT_MUSIC, grain);
    playback.last_submit_ended_us = clock_now_us();
    if (result < 0) {
        run_log_write("music submit_failed code=0x%08x", (unsigned int)result);
        return 0;
    }
    block_us = playback.last_submit_ended_us - submit_started_us;
    playback.last_block_us = block_us;
    playback.frames_submitted = playback.frames_submitted + MUSIC_GRAIN_FRAMES;
    playback.grains_submitted = playback.grains_submitted + 1;
    window.rest = rest;
    window.grains = window.grains + 1;
    window.decode_us_total = window.decode_us_total + decode_us;
    window.decode_us_max = larger_of(window.decode_us_max, decode_us);
    window.block_us_total = window.block_us_total + block_us;
    window.block_us_max = larger_of(window.block_us_max, block_us);
    if (window.grains == WINDOW_GRAINS) {
        log_window_if_any();
    }
    return 1;
}

static void mark_playback_stopped(const char *reason)
{
    run_log_write(LOG_FOR_TOOLS_MUSIC_STOPPED, loaded_name, reason, playback.frames_submitted, playback.grains_submitted, playback.crc_total, playback.underruns, playback.late);
    is_playing = 0;
}

static void abandon_playback(void)
{
    log_window_if_any();
    audio_port_close(AUDIO_PORT_MUSIC);
    mark_playback_stopped("submit_failed");
}

static void finish_playback(const char *reason)
{
    memset(grain, 0, sizeof(grain));
    note_window_start();
    if (!submit_grain(0)) {
        abandon_playback();
        return;
    }
    log_window_if_any();
    audio_port_drain(AUDIO_PORT_MUSIC);
    audio_port_close(AUDIO_PORT_MUSIC);
    mark_playback_stopped(reason);
}

static void ramp_in_again_after_a_gap(unsigned long long now_us)
{
    unsigned long long gap_us;

    gap_us = now_us - playback.last_submit_ended_us;
    window.gap_us_max = larger_of(window.gap_us_max, gap_us);
    if (gap_us > GAP_LIMIT_US || playback.last_block_us > BLOCK_LIMIT_US) {
        playback.late = playback.late + 1;
        if (!is_stopping) {
            begin_ramp(0, PCM_RAMP_FULL_GAIN, RAMP_IN_FRAMES);
        }
    }
}

static void play_one_grain(void)
{
    unsigned long long decode_started_us;
    unsigned long long decode_ended_us;

    note_window_start();
    decode_started_us = clock_now_us();
    music_source_fill(&source, grain, MUSIC_GRAIN_FRAMES);
    decode_ended_us = clock_now_us();
    ramp_in_again_after_a_gap(decode_ended_us);
    pcm_ramp_apply(&ramp, grain, MUSIC_GRAIN_FRAMES);
    if (!submit_grain(decode_ended_us - decode_started_us)) {
        abandon_playback();
    } else if (source.has_ended) {
        finish_playback("end");
    } else if (pcm_ramp_has_faded_out(&ramp)) {
        finish_playback("stop");
    }
}

static int open_port(void)
{
    struct audio_port_facts facts;
    int result;

    result = audio_port_open(AUDIO_PORT_MUSIC, MUSIC_RATE, MUSIC_GRAIN_FRAMES);
    if (result < 0) {
        run_log_write("music port_open_failed code=0x%08x", (unsigned int)result);
        return 0;
    }
    audio_port_read_facts(AUDIO_PORT_MUSIC, &facts);
    run_log_write(LOG_FOR_TOOLS_MUSIC_PORT, (unsigned int)result, facts.rate, facts.grain, facts.mode, facts.adopt, facts.system_volume);
    return 1;
}

static void start_playing(int is_looping)
{
    if (!music_source_begin(&source, is_looping)) {
        run_log_write("music decoder_failed name=%s error=rewind", loaded_name);
        return;
    }
    if (!open_port()) {
        return;
    }
    memset(&playback, 0, sizeof(playback));
    memset(&window, 0, sizeof(window));
    playback.started_us = clock_now_us();
    is_playing = 1;
    is_stopping = 0;
    fade_generation = handled_generation;
    run_log_write(LOG_FOR_TOOLS_MUSIC_STARTED, loaded_name, is_looping, logged_loop_start(), logged_loop_last());
    memset(grain, 0, sizeof(grain));
    note_window_start();
    if (!submit_grain(0)) {
        abandon_playback();
        return;
    }
    begin_ramp(0, PCM_RAMP_FULL_GAIN, RAMP_IN_FRAMES);
}

static void act_on_wish(const struct music_wish *wish)
{
    if (track_is_loaded && strcmp(loaded_name, wish->track_name) != 0) {
        release_track();
    }
    if (!track_is_loaded && wish->track_name[0] != '\0') {
        load_track(wish->track_name);
    }
    publish_prepared_name(wish->track_name);
    if (wish->should_play && track_is_loaded) {
        start_playing(wish->is_looping);
    }
}

static void log_thread_facts_once(void)
{
    struct audio_thread_facts facts;

    if (thread_line_is_written) {
        return;
    }
    thread_line_is_written = 1;
    audio_thread_read_facts(&facts);
    run_log_write(LOG_FOR_TOOLS_MUSIC_THREAD, facts.priority, facts.cpu_mask, facts.cpu, facts.arm_mhz, facts.fpscr);
}

int music_engine_step(void)
{
    struct music_wish wish;

    log_thread_facts_once();
    read_mailbox(&wish);
    if (is_playing) {
        if (wish.generation != fade_generation) {
            fade_generation = wish.generation;
            is_stopping = 1;
            begin_ramp(ramp.gain, 0, (unsigned int)wish.fade_ticks * FRAMES_PER_TICK);
        }
        if (wish.starve_ms > 0) {
            audio_thread_sleep_us((unsigned int)wish.starve_ms * 1000);
        }
        play_one_grain();
        return 1;
    }
    if (wish.should_shut_down) {
        release_track();
        return 0;
    }
    if (wish.generation != handled_generation) {
        handled_generation = wish.generation;
        act_on_wish(&wish);
        return 1;
    }
    audio_thread_sleep_us(IDLE_SLEEP_US);
    return 1;
}
