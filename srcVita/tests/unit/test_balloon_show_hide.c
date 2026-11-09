#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define TICKS_LEFT_ON_THE_TIMER 5
#define THIRD_SLOT 2

static struct script_machine machine;
static struct script_outputs outputs;
static int closed_slots[SCRIPT_BALLOON_MAX];

static void on_balloon_closed(int slot)
{
    closed_slots[slot] = closed_slots[slot] + 1;
}

static void open_balloons_in_three_slots(void)
{
    int slot;

    memset(&machine, 0, sizeof(machine));
    memset(&outputs, 0, sizeof(outputs));
    memset(closed_slots, 0, sizeof(closed_slots));
    outputs.balloon_closed = on_balloon_closed;
    machine.outputs = &outputs;
    for (slot = 0; slot <= THIRD_SLOT; slot++) {
        machine.balloons[slot].in_use = 1;
        machine.balloons[slot].tap_group = slot;
        machine.balloons[slot].timer = TICKS_LEFT_ON_THE_TIMER;
    }
}

static void run_op(unsigned int op, unsigned int first_word)
{
    unsigned int words[8];

    memset(words, 0, sizeof(words));
    words[0] = first_word;
    (void)script_ops_media_run(&machine, NULL, op, words);
}

static void test_hidden_balloons_stay_open_but_cannot_be_tapped_or_run_out(void)
{
    int tick;

    open_balloons_in_three_slots();
    run_op(0x59, 0);
    CHECK(machine.balloons[0].in_use);
    CHECK(machine.balloons[THIRD_SLOT].in_use);
    CHECK(machine.balloons[0].is_hidden);
    CHECK_INT(closed_slots[0], 1);
    CHECK_INT(closed_slots[THIRD_SLOT], 1);
    for (tick = 0; tick < TICKS_LEFT_ON_THE_TIMER * 2; tick++) {
        script_balloons_count_down(&machine);
    }
    CHECK(machine.balloons[0].in_use);
    CHECK_INT(machine.balloons[0].timer, TICKS_LEFT_ON_THE_TIMER);
    script_balloons_tap(&machine, 0);
    CHECK(machine.balloons[0].in_use);
}

static void test_showing_reopens_hidden_balloons_and_their_timers_run(void)
{
    open_balloons_in_three_slots();
    run_op(0x59, 0);
    run_op(0x58, 0);
    CHECK(!machine.balloons[0].is_hidden);
    CHECK(machine.balloons[0].open_is_pending);
    CHECK(machine.balloons[THIRD_SLOT].open_is_pending);
    script_balloons_count_down(&machine);
    CHECK_INT(machine.balloons[0].timer, TICKS_LEFT_ON_THE_TIMER - 1);
    script_balloons_tap(&machine, 0);
    CHECK(!machine.balloons[0].in_use);
}

static void test_hiding_twice_closes_the_drawing_once(void)
{
    open_balloons_in_three_slots();
    run_op(0x59, 0);
    run_op(0x59, 0);
    CHECK_INT(closed_slots[0], 1);
}

static void test_keeping_balloons_hidden_hides_the_two_tap_slots_and_refuses_showing(void)
{
    open_balloons_in_three_slots();
    run_op(0x4a, 1);
    CHECK(machine.balloons[0].is_hidden);
    CHECK(machine.balloons[1].is_hidden);
    CHECK(!machine.balloons[THIRD_SLOT].is_hidden);
    run_op(0x58, 0);
    CHECK(machine.balloons[0].is_hidden);
    run_op(0x4a, 0);
    CHECK(machine.balloons[0].is_hidden);
    run_op(0x58, 0);
    CHECK(!machine.balloons[0].is_hidden);
    CHECK(!machine.balloons[1].is_hidden);
}

static void test_closing_the_right_balloon_lifts_the_hold(void)
{
    open_balloons_in_three_slots();
    run_op(0x4a, 1);
    run_op(0x4d, 0);
    CHECK_INT(machine.balloons_stay_hidden, 0);
    run_op(0x58, 0);
    CHECK(!machine.balloons[0].is_hidden);
}

int main(void)
{
    test_hidden_balloons_stay_open_but_cannot_be_tapped_or_run_out();
    test_showing_reopens_hidden_balloons_and_their_timers_run();
    test_hiding_twice_closes_the_drawing_once();
    test_keeping_balloons_hidden_hides_the_two_tap_slots_and_refuses_showing();
    test_closing_the_right_balloon_lifts_the_hold();
    return test_finish("unit/balloon_show_hide");
}
