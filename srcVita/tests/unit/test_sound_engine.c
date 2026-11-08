#include <stdio.h>
#include "audio_port.h"
#include "audio_thread.h"
#include "clock.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "file.h"
#include "fixtures/fixture_facts.h"
#include "fixtures/ogg_edit.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "pcm_mix.h"
#include "pcm_ramp.h"
#include "run_log.h"
#include "playback_speed.h"
#include "sound_engine.h"
#include "test_support.h"
#include "vorbis_stream.h"

#define GRAIN_SAMPLES (SOUND_GRAIN_FRAMES * 2)
#define WINDOW_GRAINS 88
#define BUSY_PORT_CODE ((int)0x80260002)
#define GRAINS_LONGER_THAN_THE_CLIP (SINE_MONO_FRAMES / SOUND_GRAIN_FRAMES + 2)
#define LARGEST_BLOCK_THAT_HOLDS_THE_FILE_BUT_NOT_THE_FRAMES 8192
#define GRAIN_US_AS_THE_FAKE_ADVANCES 11609
#define LATE_SLACK_US 2000
#define LATE_PERIOD_US (11610 + LATE_SLACK_US)
#define FIRST_BLOCK_US FAKE_AUDIO_SOUND_SHORTEST_BLOCK_US
#define DEADLINE_AFTER_A_RETURN_US 11499
#define STRETCH_MOST_PORT_OPENS_SHOW_US 7260
#define LATE_RETURN_WITH_NO_GAP_US 9500
#define LATE_RETURN_PAST_THE_DEADLINE_US 14000
#define SLIP_ONE_PORT_OPEN_SHOWED_US 21500
#define SHORT_REAL_GAP_US 3000

static short clip_frames[SINE_MONO_FRAMES * 2];

static void decode_reference_clip(void)
{
    static struct vorbis_stream stream;
    unsigned int done;
    unsigned int got;

    CHECK_INT(vorbis_stream_open(&stream, sine_mono_ogg, sizeof(sine_mono_ogg)), VORBIS_STREAM_OK);
    done = 0;
    do {
        got = vorbis_stream_read(&stream, &clip_frames[done * 2], SINE_MONO_FRAMES - done);
        done = done + got;
    } while (got > 0 && done < SINE_MONO_FRAMES);
    vorbis_stream_close(&stream);
}

static void set_up(void)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    fake_clock_set_us(1000);
    fake_clock_set_tick_per_read_us(0);
    run_log_open("run.log");
    sound_engine_init("sounds");
}

static void add_clip(const char *name)
{
    char path[FILE_PATH_MAX];

    snprintf(path, sizeof(path), "sounds/%s.ogg", name);
    fake_files_add(path, sine_mono_ogg, sizeof(sine_mono_ogg));
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static int count_in_log(const char *wanted)
{
    const char *found;
    int count;

    count = 0;
    found = strstr(log_text(), wanted);
    while (found != NULL) {
        count = count + 1;
        found = strstr(found + 1, wanted);
    }
    return count;
}

static void pump(int steps)
{
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, steps);
}

static int grains_played(void)
{
    return fake_audio_port_grain_count(AUDIO_PORT_SOUND);
}

static const short *played_grain(int index)
{
    return fake_audio_port_grain(AUDIO_PORT_SOUND, index);
}

static int grain_is_silent(const short *grain)
{
    int index;

    for (index = 0; index < GRAIN_SAMPLES; index++) {
        if (grain[index] != 0) {
            return 0;
        }
    }
    return 1;
}

static void prepare_one_clip_and_open_the_port(const char *name)
{
    add_clip(name);
    sound_request_prepare(name);
    pump(2);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
}

