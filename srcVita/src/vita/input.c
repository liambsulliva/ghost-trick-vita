#include <psp2/ctrl.h>
#include "input.h"

static unsigned int previous_buttons;

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

void input_poll(struct input_state *state)
{
    SceCtrlData pad;
    unsigned int buttons;

    sceCtrlPeekBufferPositive(0, &pad, 1);
    buttons = read_app_buttons(pad.buttons);
    state->pressed = buttons & ~previous_buttons;
    state->held = buttons;
    previous_buttons = buttons;
}
