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
