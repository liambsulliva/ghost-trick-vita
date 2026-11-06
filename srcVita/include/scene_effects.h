#ifndef SCENE_EFFECTS_H
#define SCENE_EFFECTS_H

#include "asset_name.h"
#include "spotlight_mask.h"

#define SCENE_BEAM_MAX SPOTLIGHT_MASK_TRIANGLE_MAX

struct scene_ramp {
    float value;
    float start;
    float target;
    unsigned long long elapsed_us;
    unsigned long long total_us;
};

struct scene_beam {
    char name[ASSET_NAME_MAX];
    float apex_x;
    float apex_y;
    struct scene_ramp base_x;
    struct scene_ramp base_y;
    struct scene_ramp half_width;
    int in_use;
};

struct scene_effects {
    struct scene_beam beams[SCENE_BEAM_MAX];
    int mask_is_on;
    int iris_is_on;
    float iris_x;
    float iris_y;
    struct scene_ramp iris_radius;
    struct scene_ramp brightness;
    struct scene_ramp flash_level;
};

void scene_effects_reset(struct scene_effects *effects);
void scene_effects_advance(struct scene_effects *effects, unsigned long long elapsed_us);
void scene_effects_create_beam(struct scene_effects *effects, const char *name, float apex_x, float apex_y, float base_x, float base_y, float half_width);
void scene_effects_animate_beam(struct scene_effects *effects, const char *name, float base_x, float base_y, float half_width, int ticks);
void scene_effects_delete_beam(struct scene_effects *effects, const char *name);
void scene_effects_set_mask(struct scene_effects *effects, int should_be_on);
void scene_effects_start_iris(struct scene_effects *effects, float x, float y, float radius, int ticks);
void scene_effects_dim_to(struct scene_effects *effects, float brightness, int ticks);
void scene_effects_flash(struct scene_effects *effects, float brightness, int ticks);

#endif
