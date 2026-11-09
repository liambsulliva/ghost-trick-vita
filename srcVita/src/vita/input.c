#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include "input.h"
#include "stick_aim.h"

#define TOUCH_PANEL_UNITS_PER_SCREEN_PIXEL 2.0f

static unsigned int previous_buttons;
static int touch_is_sampling;
static int stick_is_sampling;

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
    state->touch_x = 0.0f;
    state->touch_y = 0.0f;
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
    sceCtrlPeekBufferPositive(0, &pad, 1);
    stick_aim_from_raw(pad.lx, pad.ly, &state->stick_x, &state->stick_y);
    stick_aim_from_raw(pad.rx, pad.ry, &state->right_stick_x, &state->right_stick_y);
    buttons = read_app_buttons(pad.buttons);
    state->pressed = buttons & ~previous_buttons;
    state->held = buttons;
    previous_buttons = buttons;
}
