#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_lines.h"
#include "ui_clips.h"

#define UI_LINE_MAX 768

static const char *const prop_words[UI_PROP_COUNT] = { "x", "y", "w", "h", "rot", "alpha", "window" };

static int read_clip_line(struct ui_clips *clips, const char *line)
{
    struct ui_clip *clip;

    if (clips->clip_count >= UI_CLIP_MAX) {
        return 0;
    }
    clip = &clips->clips[clips->clip_count];
    if (sscanf(line, "clip %31s %d", clip->name, &clip->frame_count) != 2) {
        return 0;
    }
    clip->first_element = clips->element_count;
    clip->element_count = 0;
    clips->clip_count = clips->clip_count + 1;
    return 1;
}

static int read_element_line(struct ui_clips *clips, const char *line)
{
    static const char *const kind_words[] = { "sprite", "rect", "band", "reveal", "hourglass" };
    struct ui_element *element;
    char kind_word[16];
    unsigned int index;

    if (clips->clip_count == 0 || clips->element_count >= UI_ELEMENT_MAX) {
        return 0;
    }
    element = &clips->elements[clips->element_count];
    memset(element, 0, sizeof(*element));
    if (sscanf(line, "element %15s %95s %f %f %f", kind_word, element->texture_name, &element->red, &element->green, &element->blue) != 5) {
        return 0;
    }
    for (index = 0; index < sizeof(kind_words) / sizeof(kind_words[0]); index++) {
        if (strcmp(kind_word, kind_words[index]) == 0) {
            element->kind = (enum ui_element_kind)index;
            clips->element_count = clips->element_count + 1;
            clips->clips[clips->clip_count - 1].element_count = clips->clips[clips->clip_count - 1].element_count + 1;
            return 1;
        }
    }
    return 0;
}

static int read_track_line(struct ui_clips *clips, const char *line)
{
    struct ui_element *element;
    struct ui_track *track;
    char prop_word[16];
    int consumed;
    const char *cursor;
    int prop;

    if (clips->element_count == 0 || sscanf(line, "track %15s%n", prop_word, &consumed) != 1) {
        return 0;
    }
    element = &clips->elements[clips->element_count - 1];
    track = NULL;
    for (prop = 0; prop < UI_PROP_COUNT; prop++) {
        if (strcmp(prop_word, prop_words[prop]) == 0) {
            track = &element->tracks[prop];
        }
    }
    if (track == NULL) {
        return 0;
    }
    track->first_key = (unsigned short)clips->key_count;
    track->key_count = 0;
    cursor = line + consumed;
    for (;;) {
        char *after_frame;
        char *after_value;
        float frame;
        float value;

        frame = strtof(cursor, &after_frame);
        if (after_frame == cursor) {
            return track->key_count > 0;
        }
        value = strtof(after_frame, &after_value);
        if (after_value == after_frame || clips->key_count >= UI_KEY_MAX) {
            return 0;
        }
        clips->keys[clips->key_count].frame = frame;
        clips->keys[clips->key_count].value = value;
        clips->key_count = clips->key_count + 1;
        track->key_count = (unsigned short)(track->key_count + 1);
        cursor = after_value;
    }
}

int ui_clips_read(struct ui_clips *clips, const char *text, unsigned int text_size)
{
    char line[UI_LINE_MAX];
    unsigned int offset;

    memset(clips, 0, sizeof(*clips));
    offset = 0;
    while (offset < text_size) {
        int ok;

        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (strncmp(line, "clip ", 5) == 0) {
            ok = read_clip_line(clips, line);
        } else if (strncmp(line, "element ", 8) == 0) {
            ok = read_element_line(clips, line);
        } else {
            ok = read_track_line(clips, line);
        }
        if (!ok) {
            return 0;
        }
    }
    return 1;
}

const struct ui_clip *ui_clips_find(const struct ui_clips *clips, const char *name)
{
    int index;

    for (index = 0; index < clips->clip_count; index++) {
        if (strcmp(clips->clips[index].name, name) == 0) {
            return &clips->clips[index];
        }
    }
    return NULL;
}

float ui_element_value(const struct ui_clips *clips, const struct ui_element *element, enum ui_element_prop prop, float frame, float value_when_missing)
{
    const struct ui_track *track;
    const struct ui_key *keys;
    int index;

    track = &element->tracks[prop];
    if (track->key_count == 0) {
        return value_when_missing;
    }
    keys = &clips->keys[track->first_key];
    if (frame <= keys[0].frame) {
        return keys[0].value;
    }
    for (index = 1; index < track->key_count; index++) {
        if (frame <= keys[index].frame) {
            float span;

            span = keys[index].frame - keys[index - 1].frame;
            if (span <= 0.0f) {
                return keys[index].value;
            }
            return keys[index - 1].value + (keys[index].value - keys[index - 1].value) * (frame - keys[index - 1].frame) / span;
        }
    }
    return keys[track->key_count - 1].value;
}
