#ifndef SCENE_H
#define SCENE_H

#include "asset_name.h"

#define SCENE_STEP_MAX 1024

#define SCENE_OK 0
#define SCENE_ERR_EMPTY -1
#define SCENE_ERR_BAD_LINE -2
#define SCENE_ERR_TOO_MANY_STEPS -3
#define SCENE_ERR_NO_NAME -4

enum scene_step_kind {
    SCENE_STEP_ROOM,
    SCENE_STEP_PLATE,
    SCENE_STEP_OBJECT,
    SCENE_STEP_REMOVE_OBJECT,
    SCENE_STEP_CAMERA,
    SCENE_STEP_CAMERA_PAN,
    SCENE_STEP_CARD,
    SCENE_STEP_HIDE_CARD,
    SCENE_STEP_FADE_OUT,
    SCENE_STEP_FADE_IN,
    SCENE_STEP_WAIT,
    SCENE_STEP_BEAM_CREATE,
    SCENE_STEP_BEAM_ANIMATE,
    SCENE_STEP_BEAM_DELETE,
    SCENE_STEP_MASK_ON,
    SCENE_STEP_MASK_OFF,
    SCENE_STEP_IRIS,
    SCENE_STEP_DIM_TO,
    SCENE_STEP_FLASH
};

struct scene_step {
    enum scene_step_kind kind;
    char object_name[ASSET_NAME_MAX];
    char animation_name[ASSET_NAME_MAX];
    char texture_name[ASSET_NAME_MAX];
    char beam_name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
    float apex_x;
    float apex_y;
    float half_width;
    float radius;
    float brightness;
    int room_id;
    int layer;
    int ticks;
    int is_flipped;
};

struct scene {
    char name[ASSET_NAME_MAX];
    int step_count;
    struct scene_step steps[SCENE_STEP_MAX];
};

int scene_parse(const char *text, unsigned int text_size, struct scene *scene);

#endif
