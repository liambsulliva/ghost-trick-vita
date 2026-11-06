#include "fakes/fake_platform.h"
#include "run_log.h"
#include "scene_layers.h"
#include "test_support.h"

#define MICROSECONDS_PER_TICK 16666ULL
#define SIXTY_TICKS_US 1000000ULL

static struct scene_layers layers;

static const struct scene_text_line *line_with_id(int id)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (layers.text_lines[index].in_use && layers.text_lines[index].id == id) {
            return &layers.text_lines[index];
        }
    }
    return NULL;
}

static const struct scene_overlay *overlay_with_id(int id)
{
    int index;

    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        if (layers.overlays[index].in_use && layers.overlays[index].id == id) {
            return &layers.overlays[index];
        }
    }
    return NULL;
}

static int is_about(float value, float wanted)
{
    float difference;

    difference = value - wanted;
    if (difference < 0.0f) {
        difference = -difference;
    }
    return difference < 0.001f;
}

static void test_a_shown_line_keeps_its_place_and_fades_in_on_its_timer(void)
{
    const struct scene_text_line *line;

    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 60);
    line = line_with_id(1);
    CHECK(line != NULL);
    CHECK_TEXT(line->texture_name, "line");
    CHECK(line->center_x == 480.0f);
    CHECK(line->top_y == 72.0f);
    CHECK(line->alpha == 0.0f);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 4);
    CHECK(is_about(line->alpha, 0.25f));
    scene_layers_advance(&layers, SIXTY_TICKS_US / 4);
    CHECK(is_about(line->alpha, 0.5f));
    scene_layers_advance(&layers, SIXTY_TICKS_US);
    CHECK(line->alpha == 1.0f);
    CHECK(line->in_use);
    scene_layers_advance(&layers, SIXTY_TICKS_US);
    CHECK(line->alpha == 1.0f);
}

static void test_a_line_shown_with_no_fade_is_at_full_alpha_at_once(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 0);
    CHECK(line_with_id(1)->alpha == 1.0f);
    scene_layers_advance(&layers, MICROSECONDS_PER_TICK);
    CHECK(line_with_id(1)->alpha == 1.0f);
}

static void test_a_hidden_line_fades_out_from_where_it_is_and_then_leaves(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 60);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    scene_layers_hide_text(&layers, 1, 60);
    CHECK(is_about(line_with_id(1)->alpha, 0.5f));
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    CHECK(is_about(line_with_id(1)->alpha, 0.25f));
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    CHECK(line_with_id(1) == NULL);
}

static void test_hiding_with_no_fade_removes_the_line_at_once(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 0);
    scene_layers_hide_text(&layers, 1, 0);
    CHECK(line_with_id(1) == NULL);
}

static void test_hiding_an_unknown_line_changes_nothing(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 0);
    scene_layers_hide_text(&layers, 9, 60);
    scene_layers_advance(&layers, SIXTY_TICKS_US * 2);
    CHECK(line_with_id(1) != NULL);
    CHECK(line_with_id(1)->alpha == 1.0f);
}

static void test_showing_an_id_again_replaces_that_line(void)
{
    const struct scene_text_line *line;

    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "old", 480.0f, 72.0f, 0);
    scene_layers_hide_text(&layers, 1, 60);
    scene_layers_show_text(&layers, 1, "new", 400.0f, 90.0f, 60);
    line = line_with_id(1);
    CHECK_TEXT(line->texture_name, "new");
    CHECK(line->center_x == 400.0f);
    CHECK(line->top_y == 90.0f);
    CHECK(line->alpha == 0.0f);
    scene_layers_advance(&layers, SIXTY_TICKS_US * 2);
    CHECK(line_with_id(1) != NULL);
    CHECK(line_with_id(1)->alpha == 1.0f);
}

static void test_lines_keep_their_own_timers(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "first", 480.0f, 72.0f, 60);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    scene_layers_show_text(&layers, 2, "second", 480.0f, 122.0f, 30);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 4);
    CHECK(is_about(line_with_id(1)->alpha, 0.75f));
    CHECK(is_about(line_with_id(2)->alpha, 0.5f));
}

