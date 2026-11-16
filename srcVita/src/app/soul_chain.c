#include <math.h>
#include "soul_chain.h"

#define EASE_TICKS 30
#define TIP_MEETS_HEAD_DS_PIXELS 0.5f
#define CHAIN_GONE_DS_PIXELS 0.5f
#define TRAIL_SHARE_PER_TICK 0.15f

static const float ease_of_joint[SOUL_CHAIN_POINTS] = { 0.0f, 0.1165f, 0.1809f, 0.2344f, 0.2690f, 0.3274f, 0.4302f, 0.8401f };
static const float link_shrink[SOUL_CHAIN_POINTS] = { 0.3601f, 0.3845f, 0.4097f, 0.4358f, 0.4626f, 0.4902f, 0.5188f, 0.5479f };

static float distance_between(float from_x, float from_y, float to_x, float to_y)
{
    return sqrtf((to_x - from_x) * (to_x - from_x) + (to_y - from_y) * (to_y - from_y));
}

static void collapse_onto(struct soul_chain *chain, float x, float y)
{
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        chain->x[index] = x;
        chain->y[index] = y;
    }
    chain->head_x = x;
    chain->head_y = y;
}

static void ease_trail(struct soul_chain *chain)
{
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        chain->trail_x[index] = chain->trail_x[index] + (chain->x[index] - chain->trail_x[index]) * TRAIL_SHARE_PER_TICK;
        chain->trail_y[index] = chain->trail_y[index] + (chain->y[index] - chain->trail_y[index]) * TRAIL_SHARE_PER_TICK;
    }
}

void soul_chain_rest_at(struct soul_chain *chain, float x, float y)
{
    int index;

    collapse_onto(chain, x, y);
    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        chain->trail_x[index] = x;
        chain->trail_y[index] = y;
    }
    chain->state = SOUL_CHAIN_RESTING;
    chain->still_ticks = 0;
    chain->is_first_aiming_tick = 0;
}

void soul_chain_hold_root(struct soul_chain *chain, float x, float y)
{
    if (chain->state == SOUL_CHAIN_RESTING) {
        collapse_onto(chain, x, y);
    } else {
        chain->x[0] = x;
        chain->y[0] = y;
    }
}

void soul_chain_aim(struct soul_chain *chain, float head_x, float head_y, float reach)
{
    float length;

    if (chain->state == SOUL_CHAIN_JUMPING || chain->state == SOUL_CHAIN_RETRACTING) {
        return;
    }
    if (chain->state == SOUL_CHAIN_RESTING) {
        collapse_onto(chain, chain->x[0], chain->y[0]);
        chain->state = SOUL_CHAIN_AIMING;
        chain->is_first_aiming_tick = 1;
    }
    length = distance_between(chain->x[0], chain->y[0], head_x, head_y);
    if (length > reach) {
        head_x = chain->x[0] + (head_x - chain->x[0]) * reach / length;
        head_y = chain->y[0] + (head_y - chain->y[0]) * reach / length;
    }
    if (head_x != chain->head_x || head_y != chain->head_y) {
        chain->still_ticks = 0;
    }
    chain->head_x = head_x;
    chain->head_y = head_y;
}

void soul_chain_pick(struct soul_chain *chain, float core_x, float core_y)
{
    chain->head_x = core_x;
    chain->head_y = core_y;
    chain->still_ticks = 0;
}

void soul_chain_jump(struct soul_chain *chain, float core_x, float core_y)
{
    chain->x[SOUL_CHAIN_TIP] = core_x;
    chain->y[SOUL_CHAIN_TIP] = core_y;
    chain->state = SOUL_CHAIN_JUMPING;
}

void soul_chain_retract(struct soul_chain *chain)
{
    chain->state = SOUL_CHAIN_RETRACTING;
}

static void step_aiming(struct soul_chain *chain)
{
    int index;

    if (chain->still_ticks < EASE_TICKS) {
        for (index = 1; index < SOUL_CHAIN_POINTS; index++) {
            float share;
            float target_x;
            float target_y;
            float ease;

            share = (float)index / (float)SOUL_CHAIN_TIP;
            target_x = chain->x[0] + (chain->head_x - chain->x[0]) * share;
            target_y = chain->y[0] + (chain->head_y - chain->y[0]) * share;
            ease = chain->is_first_aiming_tick ? ease_of_joint[SOUL_CHAIN_TIP] : ease_of_joint[index];
            chain->x[index] = chain->x[index] + (target_x - chain->x[index]) * ease;
            chain->y[index] = chain->y[index] + (target_y - chain->y[index]) * ease;
        }
    }
    chain->is_first_aiming_tick = 0;
    chain->still_ticks = chain->still_ticks + 1;
    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        chain->trail_x[index] = chain->x[index];
        chain->trail_y[index] = chain->y[index];
    }
}