static void finish(void)
{
    sound_shutdown();
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK(!fake_audio_thread_is_alive(AUDIO_THREAD_SOUND));
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_prepare_decodes_the_whole_clip_then_opens_the_port_with_a_zero_grain(void)
{
    set_up();
    add_clip("ping");
    sound_request_prepare("ping");
    CHECK(!sound_prepare_is_done());
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_SOUND), 0);
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound loaded name=ping bytes=");
    CHECK_HAS_TEXT(log_text(), "frames=11025 channels=1 rate=44100 read_us=");
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    pump(1);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_INT(fake_audio_port_rate(AUDIO_PORT_SOUND), SOUND_RATE);
    CHECK_INT(fake_audio_port_grain_frames(AUDIO_PORT_SOUND), SOUND_GRAIN_FRAMES);
    CHECK_HAS_TEXT(log_text(), "sound port result=0x00000001 rate=44100 grain=512 mode=1 adopt=1");
    CHECK_INT(count_in_log("sound thread priority=64 mask=0x00040000 cpu=2 arm_mhz=333 fpscr=0x00000000"), 1);
    CHECK_INT(grains_played(), 1);
    CHECK(grain_is_silent(played_grain(0)));
    finish();
}

static void test_a_started_sound_plays_in_the_next_grain_on_both_channels_and_then_ends(void)
{
    const short *grain;
    int first;
    int index;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(3);
    first = grains_played();
    sound_request_play("ping");
    pump(GRAINS_LONGER_THAN_THE_CLIP);
    for (index = 0; index < SINE_MONO_FRAMES / SOUND_GRAIN_FRAMES; index++) {
        grain = played_grain(first + index);
        CHECK(memcmp(grain, &clip_frames[index * SOUND_GRAIN_FRAMES * 2], GRAIN_SAMPLES * sizeof(short)) == 0);
        CHECK_INT(grain[100], grain[101]);
    }
    CHECK(grain_is_silent(played_grain(first + SINE_MONO_FRAMES / SOUND_GRAIN_FRAMES + 1)));
    CHECK_HAS_TEXT(log_text(), "sound started name=ping voice=0 at=2048 waited_us=0");
    finish();
}

static void test_a_sped_up_sound_is_cut_to_its_share_of_the_clip(void)
{
    int first;
    int index;
    int full_grains;
    int frames_in_the_last_grain;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(3);
    first = grains_played();
    playback_speed_set(2 * PLAYBACK_SPEED_NORMAL);
    sound_request_play("ping");
    pump(GRAINS_LONGER_THAN_THE_CLIP);
    playback_speed_set(PLAYBACK_SPEED_NORMAL);
    full_grains = (SINE_MONO_FRAMES / 2) / SOUND_GRAIN_FRAMES;
    frames_in_the_last_grain = (SINE_MONO_FRAMES / 2) % SOUND_GRAIN_FRAMES;
    for (index = 0; index < full_grains; index++) {
        CHECK(memcmp(played_grain(first + index), &clip_frames[index * SOUND_GRAIN_FRAMES * 2], GRAIN_SAMPLES * sizeof(short)) == 0);
    }
    CHECK(memcmp(played_grain(first + full_grains), &clip_frames[full_grains * SOUND_GRAIN_FRAMES * 2], (unsigned int)frames_in_the_last_grain * 2 * sizeof(short)) == 0);
    CHECK(grain_is_silent(played_grain(first + full_grains + 1)));
    finish();
}

static void test_the_wait_before_a_sound_starts_is_logged(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_play("ping");
    fake_clock_advance_us(5000);
    pump(1);
    CHECK_HAS_TEXT(log_text(), "waited_us=5000");
    finish();
}

static void test_two_sounds_at_once_add_together(void)
{
    static short expected[GRAIN_SAMPLES];
    int first;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    first = grains_played();
    sound_request_play("ping");
    pump(1);
    sound_request_play("ping");
    pump(1);
    memcpy(expected, &clip_frames[SOUND_GRAIN_FRAMES * 2], sizeof(expected));
    pcm_mix_add(expected, clip_frames, GRAIN_SAMPLES);
    CHECK(memcmp(played_grain(first + 1), expected, sizeof(expected)) == 0);
    CHECK_HAS_TEXT(log_text(), "voice=1 at=");
    finish();
}

static void test_a_loop_keeps_playing_until_it_is_stopped_by_its_tag(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_loop("ping", "se_rain");
    pump(WINDOW_GRAINS);
    CHECK_INT(count_in_log("sound started name=ping"), 1);
    CHECK_HAS_TEXT(log_text(), "voices_max=1 started=1 dropped=0 ");
    sound_request_stop_loop("se_other");
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "voices_max=1 started=0 dropped=0 ");
    sound_request_stop_loop("se_rain");
    pump(WINDOW_GRAINS * 2);
    CHECK_HAS_TEXT(log_text(), "voices_max=0 started=0 dropped=0 ");
    finish();
}