static void test_hiding_all_text_removes_every_line_at_once_and_leaves_overlays(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "first", 480.0f, 72.0f, 60);
    scene_layers_show_text(&layers, 2, "second", 480.0f, 122.0f, 0);
    scene_layers_show_overlay(&layers, 1, "art", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_hide_all_text(&layers);
    CHECK(line_with_id(1) == NULL);
    CHECK(line_with_id(2) == NULL);
    CHECK(overlay_with_id(1) != NULL);
}

static void test_every_slot_can_hold_a_line_and_one_too_many_is_logged(void)
{
    unsigned int size;
    int id;

    fake_files_reset();
    run_log_open("run.log");
    scene_layers_clear(&layers);
    for (id = 1; id <= SCENE_TEXT_LINE_MAX; id++) {
        scene_layers_show_text(&layers, id, "line", 480.0f, 72.0f, 0);
    }
    for (id = 1; id <= SCENE_TEXT_LINE_MAX; id++) {
        CHECK(line_with_id(id) != NULL);
    }
    scene_layers_show_text(&layers, SCENE_TEXT_LINE_MAX + 1, "extra", 480.0f, 72.0f, 0);
    CHECK(line_with_id(SCENE_TEXT_LINE_MAX + 1) == NULL);
    CHECK_HAS_TEXT((const char *)fake_files_contents("run.log", &size), "scene too_many_text_lines name=extra");
}

static void test_an_overlay_grows_on_its_timer_and_stays_at_full_size(void)
{
    const struct scene_overlay *overlay;

    scene_layers_clear(&layers);
    scene_layers_show_overlay(&layers, 3, "art", 10.0f, 20.0f, 30.0f, 40.0f, 60);
    overlay = overlay_with_id(3);
    CHECK_TEXT(overlay->texture_name, "art");
    CHECK(overlay->x == 10.0f);
    CHECK(overlay->y == 20.0f);
    CHECK(overlay->width == 30.0f);
    CHECK(overlay->height == 40.0f);
    CHECK(overlay->size_fraction == 0.0f);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    CHECK(is_about(overlay->size_fraction, 0.5f));
    scene_layers_advance(&layers, SIXTY_TICKS_US);
    CHECK(overlay->size_fraction == 1.0f);
    scene_layers_advance(&layers, SIXTY_TICKS_US);
    CHECK(overlay->size_fraction == 1.0f);
    CHECK(overlay->in_use);
}

