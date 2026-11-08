#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define TICKS_BEFORE_THE_TEARDOWN 1
#define WORD_MAX 32
#define FLAG_WHEN_NOTHING_IS_PENDING 9

static struct script_machine machine;
static const struct script_outputs no_outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 256];
static unsigned char next_scene[SCRIPT_FILE_HEADER_SIZE + 64];

static const char strings[] = "\0\0b\0";

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(unsigned char *into, const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(into, 0, SCRIPT_FILE_HEADER_SIZE + word_count * 2 + sizeof(strings) - 1);
    memcpy(into, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&into[0x08], code_size);
    put_u16(&into[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&into[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static unsigned int add_flag_set_unless_a_change_is_pending(unsigned int *words, unsigned int at)
{
    words[at] = 0x6b;
    words[at + 1] = 0x33;
    words[at + 2] = 0x61;
    words[at + 3] = FLAG_WHEN_NOTHING_IS_PENDING;
    words[at + 4] = 0;
    return at + 5;
}

static void start_scene(int with_script_change_first)
{
    static const unsigned int idle_words[] = { 0x2f, 6000, 0x0 };
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    if (with_script_change_first) {
        words[count++] = 0x24;
        words[count++] = 2;
    }
    count = add_flag_set_unless_a_change_is_pending(words, count);
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(scene, words, count), NULL));
    CHECK(script_machine_add_source(&machine, "b", next_scene, make_script(next_scene, idle_words, 3), NULL));
    CHECK(script_machine_start(&machine, "a"));
}

static int flag_is_set(unsigned int flag)
{
    int value;

    return script_flags_read_known(&machine.flags, flag, &value) && value;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_with_no_change_pending_the_condition_is_clear(void)
{
    start_scene(0);
    run_ticks(TICKS_BEFORE_THE_TEARDOWN);
    CHECK(flag_is_set(FLAG_WHEN_NOTHING_IS_PENDING));
}

static void test_a_requested_script_change_sets_the_condition(void)
{
    start_scene(1);
    run_ticks(TICKS_BEFORE_THE_TEARDOWN);
    CHECK(!flag_is_set(FLAG_WHEN_NOTHING_IS_PENDING));
}

int main(void)
{
    test_with_no_change_pending_the_condition_is_clear();
    test_a_requested_script_change_sets_the_condition();
    return test_finish("unit/script_change_pending");
}
