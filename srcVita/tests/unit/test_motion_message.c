#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define FRAME_HOLD_SHIFT 10
#define TARGET_OFFSET 2
#define VERB_OFFSET 8
#define ARG_OFFSET 14
#define TABLE_WORDS 9
#define TICKS_FOR_ONE_LOOP 4

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static const char strings[] = "\0\0tapeT\0EVENT\0fire\0";
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + TABLE_WORDS * 2 + sizeof(strings) - 1];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_with_a_table_that_sends_a_message(void)
{
    static const unsigned int table[TABLE_WORDS] = { 0xfffd, 0, 0, 0xfff9, TARGET_OFFSET, VERB_OFFSET, ARG_OFFSET, 1 | (1u << FRAME_HOLD_SHIFT), 0xffff };
    unsigned int index;

    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)(TABLE_WORDS * 2);
    bytes[0x0c] = (unsigned char)(sizeof(strings) - 1);
    for (index = 0; index < TABLE_WORDS; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memcpy(&bytes[SCRIPT_FILE_HEADER_SIZE + TABLE_WORDS * 2], strings, sizeof(strings) - 1);
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[0].name = "fan";
    machine.objects[1].name = "tapeT";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
}

static void test_a_message_record_sends_its_message_to_the_named_object(void)
{
    int tick;

    start_with_a_table_that_sends_a_message();
    for (tick = 0; tick < TICKS_FOR_ONE_LOOP; tick++) {
        script_motion_update_all(&machine);
    }
    CHECK(machine.objects[1].message_count >= 1);
    CHECK_TEXT(machine.objects[1].messages[0].verb, "EVENT");
    CHECK_TEXT(machine.objects[1].messages[0].arg, "fire");
    CHECK_INT(machine.objects[0].message_count, 0);
}

int main(void)
{
    test_a_message_record_sends_its_message_to_the_named_object();
    return test_finish("unit/motion_message");
}
