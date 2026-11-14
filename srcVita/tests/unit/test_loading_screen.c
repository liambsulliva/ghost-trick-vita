#include <math.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "loading_screen.h"
#include "pause_menu.h"
#include "pause_widgets.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRAME_US 16666ull
#define WALK_US 25000ull
#define CHECK_NEAR(actual, expected) CHECK(fabsf((actual) - (expected)) < 0.001f)

static const char widgets[] =
    "text bar.loading 0 0 120 30\n"
    "slice loading_track 8 0 30 24 16\n"
    "slice loading_fill 5 64 30 16 10\n";

static const char walk_table[] =
    "pages 1\n"
    "frame t_walk 0 2 0 0 0 100 80 -40 -70\n"
    "frame t_walk 1 2 0 102 0 100 80 -30 -72\n";

static const char walk_track[] =
    "idle 0\n"
    "stride 60.000\n"
    "walk 0\nwalk 0\nwalk 1\nwalk 1\n";

#define UNEVEN_DRAWINGS 4
#define UNEVEN_WIDTH 50.0f
#define UNEVEN_PACE 7.5f

static const char uneven_table[] =
    "pages 1\n"
    "frame t_walk 0 2 0 0 0 50 80 -40 -70\n"
    "frame t_walk 1 2 0 52 0 50 80 -39 -70\n"
    "frame t_walk 2 2 0 104 0 50 80 -10 -70\n"
    "frame t_walk 3 2 0 156 0 50 80 14 -70\n";

static const char uneven_track[] =
    "idle 0\n"
    "stride 60.000\n"
    "pace 7.500\n"
    "walk 0 0.000\nwalk 0 7.500\nwalk 1 14.000\nwalk 1 21.500\n"
    "walk 2 0.000\nwalk 2 7.500\nwalk 3 -9.000\nwalk 3 -1.500\n";

static const unsigned long long uneven_frames_us[] = { 16666ull, 33333ull, 20000ull, 41000ull, 16667ull, 50000ull, 25000ull };

static struct loading_counts counts_of(enum loading_stage stage, int done, int total)
{
    struct loading_counts counts;

    counts.stage = stage;
    counts.done = done;
    counts.total = total;
    return counts;
}

static float progress_of(enum loading_stage stage, int done, int total)
{
    struct loading_counts counts;

    counts = counts_of(stage, done, total);
    return loading_screen_progress(&counts);
}

static void start_screen(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/pause_backdrop.gtex", 64, 32);
    fake_files_add_rgba_texture("tex/pause_widgets.gtex", 128, 64);
    texture_cache_init("tex", 1u << 20);
    CHECK(texture_cache_load_now("pause_backdrop")->state == CACHED_TEXTURE_READY);
    CHECK(texture_cache_load_now("pause_widgets")->state == CACHED_TEXTURE_READY);
    CHECK(pause_widgets_read_text(widgets, (unsigned int)strlen(widgets)) > 0);
    loading_screen_begin();
}

static float widest_fill(void)
{
    const struct fake_draw *draw;
    float widest;
    int index;

    widest = 0.0f;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && draw->screen_area.y > (float)RENDERER_HEIGHT - PAUSE_BAR_HEIGHT && draw->texture_area.x >= 0.5f && draw->screen_area.x + draw->screen_area.width > widest) {
            widest = draw->screen_area.x + draw->screen_area.width;
        }
    }
    return widest;
}

static void test_each_load_stage_fills_its_share_of_the_bar(void)
{
    CHECK_NEAR(progress_of(LOADING_READING, 0, 20), 0.0f);
    CHECK_NEAR(progress_of(LOADING_READING, 10, 20), 0.1f);
    CHECK_NEAR(progress_of(LOADING_ANIMATIONS, 0, 0), 0.2f);
    CHECK_NEAR(progress_of(LOADING_ANIMATIONS, 30, 30), 0.5f);
    CHECK_NEAR(progress_of(LOADING_TEXTURES, 5, 10), 0.75f);
    CHECK_NEAR(progress_of(LOADING_TEXTURES, 12, 10), 1.0f);
    CHECK_NEAR(progress_of(LOADING_DONE, 0, 0), 1.0f);
}