static void test_an_overlay_with_no_growth_is_at_full_size_at_once(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_overlay(&layers, 3, "art", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    CHECK(overlay_with_id(3)->size_fraction == 1.0f);
}

static void test_showing_an_overlay_id_again_replaces_it_and_restarts_its_growth(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_overlay(&layers, 3, "old", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_show_overlay(&layers, 3, "new", 1.0f, 2.0f, 8.0f, 8.0f, 60);
    CHECK_TEXT(overlay_with_id(3)->texture_name, "new");
    CHECK(overlay_with_id(3)->size_fraction == 0.0f);
}

static void test_showing_an_overlay_id_again_during_its_growth_starts_the_timer_over(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_overlay(&layers, 3, "art", 0.0f, 0.0f, 8.0f, 8.0f, 60);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    scene_layers_show_overlay(&layers, 3, "art", 0.0f, 0.0f, 8.0f, 8.0f, 60);
    CHECK(overlay_with_id(3)->size_fraction == 0.0f);
    scene_layers_advance(&layers, SIXTY_TICKS_US / 4);
    CHECK(is_about(overlay_with_id(3)->size_fraction, 0.25f));
}

static void test_hiding_an_overlay_removes_only_that_one(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_overlay(&layers, 1, "a", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_show_overlay(&layers, 2, "b", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_hide_overlay(&layers, 1);
    scene_layers_hide_overlay(&layers, 9);
    CHECK(overlay_with_id(1) == NULL);
    CHECK(overlay_with_id(2) != NULL);
}

static void test_every_overlay_slot_works_and_one_too_many_is_logged(void)
{
    unsigned int size;
    int id;

    fake_files_reset();
    run_log_open("run.log");
    scene_layers_clear(&layers);
    for (id = 1; id <= SCENE_OVERLAY_MAX; id++) {
        scene_layers_show_overlay(&layers, id, "a", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    }
    scene_layers_show_overlay(&layers, SCENE_OVERLAY_MAX + 1, "extra", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    CHECK(overlay_with_id(SCENE_OVERLAY_MAX) != NULL);
    CHECK(overlay_with_id(SCENE_OVERLAY_MAX + 1) == NULL);
    CHECK_HAS_TEXT((const char *)fake_files_contents("run.log", &size), "scene too_many_overlays name=extra");
}

static void test_clearing_overlays_leaves_text_and_clearing_everything_leaves_nothing(void)
{
    scene_layers_clear(&layers);
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 0);
    scene_layers_show_overlay(&layers, 1, "art", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_clear_overlays(&layers);
    CHECK(overlay_with_id(1) == NULL);
    CHECK(line_with_id(1) != NULL);
    scene_layers_show_overlay(&layers, 1, "art", 0.0f, 0.0f, 8.0f, 8.0f, 0);
    scene_layers_clear(&layers);
    CHECK(overlay_with_id(1) == NULL);
    CHECK(line_with_id(1) == NULL);
}

static void test_only_a_line_on_its_way_out_counts_as_fading_out(void)
{
    scene_layers_clear(&layers);
    CHECK(!scene_layers_text_is_fading_out(&layers));
    scene_layers_show_text(&layers, 1, "line", 480.0f, 72.0f, 60);
    CHECK(!scene_layers_text_is_fading_out(&layers));
    scene_layers_advance(&layers, SIXTY_TICKS_US);
    CHECK(!scene_layers_text_is_fading_out(&layers));
    scene_layers_show_text(&layers, 2, "other", 480.0f, 120.0f, 0);
    scene_layers_hide_text(&layers, 2, 60);
    CHECK(scene_layers_text_is_fading_out(&layers));
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    CHECK(scene_layers_text_is_fading_out(&layers));
    scene_layers_advance(&layers, SIXTY_TICKS_US / 2);
    CHECK(!scene_layers_text_is_fading_out(&layers));
    CHECK(line_with_id(2) == NULL);
    scene_layers_hide_text(&layers, 1, 60);
    CHECK(scene_layers_text_is_fading_out(&layers));
    scene_layers_hide_all_text(&layers);
    CHECK(!scene_layers_text_is_fading_out(&layers));
}

int main(void)
{
    test_only_a_line_on_its_way_out_counts_as_fading_out();
    test_a_shown_line_keeps_its_place_and_fades_in_on_its_timer();
    test_a_line_shown_with_no_fade_is_at_full_alpha_at_once();
    test_a_hidden_line_fades_out_from_where_it_is_and_then_leaves();
    test_hiding_with_no_fade_removes_the_line_at_once();
    test_hiding_an_unknown_line_changes_nothing();
    test_showing_an_id_again_replaces_that_line();
    test_lines_keep_their_own_timers();
    test_hiding_all_text_removes_every_line_at_once_and_leaves_overlays();
    test_every_slot_can_hold_a_line_and_one_too_many_is_logged();
    test_an_overlay_grows_on_its_timer_and_stays_at_full_size();
    test_an_overlay_with_no_growth_is_at_full_size_at_once();
    test_showing_an_overlay_id_again_replaces_it_and_restarts_its_growth();
    test_showing_an_overlay_id_again_during_its_growth_starts_the_timer_over();
    test_hiding_an_overlay_removes_only_that_one();
    test_every_overlay_slot_works_and_one_too_many_is_logged();
    test_clearing_overlays_leaves_text_and_clearing_everything_leaves_nothing();
    fake_files_reset();
    return test_finish("unit/scene_layers");
}
