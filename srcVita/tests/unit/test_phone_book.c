#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "test_support.h"

#define MARK_MAX 64
#define TICKS_BEFORE_THE_BOOK_OPENS 60
#define TICKS_AFTER_THE_BOOK_OPENS 400

static struct script_machine machine;
static struct script_route_pad route_pad;
static struct script_input route_input;
static struct script_route route;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 32];

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

#define TICKS_UNTIL_A_LATE_BOOK 2500
#define TICKS_AFTER_A_LATE_BOOK 2900

static unsigned int words[] = { 0x2f, 100, 0x77, 0, 0, 0x2f, 6000, 0x0 };

static void start_a_scene_that_opens_the_phone_book_later(const char *step_line)
{
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
    machine.stall_breaker_is_off = 1;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    mark_count = 0;
    CHECK(script_machine_start(&machine, "a"));
}

static void run_ticks(int ticks)
{
    struct input_state frame;
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_route_pad_frame(&route_pad, &machine, &frame);
        script_input_frame(&route_input, &machine, &frame);
        script_machine_tick(&machine);
    }
}

static void test_go_there_waits_for_the_phone_book_to_open(void)
{
    start_a_scene_that_opens_the_phone_book_later("step a phone_call tel 4 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS);
    CHECK(!machine.phone_book_is_open);
    CHECK(!was_marked("route_step_done 0:phone_call:tel:4"));
    run_ticks(TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_call:tel:4"));
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 4);
}

static void test_stay_here_answers_the_phone_book_without_picking_a_place(void)
{
    start_a_scene_that_opens_the_phone_book_later("step a phone_stay tel 0 0 -\n");
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_PHONE_STAY);
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_PHONE_STAY), "phone_stay");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS);
    CHECK(!was_marked("route_step_done 0:phone_stay:tel:0"));
    run_ticks(TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_stay:tel:0"));
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 0);
}

static void test_a_book_opened_long_after_the_step_is_still_answered(void)
{
    words[1] = TICKS_UNTIL_A_LATE_BOOK;
    start_a_scene_that_opens_the_phone_book_later("step a phone_call tel 4 0 -\n");
    run_ticks(TICKS_UNTIL_A_LATE_BOOK - 10);
    CHECK(!was_marked("route_step_ready 0:phone_call:tel:4"));
    CHECK(!was_marked("route_step_failed 0:phone_call:tel:4"));
    run_ticks(TICKS_AFTER_A_LATE_BOOK - TICKS_UNTIL_A_LATE_BOOK + 10);
    CHECK(was_marked("route_step_done 0:phone_call:tel:4"));
    CHECK(!was_marked("route_step_failed 0:phone_call:tel:4"));
    CHECK_INT(machine.phone_book_place, 4);
    words[1] = 100;
}

int main(void)
{
    test_go_there_waits_for_the_phone_book_to_open();
    test_stay_here_answers_the_phone_book_without_picking_a_place();
    test_a_book_opened_long_after_the_step_is_still_answered();
    return test_finish("unit/phone_book");
}
