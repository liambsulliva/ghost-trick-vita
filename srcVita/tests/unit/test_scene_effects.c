#include "animation.h"
#include "fakes/fake_platform.h"
#include "run_log.h"
#include "scene_effects.h"
#include "test_support.h"

static struct scene_effects effects;

static int nearly(float actual, float expected)
{
    float difference;

    difference = actual - expected;
    if (difference < 0.0f) {
        difference = -difference;
    }
    return difference < 0.01f;
}

static void advance_ticks(int ticks)
{
    scene_effects_advance(&effects, (unsigned long long)ticks * MICROSECONDS_PER_TICK);
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static void set_up(void)
{
    fake_files_reset();
    run_log_open("run.log");
    scene_effects_reset(&effects);
}

static void test_a_fresh_set_of_effects_shows_nothing(void)
{
    set_up();
    CHECK(!effects.mask_is_on);
    CHECK(!effects.iris_is_on);
    CHECK(effects.brightness.value == 1.0f);
    CHECK(effects.flash_level.value == 0.0f);
    CHECK(!effects.beams[0].in_use);
}

static void test_a_new_beam_holds_its_shape_until_animated(void)
{
    const struct scene_beam *beam;

    set_up();
    scene_effects_create_beam(&effects, "left", 56.0f, 128.0f, 58.0f, 367.0f, 3.0f);
    beam = &effects.beams[0];
    CHECK(beam->in_use);
    CHECK_TEXT(beam->name, "left");
    CHECK(beam->apex_x == 56.0f);
    CHECK(beam->apex_y == 128.0f);
    CHECK(beam->base_x.value == 58.0f);
    CHECK(beam->base_y.value == 367.0f);
    CHECK(beam->half_width.value == 3.0f);
    advance_ticks(100);
    CHECK(beam->base_x.value == 58.0f);
    CHECK(beam->half_width.value == 3.0f);
}

static void test_an_animated_beam_moves_in_a_straight_line_and_stops_at_its_target(void)
{
    const struct scene_beam *beam;

    set_up();
    scene_effects_create_beam(&effects, "left", 56.0f, 128.0f, 56.0f, 300.0f, 0.0f);
    beam = &effects.beams[0];
    scene_effects_animate_beam(&effects, "left", 116.0f, 360.0f, 30.0f, 60);
    CHECK(beam->base_x.value == 56.0f);
    advance_ticks(15);
    CHECK(nearly(beam->base_x.value, 71.0f));
    CHECK(nearly(beam->base_y.value, 315.0f));
    CHECK(nearly(beam->half_width.value, 7.5f));
    advance_ticks(30);
    CHECK(nearly(beam->base_x.value, 101.0f));
    advance_ticks(14);
    CHECK(beam->base_x.value < 116.0f);
    advance_ticks(1);
    CHECK(beam->base_x.value == 116.0f);
    CHECK(beam->base_y.value == 360.0f);
    CHECK(beam->half_width.value == 30.0f);
    advance_ticks(100);
    CHECK(beam->base_x.value == 116.0f);
    CHECK(beam->apex_x == 56.0f);
}

static void test_animating_mid_flight_continues_from_where_the_beam_is(void)
{
    const struct scene_beam *beam;

    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    beam = &effects.beams[0];
    scene_effects_animate_beam(&effects, "left", 100.0f, 100.0f, 0.0f, 10);
    advance_ticks(5);
    scene_effects_animate_beam(&effects, "left", 0.0f, 100.0f, 0.0f, 10);
    CHECK(nearly(beam->base_x.value, 50.0f));
    advance_ticks(5);
    CHECK(nearly(beam->base_x.value, 25.0f));
    advance_ticks(5);
    CHECK(beam->base_x.value == 0.0f);
}

static void test_a_change_of_no_ticks_happens_at_once(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_animate_beam(&effects, "left", 10.0f, 20.0f, 30.0f, 0);
    CHECK(effects.beams[0].base_x.value == 10.0f);
    CHECK(effects.beams[0].half_width.value == 30.0f);
    scene_effects_dim_to(&effects, 0.5f, 0);
    CHECK(effects.brightness.value == 0.5f);
    advance_ticks(1);
    CHECK(effects.brightness.value == 0.5f);
}

static void test_each_beam_runs_on_its_own_timer(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_create_beam(&effects, "right", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_animate_beam(&effects, "left", 0.0f, 100.0f, 30.0f, 10);
    advance_ticks(5);
    scene_effects_animate_beam(&effects, "right", 0.0f, 100.0f, 30.0f, 10);
    advance_ticks(5);
    CHECK(effects.beams[0].half_width.value == 30.0f);
    CHECK(nearly(effects.beams[1].half_width.value, 15.0f));
}

static void test_deleting_a_beam_frees_its_place_and_stopped_beams_stay_stopped(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_create_beam(&effects, "right", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_delete_beam(&effects, "left");
    CHECK(!effects.beams[0].in_use);
    CHECK(effects.beams[1].in_use);
    scene_effects_delete_beam(&effects, "nobody");
    CHECK(effects.beams[1].in_use);
    scene_effects_create_beam(&effects, "other", 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    CHECK_TEXT(effects.beams[0].name, "other");
}

static void test_creating_a_beam_with_a_used_name_replaces_it(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_animate_beam(&effects, "left", 50.0f, 100.0f, 0.0f, 100);
    advance_ticks(10);
    scene_effects_create_beam(&effects, "left", 7.0f, 0.0f, 9.0f, 100.0f, 2.0f);
    advance_ticks(10);
    CHECK(effects.beams[0].apex_x == 7.0f);
    CHECK(effects.beams[0].base_x.value == 9.0f);
    CHECK(!effects.beams[1].in_use);
}

static void test_too_many_beams_are_refused_and_logged(void)
{
    char name[16];
    int index;

    set_up();
    for (index = 0; index <= SCENE_BEAM_MAX; index++) {
        snprintf(name, sizeof(name), "b%d", index);
        scene_effects_create_beam(&effects, name, 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    }
    CHECK_HAS_TEXT(log_text(), "scene too_many_beams name=b4");
    CHECK_TEXT(effects.beams[SCENE_BEAM_MAX - 1].name, "b3");
}

static void test_animating_a_missing_beam_is_logged_and_harmless(void)
{
    set_up();
    scene_effects_animate_beam(&effects, "nobody", 0.0f, 0.0f, 0.0f, 10);
    CHECK_HAS_TEXT(log_text(), "scene beam_missing name=nobody");
}

static void test_animating_a_deleted_beam_is_logged_and_does_not_bring_it_back(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_delete_beam(&effects, "left");
    scene_effects_animate_beam(&effects, "left", 0.0f, 100.0f, 30.0f, 10);
    CHECK_HAS_TEXT(log_text(), "scene beam_missing name=left");
    CHECK(!effects.beams[0].in_use);
}

static void test_dimming_moves_brightness_over_its_ticks_and_back(void)
{
    set_up();
    scene_effects_dim_to(&effects, 0.5f, 10);
    CHECK(effects.brightness.value == 1.0f);
    advance_ticks(5);
    CHECK(nearly(effects.brightness.value, 0.75f));
    advance_ticks(5);
    CHECK(effects.brightness.value == 0.5f);
    scene_effects_dim_to(&effects, 1.0f, 10);
    advance_ticks(5);
    CHECK(nearly(effects.brightness.value, 0.75f));
    advance_ticks(5);
    CHECK(effects.brightness.value == 1.0f);
}

static void test_a_flash_starts_at_its_brightness_and_falls_to_nothing(void)
{
    set_up();
    scene_effects_flash(&effects, 0.5f, 8);
    CHECK(effects.flash_level.value == 0.5f);
    advance_ticks(4);
    CHECK(nearly(effects.flash_level.value, 0.25f));
    advance_ticks(4);
    CHECK(effects.flash_level.value == 0.0f);
    scene_effects_flash(&effects, 1.0f, 8);
    CHECK(effects.flash_level.value == 1.0f);
    scene_effects_flash(&effects, 0.5f, 8);
    advance_ticks(4);
    CHECK(nearly(effects.flash_level.value, 0.25f));
}

static void test_the_iris_opens_from_nothing(void)
{
    set_up();
    scene_effects_set_mask(&effects, 1);
    scene_effects_start_iris(&effects, 144.0f, 359.0f, 384.0f, 120);
    CHECK(effects.iris_is_on);
    CHECK(effects.iris_x.value == 144.0f);
    CHECK(effects.iris_y.value == 359.0f);
    CHECK(effects.iris_radius.value == 0.0f);
    advance_ticks(60);
    CHECK(nearly(effects.iris_radius.value, 192.0f));
    scene_effects_start_iris(&effects, 144.0f, 359.0f, 0.0f, 60);
    CHECK(nearly(effects.iris_radius.value, 192.0f));
    advance_ticks(60);
    CHECK(effects.iris_radius.value == 0.0f);
}

static void test_a_dissolve_runs_from_nothing_to_all_and_then_turns_itself_off(void)
{
    set_up();
    CHECK(!effects.dissolve_is_on);
    scene_effects_start_dissolve(&effects, 2.0f, 32);
    CHECK(effects.dissolve_is_on);
    CHECK(effects.dissolve_end_zoom == 2.0f);
    CHECK(effects.dissolve_progress.value == 0.0f);
    advance_ticks(16);
    CHECK(effects.dissolve_is_on);
    CHECK(nearly(effects.dissolve_progress.value, 0.5f));
    advance_ticks(15);
    CHECK(effects.dissolve_is_on);
    advance_ticks(1);
    CHECK(!effects.dissolve_is_on);
    advance_ticks(100);
    CHECK(!effects.dissolve_is_on);
}

static void test_a_new_dissolve_starts_over_from_nothing(void)
{
    set_up();
    scene_effects_start_dissolve(&effects, 2.0f, 32);
    advance_ticks(16);
    scene_effects_start_dissolve(&effects, 3.0f, 8);
    CHECK(effects.dissolve_progress.value == 0.0f);
    CHECK(effects.dissolve_end_zoom == 3.0f);
    advance_ticks(4);
    CHECK(nearly(effects.dissolve_progress.value, 0.5f));
}

static void test_resetting_the_effects_ends_a_dissolve(void)
{
    set_up();
    scene_effects_start_dissolve(&effects, 2.0f, 32);
    advance_ticks(8);
    scene_effects_reset(&effects);
    CHECK(!effects.dissolve_is_on);
    CHECK(effects.dissolve_progress.value == 0.0f);
}

static void test_turning_the_mask_on_or_off_ends_the_iris(void)
{
    set_up();
    scene_effects_start_iris(&effects, 0.0f, 0.0f, 100.0f, 10);
    advance_ticks(10);
    scene_effects_set_mask(&effects, 0);
    CHECK(!effects.iris_is_on);
    CHECK(!effects.mask_is_on);
    scene_effects_start_iris(&effects, 0.0f, 0.0f, 100.0f, 10);
    CHECK(effects.iris_radius.value == 0.0f);
    scene_effects_set_mask(&effects, 1);
    CHECK(!effects.iris_is_on);
    CHECK(effects.mask_is_on);
}

static void test_effects_run_side_by_side(void)
{
    set_up();
    scene_effects_create_beam(&effects, "left", 0.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    scene_effects_animate_beam(&effects, "left", 0.0f, 100.0f, 30.0f, 10);
    scene_effects_dim_to(&effects, 0.0f, 10);
    scene_effects_flash(&effects, 1.0f, 10);
    scene_effects_start_iris(&effects, 0.0f, 0.0f, 100.0f, 10);
    advance_ticks(5);
    CHECK(nearly(effects.beams[0].half_width.value, 15.0f));
    CHECK(nearly(effects.brightness.value, 0.5f));
    CHECK(nearly(effects.flash_level.value, 0.5f));
    CHECK(nearly(effects.iris_radius.value, 50.0f));
}

int main(void)
{
    test_a_fresh_set_of_effects_shows_nothing();
    test_a_new_beam_holds_its_shape_until_animated();
    test_an_animated_beam_moves_in_a_straight_line_and_stops_at_its_target();
    test_animating_mid_flight_continues_from_where_the_beam_is();
    test_a_change_of_no_ticks_happens_at_once();
    test_each_beam_runs_on_its_own_timer();
    test_deleting_a_beam_frees_its_place_and_stopped_beams_stay_stopped();
    test_creating_a_beam_with_a_used_name_replaces_it();
    test_too_many_beams_are_refused_and_logged();
    test_animating_a_missing_beam_is_logged_and_harmless();
    test_animating_a_deleted_beam_is_logged_and_does_not_bring_it_back();
    test_dimming_moves_brightness_over_its_ticks_and_back();
    test_a_flash_starts_at_its_brightness_and_falls_to_nothing();
    test_the_iris_opens_from_nothing();
    test_turning_the_mask_on_or_off_ends_the_iris();
    test_a_dissolve_runs_from_nothing_to_all_and_then_turns_itself_off();
    test_a_new_dissolve_starts_over_from_nothing();
    test_resetting_the_effects_ends_a_dissolve();
    test_effects_run_side_by_side();
    fake_files_reset();
    return test_finish("unit/scene_effects");
}
