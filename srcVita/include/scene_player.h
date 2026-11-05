#ifndef SCENE_PLAYER_H
#define SCENE_PLAYER_H

#include "animation.h"
#include "scene.h"

#define SCENE_PLATE_MAX 16
#define SCENE_OBJECT_MAX 64

struct scene_plate {
    char texture_name[SCENE_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

struct scene_object {
    char name[SCENE_NAME_MAX];
    struct animation_player player;
    float x;
    float y;
    int layer;
    int is_flipped;
    int in_use;
};

struct scene_picture {
    struct scene_plate plates[SCENE_PLATE_MAX];
    int plate_count;
    struct scene_object objects[SCENE_OBJECT_MAX];
    float camera_x;
    float camera_y;
    float fade_amount;
    char card_texture[SCENE_NAME_MAX];
    char left_bar_texture[SCENE_NAME_MAX];
    char right_bar_texture[SCENE_NAME_MAX];
};

enum scene_player_status {
    SCENE_PLAYER_IDLE,
    SCENE_PLAYER_PRELOADING,
    SCENE_PLAYER_RUNNING,
    SCENE_PLAYER_FINISHED
};

void scene_player_init(const char *scene_folder);
int scene_player_play(const char *scene_name);
void scene_player_update(unsigned long long elapsed_us);
void scene_player_stop_script(void);
void scene_player_set_paused(int is_paused);
int scene_player_is_paused(void);
enum scene_player_status scene_player_status(void);
int scene_player_current_step(void);
const struct scene_picture *scene_player_picture(void);
void scene_player_clear_room(void);
void scene_player_add_plate(const char *texture_name, float x, float y, float width, float height);
int scene_player_set_object(const char *object_name, const char *animation_name, float x, float y, int layer, int is_flipped);
void scene_player_set_camera(float x, float y);
void scene_player_log_coverage(void);
void scene_player_shutdown(void);

#endif
