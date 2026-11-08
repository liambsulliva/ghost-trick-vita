#include <stdio.h>
#include <string.h>
#include "background_reader.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "file.h"
#include "run_log.h"
#include "test_support.h"
#include "texture_cache.h"

#define SMALL_BYTES 16u
#define LARGE_SIDE 1024u
#define LARGE_BYTES (LARGE_SIDE * LARGE_SIDE * 4u)
#define FRAMES_PAST_THE_DRAWN_SPARING 61

static void reset_with_budget(unsigned int budget)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/big_b.gtex", LARGE_SIDE, LARGE_SIDE);
    fake_files_add_rgba_texture("tex/big_c.gtex", LARGE_SIDE, LARGE_SIDE);
    texture_cache_init("tex", budget);
}

static void finish_reads(void)
{
    int round;

    for (round = 0; round < 8; round++) {
        texture_cache_upload_finished_reads(8);
    }
}

static void test_read_ahead_never_evicts_a_texture_drawn_this_second(void)
{
    struct texture_cache_totals totals;
    const struct cached_texture *drawn;

    reset_with_budget(SMALL_BYTES);
    drawn = texture_cache_load_now("a");
    texture_cache_begin_frame();
    texture_cache_keep_or_start_reading_ahead("b");
    finish_reads();
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 0);
    CHECK_INT(fake_renderer_textures_created(), 1);
    CHECK(drawn->state == CACHED_TEXTURE_READY);
}

static void test_read_ahead_takes_the_room_of_a_texture_not_drawn_for_a_second(void)
{
    struct texture_cache_totals totals;
    int frame;

    reset_with_budget(SMALL_BYTES);
    texture_cache_load_now("a");
    for (frame = 0; frame < FRAMES_PAST_THE_DRAWN_SPARING; frame++) {
        texture_cache_begin_frame();
    }
    texture_cache_keep_or_start_reading_ahead("b");
    finish_reads();
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(fake_renderer_textures_created(), 2);
}

static void test_read_ahead_does_not_evict_another_texture_wanted_ahead(void)
{
    struct texture_cache_totals totals;
    int frame;

    reset_with_budget(SMALL_BYTES);
    texture_cache_load_now("a");
    for (frame = 0; frame < FRAMES_PAST_THE_DRAWN_SPARING; frame++) {
        texture_cache_begin_frame();
        texture_cache_keep_or_start_reading_ahead("a");
    }
    texture_cache_keep_or_start_reading_ahead("b");
    finish_reads();
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 0);
    CHECK_INT(fake_renderer_textures_created(), 1);
}

static void test_drawn_texture_wins_the_room_over_a_read_ahead_one(void)
{
    struct texture_cache_totals totals;
    const struct cached_texture *drawn;

    reset_with_budget(LARGE_BYTES + SMALL_BYTES);
    fake_reader_hold_reads(1);
    texture_cache_keep_or_start_reading_ahead("big_b");
    drawn = texture_cache_use_or_start_loading("big_c");
    fake_reader_release_reads(2);
    finish_reads();
    texture_cache_begin_frame();
    drawn = texture_cache_use_or_start_loading("big_c");
    finish_reads();
    texture_cache_read_totals(&totals);
    CHECK(drawn->state == CACHED_TEXTURE_READY);
    CHECK_INT(totals.bytes_in_use, LARGE_BYTES);
    CHECK_INT(totals.read_ahead_dropped + totals.evictions, 1);
}

static void test_drawing_still_evicts_textures_wanted_ahead_when_nothing_else_is_left(void)
{
    struct texture_cache_totals totals;
    const struct cached_texture *drawn;

    reset_with_budget(LARGE_BYTES + SMALL_BYTES);
    texture_cache_keep_or_start_reading_ahead("big_b");
    finish_reads();
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.bytes_in_use, LARGE_BYTES);
    texture_cache_begin_frame();
    texture_cache_keep_or_start_reading_ahead("big_b");
    drawn = texture_cache_load_now("big_c");
    texture_cache_read_totals(&totals);
    CHECK(drawn->state == CACHED_TEXTURE_READY);
    CHECK_INT(totals.evictions, 1);
}

static void test_only_drawn_loads_count_as_drawn_art_loading(void)
{
    reset_with_budget(LARGE_BYTES * 4u);
    fake_reader_hold_reads(1);
    texture_cache_keep_or_start_reading_ahead("big_b");
    CHECK_INT(texture_cache_loading_count(), 1);
    CHECK_INT(texture_cache_drawn_loading_count(), 0);
    texture_cache_use_or_start_loading("big_c");
    CHECK_INT(texture_cache_drawn_loading_count(), 1);
    texture_cache_use_or_start_loading("big_b");
    CHECK_INT(texture_cache_drawn_loading_count(), 2);
    fake_reader_release_reads(2);
    finish_reads();
    CHECK_INT(texture_cache_drawn_loading_count(), 0);
}

static void test_drawing_evicts_a_read_ahead_guess_before_a_texture_drawn_lately(void)
{
    struct texture_cache_totals totals;
    const struct cached_texture *drawn_lately;
    const struct cached_texture *guessed;
    int frame;

    reset_with_budget(LARGE_BYTES * 2u);
    texture_cache_keep_or_start_reading_ahead("big_b");
    finish_reads();
    drawn_lately = texture_cache_load_now("big_c");
    for (frame = 0; frame < 5; frame++) {
        texture_cache_begin_frame();
        texture_cache_keep_or_start_reading_ahead("big_b");
    }
    fake_files_add_rgba_texture("tex/big_d.gtex", LARGE_SIDE, LARGE_SIDE);
    texture_cache_load_now("big_d");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK(drawn_lately->state == CACHED_TEXTURE_READY && drawn_lately->texture != RENDERER_NO_TEXTURE);
    guessed = texture_cache_use_or_start_loading("big_b");
    CHECK(guessed->state == CACHED_TEXTURE_LOADING);
}

int main(void)
{
    test_drawing_evicts_a_read_ahead_guess_before_a_texture_drawn_lately();
    test_only_drawn_loads_count_as_drawn_art_loading();
    test_read_ahead_never_evicts_a_texture_drawn_this_second();
    test_read_ahead_takes_the_room_of_a_texture_not_drawn_for_a_second();
    test_read_ahead_does_not_evict_another_texture_wanted_ahead();
    test_drawn_texture_wins_the_room_over_a_read_ahead_one();
    test_drawing_still_evicts_textures_wanted_ahead_when_nothing_else_is_left();
    fake_files_reset();
    return test_finish("unit/read_ahead_room");
}
