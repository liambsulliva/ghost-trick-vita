#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define STEPS_TO_SETTLE 120

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void settle(void)
{
    int step;

    for (step = 0; step < STEPS_TO_SETTLE; step++) {
        script_pills_step(&machine);
    }
}

static void test_the_trick_pill_slides_out_while_held_and_returns(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = 3;
    machine.pause_bit = 0;
    script_pills_register(&machine, SCRIPT_PILL_TRICK, 1);
    settle();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    machine.soul.trick_pill_is_held_out = 1;
    settle();
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
    machine.soul.trick_pill_is_held_out = 0;
    settle();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
}

static void test_a_pill_hidden_while_pressed_does_not_hold_the_others_out(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.mode_byte = 3;
    machine.pause_bit = 0;
    script_pills_register(&machine, SCRIPT_PILL_TRICK, 1);
    settle();
    script_pills_press(&machine, SCRIPT_PILL_TRICK, 1);
    script_pills_unregister_all(&machine);
    script_pills_register_all(&machine);
    settle();
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_TRICK));
}

int main(void)
{
    test_a_pill_hidden_while_pressed_does_not_hold_the_others_out();
    test_the_trick_pill_slides_out_while_held_and_returns();
    return test_finish("unit/trick_pill_hold");
}
