#ifndef INPUT_H
#define INPUT_H

struct input_state {
    unsigned int held;
    unsigned int pressed;
};

void input_poll(struct input_state *state);

#endif
