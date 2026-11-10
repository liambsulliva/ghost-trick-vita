#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define FAR_X 180
#define PEN_PAST_EVERY_CORE_X 230.0f
#define TICKS_FOR_A_DRAG 60
#define TICKS_TO_SETTLE 60
#define NO_OBJECT (-1)

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = ROW_Y;
}

static void start_in_the_ghost_view_with_three_cores_in_a_row(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 3;
    place_core(0, "body", BODY_X);
    place_core(1, "gate", GATE_X);
    place_core(2, "far", FAR_X);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_step_before_tasks(&machine);
    machine.soul.is_on = 1;
    script_soul_step_before_tasks(&machine);
}

static void forget_messages(void)
{
    int index;

    for (index = 0; index < machine.object_count; index++) {
        machine.objects[index].message_count = 0;
    }
}

static int count_of(const char *verb, const char *arg)
{
    int index;
    int count;
    const struct script_object *object;

    object = &machine.objects[0];
    count = 0;
    for (index = 0; index < object->message_count; index++) {
        if (strcmp(object->messages[index].verb, verb) == 0 && strcmp(object->messages[index].arg, arg) == 0) {
            count = count + 1;
        }
    }
    return count;
}

static void step_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void drag_from_the_body_to(float x, int ticks)
{
    script_soul_set_touch(&machine, 1, (float)BODY_X, (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_set_touch(&machine, 1, x, (float)ROW_Y);
    step_ticks(ticks);
}

static void test_each_ghost_has_its_native_reach(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    CHECK(script_soul_reach(&machine) == SCRIPT_CICEL_REACH_DS_PIXELS);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    CHECK(script_soul_reach(&machine) == SCRIPT_MISSILE_REACH_DS_PIXELS);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_BOSS);
    CHECK(script_soul_reach(&machine) == SCRIPT_BOSS_REACH_DS_PIXELS);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    script_soul_run_command(&machine, "Exception", "CicelHandLong:On");
    CHECK(script_soul_reach(&machine) == SCRIPT_LONG_HAND_REACH_DS_PIXELS);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    CHECK(script_soul_reach(&machine) == SCRIPT_MISSILE_REACH_DS_PIXELS);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    script_soul_run_command(&machine, "Exception", "CicelHandLong:Off");
    CHECK(script_soul_reach(&machine) == SCRIPT_CICEL_REACH_DS_PIXELS);
}

static void test_one_long_drag_takes_the_soul_one_core_only(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_a_new_touch_on_the_tip_drags_on_from_the_new_core(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    script_soul_set_touch(&machine, 0, 0.0f, 0.0f);
    step_ticks(TICKS_TO_SETTLE);
    script_soul_set_touch(&machine, 1, (float)GATE_X, (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_set_touch(&machine, 1, PEN_PAST_EVERY_CORE_X, (float)ROW_Y);
    step_ticks(TICKS_FOR_A_DRAG);
    CHECK_INT(machine.soul.active_object, 2);
}

static void test_a_touch_away_from_the_tip_does_not_drag(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_set_touch(&machine, 1, (float)(BODY_X + 30), (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_set_touch(&machine, 1, PEN_PAST_EVERY_CORE_X, (float)ROW_Y);
    step_ticks(TICKS_FOR_A_DRAG);
    CHECK_INT(machine.soul.active_object, 0);
}

static void test_the_touch_check_tutorial_hears_click_touch_and_release(void)
{
    int touches;
    int tick;

    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_run_command(&machine, "SVTUTO", "TouchCheck");
    forget_messages();
    step_ticks(3);
    CHECK_INT(count_of("SVTUTO", "onClick"), 0);
    script_machine_note_press(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK_INT(count_of("SVTUTO", "onClick"), 0);
    script_soul_set_touch(&machine, 1, (float)BODY_X, (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_set_touch(&machine, 1, (float)(BODY_X + 20), (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK_INT(count_of("SVTUTO", "onClick"), 1);
    forget_messages();
    touches = 0;
    for (tick = 0; tick < 4; tick++) {
        script_soul_step_before_tasks(&machine);
        touches = touches + count_of("SVTUTO", "onTouch");
        forget_messages();
    }
    CHECK_INT(touches, 4);
    script_soul_set_touch(&machine, 0, 0.0f, 0.0f);
    step_ticks(TICKS_TO_SETTLE);
    CHECK_INT(count_of("SVTUTO", "onRelease"), 1);
}

static void test_the_move_tutorial_hears_move_while_the_direction_is_held(void)
{
    int moves;
    int tick;

    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_run_command(&machine, "SVTUTO", "MoveStart");
    forget_messages();
    step_ticks(3);
    CHECK_INT(count_of("SVTUTO", "move"), 0);
    moves = 0;
    for (tick = 0; tick < 3; tick++) {
        script_soul_set_held(&machine, 1.0f, 0.0f);
        script_soul_step_before_tasks(&machine);
        moves = moves + count_of("SVTUTO", "move");
        forget_messages();
    }
    CHECK_INT(moves, 3);
}

static void test_the_tutorial_view_holds_the_soul_until_its_check_begins(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_run_command(&machine, "SoulVision", "On");
    step_ticks(TICKS_TO_SETTLE);
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
    script_soul_set_held(&machine, 1.0f, 0.0f);
    step_ticks(TICKS_FOR_A_DRAG);
    script_soul_set_held(&machine, 0.0f, 0.0f);
    CHECK_INT(machine.soul.active_object, 0);
    script_soul_set_touch(&machine, 0, 0.0f, 0.0f);
    step_ticks(TICKS_TO_SETTLE);
    script_soul_run_command(&machine, "SVTUTO", "TouchCheck");
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_the_tutorial_view_holds_the_soul_again_once_its_move_ends(void)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_run_command(&machine, "SoulVision", "On");
    step_ticks(TICKS_TO_SETTLE);
    script_soul_run_command(&machine, "SVTUTO", "MoveStart");
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 1);
    script_soul_set_touch(&machine, 0, 0.0f, 0.0f);
    step_ticks(TICKS_TO_SETTLE);
    script_soul_run_command(&machine, "SVTUTO", "MoveEnd");
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 1);
    script_soul_set_held(&machine, -1.0f, 0.0f);
    step_ticks(TICKS_FOR_A_DRAG);
    script_soul_set_held(&machine, 0.0f, 0.0f);
    CHECK_INT(machine.soul.active_object, 1);
}

static void check_the_soul_stays_put_until_the_event_ends(const char *verb, const char *hold, const char *release)
{
    start_in_the_ghost_view_with_three_cores_in_a_row();
    script_soul_run_command(&machine, verb, hold);
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
    script_soul_set_touch(&machine, 0, 0.0f, 0.0f);
    step_ticks(TICKS_TO_SETTLE);
    script_soul_run_command(&machine, verb, release);
    step_ticks(TICKS_TO_SETTLE);
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_scripted_events_in_the_ghost_view_hold_the_soul(void)
{
    check_the_soul_stays_put_until_the_event_ends("CustomSoulVision", "DemoModeOn", "DemoModeOff");
    check_the_soul_stays_put_until_the_event_ends("Button", "Input:Off", "Input:On");
    start_in_the_ghost_view_with_three_cores_in_a_row();
    machine.demo_soul_is_on = 1;
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
    start_in_the_ghost_view_with_three_cores_in_a_row();
    machine.soul.ghost_pill_is_held_out = 1;
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
    start_in_the_ghost_view_with_three_cores_in_a_row();
    machine.film_frame_is_on = 1;
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
    start_in_the_ghost_view_with_three_cores_in_a_row();
    machine.movie_ticks_left = TICKS_FOR_A_DRAG * 4;
    drag_from_the_body_to(PEN_PAST_EVERY_CORE_X, TICKS_FOR_A_DRAG + TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.active_object, 0);
}

static void test_the_ghost_button_waits_for_a_trick_to_finish(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    place_core(0, "body", BODY_X);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", "Trick:On");
    machine.soul.ghost_pill_is_held_out = 1;
    machine.objects[0].actor.busy_cores = 1;
    step_ticks(TICKS_TO_SETTLE);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    script_soul_note_ghost(&machine);
    step_ticks(TICKS_TO_SETTLE);
    CHECK_INT(machine.soul.is_on, 0);
    machine.soul.ghost_pill_is_held_out = 0;
    machine.objects[0].actor.busy_cores = 0;
    step_ticks(TICKS_TO_SETTLE);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    script_soul_note_ghost(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK_INT(machine.soul.is_on, 1);
}

static void test_a_core_left_busy_without_a_ghost_hold_still_opens_the_view(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 3;
    place_core(0, "alarm", BODY_X);
    place_core(1, "gate", GATE_X);
    place_core(2, "far", FAR_X);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", "Trick:On");
    machine.objects[0].actor.busy_cores = 1;
    step_ticks(TICKS_TO_SETTLE);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    script_soul_note_ghost(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK_INT(machine.soul.is_on, 1);
}

int main(void)
{
    test_each_ghost_has_its_native_reach();
    test_one_long_drag_takes_the_soul_one_core_only();
    test_a_new_touch_on_the_tip_drags_on_from_the_new_core();
    test_a_touch_away_from_the_tip_does_not_drag();
    test_the_touch_check_tutorial_hears_click_touch_and_release();
    test_the_move_tutorial_hears_move_while_the_direction_is_held();
    test_the_tutorial_view_holds_the_soul_until_its_check_begins();
    test_the_tutorial_view_holds_the_soul_again_once_its_move_ends();
    test_scripted_events_in_the_ghost_view_hold_the_soul();
    test_the_ghost_button_waits_for_a_trick_to_finish();
    test_a_core_left_busy_without_a_ghost_hold_still_opens_the_view();
    return test_finish("unit/soul_touch_rules");
}
