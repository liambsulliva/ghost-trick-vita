#ifndef ANIMATION_H
#define ANIMATION_H

#include "texture_cache.h"

#define ANIMATION_FRAME_MAX 256
#define ANIMATION_NAME_MAX 96
#define ANIMATION_DEFAULT_TICKS_PER_SECOND 60

#define ANIMATION_OK 0
#define ANIMATION_ERR_EMPTY -1
#define ANIMATION_ERR_BAD_LINE -2
#define ANIMATION_ERR_TOO_MANY_FRAMES -3
#define ANIMATION_ERR_BAD_TICKS -4

struct animation_frame {
    char texture_name[TEXTURE_NAME_MAX];
    unsigned int ticks;
    float x;
    float y;
    float width;
    float height;
};

struct animation {
    char name[ANIMATION_NAME_MAX];
    unsigned int ticks_per_second;
    int loops;
    unsigned int frame_count;
    struct animation_frame frames[ANIMATION_FRAME_MAX];
};

typedef int (*animation_frame_ready_check)(const struct animation_frame *frame);

struct animation_player {
    const struct animation *animation;
    unsigned int frame_index;
    unsigned long long time_in_frame_us;
    unsigned int late_frame_count;
    int waiting_for_frame;
    int is_finished;
    int is_paused;
    unsigned char frame_was_shown[ANIMATION_FRAME_MAX];
};

int animation_parse(const char *text, unsigned int text_size, struct animation *animation);
void animation_player_start(struct animation_player *player, const struct animation *animation);
void animation_player_advance(struct animation_player *player, unsigned long long elapsed_us, animation_frame_ready_check frame_is_ready);
unsigned int animation_player_count_shown_frames(const struct animation_player *player);
const struct animation_frame *animation_player_current_frame(const struct animation_player *player);

#endif
