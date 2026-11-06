#include <stdio.h>
#include "audio_port.h"
#include "audio_thread.h"
#include "crc32.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "file.h"
#include "fixtures/fixture_facts.h"
#include "fixtures/ogg_edit.h"
#include "fixtures/sine_stereo_ogg.h"
#include "music_engine.h"
#include "pcm_ramp.h"
#include "run_log.h"
#include "test_support.h"
#include "vorbis_stream.h"

#define GRAIN_BYTES (MUSIC_GRAIN_FRAMES * 4)
#define RAMP_IN_FRAMES 512
#define WINDOW_GRAINS 11
#define GRAIN_US_AS_THE_FAKE_ADVANCES 92879
#define FIRST_BLOCK_US FAKE_AUDIO_MUSIC_SHORTEST_BLOCK_US
#define LATE_SLACK_US 1000
#define LATE_PERIOD_US (92880 + LATE_SLACK_US)
#define SUBMIT_DEADLINE_US 22080
#define LATE_RETURN_WITH_NO_GAP_US 5000
#define LATE_RETURN_THAT_EATS_THE_DEADLINE_US 15000
#define SUBMIT_AFTER_THE_LATE_RETURN_AT_THE_LIMIT_US 8081
#define BUSY_PORT_CODE ((int)0x80260002)
#define LOOP_START 11025
#define LOOP_LAST 33074
#define LOOP_TEXT_MAX 64
#define LONG_FADE_FRAMES 441000
#define SHORT_FADE_FRAMES 735
#define GAIN_WHEN_THE_LONG_FADE_WAS_SHORTENED 31551

static short linear[SINE_STEREO_FRAMES * 2];

static void decode_linear_reference(void)
{
    static struct vorbis_stream stream;
    unsigned int done;
    unsigned int got;

    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    done = 0;
    do {
        got = vorbis_stream_read(&stream, &linear[done * 2], SINE_STEREO_FRAMES - done);
        done = done + got;
    } while (got > 0 && done < SINE_STEREO_FRAMES);
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
    music_engine_init("music");
}

static void add_track(const char *name)
{
    char path[FILE_PATH_MAX];

    snprintf(path, sizeof(path), "music/%s.ogg", name);
    fake_files_add(path, sine_stereo_ogg, sizeof(sine_stereo_ogg));
}

static void add_loop_file(const char *name, const char *text)
{
    char path[FILE_PATH_MAX];

    snprintf(path, sizeof(path), "music/%s.loop", name);
    fake_files_add(path, (const unsigned char *)text, (unsigned int)strlen(text));
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
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, steps);
}

static int grains_played(void)
{
    return fake_audio_port_grain_count(AUDIO_PORT_MUSIC);
}

static const short *played_grain(int index)
{
    return fake_audio_port_grain(AUDIO_PORT_MUSIC, index);
}

static const char *port_events(void)
{
    return fake_audio_port_events(AUDIO_PORT_MUSIC);
}

static int grain_is_silent(const short *grain)
{
    int index;

    for (index = 0; index < MUSIC_GRAIN_FRAMES * 2; index++) {
        if (grain[index] != 0) {
            return 0;
        }
    }
    return 1;
}

static void pump_until_port_closes(void)
{
    int step;

    for (step = 0; step < 1000 && fake_audio_port_is_open(AUDIO_PORT_MUSIC); step++) {
        pump(1);
    }
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
}

static void finish(void)
{
    music_shutdown();
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_thread_is_alive(AUDIO_THREAD_MUSIC));
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_prepare_loads_the_track_and_reports_done_without_opening_the_port(void)
{
    set_up();
    add_track("song");
    music_request_prepare("song");
    CHECK(!music_prepare_is_done("song"));
    pump(1);
    CHECK(music_prepare_is_done("song"));
    CHECK(!music_prepare_is_done("another"));
    CHECK_HAS_TEXT(log_text(), "music loaded name=song bytes=");
    CHECK_HAS_TEXT(log_text(), "frames=44100 rate=44100 channels=2 loop_start=-1 loop_last=-1 decoder_bytes=");
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    CHECK_HAS_TEXT(log_text(), "music main_thread priority=64 mask=0x00040000 cpu=2 arm_mhz=333");
    CHECK_INT(count_in_log("music thread priority=64 mask=0x00040000 cpu=2 arm_mhz=333 fpscr=0x00000000"), 1);
    pump(5);
    CHECK_INT(count_in_log("music thread "), 1);
    finish();
}

