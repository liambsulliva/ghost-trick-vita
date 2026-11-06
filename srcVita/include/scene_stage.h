#ifndef SCENE_STAGE_H
#define SCENE_STAGE_H

#include "animation.h"
#include "asset_name.h"
#include "scene_effects.h"

#define SCENE_PLATE_MAX 16
#define SCENE_OBJECT_MAX 64

struct scene_plate {
    char texture_name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

struct scene_object {
    char name[ASSET_NAME_MAX];
    struct animation_player player;
    float x;
    float y;
    int layer;
    int is_flipped;
    int in_use;
};

struct scene_stage {
    struct scene_plate plates[SCENE_PLATE_MAX];
    int plate_count;
    struct scene_object objects[SCENE_OBJECT_MAX];
    float camera_x;
    float camera_y;
    float fade_amount;
    char card_texture[ASSET_NAME_MAX];
    struct scene_effects effects;
};

void scene_stage_reset(void);
const struct scene_stage *scene_stage_view(void);
struct scene_effects *scene_stage_effects(void);
void scene_stage_clear_room(void);
void scene_stage_add_plate(const char *texture_name, float x, float y, float width, float height);
int scene_stage_set_object(const char *object_name, const char *animation_name, float x, float y, int layer, int is_flipped);
void scene_stage_remove_object(const char *object_name);
void scene_stage_set_camera(float x, float y);
void scene_stage_show_card(const char *texture_name);
void scene_stage_hide_card(void);
void scene_stage_set_fade(float fade_amount);
void scene_stage_advance_objects(unsigned long long elapsed_us);
void scene_stage_log_coverage(void);
void scene_stage_request_animation_textures(const struct animation *animation);

#endif
