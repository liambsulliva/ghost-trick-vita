#include <math.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "loading_screen.h"
#include "pause_widgets.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRAME_US 16666ull
#define CHECK_NEAR(actual, expected) CHECK(fabsf((actual) - (expected)) < 0.001f)

static const char widgets[] =
    "text bar.loading 0 0 120 30\n"
    "slice loading_track 8 0 30 24 16\n"
    "slice loading_fill 5 64 30 16 10\n";

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

int main(void)
{
    test_each_load_stage_fills_its_share_of_the_bar();
    test_the_bar_eases_forward_and_never_goes_back();
    test_the_screen_draws_a_fill_that_grows_with_progress();
    test_the_warm_up_draws_every_kind_of_primitive_unseen();
    return test_finish("unit/loading_screen");
}
