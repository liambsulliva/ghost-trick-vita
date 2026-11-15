#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define DEMO_MODE 2
#define NORMAL_PLAY_MODE 3
#define STEPS_FOR_A_PILL_TO_SLIDE_IN 40
#define TICKS_TO_LEAVE_THE_VIEW 40
#define TICKS_FOR_THE_TRICK_ACTION 8
#define TICKS_OF_THE_SCRIPTED_SEQUENCE 60
#define CORE_INFO_WITHOUT_A_TRICK 238
#define NOT_FOUND -1

static struct script_machine machine;
static const struct script_outputs no_outputs;
static const int trickless_core_infos[] = { CORE_INFO_WITHOUT_A_TRICK };

static void step(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_step_before_tasks(&machine);
        machine.tick = machine.tick + 1;
    }
}

static void start_in_the_ghost_view_on_a_core(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "lamp";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.is_visible = 1;
    machine.objects[0].actor.has_core = 1;
    machine.trickless_core_infos = trickless_core_infos;
    machine.trickless_core_info_count = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", "All:On");
    step(STEPS_FOR_A_PILL_TO_SLIDE_IN);
    script_soul_note_ghost(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW + STEPS_FOR_A_PILL_TO_SLIDE_IN);
    machine.objects[0].message_count = 0;
}

static int message_at(const char *verb, const char *arg)
{
    int index;

    for (index = 0; index < machine.objects[0].message_count; index++) {
        if (strcmp(machine.objects[0].messages[index].verb, verb) == 0 && strcmp(machine.objects[0].messages[index].arg, arg) == 0) {
            return index;
        }
    }
    return NOT_FOUND;
}

static void test_the_ghost_view_shows_the_trick_pill_for_a_core_with_a_trick(void)
{
    start_in_the_ghost_view_on_a_core();
    CHECK(machine.soul.is_on);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_BACK));
}

static void test_cross_in_the_ghost_view_leaves_it_and_then_tricks(void)
{
    start_in_the_ghost_view_on_a_core();
    script_machine_note_press(&machine);
    step(1);
    CHECK(machine.soul.is_leaving_view);
    CHECK_INT(machine.soul.tricks_fired, 0);
    CHECK_INT(message_at("core", "ready"), NOT_FOUND);
    step(TICKS_TO_LEAVE_THE_VIEW);
    CHECK(!machine.soul.is_on);
    CHECK(!machine.soul.is_leaving_view);
    CHECK_INT(machine.mode_byte, NORMAL_PLAY_MODE);
    CHECK_INT(machine.soul.tricks_fired, 1);
    CHECK(message_at("SYSTEM", "SV_END") != NOT_FOUND);
    CHECK(message_at("core", "ready") > message_at("SYSTEM", "SV_END"));
    step(TICKS_FOR_THE_TRICK_ACTION);
    CHECK(message_at("core", "action") > message_at("core", "ready"));
}

static void test_the_trick_pill_in_the_ghost_view_does_the_same(void)
{
    start_in_the_ghost_view_on_a_core();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    script_machine_note_press(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW + TICKS_FOR_THE_TRICK_ACTION);
    CHECK_INT(machine.soul.tricks_fired, 1);
    CHECK(message_at("core", "action") != NOT_FOUND);
}

static void test_a_scripted_sequence_started_by_the_trick_keeps_its_state(void)
{
    int message_count_at_the_action;

    start_in_the_ghost_view_on_a_core();
    script_machine_note_press(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW + TICKS_FOR_THE_TRICK_ACTION);
    CHECK(message_at("core", "action") != NOT_FOUND);
    message_count_at_the_action = machine.objects[0].message_count;
    machine.mode_byte = DEMO_MODE;
    script_soul_run_command(&machine, "SoulVision", "Start");
    step(TICKS_OF_THE_SCRIPTED_SEQUENCE);
    CHECK_INT(machine.mode_byte, DEMO_MODE);
    CHECK(machine.soul.is_on);
    CHECK(!machine.soul.is_leaving_view);
    CHECK_INT(machine.soul.tricks_fired, 1);
    CHECK(machine.objects[0].message_count >= message_count_at_the_action);
    CHECK(message_at("SYSTEM", "SV_FINAL") < message_count_at_the_action);
}

static void test_a_busy_core_keeps_the_ghost_view_and_hides_the_pill(void)
{
    start_in_the_ghost_view_on_a_core();
    machine.objects[0].actor.busy_cores = 1;
    step(STEPS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    script_machine_note_press(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW);
    CHECK(machine.soul.is_on);
    CHECK_INT(machine.soul.tricks_fired, 0);
}

static void test_a_core_without_a_trick_keeps_the_ghost_view(void)
{
    start_in_the_ghost_view_on_a_core();
    machine.objects[0].actor.core_infos[0] = CORE_INFO_WITHOUT_A_TRICK;
    step(STEPS_FOR_A_PILL_TO_SLIDE_IN);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    script_machine_note_press(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW);
    CHECK(machine.soul.is_on);
    CHECK_INT(machine.soul.tricks_fired + machine.soul.tricks_refused, 0);
}

static void test_circle_still_leaves_without_a_trick(void)
{
    start_in_the_ghost_view_on_a_core();
    script_soul_note_ghost(&machine);
    step(TICKS_TO_LEAVE_THE_VIEW + TICKS_FOR_THE_TRICK_ACTION);
    CHECK(!machine.soul.is_on);
    CHECK_INT(machine.soul.tricks_fired, 0);
}

int main(void)
{
    test_the_ghost_view_shows_the_trick_pill_for_a_core_with_a_trick();
    test_cross_in_the_ghost_view_leaves_it_and_then_tricks();
    test_the_trick_pill_in_the_ghost_view_does_the_same();
    test_a_scripted_sequence_started_by_the_trick_keeps_its_state();
    test_a_busy_core_keeps_the_ghost_view_and_hides_the_pill();
    test_a_core_without_a_trick_keeps_the_ghost_view();
    test_circle_still_leaves_without_a_trick();
    return test_finish("unit/trick_from_ghost_view");
}
