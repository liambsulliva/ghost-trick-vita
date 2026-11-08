#include <string.h>
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static struct script_outputs outputs;
static int shown_x;
static int shown_y;
static int shows;

static void on_camera_moved(int x, int y)
{
    shown_x = x;
    shown_y = y;
    shows = shows + 1;
}

static void start_shake(int level, int ticks)
{
    memset(&machine, 0, sizeof(machine));
    memset(&outputs, 0, sizeof(outputs));
    outputs.camera_moved = on_camera_moved;
    machine.outputs = &outputs;
    machine.camera_x = 100;
    machine.camera_y = 50;
    machine.shake_level = level;
    machine.shake_ticks_left = ticks;
    shows = 0;
}

static int is_scaled_direction(int dx, int dy, int scale)
{
    int ax = dx < 0 ? -dx : dx;
    int ay = dy < 0 ? -dy : dy;

    return (ax == 0 || ax == scale) && (ay == 0 || ay == scale) && (ax != 0 || ay != 0);
}

static void test_level_one_shakes_two_pixels_and_changes_every_fourth_tick(void)
{
    int tick;
    int changes_on_a_fourth_tick;
    int changes_elsewhere;
    int last_x;
    int last_y;

    start_shake(1, 16);
    last_x = 0;
    last_y = 0;
    changes_on_a_fourth_tick = 0;
    changes_elsewhere = 0;
    for (tick = 0; tick < 15; tick++) {
        script_machine_update_camera(&machine);
        if (machine.shake_offset_x != last_x || machine.shake_offset_y != last_y) {
            if (machine.shake_ticks_left % 4 == 0) {
                changes_on_a_fourth_tick = changes_on_a_fourth_tick + 1;
            } else {
                changes_elsewhere = changes_elsewhere + 1;
            }
        }
        if (machine.shake_offset_x != 0 || machine.shake_offset_y != 0) {
            CHECK(is_scaled_direction(machine.shake_offset_x, machine.shake_offset_y, 2));
            CHECK_INT(shown_x, 100 + machine.shake_offset_x);
            CHECK_INT(shown_y, 50 + machine.shake_offset_y);
        }
        last_x = machine.shake_offset_x;
        last_y = machine.shake_offset_y;
    }
    CHECK(changes_on_a_fourth_tick >= 1);
    CHECK_INT(changes_elsewhere, 0);
    script_machine_update_camera(&machine);
    CHECK_INT(machine.shake_ticks_left, 0);
    CHECK_INT(machine.shake_offset_x, 0);
    CHECK_INT(machine.shake_offset_y, 0);
    CHECK_INT(shown_x, 100);
    CHECK_INT(shown_y, 50);
    CHECK_INT(machine.camera_x, 100);
}

static void test_level_zero_shakes_one_pixel(void)
{
    int tick;

    start_shake(0, 9);
    for (tick = 0; tick < 4; tick++) {
        script_machine_update_camera(&machine);
    }
    CHECK(is_scaled_direction(machine.shake_offset_x, machine.shake_offset_y, 1));
}

int main(void)
{
    test_level_one_shakes_two_pixels_and_changes_every_fourth_tick();
    test_level_zero_shakes_one_pixel();
    return test_finish("unit/camera_shake");
}
