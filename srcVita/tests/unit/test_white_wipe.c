#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 16
#define WHITE_WIPE_EFFECT 50
#define FULL_WHITE 16
#define TICKS_TO_FULL_WHITE 60
#define TICKS_TO_CLEAR 400

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];

static const char strings[] = "\0\0";

static const struct script_outputs outputs = { 0 };

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

static void start_a_scene_with_the_white_wipe(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_soul_effect_spawned(&machine, WHITE_WIPE_EFFECT, "wipe");
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_the_wipe_turns_the_screen_white(void)
{
    start_a_scene_with_the_white_wipe();
    run_ticks(TICKS_TO_FULL_WHITE);
    CHECK_INT(machine.soul.white_wipe_level, FULL_WHITE);
}

static void test_the_wipe_clears_by_itself_when_no_script_holds_it(void)
{
    start_a_scene_with_the_white_wipe();
    run_ticks(TICKS_TO_CLEAR);
    CHECK_INT(machine.soul.white_wipe_level, 0);
}

static void test_a_script_can_hold_the_screen_white(void)
{
    start_a_scene_with_the_white_wipe();
    machine.soul.white_wipe_is_held = 1;
    run_ticks(TICKS_TO_CLEAR);
    CHECK_INT(machine.soul.white_wipe_level, FULL_WHITE);
    machine.soul.white_wipe_is_held = 0;
    run_ticks(TICKS_TO_CLEAR);
    CHECK_INT(machine.soul.white_wipe_level, 0);
}

static void test_deleting_the_effect_ends_the_wipe(void)
{
    start_a_scene_with_the_white_wipe();
    run_ticks(TICKS_TO_FULL_WHITE);
    script_soul_effect_deleted(&machine, "wipe");
    run_ticks(1);
    CHECK_INT(machine.soul.white_wipe_level, 0);
}

int main(void)
{
    test_the_wipe_turns_the_screen_white();
    test_the_wipe_clears_by_itself_when_no_script_holds_it();
    test_a_script_can_hold_the_screen_white();
    test_deleting_the_effect_ends_the_wipe();
    return test_finish("unit/white_wipe");
}
