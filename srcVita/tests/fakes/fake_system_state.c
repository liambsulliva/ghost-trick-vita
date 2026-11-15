#include "fake_system_state.h"
#include "system_state.h"

static struct system_state current_state;
static int current_language = 1;

void fake_system_state_set(int overlay, int bgm_adopt)
{
    current_state.overlay = overlay;
    current_state.bgm_adopt = bgm_adopt;
}

void fake_system_state_set_language(int language)
{
    current_language = language;
}

int system_state_language(void)
{
    return current_language;
}

void system_state_read(struct system_state *state)
{
    *state = current_state;
}
