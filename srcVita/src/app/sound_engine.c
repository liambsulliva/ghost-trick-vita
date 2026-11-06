#include <stdio.h>
#include <string.h>
#include "asset_name.h"
#include "audio_port.h"
#include "audio_thread.h"
#include "clock.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "pcm_mix.h"
#include "pcm_ramp.h"
#include "run_log.h"
#include "sound_engine.h"
#include "vorbis_stream.h"

#define SAMPLES_PER_FRAME 2
#define BYTES_PER_FRAME (SAMPLES_PER_FRAME * (unsigned int)sizeof(short))
#define GRAIN_US 11610LL
#define LATE_SLACK_US 2000LL
#define WINDOW_GRAINS 88
#define IDLE_SLEEP_US 5000
#define QUIT_WAIT_US 500000
#define LOG_NO_FRAMES "sound decoder_failed name=%s error=no_frames"
#define LOG_FOR_TOOLS_SOUND_THREAD "sound thread priority=%d mask=0x%08x cpu=%d arm_mhz=%d fpscr=0x%08x"
#define LOG_FOR_TOOLS_SOUND_PORT "sound port result=0x%08x rate=%d grain=%d mode=%d adopt=%d"
#define LOG_FOR_TOOLS_SOUND_LOADED "sound loaded name=%s bytes=%u frames=%u channels=%u rate=%u read_us=%llu decode_us=%llu"
#define LOG_FOR_TOOLS_SOUND_STARTED "sound started name=%s voice=%d at=%llu waited_us=%llu"
#define LOG_FOR_TOOLS_SOUND_DROPPED "sound dropped name=%s reason=%s"
#define LOG_FOR_TOOLS_SOUND_WINDOW "sound window index=%u at=%llu grains=%u wall_us=%llu late=%u voices_max=%d started=%u dropped=%u mix_us_max=%llu block_us_max=%llu gap_us_max=%llu period_us_max=%llu"
#define LOG_FOR_TOOLS_SOUND_STOPPED "sound stopped reason=%s submitted=%llu grains=%u late=%u"

struct sound_start {
    char name[ASSET_NAME_MAX];
    char loop_tag[ASSET_NAME_MAX];
    unsigned long long requested_us;
};

struct sound_mailbox {
    char clip_names[SOUND_CLIP_MAX][ASSET_NAME_MAX];
    int clips_asked;
    int clips_handled;
    struct sound_start starts[SOUND_START_QUEUE_MAX];
    int start_count;
    char loop_stops[SOUND_START_QUEUE_MAX][ASSET_NAME_MAX];
    int loop_stop_count;
    int stop_was_asked;
    int port_is_wanted;
    int should_shut_down;
};

struct sound_requests {
    struct sound_start starts[SOUND_START_QUEUE_MAX];
    int start_count;
    char loop_stops[SOUND_START_QUEUE_MAX][ASSET_NAME_MAX];
    int loop_stop_count;
    char clip_to_load[ASSET_NAME_MAX];
    int has_clip_to_load;
    int stop_was_asked;
    int port_is_wanted;
    int should_shut_down;
};

struct sound_clip {
    char name[ASSET_NAME_MAX];
    short *frames;
    unsigned int frame_count;
};

struct sound_voice {
    const struct sound_clip *clip;
    unsigned int next_frame;
    char loop_tag[ASSET_NAME_MAX];
};

struct sound_totals {
    unsigned long long frames_submitted;
    unsigned long long started_us;
    unsigned long long last_submit_ended_us;
    long long last_stretch_us;
    unsigned int grains_submitted;
    unsigned int late;
};

struct window_totals {
    unsigned int index;
    unsigned long long first_frame;
    unsigned int grains;
    int voices_max;
    unsigned int started;
    unsigned int dropped;
    unsigned long long mix_us_max;
    unsigned long long block_us_max;
    unsigned long long gap_us_max;
    unsigned long long period_us_max;
};

static char folder[FILE_PATH_MAX];
static int worker_is_running;

static struct sound_mailbox mailbox;

static struct vorbis_stream stream;
static struct sound_clip clips[SOUND_CLIP_MAX];
static int clip_count;
static struct sound_voice voices[SOUND_VOICE_MAX];
static struct sound_totals totals;
static struct window_totals window;
static short grain[SOUND_GRAIN_FRAMES * SAMPLES_PER_FRAME] __attribute__((aligned(AUDIO_PORT_BUFFER_ALIGNMENT)));
static int port_is_open;
static int port_has_failed;
static int thread_line_is_written;

