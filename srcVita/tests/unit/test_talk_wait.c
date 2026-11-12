#include <stdio.h>
#include <string.h>
#include "input.h"
#include "script_file.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_talk.h"
#include "test_support.h"

#define WORD_MAX 32
#define SHOWN_FLAG 7
#define TICKS_TO_REVEAL 120

static struct script_machine machine;
static struct script_talk_book book;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 256];

static const char strings[] = "\0\0talk_paused_end\0talk_press_end\0talk_long\0";

static const struct script_outputs outputs = { 0 };

static unsigned int offset_of(const char *text)
{
    unsigned int offset;

    for (offset = 0; offset < sizeof(strings) - 1; offset++) {
        if (strcmp(&strings[offset], text) == 0) {
            return offset;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static void start_talk_then_set_a_flag(const char *label)
{
    static const char book_text[] =
        "glyph 0 0 8 8 0 0 8\n"
        "talk talk_paused_end\n"
        "page - 0 0 0 2 1 0\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_press_end\n"
        "page - 0 0 0 0 1 0\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_long\n"
        "page - 0 0 0 0 1 0\n"
        "line 0 0 24 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = 0x37;
    words[count++] = offset_of(label);
    words[count++] = 0;
    words[count++] = 0x39;
    words[count++] = 0x61;
    words[count++] = SHOWN_FLAG;
    words[count++] = 0;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    CHECK(script_talk_book_parse(book_text, sizeof(book_text) - 1, &book));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.talk_book = &book;
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static int flag_is_set(unsigned int flag)
{
    int value;

    return script_flags_read_known(&machine.flags, flag, &value) && value;
}

static void test_a_shown_page_that_ends_the_message_lets_the_script_and_world_go_on(void)
{
    start_talk_then_set_a_flag("talk_paused_end");
    run_ticks(TICKS_TO_REVEAL);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK_INT(machine.talk_is_open, 1);
    CHECK_INT(machine.pause_bit, 0);
    CHECK(!script_talk_holds_the_script(&machine));
    CHECK(flag_is_set(SHOWN_FLAG));
}

static void test_a_page_that_ends_on_a_press_holds_the_script(void)
{
    start_talk_then_set_a_flag("talk_press_end");
    run_ticks(TICKS_TO_REVEAL);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK(script_talk_holds_the_script(&machine));
    CHECK(!flag_is_set(SHOWN_FLAG));
}

static int ticks_revealing_at(enum script_text_speed speed)
{
    int tick;
    int revealing;

    start_talk_then_set_a_flag("talk_long");
    machine.text_speed = speed;
    revealing = 0;
    for (tick = 0; tick < 300; tick++) {
        script_machine_tick(&machine);
        revealing = revealing + (machine.talk.phase == SCRIPT_TALK_REVEALING);
    }
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK_INT(machine.talk.glyphs_shown, 24);
    return revealing;
}

static void test_text_speed_halves_or_doubles_the_glyphs_shown_each_tick(void)
{
    int normal;

    normal = ticks_revealing_at(SCRIPT_TEXT_NORMAL);
    CHECK(normal >= 23 && normal <= 25);
    CHECK_INT(ticks_revealing_at(SCRIPT_TEXT_SLOW), 2 * normal);
    CHECK_INT(ticks_revealing_at(SCRIPT_TEXT_FAST), normal / 2);
}

static int ticks_until_the_talk_closes(unsigned int held)
{
    static struct script_input pad;
    struct input_state frame;
    int tick;

    start_talk_then_set_a_flag("talk_long");
    memset(&pad, 0, sizeof(pad));
    memset(&frame, 0, sizeof(frame));
    frame.held = held;
    for (tick = 0; tick < 600 && !flag_is_set(SHOWN_FLAG); tick++) {
        script_input_frame(&pad, &machine, &frame);
        script_machine_tick(&machine);
    }
    return tick;
}

static void test_holding_circle_finishes_and_closes_talk_as_fast_as_it_can(void)
{
    int held;

    CHECK_INT(ticks_until_the_talk_closes(0), 600);
    held = ticks_until_the_talk_closes(INPUT_CIRCLE);
    CHECK(held < 40);
    CHECK_INT(ticks_until_the_talk_closes(INPUT_CROSS), 600);
}

int main(void)
{
    test_a_shown_page_that_ends_the_message_lets_the_script_and_world_go_on();
    test_a_page_that_ends_on_a_press_holds_the_script();
    test_text_speed_halves_or_doubles_the_glyphs_shown_each_tick();
    test_holding_circle_finishes_and_closes_talk_as_fast_as_it_can();
    return test_finish("unit/talk_wait");
}
