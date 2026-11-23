#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_flags.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "script_ui_view.h"
#include "renderer.h"
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

static const struct script_outputs outputs = { .marked = on_marked, .hud_changed = script_ui_view_hud_changed };

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

#define BOOK_IS_HELD_FLAG 0x1022u
#define FATE_SECTION_FLAG 0x1020u
#define PLACE_THE_BOOK_HAD 7
#define PRESSES_PAST_THE_EDGE 5
#define TOUCH_SCAN_STEP 4
#define WAIT_WORD 1
#define PLACE_WORD 12

static unsigned int words[] = { 0x2f, 100, 0x76, 0, 4, 0, 0x76, 0, 22, 0, 0x77, 0, 0, 0x2f, 6000, 0x0 };

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
    words[WAIT_WORD] = TICKS_UNTIL_A_LATE_BOOK;
    start_a_scene_that_opens_the_phone_book_later("step a phone_call tel 4 0 -\n");
    run_ticks(TICKS_UNTIL_A_LATE_BOOK - 10);
    CHECK(!was_marked("route_step_ready 0:phone_call:tel:4"));
    CHECK(!was_marked("route_step_failed 0:phone_call:tel:4"));
    run_ticks(TICKS_AFTER_A_LATE_BOOK - TICKS_UNTIL_A_LATE_BOOK + 10);
    CHECK(was_marked("route_step_done 0:phone_call:tel:4"));
    CHECK(!was_marked("route_step_failed 0:phone_call:tel:4"));
    CHECK_INT(machine.phone_book_place, 4);
    words[WAIT_WORD] = 100;
}

static void start_a_scene_the_player_plays(unsigned int place)
{
    unsigned int index;

    words[PLACE_WORD] = place;
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    script_input_reset(&route_input);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    mark_count = 0;
    CHECK(script_machine_start(&machine, "a"));
    words[PLACE_WORD] = 0;
}

static void press_on_the_pad(unsigned int buttons)
{
    struct input_state frame;

    memset(&frame, 0, sizeof(frame));
    frame.pressed = buttons;
    script_input_frame(&route_input, &machine, &frame);
    script_machine_tick(&machine);
}

static void wait_for_the_book(void)
{
    int tick;

    for (tick = 0; tick < TICKS_AFTER_THE_BOOK_OPENS && !machine.phone_book_is_open; tick++) {
        press_on_the_pad(0u);
    }
    CHECK(machine.phone_book_is_open);
}

static void test_the_player_picks_any_place_the_book_offers(void)
{
    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    CHECK_INT(machine.phone_book.count, 3);
    CHECK_INT(machine.phone_book.places[0], 1);
    CHECK_INT(machine.phone_book.places[1], 4);
    CHECK_INT(machine.phone_book.places[2], 22);
    CHECK_INT(machine.phone_book.cursor, 1);
    press_on_the_pad(INPUT_DOWN);
    CHECK_INT(machine.phone_book.cursor, 2);
    press_on_the_pad(INPUT_DOWN);
    CHECK_INT(machine.phone_book.cursor, 0);
    press_on_the_pad(INPUT_CROSS);
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 1);
    CHECK(was_marked("phone_book_answered movie place=1"));
}

static void test_the_player_stays_here_with_circle(void)
{
    start_a_scene_the_player_plays(22);
    wait_for_the_book();
    CHECK_INT(machine.phone_book.cursor, 2);
    press_on_the_pad(INPUT_CIRCLE);
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 0);
    CHECK(was_marked("phone_book_answered cancel place=0"));
}

static void test_the_book_waits_while_the_script_holds_it(void)
{
    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    script_flags_write(&machine.flags, BOOK_IS_HELD_FLAG, 1);
    press_on_the_pad(INPUT_CROSS);
    CHECK(machine.phone_book_is_open);
    script_flags_write(&machine.flags, BOOK_IS_HELD_FLAG, 0);
    press_on_the_pad(INPUT_CROSS);
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 4);
}

static void test_the_book_keeps_the_pad_from_the_ghost(void)
{
    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(INPUT_L);
    CHECK(!machine.soul.is_on);
    CHECK(machine.phone_book_is_open);
}

static void test_the_route_never_picks_a_place_the_book_lacks(void)
{
    start_a_scene_that_opens_the_phone_book_later("step a phone_call tel 13 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS + TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(machine.phone_book_is_open);
    CHECK(!was_marked("route_step_done 0:phone_call:tel:13"));
}

static void start_a_traced_call_the_player_plays(unsigned int place)
{
    start_a_scene_the_player_plays(place);
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, 1);
}

