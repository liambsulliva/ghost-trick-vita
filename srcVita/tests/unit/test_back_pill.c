#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define STEPS_FOR_A_PILL_TO_SLIDE_IN 40

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void start_in_the_ghost_view_after(const char *button_command)
{
    int step;

    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "bed";
    machine.objects[0].actor.exists = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", "All:Off");
    script_soul_run_command(&machine, "Button", "Action:On");
    script_soul_run_command(&machine, "Button", button_command);
    machine.soul.is_on = 1;
    for (step = 0; step < STEPS_FOR_A_PILL_TO_SLIDE_IN; step++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void test_the_trick_button_offers_back_in_the_ghost_view(void)
{
    start_in_the_ghost_view_after("Trick:On");
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_BACK));
    script_soul_note_ghost(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK(!machine.soul.is_on || machine.soul.is_leaving_view);
}

static void test_without_the_trick_button_the_ghost_view_holds_the_player(void)
{
    start_in_the_ghost_view_after("Trick:Off");
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_BACK));
}

int main(void)
{
    test_the_trick_button_offers_back_in_the_ghost_view();
    test_without_the_trick_button_the_ghost_view_holds_the_player();
    return test_finish("unit/back_pill");
}
