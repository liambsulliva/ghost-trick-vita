#include "script_machine.h"
#include "test_support.h"

#define PILL_IS_SHOWN 1

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void test_select_asks_for_the_record_while_the_player_has_the_controls(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
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

int main(void)
{
    test_select_asks_for_the_record_while_the_player_has_the_controls();
    test_select_does_nothing_while_a_scene_has_the_controls();
    test_the_hourglass_does_not_ask_for_the_record();
    return test_finish("unit/select_wants_the_record");
}
