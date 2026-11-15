#include <stdio.h>
#include <string.h>
#include "ds_units.h"
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_FRAME_CORE_AND_ACTOR 0xca
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define ROOM_WIDTH 1024
#define ROOM_HEIGHT 768
#define CORE_X 300
#define CORE_Y 500
#define ACTOR_X 380
#define ACTOR_Y 420
#define CAMERA_X_AT_START 100
#define CAMERA_Y_AT_START 150
#define SPEED_16_PIXELS 0x10000
#define TICKS_FOR_THE_PAN 40
#define NATIVE_ROWS_ABOVE_THE_MIDDLE 20

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static const char strings[] = "\0\0";
static const struct script_outputs outputs = { 0 };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void frame_at_speed(int speed)
{
    unsigned int words[6];
    unsigned int index;
    unsigned int code_size;
    int tick;

    words[0] = OP_FRAME_CORE_AND_ACTOR;
    words[1] = (unsigned int)speed & 0xffffu;
    words[2] = ((unsigned int)speed >> 16) & 0xffffu;
    words[3] = OP_WAIT;
    words[4] = A_LONG_WAIT;
    words[5] = OP_END;
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = sizeof(words) / sizeof(words[0]) * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    machine.camera_x = CAMERA_X_AT_START;
    machine.camera_y = CAMERA_Y_AT_START;
    machine.objects[0].is_deleted = 1;
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.x = CORE_X;
    machine.objects[0].actor.y = CORE_Y;
    machine.objects[1].is_deleted = 1;
    machine.objects[1].actor.exists = 1;
    machine.objects[1].actor.x = ACTOR_X;
    machine.objects[1].actor.y = ACTOR_Y;
    machine.soul.active_object = 0;
    machine.soul.active_slot = 0;
    machine.camera_follow_object = 1;
    for (tick = 0; tick < TICKS_FOR_THE_PAN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_plain_speed_frames_across_and_keeps_the_height(void)
{
    frame_at_speed(SPEED_16_PIXELS);
    CHECK_INT(machine.camera_x, (CORE_X + ACTOR_X) / 2 - DS_VIEW_WIDTH / 2);
    CHECK_INT(machine.camera_y, CAMERA_Y_AT_START);
}

static void test_a_negative_speed_also_frames_the_height(void)
{
    frame_at_speed(-SPEED_16_PIXELS);
    CHECK_INT(machine.camera_x, (CORE_X + ACTOR_X) / 2 - DS_VIEW_WIDTH / 2);
    CHECK_INT(machine.camera_y, (CORE_Y + ACTOR_Y) / 2 - DS_VIEW_HEIGHT / 2 - NATIVE_ROWS_ABOVE_THE_MIDDLE);
}

int main(void)
{
    test_a_plain_speed_frames_across_and_keeps_the_height();
    test_a_negative_speed_also_frames_the_height();
    return test_finish("unit/framing_both_ways");
}
