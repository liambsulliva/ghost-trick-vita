#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 4];

static void set_up_two_objects(void)
{
    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = sizeof(bytes) - SCRIPT_FILE_HEADER_SIZE;
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[0].name = "lamp";
    machine.objects[1].name = "phone";
    machine.objects[0].main_vm.source = &source;
    machine.objects[1].main_vm.source = &source;
    script_soul_set_first_core(&machine, 0, 0);
}

static struct script_vm vm_owned_by(int object_index)
{
    struct script_vm vm;

    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[object_index];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
    return vm;
}

static void test_each_placing_op_sets_the_core_of_its_own_ghost(void)
{
    static const unsigned int ops[] = { 0x9c, 0xa4, 0xa7 };
    static const int ghosts[] = { SCRIPT_GHOST_CICEL, SCRIPT_GHOST_MISSILE, SCRIPT_GHOST_BOSS };
    static const unsigned int second_core[4] = { 1, 0, 0, 0 };
    struct script_vm vm;
    unsigned int index;

    for (index = 0; index < sizeof(ops) / sizeof(ops[0]); index++) {
        set_up_two_objects();
        machine.soul.active_ghost = ghosts[index];
        vm = vm_owned_by(1);
        CHECK_INT(script_opcode_family(ops[index]), SCRIPT_OP_ACTOR);
        script_ops_actor_run(&machine, &vm, ops[index], second_core);
        CHECK_INT(machine.soul.active_object, 1);
        CHECK_INT(machine.soul.active_slot, 1);
    }
}

static void test_placing_another_ghost_leaves_the_ghost_in_play_where_it_is(void)
{
    static const unsigned int second_core[4] = { 1, 0, 0, 0 };
    struct script_vm vm;

    set_up_two_objects();
    machine.soul.active_ghost = SCRIPT_GHOST_CICEL;
    vm = vm_owned_by(1);
    script_ops_actor_run(&machine, &vm, 0xa4, second_core);
    CHECK_INT(machine.soul.active_object, 0);
    CHECK_INT(machine.soul.active_slot, 0);
    CHECK_INT(machine.soul.ghost_core_objects[SCRIPT_GHOST_MISSILE], 1);
    CHECK_INT(machine.soul.ghost_core_slots[SCRIPT_GHOST_MISSILE], 1);
}

int main(void)
{
    test_each_placing_op_sets_the_core_of_its_own_ghost();
    test_placing_another_ghost_leaves_the_ghost_in_play_where_it_is();
    return test_finish("unit/soul_placed_on_core");
}
