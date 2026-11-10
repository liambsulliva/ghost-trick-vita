#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 16
#define TICKS_TO_RUN 3
#define FIRST_CORE_BIT 1u
#define SECOND_CORE_BIT 2u

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

static void add_object(const char *name, unsigned int busy_cores)
{
    struct script_object *object;

    object = &machine.objects[machine.object_count];
    memset(object, 0, sizeof(*object));
    object->name = name;
    object->idle_handler = SCRIPT_NO_SELECTION;
    object->main_vm.has_ended = 1;
    object->actor.exists = 1;
    object->actor.has_core = 1;
    object->actor.busy_cores = busy_cores;
    machine.object_count = machine.object_count + 1;
}

static void run_a_script_that_hands_the_trick_back_to_another_object(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;
    int tick;

    count = 0;
    words[count++] = 0x83;
    words[count++] = offset_of("motor");
    words[count++] = 0xe9;
    words[count++] = 0;
    words[count++] = 0xea;
    words[count++] = 1;
    words[count++] = 0xe1;
    words[count++] = 0x116;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 0;
    add_object("fan", FIRST_CORE_BIT);
    add_object("motor", FIRST_CORE_BIT);
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_turning_a_trick_on_reaches_the_selected_object(void)
{
    run_a_script_that_hands_the_trick_back_to_another_object();
    CHECK_INT((int)(machine.objects[1].actor.busy_cores & FIRST_CORE_BIT), 0);
}

static void test_turning_a_trick_off_reaches_the_selected_object(void)
{
    run_a_script_that_hands_the_trick_back_to_another_object();
    CHECK_INT((int)(machine.objects[1].actor.busy_cores & SECOND_CORE_BIT), (int)SECOND_CORE_BIT);
}

static void test_the_object_running_the_script_keeps_its_own_tricks(void)
{
    run_a_script_that_hands_the_trick_back_to_another_object();
    CHECK_INT((int)machine.objects[0].actor.busy_cores, (int)FIRST_CORE_BIT);
}

static void test_marking_a_corpse_reaches_the_selected_object(void)
{
    run_a_script_that_hands_the_trick_back_to_another_object();
    CHECK_INT(machine.objects[1].is_dead, 1);
    CHECK_INT(machine.objects[0].is_dead, 0);
}

int main(void)
{
    test_turning_a_trick_on_reaches_the_selected_object();
    test_turning_a_trick_off_reaches_the_selected_object();
    test_the_object_running_the_script_keeps_its_own_tricks();
    test_marking_a_corpse_reaches_the_selected_object();
    return test_finish("unit/core_trick_toggle");
}
