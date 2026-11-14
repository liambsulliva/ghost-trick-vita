#ifndef MENU_INPUT_H
#define MENU_INPUT_H

#include "input.h"

#define MENU_INPUT_REPEAT_DELAY_US 400000ull
#define MENU_INPUT_REPEAT_EVERY_US 90000ull
#define MENU_INPUT_DIRECTIONS (INPUT_UP | INPUT_DOWN | INPUT_LEFT | INPUT_RIGHT)

struct menu_input {
    int stick_was_out;
    unsigned int repeat_directions;
    int repeat_is_armed;
    unsigned long long repeat_held_us;
};

void menu_input_reset(struct menu_input *state);
unsigned int menu_input_stick_presses(struct menu_input *state, const struct input_state *input);
unsigned int menu_input_repeat_presses(struct menu_input *state, const struct input_state *input, unsigned int fresh, unsigned long long elapsed_us);

#endif
