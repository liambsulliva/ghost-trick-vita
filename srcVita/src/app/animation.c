#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "text_lines.h"

#define ANIMATION_LINE_MAX 256

static int read_frame_line(const char *line, struct animation_frame *frame)
{
    int fields_read;

    fields_read = sscanf(line, "frame %95s %u %f %f %f %f", frame->texture_name, &frame->ticks, &frame->x, &frame->y, &frame->width, &frame->height);
    if (fields_read != 6) {
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
    if (strncmp(line, "animation ", 10) == 0) {
        return sscanf(line, "animation %95s", animation->name) == 1 ? ANIMATION_OK : ANIMATION_ERR_BAD_LINE;
    }
    if (strncmp(line, "ticks_per_second ", 17) == 0) {
        if (sscanf(line, "ticks_per_second %u", &animation->ticks_per_second) != 1 || animation->ticks_per_second == 0) {
            return ANIMATION_ERR_BAD_LINE;
        }
        return ANIMATION_OK;
    }
    if (strncmp(line, "loops ", 6) == 0) {
        return sscanf(line, "loops %d", &animation->loops) == 1 ? ANIMATION_OK : ANIMATION_ERR_BAD_LINE;
    }
    if (strncmp(line, "frame ", 6) == 0) {
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
    animation->ticks_per_second = ANIMATION_DEFAULT_TICKS_PER_SECOND;
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

static unsigned long long frame_duration_us(const struct animation *animation, const struct animation_frame *frame)
{
    return (unsigned long long)frame->ticks * 1000000ULL / animation->ticks_per_second;
}

void animation_player_start(struct animation_player *player, const struct animation *animation)
{
    player->animation = animation;
    player->frame_index = 0;
    player->time_in_frame_us = 0;
    player->is_finished = 0;
    player->is_paused = 0;
    memset(player->frame_was_shown, 0, sizeof(player->frame_was_shown));
    player->frame_was_shown[0] = 1;
}

void animation_player_advance(struct animation_player *player, unsigned long long elapsed_us)
{
    const struct animation *animation;
    unsigned long long duration_us;

    animation = player->animation;
    if (animation == NULL || player->is_finished || player->is_paused) {
        return;
    }
    player->time_in_frame_us = player->time_in_frame_us + elapsed_us;
    while (1) {
        duration_us = frame_duration_us(animation, &animation->frames[player->frame_index]);
        if (player->time_in_frame_us < duration_us) {
            return;
        }
        player->time_in_frame_us = player->time_in_frame_us - duration_us;
        if (player->frame_index + 1 < animation->frame_count) {
            player->frame_index = player->frame_index + 1;
        } else if (animation->loops) {
            player->frame_index = 0;
        } else {
            player->is_finished = 1;
            player->time_in_frame_us = 0;
            return;
        }
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
