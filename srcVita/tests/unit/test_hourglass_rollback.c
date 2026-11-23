#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "script_file.h"
#include "script_flags.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_route_pad.h"
#include "script_ui_view.h"
#include "test_support.h"

#define LONG_WAIT_TICKS 60000
#define TICKS_FOR_THE_PILL 60
#define MARK_MAX 64
#define FATE_SECTION_FLAG 0x1020u
#define TICKS_FOR_THE_RETRY 20
#define TICKS_TO_OPEN_AND_SHOW_THE_QUESTION 2
#define TICKS_FOR_THE_PILLS_TO_LEAVE 60
#define TICKS_WHILE_THE_QUESTION_WAITS 30
#define PAD_FRAMES_MOST 300
#define TOUCH_SCAN_STEP 2

static struct script_machine machine;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];

static void on_marked(const char *what, const char *name)
{
    if (mark_count < MARK_MAX) {
        snprintf(marks[mark_count], sizeof(marks[mark_count]), "%s %s", what, name);
        mark_count = mark_count + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked, .hud_changed = script_ui_view_hud_changed };

static int count_marks(const char *text)
{
    int index;
    int count;

    count = 0;
    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            count = count + 1;
        }
    }
    return count;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void put_u32(unsigned char *at, unsigned int value)
{
    put_u16(at, value & 0xffffu);
    put_u16(at + 2, value >> 16);
}

