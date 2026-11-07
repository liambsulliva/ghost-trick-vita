#include "soul_chain.h"
#include "test_support.h"

#define REACH 48.0f
#define D_PAD_STEP 6.0f
#define TENTHS 10.0f

static struct soul_chain chain;

static int tenths(float value)
{
    return (int)(value * TENTHS + (value < 0.0f ? -0.5f : 0.5f));
}

static int ticks_until_resting(void)
{
    int ticks;

    ticks = 0;
    while (chain.state != SOUL_CHAIN_RESTING && ticks < 100) {
        soul_chain_step(&chain);
        ticks = ticks + 1;
    }
    return ticks;
}

static void test_a_held_d_pad_moves_the_tip_at_a_steady_rate_up_to_the_reach(void)
{
    int tick;

    soul_chain_rest_at(&chain, 0.0f, 0.0f);
    for (tick = 1; tick <= 9; tick++) {
        soul_chain_aim(&chain, chain.x[SOUL_CHAIN_TIP] + D_PAD_STEP, 0.0f, REACH);
        soul_chain_step(&chain);
        if (tick == 1 || tick == 5) {
            CHECK_INT(tenths(chain.x[SOUL_CHAIN_TIP]), tenths(5.04f * (float)tick));
        }
    }
    for (tick = 0; tick < 20; tick++) {
        soul_chain_aim(&chain, chain.x[SOUL_CHAIN_TIP] + D_PAD_STEP, 0.0f, REACH);
        soul_chain_step(&chain);
    }
    CHECK_INT(tenths(chain.head_x), 480);
    CHECK_INT(chain.x[SOUL_CHAIN_TIP] <= REACH, 1);
}

static void test_a_touch_pulls_the_tip_most_of_the_way_each_tick(void)
{
    soul_chain_rest_at(&chain, 0.0f, 0.0f);
    soul_chain_aim(&chain, 40.0f, 0.0f, REACH);
    soul_chain_step(&chain);
    CHECK_INT(tenths(chain.x[SOUL_CHAIN_TIP]), 336);
    soul_chain_aim(&chain, 40.0f, 0.0f, REACH);
    soul_chain_step(&chain);
    CHECK_INT(tenths(chain.x[SOUL_CHAIN_TIP]), 390);
}

static void test_turning_bows_the_line_before_it_straightens(void)
{
    int tick;
    float bow;

    soul_chain_rest_at(&chain, 0.0f, 0.0f);
    for (tick = 0; tick < 40; tick++) {
        soul_chain_aim(&chain, 40.0f, 0.0f, REACH);
        soul_chain_step(&chain);
    }
    soul_chain_aim(&chain, 0.0f, 40.0f, REACH);
    soul_chain_step(&chain);
    soul_chain_step(&chain);
    bow = chain.x[3] - chain.x[SOUL_CHAIN_TIP] * 3.0f / 7.0f;
    CHECK_INT(bow > 1.0f, 1);
    for (tick = 0; tick < 29; tick++) {
        soul_chain_step(&chain);
    }
    CHECK_INT(tenths(chain.x[3] - chain.x[SOUL_CHAIN_TIP] * 3.0f / 7.0f) < 10, 1);
}

static void straight_chain_to(float length)
{
    int index;

    soul_chain_rest_at(&chain, 0.0f, 0.0f);
    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        chain.x[index] = length * (float)index / (float)SOUL_CHAIN_TIP;
        chain.trail_x[index] = chain.x[index];
    }
    chain.state = SOUL_CHAIN_AIMING;
}

static void test_a_jump_pulls_the_root_onto_the_new_core_within_a_few_ticks(void)
{
    straight_chain_to(46.0f);
    soul_chain_jump(&chain, 46.0f, 0.0f);
    soul_chain_step(&chain);
    CHECK_INT(chain.x[0] > 20.0f && chain.x[0] < 30.0f, 1);
    CHECK_INT(ticks_until_resting() + 1, 6);
    CHECK_INT(tenths(chain.x[0]), 460);
    straight_chain_to(16.0f);
    soul_chain_jump(&chain, 16.0f, 0.0f);
    CHECK_INT(ticks_until_resting(), 5);
    straight_chain_to(64.0f);
    soul_chain_jump(&chain, 64.0f, 0.0f);
    CHECK_INT(ticks_until_resting(), 7);
}

static void test_the_trail_follows_a_jump_with_a_soft_finish(void)
{
    int tick;

    straight_chain_to(46.0f);
    soul_chain_jump(&chain, 46.0f, 0.0f);
    for (tick = 0; tick < 12; tick++) {
        soul_chain_step(&chain);
    }
    CHECK_INT(chain.trail_x[0] > 46.0f * 0.7f && chain.trail_x[0] < 46.0f * 0.9f, 1);
    for (tick = 12; tick < 20; tick++) {
        soul_chain_step(&chain);
    }
    CHECK_INT(chain.trail_x[0] >= 46.0f * 0.95f, 1);
    CHECK_INT(chain.trail_x[0] < 46.0f, 1);
}

static void test_a_release_with_no_core_shrinks_back_to_the_root(void)
{
    straight_chain_to(48.0f);
    soul_chain_retract(&chain);
    CHECK_INT(ticks_until_resting(), 7);
    CHECK_INT(tenths(chain.x[SOUL_CHAIN_TIP]), 0);
}

static void test_the_tip_meets_the_head_when_a_core_is_picked(void)
{
    int tick;

    soul_chain_rest_at(&chain, 0.0f, 0.0f);
    soul_chain_aim(&chain, 30.0f, 0.0f, REACH);
    soul_chain_step(&chain);
    soul_chain_pick(&chain, 40.0f, 4.0f);
    for (tick = 0; tick < 4 && !soul_chain_tip_is_at_head(&chain); tick++) {
        soul_chain_step(&chain);
    }
    CHECK_INT(soul_chain_tip_is_at_head(&chain), 1);
}

int main(void)
{
    test_a_held_d_pad_moves_the_tip_at_a_steady_rate_up_to_the_reach();
    test_a_touch_pulls_the_tip_most_of_the_way_each_tick();
    test_turning_bows_the_line_before_it_straightens();
    test_a_jump_pulls_the_root_onto_the_new_core_within_a_few_ticks();
    test_the_trail_follows_a_jump_with_a_soft_finish();
    test_a_release_with_no_core_shrinks_back_to_the_root();
    test_the_tip_meets_the_head_when_a_core_is_picked();
    return test_finish("unit/soul_chain");
}
