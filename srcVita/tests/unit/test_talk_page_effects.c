#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_talk.h"
#include "test_support.h"

#define WORD_MAX 32
#define TICKS_TO_OPEN 40
#define WHITE_OUT_OP 0xe
#define WHITE_OUT_FRAMES 4
#define NATIVE_WHITE 16

static struct script_machine machine;
static struct script_talk_book book;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 256];

static const char strings[] = "\0\0talk_flash\0talk_quake\0talk_plain\0talk_old_row\0";

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

static void white_out_then_talk(const char *label)
{
    static const char book_text[] =
        "glyph 0 0 8 8 0 0 8\n"
        "talk talk_flash\n"
        "page - 0 0 0 0 1 0 1 0 0\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_quake\n"
        "page - 0 0 0 0 1 0 0 2 30\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_plain\n"
        "page - 0 0 0 0 1 0 0 0 0\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_old_row\n"
        "page - 0 0 0 0 1 0\n"
        "line 0 0 3 0 0 0\n";
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = WHITE_OUT_OP;
    words[count++] = WHITE_OUT_FRAMES;
    words[count++] = 0x2f;
    words[count++] = 10;
    words[count++] = 0x37;
    words[count++] = offset_of(label);
    words[count++] = 0;
    words[count++] = 0x39;
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

static void test_a_page_that_flashes_brings_a_white_screen_back(void)
{
    white_out_then_talk("talk_flash");
    run_ticks(10);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase != SCRIPT_TALK_CLOSED, 1);
    CHECK_INT(machine.brightness, 0);
}

static void test_a_page_with_no_flash_leaves_a_white_screen_white(void)
{
    white_out_then_talk("talk_plain");
    run_ticks(10 + TICKS_TO_OPEN);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
}

static void test_a_page_that_quakes_shakes_the_screen_for_its_frames(void)
{
    int tick;
    int shook;

    white_out_then_talk("talk_quake");
    shook = 0;
    for (tick = 0; tick < 10 + TICKS_TO_OPEN; tick++) {
        script_machine_tick(&machine);
        if (machine.shake_ticks_left > 0) {
            shook = 1;
            CHECK_INT(machine.shake_level, 2);
            CHECK(machine.shake_ticks_left <= 30);
        }
    }
    CHECK(shook);
    run_ticks(40);
    CHECK_INT(machine.shake_ticks_left, 0);
}

static void test_a_page_row_baked_before_the_effects_still_reads(void)
{
    white_out_then_talk("talk_old_row");
    run_ticks(10 + TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase != SCRIPT_TALK_CLOSED, 1);
    CHECK_INT(machine.shake_ticks_left, 0);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
}

int main(void)
{
    test_a_page_that_flashes_brings_a_white_screen_back();
    test_a_page_with_no_flash_leaves_a_white_screen_white();
    test_a_page_that_quakes_shakes_the_screen_for_its_frames();
    test_a_page_row_baked_before_the_effects_still_reads();
    return test_finish("unit/talk_page_effects");
}
