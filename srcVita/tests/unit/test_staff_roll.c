#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_staff_roll.h"
#include "test_support.h"

#define OP_ENDING_MODE 0x6a
#define OP_STAFF_ROLL_START 0xf8
#define OP_STAFF_ROLL_WAIT 0xf9
#define OP_STAFF_ROLL_END 0xfa
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define MODE_BYTE_STAFF_ROLL 0x17
#define PAGE 4
#define FIRST_REST 140
#define EACH_REST 120
#define STEP_SLACK 4
#define TICK_LIMIT 5000
#define TICKS_FOR_THE_SCRIPT_TO_SETTLE 30

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static int roll_is_drawn;
static int page_drawn;
static int screens_drawn;
static float scroll_drawn;

static const char strings[] = "\0\0";

static void staff_roll_changed(int is_drawn, int page, int screen_count, float scroll)
{
    roll_is_drawn = is_drawn;
    page_drawn = page;
    screens_drawn = screen_count;
    scroll_drawn = scroll;
}

static const struct script_outputs outputs = { .staff_roll_changed = staff_roll_changed };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static int ticks_until_the_page_is_over(struct script_staff_roll *roll)
{
    int ticks;

    for (ticks = 0; ticks < TICK_LIMIT && script_staff_roll_holds_the_script(roll); ticks++) {
        script_staff_roll_step(roll);
    }
    return ticks;
}

static void test_a_page_of_one_screen_rests_and_is_over_without_scrolling(void)
{
    struct script_staff_roll roll;
    int ticks;

    memset(&roll, 0, sizeof(roll));
    script_staff_roll_start(&roll, 0);
    ticks = ticks_until_the_page_is_over(&roll);
    CHECK(ticks >= FIRST_REST + EACH_REST);
    CHECK(ticks <= FIRST_REST + EACH_REST + STEP_SLACK);
    CHECK(script_staff_roll_scroll(&roll) == 0.0f);
    CHECK_INT(script_staff_roll_is_drawn(&roll), 1);
}

static void test_each_further_screen_scrolls_in_at_a_pixel_a_tick_and_rests(void)
{
    struct script_staff_roll roll;
    int ticks;

    memset(&roll, 0, sizeof(roll));
    script_staff_roll_set_screens(&roll, PAGE, 3);
    script_staff_roll_start(&roll, PAGE);
    CHECK_INT(roll.screen_count, 3);
    ticks = ticks_until_the_page_is_over(&roll);
    CHECK(ticks >= FIRST_REST + 2 * (SCRIPT_STAFF_SCREEN_HEIGHT + EACH_REST));
    CHECK(ticks <= FIRST_REST + 2 * (SCRIPT_STAFF_SCREEN_HEIGHT + EACH_REST + STEP_SLACK) + STEP_SLACK);
    CHECK(script_staff_roll_scroll(&roll) == (float)(2 * SCRIPT_STAFF_SCREEN_HEIGHT));
}

static void test_the_scroll_stops_on_each_screen_top(void)
{
    struct script_staff_roll roll;
    int ticks;
    int ticks_resting_on_the_second_screen;

    memset(&roll, 0, sizeof(roll));
    script_staff_roll_set_screens(&roll, PAGE, 3);
    script_staff_roll_start(&roll, PAGE);
    ticks_resting_on_the_second_screen = 0;
    for (ticks = 0; ticks < TICK_LIMIT && script_staff_roll_holds_the_script(&roll); ticks++) {
        script_staff_roll_step(&roll);
        if (script_staff_roll_scroll(&roll) == (float)SCRIPT_STAFF_SCREEN_HEIGHT) {
            ticks_resting_on_the_second_screen = ticks_resting_on_the_second_screen + 1;
        }
    }
    CHECK(ticks_resting_on_the_second_screen >= EACH_REST);
    CHECK(ticks_resting_on_the_second_screen <= EACH_REST + STEP_SLACK);
}

static void test_a_screen_shows_only_its_rows_that_are_on_screen(void)
{
    struct script_staff_screen_span span;

    CHECK_INT(script_staff_roll_screen_span(0.0f, 0, &span), 1);
    CHECK(span.top == 0.0f && span.first_row == 0.0f && span.row_count == 192.0f);
    CHECK_INT(script_staff_roll_screen_span(0.0f, 1, &span), 0);
    CHECK_INT(script_staff_roll_screen_span(50.0f, 0, &span), 1);
    CHECK(span.top == 0.0f && span.first_row == 50.0f && span.row_count == 142.0f);
    CHECK_INT(script_staff_roll_screen_span(50.0f, 1, &span), 1);
    CHECK(span.top == 142.0f && span.first_row == 0.0f && span.row_count == 50.0f);
    CHECK_INT(script_staff_roll_screen_span(192.0f, 0, &span), 0);
}

static void start_the_ending_scene(void)
{
    static const unsigned int words[] = { OP_WAIT, TICKS_FOR_THE_SCRIPT_TO_SETTLE, OP_ENDING_MODE, OP_STAFF_ROLL_START, PAGE, OP_STAFF_ROLL_WAIT, OP_STAFF_ROLL_END, OP_WAIT, A_LONG_WAIT, OP_END };
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = sizeof(words) / sizeof(words[0]) * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_staff_roll_set_screens(&machine.staff_roll, PAGE, 2);
}

static void test_the_script_waits_for_the_page_then_ends_the_roll(void)
{
    int ticks;
    int ticks_held;
    int mode_before;

    start_the_ending_scene();
    for (ticks = 0; ticks < TICK_LIMIT && !machine.ending_mode_is_on; ticks++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.ending_mode_is_on, 1);
    mode_before = machine.mode_byte;
    CHECK(mode_before != MODE_BYTE_STAFF_ROLL);
    script_machine_tick(&machine);
    CHECK_INT(machine.mode_byte, MODE_BYTE_STAFF_ROLL);
    CHECK_INT(roll_is_drawn, 1);
    CHECK_INT(page_drawn, PAGE);
    CHECK_INT(screens_drawn, 2);
    ticks_held = 0;
    for (ticks = 0; ticks < TICK_LIMIT && machine.staff_roll.is_on; ticks++) {
        script_machine_tick(&machine);
        if (machine.staff_roll.state != SCRIPT_STAFF_ROLL_CLOSING && machine.staff_roll.state != SCRIPT_STAFF_ROLL_LEAVING) {
            ticks_held = ticks_held + 1;
        }
    }
    CHECK(ticks_held >= FIRST_REST + SCRIPT_STAFF_SCREEN_HEIGHT + EACH_REST);
    CHECK(ticks < TICK_LIMIT);
    CHECK(scroll_drawn == (float)SCRIPT_STAFF_SCREEN_HEIGHT);
    CHECK_INT(roll_is_drawn, 0);
    CHECK_INT(machine.mode_byte, mode_before);
    CHECK_INT(machine.ending_mode_is_on, 0);
}

int main(void)
{
    test_a_page_of_one_screen_rests_and_is_over_without_scrolling();
    test_each_further_screen_scrolls_in_at_a_pixel_a_tick_and_rests();
    test_the_scroll_stops_on_each_screen_top();
    test_a_screen_shows_only_its_rows_that_are_on_screen();
    test_the_script_waits_for_the_page_then_ends_the_roll();
    return test_finish("unit/staff_roll");
}
