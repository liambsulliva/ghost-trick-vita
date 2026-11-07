#include "stick_aim.h"
#include "test_support.h"

#define RAW_CENTRE 128
#define RAW_RIGHT_EDGE 255
#define RAW_LEFT_EDGE 0
#define RAW_TOP_EDGE 0
#define RAW_INSIDE_DEAD_ZONE 150
#define HUNDREDTHS 100.0f

static int hundredths(float value)
{
    return (int)(value * HUNDREDTHS + (value < 0.0f ? -0.5f : 0.5f));
}

static void test_a_resting_stick_aims_nowhere(void)
{
    float stick_x;
    float stick_y;

    stick_aim_from_raw(RAW_CENTRE, RAW_CENTRE, &stick_x, &stick_y);
    CHECK_INT(hundredths(stick_x), 0);
    CHECK_INT(hundredths(stick_y), 0);
    stick_aim_from_raw(RAW_INSIDE_DEAD_ZONE, RAW_CENTRE, &stick_x, &stick_y);
    CHECK_INT(hundredths(stick_x), 0);
}

static void test_a_full_push_aims_at_full_strength_along_its_direction(void)
{
    float stick_x;
    float stick_y;

    stick_aim_from_raw(RAW_RIGHT_EDGE, RAW_CENTRE, &stick_x, &stick_y);
    CHECK_INT(hundredths(stick_x), 100);
    CHECK_INT(hundredths(stick_y), 0);
    stick_aim_from_raw(RAW_CENTRE, RAW_TOP_EDGE, &stick_x, &stick_y);
    CHECK_INT(hundredths(stick_x), 0);
    CHECK_INT(hundredths(stick_y), -100);
}

static void test_strength_grows_from_the_edge_of_the_dead_zone(void)
{
    float stick_x;
    float stick_y;
    int half_way_out;

    half_way_out = RAW_CENTRE + (int)(127.0f * 0.625f);
    stick_aim_from_raw(half_way_out, RAW_CENTRE, &stick_x, &stick_y);
    CHECK_INT(hundredths(stick_x), 50);
}

static void test_a_full_diagonal_moves_like_the_d_pad_diagonal(void)
{
    CHECK_INT(hundredths(stick_aim_axis_speed(0.71f)), 95);
    CHECK_INT(hundredths(stick_aim_axis_speed(0.9f)), 100);
    CHECK_INT(hundredths(stick_aim_axis_speed(-0.9f)), -100);
    CHECK_INT(hundredths(stick_aim_axis_speed(0.375f)), 50);
}

static void test_the_first_push_presses_its_strongest_axis(void)
{
    int pressed_x;
    int pressed_y;
    float stick_x;
    float stick_y;

    stick_aim_from_raw(RAW_LEFT_EDGE, RAW_CENTRE + 20, &stick_x, &stick_y);
    stick_aim_first_push(stick_x, stick_y, &pressed_x, &pressed_y);
    CHECK_INT(pressed_x, -1);
    CHECK_INT(pressed_y, 0);
    stick_aim_first_push(0.2f, 0.8f, &pressed_x, &pressed_y);
    CHECK_INT(pressed_x, 0);
    CHECK_INT(pressed_y, 1);
}

int main(void)
{
    test_a_resting_stick_aims_nowhere();
    test_a_full_push_aims_at_full_strength_along_its_direction();
    test_strength_grows_from_the_edge_of_the_dead_zone();
    test_a_full_diagonal_moves_like_the_d_pad_diagonal();
    test_the_first_push_presses_its_strongest_axis();
    return test_finish("unit/stick_aim");
}
