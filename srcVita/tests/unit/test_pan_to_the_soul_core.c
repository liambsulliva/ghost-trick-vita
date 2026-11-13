#include <string.h>
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define CUT_SPEED_LOW 0x8000u
#define CUT_SPEED_HIGH 0x3eu
#define ROOM_WIDTH 1024
#define ROOM_HEIGHT 640
#define ACTOR_X 400
#define ACTOR_Y 300
#define CORE_X 20
#define CORE_Y 10
#define BAND_BOTTOM 400
#define BAND_TOP 200
#define VIEW_HEIGHT 192
#define LEFT_OF_THE_CORE 128
#define ABOVE_THE_CORE 96

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void set_up_a_soul_in_a_core(int actor_y)
{
    int index;

    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    for (index = 0; index < SCRIPT_CAMERA_BAND_MAX; index++) {
        machine.camera_band_keys[index] = 0xffff;
        machine.camera_band_values[index] = 0xffff;
    }
    machine.object_count = 1;
    machine.objects[0].name = "lamp";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.has_core = 1;
    machine.objects[0].actor.x = ACTOR_X;
    machine.objects[0].actor.y = actor_y;
    machine.objects[0].actor.core_x = CORE_X;
    machine.objects[0].actor.core_y = CORE_Y;
    script_soul_set_first_core(&machine, 0, 0);
}

static void pan(void)
{
    struct script_vm vm;
    unsigned int words[4] = { CUT_SPEED_LOW, CUT_SPEED_HIGH, 0, 0 };

    memset(&vm, 0, sizeof(vm));
    vm.selected_object = SCRIPT_NO_SELECTION;
    CHECK_INT(script_opcode_family(0xcd), SCRIPT_OP_VIEW);
    script_ops_view_run(&machine, &vm, 0xcd, words);
}

static void test_the_camera_goes_left_of_and_above_the_core(void)
{
    set_up_a_soul_in_a_core(ACTOR_Y);
    pan();
    CHECK_INT(machine.camera_x, ACTOR_X + CORE_X - LEFT_OF_THE_CORE);
    CHECK_INT(machine.camera_y, ACTOR_Y + CORE_Y - ABOVE_THE_CORE);
}

static void test_a_band_holding_the_core_sets_the_camera_bottom(void)
{
    set_up_a_soul_in_a_core(ACTOR_Y);
    machine.camera_band_keys[0] = BAND_BOTTOM;
    machine.camera_band_values[0] = BAND_TOP;
    pan();
    CHECK_INT(machine.camera_y, BAND_BOTTOM - VIEW_HEIGHT);
}

static void test_the_camera_top_stops_at_the_room_top(void)
{
    set_up_a_soul_in_a_core(CORE_Y);
    pan();
    CHECK_INT(machine.camera_y, 0);
}

static void test_no_soul_core_leaves_the_camera(void)
{
    set_up_a_soul_in_a_core(ACTOR_Y);
    machine.soul.active_object = -1;
    machine.camera_x = 7;
    pan();
    CHECK_INT(machine.camera_x, 7);
}

int main(void)
{
    test_the_camera_goes_left_of_and_above_the_core();
    test_a_band_holding_the_core_sets_the_camera_bottom();
    test_the_camera_top_stops_at_the_room_top();
    test_no_soul_core_leaves_the_camera();
    return test_finish("unit/pan_to_the_soul_core");
}