static void test_a_book_opened_in_a_fate_section_offers_only_go_there_and_stay_here(void)
{
    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    CHECK(machine.phone_book.is_traced);
    CHECK_INT(machine.phone_book.count, SCRIPT_PHONE_TRACE_CHOICES);
    CHECK_INT(machine.phone_book.cursor, SCRIPT_PHONE_TRACE_GO_THERE);
    CHECK_INT(script_phone_book_index_of(&machine, 4), -1);
    CHECK(was_marked("phone_book_opened mode=0 place=4 trace=1"));
}

static void test_a_book_opened_outside_a_fate_section_marks_the_list(void)
{
    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    CHECK(!machine.phone_book.is_traced);
    CHECK(was_marked("phone_book_opened mode=0 place=4 trace=0"));
}

static void test_go_there_in_a_traced_call_keeps_the_place_the_book_had(void)
{
    start_a_traced_call_the_player_plays(4);
    machine.phone_book_place = PLACE_THE_BOOK_HAD;
    wait_for_the_book();
    press_on_the_pad(INPUT_CROSS);
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, PLACE_THE_BOOK_HAD);
    CHECK(was_marked("phone_book_answered movie place=7"));
}

static void test_stay_here_is_the_second_choice_of_a_traced_call(void)
{
    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(INPUT_DOWN);
    CHECK_INT(machine.phone_book.cursor, SCRIPT_PHONE_TRACE_STAY_HERE);
    press_on_the_pad(INPUT_CROSS);
    CHECK(!machine.phone_book_is_open);
    CHECK_INT(machine.phone_book_place, 0);
    CHECK(was_marked("phone_book_answered cancel place=0"));
    CHECK(!was_marked("phone_book_answered movie place=0"));
}

static void test_circle_stays_here_in_a_traced_call(void)
{
    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(INPUT_CIRCLE);
    CHECK(!machine.phone_book_is_open);
    CHECK(was_marked("phone_book_answered cancel place=0"));
}

static void test_a_traced_call_reaches_no_other_place(void)
{
    int press;

    start_a_traced_call_the_player_plays(22);
    wait_for_the_book();
    for (press = 0; press < PRESSES_PAST_THE_EDGE; press++) {
        press_on_the_pad(INPUT_DOWN);
    }
    CHECK_INT(machine.phone_book.cursor, SCRIPT_PHONE_TRACE_STAY_HERE);
    for (press = 0; press < PRESSES_PAST_THE_EDGE; press++) {
        press_on_the_pad(INPUT_UP);
    }
    CHECK_INT(machine.phone_book.cursor, SCRIPT_PHONE_TRACE_GO_THERE);
    press_on_the_pad(INPUT_CROSS);
    CHECK_INT(machine.phone_book_place, 0);
    CHECK(was_marked("phone_book_answered movie place=0"));
}

static void test_the_fate_flag_is_read_when_the_book_opens(void)
{
    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, 1);
    press_on_the_pad(0u);
    CHECK(!machine.phone_book.is_traced);
    CHECK_INT(machine.phone_book.count, 3);
    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, 0);
    press_on_the_pad(0u);
    CHECK(machine.phone_book.is_traced);
    CHECK_INT(machine.phone_book.count, SCRIPT_PHONE_TRACE_CHOICES);
}

static void touch_the_screen(float x, float y)
{
    struct input_state frame;

    memset(&frame, 0, sizeof(frame));
    frame.touch_is_down = 1;
    frame.touch_x = x;
    frame.touch_y = y;
    script_input_frame(&route_input, &machine, &frame);
    script_machine_tick(&machine);
}

static int find_the_choice_on_screen(int choice, float *x, float *y)
{
    int scan_x;
    int scan_y;

    for (scan_y = 0; scan_y < RENDERER_HEIGHT; scan_y = scan_y + TOUCH_SCAN_STEP) {
        for (scan_x = 0; scan_x < RENDERER_WIDTH; scan_x = scan_x + TOUCH_SCAN_STEP) {
            if (script_ui_view_phone_choice_at((float)scan_x, (float)scan_y) == choice) {
                *x = (float)scan_x;
                *y = (float)scan_y;
                return 1;
            }
        }
    }
    return 0;
}

