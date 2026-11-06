#ifndef ANIMATION_H
#define ANIMATION_H

#include "asset_name.h"

#define ANIMATION_FRAME_MAX 1024
#define TICKS_PER_SECOND 60
#define MICROSECONDS_PER_TICK (1000000ULL / TICKS_PER_SECOND)
#define ANIMATION_BLANK_TEXTURE "-"

#define ANIMATION_OK 0
#define ANIMATION_ERR_EMPTY -1
#define ANIMATION_ERR_BAD_LINE -2
#define ANIMATION_ERR_TOO_MANY_FRAMES -3
#define ANIMATION_ERR_BAD_TICKS -4
#define ANIMATION_ERR_NO_MEMORY -5

struct animation_frame {
    char texture_name[ASSET_NAME_MAX];
    unsigned int ticks;
    float x;
    float y;
    float width;
    float height;
    float u;
    float v;
    float u_size;
    float v_size;
};

struct animation {
    char name[ASSET_NAME_MAX];
    int loops;
    unsigned int frame_count;
    struct animation_frame *frames;
};

typedef int (*animation_frame_ready_check)(const struct animation_frame *frame);

struct animation_player {
    const struct animation *animation;
    unsigned int frame_index;
    unsigned long long time_in_frame_us;
    unsigned int late_frame_count;
    int waiting_for_frame;
    int is_finished;
    unsigned char frame_was_shown[ANIMATION_FRAME_MAX];
};

int animation_parse(const char *text, unsigned int text_size, struct animation *animation);
void animation_free_frames(struct animation *animation);
int animation_frame_is_blank(const struct animation_frame *frame);
void animation_player_start(struct animation_player *player, const struct animation *animation);
void animation_player_advance(struct animation_player *player, unsigned long long elapsed_us, animation_frame_ready_check frame_is_ready);
unsigned int animation_player_count_shown_frames(const struct animation_player *player);
const struct animation_frame *animation_player_current_frame(const struct animation_player *player);

#endif
