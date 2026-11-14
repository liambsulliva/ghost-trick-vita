#include "menu_input.h"
#include "stick_aim.h"

void menu_input_reset(struct menu_input *state)
{
    state->stick_was_out = 1;
    state->repeat_directions = 0;
    state->repeat_is_armed = 0;
    state->repeat_held_us = 0;
}

static unsigned int stick_directions(const struct input_state *input)
{
    int pushed_x;
    int pushed_y;

    if (input->stick_x == 0.0f && input->stick_y == 0.0f) {
        return 0;
    }
    stick_aim_first_push(input->stick_x, input->stick_y, &pushed_x, &pushed_y);
    return (pushed_x < 0 ? INPUT_LEFT : 0) | (pushed_x > 0 ? INPUT_RIGHT : 0) | (pushed_y < 0 ? INPUT_UP : 0) | (pushed_y > 0 ? INPUT_DOWN : 0);
}

unsigned int menu_input_stick_presses(struct menu_input *state, const struct input_state *input)
{
    unsigned int pressed;
    int stick_is_out;

    stick_is_out = input->stick_x != 0.0f || input->stick_y != 0.0f;
    pressed = stick_is_out && !state->stick_was_out ? stick_directions(input) : 0;
    state->stick_was_out = stick_is_out;
    return pressed;
}

unsigned int menu_input_repeat_presses(struct menu_input *state, const struct input_state *input, unsigned int fresh, unsigned long long elapsed_us)
{
    unsigned int held;
    unsigned long long before;

    held = (input->held & MENU_INPUT_DIRECTIONS) | stick_directions(input);
    if (held == 0 || held != state->repeat_directions) {
        state->repeat_directions = held;
        state->repeat_is_armed = held != 0 && (fresh & MENU_INPUT_DIRECTIONS) != 0;
        state->repeat_held_us = 0;
        return 0;
    }
    if (!state->repeat_is_armed) {
        return 0;
    }
    before = state->repeat_held_us;
    state->repeat_held_us = state->repeat_held_us + elapsed_us;
    if (state->repeat_held_us < MENU_INPUT_REPEAT_DELAY_US) {
        return 0;
    }
    if (before < MENU_INPUT_REPEAT_DELAY_US || (state->repeat_held_us - MENU_INPUT_REPEAT_DELAY_US) / MENU_INPUT_REPEAT_EVERY_US != (before - MENU_INPUT_REPEAT_DELAY_US) / MENU_INPUT_REPEAT_EVERY_US) {
        return held;
    }
    return 0;
}
