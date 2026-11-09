#include <math.h>
#include "stick_aim.h"

#define STICK_CENTRE 128.0f
#define STICK_HALF_RANGE 127.0f
#define STICK_DEAD_ZONE 0.25f

void stick_aim_from_raw(int raw_x, int raw_y, float *stick_x, float *stick_y)
{
    float x;
    float y;
    float length;
    float share_past_dead_zone;

    *stick_x = 0.0f;
    *stick_y = 0.0f;
    x = ((float)raw_x - STICK_CENTRE) / STICK_HALF_RANGE;
    y = ((float)raw_y - STICK_CENTRE) / STICK_HALF_RANGE;
    length = sqrtf(x * x + y * y);
    if (length <= STICK_DEAD_ZONE) {
        return;
    }
    share_past_dead_zone = (length - STICK_DEAD_ZONE) / (1.0f - STICK_DEAD_ZONE);
    if (share_past_dead_zone > 1.0f) {
        share_past_dead_zone = 1.0f;
    }
    *stick_x = x / length * share_past_dead_zone;
    *stick_y = y / length * share_past_dead_zone;
}

float stick_aim_axis_speed(float axis)
{
    float speed;

    speed = axis / STICK_AXIS_FOR_FULL_SPEED;
    if (speed > 1.0f) {
        return 1.0f;
    }
    if (speed < -1.0f) {
        return -1.0f;
    }
    return speed;
}

static int sign_of(float value)
{
    if (value > 0.0f) {
        return 1;
    }
    return value < 0.0f ? -1 : 0;
}

void stick_aim_first_push(float stick_x, float stick_y, int *pressed_x, int *pressed_y)
{
    *pressed_x = 0;
    *pressed_y = 0;
    if (fabsf(stick_x) >= fabsf(stick_y)) {
        *pressed_x = sign_of(stick_x);
        return;
    }
    *pressed_y = sign_of(stick_y);
}