static void test_one_sound_past_the_voice_limit_is_dropped_and_logged(void)
{
    char expected[64];
    int index;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    for (index = 0; index < SOUND_VOICE_MAX + 1; index++) {
        sound_request_play("ping");
    }
    pump(1);
    CHECK_INT(count_in_log("sound started name=ping"), SOUND_VOICE_MAX);
    CHECK_HAS_TEXT(log_text(), "voice=3 ");
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=all_voices_busy");
    pump(WINDOW_GRAINS);
    snprintf(expected, sizeof(expected), "voices_max=%d started=%d dropped=1 ", SOUND_VOICE_MAX, SOUND_VOICE_MAX);
    CHECK_HAS_TEXT(log_text(), expected);
    finish();
}

static void test_a_finished_voice_is_free_again_and_plays_from_the_top(void)
{
    int first_after_replay;
    int index;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    for (index = 0; index < SOUND_VOICE_MAX; index++) {
        sound_request_play("ping");
    }
    pump(GRAINS_LONGER_THAN_THE_CLIP);
    first_after_replay = grains_played();
    sound_request_play("ping");
    pump(1);
    CHECK_INT(count_in_log("sound started name=ping"), SOUND_VOICE_MAX + 1);
    CHECK(strstr(log_text(), "sound dropped") == NULL);
    CHECK(memcmp(played_grain(first_after_replay), clip_frames, GRAIN_SAMPLES * sizeof(short)) == 0);
    finish();
}

static void test_a_voice_that_played_a_long_clip_plays_a_shorter_one_from_its_start(void)
{
    int first_after_replay;

    set_up();
    fake_files_add("sounds/long.ogg", sine_stereo_ogg, sizeof(sine_stereo_ogg));
    add_clip("short");
    sound_request_prepare("long");
    sound_request_prepare("short");
    pump(3);
    sound_request_play("long");
    pump(SINE_STEREO_FRAMES / SOUND_GRAIN_FRAMES + 2);
    first_after_replay = grains_played();
    sound_request_play("short");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound started name=short voice=0");
    CHECK(memcmp(played_grain(first_after_replay), clip_frames, GRAIN_SAMPLES * sizeof(short)) == 0);
    finish();
}

static void test_more_starts_than_the_queue_holds_are_dropped_and_logged(void)
{
    int index;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    for (index = 0; index < SOUND_START_QUEUE_MAX + 1; index++) {
        sound_request_play("ping");
    }
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=queue_full");
    finish();
}

static void test_a_clip_that_was_not_prepared_is_dropped_and_logged(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_play("other");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=other reason=not_loaded");
    finish();
}

static void test_a_missing_file_reports_done_and_the_others_still_load(void)
{
    set_up();
    add_clip("ping");
    sound_request_prepare("gone");
    sound_request_prepare("ping");
    pump(1);
    CHECK(!sound_prepare_is_done());
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound file_missing name=gone code=-2");
    CHECK_HAS_TEXT(log_text(), "sound loaded name=ping");
    pump(1);
    sound_request_play("gone");
    sound_request_play("ping");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=gone reason=not_loaded");
    CHECK_HAS_TEXT(log_text(), "sound started name=ping");
    finish();
}

static void test_a_clip_asked_for_twice_loads_once(void)
{
    set_up();
    add_clip("ping");
    sound_request_prepare("ping");
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    pump(3);
    sound_request_stop();
    sound_request_prepare("ping");
    CHECK(sound_prepare_is_done());
    pump(3);
    CHECK_INT(count_in_log("sound loaded"), 1);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_SOUND), 2);
    finish();
}

static void test_a_full_prepare_list_is_logged_once_and_never_holds_up_the_preload(void)
{
    char name[32];
    int index;

    set_up();
    for (index = 0; index < SOUND_CLIP_MAX + 3; index++) {
        snprintf(name, sizeof(name), "clip_%d", index);
        add_clip(name);
        sound_request_prepare(name);
    }
    snprintf(name, sizeof(name), "first_left_out=clip_%d", SOUND_CLIP_MAX);
    CHECK_HAS_TEXT(log_text(), name);
    CHECK_INT(count_in_log("sound prepare_list_full"), 1);
    pump(SOUND_CLIP_MAX);
    CHECK(sound_prepare_is_done());
    CHECK_INT(count_in_log("sound loaded"), SOUND_CLIP_MAX);
    finish();
}

