#include <stdio.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "script_tables.h"
#include "script_view.h"
#include "texture_cache.h"
#include "test_support.h"

#define TEST_MOVIE_FRAMES 20

static struct script_tables tables;

static void start_with_a_movie_on_disk(void)
{
    static const char effects[] = "movie time_jump 20 3 0 82\n";
    char path[64];
    int frame;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    for (frame = 0; frame < TEST_MOVIE_FRAMES; frame++) {
        snprintf(path, sizeof(path), "tex/movie_time_jump_%03d.gtex", frame);
        fake_files_add_rgba_texture(path, 2, 2);
    }
    texture_cache_init("tex", 1u << 20);
    script_tables_clear(&tables);
    CHECK(script_tables_read_effects(&tables, effects, (unsigned int)strlen(effects)));
    script_view_start(&tables);
}

static void finish_every_read(void)
{
    int round;

    for (round = 0; round < 64; round++) {
        fake_reader_release_reads(64);
        texture_cache_upload_finished_reads(64);
        texture_cache_begin_frame();
    }
}

static void test_a_movie_waits_until_its_first_frames_are_loaded(void)
{
    start_with_a_movie_on_disk();
    fake_reader_hold_reads(1);
    script_view_movie_started("time_jump");
    CHECK(script_view_movie_covers_the_screen());
    CHECK(script_view_movie_is_waiting_for_frames());
    fake_reader_hold_reads(0);
    finish_every_read();
    CHECK(!script_view_movie_is_waiting_for_frames());
}

static void test_a_playing_movie_waits_for_the_frame_it_reached(void)
{
    int tick;

    start_with_a_movie_on_disk();
    script_view_movie_started("time_jump");
    CHECK(script_view_movie_is_waiting_for_frames());
    finish_every_read();
    CHECK(!script_view_movie_is_waiting_for_frames());
    texture_cache_clear();
    texture_cache_init("tex", 1u << 20);
    fake_reader_hold_reads(1);
    for (tick = 0; tick < 3 * 15; tick++) {
        script_view_advance();
    }
    CHECK(script_view_movie_is_waiting_for_frames());
    fake_reader_hold_reads(0);
    finish_every_read();
    CHECK(!script_view_movie_is_waiting_for_frames());
}

static void test_no_movie_never_waits(void)
{
    start_with_a_movie_on_disk();
    CHECK(!script_view_movie_is_waiting_for_frames());
}

int main(void)
{
    test_a_movie_waits_until_its_first_frames_are_loaded();
    test_a_playing_movie_waits_for_the_frame_it_reached();
    test_no_movie_never_waits();
    fake_files_reset();
    return test_finish("unit/movie_waits_for_frames");
}
