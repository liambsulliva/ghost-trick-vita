#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 16
#define TICKS_TO_RUN 3
#define OP_OBJECT 0x16
#define OP_JUMP 0x22
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define WORDS_BEFORE_THE_ROOT_GOES_ON 8

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];

static const char strings[] = "\0\0motor\0";

static const struct script_outputs outputs = { 0 };

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

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static void fill_every_slot(int deleted_slot)
{
    int index;

    machine.object_count = 0;
    for (index = 0; index < SCRIPT_OBJECT_MAX; index++) {
        struct script_object *object;

        object = &machine.objects[index];
        memset(object, 0, sizeof(*object));
        object->name = "fan";
        object->idle_handler = SCRIPT_NO_SELECTION;
        object->main_vm.has_ended = 1;
        object->is_deleted = index == deleted_slot;
        machine.object_count = machine.object_count + 1;
    }
}

static void run_a_script_that_creates_an_object(int deleted_slot)
{
    unsigned int words[WORD_MAX];
    unsigned int count;
    int tick;

    count = 0;
    words[count++] = OP_OBJECT;
    words[count++] = OP_JUMP;
    words[count++] = SCRIPT_FILE_HEADER_SIZE + 2 * WORDS_BEFORE_THE_ROOT_GOES_ON;
    words[count++] = 0;
    words[count++] = offset_of("motor");
    words[count++] = OP_WAIT;
    words[count++] = A_LONG_WAIT;
    words[count++] = OP_END;
    words[count++] = OP_WAIT;
    words[count++] = A_LONG_WAIT;
    words[count++] = OP_END;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    fill_every_slot(deleted_slot);
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_new_object_takes_a_deleted_slot_once_every_slot_is_used(void)
{
    run_a_script_that_creates_an_object(40);
    CHECK_INT(script_machine_index_of_object(&machine, "motor"), 40);
    CHECK_INT(machine.objects[40].is_deleted, 0);
    CHECK_INT(machine.object_count, SCRIPT_OBJECT_MAX);
}

static void test_a_new_object_is_refused_when_no_slot_is_free(void)
{
    run_a_script_that_creates_an_object(-1);
    CHECK_INT(script_machine_index_of_object(&machine, "motor"), SCRIPT_NO_OBJECT);
    CHECK_INT(machine.object_count, SCRIPT_OBJECT_MAX);
}

int main(void)
{
    test_a_new_object_takes_a_deleted_slot_once_every_slot_is_used();
    test_a_new_object_is_refused_when_no_slot_is_free();
    return test_finish("unit/object_slots");
}
