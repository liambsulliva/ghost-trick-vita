#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define DEMO_MODE 2
#define LEFT_SLOT 0
#define RIGHT_SLOT 1
#define FOREVER 0xffff

static struct script_machine machine;
static struct script_outputs outputs;
static struct script_hud_view last_view;
static int closed_slots[SCRIPT_BALLOON_MAX];

static void on_hud_changed(const struct script_hud_view *view)
{
    last_view = *view;
}

static void on_balloon_closed(int slot)
{
    closed_slots[slot] = closed_slots[slot] + 1;
}

static void start_with_both_tap_balloons_open(int mode)
{
    memset(&outputs, 0, sizeof(outputs));
    memset(closed_slots, 0, sizeof(closed_slots));
    memset(&last_view, 0, sizeof(last_view));
    outputs.hud_changed = on_hud_changed;
    outputs.balloon_closed = on_balloon_closed;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = mode;
    machine.pause_bit = 0;
    machine.balloons[LEFT_SLOT].in_use = 1;
    machine.balloons[LEFT_SLOT].tap_group = SCRIPT_TAP_GROUP_LEFT;
    machine.balloons[LEFT_SLOT].timer = FOREVER;
    machine.balloons[RIGHT_SLOT].in_use = 1;
    machine.balloons[RIGHT_SLOT].tap_group = SCRIPT_TAP_GROUP_RIGHT;
    machine.balloons[RIGHT_SLOT].kind = SCRIPT_BALLOON_KIND_TAP_MARKS;
    machine.balloons[RIGHT_SLOT].timer = FOREVER;
    machine.balloons[RIGHT_SLOT].tap_target = "game";
    machine.balloons[RIGHT_SLOT].tap_verb = "EVENT";
    machine.balloons[RIGHT_SLOT].tap_arg = "tapped";
}

static void press_square_and_triangle(void)
{
    script_soul_note_tap(&machine, SCRIPT_TAP_GROUP_RIGHT);
    script_soul_step_before_tasks(&machine);
    script_soul_note_tap(&machine, SCRIPT_TAP_GROUP_LEFT);
    script_soul_step_before_tasks(&machine);
}

static void test_square_and_triangle_tap_the_balloons_in_normal_play(void)
{
    start_with_both_tap_balloons_open(NORMAL_PLAY_MODE);
    CHECK(script_balloons_are_live(&machine));
    CHECK_INT(script_soul_route_balloon_group(&machine, "EVENT:tapped"), SCRIPT_TAP_GROUP_RIGHT);
    press_square_and_triangle();
    CHECK(!machine.balloons[LEFT_SLOT].in_use);
    CHECK(!machine.balloons[RIGHT_SLOT].in_use);
}

static void test_a_scripted_event_keeps_the_balloons_out_of_reach(void)
{
    start_with_both_tap_balloons_open(DEMO_MODE);
    CHECK(!script_balloons_are_live(&machine));
    CHECK_INT(script_soul_route_balloon_group(&machine, "EVENT:tapped"), 0);
    press_square_and_triangle();
    CHECK(machine.balloons[LEFT_SLOT].in_use);
    CHECK(machine.balloons[RIGHT_SLOT].in_use);
    CHECK_INT(machine.soul.taps_taken, 0);
    CHECK(!last_view.tap_balloons_are_live);
}

static void test_the_balloons_come_back_when_the_player_has_control_again(void)
{
    start_with_both_tap_balloons_open(DEMO_MODE);
    script_soul_step_before_tasks(&machine);
    CHECK(!last_view.tap_balloons_are_live);
    machine.mode_byte = NORMAL_PLAY_MODE;
    script_soul_step_before_tasks(&machine);
    CHECK(last_view.tap_balloons_are_live);
    press_square_and_triangle();
    CHECK(!machine.balloons[RIGHT_SLOT].in_use);
}

static void test_a_swirl_keeps_the_balloons_out_of_reach_in_normal_play(void)
{
    start_with_both_tap_balloons_open(NORMAL_PLAY_MODE);
    script_machine_set_swirl(&machine, 1);
    CHECK(!script_balloons_are_live(&machine));
    press_square_and_triangle();
    CHECK(machine.balloons[RIGHT_SLOT].in_use);
}

static void test_a_tap_balloon_op_in_a_scripted_event_only_closes_the_one_there(void)
{
    unsigned int words[8];

    start_with_both_tap_balloons_open(DEMO_MODE);
    memset(words, 0, sizeof(words));
    words[0] = FOREVER;
    (void)script_ops_media_run(&machine, NULL, 0x46, words);
    CHECK(!machine.balloons[LEFT_SLOT].in_use);
    CHECK_INT(closed_slots[LEFT_SLOT], 1);
    CHECK(machine.balloons[RIGHT_SLOT].in_use);
}

int main(void)
{
    test_square_and_triangle_tap_the_balloons_in_normal_play();
    test_a_scripted_event_keeps_the_balloons_out_of_reach();
    test_the_balloons_come_back_when_the_player_has_control_again();
    test_a_swirl_keeps_the_balloons_out_of_reach_in_normal_play();
    test_a_tap_balloon_op_in_a_scripted_event_only_closes_the_one_there();
    return test_finish("unit/balloons_only_in_normal_play");
}
