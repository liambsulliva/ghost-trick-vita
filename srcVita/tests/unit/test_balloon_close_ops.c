#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define ACTOR_GROUP 0

static struct script_machine machine;
static struct script_outputs outputs;
static int closed_slots[SCRIPT_BALLOON_MAX];

static void on_balloon_closed(int slot)
{
    closed_slots[slot] = closed_slots[slot] + 1;
}

static void open_three_balloons(void)
{
    memset(&machine, 0, sizeof(machine));
    memset(&outputs, 0, sizeof(outputs));
    memset(closed_slots, 0, sizeof(closed_slots));
    outputs.balloon_closed = on_balloon_closed;
    machine.outputs = &outputs;
    machine.balloons[0].in_use = 1;
    machine.balloons[0].tap_group = ACTOR_GROUP;
    machine.balloons[1].in_use = 1;
    machine.balloons[1].tap_group = SCRIPT_TAP_GROUP_LEFT;
    machine.balloons[2].in_use = 1;
    machine.balloons[2].tap_group = SCRIPT_TAP_GROUP_RIGHT;
}

static void run_op(unsigned int op)
{
    unsigned int words[8];

    memset(words, 0, sizeof(words));
    (void)script_ops_media_run(&machine, NULL, op, words);
}

static void test_close_slot_one_leaves_actor_and_left_balloons_open(void)
{
    open_three_balloons();
    run_op(0x4d);
    CHECK(machine.balloons[0].in_use);
    CHECK(machine.balloons[1].in_use);
    CHECK(!machine.balloons[2].in_use);
    CHECK_INT(closed_slots[0], 0);
    CHECK_INT(closed_slots[2], 1);
}

static void test_close_all_closes_every_balloon(void)
{
    open_three_balloons();
    run_op(0x57);
    CHECK(!machine.balloons[0].in_use);
    CHECK(!machine.balloons[1].in_use);
    CHECK(!machine.balloons[2].in_use);
    CHECK_INT(closed_slots[0] + closed_slots[1] + closed_slots[2], 3);
}

static void test_talk_balloon_with_label_zero_closes_slot_zero_only(void)
{
    open_three_balloons();
    run_op(0x45);
    CHECK(machine.balloons[0].in_use);
    CHECK(!machine.balloons[1].in_use);
    CHECK(machine.balloons[2].in_use);
}

int main(void)
{
    test_close_slot_one_leaves_actor_and_left_balloons_open();
    test_close_all_closes_every_balloon();
    test_talk_balloon_with_label_zero_closes_slot_zero_only();
    return test_finish("unit/balloon_close_ops");
}