static void test_tapping_stay_here_chooses_it(void)
{
    float go_x;
    float go_y;
    float stay_x;
    float stay_y;

    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(0u);
    CHECK(find_the_choice_on_screen(SCRIPT_PHONE_TRACE_GO_THERE, &go_x, &go_y));
    CHECK(find_the_choice_on_screen(SCRIPT_PHONE_TRACE_STAY_HERE, &stay_x, &stay_y));
    CHECK(stay_y > go_y);
    touch_the_screen(stay_x, stay_y);
    CHECK(!machine.phone_book_is_open);
    CHECK(was_marked("phone_book_answered cancel place=0"));
}

static void test_tapping_go_there_chooses_it(void)
{
    float x;
    float y;

    start_a_traced_call_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(INPUT_DOWN);
    CHECK(find_the_choice_on_screen(SCRIPT_PHONE_TRACE_GO_THERE, &x, &y));
    touch_the_screen(x, y);
    CHECK(!machine.phone_book_is_open);
    CHECK(was_marked("phone_book_answered movie place=0"));
}

static void test_the_list_takes_no_tap(void)
{
    float x;
    float y;

    start_a_scene_the_player_plays(4);
    wait_for_the_book();
    press_on_the_pad(0u);
    CHECK(!find_the_choice_on_screen(SCRIPT_PHONE_TRACE_GO_THERE, &x, &y));
    touch_the_screen((float)RENDERER_WIDTH / 2.0f, (float)RENDERER_HEIGHT / 2.0f);
    CHECK(machine.phone_book_is_open);
}

static void start_a_traced_call_the_route_answers(const char *step_line)
{
    start_a_scene_that_opens_the_phone_book_later(step_line);
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, 1);
}

static void test_the_route_answers_a_traced_call_without_a_place(void)
{
    start_a_traced_call_the_route_answers("step a phone_call tel 0 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS + TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_call:tel:0"));
    CHECK(was_marked("phone_book_answered movie place=0"));
    CHECK(!was_marked("route_place_in_traced_call 0:phone_call:tel:0"));
}

static void test_the_route_stays_here_in_a_traced_call(void)
{
    start_a_traced_call_the_route_answers("step a phone_stay tel 0 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS + TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_stay:tel:0"));
    CHECK(was_marked("phone_book_answered cancel place=0"));
}

static void test_a_route_place_in_a_traced_call_is_marked(void)
{
    start_a_traced_call_the_route_answers("step a phone_call tel 4 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS + TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_call:tel:4"));
    CHECK(was_marked("route_place_in_traced_call 0:phone_call:tel:4"));
    CHECK_INT(machine.phone_book_place, 0);
}

static void test_a_route_place_in_a_listed_call_is_not_marked(void)
{
    start_a_scene_that_opens_the_phone_book_later("step a phone_call tel 4 0 -\n");
    run_ticks(TICKS_BEFORE_THE_BOOK_OPENS + TICKS_AFTER_THE_BOOK_OPENS);
    CHECK(was_marked("route_step_done 0:phone_call:tel:4"));
    CHECK(!was_marked("route_place_in_traced_call 0:phone_call:tel:4"));
}

int main(void)
{
    test_go_there_waits_for_the_phone_book_to_open();
    test_stay_here_answers_the_phone_book_without_picking_a_place();
    test_a_book_opened_long_after_the_step_is_still_answered();
    test_the_player_picks_any_place_the_book_offers();
    test_the_player_stays_here_with_circle();
    test_the_book_waits_while_the_script_holds_it();
    test_the_book_keeps_the_pad_from_the_ghost();
    test_the_route_never_picks_a_place_the_book_lacks();
    test_a_book_opened_in_a_fate_section_offers_only_go_there_and_stay_here();
    test_a_book_opened_outside_a_fate_section_marks_the_list();
    test_go_there_in_a_traced_call_keeps_the_place_the_book_had();
    test_stay_here_is_the_second_choice_of_a_traced_call();
    test_circle_stays_here_in_a_traced_call();
    test_a_traced_call_reaches_no_other_place();
    test_the_fate_flag_is_read_when_the_book_opens();
    test_tapping_stay_here_chooses_it();
    test_tapping_go_there_chooses_it();
    test_the_list_takes_no_tap();
    test_the_route_answers_a_traced_call_without_a_place();
    test_the_route_stays_here_in_a_traced_call();
    test_a_route_place_in_a_traced_call_is_marked();
    test_a_route_place_in_a_listed_call_is_not_marked();
    return test_finish("unit/phone_book");
}