static void test_running_out_of_memory_logs_and_stays_silent(void)
{
    set_up();
    add_clip("ping");
    fake_memory_set_largest_block(LARGEST_BLOCK_THAT_HOLDS_THE_FILE_BUT_NOT_THE_FRAMES);
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound out_of_memory name=ping");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    pump(1);
    sound_request_play("ping");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=not_loaded");
    finish();

    set_up();
    add_clip("ping");
    fake_memory_set_largest_block(16);
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound out_of_memory name=ping");
    finish();
}

static void test_a_clip_at_another_rate_is_refused(void)
{
    static unsigned char other_rate[sizeof(sine_mono_ogg)];

    set_up();
    memcpy(other_rate, sine_mono_ogg, sizeof(sine_mono_ogg));
    other_rate[SINE_RATE_OFFSET_IN_OGG] = 0x22;
    other_rate[SINE_RATE_OFFSET_IN_OGG + 1] = 0x56;
    ogg_edit_refresh_crcs(other_rate, sizeof(other_rate));
    fake_files_add("sounds/ping.ogg", other_rate, sizeof(other_rate));
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound wrong_rate name=ping rate=22050");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    finish();
}

static void test_a_file_that_is_not_ogg_is_refused(void)
{
    static const unsigned char not_ogg[64] = "this is not an ogg file";

    set_up();
    fake_files_add("sounds/ping.ogg", not_ogg, sizeof(not_ogg));
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    finish();
}

static void test_a_flipped_bit_in_a_header_is_refused_and_the_other_clips_still_load(void)
{
    static unsigned char flipped[sizeof(sine_mono_ogg)];

    set_up();
    memcpy(flipped, sine_mono_ogg, sizeof(sine_mono_ogg));
    flipped[SINE_MONO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("sounds/flipped.ogg", flipped, sizeof(flipped));
    add_clip("ping");
    sound_request_prepare("flipped");
    sound_request_prepare("ping");
    pump(2);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=flipped error=-1");
    CHECK_HAS_TEXT(log_text(), "sound loaded name=ping");
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    finish();
}

static void test_a_clip_of_only_headers_is_a_decoder_failure_not_a_memory_failure(void)
{
    set_up();
    fake_files_add("sounds/ping.ogg", sine_mono_ogg, SINE_MONO_HEADER_PAGES_BYTES);
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=no_frames");
    CHECK(strstr(log_text(), "sound out_of_memory") == NULL);
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    pump(1);
    sound_request_play("ping");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=not_loaded");
    finish();
}

static void test_a_clip_that_promises_frames_and_decodes_none_is_refused_and_freed(void)
{
    static unsigned char undecodable[sizeof(sine_mono_ogg)];

    set_up();
    memcpy(undecodable, sine_mono_ogg, sizeof(sine_mono_ogg));
    memset(&undecodable[SINE_MONO_AUDIO_BODY_OFFSET], 0xFF, sizeof(undecodable) - SINE_MONO_AUDIO_BODY_OFFSET);
    ogg_edit_refresh_crcs(undecodable, sizeof(undecodable));
    fake_files_add("sounds/ping.ogg", undecodable, sizeof(undecodable));
    sound_request_prepare("ping");
    pump(1);
    CHECK(sound_prepare_is_done());
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=no_frames");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    finish();
}

static void test_each_window_line_counts_its_grains_and_its_largest_period(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(WINDOW_GRAINS - 1);
    CHECK_HAS_TEXT(log_text(), "sound window index=0 at=0 grains=88 wall_us=");
    CHECK_HAS_TEXT(log_text(), "late=0 voices_max=0 started=0 dropped=0 mix_us_max=0");
    CHECK_HAS_TEXT(log_text(), "period_us_max=11609");
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "sound window index=1 at=45056 grains=88 wall_us=");
    finish();
}

