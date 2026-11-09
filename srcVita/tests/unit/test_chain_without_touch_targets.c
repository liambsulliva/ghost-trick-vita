#include <stdio.h>
#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define PEN_PAST_THE_GATE_X 170.0f
#define TICKS_FOR_A_DRAG 60

static struct script_machine machine;
static int marks_seen;

static void on_marked(const char *what, const char *name)
{
    (void)name;
    if (strcmp(what, "chain_moved_without_touch_targets") == 0) {
        marks_seen = marks_seen + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = ROW_Y;
}

static void drag_to_the_gate_with_the_buttons(const char *button_command)
{
    int tick;

    marks_seen = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    place_core(0, "body", BODY_X);
    place_core(1, "gate", GATE_X);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", button_command);
    machine.soul.is_on = 1;
    for (tick = 0; tick < TICKS_FOR_A_DRAG; tick++) {
        script_soul_step_before_tasks(&machine);
    }
    script_soul_set_touch(&machine, 1, (float)BODY_X, (float)ROW_Y);
    script_soul_step_before_tasks(&machine);
    script_soul_set_touch(&machine, 1, PEN_PAST_THE_GATE_X, (float)ROW_Y);
    for (tick = 0; tick < TICKS_FOR_A_DRAG; tick++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void test_a_move_with_no_touch_target_on_screen_is_marked(void)
{
    drag_to_the_gate_with_the_buttons("All:Off");
    CHECK_INT(machine.soul.active_object, 1);
    CHECK_INT(marks_seen, 1);
}

static void test_a_move_with_the_back_button_up_is_not_marked(void)
{
    drag_to_the_gate_with_the_buttons("Trick:On");
    CHECK_INT(machine.soul.active_object, 1);
    CHECK_INT(marks_seen, 0);
}

int main(void)
{
    test_a_move_with_no_touch_target_on_screen_is_marked();
    test_a_move_with_the_back_button_up_is_not_marked();
    return test_finish("unit/chain_without_touch_targets");
}
