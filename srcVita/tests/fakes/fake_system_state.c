#include "fake_system_state.h"
#include "system_state.h"

static struct system_state current_state;

void fake_system_state_set(int overlay, int bgm_adopt)
{
    current_state.overlay = overlay;
    current_state.bgm_adopt = bgm_adopt;
}

void system_state_read(struct system_state *state)
{
    *state = current_state;
}
