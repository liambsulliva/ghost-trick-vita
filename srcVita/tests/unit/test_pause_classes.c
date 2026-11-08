#include <string.h>
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_vm vm;

static void set_up_an_object_vm_of_class(int pause_class)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "phone";
    machine.objects[0].main_vm.kind = SCRIPT_VM_MAIN;
    machine.objects[0].main_vm.pause_class = pause_class;
    memset(&vm, 0, sizeof(vm));
    vm.kind = SCRIPT_VM_MAIN;
    vm.owner = &machine.objects[0];
}

static void in_the_ghost_view_with_the_world_halted(void)
{
    machine.soul.is_on = 1;
    machine.pause_bit = 1;
    machine.talk_is_open = 0;
}

static void test_class_one_runs_in_the_ghost_view(void)
{
    set_up_an_object_vm_of_class(PAUSE_CLASS_ONE);
    in_the_ghost_view_with_the_world_halted();
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 0);
    machine.soul.is_on = 0;
    machine.soul.is_leaving_view = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 0);
}

static void test_class_one_waits_while_a_talk_shows(void)
{
    set_up_an_object_vm_of_class(PAUSE_CLASS_ONE);
    in_the_ghost_view_with_the_world_halted();
    machine.talk_is_open = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
}

static void test_class_one_waits_through_a_dead_talk_and_a_choice_panel(void)
{
    set_up_an_object_vm_of_class(PAUSE_CLASS_ONE);
    in_the_ghost_view_with_the_world_halted();
    machine.soul.dead_talk_is_on = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
    machine.soul.dead_talk_is_on = 0;
    machine.choice.is_open = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
}

static void test_class_one_follows_the_halt_outside_the_view(void)
{
    set_up_an_object_vm_of_class(PAUSE_CLASS_ONE);
    machine.pause_bit = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
    machine.pause_bit = 0;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 0);
}

static void test_the_other_classes_keep_their_rules(void)
{
    set_up_an_object_vm_of_class(PAUSE_CLASS_NORMAL);
    in_the_ghost_view_with_the_world_halted();
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
    set_up_an_object_vm_of_class(PAUSE_CLASS_TALK);
    in_the_ghost_view_with_the_world_halted();
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 0);
    machine.talk_is_open = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 1);
    set_up_an_object_vm_of_class(PAUSE_CLASS_FREE);
    in_the_ghost_view_with_the_world_halted();
    machine.talk_is_open = 1;
    CHECK_INT(script_machine_vm_is_paused(&machine, &vm), 0);
}

int main(void)
{
    test_class_one_runs_in_the_ghost_view();
    test_class_one_waits_while_a_talk_shows();
    test_class_one_waits_through_a_dead_talk_and_a_choice_panel();
    test_class_one_follows_the_halt_outside_the_view();
    test_the_other_classes_keep_their_rules();
    return test_finish("unit/pause_classes");
}
