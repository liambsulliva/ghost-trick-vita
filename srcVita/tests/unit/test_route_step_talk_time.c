#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "test_support.h"

#define MARK_MAX 64
#define TICKS_A_LONG_TALK_HOLDS 2400
#define TICKS_PAST_THE_STEP_LIMIT 1900

static struct script_machine machine;
static struct script_route_pad route_pad;
static struct script_input route_input;
static struct script_route route;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 8];
static const unsigned int words[] = { 0x2f, 30000, 0x0 };

static void on_marked(const char *what, const char *name)
{
    if (mark_count < MARK_MAX) {
        snprintf(marks[mark_count], sizeof(marks[mark_count]), "%s %s", what, name);
        mark_count = mark_count + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static int was_marked(const char *text)
{
    int index;

    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            return 1;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_a_step_that_never_finishes(void)
{
    static const char step_line[] = "step a trick nobody 0 0 -\n";
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memset(&route, 0, sizeof(route));
    CHECK(script_route_read(&route, step_line, strlen(step_line)));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = 1;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    mark_count = 0;
    CHECK(script_machine_start(&machine, "a"));
}

static void run_ticks(int ticks, int talk_is_open)
{
    struct input_state frame;
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_route_pad_frame(&route_pad, &machine, &frame);
        script_input_frame(&route_input, &machine, &frame);
        if (talk_is_open) {
            machine.talk.phase = SCRIPT_TALK_WAITING;
        }
        script_machine_tick(&machine);
    }
    machine.talk.phase = SCRIPT_TALK_CLOSED;
}

static void test_ticks_a_talk_holds_the_player_do_not_count_against_a_step(void)
{
    start_a_step_that_never_finishes();
    run_ticks(TICKS_A_LONG_TALK_HOLDS, 1);
    CHECK(was_marked("route_step_began 0:trick:nobody:0"));
    CHECK(!was_marked("route_step_failed 0:trick:nobody:0"));
    run_ticks(TICKS_PAST_THE_STEP_LIMIT, 0);
    CHECK(was_marked("route_step_failed 0:trick:nobody:0"));
}

int main(void)
{
    test_ticks_a_talk_holds_the_player_do_not_count_against_a_step();
    return test_finish("unit/route_step_talk_time");
}
