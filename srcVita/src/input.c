#include <psp2/ctrl.h>
#include "input.h"

static unsigned int previous_buttons;

void input_poll(struct input_state *state)
{
    SceCtrlData pad;
    unsigned int buttons;

    sceCtrlPeekBufferPositive(0, &pad, 1);
    buttons = pad.buttons;
    state->held = buttons;
    state->pressed = buttons & ~previous_buttons;
    previous_buttons = buttons;
}