static void test_the_bar_eases_forward_and_never_goes_back(void)
{
    struct loading_counts counts;
    float last;
    int frame;

    start_screen();
    counts = counts_of(LOADING_TEXTURES, 10, 10);
    loading_screen_update(&counts, FRAME_US);
    CHECK(loading_screen_shown_progress() > 0.0f);
    CHECK(loading_screen_shown_progress() < 0.1f);
    last = loading_screen_shown_progress();
    counts = counts_of(LOADING_READING, 0, 10);
    for (frame = 0; frame < 120; frame++) {
        loading_screen_update(&counts, FRAME_US);
        CHECK(loading_screen_shown_progress() >= last);
        last = loading_screen_shown_progress();
    }
    CHECK_NEAR(loading_screen_shown_progress(), 1.0f);
}

static void test_the_screen_draws_a_fill_that_grows_with_progress(void)
{
    struct loading_counts counts;
    float half;

    start_screen();
    counts = counts_of(LOADING_ANIMATIONS, 0, 10);
    loading_screen_update(&counts, 1000000ull);
    fake_renderer_clear_draws();
    loading_screen_draw();
    half = widest_fill();
    CHECK(half > 300.0f);
    counts = counts_of(LOADING_DONE, 0, 0);
    loading_screen_update(&counts, 1000000ull);
    fake_renderer_clear_draws();
    loading_screen_draw();
    CHECK(widest_fill() > half);
    CHECK(widest_fill() <= (float)RENDERER_WIDTH);
}

static void test_the_warm_up_draws_every_kind_of_primitive_unseen(void)
{
    int index;

    start_screen();
    fake_renderer_clear_draws();
    loading_screen_warm_up_the_renderer();
    CHECK_INT(fake_renderer_draw_count(), 5);
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        CHECK_NEAR(fake_renderer_draw(index)->color.alpha, 0.0f);
    }
}

static int cat_draws_at(float x)
{
    const struct fake_draw *draw;
    int found;
    int index;

    found = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && fabsf(draw->screen_area.x - x) < 0.5f && fabsf(draw->screen_area.width - 100.0f) < 0.01f) {
            found = found + 1;
        }
    }
    return found;
}

static void test_the_cat_walks_in_from_the_left_out_to_the_right_and_comes_round_again(void)
{
    struct loading_counts counts;
    float start;
    float last;
    float farthest;
    int came_round;
    int frame;

    start_screen();
    fake_files_add_rgba_texture("tex/pause_cat_00.gtex", 256, 128);
    CHECK(texture_cache_load_now("pause_cat_00")->state == CACHED_TEXTURE_READY);
    CHECK_INT(pause_menu_read_cat_text(walk_table, (unsigned int)strlen(walk_table)), 2);
    CHECK_INT(pause_menu_read_track_text(walk_track, (unsigned int)strlen(walk_track)), 1);
    loading_screen_begin();
    start = loading_screen_cat_feet_x();
    CHECK_NEAR(start, -60.0f);
    counts = counts_of(LOADING_TEXTURES, 0, 10);
    for (frame = 0; frame < 12; frame++) {
        loading_screen_update(&counts, WALK_US);
    }
    CHECK_NEAR(loading_screen_cat_feet_x(), start + 3.0f * 60.0f);
    fake_renderer_clear_draws();
    loading_screen_draw();
    CHECK_INT(cat_draws_at(start + 3.0f * 60.0f - 40.0f), 1);
    loading_screen_begin();
    last = start;
    farthest = start;
    came_round = 0;
    for (frame = 0; frame < 400 && !came_round; frame++) {
        loading_screen_update(&counts, FRAME_US + 1ull);
        if (loading_screen_cat_feet_x() < last) {
            came_round = 1;
            CHECK_NEAR(loading_screen_cat_feet_x(), start);
        }
        last = loading_screen_cat_feet_x();
        farthest = last > farthest ? last : farthest;
    }
    CHECK(came_round);
    CHECK(farthest - 40.0f >= (float)RENDERER_WIDTH - 60.0f);
    CHECK(farthest - 30.0f < (float)RENDERER_WIDTH);
    CHECK(pause_menu_read_track_text("idle 0\n", 7u) == 1);
}

