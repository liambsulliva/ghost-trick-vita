#include <string.h>
#include "script_classes.h"
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define GMK_COMMON_CLASS 0x789c5740u
#define NORMAL_PLAY_MODE 3
#define CORE_INFO_WITH_A_TRICK 89
#define CORE_INFO_WITHOUT_A_TRICK 238
#define TRICK_REFUSED_SOUND_KIND 4
#define NO_SOUND -1
#define STEPS_FOR_THE_PILL_TO_RISE 120
#define CORE_VALUE_CORE_SHIFT 12
#define FRAME_HOLD_SHIFT 10
#define TABLE_WORDS 7
#define TICKS_TO_PLAY_THE_TABLE 4

static struct script_machine machine;
static int last_sound;
static const int trickless_core_infos[] = { CORE_INFO_WITHOUT_A_TRICK };

static void on_sound(int kind)
{
    last_sound = kind;
}

static const struct script_outputs outputs = { .sound_played = on_sound };
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + TABLE_WORDS * 2];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void set_up_a_gimmick(unsigned int class_id)
{
    int step;

    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "tree";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.is_visible = 1;
    machine.objects[0].actor.has_core = 1;
    machine.objects[0].actor.class_id = class_id;
    machine.trickless_core_infos = trickless_core_infos;
    machine.trickless_core_info_count = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    machine.soul.trick_is_on = 1;
    script_soul_set_first_core(&machine, 0, 0);
    script_pills_register(&machine, SCRIPT_PILL_TRICK, 1);
    for (step = 0; step < STEPS_FOR_THE_PILL_TO_RISE; step++) {
        script_pills_step(&machine);
    }
    last_sound = NO_SOUND;
}

static void run_core_info_op(unsigned int core, unsigned int core_info)
{
    struct script_vm vm;
    unsigned int words[4] = { 0, 0, 0, 0 };

    memset(&vm, 0, sizeof(vm));
    vm.owner = &machine.objects[0];
    vm.selected_object = SCRIPT_NO_SELECTION;
    words[0] = core;
    words[1] = core_info;
    CHECK_INT(script_opcode_family(0x9f), SCRIPT_OP_ACTOR);
    script_ops_actor_run(&machine, &vm, 0x9f, words);
}

static void press_trick(void)
{
    script_machine_note_press(&machine);
    script_soul_step_before_tasks(&machine);
}

static void test_the_op_names_the_core_info_of_a_gimmick(void)
{
    set_up_a_gimmick(GMK_COMMON_CLASS);
    run_core_info_op(0, CORE_INFO_WITHOUT_A_TRICK);
    CHECK_INT(machine.objects[0].actor.core_infos[0], CORE_INFO_WITHOUT_A_TRICK);
    run_core_info_op(1, CORE_INFO_WITH_A_TRICK);
    CHECK_INT(machine.objects[0].actor.core_infos[1], CORE_INFO_WITH_A_TRICK);
}

static void test_the_op_leaves_an_object_that_is_not_a_gimmick(void)
{
    set_up_a_gimmick(SCRIPT_CLASS_HUM_COMMON);
    run_core_info_op(0, CORE_INFO_WITHOUT_A_TRICK);
    CHECK_INT(machine.objects[0].actor.core_infos[0], 0);
}

static void test_a_core_whose_info_names_no_trick_refuses_the_press(void)
{
    set_up_a_gimmick(GMK_COMMON_CLASS);
    run_core_info_op(0, CORE_INFO_WITHOUT_A_TRICK);
    press_trick();
    CHECK_INT(machine.soul.tricks_fired, 0);
    CHECK_INT(machine.soul.tricks_refused, 1);
    CHECK_INT(last_sound, TRICK_REFUSED_SOUND_KIND);
}

static void test_a_core_whose_info_names_a_trick_fires_it(void)
{
    set_up_a_gimmick(GMK_COMMON_CLASS);
    run_core_info_op(0, CORE_INFO_WITHOUT_A_TRICK);
    run_core_info_op(0, CORE_INFO_WITH_A_TRICK);
    press_trick();
    CHECK_INT(machine.soul.tricks_fired, 1);
    CHECK_INT(last_sound, NO_SOUND);
}

static void queue_a_table_that_names_a_core_info(unsigned int core, unsigned int core_info)
{
    unsigned int table[TABLE_WORDS] = { 0xfffd, 0, 0, 0xfff6, 0, 1u | (1u << FRAME_HOLD_SHIFT), 0xffff };
    unsigned int index;

    table[4] = (core << CORE_VALUE_CORE_SHIFT) | core_info;
    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)(TABLE_WORDS * 2);
    for (index = 0; index < TABLE_WORDS; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, sizeof(bytes)), SCRIPT_FILE_OK);
    machine.objects[0].main_vm.source = &source;
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
}

static void play_the_table(void)
{
    int tick;

    for (tick = 0; tick < TICKS_TO_PLAY_THE_TABLE; tick++) {
        script_motion_update_all(&machine);
    }
}

static void test_a_motion_table_names_the_core_info_of_a_gimmick(void)
{
    set_up_a_gimmick(GMK_COMMON_CLASS);
    run_core_info_op(0, CORE_INFO_WITHOUT_A_TRICK);
    queue_a_table_that_names_a_core_info(0, CORE_INFO_WITH_A_TRICK);
    play_the_table();
    CHECK_INT(machine.objects[0].actor.core_infos[0], CORE_INFO_WITH_A_TRICK);
    press_trick();
    CHECK_INT(machine.soul.tricks_fired, 1);
}

static void test_a_motion_table_leaves_the_core_info_of_a_human(void)
{
    set_up_a_gimmick(SCRIPT_CLASS_HUM_COMMON);
    queue_a_table_that_names_a_core_info(0, CORE_INFO_WITHOUT_A_TRICK);
    play_the_table();
    CHECK_INT(machine.objects[0].actor.core_infos[0], 0);
}

int main(void)
{
    test_a_motion_table_names_the_core_info_of_a_gimmick();
    test_a_motion_table_leaves_the_core_info_of_a_human();
    test_the_op_names_the_core_info_of_a_gimmick();
    test_the_op_leaves_an_object_that_is_not_a_gimmick();
    test_a_core_whose_info_names_no_trick_refuses_the_press();
    test_a_core_whose_info_names_a_trick_fires_it();
    return test_finish("unit/core_trick_info");
}
