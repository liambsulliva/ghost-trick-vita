#ifndef STAFF_CREDITS_H
#define STAFF_CREDITS_H

#include "asset_name.h"
#include "music_engine.h"
#include "script_machine.h"
#include "script_staff_roll.h"

#define STAFF_CREDITS_FILE_NAME "effects"

enum staff_credits_step {
    STAFF_CREDITS_OFF,
    STAFF_CREDITS_LOADING,
    STAFF_CREDITS_FADING_IN,
    STAFF_CREDITS_SHOWING,
    STAFF_CREDITS_HOLDING,
    STAFF_CREDITS_FADING_OUT,
    STAFF_CREDITS_CLOSING
};

struct staff_credits {
    struct script_staff_page_row pages[SCRIPT_STAFF_PAGE_MAX];
    int page_count;
    char music_name[ASSET_NAME_MAX];
    struct script_staff_roll roll;
    struct script_fade fade;
    struct music_memory music_before;
    enum staff_credits_step step;
    int page_index;
    int hold_ticks_left;
    int brightness;
};

void staff_credits_clear(struct staff_credits *credits);
int staff_credits_read(struct staff_credits *credits, const char *text, unsigned int text_size);
int staff_credits_can_play(const struct staff_credits *credits);
void staff_credits_start(struct staff_credits *credits);
int staff_credits_step(struct staff_credits *credits, int page_is_ready);
void staff_credits_stop(struct staff_credits *credits);
int staff_credits_is_playing(const struct staff_credits *credits);
const struct script_staff_page_row *staff_credits_page(const struct staff_credits *credits);
int staff_credits_page_is_drawn(const struct staff_credits *credits);
float staff_credits_scroll(const struct staff_credits *credits);
float staff_credits_black(const struct staff_credits *credits);

#endif
