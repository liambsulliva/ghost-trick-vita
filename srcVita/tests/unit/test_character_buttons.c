#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define STEPS_FOR_A_PILL_TO_SLIDE_IN 40
#define NORMAL_PLAY_MODE 3

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void start_in_normal_play_with_two_cores(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[0].name = "cell";
    machine.objects[1].name = "spoon";
    machine.objects[0].actor.exists = 1;
    machine.objects[1].actor.exists = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
}

static void step_until_up(enum script_pill_id id)
{
    int step;

    for (step = 0; step < STEPS_FOR_A_PILL_TO_SLIDE_IN && !script_pills_is_up(&machine, id); step++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void press_the_character_button(void)
{
    script_soul_note_character_button(&machine);
    script_soul_step_before_tasks(&machine);
}

static int object_got(int object_index, const char *verb, const char *arg)
{
    int index;
    const struct script_object *object;

    object = &machine.objects[object_index];
    for (index = 0; index < object->message_count; index++) {
        if (strcmp(object->messages[index].verb, verb) == 0 && strcmp(object->messages[index].arg, arg) == 0) {
            return 1;
        }
    }
    return 0;
}

static void test_the_jowd_button_calls_jowd_to_every_object(void)
{
    start_in_normal_play_with_two_cores();
    script_soul_run_command(&machine, "Button", "Jawed:On");
    step_until_up(SCRIPT_PILL_JOWD);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_JOWD));
    press_the_character_button();
    CHECK(object_got(0, "JAWED", "CALL"));
    CHECK(object_got(1, "JAWED", "CALL"));
}

static void test_the_jowd_button_is_gone_once_turned_off(void)
{
    start_in_normal_play_with_two_cores();
    script_soul_run_command(&machine, "Button", "Jawed:On");
    script_soul_run_command(&machine, "Button", "Jawed:Off");
    step_until_up(SCRIPT_PILL_JOWD);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_JOWD));
}

static void test_the_switch_swaps_ghosts_in_the_ghost_view_and_each_keeps_its_core(void)
{
    start_in_normal_play_with_two_cores();
    script_soul_run_command(&machine, "Button", "Missile:On");
    script_soul_run_command(&machine, "Button", "Cicel:On");
    machine.soul.is_on = 1;
    step_until_up(SCRIPT_PILL_MISSILE);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_MISSILE));
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_CICEL));
    press_the_character_button();
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_MISSILE);
    script_soul_set_first_core(&machine, 1, 0);
    step_until_up(SCRIPT_PILL_CICEL);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_CICEL));
    press_the_character_button();
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_CICEL);
    CHECK_INT(machine.soul.active_object, 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_the_switch_needs_both_character_buttons(void)
{
    start_in_normal_play_with_two_cores();
    script_soul_run_command(&machine, "Button", "Missile:On");
    machine.soul.is_on = 1;
    step_until_up(SCRIPT_PILL_MISSILE);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_MISSILE));
}

static void test_exceptions_choose_the_active_ghost(void)
{
    start_in_normal_play_with_two_cores();
    script_soul_run_command(&machine, "Exception", "Missile:On");
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_MISSILE);
    script_soul_run_command(&machine, "Exception", "Boss:On");
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_BOSS);
    script_soul_run_command(&machine, "Exception", "Cicel:On");
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_CICEL);
    script_soul_run_command(&machine, "Exception", "Missile:On");
    script_soul_run_command(&machine, "Exception", "Missile:Off");
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_CICEL);
}

int main(void)
{
    test_the_jowd_button_calls_jowd_to_every_object();
    test_the_jowd_button_is_gone_once_turned_off();
    test_the_switch_swaps_ghosts_in_the_ghost_view_and_each_keeps_its_core();
    test_the_switch_needs_both_character_buttons();
    test_exceptions_choose_the_active_ghost();
    return test_finish("unit/character_buttons");
}
