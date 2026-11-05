#include "timing.h"

void frame_timer_start(struct frame_timer *timer)
{
    timer->last_us = timing_now_us();
    timer->delta_us = 0;
}

unsigned long long frame_timer_tick(struct frame_timer *timer)
{
    unsigned long long now;

    now = timing_now_us();
    timer->delta_us = now - timer->last_us;
    timer->last_us = now;
    return timer->delta_us;
}

void fixed_step_init(struct fixed_step *step, unsigned long long step_us)
{
    step->step_us = step_us;
    step->accumulator_us = 0;
}

int fixed_step_feed(struct fixed_step *step, unsigned long long elapsed_us)
{
    int count;

    count = 0;
    step->accumulator_us = step->accumulator_us + elapsed_us;
    while (step->accumulator_us >= step->step_us) {
        if (count == FIXED_STEP_MAX_PER_FEED) {
            step->accumulator_us = 0;
            break;
        }
        step->accumulator_us = step->accumulator_us - step->step_us;
        count = count + 1;
    }
    return count;
}