static void test_playback_starts_with_a_zero_grain_then_ramps_in_then_matches_the_decode(void)
{
    const short *grain;
    int frame;

    set_up();
    add_track("song");
    music_request_play("song", 0);
    pump(1);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_INT(fake_audio_port_rate(AUDIO_PORT_MUSIC), MUSIC_RATE);
    CHECK_INT(fake_audio_port_grain_frames(AUDIO_PORT_MUSIC), MUSIC_GRAIN_FRAMES);
    CHECK_HAS_TEXT(log_text(), "music port result=0x00000000 rate=44100 grain=4096 mode=1 adopt=1");
    CHECK_HAS_TEXT(log_text(), "music started name=song looping=0 loop_start=-1 loop_last=-1");
    CHECK_HAS_TEXT(log_text(), "music ramp at=4096 frames=512 from=0 to=32768");
    CHECK_INT(grains_played(), 1);
    CHECK(grain_is_silent(played_grain(0)));
    pump(1);
    grain = played_grain(1);
    for (frame = 0; frame < RAMP_IN_FRAMES; frame++) {
        CHECK_INT(grain[frame * 2], (linear[frame * 2] * (PCM_RAMP_FULL_GAIN * frame / RAMP_IN_FRAMES)) >> 15);
    }
    CHECK(memcmp(&grain[RAMP_IN_FRAMES * 2], &linear[RAMP_IN_FRAMES * 2], (MUSIC_GRAIN_FRAMES - RAMP_IN_FRAMES) * 4) == 0);
    pump(1);
    CHECK(memcmp(played_grain(2), &linear[MUSIC_GRAIN_FRAMES * 2], GRAIN_BYTES) == 0);
    finish();
}

static unsigned int crc_of_grains(int first, int count)
{
    unsigned int crc;
    int index;

    crc = 0;
    for (index = first; index < first + count; index++) {
        crc = crc32_add(crc, (const unsigned char *)played_grain(index), GRAIN_BYTES);
    }
    return crc;
}

static void test_each_window_line_describes_exactly_what_was_submitted(void)
{
    char wanted[256];

    set_up();
    add_track("song");
    fake_clock_set_tick_per_read_us(1000);
    music_request_play("song", 1);
    pump(WINDOW_GRAINS);
    snprintf(wanted, sizeof(wanted), "music window name=song index=0 at=0 source_frame=0 grains=11 frames=45056 crc32=%08x crc_total=%08x wall_us=", crc_of_grains(0, WINDOW_GRAINS), crc_of_grains(0, WINDOW_GRAINS));
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 loops=0 decode_us_avg=909 decode_us_max=1000 ");
    pump(WINDOW_GRAINS);
    CHECK_INT(grains_played(), WINDOW_GRAINS * 2);
    snprintf(wanted, sizeof(wanted), "music window name=song index=1 at=45056 source_frame=40960 grains=11 frames=45056 crc32=%08x crc_total=%08x wall_us=", crc_of_grains(WINDOW_GRAINS, WINDOW_GRAINS), crc_of_grains(0, WINDOW_GRAINS * 2));
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "loops=1 ");
    fake_clock_set_tick_per_read_us(0);
    finish();
}

static void test_wall_time_comes_from_the_first_submit(void)
{
    char wanted[128];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(WINDOW_GRAINS);
    snprintf(wanted, sizeof(wanted), "wall_us=%llu ", FIRST_BLOCK_US + GRAIN_US_AS_THE_FAKE_ADVANCES * (WINDOW_GRAINS - 1));
    CHECK_HAS_TEXT(log_text(), wanted);
    finish();
}

static void test_the_stopped_line_totals_every_grain_and_the_last_window_is_not_lost(void)
{
    char wanted[256];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(4);
    music_request_stop(0);
    pump_until_port_closes();
    snprintf(wanted, sizeof(wanted), "music stopped name=song reason=stop submitted=%d grains=%d crc_total=%08x late=0", grains_played() * MUSIC_GRAIN_FRAMES, grains_played(), crc_of_grains(0, grains_played()));
    CHECK_HAS_TEXT(log_text(), wanted);
    snprintf(wanted, sizeof(wanted), "music window name=song index=0 at=0 source_frame=0 grains=%d ", grains_played());
    CHECK_HAS_TEXT(log_text(), wanted);
    finish();
}

static void test_the_first_grain_after_idle_has_no_period_to_be_late_on(void)
{
    set_up();
    add_track("song");
    fake_clock_set_us(1000000);
    music_request_play("song", 1);
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_HAS_TEXT(log_text(), "period_us_max=92879");
    CHECK(strstr(log_text(), "gap_us_max=1000000") == NULL);
    finish();
}

