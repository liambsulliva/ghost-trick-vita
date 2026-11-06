#include "clock.h"
#include "timing.h"

void frame_timer_start(struct frame_timer *timer)
{
    timer->last_us = clock_now_us();
}

unsigned long long frame_timer_tick(struct frame_timer *timer)
{
    unsigned long long now;
    unsigned long long elapsed_us;

    now = clock_now_us();
    elapsed_us = now - timer->last_us;
    timer->last_us = now;
    return elapsed_us;
}
