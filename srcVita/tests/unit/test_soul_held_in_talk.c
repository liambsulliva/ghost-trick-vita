#include <math.h>
#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define CORE_X 100
#define CORE_Y 80
#define TICKS_HELD 20
#define MOVED_DS_PIXELS 4.0f

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void start_with_the_soul_on_one_core(int talk_is_open)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "lamp";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.x = CORE_X;
    machine.objects[0].actor.y = CORE_Y;
    script_soul_set_first_core(&machine, 0, 0);
    machine.soul.is_on = 1;
    machine.talk_is_open = talk_is_open;
    machine.pause_bit = talk_is_open;
}

static float tip_distance_after_holding(float held_x, float held_y, int direction_x)
{
    int tick;

    for (tick = 0; tick < TICKS_HELD; tick++) {
        script_soul_set_held(&machine, held_x, held_y);
        if (tick == 0 && direction_x != 0) {
            script_soul_note_direction(&machine, direction_x, 0);
        }
        script_soul_step_before_tasks(&machine);
    }
    return hypotf(machine.soul.tip_x - (float)CORE_X, machine.soul.tip_y - (float)CORE_Y);
}

static void test_the_stick_moves_the_soul_when_nobody_is_talking(void)
{
    start_with_the_soul_on_one_core(0);
    CHECK(tip_distance_after_holding(1.0f, 0.0f, 0) > MOVED_DS_PIXELS);
}

static void test_the_stick_leaves_the_soul_still_while_talk_is_open(void)
{
    start_with_the_soul_on_one_core(1);
    CHECK(tip_distance_after_holding(1.0f, 0.0f, 0) < MOVED_DS_PIXELS);
    CHECK_INT(machine.soul.active_object, 0);
}

static void test_a_direction_press_during_talk_is_dropped_not_kept_for_later(void)
{
    start_with_the_soul_on_one_core(1);
    (void)tip_distance_after_holding(0.0f, 0.0f, 1);
    CHECK_INT(machine.soul.direction_x, 0);
    machine.talk_is_open = 0;
    machine.pause_bit = 0;
    CHECK(tip_distance_after_holding(0.0f, 0.0f, 0) < MOVED_DS_PIXELS);
}

int main(void)
{
    test_the_stick_moves_the_soul_when_nobody_is_talking();
    test_the_stick_leaves_the_soul_still_while_talk_is_open();
    test_a_direction_press_during_talk_is_dropped_not_kept_for_later();
    return test_finish("unit/soul_held_in_talk");
}
