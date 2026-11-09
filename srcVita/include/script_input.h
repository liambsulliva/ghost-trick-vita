#ifndef SCRIPT_INPUT_H
#define SCRIPT_INPUT_H

#include "input.h"

struct script_machine;

struct script_input {
    int touch_was_down;
    int touch_is_dragging;
    float touch_last_x;
    float touch_last_y;
    int stick_was_out;
};

void script_input_reset(struct script_input *input);
void script_input_frame(struct script_input *input, struct script_machine *machine, const struct input_state *frame);

#endif
