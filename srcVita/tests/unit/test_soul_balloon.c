#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_talk.h"
#include "test_support.h"

#define WORD_MAX 32
#define TICKS_TO_OPEN 40
#define SOUL_BALLOON_SLOT 1
#define FOREVER 0xffffu

static struct script_machine machine;
static struct script_talk_book book;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 256];

static const char strings[] = "\0\0talk_plain\0game\0EVENT\0tapped\0";

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

static void run_script(const unsigned int *words, unsigned int count)
{
    static const char book_text[] =
        "glyph 0 0 8 8 0 0 8\n"
        "talk talk_plain\n"
        "page - 0 0 0 0 1 0 0 0 0\n"
        "line 0 0 3 0 0 0\n";

    CHECK(script_talk_book_parse(book_text, sizeof(book_text) - 1, &book));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.talk_book = &book;
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_machine_tick(&machine);
}

static void open_a_balloon_that_talks(unsigned int ticks)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = 0x4c;
    words[count++] = offset_of("talk_plain");
    words[count++] = 0;
    words[count++] = ticks;
    words[count++] = 0;
    words[count++] = 0;
    words[count++] = 0;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    run_script(words, count);
}

static void open_a_balloon_that_sends(unsigned int ticks)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = 0x4e;
    words[count++] = ticks;
    words[count++] = 0;
    words[count++] = offset_of("game");
    words[count++] = offset_of("EVENT");
    words[count++] = offset_of("tapped");
    words[count++] = 0;
    words[count++] = 0;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    run_script(words, count);
}

static void test_a_script_with_no_actor_opens_the_soul_s_balloon(void)
{
    open_a_balloon_that_sends(FOREVER);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].in_use, 1);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].kind, SCRIPT_BALLOON_KIND_TAP_MARKS);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].tap_group, SCRIPT_TAP_GROUP_RIGHT);
    CHECK(strcmp(machine.balloons[SOUL_BALLOON_SLOT].tap_arg, "tapped") == 0);
}

static void test_a_balloon_opened_forever_does_not_run_out(void)
{
    int tick;

    open_a_balloon_that_sends(FOREVER);
    for (tick = 0; tick < 70000; tick++) {
        script_balloons_count_down(&machine);
    }
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].in_use, 1);
}

static void test_a_balloon_opened_for_no_time_only_closes_the_one_there(void)
{
    open_a_balloon_that_sends(0);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].in_use, 0);
}

static void test_tapping_a_balloon_that_talks_opens_its_talk(void)
{
    open_a_balloon_that_talks(FOREVER);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].in_use, 1);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].kind, SCRIPT_BALLOON_KIND_TAP_MARKS);
    CHECK_INT(machine.talk.phase, SCRIPT_TALK_CLOSED);
    script_balloons_tap(&machine, SCRIPT_TAP_GROUP_RIGHT);
    CHECK_INT(machine.balloons[SOUL_BALLOON_SLOT].in_use, 0);
    CHECK(machine.talk.phase != SCRIPT_TALK_CLOSED);
}

int main(void)
{
    test_a_script_with_no_actor_opens_the_soul_s_balloon();
    test_a_balloon_opened_forever_does_not_run_out();
    test_a_balloon_opened_for_no_time_only_closes_the_one_there();
    test_tapping_a_balloon_that_talks_opens_its_talk();
    return test_finish("unit/soul_balloon");
}
