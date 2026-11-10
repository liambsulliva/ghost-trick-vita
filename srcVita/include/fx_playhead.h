#ifndef FX_PLAYHEAD_H
#define FX_PLAYHEAD_H

#include "script_tables.h"

struct fx_playhead {
    int frame;
    int age_ticks;
    int frames_stepped;
    int status;
    int is_over;
    unsigned int rolls;
};

void fx_playhead_start(struct fx_playhead *playhead);
void fx_playhead_advance(struct fx_playhead *playhead, const struct script_fx_movie *movie, const struct script_fx_jump *jumps);
void fx_playhead_set_status(struct fx_playhead *playhead, const struct script_fx_movie *movie, const struct script_fx_jump *jumps, int status);
int fx_playhead_frame_at_age(const struct script_fx_movie *movie, const struct script_fx_jump *jumps, int age_ticks, int *is_over);

#endif