static void test_each_window_reports_its_own_largest_period(void)
{
    char wanted[64];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(SUBMIT_DEADLINE_US + 500);
    pump(WINDOW_GRAINS - 3);
    snprintf(wanted, sizeof(wanted), "period_us_max=%llu", FIRST_BLOCK_US + SUBMIT_DEADLINE_US + 500);
    CHECK_HAS_TEXT(log_text(), wanted);
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "index=1 ");
    CHECK_INT(count_in_log(wanted), 1);
    CHECK_HAS_TEXT(log_text(), "period_us_max=92879");
    finish();
}

static void test_a_submit_inside_the_deadline_does_not_stretch_the_period(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(SUBMIT_DEADLINE_US - 1000);
    pump(WINDOW_GRAINS - 3);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_HAS_TEXT(log_text(), "period_us_max=92879");
    CHECK_INT(count_in_log("music ramp"), 1);
    finish();
}

static void test_the_gap_is_measured_up_to_the_start_of_the_next_submit(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(2);
    fake_clock_set_tick_per_read_us(1000);
    pump(1);
    fake_clock_set_tick_per_read_us(0);
    pump(WINDOW_GRAINS - 3);
    CHECK_HAS_TEXT(log_text(), "gap_us_max=2000 ");
    finish();
}

static void test_a_gap_counts_as_late_and_ramps_the_next_grain_in_again(void)
{
    const short *grain;
    int frame;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(4);
    CHECK_INT(count_in_log("music ramp"), 1);
    fake_clock_advance_us(300000);
    pump(1);
    CHECK_HAS_TEXT(log_text(), "music ramp at=16384 frames=512 from=0 to=32768");
    grain = played_grain(4);
    CHECK_INT(grain[0], 0);
    CHECK_INT(grain[1], 0);
    for (frame = 1; frame < RAMP_IN_FRAMES; frame++) {
        CHECK_INT(grain[frame * 2], (linear[(MUSIC_GRAIN_FRAMES * 3 + frame) * 2] * (PCM_RAMP_FULL_GAIN * frame / RAMP_IN_FRAMES)) >> 15);
    }
    CHECK(memcmp(&grain[RAMP_IN_FRAMES * 2], &linear[(MUSIC_GRAIN_FRAMES * 3 + RAMP_IN_FRAMES) * 2], (MUSIC_GRAIN_FRAMES - RAMP_IN_FRAMES) * 4) == 0);
    pump(WINDOW_GRAINS - 5);
    CHECK_HAS_TEXT(log_text(), " late=1 ");
    CHECK_HAS_TEXT(log_text(), "gap_us_max=300000");
    CHECK_INT(count_in_log("music ramp"), 2);
    finish();
}

static void test_a_period_at_the_limit_is_not_late(void)
{
    char wanted[64];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US);
    pump(WINDOW_GRAINS - 3);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_INT(count_in_log("music ramp"), 1);
    finish();
}

static void test_a_stretch_the_next_period_brings_back_to_the_slack_is_not_late(void)
{
    char wanted[64];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US + 1);
    pump(WINDOW_GRAINS - 3);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US + 1);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_period_past_the_limit_is_late_once_and_its_grain_ramps_in(void)
{
    char wanted[64];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US + 2);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 2);
    CHECK_HAS_TEXT(log_text(), "music ramp at=12288 frames=512 from=0 to=32768");
    pump(WINDOW_GRAINS - 4);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", LATE_PERIOD_US + 2);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), " late=1 ");
    CHECK_INT(count_in_log("music ramp"), 2);
    finish();
}

static void test_a_stretch_exactly_at_the_slack_starts_no_count(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US);
    pump(1);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, 3);
    pump(WINDOW_GRAINS - 4);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_gap_on_the_last_grain_of_a_window_counts_in_the_next_window(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(WINDOW_GRAINS - 1);
    fake_clock_advance_us(300000);
    pump(1);
    CHECK_HAS_TEXT(log_text(), "index=0 ");
    CHECK_HAS_TEXT(log_text(), "gap_us_max=300000");
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "index=1 ");
    CHECK_HAS_TEXT(log_text(), " late=1 ");
    finish();
}

static void test_a_sustained_stretch_counts_each_grain(void)
{
    int grain_index;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(1);
    for (grain_index = 1; grain_index < WINDOW_GRAINS; grain_index++) {
        fake_clock_advance_us(SUBMIT_DEADLINE_US + 2000);
        pump(1);
    }
    CHECK_HAS_TEXT(log_text(), " late=9 ");
    finish();
}

static void test_the_ramp_goes_on_the_grain_whose_submit_will_miss_the_deadline(void)
{
    set_up();
    add_track("song");
    fake_clock_set_tick_per_read_us(1000);
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(SUBMIT_DEADLINE_US + LATE_SLACK_US - 2000 - 1000 + 1);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 2);
    CHECK_HAS_TEXT(log_text(), "music ramp at=12288 frames=512 from=0 to=32768");
    fake_clock_set_tick_per_read_us(0);
    finish();
}

