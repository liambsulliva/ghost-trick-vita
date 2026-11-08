#include <string.h>
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static struct script_outputs outputs;
static struct script_vm vm;
static int closed[SCRIPT_BALLOON_MAX];

static void on_balloon_closed(int slot)
{
    closed[slot] = closed[slot] + 1;
}

static void set_up(void)
{
    memset(&machine, 0, sizeof(machine));
    memset(&outputs, 0, sizeof(outputs));
    memset(&vm, 0, sizeof(vm));
    memset(closed, 0, sizeof(closed));
    outputs.balloon_closed = on_balloon_closed;
    machine.outputs = &outputs;
    machine.object_count = 1;
    machine.objects[0].name = "speaker";
    machine.objects[0].actor.exists = 1;
    vm.owner = &machine.objects[0];
    vm.selected_object = SCRIPT_NO_SELECTION;
}

static void open_timed_balloon(int ticks)
{
    unsigned int words[8];

    memset(words, 0, sizeof(words));
    words[0] = 1;
    words[1] = (unsigned int)ticks;
    (void)script_ops_media_run(&machine, &vm, 0x52, words);
}

static void test_timed_balloons_use_slots_two_to_seven(void)
{
    int slot;

    set_up();
    for (slot = 0; slot < SCRIPT_BALLOON_MAX - SCRIPT_TAP_BALLOON_SLOTS; slot++) {
        open_timed_balloon(100 + slot);
    }
    CHECK(!machine.balloons[0].in_use);
    CHECK(!machine.balloons[1].in_use);
    for (slot = SCRIPT_TAP_BALLOON_SLOTS; slot < SCRIPT_BALLOON_MAX; slot++) {
        CHECK(machine.balloons[slot].in_use);
        CHECK_INT(machine.balloons[slot].tap_group, SCRIPT_NO_TAP_GROUP);
    }
}

static void test_a_full_pool_replaces_the_balloon_with_least_time_left(void)
{
    int slot;

    set_up();
    for (slot = 0; slot < SCRIPT_BALLOON_MAX - SCRIPT_TAP_BALLOON_SLOTS; slot++) {
        open_timed_balloon(100 + slot);
    }
    machine.balloons[5].timer = 3;
    open_timed_balloon(77);
    CHECK_INT(closed[5], 1);
    CHECK_INT(machine.balloons[5].timer, 77);
    CHECK_INT(closed[2] + closed[3] + closed[4] + closed[6] + closed[7], 0);
}

int main(void)
{
    test_timed_balloons_use_slots_two_to_seven();
    test_a_full_pool_replaces_the_balloon_with_least_time_left();
    return test_finish("unit/balloon_slots");
}