static unsigned int make_script(unsigned char *bytes, unsigned int capacity, const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(bytes, 0, capacity);
    memcpy(bytes, "1LMG", 4);
    code_size = word_count * 2;
    put_u32(&bytes[0x08], code_size);
    put_u32(&bytes[0x0c], 2);
    put_u32(&bytes[0x10], 0);
    for (index = 0; index < word_count; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    return SCRIPT_FILE_HEADER_SIZE + code_size + 2;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void start_a_scene_with_the_rollback_on(int in_the_four_minutes)
{
    static const unsigned int words[] = { 0xfe, 0x2f, LONG_WAIT_TICKS, 0x0 };
    unsigned int size;

    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    size = make_script(scene, sizeof(scene), words, sizeof(words) / sizeof(words[0]));
    CHECK(script_machine_add_source(&machine, "attempt", scene, size, NULL));
    CHECK(script_machine_start(&machine, "attempt"));
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, in_the_four_minutes);
    run_ticks(TICKS_FOR_THE_PILL);
}

static void press_the_hourglass_and_see_the_question(void)
{
    start_a_scene_with_the_rollback_on(1);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(TICKS_TO_OPEN_AND_SHOW_THE_QUESTION);
    CHECK(machine.soul.question_is_open);
    CHECK_INT(machine.soul.question_kind, SCRIPT_QUESTION_ROLLBACK);
    CHECK_INT(machine.soul.question_cursor, SCRIPT_QUESTION_OPTION_NO);
    CHECK_INT(count_marks("question_opened rollback"), 1);
}

static void check_nothing_restarted(void)
{
    CHECK_INT(count_marks("rollback_retry reload"), 0);
    CHECK_INT(count_marks("rollback_retry restart"), 0);
    CHECK_INT(count_marks("script_begun attempt"), 1);
    CHECK_INT(machine.retries_of_the_scene, 0);
}

static void check_the_attempt_restarted_once(void)
{
    CHECK(!machine.soul.question_is_open);
    CHECK_INT(count_marks("question_answered yes"), 1);
    CHECK_INT(count_marks("rollback_retry reload"), 1);
    CHECK_INT(count_marks("script_begun attempt"), 2);
    CHECK_INT(machine.retries_of_the_scene, 1);
}

static void check_the_question_closed_on_no(void)
{
    unsigned int tick;

    CHECK(!machine.soul.question_is_open);
    CHECK_INT(machine.soul.question_answer, SCRIPT_QUESTION_ANSWER_NO);
    CHECK_INT(count_marks("question_answered no"), 1);
    tick = machine.tick;
    run_ticks(TICKS_FOR_THE_RETRY);
    CHECK_INT(machine.tick, tick + TICKS_FOR_THE_RETRY);
    check_nothing_restarted();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
}

static void send_a_frame(unsigned int pressed, int touch_is_down, float touch_x, float touch_y)
{
    struct script_input input;
    struct input_state frame;

    script_input_reset(&input);
    memset(&frame, 0, sizeof(frame));
    frame.pressed = pressed;
    frame.touch_is_down = touch_is_down;
    frame.touch_x = touch_x;
    frame.touch_y = touch_y;
    script_input_frame(&input, &machine, &frame);
}

static int find_the_option_on_screen(int option, float *x, float *y)
{
    int scan_x;
    int scan_y;

    for (scan_y = 0; scan_y < RENDERER_HEIGHT; scan_y = scan_y + TOUCH_SCAN_STEP) {
        for (scan_x = 0; scan_x < RENDERER_WIDTH; scan_x = scan_x + TOUCH_SCAN_STEP) {
            if (script_ui_view_question_option_at((float)scan_x, (float)scan_y) == option) {
                *x = (float)scan_x;
                *y = (float)scan_y;
                return 1;
            }
        }
    }
    return 0;
}

static void test_the_hourglass_stays_away_outside_the_four_minutes(void)
{
    start_a_scene_with_the_rollback_on(0);
    CHECK_INT(machine.soul.rollback_is_on, 1);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(10);
    CHECK(!machine.soul.question_is_open);
    check_nothing_restarted();
}

static void test_the_hourglass_asks_and_holds_the_world_without_restarting(void)
{
    unsigned int tick;

    press_the_hourglass_and_see_the_question();
    tick = machine.tick;
    run_ticks(TICKS_FOR_THE_PILLS_TO_LEAVE);
    CHECK(machine.soul.question_is_open);
    CHECK_INT(machine.tick, tick);
    CHECK(!script_pills_any_on_screen(&machine));
    check_nothing_restarted();
}

static void test_yes_restarts_the_attempt_as_the_hourglass_did(void)
{
    press_the_hourglass_and_see_the_question();
    script_soul_note_direction(&machine, -1, 0);
    run_ticks(1);
    CHECK_INT(machine.soul.question_cursor, SCRIPT_QUESTION_OPTION_YES);
    CHECK(machine.soul.question_is_open);
    script_machine_note_press(&machine);
    run_ticks(TICKS_FOR_THE_RETRY);
    check_the_attempt_restarted_once();
}

static void test_a_press_on_the_marked_no_closes_the_question(void)
{
    press_the_hourglass_and_see_the_question();
    script_machine_note_press(&machine);
    run_ticks(1);
    check_the_question_closed_on_no();
}

static void test_circle_and_l_answer_no(void)
{
    press_the_hourglass_and_see_the_question();
    send_a_frame(INPUT_LEFT, 0, 0.0f, 0.0f);
    run_ticks(1);
    CHECK_INT(machine.soul.question_cursor, SCRIPT_QUESTION_OPTION_YES);
    send_a_frame(INPUT_CIRCLE, 0, 0.0f, 0.0f);
    run_ticks(1);
    check_the_question_closed_on_no();
    press_the_hourglass_and_see_the_question();
    send_a_frame(INPUT_L, 0, 0.0f, 0.0f);
    run_ticks(1);
    check_the_question_closed_on_no();
}

static void test_a_touch_on_a_button_answers(void)
{
    float x;
    float y;

    press_the_hourglass_and_see_the_question();
    CHECK(find_the_option_on_screen(SCRIPT_QUESTION_OPTION_NO, &x, &y));
    send_a_frame(0, 1, x, y);
    run_ticks(1);
    check_the_question_closed_on_no();
    press_the_hourglass_and_see_the_question();
    send_a_frame(0, 1, 0.0f, 0.0f);
    run_ticks(1);
    CHECK(machine.soul.question_is_open);
    CHECK(find_the_option_on_screen(SCRIPT_QUESTION_OPTION_YES, &x, &y));
    send_a_frame(0, 1, x, y);
    run_ticks(TICKS_FOR_THE_RETRY);
    check_the_attempt_restarted_once();
}

static void test_the_route_pad_answers_yes_after_reading_the_question(void)
{
    struct script_route_pad pad;
    struct script_input input;
    struct input_state frame;
    int frames;

    press_the_hourglass_and_see_the_question();
    script_route_pad_reset(&pad);
    script_input_reset(&input);
    for (frames = 0; frames < PAD_FRAMES_MOST && machine.soul.question_is_open; frames++) {
        script_route_pad_frame(&pad, &machine, &frame);
        script_input_frame(&input, &machine, &frame);
        script_machine_tick(&machine);
        if (frames == TICKS_WHILE_THE_QUESTION_WAITS) {
            CHECK(machine.soul.question_is_open);
        }
    }
    CHECK(frames > TICKS_WHILE_THE_QUESTION_WAITS);
    run_ticks(TICKS_FOR_THE_RETRY);
    check_the_attempt_restarted_once();
}

static void test_the_hourglass_waits_out_a_talk(void)
{
    start_a_scene_with_the_rollback_on(1);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    machine.talk_is_open = 1;
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(1);
    machine.talk_is_open = 0;
    run_ticks(10);
    CHECK(!machine.soul.question_is_open);
    CHECK_INT(count_marks("rollback_retry reload"), 0);
    CHECK_INT(machine.soul.rollback_is_pending, 0);
}

int main(void)
{
    test_the_hourglass_stays_away_outside_the_four_minutes();
    test_the_hourglass_asks_and_holds_the_world_without_restarting();
    test_yes_restarts_the_attempt_as_the_hourglass_did();
    test_a_press_on_the_marked_no_closes_the_question();
    test_circle_and_l_answer_no();
    test_a_touch_on_a_button_answers();
    test_the_route_pad_answers_yes_after_reading_the_question();
    test_the_hourglass_waits_out_a_talk();
    return test_finish("unit/hourglass_rollback");
}
