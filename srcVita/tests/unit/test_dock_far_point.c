#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define LAMP_NAME_OFFSET 2
#define FAR_POINT 10

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 + 8];
static const char strings[] = "\0\0lamp\0\0";

static void set_up_a_lamp_and_a_phone(void)
{
    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = 2;
    bytes[0x0c] = sizeof(strings) - 1;
    memcpy(&bytes[SCRIPT_FILE_HEADER_SIZE + 2], strings, sizeof(strings) - 1);
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[0].name = "lamp";
    machine.objects[1].name = "phone";
    machine.objects[0].actor.exists = 1;
    machine.objects[1].actor.exists = 1;
    machine.objects[0].actor.points[FAR_POINT][0] = 30;
    machine.objects[0].actor.points[FAR_POINT][1] = 40;
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

static void test_a_dock_to_a_point_past_the_first_four_is_kept(void)
{
    static const unsigned int words[3] = { LAMP_NAME_OFFSET, FAR_POINT, 0 };
    struct script_vm vm;

    set_up_a_lamp_and_a_phone();
    vm = vm_owned_by(1);
    script_ops_actor_run(&machine, &vm, 0x85, words);
    CHECK_INT(machine.objects[1].actor.dock_is_pending, 1);
    CHECK_INT(machine.objects[1].actor.dock_target, 0);
    CHECK_INT(machine.objects[1].actor.dock_target_point, FAR_POINT);
}

static void test_a_dock_past_the_point_table_is_refused(void)
{
    static const unsigned int words[3] = { LAMP_NAME_OFFSET, SCRIPT_ACTOR_POINT_MAX, 0 };
    struct script_vm vm;

    set_up_a_lamp_and_a_phone();
    vm = vm_owned_by(1);
    script_ops_actor_run(&machine, &vm, 0x85, words);
    CHECK_INT(machine.objects[1].actor.dock_is_pending, 0);
}

int main(void)
{
    test_a_dock_to_a_point_past_the_first_four_is_kept();
    test_a_dock_past_the_point_table_is_refused();
    return test_finish("unit/dock_far_point");
}
