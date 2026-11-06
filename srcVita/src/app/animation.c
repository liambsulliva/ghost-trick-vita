#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "text_lines.h"

#define ANIMATION_LINE_MAX 256

static int read_frame_line(const char *line, struct animation_frame *frame)
{
    int fields_read;

    fields_read = sscanf(line, "frame " ASSET_NAME_SCAN " %u %f %f %f %f %f %f %f %f", frame->texture_name, &frame->ticks, &frame->x, &frame->y, &frame->width, &frame->height, &frame->u, &frame->v, &frame->u_size, &frame->v_size);
    if (fields_read != 10) {
        return ANIMATION_ERR_BAD_LINE;
    }
    if (frame->ticks == 0) {
        return ANIMATION_ERR_BAD_TICKS;
    }
    return ANIMATION_OK;
}

static int read_one_line(const char *line, struct animation *animation)
{
    if (line[0] == '\0') {
        return ANIMATION_OK;
    }
    if (text_line_starts_with(line, "animation ")) {
        return sscanf(line, "animation " ASSET_NAME_SCAN, animation->name) == 1 ? ANIMATION_OK : ANIMATION_ERR_BAD_LINE;
    }
    if (text_line_starts_with(line, "loops ")) {
        return sscanf(line, "loops %d", &animation->loops) == 1 ? ANIMATION_OK : ANIMATION_ERR_BAD_LINE;
    }
    if (text_line_starts_with(line, "frame ")) {
        int result;

        if (animation->frame_count >= ANIMATION_FRAME_MAX) {
            return ANIMATION_ERR_TOO_MANY_FRAMES;
        }
        result = read_frame_line(line, &animation->frames[animation->frame_count]);
        if (result == ANIMATION_OK) {
            animation->frame_count = animation->frame_count + 1;
        }
        return result;
    }
    return ANIMATION_ERR_BAD_LINE;
}

int animation_parse(const char *text, unsigned int text_size, struct animation *animation)
{
    char line[ANIMATION_LINE_MAX];
    unsigned int offset;
    int result;

    animation->name[0] = '\0';
    animation->loops = 0;
    animation->frame_count = 0;

    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        result = read_one_line(line, animation);
        if (result != ANIMATION_OK) {
            return result;
        }
    }
    if (animation->frame_count == 0) {
        return ANIMATION_ERR_EMPTY;
    }
    return ANIMATION_OK;
}

int animation_frame_is_blank(const struct animation_frame *frame)
{
    return strcmp(frame->texture_name, ANIMATION_BLANK_TEXTURE) == 0;
}

static unsigned long long frame_duration_us(const struct animation_frame *frame)
{
    return (unsigned long long)frame->ticks * 1000000ULL / TICKS_PER_SECOND;
}

void animation_player_start(struct animation_player *player, const struct animation *animation)
{
    player->animation = animation;
    player->frame_index = 0;
    player->time_in_frame_us = 0;
    player->late_frame_count = 0;
    player->waiting_for_frame = 0;
    player->is_finished = 0;
    memset(player->frame_was_shown, 0, sizeof(player->frame_was_shown));
    player->frame_was_shown[0] = 1;
}

static int next_frame_index(const struct animation *animation, unsigned int frame_index)
{
    if (frame_index + 1 < animation->frame_count) {
        return (int)frame_index + 1;
    }
    if (animation->loops) {
        return 0;
    }
    return -1;
}

static void move_to_frame_for_elapsed_time(struct animation_player *player, unsigned long long elapsed_us, animation_frame_ready_check frame_is_ready)
{
    const struct animation *animation;
    unsigned long long duration_us;
    int next_index;

    animation = player->animation;
    if (animation == NULL || player->is_finished) {
        return;
    }
    player->time_in_frame_us = player->time_in_frame_us + elapsed_us;
    while (1) {
        duration_us = frame_duration_us(&animation->frames[player->frame_index]);
        if (player->time_in_frame_us < duration_us) {
            return;
        }
        next_index = next_frame_index(animation, player->frame_index);
        if (next_index < 0) {
            player->is_finished = 1;
            player->time_in_frame_us = 0;
            return;
        }
        if (frame_is_ready != NULL && !frame_is_ready(&animation->frames[next_index])) {
            if (!player->waiting_for_frame) {
                player->waiting_for_frame = 1;
                player->late_frame_count = player->late_frame_count + 1;
            }
            player->time_in_frame_us = duration_us;
            return;
        }
        player->waiting_for_frame = 0;
        player->time_in_frame_us = player->time_in_frame_us - duration_us;
        player->frame_index = (unsigned int)next_index;
    }
}

void animation_player_advance(struct animation_player *player, unsigned long long elapsed_us, animation_frame_ready_check frame_is_ready)
{
    move_to_frame_for_elapsed_time(player, elapsed_us, frame_is_ready);
    if (player->animation != NULL) {
        player->frame_was_shown[player->frame_index] = 1;
    }
}

unsigned int animation_player_count_shown_frames(const struct animation_player *player)
{
    unsigned int count;
    unsigned int index;

    count = 0;
    if (player->animation == NULL) {
        return 0;
    }
    for (index = 0; index < player->animation->frame_count; index++) {
        if (player->frame_was_shown[index]) {
            count = count + 1;
        }
    }
    return count;
}

const struct animation_frame *animation_player_current_frame(const struct animation_player *player)
{
    if (player->animation == NULL) {
        return NULL;
    }
    return &player->animation->frames[player->frame_index];
}
