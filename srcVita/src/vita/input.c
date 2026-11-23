#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include "input.h"
#include "run_log.h"
#include "stick_aim.h"

#define TOUCH_PANEL_UNITS_PER_SCREEN_PIXEL 2.0f
#define STICK_LOG_POLLS 60
#define RAW_UNITS 256

static unsigned int previous_buttons;
static int touch_is_sampling;
static int stick_is_sampling;
static struct stick_aim_rest left_stick_rest;
static struct stick_aim_rest right_stick_rest;
static int stick_log_polls;
static int right_x_low = RAW_UNITS;
static int right_x_high = -1;
static int right_y_low = RAW_UNITS;
static int right_y_high = -1;

static void note_right_stick(int raw_x, int raw_y, const struct input_state *state)
{
    right_x_low = raw_x < right_x_low ? raw_x : right_x_low;
    right_x_high = raw_x > right_x_high ? raw_x : right_x_high;
    right_y_low = raw_y < right_y_low ? raw_y : right_y_low;
    right_y_high = raw_y > right_y_high ? raw_y : right_y_high;
    stick_log_polls = stick_log_polls + 1;
    if (stick_log_polls < STICK_LOG_POLLS) {
        return;
    }
    run_log_write("right_stick x=%d..%d y=%d..%d rest=%d,%d steady=%d aim=%.2f,%.2f", right_x_low, right_x_high, right_y_low, right_y_high, (int)right_stick_rest.centre_x, (int)right_stick_rest.centre_y, right_stick_rest.steady_frames, (double)state->right_stick_x, (double)state->right_stick_y);
    stick_log_polls = 0;
    right_x_low = RAW_UNITS;
    right_x_high = -1;
    right_y_low = RAW_UNITS;
    right_y_high = -1;
}

static unsigned int add_button_if_held(unsigned int vita_buttons, unsigned int vita_button, unsigned int app_button)
{
    if (vita_buttons & vita_button) {
        return app_button;
    }
    return 0;
}

static unsigned int read_app_buttons(unsigned int vita_buttons)
{
    unsigned int buttons;

    buttons = 0;
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_UP, INPUT_UP);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_DOWN, INPUT_DOWN);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_LEFT, INPUT_LEFT);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_RIGHT, INPUT_RIGHT);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_CROSS, INPUT_CROSS);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_CIRCLE, INPUT_CIRCLE);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_SQUARE, INPUT_SQUARE);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_TRIANGLE, INPUT_TRIANGLE);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_LTRIGGER, INPUT_L);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_RTRIGGER, INPUT_R);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_START, INPUT_START);
    buttons |= add_button_if_held(vita_buttons, SCE_CTRL_SELECT, INPUT_SELECT);
    return buttons;
}

static void read_front_touch(struct input_state *state)
{
    SceTouchData touch;

    if (!touch_is_sampling) {
        sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
        touch_is_sampling = 1;
    }
    state->touch_is_down = 0;
    if (sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1) > 0 && touch.reportNum > 0) {
        state->touch_is_down = 1;
        state->touch_x = (float)touch.report[0].x / TOUCH_PANEL_UNITS_PER_SCREEN_PIXEL;
        state->touch_y = (float)touch.report[0].y / TOUCH_PANEL_UNITS_PER_SCREEN_PIXEL;
    }
}

void input_poll(struct input_state *state)
{
    SceCtrlData pad;
    unsigned int buttons;

    read_front_touch(state);
    if (!stick_is_sampling) {
        sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
        stick_is_sampling = 1;
    }
    if (sceCtrlPeekBufferPositive(0, &pad, 1) < 1) {
        pad.buttons = 0;
        pad.lx = STICK_AIM_RAW_CENTRE;
        pad.ly = STICK_AIM_RAW_CENTRE;
        pad.rx = STICK_AIM_RAW_CENTRE;
        pad.ry = STICK_AIM_RAW_CENTRE;
    }
    stick_aim_read(&left_stick_rest, pad.lx, pad.ly, &state->stick_x, &state->stick_y);
    stick_aim_read(&right_stick_rest, pad.rx, pad.ry, &state->right_stick_x, &state->right_stick_y);
    note_right_stick(pad.rx, pad.ry, state);
    buttons = read_app_buttons(pad.buttons);
    state->pressed = buttons & ~previous_buttons;
    state->held = buttons;
    previous_buttons = buttons;
}
