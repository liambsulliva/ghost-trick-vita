#include <string.h>
#define input_poll real_input_poll
#include "../../src/vita/input.c"
#undef input_poll
#include "test_support.h"

#define PANEL_X 600
#define PANEL_Y 420
#define SCREEN_X 300.0f
#define SCREEN_Y 210.0f
#define STICK_CENTRE 128

static int finger_is_down;
static unsigned int buttons_held;

int sceCtrlSetSamplingMode(int mode)
{
    (void)mode;
    return 0;
}

int sceCtrlPeekBufferPositive(int port, SceCtrlData *pad_data, int count)
{
    (void)port;
    (void)count;
    pad_data->buttons = buttons_held;
    pad_data->lx = STICK_CENTRE;
    pad_data->ly = STICK_CENTRE;
    pad_data->rx = STICK_CENTRE;
    pad_data->ry = STICK_CENTRE;
    return 1;
}

int sceTouchSetSamplingState(int port, int state)
{
    (void)port;
    (void)state;
    return 0;
}

int sceTouchPeek(int port, SceTouchData *data, int count)
{
    (void)port;
    (void)count;
    data->reportNum = finger_is_down ? 1u : 0u;
    data->report[0].x = PANEL_X;
    data->report[0].y = PANEL_Y;
    return 1;
}

static void test_a_lifted_finger_keeps_the_place_it_last_touched(void)
{
    struct input_state state;

    memset(&state, 0, sizeof(state));
    finger_is_down = 1;
    real_input_poll(&state);
    CHECK(state.touch_is_down);
    CHECK(state.touch_x == SCREEN_X && state.touch_y == SCREEN_Y);
    finger_is_down = 0;
    real_input_poll(&state);
    CHECK(!state.touch_is_down);
    CHECK(state.touch_x == SCREEN_X && state.touch_y == SCREEN_Y);
}

static void test_a_button_is_pressed_once_and_held_after(void)
{
    struct input_state state;

    memset(&state, 0, sizeof(state));
    buttons_held = SCE_CTRL_CROSS;
    real_input_poll(&state);
    CHECK_INT((int)state.pressed, (int)INPUT_CROSS);
    real_input_poll(&state);
    CHECK_INT((int)state.pressed, 0);
    CHECK_INT((int)state.held, (int)INPUT_CROSS);
    buttons_held = 0;
    real_input_poll(&state);
}

int main(void)
{
    test_a_lifted_finger_keeps_the_place_it_last_touched();
    test_a_button_is_pressed_once_and_held_after();
    return test_finish("unit/vita_input");
}
