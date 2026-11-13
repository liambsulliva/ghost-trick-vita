#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define TICKS_IN_THE_VIEW 90
#define ROOM_WIDTH 1024
#define ROOM_HEIGHT 192
#define CORE_FAR_RIGHT_X 800
#define CORE_Y 96

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void start_a_room_with_a_far_core(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    machine.object_count = 1;
    machine.objects[0].name = "body";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.has_core = 1;
    machine.objects[0].actor.x = CORE_FAR_RIGHT_X;
    machine.objects[0].actor.y = CORE_Y;
    script_soul_set_first_core(&machine, 0, 0);
}

static void test_the_player_view_follows_the_core(void)
{
    start_a_room_with_a_far_core();
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    run_ticks(TICKS_IN_THE_VIEW);
    CHECK(machine.camera_x > 0);
}

static void test_the_tutorial_view_leaves_the_camera_where_the_script_put_it(void)
{
    start_a_room_with_a_far_core();
    script_soul_run_command(&machine, "SoulVision", "On");
    run_ticks(TICKS_IN_THE_VIEW);
    CHECK_INT(machine.camera_x, 0);
    CHECK_INT(machine.soul.is_on, 1);
}

static void test_a_busy_core_is_not_followed(void)
{
    start_a_room_with_a_far_core();
    machine.objects[0].actor.busy_cores = 1;
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    run_ticks(TICKS_IN_THE_VIEW);
    CHECK_INT(machine.camera_x, 0);
}

static void test_a_player_view_after_a_tutorial_follows_again(void)
{
    start_a_room_with_a_far_core();
    script_soul_run_command(&machine, "SoulVision", "On");
    run_ticks(TICKS_IN_THE_VIEW);
    script_soul_run_command(&machine, "SVTUTO", "ReturnGame");
    run_ticks(TICKS_IN_THE_VIEW);
    script_soul_run_command(&machine, "CustomSoulVision", "NoEffectStart");
    run_ticks(TICKS_IN_THE_VIEW);
    CHECK(machine.camera_x > 0);
}

int main(void)
{
    test_the_player_view_follows_the_core();
    test_the_tutorial_view_leaves_the_camera_where_the_script_put_it();
    test_a_busy_core_is_not_followed();
    test_a_player_view_after_a_tutorial_follows_again();
    return test_finish("unit/ghost_view_camera_follow");
}
