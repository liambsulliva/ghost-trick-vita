#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define FRAME_HOLD_SHIFT 10
#define FIRST_FRAME 117
#define LAST_FRAME 125
#define FRAME_COUNT (LAST_FRAME - FIRST_FRAME + 1)
#define TABLE_WORDS (3 + FRAME_COUNT + 1)
#define TICKS_FOR_TWO_LOOPS (FRAME_COUNT * 2 + 4)

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + TABLE_WORDS * 2];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_with_frames_that_loop(void)
{
    unsigned int table[TABLE_WORDS];
    unsigned int index;

    table[0] = 0xfffd;
    table[1] = 0;
    table[2] = 0;
    for (index = 0; index < FRAME_COUNT; index++) {
        table[3 + index] = (FIRST_FRAME + index) | (1u << FRAME_HOLD_SHIFT);
    }
    table[TABLE_WORDS - 1] = 0xffff;
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
    machine.objects[0].name = "santa";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
}

static void test_the_frame_register_reads_the_frame_number_every_loop(void)
{
    int tick;
    int times_on_the_last_frame;
    int lowest;

    start_with_frames_that_loop();
    times_on_the_last_frame = 0;
    lowest = LAST_FRAME;
    for (tick = 0; tick < TICKS_FOR_TWO_LOOPS; tick++) {
        script_motion_update_all(&machine);
        if (machine.objects[0].actor.frame_number == LAST_FRAME) {
            times_on_the_last_frame = times_on_the_last_frame + 1;
        }
        if (machine.objects[0].actor.frame_number < lowest) {
            lowest = machine.objects[0].actor.frame_number;
        }
    }
    CHECK(times_on_the_last_frame >= 2);
    CHECK_INT(lowest, FIRST_FRAME);
}

int main(void)
{
    test_the_frame_register_reads_the_frame_number_every_loop();
    return test_finish("unit/frame_number_register");
}