void sound_engine_init(const char *sound_folder)
{
    int result;

    snprintf(folder, sizeof(folder), "%s", sound_folder);
    memset(&mailbox, 0, sizeof(mailbox));
    memset(voices, 0, sizeof(voices));
    clip_count = 0;
    port_is_open = 0;
    port_has_failed = 0;
    thread_line_is_written = 0;
    result = audio_thread_start(AUDIO_THREAD_SOUND, sound_engine_step);
    worker_is_running = result >= 0;
    if (!worker_is_running) {
        run_log_write("sound thread_failed code=0x%08x", (unsigned int)result);
    }
}

void sound_request_prepare(const char *name)
{
    int index;
    int is_known;
    int is_full;

    is_known = 0;
    audio_mailbox_lock();
    for (index = 0; index < mailbox.clips_asked; index++) {
        if (strcmp(mailbox.clip_names[index], name) == 0) {
            is_known = 1;
        }
    }
    is_full = !is_known && mailbox.clips_asked >= SOUND_CLIP_MAX;
    if (!is_known && !is_full) {
        snprintf(mailbox.clip_names[mailbox.clips_asked], ASSET_NAME_MAX, "%s", name);
        mailbox.clips_asked = mailbox.clips_asked + 1;
    }
    mailbox.port_is_wanted = 1;
    audio_mailbox_unlock();
    if (is_full) {
        run_log_write("sound too_many_clips name=%s", name);
    }
}

int sound_prepare_is_done(void)
{
    int is_done;

    if (!worker_is_running) {
        return 1;
    }
    audio_mailbox_lock();
    is_done = mailbox.clips_handled >= mailbox.clips_asked;
    audio_mailbox_unlock();
    return is_done;
}

static void request_start(const char *name, const char *loop_tag)
{
    int is_full;

    audio_mailbox_lock();
    is_full = mailbox.start_count >= SOUND_START_QUEUE_MAX;
    if (!is_full) {
        snprintf(mailbox.starts[mailbox.start_count].name, ASSET_NAME_MAX, "%s", name);
        snprintf(mailbox.starts[mailbox.start_count].loop_tag, ASSET_NAME_MAX, "%s", loop_tag);
        mailbox.starts[mailbox.start_count].requested_us = clock_now_us();
        mailbox.start_count = mailbox.start_count + 1;
    }
    mailbox.port_is_wanted = 1;
    audio_mailbox_unlock();
    if (is_full) {
        run_log_write(LOG_FOR_TOOLS_SOUND_DROPPED, name, "queue_full");
    }
}

void sound_request_play(const char *name)
{
    request_start(name, "");
}

void sound_request_loop(const char *name, const char *loop_tag)
{
    request_start(name, loop_tag);
}

void sound_request_stop_loop(const char *loop_tag)
{
    audio_mailbox_lock();
    if (mailbox.loop_stop_count < SOUND_START_QUEUE_MAX) {
        snprintf(mailbox.loop_stops[mailbox.loop_stop_count], ASSET_NAME_MAX, "%s", loop_tag);
        mailbox.loop_stop_count = mailbox.loop_stop_count + 1;
    }
    audio_mailbox_unlock();
}

void sound_request_stop(void)
{
    audio_mailbox_lock();
    mailbox.stop_was_asked = 1;
    mailbox.port_is_wanted = 0;
    mailbox.start_count = 0;
    audio_mailbox_unlock();
}

void sound_shutdown(void)
{
    if (!worker_is_running) {
        return;
    }
    audio_mailbox_lock();
    mailbox.should_shut_down = 1;
    audio_mailbox_unlock();
    audio_thread_wait_for_end(AUDIO_THREAD_SOUND, QUIT_WAIT_US);
    worker_is_running = 0;
}

static void take_requests(struct sound_requests *requests)
{
    audio_mailbox_lock();
    requests->stop_was_asked = mailbox.stop_was_asked;
    mailbox.stop_was_asked = 0;
    requests->port_is_wanted = mailbox.port_is_wanted;
    requests->should_shut_down = mailbox.should_shut_down;
    requests->has_clip_to_load = mailbox.clips_handled < mailbox.clips_asked;
    if (requests->has_clip_to_load) {
        memcpy(requests->clip_to_load, mailbox.clip_names[mailbox.clips_handled], ASSET_NAME_MAX);
    }
    requests->loop_stop_count = mailbox.loop_stop_count;
    memcpy(requests->loop_stops, mailbox.loop_stops, (size_t)mailbox.loop_stop_count * sizeof(mailbox.loop_stops[0]));
    mailbox.loop_stop_count = 0;
    requests->start_count = 0;
    if (port_is_open || port_has_failed) {
        requests->start_count = mailbox.start_count;
        memcpy(requests->starts, mailbox.starts, (size_t)mailbox.start_count * sizeof(mailbox.starts[0]));
        mailbox.start_count = 0;
    }
    audio_mailbox_unlock();
}