static void start_walking(void)
{
    start_screen();
    fake_files_add_rgba_texture("tex/pause_cat_00.gtex", 256, 128);
    CHECK(texture_cache_load_now("pause_cat_00")->state == CACHED_TEXTURE_READY);
    CHECK_INT(pause_menu_read_cat_text(walk_table, (unsigned int)strlen(walk_table)), 2);
    CHECK_INT(pause_menu_read_track_text(walk_track, (unsigned int)strlen(walk_track)), 1);
    loading_screen_begin();
}

static void test_the_cat_walks_at_two_thirds_of_the_game_pace(void)
{
    struct loading_counts counts;
    float start;
    int frame;

    start_walking();
    start = loading_screen_cat_feet_x();
    counts = counts_of(LOADING_TEXTURES, 0, 10);
    for (frame = 0; frame < 60; frame++) {
        loading_screen_update(&counts, FRAME_US + 1ull);
    }
    CHECK_NEAR(loading_screen_cat_feet_x(), start + 10.0f * 60.0f);
}

static void test_a_long_frame_moves_the_cat_one_drawing_at_most(void)
{
    struct loading_counts counts;
    float start;

    start_walking();
    start = loading_screen_cat_feet_x();
    counts = counts_of(LOADING_TEXTURES, 0, 10);
    loading_screen_update(&counts, 500000ull);
    CHECK_NEAR(loading_screen_cat_feet_x(), start);
    loading_screen_update(&counts, 500000ull);
    CHECK_NEAR(loading_screen_cat_feet_x(), start + 60.0f);
}

static int drawn_body_x(float *body_x)
{
    const struct fake_draw *draw;
    int index;
    int drawing;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && fabsf(draw->screen_area.width - UNEVEN_WIDTH) < 0.01f) {
            drawing = (int)(draw->texture_area.x * 256.0f / 52.0f + 0.5f);
            *body_x = draw->screen_area.x + UNEVEN_WIDTH / 2.0f;
            return drawing >= 0 && drawing < UNEVEN_DRAWINGS && fabsf(draw->screen_area.x - floorf(draw->screen_area.x)) < 0.001f;
        }
    }
    return 0;
}

static void test_the_cat_s_body_moves_on_evenly_whatever_each_drawing_carries(void)
{
    struct loading_counts counts;
    unsigned long long elapsed_us;
    float last;
    float body_x;
    float moved;
    int came_round;
    int compared;
    int frame;

    start_screen();
    fake_files_add_rgba_texture("tex/pause_cat_00.gtex", 256, 128);
    CHECK(texture_cache_load_now("pause_cat_00")->state == CACHED_TEXTURE_READY);
    CHECK_INT(pause_menu_read_cat_text(uneven_table, (unsigned int)strlen(uneven_table)), UNEVEN_DRAWINGS);
    CHECK_INT(pause_menu_read_track_text(uneven_track, (unsigned int)strlen(uneven_track)), 1);
    loading_screen_begin();
    counts = counts_of(LOADING_TEXTURES, 0, 10);
    loading_screen_update(&counts, 1000000ull);
    fake_renderer_clear_draws();
    loading_screen_draw();
    CHECK(drawn_body_x(&last));
    came_round = 0;
    compared = 0;
    for (frame = 0; frame < 600; frame++) {
        elapsed_us = uneven_frames_us[frame % (int)(sizeof(uneven_frames_us) / sizeof(uneven_frames_us[0]))];
        loading_screen_update(&counts, elapsed_us);
        fake_renderer_clear_draws();
        loading_screen_draw();
        CHECK(drawn_body_x(&body_x));
        moved = body_x - last;
        last = body_x;
        if (moved < 0.0f) {
            came_round = came_round + 1;
            continue;
        }
        CHECK(fabsf(moved - UNEVEN_PACE * (float)elapsed_us / (float)WALK_US) < 1.01f);
        compared = compared + 1;
    }
    CHECK(came_round >= 2);
    CHECK(compared > 590);
    CHECK(pause_menu_read_track_text("idle 0\n", 7u) == 1);
}

