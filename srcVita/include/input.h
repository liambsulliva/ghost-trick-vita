#ifndef INPUT_H
#define INPUT_H

#define INPUT_UP 0x0001u
#define INPUT_DOWN 0x0002u
#define INPUT_LEFT 0x0004u
#define INPUT_RIGHT 0x0008u
#define INPUT_CROSS 0x0010u
#define INPUT_CIRCLE 0x0020u
#define INPUT_SQUARE 0x0040u
#define INPUT_TRIANGLE 0x0080u
#define INPUT_L 0x0100u
#define INPUT_R 0x0200u
#define INPUT_START 0x0400u
#define INPUT_SELECT 0x0800u

struct input_state {
    unsigned int pressed;
    unsigned int held;
    int touch_is_down;
    float touch_x;
    float touch_y;
    float stick_x;
    float stick_y;
    float right_stick_x;
    float right_stick_y;
};

void input_poll(struct input_state *state);

#endif