static void note_clip_is_handled(void)
{
    audio_mailbox_lock();
    mailbox.clips_handled = mailbox.clips_handled + 1;
    audio_mailbox_unlock();
}

static int read_clip_file(const char *name, unsigned char **data, unsigned int *size)
{
    char path[FILE_PATH_MAX];
    int result;

    if (!file_path_join(path, folder, name, ".ogg")) {
        run_log_write("sound path_too_long name=%s", name);
        return 0;
    }
    result = file_read_whole(path, data, size);
    if (result == FILE_ERR_MEMORY) {
        run_log_write("sound out_of_memory name=%s", name);
    } else if (result != FILE_OK) {
        run_log_write("sound file_missing name=%s code=%d", name, result);
    }
    return result == FILE_OK;
}

static unsigned int decode_stream_into(short *frames, unsigned int frame_limit)
{
    unsigned int decoded;
    unsigned int got;

    decoded = 0;
    while (decoded < frame_limit) {
        got = vorbis_stream_read(&stream, &frames[decoded * SAMPLES_PER_FRAME], frame_limit - decoded);
        if (got == 0) {
            break;
        }
        decoded = decoded + got;
    }
    return decoded;
}

static void decode_clip(const char *name, unsigned int size, unsigned long long read_us)
{
    struct sound_clip *clip;
    unsigned long long decode_started_us;

    decode_started_us = clock_now_us();
    if (stream.sample_rate != SOUND_RATE) {
        run_log_write("sound wrong_rate name=%s rate=%u", name, stream.sample_rate);
        return;
    }
    if (stream.frame_count == 0) {
        run_log_write(LOG_NO_FRAMES, name);
        return;
    }
    clip = &clips[clip_count];
    clip->frames = mem_alloc(stream.frame_count * BYTES_PER_FRAME);
    if (clip->frames == NULL) {
        run_log_write("sound out_of_memory name=%s", name);
        return;
    }
    clip->frame_count = decode_stream_into(clip->frames, stream.frame_count);
    if (clip->frame_count == 0) {
        run_log_write(LOG_NO_FRAMES, name);
        mem_free(clip->frames);
        return;
    }
    snprintf(clip->name, sizeof(clip->name), "%s", name);
    clip_count = clip_count + 1;
    run_log_write(LOG_FOR_TOOLS_SOUND_LOADED, name, size, clip->frame_count, stream.channel_count, stream.sample_rate, read_us, clock_now_us() - decode_started_us);
}

static void load_clip(const char *name)
{
    unsigned long long started_us;
    unsigned long long read_us;
    unsigned char *data;
    unsigned int size;
    int error;

    started_us = clock_now_us();
    if (!read_clip_file(name, &data, &size)) {
        return;
    }
    read_us = clock_now_us() - started_us;
    error = vorbis_stream_open(&stream, data, size);
    if (error != VORBIS_STREAM_OK) {
        run_log_write("sound decoder_failed name=%s error=%d", name, error);
    } else {
        decode_clip(name, size, read_us);
        vorbis_stream_close(&stream);
    }
    mem_free(data);
}

static const struct sound_clip *find_clip(const char *name)
{
    int index;

    for (index = 0; index < clip_count; index++) {
        if (strcmp(clips[index].name, name) == 0) {
            return &clips[index];
        }
    }
    return NULL;
}

static void drop_sound(const char *name, const char *reason)
{
    window.dropped = window.dropped + 1;
    run_log_write(LOG_FOR_TOOLS_SOUND_DROPPED, name, reason);
}

static int find_free_voice(void)
{
    int index;

    for (index = 0; index < SOUND_VOICE_MAX; index++) {
        if (voices[index].clip == NULL) {
            return index;
        }
    }
    return -1;
}

static void start_voice(const struct sound_start *start)
{
    const struct sound_clip *clip;
    int voice;

    clip = find_clip(start->name);
    if (clip == NULL) {
        drop_sound(start->name, "not_prepared");
        return;
    }
    voice = find_free_voice();
    if (voice < 0) {
        drop_sound(start->name, "all_voices_busy");
        return;
    }
    voices[voice].clip = clip;
    voices[voice].next_frame = 0;
    memcpy(voices[voice].loop_tag, start->loop_tag, ASSET_NAME_MAX);
    window.started = window.started + 1;
    run_log_write(LOG_FOR_TOOLS_SOUND_STARTED, start->name, voice, totals.frames_submitted, clock_now_us() - start->requested_us);
}

