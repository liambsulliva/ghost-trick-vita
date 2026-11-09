#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define LAMP_NAME_OFFSET 2
#define TELEPHONE_LINE_CLASS_LOW 0x2b66u
#define TELEPHONE_LINE_CLASS_HIGH 0x1906u
#define TELEPHONE_CORE_CLASS_LOW 0x65b4u
#define TELEPHONE_CORE_CLASS_HIGH 0x74c3u

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 + 8];
static const char strings[] = "\0\0lamp\0\0";
static struct script_vm vm;

static const unsigned int make_line[2] = { TELEPHONE_LINE_CLASS_LOW, TELEPHONE_LINE_CLASS_HIGH };
static const unsigned int make_core[2] = { TELEPHONE_CORE_CLASS_LOW, TELEPHONE_CORE_CLASS_HIGH };
static const unsigned int place_at_106_207[6] = { 0xa000, 0x6, 0xf000, 0xc, 0, 0 };
static const unsigned int place_at_30_40[6] = { 0xe000, 0x1, 0x8000, 0x2, 0, 0 };
static const unsigned int select_the_lamp[1] = { LAMP_NAME_OFFSET };
static const unsigned int no_words[1] = { 0 };

static void set_up_a_lamp_and_a_phone_script(void)
{
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
    machine.objects[0].actor.x = 300;
    machine.objects[0].actor.y = 150;
    machine.objects[1].actor.x = 100;
    machine.objects[1].actor.y = 50;
    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[1];
    vm.selected_object = SCRIPT_NO_SELECTION;
    vm.source = &source;
}

static int units_in_use(void)
{
    int unit;
    int count;

    count = 0;
    for (unit = 0; unit < SCRIPT_FREE_UNIT_MAX; unit++) {
        count = count + machine.free_units[unit].in_use;
    }
    return count;
}

static void test_placing_a_new_free_unit_leaves_the_owner_where_it_was(void)
{
    struct script_actor *line;

    set_up_a_lamp_and_a_phone_script();
    script_ops_actor_run(&machine, &vm, 0x82, make_line);
    script_ops_actor_run(&machine, &vm, 0x87, place_at_106_207);
    CHECK_INT(machine.objects[1].actor.x, 100);
    CHECK_INT(machine.objects[1].actor.y, 50);
    line = script_machine_current_actor(&machine, &vm);
    CHECK(line != NULL);
    CHECK(line != &machine.objects[1].actor);
    CHECK_INT(line->x, 106);
    CHECK_INT(line->y, 207);
    CHECK(script_machine_current_object(&machine, &vm) == NULL);
}

static void test_clearing_the_selection_hands_actor_ops_back_to_the_owner(void)
{
    set_up_a_lamp_and_a_phone_script();
    script_ops_actor_run(&machine, &vm, 0x82, make_line);
    script_ops_actor_run(&machine, &vm, 0x116, no_words);
    script_ops_actor_run(&machine, &vm, 0x87, place_at_30_40);
    CHECK_INT(machine.objects[1].actor.x, 30);
    CHECK_INT(machine.objects[1].actor.y, 40);
    CHECK_INT(units_in_use(), 1);
}

static void test_selecting_an_object_replaces_the_free_unit(void)
{
    set_up_a_lamp_and_a_phone_script();
    script_ops_actor_run(&machine, &vm, 0x82, make_line);
    script_ops_actor_run(&machine, &vm, 0x83, select_the_lamp);
    script_ops_actor_run(&machine, &vm, 0x87, place_at_30_40);
    CHECK_INT(machine.objects[0].actor.x, 30);
    CHECK_INT(machine.objects[0].actor.y, 40);
    CHECK_INT(machine.objects[1].actor.x, 100);
}

static void test_ending_a_class_ends_only_its_own_units(void)
{
    set_up_a_lamp_and_a_phone_script();
    script_ops_actor_run(&machine, &vm, 0x82, make_line);
    script_ops_actor_run(&machine, &vm, 0x116, no_words);
    script_ops_actor_run(&machine, &vm, 0x82, make_core);
    script_ops_actor_run(&machine, &vm, 0x116, no_words);
    script_ops_actor_run(&machine, &vm, 0x82, make_line);
    script_ops_actor_run(&machine, &vm, 0x116, no_words);
    CHECK_INT(units_in_use(), 3);
    script_ops_actor_run(&machine, &vm, 0x84, make_line);
    CHECK_INT(units_in_use(), 1);
}

static void test_a_full_set_of_units_still_keeps_the_owner_in_place(void)
{
    int unit;

    set_up_a_lamp_and_a_phone_script();
    for (unit = 0; unit < SCRIPT_FREE_UNIT_MAX; unit++) {
        script_ops_actor_run(&machine, &vm, 0x82, make_core);
        script_ops_actor_run(&machine, &vm, 0x116, no_words);
    }
    script_ops_actor_run(&machine, &vm, 0x82, make_core);
    script_ops_actor_run(&machine, &vm, 0x87, place_at_106_207);
    CHECK(script_machine_current_actor(&machine, &vm) == NULL);
    CHECK_INT(machine.objects[1].actor.x, 100);
    CHECK_INT(machine.objects[1].actor.y, 50);
}

int main(void)
{
    test_placing_a_new_free_unit_leaves_the_owner_where_it_was();
    test_clearing_the_selection_hands_actor_ops_back_to_the_owner();
    test_selecting_an_object_replaces_the_free_unit();
    test_ending_a_class_ends_only_its_own_units();
    test_a_full_set_of_units_still_keeps_the_owner_in_place();
    return test_finish("unit/free_unit");
}
