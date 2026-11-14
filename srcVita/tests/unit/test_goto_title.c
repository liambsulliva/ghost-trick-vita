#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define TICKS_PAST_THE_CHANGE 60

static struct script_machine machine;
static const struct script_outputs no_outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(unsigned char *into, const char *strings, unsigned int strings_size)
{
    static const unsigned int words[] = { 0x24, 2, 0x2f, 6000, 0x0 };
    unsigned int index;
    unsigned int code_size;

    memset(into, 0, sizeof(scene));
    memcpy(into, "1LMG", 4);
    code_size = (unsigned int)(sizeof(words) / sizeof(words[0])) * 2u;
    put_u16(&into[0x08], code_size);
    put_u16(&into[0x0c], strings_size);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&into[SCRIPT_FILE_HEADER_SIZE + code_size], strings, strings_size);
    return SCRIPT_FILE_HEADER_SIZE + code_size + strings_size;
}

static void run_scene_that_changes_to(const char *strings, unsigned int strings_size)
{
    int tick;

    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(scene, strings, strings_size), NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < TICKS_PAST_THE_CHANGE && !machine.is_finished; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_goto_title_finishes_the_chain_and_asks_for_the_title(void)
{
    static const char strings[] = "\0\0GOTO TITLE\0";

    run_scene_that_changes_to(strings, sizeof(strings) - 1);
    CHECK_INT(machine.is_finished, 1);
    CHECK_INT(machine.title_is_wanted, 1);
}

static void test_goto_gametitle_also_asks_for_the_title(void)
{
    static const char strings[] = "\0\0GOTO GAMETITLE\0";

    run_scene_that_changes_to(strings, sizeof(strings) - 1);
    CHECK_INT(machine.is_finished, 1);
    CHECK_INT(machine.title_is_wanted, 1);
}

static void test_a_scene_missing_from_the_bake_finishes_without_the_title(void)
{
    static const char strings[] = "\0\0st99-st99_root\0";

    run_scene_that_changes_to(strings, sizeof(strings) - 1);
    CHECK_INT(machine.is_finished, 1);
    CHECK_INT(machine.title_is_wanted, 0);
}

int main(void)
{
    test_goto_title_finishes_the_chain_and_asks_for_the_title();
    test_goto_gametitle_also_asks_for_the_title();
    test_a_scene_missing_from_the_bake_finishes_without_the_title();
    return test_finish("unit/goto_title");
}
