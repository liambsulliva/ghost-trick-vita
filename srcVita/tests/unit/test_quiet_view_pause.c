#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define TICKS_FOR_THE_SCENE_TO_START 60
#define TICKS_BEFORE_THE_SCENE_ASKS_FOR_DEMO_MODE 200
#define DEMO_MODE 2
#define NORMAL_PLAY_MODE 3

static struct script_machine machine;
static const struct script_outputs no_outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void start_a_scene(const unsigned int *words, unsigned int count)
{
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], count * 2);
    for (index = 0; index < count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + count * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    run_ticks(TICKS_FOR_THE_SCENE_TO_START);
}

static void start_an_idle_scene(void)
{
    static const unsigned int words[] = { 0x2f, 6000, 0x0 };

    start_a_scene(words, sizeof(words) / sizeof(words[0]));
}

static void start_a_scene_that_asks_for_demo_mode_later(void)
{
    static const unsigned int words[] = { 0x2f, TICKS_BEFORE_THE_SCENE_ASKS_FOR_DEMO_MODE, 0x68, 0x2f, 6000, 0x0 };

    start_a_scene(words, sizeof(words) / sizeof(words[0]));
}

static void test_a_view_opened_quietly_never_pauses_the_world(void)
{
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    CHECK_INT(machine.pause_bit, 0);
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOn");
    CHECK_INT(machine.pause_bit, 0);
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOff");
    CHECK_INT(machine.pause_bit, 0);
    CHECK_INT(machine.soul.is_on, 1);
}

static void test_a_view_opened_with_its_effect_keeps_its_pause_through_demo_mode(void)
{
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    CHECK_INT(machine.pause_bit, 1);
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOn");
    CHECK_INT(machine.pause_bit, 1);
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOff");
    CHECK_INT(machine.pause_bit, 1);
}

static void test_the_mode_a_script_asks_for_outlasts_a_return_sent_the_same_tick(void)
{
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    script_machine_send(&machine, "GameApp", "CustomSoulVision", "ReturnDemo");
    script_machine_tick(&machine);
    CHECK_INT(machine.mode_byte, DEMO_MODE);
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    script_machine_send(&machine, "GameApp", "CustomSoulVision", "ReturnDemo");
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.mode_a_script_asked_for = NORMAL_PLAY_MODE;
    script_machine_tick(&machine);
    CHECK_INT(machine.mode_byte, NORMAL_PLAY_MODE);
    CHECK_INT(machine.mode_a_script_asked_for, 0);
}

static void test_aborting_a_dead_talk_leaves_the_view_for_demo_mode_and_restarting_reopens_it(void)
{
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    machine.soul.dead_talk_is_on = 1;
    CHECK_INT(machine.pause_bit, 1);
    script_soul_run_command(&machine, "DeadTalk", "Abort");
    CHECK_INT(machine.soul.dead_talk_is_on, 0);
    CHECK_INT(machine.soul.is_on, 0);
    CHECK_INT(machine.pause_bit, 0);
    CHECK_INT(machine.mode_byte, 2);
    script_soul_run_command(&machine, "DeadTalk", "Restart");
    CHECK_INT(machine.soul.is_on, 1);
    CHECK_INT(machine.soul.dead_talk_is_on, 1);
    CHECK_INT(machine.pause_bit, 0);
}

static void test_asking_for_demo_mode_closes_an_open_view_and_frees_the_world(void)
{
    start_a_scene_that_asks_for_demo_mode_later();
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectFinish");
    CHECK_INT(machine.pause_bit, 1);
    run_ticks(TICKS_BEFORE_THE_SCENE_ASKS_FOR_DEMO_MODE);
    CHECK_INT(machine.soul.is_on, 0);
    CHECK_INT(machine.soul.is_leaving_view, 0);
    CHECK_INT(machine.pause_bit, 0);
    CHECK_INT(machine.mode_byte, DEMO_MODE);
}

static void test_a_view_opened_after_demo_mode_was_asked_for_stays_open(void)
{
    start_an_idle_scene();
    machine.mode_a_script_asked_for = DEMO_MODE;
    script_soul_run_command(&machine, "System", "ChangeMode");
    script_soul_run_command(&machine, "CustomSoulVision", "Start");
    script_machine_tick(&machine);
    CHECK_INT(machine.soul.is_on, 1);
    CHECK_INT(machine.mode_byte, DEMO_MODE);
}

static void test_a_view_a_script_reopens_quietly_waits_on_the_player(void)
{
    start_an_idle_scene();
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOn");
    CHECK_INT(script_soul_waits_on_the_player(&machine), 0);
    script_soul_run_command(&machine, "CustomSoulVision", "DemoModeOff");
    CHECK_INT(script_soul_waits_on_the_player(&machine), 1);
}

int main(void)
{
    test_a_view_opened_quietly_never_pauses_the_world();
    test_a_view_opened_with_its_effect_keeps_its_pause_through_demo_mode();
    test_the_mode_a_script_asks_for_outlasts_a_return_sent_the_same_tick();
    test_aborting_a_dead_talk_leaves_the_view_for_demo_mode_and_restarting_reopens_it();
    test_asking_for_demo_mode_closes_an_open_view_and_frees_the_world();
    test_a_view_opened_after_demo_mode_was_asked_for_stays_open();
    test_a_view_a_script_reopens_quietly_waits_on_the_player();
    return test_finish("unit/quiet_view_pause");
}
