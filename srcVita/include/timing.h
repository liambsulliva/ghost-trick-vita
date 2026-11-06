#ifndef TIMING_H
#define TIMING_H

struct frame_timer {
    unsigned long long last_us;
};

void frame_timer_start(struct frame_timer *timer);
unsigned long long frame_timer_tick(struct frame_timer *timer);

#endif
