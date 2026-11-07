#include <stdio.h>
#include "background_reader.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "soul_transition.h"
#include "test_support.h"
#include "texture_cache.h"

#define SPOKE_TICKS 12
#define ENTERING_LAST_TICK 32
#define LEAVING_LAST_TICK 12
#define SPUN_LAYERS 4

static void start_with_spoke_art(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/soul_spokes.gtex", 4, 4);
    texture_cache_init("tex", 1024);
    (void)texture_cache_use_or_start_loading("soul_spokes");
    texture_cache_upload_finished_reads(1);
    soul_transition_stop();
}

static int draws_at_this_tick(void)
{
    fake_renderer_clear_draws();
    soul_transition_draw();
    return fake_renderer_draw_count();
}

static void test_entering_shows_spokes_then_a_dark_frame_then_spun_layers(void)
{
    int tick;

    start_with_spoke_art();
    soul_transition_begin_entering();
    for (tick = 1; tick <= SPOKE_TICKS; tick++) {
        soul_transition_advance();
        CHECK_INT(draws_at_this_tick(), 1);
        if (soul_transition_wants_snapshot()) {
            CHECK_INT(tick, 11);
            soul_transition_snapshot_taken();
        }
    }
    soul_transition_advance();
    CHECK_INT(draws_at_this_tick(), 0);
    for (tick = 14; tick <= ENTERING_LAST_TICK; tick++) {
        soul_transition_advance();
        CHECK_INT(draws_at_this_tick(), SPUN_LAYERS);
        CHECK(fake_renderer_draw(0)->is_snapshot);
    }
    soul_transition_advance();
    CHECK_INT(draws_at_this_tick(), 0);
}

static void test_spun_layers_stay_away_without_a_snapshot(void)
{
    int tick;

    start_with_spoke_art();
    soul_transition_begin_entering();
    for (tick = 1; tick <= 20; tick++) {
        soul_transition_advance();
    }
    CHECK_INT(draws_at_this_tick(), 0);
}

static void test_a_late_frame_still_takes_the_snapshot(void)
{
    int tick;

    start_with_spoke_art();
    soul_transition_begin_entering();
    for (tick = 1; tick <= 12; tick++) {
        soul_transition_advance();
    }
    CHECK(soul_transition_wants_snapshot());
    soul_transition_snapshot_taken();
    CHECK(!soul_transition_wants_snapshot());
}

static void test_leaving_is_two_black_frames_then_black_spokes(void)
{
    int tick;

    start_with_spoke_art();
    soul_transition_begin_leaving();
    for (tick = 1; tick <= LEAVING_LAST_TICK; tick++) {
        soul_transition_advance();
        CHECK_INT(draws_at_this_tick(), 1);
        CHECK(fake_renderer_draw(0)->color.red == 0.0f);
        CHECK_INT(fake_renderer_draw(0)->is_filled_rect, tick <= 2);
    }
    soul_transition_advance();
    CHECK_INT(draws_at_this_tick(), 0);
    CHECK(!soul_transition_wants_snapshot());
}

int main(void)
{
    test_entering_shows_spokes_then_a_dark_frame_then_spun_layers();
    test_spun_layers_stay_away_without_a_snapshot();
    test_a_late_frame_still_takes_the_snapshot();
    test_leaving_is_two_black_frames_then_black_spokes();
    return test_finish("unit/soul_transition");
}
