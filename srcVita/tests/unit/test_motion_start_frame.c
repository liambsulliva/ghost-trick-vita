#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define FRAME_HOLD_SHIFT 10
#define FIRST_FRAME 10
#define START_FRAME 12
#define POINT_ONE_X 7
#define POINT_ONE_Y 9
#define TABLE_WORDS 22

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + TABLE_WORDS * 2];
static struct script_vm vm;

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void queue_a_table_with_a_point_between_its_frames(void)
{
    static const unsigned int table[TABLE_WORDS] = {
        0xfffd, 0, 0,
        0xfffc, 0, 0, 0, 0,
        FIRST_FRAME | (1u << FRAME_HOLD_SHIFT),
        0xfff5, 1, POINT_ONE_X, POINT_ONE_Y,
        (FIRST_FRAME + 1) | (1u << FRAME_HOLD_SHIFT),
        START_FRAME | (1u << FRAME_HOLD_SHIFT),
        (START_FRAME + 1) | (1u << FRAME_HOLD_SHIFT),
        0xfffe, 0, 0, 0, 0, 0
    };
    unsigned int index;

    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)(TABLE_WORDS * 2);
    for (index = 0; index < TABLE_WORDS; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "gun";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[0];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
}

static void test_a_table_starts_on_its_first_frame_by_default(void)
{
    queue_a_table_with_a_point_between_its_frames();
    script_motion_update_all(&machine);
    CHECK_INT(machine.objects[0].actor.frame_number, FIRST_FRAME);
}

static void test_a_start_frame_skips_the_frames_before_it_and_keeps_their_points(void)
{
    static const unsigned int start_at[1] = { START_FRAME };

    queue_a_table_with_a_point_between_its_frames();
    script_ops_actor_run(&machine, &vm, 0x118, start_at);
    script_motion_update_all(&machine);
    CHECK_INT(machine.objects[0].actor.frame_number, START_FRAME);
    CHECK_INT(machine.objects[0].actor.points[1][0], POINT_ONE_X);
    CHECK_INT(machine.objects[0].actor.points[1][1], POINT_ONE_Y);
    CHECK_INT(machine.objects[0].actor.starts_at_frame, 0);
    script_motion_update_all(&machine);
    CHECK_INT(machine.objects[0].actor.frame_number, START_FRAME + 1);
}

int main(void)
{
    test_a_table_starts_on_its_first_frame_by_default();
    test_a_start_frame_skips_the_frames_before_it_and_keeps_their_points();
    return test_finish("unit/motion_start_frame");
}