static void step_jumping(struct soul_chain *chain)
{
    float old_x[SOUL_CHAIN_POINTS];
    float old_y[SOUL_CHAIN_POINTS];
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        old_x[index] = chain->x[index];
        old_y[index] = chain->y[index];
    }
    for (index = SOUL_CHAIN_TIP - 1; index >= 0; index--) {
        chain->x[index] = chain->x[index + 1] + (old_x[index] - old_x[index + 1]) * link_shrink[SOUL_CHAIN_TIP - 1 - index];
        chain->y[index] = chain->y[index + 1] + (old_y[index] - old_y[index + 1]) * link_shrink[SOUL_CHAIN_TIP - 1 - index];
    }
    if (distance_between(chain->x[0], chain->y[0], chain->x[SOUL_CHAIN_TIP], chain->y[SOUL_CHAIN_TIP]) < CHAIN_GONE_DS_PIXELS) {
        collapse_onto(chain, chain->x[SOUL_CHAIN_TIP], chain->y[SOUL_CHAIN_TIP]);
        chain->state = SOUL_CHAIN_RESTING;
    }
    ease_trail(chain);
}

static void step_retracting(struct soul_chain *chain)
{
    float old_x[SOUL_CHAIN_POINTS];
    float old_y[SOUL_CHAIN_POINTS];
    int index;

    for (index = 0; index < SOUL_CHAIN_POINTS; index++) {
        old_x[index] = chain->x[index];
        old_y[index] = chain->y[index];
    }
    for (index = 1; index < SOUL_CHAIN_POINTS; index++) {
        chain->x[index] = chain->x[index - 1] + (old_x[index] - old_x[index - 1]) * link_shrink[SOUL_CHAIN_POINTS - index];
        chain->y[index] = chain->y[index - 1] + (old_y[index] - old_y[index - 1]) * link_shrink[SOUL_CHAIN_POINTS - index];
    }
    if (distance_between(chain->x[0], chain->y[0], chain->x[SOUL_CHAIN_TIP], chain->y[SOUL_CHAIN_TIP]) < CHAIN_GONE_DS_PIXELS) {
        collapse_onto(chain, chain->x[0], chain->y[0]);
        chain->state = SOUL_CHAIN_RESTING;
    }
    ease_trail(chain);
}

void soul_chain_step(struct soul_chain *chain)
{
    switch (chain->state) {
    case SOUL_CHAIN_AIMING:
        step_aiming(chain);
        break;
    case SOUL_CHAIN_JUMPING:
        step_jumping(chain);
        break;
    case SOUL_CHAIN_RETRACTING:
        step_retracting(chain);
        break;
    default:
        ease_trail(chain);
        break;
    }
}

int soul_chain_tip_is_at_head(const struct soul_chain *chain)
{
    return distance_between(chain->x[SOUL_CHAIN_TIP], chain->y[SOUL_CHAIN_TIP], chain->head_x, chain->head_y) < TIP_MEETS_HEAD_DS_PIXELS;
}

#define CLOUD_SPACING 10.0f
#define CLOUD_SIDE_SPREAD 10.0f
#define CLOUD_RADIUS_LEAST 8.0f
#define CLOUD_RADIUS_SPREAD 11.0f
#define CLOUD_STRAY_EVERY 4
#define CLOUD_STRAY_RISE_LEAST 14.0f
#define CLOUD_STRAY_RISE_SPREAD 28.0f
#define CLOUD_STRAY_RADIUS_LEAST 2.0f
#define CLOUD_STRAY_RADIUS_SPREAD 4.5f

static float cloud_fraction(unsigned int seed, unsigned int salt)
{
    unsigned int value;

    value = (seed * 31u + salt) * 2654435761u;
    value = value ^ (value >> 13);
    return (float)((value >> 8) & 1023u) / 1023.0f;
}

float soul_seeded_fraction(unsigned int seed, unsigned int salt)
{
    return cloud_fraction(seed, salt);
}

unsigned int soul_animation_step(unsigned int age_ticks)
{
    return age_ticks / SOUL_TICKS_PER_ANIMATION_STEP;
}

