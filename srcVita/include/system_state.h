#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#define SYSTEM_STATE_UNREADABLE -1

struct system_state {
    int overlay;
    int bgm_adopt;
};

void system_state_read(struct system_state *state);

#endif
