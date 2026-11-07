#ifndef UI_CLIPS_H
#define UI_CLIPS_H

#include "asset_name.h"

#define UI_CLIP_MAX 48
#define UI_ELEMENT_MAX 320
#define UI_KEY_MAX 4096
#define UI_CLIP_NAME_MAX 32

enum ui_element_kind {
    UI_ELEMENT_SPRITE,
    UI_ELEMENT_RECT,
    UI_ELEMENT_BAND,
    UI_ELEMENT_REVEAL,
    UI_ELEMENT_HOURGLASS
};

enum ui_element_prop {
    UI_PROP_X,
    UI_PROP_Y,
    UI_PROP_WIDTH,
    UI_PROP_HEIGHT,
    UI_PROP_ROTATION,
    UI_PROP_ALPHA,
    UI_PROP_WINDOW,
    UI_PROP_COUNT
};

struct ui_track {
    unsigned short first_key;
    unsigned short key_count;
};

struct ui_key {
    float frame;
    float value;
};

struct ui_element {
    enum ui_element_kind kind;
    char texture_name[ASSET_NAME_MAX];
    float red;
    float green;
    float blue;
    struct ui_track tracks[UI_PROP_COUNT];
};

struct ui_clip {
    char name[UI_CLIP_NAME_MAX];
    int frame_count;
    int first_element;
    int element_count;
};

struct ui_clips {
    struct ui_clip clips[UI_CLIP_MAX];
    int clip_count;
    struct ui_element elements[UI_ELEMENT_MAX];
    int element_count;
    struct ui_key keys[UI_KEY_MAX];
    int key_count;
};

int ui_clips_read(struct ui_clips *clips, const char *text, unsigned int text_size);
const struct ui_clip *ui_clips_find(const struct ui_clips *clips, const char *name);
float ui_element_value(const struct ui_clips *clips, const struct ui_element *element, enum ui_element_prop prop, float frame, float value_when_missing);

#endif