static void test_a_submit_inside_the_deadline_is_not_late(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(11000);
    pump(WINDOW_GRAINS - 1);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_HAS_TEXT(log_text(), "period_us_max=11609");
    finish();
}

static void test_a_period_at_the_limit_is_not_late(void)
{
    char wanted[64];

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US);
    pump(WINDOW_GRAINS - 1);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_stretch_the_next_period_brings_back_to_the_slack_is_not_late(void)
{
    char wanted[64];

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US + 1);
    pump(WINDOW_GRAINS - 1);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US + 1);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_stretch_exactly_at_the_slack_starts_no_count(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US);
    pump(1);
    fake_audio_port_add_block_once_us(AUDIO_PORT_SOUND, 3);
    pump(WINDOW_GRAINS - 2);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_period_past_the_limit_is_late_in_the_window_and_stopped_lines(void)
{
    char wanted[64];

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US + 2);
    pump(WINDOW_GRAINS - 1);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US + 2);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=1 voices_max=0 ");
    sound_request_stop();
    pump(1);
    CHECK_HAS_TEXT(log_text(), " late=1\n");
    finish();
}

static void test_the_stretch_most_port_opens_show_is_paid_back_and_is_not_late(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(7);
    fake_audio_port_add_block_once_us(AUDIO_PORT_SOUND, STRETCH_MOST_PORT_OPENS_SHOW_US);
    pump(WINDOW_GRAINS - 8);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_any_late_return_the_next_period_pays_back_is_not_late(void)
{
    char wanted[64];

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(20);
    fake_audio_port_add_block_once_us(AUDIO_PORT_SOUND, LATE_RETURN_WITH_NO_GAP_US);
    pump(WINDOW_GRAINS - 21);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", GRAIN_US_AS_THE_FAKE_ADVANCES + LATE_RETURN_WITH_NO_GAP_US);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_return_too_late_for_the_next_submit_is_late(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_audio_port_add_block_once_us(AUDIO_PORT_SOUND, LATE_RETURN_PAST_THE_DEADLINE_US);
    pump(WINDOW_GRAINS - 1);
    CHECK_HAS_TEXT(log_text(), "late=1 ");
    finish();
}

static void test_the_slip_one_port_open_showed_stays_and_is_late_once(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_clock_advance_us(DEADLINE_AFTER_A_RETURN_US + SLIP_ONE_PORT_OPEN_SHOWED_US);
    pump(WINDOW_GRAINS - 1);
    CHECK_HAS_TEXT(log_text(), "late=1 ");
    finish();
}

static void test_a_short_real_gap_mid_play_is_late(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(20);
    fake_clock_advance_us(DEADLINE_AFTER_A_RETURN_US + SHORT_REAL_GAP_US);
    pump(WINDOW_GRAINS - 21);
    CHECK_HAS_TEXT(log_text(), "late=1 ");
    finish();
}

static void test_the_first_grain_after_idle_has_no_period_to_be_late_on(void)
{
    set_up();
    add_clip("ping");
    sound_request_prepare("ping");
    pump(1);
    fake_audio_port_add_block_once_us(AUDIO_PORT_SOUND, LATE_PERIOD_US);
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "sound window index=0 at=0 grains=88 ");
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_HAS_TEXT(log_text(), "period_us_max=11609");
    finish();
}

static void test_the_last_window_is_timed_before_the_drain(void)
{
    char wanted[64];

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    pump(3);
    sound_request_stop();
    pump(1);
    snprintf(wanted, sizeof(wanted), "grains=6 wall_us=%llu ", FIRST_BLOCK_US + 5 * GRAIN_US_AS_THE_FAKE_ADVANCES);
    CHECK_HAS_TEXT(log_text(), wanted);
    finish();
}

