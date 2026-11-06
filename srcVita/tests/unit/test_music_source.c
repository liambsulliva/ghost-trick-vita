#include "fixtures/fixture_facts.h"
#include "fixtures/sine_stereo_ogg.h"
#include "music_source.h"
#include "test_support.h"

#define GRAIN_FRAMES 4096
#define LOOP_START 11025
#define LOOP_LAST 33074

static struct music_source source;
static short linear[(SINE_STEREO_FRAMES + GRAIN_FRAMES) * 2];
static short output[SINE_STEREO_FRAMES * 4 * 2];

static void decode_linear_reference(void)
{
    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    CHECK(music_source_begin(&source, 0));
    music_source_fill(&source, linear, SINE_STEREO_FRAMES);
    music_source_close(&source);
}

static void fill_in_grains(unsigned int frame_count)
{
    unsigned int done;

    for (done = 0; done < frame_count; done = done + GRAIN_FRAMES) {
        music_source_fill(&source, &output[done * 2], GRAIN_FRAMES);
    }
}

static void test_without_looping_the_end_pads_with_zeros_and_says_so(void)
{
    short grain[GRAIN_FRAMES * 2];
    int frame;
    int last_frames;

    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    music_source_set_loop_points(&source, LOOP_START, LOOP_LAST);
    CHECK(music_source_begin(&source, 0));
    fill_in_grains(SINE_STEREO_FRAMES - SINE_STEREO_FRAMES % GRAIN_FRAMES);
    CHECK(!source.has_ended);
    CHECK(memcmp(output, linear, (SINE_STEREO_FRAMES - SINE_STEREO_FRAMES % GRAIN_FRAMES) * 4) == 0);
    music_source_fill(&source, grain, GRAIN_FRAMES);
    CHECK(source.has_ended);
    CHECK_INT(source.loop_count, 0);
    CHECK_INT(source.next_frame, SINE_STEREO_FRAMES);
    last_frames = SINE_STEREO_FRAMES % GRAIN_FRAMES;
    CHECK(memcmp(grain, &linear[(SINE_STEREO_FRAMES - last_frames) * 2], (size_t)last_frames * 4) == 0);
    for (frame = last_frames; frame < GRAIN_FRAMES; frame++) {
        CHECK_INT(grain[frame * 2], 0);
        CHECK_INT(grain[frame * 2 + 1], 0);
    }
    music_source_close(&source);
}

static void test_looping_with_points_seeks_back_after_the_last_frame(void)
{
    unsigned int after_first_pass;
    unsigned int loop_length;

    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    music_source_set_loop_points(&source, LOOP_START, LOOP_LAST);
    CHECK(music_source_begin(&source, 1));
    fill_in_grains(GRAIN_FRAMES * 20);
    after_first_pass = LOOP_LAST + 1;
    loop_length = LOOP_LAST + 1 - LOOP_START;
    CHECK(memcmp(output, linear, after_first_pass * 4) == 0);
    CHECK(memcmp(&output[after_first_pass * 2], &linear[LOOP_START * 2], loop_length * 4) == 0);
    CHECK(memcmp(&output[(after_first_pass + loop_length) * 2], &linear[LOOP_START * 2], loop_length * 4) == 0);
    CHECK_INT(source.loop_count, 3);
    CHECK(!source.has_ended);
    music_source_close(&source);
}

static void test_looping_without_points_restarts_at_the_beginning(void)
{
    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    CHECK(music_source_begin(&source, 1));
    fill_in_grains(GRAIN_FRAMES * 22);
    CHECK(memcmp(output, linear, SINE_STEREO_FRAMES * 4) == 0);
    CHECK(memcmp(&output[SINE_STEREO_FRAMES * 2], linear, SINE_STEREO_FRAMES * 4) == 0);
    CHECK_INT(source.loop_count, 2);
    CHECK(!source.has_ended);
    music_source_close(&source);
}

static void test_begin_rewinds_a_source_that_has_played(void)
{
    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    CHECK(music_source_begin(&source, 0));
    fill_in_grains(GRAIN_FRAMES * 3);
    CHECK(music_source_begin(&source, 0));
    CHECK_INT(source.next_frame, 0);
    fill_in_grains(GRAIN_FRAMES * 2);
    CHECK(memcmp(output, linear, GRAIN_FRAMES * 2 * 4) == 0);
    music_source_close(&source);
}

static void test_a_loop_of_one_frame_keeps_giving_that_frame(void)
{
    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    music_source_set_loop_points(&source, 1000, 1000);
    CHECK(music_source_begin(&source, 1));
    music_source_fill(&source, output, 1004);
    CHECK_INT(output[1001 * 2], linear[1000 * 2]);
    CHECK_INT(output[1002 * 2], linear[1000 * 2]);
    CHECK_INT(source.loop_count, 3);
    music_source_close(&source);
}

static void test_a_looping_stream_without_audio_ends_instead_of_spinning(void)
{
    short grain[GRAIN_FRAMES * 2];

    CHECK_INT(music_source_open(&source, sine_stereo_ogg, SINE_STEREO_HEADER_PAGES_BYTES), VORBIS_STREAM_OK);
    CHECK(music_source_begin(&source, 1));
    music_source_fill(&source, grain, GRAIN_FRAMES);
    CHECK(source.has_ended);
    CHECK_INT(grain[0], 0);
    CHECK_INT(grain[GRAIN_FRAMES * 2 - 1], 0);
    music_source_close(&source);
}

static void test_a_loop_start_at_the_very_end_ends_instead_of_spinning(void)
{
    short grain[GRAIN_FRAMES * 2];

    CHECK_INT(music_source_open(&source, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    music_source_set_loop_points(&source, SINE_STEREO_FRAMES, SINE_STEREO_FRAMES + 100);
    CHECK(music_source_begin(&source, 1));
    fill_in_grains(GRAIN_FRAMES * 10);
    CHECK(!source.has_ended);
    music_source_fill(&source, grain, GRAIN_FRAMES);
    CHECK(source.has_ended);
    CHECK_INT(source.loop_count, 1);
    music_source_close(&source);
}

int main(void)
{
    decode_linear_reference();
    test_without_looping_the_end_pads_with_zeros_and_says_so();
    test_looping_with_points_seeks_back_after_the_last_frame();
    test_looping_without_points_restarts_at_the_beginning();
    test_begin_rewinds_a_source_that_has_played();
    test_a_loop_of_one_frame_keeps_giving_that_frame();
    test_a_looping_stream_without_audio_ends_instead_of_spinning();
    test_a_loop_start_at_the_very_end_ends_instead_of_spinning();
    return test_finish("unit/music_source");
}
