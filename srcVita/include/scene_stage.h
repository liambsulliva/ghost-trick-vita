#ifndef SCENE_STAGE_H
#define SCENE_STAGE_H

#include "animation.h"
#include "asset_name.h"
#include "scene_effects.h"
#include "scene_layers.h"

#define SCENE_PLATE_MAX 16
#define SCENE_OBJECT_MAX 128

struct scene_plate {
    char texture_name[ASSET_NAME_MAX];
    char soul_texture_name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

#define SCENE_COLOR_LEVEL_NORMAL 256

struct scene_object {
    char name[ASSET_NAME_MAX];
    struct animation_player player;
    float x;
    float y;
    int layer;
    int is_flipped;
    int in_use;
    int hides_in_living_view;
    int hides_in_soul_view;
    int has_soul_tint;
    float soul_red;
    float soul_green;
    float soul_blue;
    float soul_alpha;
    int soul_is_flat;
    int color_level;
};

struct scene_stage {
    struct scene_plate plates[SCENE_PLATE_MAX];
    int plate_count;
    struct scene_object objects[SCENE_OBJECT_MAX];
    float camera_x;
    float camera_y;
    float object_origin_x;
    float object_origin_y;
    float fade_amount;
    int soul_mode_is_on;
    int mono_mode_is_on;
    int background_color_level;
    char card_texture[ASSET_NAME_MAX];
    struct scene_effects effects;
    struct scene_layers layers;
};

void scene_stage_reset(void);
const struct scene_stage *scene_stage_view(void);
struct scene_effects *scene_stage_effects(void);
struct scene_layers *scene_stage_layers(void);
void scene_stage_clear_room(void);
void scene_stage_clear_plates(void);
void scene_stage_add_plate(const char *texture_name, float x, float y, float width, float height);
void scene_stage_set_last_plate_soul_texture(const char *texture_name);
void scene_stage_set_soul_mode(int is_on);
void scene_stage_set_mono_mode(int is_on);
void scene_stage_set_object_flipped(const char *object_name, int is_flipped);
void scene_stage_set_object_soul_tint(const char *object_name, float red, float green, float blue, float alpha, int is_flat);
void scene_stage_set_object_color_level(const char *object_name, int level);
void scene_stage_set_background_color_level(int level);
int scene_stage_set_object(const char *object_name, const char *animation_name, float x, float y, int layer, int is_flipped);
void scene_stage_remove_object(const char *object_name);
void scene_stage_place_object(const char *object_name, const char *animation_name, float x, float y, int layer, int frame_index);
void scene_stage_set_camera(float x, float y);
void scene_stage_set_object_origin(float x, float y);
void scene_stage_set_object_views(const char *object_name, int shows_in_living_view, int shows_in_soul_view);
void scene_stage_show_card(const char *texture_name);
void scene_stage_hide_card(void);
void scene_stage_set_fade(float fade_amount);
void scene_stage_advance_objects(unsigned long long elapsed_us);
void scene_stage_log_coverage(void);
void scene_stage_request_animation_textures(const struct animation *animation);
int scene_stage_plays_animation(const struct animation *animation);

#endif
