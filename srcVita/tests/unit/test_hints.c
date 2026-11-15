#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define DELAY_TICKS 100
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define LAMP_X 600
#define ROOM_WIDTH 800
#define ROOM_HEIGHT 192
#define BEYOND_THE_ROOM 2000
#define VIEW_WIDTH 256
#define HOLD_TICKS 60
#define MOST_PAN_TICKS 45
#define FIRST_GATE_TRICK 3
#define LAMP_TRICK 6
#define RETRY_STEP 7
#define CHARACTER_STEP 10
#define TRICK_AFTER_A_BALLOON 12
#define BODY 0
#define GATE 1
#define LAMP 2
#define LEFT_SLOT 0
#define RIGHT_SLOT 1

struct step_row {
    const char *script;
    enum script_route_action action;
    const char *object;
    const char *wait;
};

static const struct step_row step_rows[] = {
    { "a", SCRIPT_ROUTE_GHOST_ON, "body", "act:game:act010&free" },
    { "a", SCRIPT_ROUTE_MOVE, "gate", "view&usable:gate" },
    { "a", SCRIPT_ROUTE_GHOST_OFF, "gate", "view&core:gate" },
    { "a", SCRIPT_ROUTE_TRICK, "gate", "core:gate&playable" },
    { "a", SCRIPT_ROUTE_GHOST_ON, "gate", "act:game:act020&free" },
    { "a", SCRIPT_ROUTE_MOVE, "lamp", "view&usable:lamp" },
    { "a", SCRIPT_ROUTE_TRICK, "lamp", "core:lamp&playable" },
    { "a", SCRIPT_ROUTE_WAIT, "-", "retried" },
    { "a", SCRIPT_ROUTE_TRICK, "body", "core:body&playable" },
    { "b", SCRIPT_ROUTE_GHOST_OFF, "body", "view&core:body" },
    { "b", SCRIPT_ROUTE_CHARACTER, "-", "core:body&playable" },
    { "c", SCRIPT_ROUTE_TAP_BALLOON, "talk", "balloon:talk" },
    { "c", SCRIPT_ROUTE_TRICK, "gate", "core:gate&playable" }
};

static struct script_machine machine;
static struct script_route route;
static struct script_outputs outputs;
static struct script_hint_view view;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];

static void on_hint_changed(const struct script_hint_view *new_view)
{
    view = *new_view;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = ROW_Y;
}

static void start_the_scene(void)
{
    static const unsigned int words[] = { 0x2f, 60000, 0x0 };
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memset(&route, 0, sizeof(route));
    for (index = 0; index < sizeof(step_rows) / sizeof(step_rows[0]); index++) {
        snprintf(route.steps[index].script, sizeof(route.steps[index].script), "%s", step_rows[index].script);
        route.steps[index].action = step_rows[index].action;
        snprintf(route.steps[index].object, sizeof(route.steps[index].object), "%s", step_rows[index].object);
        snprintf(route.steps[index].wait, sizeof(route.steps[index].wait), "%s", step_rows[index].wait);
    }
    route.step_count = (int)(sizeof(step_rows) / sizeof(step_rows[0]));
    memset(&outputs, 0, sizeof(outputs));
    memset(&view, 0, sizeof(view));
    outputs.hint_changed = on_hint_changed;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.hints.delay_ticks = DELAY_TICKS;
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 3;
    place_core(BODY, "body", BODY_X);
    place_core(GATE, "gate", GATE_X);
    place_core(LAMP, "lamp", LAMP_X);
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, BODY, 0);
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void press(int tap_group)
{
    script_soul_note_tap(&machine, tap_group);
    run_ticks(1);
}

static void hold_a_tap_balloon(int slot, int tap_group)
{
    machine.balloons[slot].in_use = 1;
    machine.balloons[slot].tap_group = tap_group;
    machine.balloons[slot].timer = 0xffff;
}

static void test_the_bubble_comes_up_over_the_soul_after_the_delay(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS - 1);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
    CHECK(!view.bubble_is_up);
    run_ticks(1);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_OFFERED);
    CHECK(view.bubble_is_up);
    CHECK_INT(view.bubble_tap_group, SCRIPT_TAP_GROUP_RIGHT);
    CHECK_INT((int)view.bubble_x, BODY_X);
    CHECK_INT(machine.hints.target_step, FIRST_GATE_TRICK);
    CHECK(view.flash_object == NULL);
}

static void test_the_clock_runs_only_while_the_player_has_control(void)
{
    start_the_scene();
    machine.soul.input_is_off = 1;
    run_ticks(DELAY_TICKS * 2);
    CHECK_INT(machine.hints.stuck_ticks, 0);
    machine.soul.input_is_off = 0;
    machine.narration.is_running = 1;
    run_ticks(DELAY_TICKS);
    CHECK_INT(machine.hints.stuck_ticks, 0);
    machine.narration.is_running = 0;
    machine.soul.is_on = 1;
    machine.soul.player_opened_soul_view = 1;
    machine.pause_bit = 1;
    run_ticks(DELAY_TICKS / 2);
    CHECK_INT(machine.hints.stuck_ticks, DELAY_TICKS / 2);
}

