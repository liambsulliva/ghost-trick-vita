#include "fx_playhead.h"

#include <string.h>

#define ROLL_MULTIPLIER 1103515245u
#define ROLL_INCREMENT 12345u
#define ROLL_SHIFT 16

void fx_playhead_start(struct fx_playhead *playhead)
{
    memset(playhead, 0, sizeof(*playhead));
}

static int movie_reads_status(const struct script_fx_movie *movie, const struct script_fx_jump *jumps)
{
    int index;

    for (index = 0; index < movie->jump_count; index++) {
        if (jumps[movie->first_jump + index].condition == SCRIPT_FX_JUMP_WHEN_STATUS_IS) {
            return 1;
        }
    }
    return 0;
}

static int roll(struct fx_playhead *playhead, int sides)
{
    playhead->rolls = playhead->rolls * ROLL_MULTIPLIER + ROLL_INCREMENT;
    return sides > 0 ? (int)((playhead->rolls >> ROLL_SHIFT) % (unsigned int)sides) : 0;
}

static int jump_applies(struct fx_playhead *playhead, const struct script_fx_jump *jump)
{
    switch (jump->condition) {
    case SCRIPT_FX_JUMP_WHEN_STATUS_IS:
        return playhead->status == jump->value;
    case SCRIPT_FX_JUMP_ONE_TIME_IN:
        return roll(playhead, jump->value) == 0;
    default:
        return 1;
    }
}

static void step_one_frame(struct fx_playhead *playhead, const struct script_fx_movie *movie, const struct script_fx_jump *jumps)
{
    int index;

    for (index = 0; index < movie->jump_count; index++) {
        const struct script_fx_jump *jump;

        jump = &jumps[movie->first_jump + index];
        if (jump->frame == playhead->frame && jump_applies(playhead, jump)) {
            if (jump->target != SCRIPT_FX_NO_FRAME) {
                playhead->frame = jump->target;
                return;
            }
            break;
        }
    }
    playhead->frame = playhead->frame + 1;
    if (playhead->frame < movie->frame_count) {
        return;
    }
    if (movie->after_last == SCRIPT_FX_AFTER_LOOP) {
        playhead->frame = movie->loop_start;
        return;
    }
    playhead->frame = movie->frame_count - 1;
    playhead->is_over = movie->after_last == SCRIPT_FX_AFTER_END;
}

void fx_playhead_advance(struct fx_playhead *playhead, const struct script_fx_movie *movie, const struct script_fx_jump *jumps)
{
    int frames_due;

    if (movie == NULL || movie->frame_count <= 0 || movie->ticks_per_frame <= 0.0f || playhead->is_over) {
        return;
    }
    playhead->age_ticks = playhead->age_ticks + 1;
    frames_due = (int)((float)playhead->age_ticks / movie->ticks_per_frame);
    while (playhead->frames_stepped < frames_due && !playhead->is_over) {
        playhead->frames_stepped = playhead->frames_stepped + 1;
        step_one_frame(playhead, movie, jumps);
    }
}

void fx_playhead_set_status(struct fx_playhead *playhead, const struct script_fx_movie *movie, const struct script_fx_jump *jumps, int status)
{
    if (movie == NULL) {
        return;
    }
    if (movie_reads_status(movie, jumps)) {
        playhead->status = status;
        return;
    }
    if (status >= 0 && status < SCRIPT_FX_STATUS_MAX && movie->status_frames[status] != SCRIPT_FX_NO_FRAME && movie->status_frames[status] < movie->frame_count) {
        playhead->frame = movie->status_frames[status];
        playhead->is_over = 0;
    }
}

int fx_playhead_frame_at_age(const struct script_fx_movie *movie, const struct script_fx_jump *jumps, int age_ticks, int *is_over)
{
    struct fx_playhead playhead;
    int tick;

    fx_playhead_start(&playhead);
    for (tick = 0; tick < age_ticks && !playhead.is_over; tick++) {
        fx_playhead_advance(&playhead, movie, jumps);
    }
    *is_over = playhead.is_over;
    return playhead.frame;
}
