#include "script_machine.h"
#include "test_support.h"

#define PILL_IS_SHOWN 1

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void test_select_asks_for_the_record_while_the_player_has_the_controls(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = SCRIPT_MODE_NORMAL;
    machine.soul.pills[SCRIPT_PILL_GHOST].state = PILL_IS_SHOWN;
    script_soul_note_pill(&machine, SCRIPT_PILL_NOTEBOOK);
    CHECK_INT(machine.record_is_wanted, 1);
}

static void test_select_does_nothing_while_a_scene_has_the_controls(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    script_soul_note_pill(&machine, SCRIPT_PILL_NOTEBOOK);
    CHECK_INT(machine.record_is_wanted, 0);
}

static void test_the_hourglass_does_not_ask_for_the_record(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.soul.pills[SCRIPT_PILL_GHOST].state = PILL_IS_SHOWN;
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    CHECK_INT(machine.record_is_wanted, 0);
}

static void test_a_scripted_event_takes_the_pause_and_the_record_away(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = SCRIPT_MODE_NORMAL;
    machine.soul.rollback_is_on = 1;
    machine.soul.pills[SCRIPT_PILL_HOURGLASS].state = PILL_IS_SHOWN;
    CHECK(script_pills_player_has_control(&machine));
    machine.mode_byte = SCRIPT_MODE_DEMO;
    CHECK(!script_pills_player_has_control(&machine));
    script_soul_note_pill(&machine, SCRIPT_PILL_NOTEBOOK);
    CHECK_INT(machine.record_is_wanted, 0);
    machine.mode_byte = SCRIPT_MODE_NORMAL;
    machine.soul.is_on = 1;
    CHECK(script_pills_player_has_control(&machine));
    machine.soul.view_demo_is_on = 1;
    CHECK(!script_pills_player_has_control(&machine));
    machine.soul.view_demo_is_on = 0;
    machine.demo_soul_is_on = 1;
    CHECK(!script_pills_player_has_control(&machine));
}

int main(void)
{
    test_select_asks_for_the_record_while_the_player_has_the_controls();
    test_select_does_nothing_while_a_scene_has_the_controls();
    test_the_hourglass_does_not_ask_for_the_record();
    test_a_scripted_event_takes_the_pause_and_the_record_away();
    return test_finish("unit/select_wants_the_record");
}
