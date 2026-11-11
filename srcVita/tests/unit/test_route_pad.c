#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_route_pad.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define DEMO_MODE 2
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define TICKS_FOR_A_PILL_TO_SLIDE_IN 40
#define TICKS_FOR_A_MOVE 120
#define CHOOSE_DWELL_TICKS 200
#define TICKS_FOR_THE_REWIND 200
#define TICKS_FOR_A_DEAD_TALK_TO_START 5
#define OPEN_CHOICE_OP 0x3b
#define GATE_OUT_OF_REACH 300

static struct script_machine machine;
static struct script_route_pad route_pad;
static struct script_input route_input;
static struct script_route route;
static const struct script_outputs no_outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];

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

static void start_with_one_step(enum script_route_action action, const char *object, const char *wait)
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
    snprintf(route.steps[0].script, sizeof(route.steps[0].script), "a");
    route.steps[0].action = action;
    snprintf(route.steps[0].object, sizeof(route.steps[0].object), "%s", object);
    snprintf(route.steps[0].wait, sizeof(route.steps[0].wait), "%s", wait);
    route.step_count = 1;
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = 1;
    machine.stall_breaker_is_off = 1;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 2;
    place_core(0, "body", BODY_X);
    place_core(1, "gate", GATE_X);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        struct input_state frame;

        script_route_pad_frame(&route_pad, &machine, &frame);
        script_input_frame(&route_input, &machine, &frame);
        script_soul_step_before_tasks(&machine);
        machine.tick = machine.tick + 1;
    }
}

static void test_the_pad_opens_the_ghost_view_only_once_its_pill_is_up(void)
{
    start_with_one_step(SCRIPT_ROUTE_GHOST_ON, "body", "playable");
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(!machine.soul.is_on);
    script_soul_run_command(&machine, "Button", "Trick:On");
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(machine.soul.is_on);
}

static void test_the_pad_offers_the_ghost_view_in_demo_mode(void)
{
    start_with_one_step(SCRIPT_ROUTE_GHOST_ON, "body", "no_talk");
    machine.mode_byte = DEMO_MODE;
    machine.soul.a_trick_is_playing_out = 1;
    script_soul_run_command(&machine, "Button", "Trick:On");
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN * 2);
    CHECK(machine.soul.is_on);
}

static void test_demo_mode_keeps_the_ghost_view_shut_while_input_is_off(void)
{
    start_with_one_step(SCRIPT_ROUTE_GHOST_ON, "body", "no_talk");
    machine.mode_byte = DEMO_MODE;
    machine.soul.input_is_off = 1;
    script_soul_run_command(&machine, "Button", "Trick:On");
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN * 2);
    CHECK(!machine.soul.is_on);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
}