static void test_no_bubble_when_hints_are_off_or_auto_play_drives(void)
{
    start_the_scene();
    machine.hints.delay_ticks = 0;
    run_ticks(DELAY_TICKS * 3);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
    CHECK(!view.bubble_is_up);
    start_the_scene();
    machine.route_is_driving = 1;
    run_ticks(DELAY_TICKS * 3);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
}

static void test_the_bubble_hides_in_the_ghost_view_and_comes_back(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS);
    machine.soul.is_on = 1;
    run_ticks(1);
    CHECK(!view.bubble_is_up);
    press(SCRIPT_TAP_GROUP_RIGHT);
    CHECK(!machine.hints.was_used);
    machine.soul.is_on = 0;
    run_ticks(1);
    CHECK(view.bubble_is_up);
}

static void test_the_bubble_takes_the_tap_button_no_balloon_holds(void)
{
    start_the_scene();
    hold_a_tap_balloon(RIGHT_SLOT, SCRIPT_TAP_GROUP_RIGHT);
    run_ticks(DELAY_TICKS);
    CHECK(view.bubble_is_up);
    CHECK_INT(view.bubble_tap_group, SCRIPT_TAP_GROUP_LEFT);
    press(SCRIPT_TAP_GROUP_RIGHT);
    CHECK(!machine.hints.was_used);
    CHECK(!machine.balloons[RIGHT_SLOT].in_use);
    run_ticks(1);
    CHECK_INT(view.bubble_tap_group, SCRIPT_TAP_GROUP_RIGHT);
    start_the_scene();
    hold_a_tap_balloon(RIGHT_SLOT, SCRIPT_TAP_GROUP_RIGHT);
    hold_a_tap_balloon(LEFT_SLOT, SCRIPT_TAP_GROUP_LEFT);
    run_ticks(DELAY_TICKS * 2);
    CHECK(!view.bubble_is_up);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
}

static void test_a_press_flashes_a_target_that_is_on_screen(void)
{
    int camera_x;

    start_the_scene();
    run_ticks(DELAY_TICKS);
    camera_x = machine.camera_x;
    press(SCRIPT_TAP_GROUP_RIGHT);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_SHOWN);
    CHECK(machine.hints.was_used);
    CHECK(!view.bubble_is_up);
    CHECK_INT(machine.pause_bit, 0);
    CHECK_INT(machine.camera_x, camera_x);
    CHECK_TEXT(view.flash_object, "gate");
    run_ticks(5);
    CHECK(view.flash_alpha > 0.0f);
    CHECK(!view.pulse_is_on);
    machine.soul.is_on = 1;
    run_ticks(1);
    CHECK(view.pulse_is_on);
    CHECK_INT((int)view.pulse_x, GATE_X);
    CHECK(view.flash_alpha == 0.0f);
}

static void test_a_scene_gives_one_hint_until_another_scene_begins(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS);
    press(SCRIPT_TAP_GROUP_RIGHT);
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_SPENT);
    run_ticks(DELAY_TICKS * 3);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_SPENT);
    CHECK(!view.bubble_is_up);
    CHECK(view.flash_object == NULL);
    script_hints_script_began(&machine, "b", 0);
    run_ticks(DELAY_TICKS);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_OFFERED);
    CHECK(view.bubble_is_up);
}

static void test_the_camera_pans_to_a_target_off_screen_and_back(void)
{
    int home_x;
    int ticks;

    start_the_scene();
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    run_ticks(DELAY_TICKS);
    CHECK_INT(machine.hints.target_step, LAMP_TRICK);
    home_x = machine.camera_x;
    machine.soul.mark_age_ticks = -1;
    press(SCRIPT_TAP_GROUP_RIGHT);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_PANNING_OUT);
    CHECK_INT(machine.pause_bit, 1);
    CHECK(script_hints_hold_the_soul(&machine));
    CHECK(!script_soul_player_has_control(&machine));
    for (ticks = 0; ticks < MOST_PAN_TICKS && machine.hints.phase == SCRIPT_HINT_PANNING_OUT; ticks++) {
        run_ticks(1);
    }
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_HOLDING);
    CHECK_INT(machine.camera_x, LAMP_X - VIEW_WIDTH / 2);
    run_ticks(HOLD_TICKS / 4);
    CHECK_TEXT(view.flash_object, "lamp");
    CHECK(view.flash_alpha > 0.0f);
    run_ticks(HOLD_TICKS);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_PANNING_BACK);
    run_ticks(MOST_PAN_TICKS);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_SHOWN);
    CHECK_INT(machine.camera_x, home_x);
    CHECK_INT(machine.pause_bit, 0);
    CHECK(!script_hints_hold_the_soul(&machine));
    CHECK(machine.soul.mark_age_ticks >= 0);
}

static void test_presses_are_dropped_while_the_camera_pans(void)
{
    start_the_scene();
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    run_ticks(DELAY_TICKS);
    press(SCRIPT_TAP_GROUP_RIGHT);
    script_soul_note_ghost(&machine);
    script_machine_note_press(&machine);
    run_ticks(1);
    CHECK(!machine.soul.is_on);
    CHECK_INT(machine.soul.tricks_fired, 0);
    CHECK(!machine.press_is_pending);
}