static void test_a_stop_fades_the_voices_sends_a_zero_grain_drains_and_closes(void)
{
    static short expected_fade[GRAIN_SAMPLES];
    struct pcm_ramp fade;
    const short *fading;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_play("ping");
    pump(2);
    fake_audio_port_clear_events(AUDIO_PORT_SOUND);
    sound_request_stop();
    pump(1);
    CHECK_TEXT(fake_audio_port_events(AUDIO_PORT_SOUND), "submit submit drain close ");
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_HAS_TEXT(log_text(), "sound stopped reason=stop ");
    fading = played_grain(grains_played() - 2);
    CHECK_INT(fading[0], clip_frames[SOUND_GRAIN_FRAMES * 2 * 2]);
    CHECK(fading[(SOUND_GRAIN_FRAMES - 1) * 2] < 200 && fading[(SOUND_GRAIN_FRAMES - 1) * 2] > -200);
    memcpy(expected_fade, &clip_frames[SOUND_GRAIN_FRAMES * 2 * 2], sizeof(expected_fade));
    pcm_ramp_begin(&fade, PCM_RAMP_FULL_GAIN, 0, SOUND_GRAIN_FRAMES);
    pcm_ramp_apply(&fade, expected_fade, SOUND_GRAIN_FRAMES);
    CHECK_INT(fading[SOUND_GRAIN_FRAMES], expected_fade[SOUND_GRAIN_FRAMES]);
    CHECK(memcmp(fading, expected_fade, sizeof(expected_fade)) == 0);
    CHECK(grain_is_silent(played_grain(grains_played() - 1)));
    pump(5);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_SOUND), 1);
    finish();
}

static void test_a_stop_forgets_voices_and_starts_still_waiting(void)
{
    int first_after_reopen;

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_play("ping");
    pump(1);
    sound_request_play("ping");
    sound_request_stop();
    pump(1);
    sound_request_prepare("ping");
    pump(1);
    first_after_reopen = grains_played();
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
    pump(1);
    CHECK(grain_is_silent(played_grain(first_after_reopen)));
    CHECK_INT(count_in_log("sound started"), 1);
    finish();
}

static void test_a_stop_while_the_port_is_closed_forgets_starts_still_waiting(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_stop();
    pump(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    sound_request_play("ping");
    sound_request_stop();
    pump(2);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    sound_request_prepare("ping");
    pump(3);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_INT(count_in_log("sound started"), 0);
    finish();
}

static void test_preparing_a_new_clip_while_the_port_is_open_closes_it_first(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    add_clip("other");
    sound_request_prepare("other");
    pump(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK(!sound_prepare_is_done());
    pump(1);
    CHECK(sound_prepare_is_done());
    pump(1);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_SOUND), 2);
    finish();
}

static void test_a_port_that_will_not_open_logs_its_code_and_stays_silent(void)
{
    set_up();
    add_clip("ping");
    fake_audio_port_fail_open(AUDIO_PORT_SOUND, BUSY_PORT_CODE);
    sound_request_prepare("ping");
    pump(10);
    CHECK(sound_prepare_is_done());
    CHECK_INT(count_in_log("sound port_open_failed code=0x80260002"), 1);
    sound_request_play("ping");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=no_port");
    CHECK_INT(grains_played(), 0);
    sound_request_stop();
    sound_request_prepare("ping");
    pump(2);
    CHECK_INT(count_in_log("sound port_open_failed"), 2);
    finish();
}

