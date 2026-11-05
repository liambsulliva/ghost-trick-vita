#ifndef TIMING_H
#define TIMING_H

#define FIXED_STEP_MAX_PER_FEED 5

struct frame_timer {
    unsigned long long last_us;
    unsigned long long delta_us;
};

struct fixed_step {
    unsigned long long step_us;
    unsigned long long accumulator_us;
};

unsigned long long timing_now_us(void);
void frame_timer_start(struct frame_timer *timer);
unsigned long long frame_timer_tick(struct frame_timer *timer);
void fixed_step_init(struct fixed_step *step, unsigned long long step_us);
int fixed_step_feed(struct fixed_step *step, unsigned long long elapsed_us);

#endif
