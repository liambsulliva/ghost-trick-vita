#include <stdio.h>
#include "background_reader.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "scene_stage.h"
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
    soul_transition_draw(SCENE_GHOST_CICEL);
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

static int draw_is_sissel_red(const struct fake_draw *draw)
{
    return draw->color.red > 0.0f && draw->color.green == 0.0f && draw->color.blue == 0.0f;
}

static void test_missiles_view_draws_no_sissel_red(void)
{
    int index;
    int tick;

    start_with_spoke_art();
    soul_transition_begin_entering();
    for (tick = 1; tick <= SPOKE_TICKS; tick++) {
        soul_transition_advance();
        fake_renderer_clear_draws();
        soul_transition_draw(SCENE_GHOST_MISSILE);
        for (index = 0; index < fake_renderer_draw_count(); index++) {
            CHECK(!draw_is_sissel_red(fake_renderer_draw(index)));
            CHECK(fake_renderer_draw(index)->color.green > 0.0f);
        }
    }
    fake_renderer_clear_draws();
    soul_transition_draw_ghost_lines(SCENE_GHOST_MISSILE);
    CHECK_INT(fake_renderer_draw_count(), 2);
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        CHECK(!draw_is_sissel_red(fake_renderer_draw(index)));
    }
    CHECK(fake_renderer_draw(1)->is_filled_rect);
    CHECK(fake_renderer_draw(1)->color.green > 0.0f);
}

static void test_each_ghost_gets_its_own_line_texture(void)
{
    renderer_texture sissel_lines;
    renderer_texture missile_lines;

    start_with_spoke_art();
    soul_transition_release_textures();
    fake_renderer_clear_draws();
    soul_transition_draw_ghost_lines(SCENE_GHOST_CICEL);
    sissel_lines = fake_renderer_draw(0)->texture;
    CHECK(draw_is_sissel_red(fake_renderer_draw(1)));
    fake_renderer_clear_draws();
    soul_transition_draw_ghost_lines(SCENE_GHOST_MISSILE);
    missile_lines = fake_renderer_draw(0)->texture;
    CHECK(sissel_lines != missile_lines);
    soul_transition_release_textures();
}

static void test_a_time_move_rings_and_spokes_then_spins_the_capture(void)
{
    float radius;
    float half_width;
    float alpha;
    float last_radius;
    int tick;

    start_with_spoke_art();
    soul_transition_begin_time_move();
    last_radius = 0.0f;
    for (tick = 1; tick <= SPOKE_TICKS; tick++) {
        soul_transition_advance();
        if (tick < 3) {
            CHECK_INT(draws_at_this_tick(), 1);
            CHECK(!soul_transition_ring_at(tick, &radius, &half_width, &alpha));
        } else {
            CHECK_INT(draws_at_this_tick(), 2);
            CHECK(soul_transition_ring_at(tick, &radius, &half_width, &alpha));
            CHECK(fake_renderer_draw(0)->color.red > 0.99f && fake_renderer_draw(0)->color.green < 0.01f);
            CHECK(fake_renderer_draw(0)->color.alpha > 0.0f && fake_renderer_draw(0)->color.alpha <= 16.0f / 31.0f + 0.001f);
            CHECK(radius > last_radius);
            last_radius = radius;
        }
        if (soul_transition_wants_snapshot()) {
            CHECK_INT(tick, 11);
            soul_transition_snapshot_taken();
        }
    }
    CHECK(soul_transition_ring_at(3, &radius, &half_width, &alpha));
    CHECK(radius > 82.0f && radius < 89.0f);
    CHECK(half_width > 2.5f && half_width < 2.8f);
    CHECK(alpha > 1.9f / 31.0f && alpha < 2.1f / 31.0f);
    soul_transition_advance();
    CHECK_INT(draws_at_this_tick(), 0);
    for (tick = 14; tick <= ENTERING_LAST_TICK; tick++) {
        soul_transition_advance();
        CHECK_INT(draws_at_this_tick(), SPUN_LAYERS);
    }
    soul_transition_advance();
    CHECK_INT(draws_at_this_tick(), 0);
    soul_transition_release_textures();
}

int main(void)
{
    test_missiles_view_draws_no_sissel_red();
    test_each_ghost_gets_its_own_line_texture();
    test_entering_shows_spokes_then_a_dark_frame_then_spun_layers();
    test_spun_layers_stay_away_without_a_snapshot();
    test_a_late_frame_still_takes_the_snapshot();
    test_leaving_is_two_black_frames_then_black_spokes();
    test_a_time_move_rings_and_spokes_then_spins_the_capture();
    return test_finish("unit/soul_transition");
}
