#include <string.h>
#include <math.h>
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

static float distance_from_line(float x, float y, float to_x, float to_y)
{
    float length;

    length = sqrtf(to_x * to_x + to_y * to_y);
    return fabsf(x * to_y - y * to_x) / length;
}

static void test_the_cloud_covers_the_whole_line_with_blobs_of_many_sizes(void)
{
    struct soul_chain_blob blobs[SOUL_CHAIN_CLOUD_BLOBS];
    struct soul_chain_blob later[SOUL_CHAIN_CLOUD_BLOBS];
    float line_x[SOUL_CHAIN_POINTS];
    float line_y[SOUL_CHAIN_POINTS];
    float smallest;
    float largest;
    float furthest_along;
    int strays;
    int moved;
    int count;
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        line_x[index] = 30.0f * (float)index;
        line_y[index] = 40.0f * (float)index;
    }
    count = soul_chain_cloud(line_x, line_y, 1.0f, 7u, blobs);
    CHECK(count >= 30 && count <= SOUL_CHAIN_CLOUD_BLOBS);
    smallest = 1000.0f;
    largest = 0.0f;
    furthest_along = 0.0f;
    strays = 0;
    for (index = 0; index < count; index++) {
        float along;

        along = (blobs[index].x * 30.0f + blobs[index].y * 40.0f) / 50.0f;
        if (blobs[index].radius < 7.0f) {
            strays = strays + 1;
            CHECK(blobs[index].y < blobs[index].x * 40.0f / 30.0f);
            continue;
        }
        CHECK(distance_from_line(blobs[index].x, blobs[index].y, 30.0f, 40.0f) <= blobs[index].radius + 3.0f);
        smallest = blobs[index].radius < smallest ? blobs[index].radius : smallest;
        largest = blobs[index].radius > largest ? blobs[index].radius : largest;
        furthest_along = along > furthest_along ? along : furthest_along;
    }
    CHECK(largest >= smallest * 1.5f);
    CHECK(largest <= 20.0f);
    CHECK(furthest_along >= 350.0f - 12.0f);
    CHECK(strays >= 2);
    CHECK(soul_chain_cloud(line_x, line_y, 1.0f, 8u, later) == count);
    moved = 0;
    for (index = 0; index < count; index++) {
        moved = moved + (later[index].radius != blobs[index].radius);
    }
    CHECK(moved > count / 2);
    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        line_x[index] = 0.5f * (float)index;
        line_y[index] = 0.0f;
    }
    CHECK(soul_chain_cloud(line_x, line_y, 1.0f, 7u, blobs) == 0);
}

static void test_the_cloud_keeps_pace_with_the_soul_body(void)
{
    struct soul_chain_blob first[SOUL_CHAIN_CLOUD_BLOBS];
    struct soul_chain_blob held[SOUL_CHAIN_CLOUD_BLOBS];
    float line_x[SOUL_CHAIN_POINTS];
    float line_y[SOUL_CHAIN_POINTS];
    unsigned int age;
    int count;
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        line_x[index] = 30.0f * (float)index;
        line_y[index] = 0.0f;
    }
    count = soul_chain_cloud(line_x, line_y, 1.0f, soul_animation_step(0u), first);
    for (age = 1u; age < SOUL_TICKS_PER_ANIMATION_STEP; age++) {
        CHECK_INT(soul_animation_step(age), 0);
        CHECK(soul_chain_cloud(line_x, line_y, 1.0f, soul_animation_step(age), held) == count);
        CHECK(memcmp(first, held, sizeof(first[0]) * (unsigned int)count) == 0);
    }
    CHECK_INT(soul_animation_step(SOUL_TICKS_PER_ANIMATION_STEP), 1);
    CHECK(soul_chain_cloud(line_x, line_y, 1.0f, soul_animation_step(SOUL_TICKS_PER_ANIMATION_STEP), held) == count);
    CHECK(memcmp(first, held, sizeof(first[0]) * (unsigned int)count) != 0);
}

int main(void)
{
    test_the_cloud_keeps_pace_with_the_soul_body();
    test_the_cloud_covers_the_whole_line_with_blobs_of_many_sizes();
    test_a_held_d_pad_moves_the_tip_at_a_steady_rate_up_to_the_reach();
    test_a_touch_pulls_the_tip_most_of_the_way_each_tick();
    test_turning_bows_the_line_before_it_straightens();
    test_a_jump_pulls_the_root_onto_the_new_core_within_a_few_ticks();
    test_the_trail_follows_a_jump_with_a_soft_finish();
    test_a_release_with_no_core_shrinks_back_to_the_root();
    test_the_tip_meets_the_head_when_a_core_is_picked();
    return test_finish("unit/soul_chain");
}
