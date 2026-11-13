#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define A_FLAG_OF_THE_SCENE_BANK 0x8001u
#define A_FLAG_OF_THE_GAME_BANK 0x1001u

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 8];
static const struct script_outputs outputs = { 0 };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_a_scene_that_waits(void)
{
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], 6);
    put_u16(&scene[SCRIPT_FILE_HEADER_SIZE], OP_WAIT);
    put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2], A_LONG_WAIT);
    put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 4], OP_END);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + 6, NULL));
    CHECK(script_machine_start(&machine, "a"));
}

static void test_a_new_scene_starts_with_the_scene_bank_clear(void)
{
    start_a_scene_that_waits();
    script_flags_write(&machine.flags, A_FLAG_OF_THE_SCENE_BANK, 1);
    script_flags_write(&machine.flags, A_FLAG_OF_THE_GAME_BANK, 1);
    CHECK(script_machine_start(&machine, "a"));
    CHECK(!script_flags_read(&machine.flags, A_FLAG_OF_THE_SCENE_BANK));
    CHECK(script_flags_read(&machine.flags, A_FLAG_OF_THE_GAME_BANK));
}

static void test_a_retry_keeps_the_scene_bank(void)
{
    start_a_scene_that_waits();
    script_flags_write(&machine.flags, A_FLAG_OF_THE_SCENE_BANK, 1);
    machine.scene_is_being_retried = 1;
    CHECK(script_machine_start(&machine, "a"));
    CHECK(script_flags_read(&machine.flags, A_FLAG_OF_THE_SCENE_BANK));
}

int main(void)
{
    test_a_new_scene_starts_with_the_scene_bank_clear();
    test_a_retry_keeps_the_scene_bank();
    return test_finish("unit/chapter_flags");
}
