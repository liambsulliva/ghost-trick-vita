#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 16
#define TICKS_TO_RUN 3

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];

static const char strings[] = "\0\0@demo_\0memo1\0";

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

static void add_object(const char *name)
{
    struct script_object *object;

    object = &machine.objects[machine.object_count];
    memset(object, 0, sizeof(*object));
    object->name = name;
    object->idle_handler = SCRIPT_NO_SELECTION;
    object->main_vm.has_ended = 1;
    object->actor.exists = 1;
    machine.object_count = machine.object_count + 1;
}

static void run_a_script_that_deletes_objects(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;
    int tick;

    count = 0;
    words[count++] = 0x17;
    words[count++] = offset_of("@demo_");
    words[count++] = 0x17;
    words[count++] = offset_of("memo1");
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 0;
    add_object("demo_hum1");
    add_object("st13_demo_gim2");
    add_object("memo1");
    add_object("memo2");
    add_object("stage_master");
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_partial_name_deletes_every_object_whose_name_holds_it(void)
{
    run_a_script_that_deletes_objects();
    CHECK(machine.objects[0].is_deleted);
    CHECK(machine.objects[1].is_deleted);
    CHECK(!machine.objects[0].actor.exists);
}

static void test_a_full_name_deletes_only_that_object(void)
{
    run_a_script_that_deletes_objects();
    CHECK(machine.objects[2].is_deleted);
    CHECK(!machine.objects[3].is_deleted);
    CHECK(!machine.objects[4].is_deleted);
}

static void test_a_deleted_object_is_no_longer_found_by_name(void)
{
    run_a_script_that_deletes_objects();
    CHECK_INT(script_machine_index_of_object(&machine, "memo1"), SCRIPT_NO_OBJECT);
    CHECK_INT(script_machine_index_of_object(&machine, "memo2"), 3);
}

int main(void)
{
    test_a_partial_name_deletes_every_object_whose_name_holds_it();
    test_a_full_name_deletes_only_that_object();
    test_a_deleted_object_is_no_longer_found_by_name();
    return test_finish("unit/object_delete");
}