static void test_a_faint_glow_in_the_pause_circle_reds_rises_from_the_floor_behind_the_cat(void)
{
    const struct fake_draw *draw;
    struct loading_counts counts;
    float floor_top;
    float covered;
    float last_alpha;
    float last_red;
    float last_top;
    int index;
    int strips;
    int backdrop_at;
    int first_strip_at;

    start_screen();
    counts = counts_of(LOADING_READING, 0, 10);
    loading_screen_update(&counts, 1000000ull);
    fake_renderer_clear_draws();
    loading_screen_draw();
    floor_top = (float)RENDERER_HEIGHT - PAUSE_BAR_HEIGHT;
    strips = 0;
    covered = 0.0f;
    last_alpha = 1.0f;
    last_red = 1.0f;
    last_top = floor_top;
    backdrop_at = -1;
    first_strip_at = -1;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && backdrop_at < 0) {
            backdrop_at = index;
        }
        if (!draw->is_filled_rect || draw->screen_area.y >= floor_top || draw->screen_area.y + draw->screen_area.height > floor_top || draw->color.alpha >= 1.0f) {
            continue;
        }
        if (first_strip_at < 0) {
            first_strip_at = index;
            CHECK(draw->color.red > 0.95f);
        }
        CHECK_NEAR(draw->screen_area.x, 0.0f);
        CHECK_NEAR(draw->screen_area.width, (float)RENDERER_WIDTH);
        CHECK_NEAR(draw->screen_area.y + draw->screen_area.height, last_top);
        CHECK(draw->color.alpha <= 0.35f && draw->color.alpha < last_alpha);
        CHECK(draw->color.red <= last_red && draw->color.red >= 74.0f / 255.0f - 0.001f);
        CHECK_NEAR(draw->color.green, 0.0f);
        CHECK_NEAR(draw->color.blue, 0.0f);
        last_alpha = draw->color.alpha;
        last_red = draw->color.red;
        last_top = draw->screen_area.y;
        covered = covered + draw->screen_area.height;
        strips = strips + 1;
    }
    CHECK(strips > 16);
    CHECK_NEAR(covered, 256.0f);
    CHECK(last_alpha < 0.01f);
    CHECK(last_red < 80.0f / 255.0f);
    CHECK(backdrop_at >= 0 && backdrop_at < first_strip_at);
}

int main(void)
{
    test_each_load_stage_fills_its_share_of_the_bar();
    test_the_bar_eases_forward_and_never_goes_back();
    test_the_screen_draws_a_fill_that_grows_with_progress();
    test_the_warm_up_draws_every_kind_of_primitive_unseen();
    test_the_cat_walks_in_from_the_left_out_to_the_right_and_comes_round_again();
    test_the_cat_walks_at_two_thirds_of_the_game_pace();
    test_a_long_frame_moves_the_cat_one_drawing_at_most();
    test_the_cat_s_body_moves_on_evenly_whatever_each_drawing_carries();
    test_a_faint_glow_in_the_pause_circle_reds_rises_from_the_floor_behind_the_cat();
    return test_finish("unit/loading_screen");
}
