#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define ROW_Y 100
#define BODY_X 100
#define CORPSE_X 140
#define LAMP_X 60
#define BODY 0
#define CORPSE 1
#define LAMP 2
#define TICKS_TO_REACH_THE_CORPSE 200
#define TICKS_BEFORE_THE_LEAP_SWIRL 100
#define TICKS_TO_REACH_THE_LAMP 120

static struct script_machine machine;
static const struct script_outputs no_outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = ROW_Y;
}

static void start_in_the_ghost_view_beside_a_corpse(void)
{
    static const unsigned int words[] = { 0x2f, 60000, 0x0 };
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 3;
    place_core(BODY, "body", BODY_X);
    place_core(CORPSE, "corpse", CORPSE_X);
    place_core(LAMP, "lamp", LAMP_X);
    machine.objects[CORPSE].is_dead = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, BODY, 0);
    machine.soul.is_on = 1;
}

static void hold(float held_x, int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_set_held(&machine, held_x, 0.0f);
        script_soul_step_before_tasks(&machine);
        machine.tick = machine.tick + 1;
    }
}

static void hold_right(int ticks)
{
    hold(1.0f, ticks);
}

static void connect_to_the_corpse(void)
{
    int tick;

    for (tick = 0; tick < TICKS_TO_REACH_THE_CORPSE && !machine.soul.dead_core_holds_the_tip; tick++) {
        hold_right(1);
    }
    CHECK(machine.soul.dead_core_holds_the_tip);
    CHECK_INT(machine.soul.reached_object, CORPSE);
    CHECK_INT(machine.soul.active_object, BODY);
}

static void test_the_stick_cannot_pull_the_chain_out_while_the_corpse_holds_it(void)
{
    start_in_the_ghost_view_beside_a_corpse();
    connect_to_the_corpse();
    hold(-1.0f, TICKS_BEFORE_THE_LEAP_SWIRL);
    CHECK_INT(machine.soul.active_object, BODY);
    CHECK(!machine.soul.tip_is_out);
}

static void test_the_stick_moves_the_soul_again_once_the_view_reopens(void)
{
    start_in_the_ghost_view_beside_a_corpse();
    connect_to_the_corpse();
    hold_right(TICKS_BEFORE_THE_LEAP_SWIRL + 10);
    CHECK(!machine.soul.is_on);
    machine.soul.leap_message_at = -1;
    machine.pause_bit = 0;
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    machine.soul.is_on = 1;
    hold(-1.0f, TICKS_TO_REACH_THE_LAMP);
    CHECK_INT(machine.soul.active_object, LAMP);
}

int main(void)
{
    test_the_stick_cannot_pull_the_chain_out_while_the_corpse_holds_it();
    test_the_stick_moves_the_soul_again_once_the_view_reopens();
    return test_finish("unit/dead_core_holds_the_tip");
}