static void test_submit_failures_close_the_port_and_keep_stepping(void)
{
    set_up();
    add_clip("ping");
    fake_audio_port_fail_submit_after(AUDIO_PORT_SOUND, 0, BUSY_PORT_CODE);
    sound_request_prepare("ping");
    pump(10);
    CHECK_HAS_TEXT(log_text(), "sound submit_failed code=0x80260002");
    CHECK_HAS_TEXT(log_text(), "sound stopped reason=submit_failed");
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    finish();

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_audio_port_fail_submit_after(AUDIO_PORT_SOUND, 2, BUSY_PORT_CODE);
    pump(4);
    CHECK_INT(count_in_log("sound submit_failed"), 1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    finish();

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_audio_port_fail_submit_after(AUDIO_PORT_SOUND, 0, BUSY_PORT_CODE);
    sound_request_stop();
    pump(2);
    CHECK_INT(count_in_log("sound submit_failed"), 1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    finish();

    set_up();
    prepare_one_clip_and_open_the_port("ping");
    fake_audio_port_fail_submit_after(AUDIO_PORT_SOUND, 1, BUSY_PORT_CODE);
    sound_request_stop();
    pump(2);
    CHECK_INT(count_in_log("sound submit_failed"), 1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    finish();
}

static void test_shutdown_while_playing_closes_and_frees(void)
{
    set_up();
    prepare_one_clip_and_open_the_port("ping");
    sound_request_play("ping");
    pump(1);
    finish();
    CHECK_HAS_TEXT(fake_audio_port_events(AUDIO_PORT_SOUND), "submit drain close ");
}

static void test_the_idle_worker_sleeps_instead_of_spinning(void)
{
    set_up();
    fake_clock_set_us(0);
    pump(3);
    CHECK_INT(grains_played(), 0);
    CHECK(clock_now_us() >= 15000);
    finish();
}

static void test_a_worker_that_cannot_start_never_holds_anything_up(void)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    run_log_open("run.log");
    fake_audio_thread_fail_start(AUDIO_THREAD_SOUND, BUSY_PORT_CODE);
    sound_engine_init("sounds");
    CHECK_HAS_TEXT(log_text(), "sound thread_failed code=0x80260002");
    sound_request_prepare("ping");
    CHECK(sound_prepare_is_done());
    sound_shutdown();
}

int main(void)
{
    decode_reference_clip();
    test_prepare_decodes_the_whole_clip_then_opens_the_port_with_a_zero_grain();
    test_a_started_sound_plays_in_the_next_grain_on_both_channels_and_then_ends();
    test_a_sped_up_sound_is_cut_to_its_share_of_the_clip();
    test_the_wait_before_a_sound_starts_is_logged();
    test_two_sounds_at_once_add_together();
    test_one_sound_past_the_voice_limit_is_dropped_and_logged();
    test_a_loop_keeps_playing_until_it_is_stopped_by_its_tag();
    test_a_finished_voice_is_free_again_and_plays_from_the_top();
    test_a_voice_that_played_a_long_clip_plays_a_shorter_one_from_its_start();
    test_more_starts_than_the_queue_holds_are_dropped_and_logged();
    test_a_clip_that_was_not_prepared_is_dropped_and_logged();
    test_a_missing_file_reports_done_and_the_others_still_load();
    test_a_clip_asked_for_twice_loads_once();
    test_a_full_prepare_list_is_logged_once_and_never_holds_up_the_preload();
    test_running_out_of_memory_logs_and_stays_silent();
    test_a_clip_at_another_rate_is_refused();
    test_a_file_that_is_not_ogg_is_refused();
    test_a_flipped_bit_in_a_header_is_refused_and_the_other_clips_still_load();
    test_a_clip_of_only_headers_is_a_decoder_failure_not_a_memory_failure();
    test_a_clip_that_promises_frames_and_decodes_none_is_refused_and_freed();
    test_each_window_line_counts_its_grains_and_its_largest_period();
    test_a_submit_inside_the_deadline_is_not_late();
    test_a_period_at_the_limit_is_not_late();
    test_a_stretch_the_next_period_brings_back_to_the_slack_is_not_late();
    test_a_stretch_exactly_at_the_slack_starts_no_count();
    test_a_period_past_the_limit_is_late_in_the_window_and_stopped_lines();
    test_the_stretch_most_port_opens_show_is_paid_back_and_is_not_late();
    test_any_late_return_the_next_period_pays_back_is_not_late();
    test_a_return_too_late_for_the_next_submit_is_late();
    test_the_slip_one_port_open_showed_stays_and_is_late_once();
    test_a_short_real_gap_mid_play_is_late();
    test_the_first_grain_after_idle_has_no_period_to_be_late_on();
    test_the_last_window_is_timed_before_the_drain();
    test_a_stop_fades_the_voices_sends_a_zero_grain_drains_and_closes();
    test_a_stop_forgets_voices_and_starts_still_waiting();
    test_a_stop_while_the_port_is_closed_forgets_starts_still_waiting();
    test_preparing_a_new_clip_while_the_port_is_open_closes_it_first();
    test_a_port_that_will_not_open_logs_its_code_and_stays_silent();
    test_submit_failures_close_the_port_and_keep_stepping();
    test_shutdown_while_playing_closes_and_frees();
    test_the_idle_worker_sleeps_instead_of_spinning();
    test_a_worker_that_cannot_start_never_holds_anything_up();
    fake_audio_reset();
    fake_files_reset();
    return test_finish("unit/sound_engine");
}
