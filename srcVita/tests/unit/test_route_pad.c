#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_route_pad.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define TICKS_FOR_A_PILL_TO_SLIDE_IN 40
#define TICKS_FOR_A_MOVE 120
#define CHOOSE_DWELL_TICKS 200
#define OPEN_CHOICE_OP 0x3b

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

int main(void)
{
    test_the_pad_opens_the_ghost_view_only_once_its_pill_is_up();
    test_the_pad_moves_the_soul_with_the_stick();
    test_the_pad_picks_the_route_panel_only_once_the_step_has_begun();
    return test_finish("unit/route_pad");
}