static void test_a_submit_expected_right_at_the_deadline_needs_no_ramp(void)
{
    set_up();
    add_track("song");
    fake_clock_set_tick_per_read_us(1000);
    music_request_play("song", 1);
    pump(3);
    fake_clock_advance_us(SUBMIT_DEADLINE_US + LATE_SLACK_US - 2000 - 1000);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 1);
    fake_clock_set_tick_per_read_us(0);
    finish();
}

static void test_a_return_late_inside_the_slack_is_not_late(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_PERIOD_US - GRAIN_US_AS_THE_FAKE_ADVANCES);
    pump(WINDOW_GRAINS - 3);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK_INT(count_in_log("music ramp"), 1);
    finish();
}

static void test_a_late_return_the_next_period_pays_back_is_no_gap_and_gets_no_ramp(void)
{
    char wanted[64];

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_RETURN_WITH_NO_GAP_US);
    pump(WINDOW_GRAINS - 3);
    CHECK_INT(count_in_log("music ramp"), 1);
    CHECK(memcmp(played_grain(4), &linear[MUSIC_GRAIN_FRAMES * 3 * 2], GRAIN_BYTES) == 0);
    snprintf(wanted, sizeof(wanted), "period_us_max=%d", GRAIN_US_AS_THE_FAKE_ADVANCES + LATE_RETURN_WITH_NO_GAP_US);
    CHECK_HAS_TEXT(log_text(), wanted);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    CHECK(strstr(log_text(), "late=1") == NULL);
    finish();
}

static void test_a_submit_inside_what_a_late_return_leaves_of_the_deadline_gets_no_ramp(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_RETURN_THAT_EATS_THE_DEADLINE_US);
    pump(1);
    fake_clock_advance_us(SUBMIT_AFTER_THE_LATE_RETURN_AT_THE_LIMIT_US);
    pump(WINDOW_GRAINS - 4);
    CHECK_INT(count_in_log("music ramp"), 1);
    CHECK_HAS_TEXT(log_text(), "late=0 ");
    finish();
}

static void test_a_submit_past_what_a_late_return_leaves_of_the_deadline_is_late_and_ramps(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_RETURN_THAT_EATS_THE_DEADLINE_US);
    pump(1);
    fake_clock_advance_us(SUBMIT_AFTER_THE_LATE_RETURN_AT_THE_LIMIT_US + 1);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 2);
    CHECK_HAS_TEXT(log_text(), "music ramp at=16384 frames=512 from=0 to=32768");
    pump(WINDOW_GRAINS - 5);
    CHECK_HAS_TEXT(log_text(), " late=1 ");
    CHECK_INT(count_in_log("music ramp"), 2);
    finish();
}

static void test_the_deadline_is_back_to_normal_once_a_late_return_is_paid_back(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_RETURN_WITH_NO_GAP_US);
    pump(2);
    fake_clock_advance_us(LATE_PERIOD_US - FIRST_BLOCK_US + 1);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 2);
    CHECK_HAS_TEXT(log_text(), "music ramp at=20480 frames=512 from=0 to=32768");
    finish();
}

static void test_a_gap_after_a_late_return_during_a_fade_counts_but_gets_no_ramp(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    music_request_stop(600);
    pump(1);
    fake_audio_port_add_block_once_us(AUDIO_PORT_MUSIC, LATE_PERIOD_US);
    pump(2);
    CHECK_INT(count_in_log("to=32768"), 1);
    music_request_stop(0);
    pump_until_port_closes();
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK_HAS_TEXT(log_text(), "late=1");
    finish();
}

static void test_starving_sleeps_once_and_the_detector_fires(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    music_request_starve(300);
    pump(1);
    CHECK_INT(count_in_log("music ramp"), 2);
    pump(WINDOW_GRAINS);
    CHECK_INT(count_in_log("music ramp"), 2);
    CHECK_HAS_TEXT(log_text(), " late=1 ");
    CHECK_HAS_TEXT(log_text(), "gap_us_max=300000");
    finish();
}

static void test_a_loop_file_with_looping_on_seeks_after_the_last_frame(void)
{
    static short expected[MUSIC_GRAIN_FRAMES * 3 * 2];
    int after_first_grain;

    set_up();
    add_track("song");
    add_loop_file("song", "11025 33074\n");
    music_request_play("song", 1);
    pump(WINDOW_GRAINS);
    CHECK_HAS_TEXT(log_text(), "loop_start=11025 loop_last=33074 decoder_bytes=");
    CHECK_HAS_TEXT(log_text(), "music started name=song looping=1 loop_start=11025 loop_last=33074");
    CHECK_HAS_TEXT(log_text(), "loops=1 ");
    after_first_grain = LOOP_LAST + 1 - MUSIC_GRAIN_FRAMES * 8;
    memcpy(expected, &linear[MUSIC_GRAIN_FRAMES * 8 * 2], (size_t)after_first_grain * 4);
    memcpy(&expected[after_first_grain * 2], &linear[LOOP_START * 2], (size_t)(MUSIC_GRAIN_FRAMES - after_first_grain) * 4);
    CHECK(memcmp(played_grain(9), expected, GRAIN_BYTES) == 0);
    finish();
}