static void test_no_bubble_for_a_target_the_camera_cannot_reach(void)
{
    start_the_scene();
    machine.objects[LAMP].actor.x = BEYOND_THE_ROOM;
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    run_ticks(DELAY_TICKS * 2);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
    CHECK(!view.bubble_is_up);
    machine.objects[LAMP].actor.x = LAMP_X;
    run_ticks(1);
    CHECK(view.bubble_is_up);
}

static void test_only_the_next_step_of_the_route_moves_the_hint_on(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS / 2);
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "lamp");
    CHECK_INT(machine.hints.next_step, 0);
    CHECK_INT(machine.hints.stuck_ticks, DELAY_TICKS / 2);
    script_hints_player_did(&machine, SCRIPT_ROUTE_TAP_BALLOON, NULL);
    CHECK_INT(machine.hints.next_step, 0);
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    CHECK_INT(machine.hints.next_step, FIRST_GATE_TRICK + 1);
    CHECK_INT(machine.hints.stuck_ticks, 0);
}

static void test_a_step_done_takes_an_unused_bubble_down(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS);
    CHECK(view.bubble_is_up);
    script_hints_player_did(&machine, SCRIPT_ROUTE_TRICK, "gate");
    run_ticks(1);
    CHECK(!view.bubble_is_up);
    CHECK(!machine.hints.was_used);
    run_ticks(DELAY_TICKS);
    CHECK(view.bubble_is_up);
    CHECK_INT(machine.hints.target_step, LAMP_TRICK);
}

static void test_an_act_a_later_step_waits_on_moves_the_hint_on(void)
{
    start_the_scene();
    script_hints_act_began(&machine, "gate", "act020");
    CHECK_INT(machine.hints.next_step, 0);
    script_hints_act_began(&machine, "game", "act010");
    CHECK_INT(machine.hints.next_step, 0);
    script_hints_act_began(&machine, "game", "act020");
    CHECK_INT(machine.hints.next_step, FIRST_GATE_TRICK + 1);
}

static void test_a_retry_keeps_the_clock_and_starts_at_the_retried_step(void)
{
    start_the_scene();
    run_ticks(DELAY_TICKS / 2);
    machine.retries_of_the_scene = 1;
    script_hints_script_began(&machine, "a", 1);
    CHECK_INT(machine.hints.next_step, RETRY_STEP);
    CHECK_INT(machine.hints.stuck_ticks, DELAY_TICKS / 2);
    run_ticks(DELAY_TICKS / 2);
    CHECK(view.bubble_is_up);
    CHECK_INT(machine.hints.target_step, RETRY_STEP + 1);
    CHECK_INT(machine.hints.target_object, BODY);
}

static void test_a_step_with_no_object_points_at_the_core_before_it(void)
{
    start_the_scene();
    script_hints_script_began(&machine, "b", 0);
    run_ticks(DELAY_TICKS);
    CHECK_INT(machine.hints.target_step, CHARACTER_STEP);
    CHECK_INT(machine.hints.target_object, BODY);
}

static void test_a_step_with_nothing_to_point_at_is_passed_over(void)
{
    start_the_scene();
    script_hints_script_began(&machine, "c", 0);
    run_ticks(DELAY_TICKS);
    CHECK_INT(machine.hints.target_step, TRICK_AFTER_A_BALLOON);
    CHECK_INT(machine.hints.target_object, GATE);
}

static void test_a_script_with_no_route_gives_no_hint(void)
{
    start_the_scene();
    script_hints_script_began(&machine, "unrouted", 0);
    run_ticks(DELAY_TICKS * 2);
    CHECK_INT(machine.hints.phase, SCRIPT_HINT_WAITING);
    CHECK(!view.bubble_is_up);
}

int main(void)
{
    test_the_bubble_comes_up_over_the_soul_after_the_delay();
    test_the_clock_runs_only_while_the_player_has_control();
    test_no_bubble_when_hints_are_off_or_auto_play_drives();
    test_the_bubble_hides_in_the_ghost_view_and_comes_back();
    test_the_bubble_takes_the_tap_button_no_balloon_holds();
    test_a_press_flashes_a_target_that_is_on_screen();
    test_a_scene_gives_one_hint_until_another_scene_begins();
    test_the_camera_pans_to_a_target_off_screen_and_back();
    test_presses_are_dropped_while_the_camera_pans();
    test_no_bubble_for_a_target_the_camera_cannot_reach();
    test_only_the_next_step_of_the_route_moves_the_hint_on();
    test_a_step_done_takes_an_unused_bubble_down();
    test_an_act_a_later_step_waits_on_moves_the_hint_on();
    test_a_retry_keeps_the_clock_and_starts_at_the_retried_step();
    test_a_step_with_no_object_points_at_the_core_before_it();
    test_a_step_with_nothing_to_point_at_is_passed_over();
    test_a_script_with_no_route_gives_no_hint();
    return test_finish("unit/hints");
}
