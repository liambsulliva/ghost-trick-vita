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

static const char strings[] = "\0\0talk_two_pauses\0talk_later_page\0";

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
        "talk talk_two_pauses\n"
        "page - 0 0 0 0 1 0 1 0 0 2\n"
        "line 0 0 3 0 0 0\n"
        "talk talk_later_page\n"
        "page - 0 0 0 0 1 0 0 0 0 0\n"
        "line 0 0 3 0 0 0\n"
        "page - 0 0 0 0 1 0 1 0 0 1\n"
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

static void test_a_page_waits_once_for_each_pause_before_its_text(void)
{
    white_out_then_talk("talk_two_pauses");
    run_ticks(10 + TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_PAUSED_BEFORE_TEXT);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
    script_talk_note_press(&machine);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_PAUSED_BEFORE_TEXT);
    CHECK_INT(machine.talk.pauses_before_done, 0);
    script_talk_note_forward(&machine);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_PAUSED_BEFORE_TEXT);
    CHECK_INT(machine.talk.pauses_before_done, 1);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
    script_talk_note_forward(&machine);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK_INT(machine.brightness, 0);
}

static void test_a_later_page_waits_for_its_own_pause(void)
{
    white_out_then_talk("talk_later_page");
    run_ticks(10 + TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK_INT(machine.talk.page_index, 0);
    script_talk_note_press(&machine);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.page_index, 1);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_PAUSED_BEFORE_TEXT);
    CHECK_INT(machine.brightness, NATIVE_WHITE);
    script_talk_note_forward(&machine);
    run_ticks(TICKS_TO_OPEN);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_WAITING);
    CHECK_INT(machine.brightness, 0);
}

int main(void)
{
    test_a_page_waits_once_for_each_pause_before_its_text();
    test_a_later_page_waits_for_its_own_pause();
    return test_finish("unit/talk_page_pauses");
}