static void test_looping_without_a_loop_file_restarts_at_frame_zero(void)
{
    static short expected[MUSIC_GRAIN_FRAMES * 2];
    int frames_at_the_end;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(WINDOW_GRAINS + 1);
    frames_at_the_end = SINE_STEREO_FRAMES - MUSIC_GRAIN_FRAMES * 10;
    memcpy(expected, &linear[MUSIC_GRAIN_FRAMES * 10 * 2], (size_t)frames_at_the_end * 4);
    memcpy(&expected[frames_at_the_end * 2], linear, (size_t)(MUSIC_GRAIN_FRAMES - frames_at_the_end) * 4);
    CHECK(memcmp(played_grain(11), expected, GRAIN_BYTES) == 0);
    finish();
}

static void test_a_track_without_looping_ends_by_itself(void)
{
    int step;

    set_up();
    add_track("song");
    add_loop_file("song", "11025 33074\n");
    music_request_play("song", 0);
    for (step = 0; step < 15; step++) {
        pump(1);
    }
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=end ");
    CHECK_HAS_TEXT(log_text(), "loops=0 ");
    CHECK(grain_is_silent(played_grain(grains_played() - 1)));
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 1);
    pump(5);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 1);
    finish();
}

static void test_a_stop_ramps_then_sends_a_zero_grain_then_drains_then_closes(void)
{
    const short *fading;
    int frame;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    fake_audio_port_clear_events(AUDIO_PORT_MUSIC);
    music_request_stop(4);
    pump(1);
    CHECK_TEXT(port_events(), "submit submit drain close ");
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music ramp at=12288 frames=2940 from=32768 to=0");
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    fading = played_grain(grains_played() - 2);
    CHECK_INT(fading[0], linear[(MUSIC_GRAIN_FRAMES * 2) * 2]);
    for (frame = 2940; frame < MUSIC_GRAIN_FRAMES; frame++) {
        CHECK_INT(fading[frame * 2], 0);
        CHECK_INT(fading[frame * 2 + 1], 0);
    }
    CHECK(fading[2000 * 2] != 0);
    CHECK(grain_is_silent(played_grain(grains_played() - 1)));
    finish();
}

static void test_a_long_fade_spans_grains_and_a_newer_request_shortens_it(void)
{
    static short expected[MUSIC_GRAIN_FRAMES * 2];
    struct pcm_ramp shorter_fade;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    music_request_stop(600);
    pump(4);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "frames=441000 from=32768 to=0");
    music_request_stop(1);
    pump(1);
    CHECK_HAS_TEXT(log_text(), "frames=735 from=31551 to=0");
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    memcpy(expected, &linear[MUSIC_GRAIN_FRAMES * 6 * 2], GRAIN_BYTES);
    pcm_ramp_begin(&shorter_fade, GAIN_WHEN_THE_LONG_FADE_WAS_SHORTENED, 0, SHORT_FADE_FRAMES);
    pcm_ramp_apply(&shorter_fade, expected, MUSIC_GRAIN_FRAMES);
    CHECK(memcmp(played_grain(grains_played() - 2), expected, GRAIN_BYTES) == 0);
    finish();
}

