#include "rain_field.h"
#include "test_support.h"

#define WIDE_SPAN 342
#define WIDE_FIRST_X (-171)
#define RIGHT_SPAN 171
#define HUNDRED 100.0f
#define TICKS_TO_RUN 300
#define LOWEST_LIVE_ROW 192.0f

static struct rain_field field;

static void test_the_span_sets_how_many_streaks_fall(void)
{
    rain_field_seed(&field);
    rain_field_start(&field, WIDE_SPAN, WIDE_FIRST_X, 0.0f, 0.0f);
    CHECK_INT(field.count, 136);
    rain_field_start(&field, RIGHT_SPAN, 0, 0.0f, 0.0f);
    CHECK_INT(field.count, 68);
}

static void test_streaks_stay_in_their_span_and_above_the_floor_of_the_screen(void)
{
    int tick;
    int index;
    int outside;

    outside = 0;
    rain_field_seed(&field);
    rain_field_start(&field, RIGHT_SPAN, 0, 100.0f, 50.0f);
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        rain_field_advance(&field, 100.0f, 50.0f);
        for (index = 0; index < field.count; index++) {
            const struct rain_streak *streak;

            streak = &field.streaks[index];
            if (streak->x < 100.0f || streak->x >= 100.0f + (float)RIGHT_SPAN) {
                outside = outside + 1;
            }
            if (streak->is_falling && streak->y > 50.0f + LOWEST_LIVE_ROW) {
                outside = outside + 1;
            }
            if (streak->speed < 16.0f || streak->speed > 31.0f || streak->half_length > 17.625f) {
                outside = outside + 1;
            }
        }
    }
    CHECK_INT(outside, 0);
}

static void test_a_streak_past_the_last_row_respawns_on_the_next_tick(void)
{
    rain_field_seed(&field);
    rain_field_start(&field, RIGHT_SPAN, 0, 0.0f, 0.0f);
    field.count = 1;
    field.streaks[0].y = 150.0f;
    field.streaks[0].speed = 16.0f;
    rain_field_advance(&field, 0.0f, 0.0f);
    CHECK_INT(field.streaks[0].is_falling, 0);
    rain_field_advance(&field, 0.0f, 0.0f);
    CHECK_INT(field.streaks[0].is_falling, 1);
}

static void test_alpha_cycles_through_sixteen_steps(void)
{
    CHECK_INT((int)(rain_field_alpha_of(0) * 31.0f + 0.5f), 1);
    CHECK_INT((int)(rain_field_alpha_of(15) * 31.0f + 0.5f), 16);
    CHECK_INT((int)(rain_field_alpha_of(16) * HUNDRED + 0.5f), (int)(rain_field_alpha_of(0) * HUNDRED + 0.5f));
}

int main(void)
{
    test_the_span_sets_how_many_streaks_fall();
    test_streaks_stay_in_their_span_and_above_the_floor_of_the_screen();
    test_a_streak_past_the_last_row_respawns_on_the_next_tick();
    test_alpha_cycles_through_sixteen_steps();
    return test_finish("unit/rain_field");
}
