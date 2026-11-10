#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define FRAME_HOLD_SHIFT 10
#define TABLE_WORD_MAX 16
#define TICKS_TO_WAIT_AT_MOST 64

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 * TABLE_WORD_MAX];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void load_a_table(const unsigned int *table, unsigned int word_count)
{
    unsigned int index;

    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)(word_count * 2);
    for (index = 0; index < word_count; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, SCRIPT_FILE_HEADER_SIZE + 2 * word_count), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "dog";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
}

static void load_two_frames_held_for(unsigned int hold_ticks)
{
    unsigned int table[6];

    table[0] = 0xfffd;
    table[1] = 0;
    table[2] = 0;
    table[3] = 1 | (hold_ticks << FRAME_HOLD_SHIFT);
    table[4] = 2 | (hold_ticks << FRAME_HOLD_SHIFT);
    table[5] = 0xffff;
    load_a_table(table, 6);
}

static void set_the_speed(unsigned int speed)
{
    unsigned int words[4];
    struct script_vm vm;

    memset(&vm, 0, sizeof(vm));
    memset(words, 0, sizeof(words));
    vm.owner = &machine.objects[0];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
    words[0] = speed;
    CHECK_INT(script_ops_actor_run(&machine, &vm, 0xae, words), SCRIPT_STEP_NEXT);
}

static void start_the_table(void)
{
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
    script_motion_update_all(&machine);
}

static int ticks_the_first_frame_shows(void)
{
    int ticks;

    CHECK_INT(machine.objects[0].actor.frame_number, 1);
    for (ticks = 0; ticks < TICKS_TO_WAIT_AT_MOST; ticks++) {
        if (machine.objects[0].actor.frame_number != 1) {
            return ticks;
        }
        script_motion_update_all(&machine);
    }
    return -1;
}

static int first_frame_ticks_at(unsigned int speed, unsigned int hold_ticks)
{
    load_two_frames_held_for(hold_ticks);
    set_the_speed(speed);
    start_the_table();
    return ticks_the_first_frame_shows();
}

static void test_the_op_is_routed_to_the_actor_family(void)
{
    CHECK_INT(script_opcode_family(0xae), SCRIPT_OP_ACTOR);
}

static void test_speeds_zero_and_one_leave_the_ticks_alone(void)
{
    CHECK_INT(first_frame_ticks_at(0, 4), 4);
    CHECK_INT(first_frame_ticks_at(1, 4), 4);
}

static void test_speed_two_halves_the_ticks(void)
{
    CHECK_INT(first_frame_ticks_at(2, 4), 2);
    CHECK_INT(first_frame_ticks_at(2, 8), 4);
}

static void test_speeds_above_two_quarter_the_ticks(void)
{
    CHECK_INT(first_frame_ticks_at(3, 8), 2);
    CHECK_INT(first_frame_ticks_at(4, 8), 2);
    CHECK_INT(first_frame_ticks_at(9, 8), 2);
}

static void test_a_shifted_frame_shows_for_one_tick_at_least(void)
{
    CHECK_INT(first_frame_ticks_at(4, 2), 1);
    CHECK_INT(first_frame_ticks_at(4, 3), 1);
    CHECK_INT(first_frame_ticks_at(2, 1), 1);
}

static void test_the_speed_stays_for_the_next_table(void)
{
    load_two_frames_held_for(4);
    set_the_speed(2);
    start_the_table();
    CHECK_INT(ticks_the_first_frame_shows(), 2);
    start_the_table();
    CHECK_INT(ticks_the_first_frame_shows(), 2);
}

static void test_speed_one_restores_the_ticks(void)
{
    load_two_frames_held_for(4);
    set_the_speed(2);
    set_the_speed(1);
    start_the_table();
    CHECK_INT(ticks_the_first_frame_shows(), 4);
}

static void test_a_frame_with_its_own_ticks_is_shifted_too(void)
{
    static const unsigned int table[] = { 0xfffd, 0, 0, 0xfffb, 1, 8, 2 | (4 << FRAME_HOLD_SHIFT), 0xffff };

    load_a_table(table, sizeof(table) / sizeof(table[0]));
    start_the_table();
    CHECK_INT(ticks_the_first_frame_shows(), 9);
    load_a_table(table, sizeof(table) / sizeof(table[0]));
    set_the_speed(2);
    start_the_table();
    CHECK_INT(ticks_the_first_frame_shows(), 5);
}

static void test_an_object_without_an_actor_keeps_no_speed(void)
{
    load_two_frames_held_for(4);
    machine.objects[0].actor.exists = 0;
    set_the_speed(2);
    CHECK_INT(machine.objects[0].actor.speed_shift, 0);
}

int main(void)
{
    test_the_op_is_routed_to_the_actor_family();
    test_speeds_zero_and_one_leave_the_ticks_alone();
    test_speed_two_halves_the_ticks();
    test_speeds_above_two_quarter_the_ticks();
    test_a_shifted_frame_shows_for_one_tick_at_least();
    test_the_speed_stays_for_the_next_table();
    test_speed_one_restores_the_ticks();
    test_a_frame_with_its_own_ticks_is_shifted_too();
    test_an_object_without_an_actor_keeps_no_speed();
    return test_finish("unit/motion_speed_shift");
}
