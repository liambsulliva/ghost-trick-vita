#include "fakes/fake_platform.h"
#include "test_support.h"
#include "timing.h"

static void test_frame_timer_measures_elapsed_time(void)
{
    struct frame_timer timer;

    fake_clock_set_us(1000);
    frame_timer_start(&timer);
    fake_clock_advance_us(16667);
    CHECK_INT(frame_timer_tick(&timer), 16667);
    fake_clock_advance_us(1);
    CHECK_INT(frame_timer_tick(&timer), 1);
}

int main(void)
{
    test_frame_timer_measures_elapsed_time();
    return test_finish("unit/timing");
}