static void test_a_gap_in_the_playback_after_a_fade_still_ramps_in_again(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    music_request_play("song", 1);
    pump(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    pump(1);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    pump(2);
    CHECK_INT(count_in_log("from=0 to=32768"), 2);
    fake_clock_advance_us(300000);
    pump(1);
    CHECK_INT(count_in_log("from=0 to=32768"), 3);
    finish();
}

static void test_a_gap_during_a_fade_counts_as_late_but_does_not_bring_the_music_back(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(3);
    music_request_stop(600);
    pump(2);
    fake_clock_advance_us(300000);
    pump(1);
    CHECK_INT(count_in_log("to=32768"), 1);
    music_request_stop(0);
    pump_until_port_closes();
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK_HAS_TEXT(log_text(), "late=1");
    finish();
}

static void test_a_stop_while_idle_frees_the_track(void)
{
    set_up();
    add_track("song");
    music_request_prepare("song");
    pump(1);
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    music_request_stop(0);
    pump(1);
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    finish();
}

static void test_play_then_stop_before_the_worker_wakes_stays_silent(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    music_request_stop(4);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_a_port_that_will_not_open_logs_its_code_and_stays_silent(void)
{
    set_up();
    add_track("song");
    fake_audio_port_fail_open(AUDIO_PORT_MUSIC, BUSY_PORT_CODE);
    music_request_prepare("song");
    pump(1);
    CHECK(music_prepare_is_done("song"));
    music_request_play("song", 1);
    pump(10);
    CHECK_HAS_TEXT(log_text(), "music port_open_failed code=0x80260002");
    CHECK_INT(count_in_log("music port_open_failed"), 1);
    CHECK_INT(grains_played(), 0);
    CHECK(strstr(log_text(), "music started") == NULL);
    finish();
}

static void test_a_submit_failure_closes_the_port_and_keeps_stepping(void)
{
    set_up();
    add_track("song");
    fake_audio_port_fail_submit_after(AUDIO_PORT_MUSIC, 3, BUSY_PORT_CODE);
    music_request_play("song", 1);
    pump(10);
    CHECK_HAS_TEXT(log_text(), "music submit_failed code=0x80260002");
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=submit_failed ");
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(strstr(port_events(), "drain") == NULL);
    CHECK_INT(grains_played(), 3);
    finish();
}

static void test_a_failing_first_grain_and_a_failing_zero_grain_also_close_the_port(void)
{
    set_up();
    add_track("song");
    fake_audio_port_fail_submit_after(AUDIO_PORT_MUSIC, 0, BUSY_PORT_CODE);
    music_request_play("song", 1);
    pump(5);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_INT(count_in_log("music submit_failed"), 1);
    finish();

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(2);
    fake_audio_port_fail_submit_after(AUDIO_PORT_MUSIC, 1, BUSY_PORT_CODE);
    music_request_stop(0);
    pump(3);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=submit_failed ");
    finish();
}

static void test_playback_after_a_failed_submit_still_starts_with_a_zero_grain(void)
{
    int second_start;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    fake_audio_port_fail_submit_after(AUDIO_PORT_MUSIC, 3, BUSY_PORT_CODE);
    pump(4);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    fake_audio_port_fail_submit_after(AUDIO_PORT_MUSIC, 0, 0);
    second_start = grains_played();
    music_request_play("song", 1);
    pump(1);
    CHECK(grain_is_silent(played_grain(second_start)));
    finish();
}

static void test_a_missing_file_reports_done_and_stays_silent(void)
{
    set_up();
    music_request_prepare("gone");
    CHECK(!music_prepare_is_done("gone"));
    pump(1);
    CHECK(music_prepare_is_done("gone"));
    CHECK_HAS_TEXT(log_text(), "music file_missing name=gone code=-2");
    music_request_play("gone", 1);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_running_out_of_memory_reports_done_and_stays_silent(void)
{
    set_up();
    add_track("song");
    fake_memory_set_largest_block(16);
    music_request_prepare("song");
    pump(1);
    CHECK(music_prepare_is_done("song"));
    CHECK_HAS_TEXT(log_text(), "music out_of_memory name=song");
    music_request_play("song", 1);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_a_file_that_is_not_ogg_reports_done_and_frees_its_bytes(void)
{
    static const unsigned char not_ogg[64] = "this is not an ogg file";

    set_up();
    fake_files_add("music/song.ogg", not_ogg, sizeof(not_ogg));
    music_request_prepare("song");
    pump(1);
    CHECK(music_prepare_is_done("song"));
    CHECK_HAS_TEXT(log_text(), "music decoder_failed name=song error=");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    music_request_play("song", 1);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_a_flipped_bit_in_a_header_reports_done_and_frees_its_bytes(void)
{
    static unsigned char flipped[sizeof(sine_stereo_ogg)];

    set_up();
    memcpy(flipped, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    flipped[SINE_STEREO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("music/song.ogg", flipped, sizeof(flipped));
    music_request_prepare("song");
    pump(1);
    CHECK(music_prepare_is_done("song"));
    CHECK_HAS_TEXT(log_text(), "music decoder_failed name=song error=-1");
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    music_request_play("song", 1);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_a_track_that_cannot_rewind_logs_it_and_stays_silent(void)
{
    static unsigned char no_length[sizeof(sine_stereo_ogg)];

    set_up();
    memcpy(no_length, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    memset(&no_length[SINE_STEREO_LAST_PAGE_GRANULE_OFFSET], 0, OGG_GRANULE_BYTES);
    ogg_edit_refresh_crcs(no_length, sizeof(no_length));
    fake_files_add("music/song.ogg", no_length, sizeof(no_length));
    music_request_play("song", 1);
    pump(3);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 1);
    music_request_play("song", 1);
    pump(3);
    CHECK_HAS_TEXT(log_text(), "music decoder_failed name=song error=rewind");
    CHECK_INT(count_in_log("music started"), 1);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    finish();
}

static void test_a_track_at_another_rate_is_refused(void)
{
    static unsigned char other_rate[sizeof(sine_stereo_ogg)];

    set_up();
    memcpy(other_rate, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    other_rate[SINE_RATE_OFFSET_IN_OGG] = 0x22;
    other_rate[SINE_RATE_OFFSET_IN_OGG + 1] = 0x56;
    ogg_edit_refresh_crcs(other_rate, sizeof(other_rate));
    fake_files_add("music/song.ogg", other_rate, sizeof(other_rate));
    music_request_play("song", 1);
    pump(3);
    CHECK_HAS_TEXT(log_text(), "music wrong_rate name=song rate=22050");
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    finish();
}

static void test_a_bad_loop_file_is_logged_and_ignored(void)
{
    static const char *const bad_files[] = { "not numbers\n", "33074 11025\n", "0 44100\n", "5\n" };
    unsigned int index;

    for (index = 0; index < sizeof(bad_files) / sizeof(bad_files[0]); index++) {
        set_up();
        add_track("song");
        add_loop_file("song", bad_files[index]);
        music_request_prepare("song");
        pump(1);
        CHECK_HAS_TEXT(log_text(), "music bad_loop_file name=song");
        CHECK_HAS_TEXT(log_text(), "loop_start=-1 loop_last=-1");
        finish();
    }
}

static void check_a_loop_file_of_this_many_bytes_still_gives_its_loop_points(unsigned int size)
{
    static char padded[LOOP_TEXT_MAX * 2];

    memset(padded, ' ', sizeof(padded));
    memcpy(padded, "11025 33074", strlen("11025 33074"));
    padded[size] = '\0';
    set_up();
    add_track("song");
    add_loop_file("song", padded);
    music_request_prepare("song");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "loop_start=11025 loop_last=33074");
    finish();
}

static void test_a_loop_file_too_long_for_its_buffer_is_read_up_to_the_limit(void)
{
    check_a_loop_file_of_this_many_bytes_still_gives_its_loop_points(LOOP_TEXT_MAX - 1);
    check_a_loop_file_of_this_many_bytes_still_gives_its_loop_points(LOOP_TEXT_MAX);
    check_a_loop_file_of_this_many_bytes_still_gives_its_loop_points(LOOP_TEXT_MAX + 1);
    check_a_loop_file_of_this_many_bytes_still_gives_its_loop_points(LOOP_TEXT_MAX * 2 - 1);
}

static void test_the_last_frame_of_the_track_is_a_valid_loop_end(void)
{
    set_up();
    add_track("song");
    add_loop_file("song", "0 44099\n");
    music_request_prepare("song");
    pump(1);
    CHECK_HAS_TEXT(log_text(), "loop_start=0 loop_last=44099");
    CHECK(strstr(log_text(), "bad_loop_file") == NULL);
    finish();
}

static void test_a_new_track_fades_the_old_one_out_then_loads_and_starts(void)
{
    set_up();
    add_track("song");
    add_track("other");
    music_request_play("song", 1);
    pump(3);
    music_request_play("other", 0);
    pump(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK(strstr(log_text(), "music loaded name=other") == NULL);
    pump(1);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music loaded name=other");
    CHECK_HAS_TEXT(log_text(), "music started name=other looping=0");
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    finish();
}

static void test_replaying_the_same_track_keeps_it_in_memory_and_starts_from_the_top(void)
{
    int second_start;

    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(4);
    music_request_stop(MUSIC_QUICK_FADE_TICKS);
    music_request_prepare("song");
    CHECK(!music_prepare_is_done("song"));
    pump(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!music_prepare_is_done("song"));
    pump(1);
    CHECK(music_prepare_is_done("song"));
    second_start = grains_played();
    music_request_play("song", 1);
    pump(3);
    CHECK_INT(count_in_log("music loaded"), 1);
    CHECK_INT(count_in_log("music started"), 2);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 2);
    CHECK(grain_is_silent(played_grain(second_start)));
    CHECK(memcmp(played_grain(second_start + 2), &linear[MUSIC_GRAIN_FRAMES * 2], GRAIN_BYTES) == 0);
    CHECK_HAS_TEXT(log_text(), "music ramp at=4096 frames=512 from=0 to=32768");
    finish();
}

static void test_shutdown_while_playing_fades_closes_and_frees(void)
{
    set_up();
    add_track("song");
    music_request_play("song", 1);
    pump(4);
    finish();
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK_HAS_TEXT(port_events(), "submit drain close ");
}

static void test_shutdown_with_nothing_loaded_ends_the_worker(void)
{
    set_up();
    finish();
}

static void test_a_worker_that_cannot_start_never_holds_anything_up(void)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    run_log_open("run.log");
    fake_audio_thread_fail_start(AUDIO_THREAD_MUSIC, BUSY_PORT_CODE);
    music_engine_init("music");
    CHECK_HAS_TEXT(log_text(), "music thread_failed code=0x80260002");
    music_request_prepare("song");
    CHECK(music_prepare_is_done("song"));
    music_request_play("song", 1);
    music_shutdown();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

int main(void)
{
    decode_linear_reference();
    test_prepare_loads_the_track_and_reports_done_without_opening_the_port();
    test_playback_starts_with_a_zero_grain_then_ramps_in_then_matches_the_decode();
    test_each_window_line_describes_exactly_what_was_submitted();
    test_wall_time_comes_from_the_first_submit();
    test_the_stopped_line_totals_every_grain_and_the_last_window_is_not_lost();
    test_the_first_grain_after_idle_has_no_period_to_be_late_on();
    test_each_window_reports_its_own_largest_period();
    test_a_submit_inside_the_deadline_does_not_stretch_the_period();
    test_the_gap_is_measured_up_to_the_start_of_the_next_submit();
    test_a_gap_counts_as_late_and_ramps_the_next_grain_in_again();
    test_a_period_at_the_limit_is_not_late();
    test_a_stretch_the_next_period_brings_back_to_the_slack_is_not_late();
    test_a_period_past_the_limit_is_late_once_and_its_grain_ramps_in();
    test_a_stretch_exactly_at_the_slack_starts_no_count();
    test_a_gap_on_the_last_grain_of_a_window_counts_in_the_next_window();
    test_a_sustained_stretch_counts_each_grain();
    test_the_ramp_goes_on_the_grain_whose_submit_will_miss_the_deadline();
    test_a_submit_expected_right_at_the_deadline_needs_no_ramp();
    test_a_return_late_inside_the_slack_is_not_late();
    test_a_late_return_the_next_period_pays_back_is_no_gap_and_gets_no_ramp();
    test_a_submit_inside_what_a_late_return_leaves_of_the_deadline_gets_no_ramp();
    test_a_submit_past_what_a_late_return_leaves_of_the_deadline_is_late_and_ramps();
    test_the_deadline_is_back_to_normal_once_a_late_return_is_paid_back();
    test_a_gap_after_a_late_return_during_a_fade_counts_but_gets_no_ramp();
    test_starving_sleeps_once_and_the_detector_fires();
    test_a_loop_file_with_looping_on_seeks_after_the_last_frame();
    test_looping_without_a_loop_file_restarts_at_frame_zero();
    test_a_track_without_looping_ends_by_itself();
    test_a_stop_ramps_then_sends_a_zero_grain_then_drains_then_closes();
    test_a_long_fade_spans_grains_and_a_newer_request_shortens_it();
    test_a_gap_in_the_playback_after_a_fade_still_ramps_in_again();
    test_a_gap_during_a_fade_counts_as_late_but_does_not_bring_the_music_back();
    test_a_stop_while_idle_frees_the_track();
    test_play_then_stop_before_the_worker_wakes_stays_silent();
    test_a_port_that_will_not_open_logs_its_code_and_stays_silent();
    test_a_submit_failure_closes_the_port_and_keeps_stepping();
    test_a_failing_first_grain_and_a_failing_zero_grain_also_close_the_port();
    test_playback_after_a_failed_submit_still_starts_with_a_zero_grain();
    test_a_missing_file_reports_done_and_stays_silent();
    test_running_out_of_memory_reports_done_and_stays_silent();
    test_a_file_that_is_not_ogg_reports_done_and_frees_its_bytes();
    test_a_flipped_bit_in_a_header_reports_done_and_frees_its_bytes();
    test_a_track_that_cannot_rewind_logs_it_and_stays_silent();
    test_a_track_at_another_rate_is_refused();
    test_a_bad_loop_file_is_logged_and_ignored();
    test_a_loop_file_too_long_for_its_buffer_is_read_up_to_the_limit();
    test_the_last_frame_of_the_track_is_a_valid_loop_end();
    test_a_new_track_fades_the_old_one_out_then_loads_and_starts();
    test_replaying_the_same_track_keeps_it_in_memory_and_starts_from_the_top();
    test_shutdown_while_playing_fades_closes_and_frees();
    test_shutdown_with_nothing_loaded_ends_the_worker();
    test_a_worker_that_cannot_start_never_holds_anything_up();
    fake_audio_reset();
    fake_files_reset();
    return test_finish("unit/music_engine");
}
