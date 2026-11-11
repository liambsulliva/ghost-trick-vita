#include <string.h>
#include "ds_units.h"
#include "script_machine.h"
#include "test_support.h"

#define STEPS_TO_SETTLE 120
#define NORMAL_PLAY_MODE 3
#define DEMO_MODE 2

static struct script_machine machine;
static struct script_outputs outputs;
static struct script_hud_view last_view;
static int swirl_reports;

static void on_hud_changed(const struct script_hud_view *view)
{
    last_view = *view;
}

static void on_swirl_changed(int is_on)
{
    (void)is_on;
    swirl_reports = swirl_reports + 1;
}

static void settle(void)
{
    int step;

    for (step = 0; step < STEPS_TO_SETTLE; step++) {
        script_pills_step(&machine);
    }
}

static void start_play_with_pills_up(void)
{
    memset(&outputs, 0, sizeof(outputs));
    outputs.hud_changed = on_hud_changed;
    outputs.swirl_changed = on_swirl_changed;
    swirl_reports = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_pills_register(&machine, SCRIPT_PILL_GHOST, 1);
    script_pills_register(&machine, SCRIPT_PILL_TRICK, 1);
    settle();
}

static void check_the_ui_is_away(void)
{
    settle();
    CHECK(script_machine_picture_is_hidden(&machine));
    CHECK(!script_pills_any_on_screen(&machine));
    script_soul_step_before_tasks(&machine);
    CHECK(last_view.picture_is_hidden);
}

static void check_the_ui_is_back(void)
{
    settle();
    CHECK(!script_machine_picture_is_hidden(&machine));
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    script_soul_step_before_tasks(&machine);
    CHECK(!last_view.picture_is_hidden);
}

static void test_the_pills_start_up(void)
{
    start_play_with_pills_up();
    check_the_ui_is_back();
}

static void test_a_swirl_takes_the_ui_away_at_once(void)
{
    start_play_with_pills_up();
    script_machine_set_swirl(&machine, 1);
    CHECK(swirl_reports == 1);
    CHECK(!script_pills_any_on_screen(&machine));
    check_the_ui_is_away();
    script_machine_set_swirl(&machine, 0);
    CHECK(swirl_reports == 2);
    check_the_ui_is_back();
}

static void test_a_film_strip_takes_the_ui_away(void)
{
    start_play_with_pills_up();
    machine.film_frame_is_on = 1;
    check_the_ui_is_away();
    machine.film_frame_is_on = 0;
    check_the_ui_is_back();
}

static void test_narration_takes_the_ui_away(void)
{
    start_play_with_pills_up();
    machine.narration.is_running = 1;
    check_the_ui_is_away();
    machine.narration.is_running = 0;
    check_the_ui_is_back();
}

static void test_a_black_screen_takes_the_ui_away(void)
{
    start_play_with_pills_up();
    machine.reported_brightness = -NATIVE_BRIGHTNESS_SCALE;
    check_the_ui_is_away();
    machine.reported_brightness = -NATIVE_BRIGHTNESS_SCALE + 1;
    check_the_ui_is_back();
}

static void test_demo_mode_offers_the_ghost_pill_only_while_a_trick_plays_out(void)
{
    start_play_with_pills_up();
    machine.mode_byte = DEMO_MODE;
    settle();
    CHECK(!script_pills_any_on_screen(&machine));
    machine.soul.a_trick_is_playing_out = 1;
    settle();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    machine.soul.a_trick_is_playing_out = 0;
    settle();
    CHECK(!script_pills_any_on_screen(&machine));
}

static void test_the_end_of_demo_mode_ends_the_trick_playing_out(void)
{
    start_play_with_pills_up();
    machine.mode_byte = DEMO_MODE;
    machine.soul.a_trick_is_playing_out = 1;
    settle();
    machine.mode_byte = NORMAL_PLAY_MODE;
    settle();
    CHECK(!machine.soul.a_trick_is_playing_out);
    machine.mode_byte = DEMO_MODE;
    settle();
    CHECK(!script_pills_any_on_screen(&machine));
}

int main(void)
{
    test_the_pills_start_up();
    test_a_swirl_takes_the_ui_away_at_once();
    test_a_film_strip_takes_the_ui_away();
    test_narration_takes_the_ui_away();
    test_a_black_screen_takes_the_ui_away();
    test_demo_mode_offers_the_ghost_pill_only_while_a_trick_plays_out();
    test_the_end_of_demo_mode_ends_the_trick_playing_out();
    return test_finish("unit/ui_off_hidden_picture");
}
