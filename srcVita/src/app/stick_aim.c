#include <math.h>
#include "stick_aim.h"

#define STICK_CENTRE ((float)STICK_AIM_RAW_CENTRE)
#define STICK_HALF_RANGE 127.0f
#define STICK_DEAD_ZONE 0.25f
#define STICK_STEADY_RAW_UNITS 3
#define STICK_REST_FRAMES 60
#define STICK_REST_REACH 0.45f

static void aim_from_centre(float raw_x, float raw_y, float centre_x, float centre_y, float *stick_x, float *stick_y)
{
    float x;
    float y;
    float length;
    float share_past_dead_zone;

    *stick_x = 0.0f;
    *stick_y = 0.0f;
    x = (raw_x - centre_x) / STICK_HALF_RANGE;
    y = (raw_y - centre_y) / STICK_HALF_RANGE;
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

void stick_aim_from_raw(int raw_x, int raw_y, float *stick_x, float *stick_y)
{
    aim_from_centre((float)raw_x, (float)raw_y, STICK_CENTRE, STICK_CENTRE, stick_x, stick_y);
}

static int is_steady(int raw, int last_raw)
{
    return raw - last_raw <= STICK_STEADY_RAW_UNITS && last_raw - raw <= STICK_STEADY_RAW_UNITS;
}

static int is_near_the_middle(int raw_x, int raw_y)
{
    float x;
    float y;

    x = ((float)raw_x - STICK_CENTRE) / STICK_HALF_RANGE;
    y = ((float)raw_y - STICK_CENTRE) / STICK_HALF_RANGE;
    return x * x + y * y <= STICK_REST_REACH * STICK_REST_REACH;
}

void stick_aim_read(struct stick_aim_rest *rest, int raw_x, int raw_y, float *stick_x, float *stick_y)
{
    if (!rest->is_known) {
        rest->is_known = 1;
        rest->centre_x = STICK_CENTRE;
        rest->centre_y = STICK_CENTRE;
        rest->last_raw_x = raw_x;
        rest->last_raw_y = raw_y;
        rest->steady_frames = 0;
    }
    if (is_steady(raw_x, rest->last_raw_x) && is_steady(raw_y, rest->last_raw_y)) {
        rest->steady_frames = rest->steady_frames + 1;
    } else {
        rest->steady_frames = 0;
    }
    rest->last_raw_x = raw_x;
    rest->last_raw_y = raw_y;
    if (rest->steady_frames >= STICK_REST_FRAMES && is_near_the_middle(raw_x, raw_y)) {
        rest->centre_x = (float)raw_x;
        rest->centre_y = (float)raw_y;
    }
    aim_from_centre((float)raw_x, (float)raw_y, rest->centre_x, rest->centre_y, stick_x, stick_y);
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