static int count_busy_voices(void)
{
    int index;
    int busy;

    busy = 0;
    for (index = 0; index < SOUND_VOICE_MAX; index++) {
        if (voices[index].clip != NULL) {
            busy = busy + 1;
        }
    }
    return busy;
}

static void mix_voice_into_grain(struct sound_voice *voice)
{
    unsigned int frames_mixed;

    frames_mixed = 0;
    while (voice->clip != NULL && frames_mixed < SOUND_GRAIN_FRAMES) {
        unsigned int frames_left;
        unsigned int frames;

        frames_left = voice->clip->frame_count - voice->next_frame;
        frames = frames_left < SOUND_GRAIN_FRAMES - frames_mixed ? frames_left : SOUND_GRAIN_FRAMES - frames_mixed;
        pcm_mix_add(&grain[frames_mixed * SAMPLES_PER_FRAME], &voice->clip->frames[voice->next_frame * SAMPLES_PER_FRAME], frames * SAMPLES_PER_FRAME);
        voice->next_frame = voice->next_frame + frames;
        frames_mixed = frames_mixed + frames;
        if (voice->next_frame >= voice->clip->frame_count) {
            if (voice->loop_tag[0] == '\0') {
                voice->clip = NULL;
            } else {
                voice->next_frame = 0;
            }
        }
    }
}

static void stop_loops(const struct sound_requests *requests)
{
    int stop;
    int index;

    for (stop = 0; stop < requests->loop_stop_count; stop++) {
        for (index = 0; index < SOUND_VOICE_MAX; index++) {
            if (voices[index].clip != NULL && voices[index].loop_tag[0] != '\0' && (requests->loop_stops[stop][0] == '*' || strcmp(voices[index].loop_tag, requests->loop_stops[stop]) == 0)) {
                voices[index].clip = NULL;
            }
        }
    }
}

static void mix_grain(void)
{
    int index;

    memset(grain, 0, sizeof(grain));
    for (index = 0; index < SOUND_VOICE_MAX; index++) {
        if (voices[index].clip != NULL) {
            mix_voice_into_grain(&voices[index]);
        }
    }
}

static unsigned long long larger_of(unsigned long long first, unsigned long long second)
{
    return first > second ? first : second;
}

static void log_window_if_any(void)
{
    unsigned int next_index;

    if (window.grains == 0) {
        return;
    }
    run_log_write(LOG_FOR_TOOLS_SOUND_WINDOW, window.index, window.first_frame, window.grains, clock_now_us() - totals.started_us, totals.late, window.voices_max, window.started, window.dropped, window.mix_us_max, window.block_us_max, window.gap_us_max, window.period_us_max);
    next_index = window.index + 1;
    memset(&window, 0, sizeof(window));
    window.index = next_index;
}

static void note_period_since(unsigned long long previous_return_us)
{
    unsigned long long period_us;
    long long stretch_us;

    period_us = totals.last_submit_ended_us - previous_return_us;
    window.period_us_max = larger_of(window.period_us_max, period_us);
    stretch_us = (long long)period_us - GRAIN_US;
    if (totals.last_stretch_us > LATE_SLACK_US && totals.last_stretch_us + stretch_us > LATE_SLACK_US) {
        totals.late = totals.late + 1;
    }
    totals.last_stretch_us = stretch_us;
}

static int submit_grain(unsigned long long mix_us)
{
    unsigned long long submit_started_us;
    unsigned long long previous_return_us;
    unsigned long long block_us;
    int result;

    if (window.grains == 0) {
        window.first_frame = totals.frames_submitted;
    }
    previous_return_us = totals.last_submit_ended_us;
    submit_started_us = clock_now_us();
    window.gap_us_max = larger_of(window.gap_us_max, submit_started_us - previous_return_us);
    result = audio_port_submit(AUDIO_PORT_SOUND, grain);
    totals.last_submit_ended_us = clock_now_us();
    if (result < 0) {
        run_log_write("sound submit_failed code=0x%08x", (unsigned int)result);
        return 0;
    }
    block_us = totals.last_submit_ended_us - submit_started_us;
    if (totals.grains_submitted > 0) {
        note_period_since(previous_return_us);
    }
    totals.frames_submitted = totals.frames_submitted + SOUND_GRAIN_FRAMES;
    totals.grains_submitted = totals.grains_submitted + 1;
    window.grains = window.grains + 1;
    window.mix_us_max = larger_of(window.mix_us_max, mix_us);
    window.block_us_max = larger_of(window.block_us_max, block_us);
    if (window.grains == WINDOW_GRAINS) {
        log_window_if_any();
    }
    return 1;
}

