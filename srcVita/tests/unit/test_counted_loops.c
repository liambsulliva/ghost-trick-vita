#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define FRAME_HOLD_SHIFT 10
#define HOLD_TICKS 2
#define TICKS_TO_WAIT_AT_MOST 64

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 12];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_with_a_looping_table(void)
{
    static const unsigned int table[] = { 0xfffd, 0, 0, 1 | (HOLD_TICKS << FRAME_HOLD_SHIFT), 2 | (HOLD_TICKS << FRAME_HOLD_SHIFT), 0xffff };
    unsigned int index;

    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)sizeof(table) / sizeof(table[0]) * 2;
    for (index = 0; index < sizeof(table) / sizeof(table[0]); index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "dog";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
    script_motion_update_all(&machine);
}

static struct script_vm vm_owned_by_the_dog(void)
{
    struct script_vm vm;

    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[0];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
    return vm;
}

static int ticks_until_the_motion_waits_no_more(struct script_vm *vm)
{
    static const unsigned int no_words[4];
    int tick;

    for (tick = 0; tick < TICKS_TO_WAIT_AT_MOST; tick++) {
        if (script_ops_actor_run(&machine, vm, 0xa9, no_words) != SCRIPT_STEP_MOVED_THEN_YIELD) {
            return tick;
        }
        script_motion_update_all(&machine);
    }
    return -1;
}

static void test_a_looping_motion_never_ends_on_its_own(void)
{
    struct script_vm vm;

    start_with_a_looping_table();
    vm = vm_owned_by_the_dog();
    CHECK_INT(ticks_until_the_motion_waits_no_more(&vm), -1);
}

static void test_one_counted_loop_ends_at_the_next_loop_back(void)
{
    static const unsigned int count_one[4] = { 1, 0, 0, 0 };
    struct script_vm vm;
    int ticks;

    start_with_a_looping_table();
    vm = vm_owned_by_the_dog();
    CHECK_INT(script_ops_actor_run(&machine, &vm, 0xaa, count_one), SCRIPT_STEP_MOVED);
    CHECK_INT(vm.pc, 2);
    ticks = ticks_until_the_motion_waits_no_more(&vm);
    CHECK(ticks > 0 && ticks <= 2 * HOLD_TICKS);
    CHECK_INT(machine.objects[0].actor.frame_word_index, 1);
}

static void test_without_an_actor_it_skips_three_words_and_yields(void)
{
    static const unsigned int count_one[4] = { 1, 0, 0, 0 };
    struct script_vm vm;

    start_with_a_looping_table();
    machine.objects[0].actor.exists = 0;
    vm = vm_owned_by_the_dog();
    CHECK_INT(script_ops_actor_run(&machine, &vm, 0xaa, count_one), SCRIPT_STEP_MOVED_THEN_YIELD);
    CHECK_INT(vm.pc, 3);
}

int main(void)
{
    test_a_looping_motion_never_ends_on_its_own();
    test_one_counted_loop_ends_at_the_next_loop_back();
    test_without_an_actor_it_skips_three_words_and_yields();
    return test_finish("unit/counted_loops");
}