static void test_the_pad_moves_the_soul_with_the_stick(void)
{
    start_with_one_step(SCRIPT_ROUTE_MOVE, "gate", "view");
    run_ticks(1);
    machine.soul.is_on = 1;
    run_ticks(TICKS_FOR_A_MOVE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void open_a_choice_of_two(void)
{
    static const unsigned int no_words[8];
    static const char *const events[2] = { "first", "second" };
    int index;

    for (index = 0; index < 2; index++) {
        machine.choice.panels[index].action = SCRIPT_CHOICE_SENDS_EVENT;
        machine.choice.panels[index].target = "body";
        machine.choice.panels[index].kind = "select";
        machine.choice.panels[index].event = events[index];
    }
    machine.choice.panel_count = 2;
    machine.choice.panel_limit = 2;
    machine.choice.control_is_up = 1;
    script_choice_run_op(&machine, NULL, OPEN_CHOICE_OP, no_words);
}

static void test_the_pad_picks_the_route_panel_only_once_the_step_has_begun(void)
{
    start_with_one_step(SCRIPT_ROUTE_CHOOSE, "second", "choice");
    route.steps[0].dwell_ticks = CHOOSE_DWELL_TICKS;
    open_a_choice_of_two();
    run_ticks(CHOOSE_DWELL_TICKS / 2);
    CHECK(machine.choice.is_open);
    run_ticks(CHOOSE_DWELL_TICKS * 2);
    CHECK(!machine.choice.is_open);
    CHECK_INT(machine.choice.chosen, 1);
    CHECK(script_soul_route_step(&machine) == NULL);
}

static int body_got(const char *verb, const char *arg)
{
    int index;

    for (index = 0; index < machine.objects[0].message_count; index++) {
        if (strcmp(machine.objects[0].messages[index].verb, verb) == 0 && strcmp(machine.objects[0].messages[index].arg, arg) == 0) {
            return 1;
        }
    }
    return 0;
}

static void test_the_pad_presses_the_rewind_button_once_it_is_up(void)
{
    start_with_one_step(SCRIPT_ROUTE_WAIT, "-", "ticks:1");
    script_soul_run_command(&machine, "Timeleap", "SELECT");
    run_ticks(TICKS_FOR_THE_REWIND);
    CHECK_INT(machine.soul.rewind_select_ticks, 0);
    CHECK(body_got("timeleap", "jump"));
}

static int gate_got(const char *verb, const char *arg)
{
    int index;

    for (index = 0; index < machine.objects[1].message_count; index++) {
        if (strcmp(machine.objects[1].messages[index].verb, verb) == 0 && strcmp(machine.objects[1].messages[index].arg, arg) == 0) {
            return 1;
        }
    }
    return 0;
}

static void test_a_dead_talk_from_a_living_core_is_a_core_talk(void)
{
    start_with_one_step(SCRIPT_ROUTE_DEAD_TALK, "-", "ticks:1");
    run_ticks(TICKS_FOR_A_DEAD_TALK_TO_START);
    CHECK(body_got("SYSTEM", "CORETALK"));
    CHECK(!body_got("SYSTEM", "DEADTALK"));
}

static void test_a_dead_talk_from_a_dead_core_talks_to_the_dead(void)
{
    start_with_one_step(SCRIPT_ROUTE_DEAD_TALK, "-", "ticks:1");
    machine.objects[1].is_dead = 1;
    script_soul_set_first_core(&machine, 1, 0);
    run_ticks(TICKS_FOR_A_DEAD_TALK_TO_START);
    CHECK(gate_got("SYSTEM", "DEADTALK"));
    CHECK(!body_got("SYSTEM", "DEADTALK"));
    CHECK(!gate_got("SYSTEM", "CORETALK"));
}

static void walk_onto_a_dead_gate_while_the_body_handles(int handler_count)
{
    static const char *const verbs[2] = { "SYSTEM", "SVTUTO" };
    static const char *const args[2] = { "SV_DEADCORE", "onClick" };
    int index;

    start_with_one_step(SCRIPT_ROUTE_MOVE, "gate", "view");
    machine.objects[1].is_dead = 1;
    machine.objects[0].handler_count = handler_count;
    for (index = 0; index < handler_count; index++) {
        machine.objects[0].handlers[index].verb = verbs[index];
        machine.objects[0].handlers[index].arg = args[index];
    }
    run_ticks(1);
    machine.soul.is_on = 1;
    run_ticks(TICKS_FOR_A_MOVE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_a_scene_waiting_on_the_dead_core_starts_the_dead_talk_on_arrival(void)
{
    walk_onto_a_dead_gate_while_the_body_handles(2);
    CHECK(machine.soul.dead_talk_is_on);
    CHECK(gate_got("SYSTEM", "DEADTALK"));
}

static void test_a_dead_core_alone_does_not_start_the_dead_talk(void)
{
    walk_onto_a_dead_gate_while_the_body_handles(1);
    CHECK(!machine.soul.dead_talk_is_on);
    CHECK(!gate_got("SYSTEM", "DEADTALK"));
}

static void test_the_pad_opens_the_ghost_view_once_the_next_core_comes_within_reach(void)
{
    start_with_one_step(SCRIPT_ROUTE_GHOST_ON, "body", "reach:gate&playable");
    machine.objects[1].actor.x = BODY_X + GATE_OUT_OF_REACH;
    script_soul_run_command(&machine, "Button", "Trick:On");
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(!machine.soul.is_on);
    machine.objects[1].actor.x = GATE_X;
    run_ticks(TICKS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(machine.soul.is_on);
}

static void test_the_pad_does_not_push_toward_a_core_out_of_reach(void)
{
    start_with_one_step(SCRIPT_ROUTE_MOVE, "gate", "view");
    machine.objects[1].actor.x = BODY_X + 300;
    run_ticks(1);
    machine.soul.is_on = 1;
    run_ticks(TICKS_FOR_A_MOVE);
    CHECK_INT(machine.soul.active_object, 0);
    CHECK(!machine.soul.tip_is_out);
}

static void start_a_move_with_missile_and_cores(int count)
{
    start_with_one_step(SCRIPT_ROUTE_MOVE, "gate", "view");
    machine.object_count = count;
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
}

static int moved_to_the_gate_past(int stepping_stone)
{
    int tick;
    int touched;

    run_ticks(1);
    machine.soul.is_on = 1;
    touched = 0;
    for (tick = 0; tick < TICKS_FOR_A_MOVE * 2 && machine.soul.active_object != 1; tick++) {
        run_ticks(1);
        touched = touched || machine.soul.active_object == stepping_stone;
    }
    return machine.soul.active_object == 1 && !touched;
}

static void test_a_core_on_the_line_is_the_hop_a_push_takes(void)
{
    int hop_object;
    int hop_slot;

    start_a_move_with_missile_and_cores(3);
    machine.objects[1].actor.x = BODY_X + 90;
    place_core(2, "lantern", BODY_X + 40);
    machine.objects[2].actor.y = ROW_Y + 3;
    CHECK(script_soul_route_hop(&machine, 1, 0, &hop_object, &hop_slot));
    CHECK_INT(hop_object, 2);
}

static void test_the_pad_aims_past_a_core_a_slanted_push_can_miss(void)
{
    int hop_object;
    int hop_slot;
    float aim_x;
    float aim_y;

    start_a_move_with_missile_and_cores(3);
    machine.objects[1].actor.x = BODY_X + 90;
    place_core(2, "lantern", BODY_X + 50);
    machine.objects[2].actor.y = ROW_Y + 13;
    CHECK(script_soul_route_hop(&machine, 1, 0, &hop_object, &hop_slot));
    CHECK_INT(hop_object, 1);
    CHECK(script_soul_route_hop_aim(&machine, 1, 0, &aim_x, &aim_y));
    CHECK(aim_y < (float)ROW_Y);
    CHECK(moved_to_the_gate_past(2));
}

static void test_a_missile_hop_never_steps_from_a_core_to_one_of_its_shape(void)
{
    int hop_object;
    int hop_slot;

    start_a_move_with_missile_and_cores(4);
    machine.objects[1].actor.x = BODY_X + 300;
    place_core(2, "first_twin", BODY_X + 100);
    place_core(3, "second_twin", BODY_X + 200);
    machine.objects[2].actor.shape = 7;
    machine.objects[3].actor.shape = 7;
    CHECK(!script_soul_route_hop(&machine, 1, 0, &hop_object, &hop_slot));
    machine.objects[3].actor.shape = 0;
    CHECK(script_soul_route_hop(&machine, 1, 0, &hop_object, &hop_slot));
    CHECK_INT(hop_object, 2);
}

int main(void)
{
    test_the_pad_opens_the_ghost_view_only_once_its_pill_is_up();
    test_the_pad_opens_the_ghost_view_once_the_next_core_comes_within_reach();
    test_the_pad_moves_the_soul_with_the_stick();
    test_the_pad_does_not_push_toward_a_core_out_of_reach();
    test_a_core_on_the_line_is_the_hop_a_push_takes();
    test_the_pad_aims_past_a_core_a_slanted_push_can_miss();
    test_a_missile_hop_never_steps_from_a_core_to_one_of_its_shape();
    test_the_pad_picks_the_route_panel_only_once_the_step_has_begun();
    test_the_pad_presses_the_rewind_button_once_it_is_up();
    test_a_dead_talk_from_a_living_core_is_a_core_talk();
    test_a_dead_talk_from_a_dead_core_talks_to_the_dead();
    test_a_scene_waiting_on_the_dead_core_starts_the_dead_talk_on_arrival();
    test_a_dead_core_alone_does_not_start_the_dead_talk();
    test_the_pad_offers_the_ghost_view_in_demo_mode();
    test_demo_mode_keeps_the_ghost_view_shut_while_input_is_off();
    return test_finish("unit/route_pad");
}
