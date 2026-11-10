#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define CELL 0
#define SPOON 1

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 + 32];

static const char strings[] = "\0\0cell\0spoon\0bell\0";

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

static void start_with_sissel_on_the_cell_and_missile_on_the_spoon(void)
{
    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = 2;
    bytes[0x0c] = (unsigned char)(sizeof(strings) - 1);
    memcpy(&bytes[SCRIPT_FILE_HEADER_SIZE + 2], strings, sizeof(strings) - 1);
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, SCRIPT_FILE_HEADER_SIZE + 2 + sizeof(strings) - 1), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[CELL].name = "cell";
    machine.objects[SPOON].name = "spoon";
    machine.objects[CELL].actor.exists = 1;
    machine.objects[SPOON].actor.exists = 1;
    script_soul_set_first_core(&machine, CELL, 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    script_soul_set_first_core(&machine, SPOON, 0);
}

static int the_check_passes(unsigned int op, const char *name)
{
    unsigned int words[4];
    struct script_vm vm;

    memset(&vm, 0, sizeof(vm));
    memset(words, 0, sizeof(words));
    vm.owner = &machine.objects[CELL];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
    vm.condition = 7;
    words[0] = offset_of(name);
    CHECK_INT(script_ops_actor_run(&machine, &vm, op, words), SCRIPT_STEP_NEXT);
    return vm.condition;
}

static void test_the_ops_are_routed_to_the_actor_family(void)
{
    CHECK_INT(script_opcode_family(0xa1), SCRIPT_OP_ACTOR);
    CHECK_INT(script_opcode_family(0xa2), SCRIPT_OP_ACTOR);
    CHECK_INT(script_opcode_family(0xa3), SCRIPT_OP_ACTOR);
}

static void test_op_a1_asks_about_sissels_core_whoever_is_active(void)
{
    start_with_sissel_on_the_cell_and_missile_on_the_spoon();
    CHECK_INT(the_check_passes(0xa1, "cell"), 1);
    CHECK_INT(the_check_passes(0xa1, "spoon"), 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    CHECK_INT(the_check_passes(0xa1, "cell"), 1);
    CHECK_INT(the_check_passes(0xa1, "spoon"), 0);
}

static void test_op_a2_asks_about_missiles_core_whoever_is_active(void)
{
    start_with_sissel_on_the_cell_and_missile_on_the_spoon();
    CHECK_INT(the_check_passes(0xa2, "spoon"), 1);
    CHECK_INT(the_check_passes(0xa2, "cell"), 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    CHECK_INT(the_check_passes(0xa2, "spoon"), 1);
    CHECK_INT(the_check_passes(0xa2, "cell"), 0);
}

static void test_ops_a0_and_a3_ask_about_the_active_ghosts_core(void)
{
    start_with_sissel_on_the_cell_and_missile_on_the_spoon();
    CHECK_INT(the_check_passes(0xa0, "spoon"), 1);
    CHECK_INT(the_check_passes(0xa3, "spoon"), 1);
    CHECK_INT(the_check_passes(0xa3, "cell"), 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    CHECK_INT(the_check_passes(0xa0, "cell"), 1);
    CHECK_INT(the_check_passes(0xa3, "cell"), 1);
    CHECK_INT(the_check_passes(0xa3, "spoon"), 0);
}

static void test_a_ghost_with_no_core_yet_fails_the_check(void)
{
    start_with_sissel_on_the_cell_and_missile_on_the_spoon();
    CHECK_INT(the_check_passes(0xa1, "bell"), 0);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[CELL].name = "cell";
    machine.objects[SPOON].name = "spoon";
    script_soul_set_first_core(&machine, CELL, 0);
    CHECK_INT(the_check_passes(0xa2, "cell"), 0);
    CHECK_INT(the_check_passes(0xa2, "spoon"), 0);
}

int main(void)
{
    test_the_ops_are_routed_to_the_actor_family();
    test_op_a1_asks_about_sissels_core_whoever_is_active();
    test_op_a2_asks_about_missiles_core_whoever_is_active();
    test_ops_a0_and_a3_ask_about_the_active_ghosts_core();
    test_a_ghost_with_no_core_yet_fails_the_check();
    return test_finish("unit/ghost_core_checks");
}