static void mark_port_closed(const char *reason)
{
    log_window_if_any();
    run_log_write(LOG_FOR_TOOLS_SOUND_STOPPED, reason, totals.frames_submitted, totals.grains_submitted, totals.late);
    memset(voices, 0, sizeof(voices));
    port_is_open = 0;
}

static void give_up_on_port(void)
{
    audio_port_close(AUDIO_PORT_SOUND);
    port_has_failed = 1;
    mark_port_closed("submit_failed");
}

static void close_port_cleanly(void)
{
    struct pcm_ramp fade;

    mix_grain();
    pcm_ramp_begin(&fade, PCM_RAMP_FULL_GAIN, 0, SOUND_GRAIN_FRAMES);
    pcm_ramp_apply(&fade, grain, SOUND_GRAIN_FRAMES);
    if (!submit_grain(0)) {
        give_up_on_port();
        return;
    }
    memset(grain, 0, sizeof(grain));
    if (!submit_grain(0)) {
        give_up_on_port();
        return;
    }
    log_window_if_any();
    audio_port_drain(AUDIO_PORT_SOUND);
    audio_port_close(AUDIO_PORT_SOUND);
    mark_port_closed("stop");
}

static void open_port(void)
{
    struct audio_port_facts facts;
    int result;

    result = audio_port_open(AUDIO_PORT_SOUND, SOUND_RATE, SOUND_GRAIN_FRAMES);
    if (result < 0) {
        run_log_write("sound port_open_failed code=0x%08x", (unsigned int)result);
        port_has_failed = 1;
        return;
    }
    audio_port_read_facts(AUDIO_PORT_SOUND, &facts);
    run_log_write(LOG_FOR_TOOLS_SOUND_PORT, (unsigned int)result, facts.rate, facts.grain, facts.mode, facts.adopt);
    memset(&totals, 0, sizeof(totals));
    memset(&window, 0, sizeof(window));
    totals.started_us = clock_now_us();
    totals.last_submit_ended_us = totals.started_us;
    port_is_open = 1;
    memset(grain, 0, sizeof(grain));
    if (!submit_grain(0)) {
        give_up_on_port();
    }
}

static void play_one_grain(const struct sound_requests *requests)
{
    unsigned long long mix_started_us;
    int busy;
    int index;

    stop_loops(requests);
    for (index = 0; index < requests->start_count; index++) {
        start_voice(&requests->starts[index]);
    }
    busy = count_busy_voices();
    if (busy > window.voices_max) {
        window.voices_max = busy;
    }
    mix_started_us = clock_now_us();
    mix_grain();
    if (!submit_grain(clock_now_us() - mix_started_us)) {
        give_up_on_port();
    }
}

static void free_clips(void)
{
    int index;

    for (index = 0; index < clip_count; index++) {
        mem_free(clips[index].frames);
    }
    clip_count = 0;
}

static void log_thread_facts_once(void)
{
    struct audio_thread_facts facts;

    if (thread_line_is_written) {
        return;
    }
    thread_line_is_written = 1;
    audio_thread_read_facts(&facts);
    run_log_write(LOG_FOR_TOOLS_SOUND_THREAD, facts.priority, facts.cpu_mask, facts.cpu, facts.arm_mhz, facts.fpscr);
}

int sound_engine_step(void)
{
    struct sound_requests requests;
    int index;

    log_thread_facts_once();
    take_requests(&requests);
    if (port_is_open) {
        if (requests.stop_was_asked || requests.should_shut_down || requests.has_clip_to_load) {
            close_port_cleanly();
        } else {
            play_one_grain(&requests);
        }
        return 1;
    }
    for (index = 0; index < requests.start_count; index++) {
        run_log_write(LOG_FOR_TOOLS_SOUND_DROPPED, requests.starts[index].name, "no_port");
    }
    if (requests.stop_was_asked) {
        port_has_failed = 0;
    }
    if (requests.should_shut_down) {
        free_clips();
        return 0;
    }
    if (requests.has_clip_to_load) {
        load_clip(requests.clip_to_load);
        note_clip_is_handled();
        return 1;
    }
    if (requests.port_is_wanted && !port_has_failed) {
        open_port();
        return 1;
    }
    audio_thread_sleep_us(IDLE_SLEEP_US);
    return 1;
}
