#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define EVERY_OBJECT_NAME_OFFSET 0
#define LAMP_NAME_OFFSET 2
#define POINT_PAST_THE_TABLE 6

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 + 8];
static const char strings[] = "*\0lamp\0\0";

static void set_up_a_lamp_and_a_phone(void)
{
    struct script_actor *lamp;

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
    lamp = &machine.objects[0].actor;
    lamp->x = 100;
    lamp->y = 50;
    lamp->points[2][0] = 7;
    lamp->points[2][1] = 8;
    lamp->has_core = 1;
    lamp->core_x = 30;
    lamp->core_y = 40;
}

static void check_node(int node, float expected_x, float expected_y)
{
    float x;
    float y;

    script_machine_node_position(&machine.objects[0], node, &x, &y);
    CHECK(x == expected_x);
    CHECK(y == expected_y);
}

static void test_nodes_read_points_then_nothing_then_cores(void)
{
    set_up_a_lamp_and_a_phone();
    check_node(2, 107.0f, 58.0f);
    check_node(POINT_PAST_THE_TABLE, 100.0f, 50.0f);
    check_node(SCRIPT_FIRST_CORE_NODE, 130.0f, 90.0f);
    check_node(SCRIPT_FIRST_CORE_NODE + 1, 100.0f, 50.0f);
    machine.objects[0].actor.has_core = 0;
    check_node(SCRIPT_FIRST_CORE_NODE, 100.0f, 50.0f);
}

static void test_a_dock_onto_a_core_node_is_kept(void)
{
    static const unsigned int words[3] = { LAMP_NAME_OFFSET, SCRIPT_FIRST_CORE_NODE, 0 };
    struct script_vm vm;

    set_up_a_lamp_and_a_phone();
    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[1];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
    script_ops_actor_run(&machine, &vm, 0x85, words);
    CHECK_INT(machine.objects[1].actor.dock_is_pending, 1);
    CHECK_INT(machine.objects[1].actor.dock_target, 0);
    CHECK_INT(machine.objects[1].actor.dock_target_point, SCRIPT_FIRST_CORE_NODE);
}

static void start_a_phone_vm(struct script_vm *vm)
{
    memset(vm, 0, sizeof(*vm));
    vm->owner = &machine.objects[1];
    vm->selected_object = SCRIPT_NO_SELECTION;
    vm->source = &source;
}

static void test_a_follow_to_every_object_lets_go(void)
{
    static const unsigned int follow_the_lamp[3] = { LAMP_NAME_OFFSET, 2, 0 };
    static const unsigned int follow_nothing[3] = { EVERY_OBJECT_NAME_OFFSET, 0, 0 };
    struct script_vm vm;

    set_up_a_lamp_and_a_phone();
    start_a_phone_vm(&vm);
    script_ops_actor_run(&machine, &vm, 0x85, follow_the_lamp);
    CHECK_INT(machine.objects[1].actor.keeps_following, 1);
    script_ops_actor_run(&machine, &vm, 0x85, follow_nothing);
    CHECK_INT(machine.objects[1].actor.keeps_following, 0);
    CHECK_INT(machine.objects[1].actor.dock_is_pending, 0);
}

static void test_a_single_dock_to_every_object_changes_nothing(void)
{
    static const unsigned int dock_on_the_lamp[3] = { LAMP_NAME_OFFSET, 2, 0 };
    static const unsigned int dock_on_nothing[3] = { EVERY_OBJECT_NAME_OFFSET, 0, 0 };
    struct script_vm vm;

    set_up_a_lamp_and_a_phone();
    start_a_phone_vm(&vm);
    script_ops_actor_run(&machine, &vm, 0x86, dock_on_the_lamp);
    script_ops_actor_run(&machine, &vm, 0x86, dock_on_nothing);
    CHECK_INT(machine.objects[1].actor.dock_is_pending, 1);
    CHECK_INT(machine.objects[1].actor.dock_target, 0);
}

int main(void)
{
    test_nodes_read_points_then_nothing_then_cores();
    test_a_dock_onto_a_core_node_is_kept();
    test_a_follow_to_every_object_lets_go();
    test_a_single_dock_to_every_object_changes_nothing();
    return test_finish("unit/dock_onto_core");
}
