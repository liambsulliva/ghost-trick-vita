#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define TICKS_TO_RUN 3

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];
static int ops_not_ported;

static void on_marked(const char *what, const char *detail)
{
    (void)detail;
    if (strcmp(what, "op_not_ported") == 0) {
        ops_not_ported = ops_not_ported + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void test_both_ops_are_control_ops_of_one_word(void)
{
    CHECK_INT(script_opcode_family(0x7f), SCRIPT_OP_CONTROL);
    CHECK_INT(script_opcode_family(0x80), SCRIPT_OP_CONTROL);
    CHECK_INT((int)script_opcode_length(0x7f), 1);
    CHECK_INT((int)script_opcode_length(0x80), 1);
}

static void test_a_script_runs_past_both_ops(void)
{
    static const unsigned int words[] = { 0x80, 0x7f, 0x80, 0x0 };
    unsigned int index;
    int tick;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    ops_not_ported = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + (unsigned int)sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(ops_not_ported, 0);
}

int main(void)
{
    test_both_ops_are_control_ops_of_one_word();
    test_a_script_runs_past_both_ops();
    return test_finish("unit/sound_slot_ops");
}