int soul_chain_cloud(const float *x, const float *y, float scale, unsigned int step, struct soul_chain_blob *blobs)
{
    float whole;
    float walked;
    float next_at;
    int count;
    int placed;
    int index;

    whole = 0.0f;
    for (index = 0; index < SOUL_CHAIN_TIP; index++) {
        whole = whole + distance_between(x[index], y[index], x[index + 1], y[index + 1]);
    }
    if (whole < CLOUD_SPACING * scale) {
        return 0;
    }
    count = 0;
    placed = 0;
    walked = 0.0f;
    next_at = CLOUD_SPACING * scale / 2.0f;
    for (index = 0; index < SOUL_CHAIN_TIP; index++) {
        float length;
        float along_x;
        float along_y;

        length = distance_between(x[index], y[index], x[index + 1], y[index + 1]);
        if (length <= 0.0f) {
            continue;
        }
        along_x = (x[index + 1] - x[index]) / length;
        along_y = (y[index + 1] - y[index]) / length;
        while (next_at <= walked + length && count < SOUL_CHAIN_CLOUD_BLOBS) {
            unsigned int seed;
            float side;
            float at_x;
            float at_y;

            seed = (unsigned int)placed * 97u + step * 13u;
            side = (cloud_fraction(seed, 1u) * 2.0f - 1.0f) * CLOUD_SIDE_SPREAD * scale;
            at_x = x[index] + along_x * (next_at - walked);
            at_y = y[index] + along_y * (next_at - walked);
            blobs[count].x = at_x - along_y * side;
            blobs[count].y = at_y + along_x * side;
            blobs[count].radius = (CLOUD_RADIUS_LEAST + cloud_fraction(seed, 2u) * CLOUD_RADIUS_SPREAD) * scale;
            count = count + 1;
            if (placed % CLOUD_STRAY_EVERY == CLOUD_STRAY_EVERY - 1 && count < SOUL_CHAIN_CLOUD_BLOBS) {
                blobs[count].x = at_x + (cloud_fraction(seed, 3u) * 2.0f - 1.0f) * CLOUD_SIDE_SPREAD * scale;
                blobs[count].y = at_y - (CLOUD_STRAY_RISE_LEAST + cloud_fraction(seed, 4u) * CLOUD_STRAY_RISE_SPREAD) * scale;
                blobs[count].radius = (CLOUD_STRAY_RADIUS_LEAST + cloud_fraction(seed, 5u) * CLOUD_STRAY_RADIUS_SPREAD) * scale;
                count = count + 1;
            }
            placed = placed + 1;
            next_at = next_at + CLOUD_SPACING * scale;
        }
        walked = walked + length;
    }
    return count;
}

#define SOUL_TWO_PI 6.2831853f
#define SOUL_BASE_RISE 4.0f
#define SOUL_BASE_RADIUS 28.0f
#define SOUL_BASE_LUMP_COUNT 7u
#define SOUL_BASE_LUMP_REACH 13.0f
#define SOUL_BASE_LUMP_JITTER 3.0f
#define SOUL_BASE_LUMP_RADIUS 19.0f
#define SOUL_WISP_COUNT 10u
#define SOUL_WISP_LIFE_STEPS 15u
#define SOUL_WISP_START_RISE 22.0f
#define SOUL_WISP_RISE 70.0f
#define SOUL_WISP_SWAY_TURNS 5.0f
#define SOUL_WISP_SWAY_NEAR 1.5f
#define SOUL_WISP_SWAY_FAR 13.0f
#define SOUL_WISP_RADIUS_LEAST 13.0f
#define SOUL_WISP_RADIUS_SPREAD 10.0f
#define SOUL_WISP_RADIUS_SMALLEST 1.0f

int soul_body_blobs(float x, float y, float scale, unsigned int step, struct soul_chain_blob *blobs)
{
    unsigned int index;
    int count;

    count = 0;
    for (index = 0; index < SOUL_WISP_COUNT; index++) {
        unsigned int age;
        unsigned int seed;
        float along;
        float rise;
        float sway;
        float radius;

        age = step + index * SOUL_WISP_LIFE_STEPS / SOUL_WISP_COUNT;
        seed = index * 97u + (age / SOUL_WISP_LIFE_STEPS) * 13u;
        along = (float)(age % SOUL_WISP_LIFE_STEPS) / (float)SOUL_WISP_LIFE_STEPS;
        rise = SOUL_WISP_START_RISE + SOUL_WISP_RISE * along * (0.75f + 0.5f * cloud_fraction(seed, 2u));
        sway = sinf(cloud_fraction(seed, 3u) * SOUL_TWO_PI + along * SOUL_WISP_SWAY_TURNS) * (SOUL_WISP_SWAY_NEAR + SOUL_WISP_SWAY_FAR * along);
        radius = (SOUL_WISP_RADIUS_LEAST + cloud_fraction(seed, 1u) * SOUL_WISP_RADIUS_SPREAD) * (1.0f - along);
        if (radius >= SOUL_WISP_RADIUS_SMALLEST) {
            blobs[count].x = x + sway * scale;
            blobs[count].y = y - rise * scale;
            blobs[count].radius = radius * scale;
            count++;
        }
    }
    blobs[count].x = x;
    blobs[count].y = y - SOUL_BASE_RISE * scale;
    blobs[count].radius = SOUL_BASE_RADIUS * scale;
    count++;
    for (index = 0; index < SOUL_BASE_LUMP_COUNT; index++) {
        float angle;
        float reach;

        angle = (float)index * SOUL_TWO_PI / (float)SOUL_BASE_LUMP_COUNT + cloud_fraction(step, index) * 0.5f;
        reach = SOUL_BASE_LUMP_REACH + cloud_fraction(step, index + 16u) * SOUL_BASE_LUMP_JITTER;
        blobs[count].x = x + cosf(angle) * reach * scale;
        blobs[count].y = y + (sinf(angle) * reach - SOUL_BASE_RISE) * scale;
        blobs[count].radius = SOUL_BASE_LUMP_RADIUS * scale;
        count++;
    }
    return count;
}
