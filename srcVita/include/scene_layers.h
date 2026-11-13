#ifndef SCENE_LAYERS_H
#define SCENE_LAYERS_H

#include "asset_name.h"

#define SCENE_TEXT_LINE_MAX 8
#define SCENE_OVERLAY_MAX 32

struct scene_text_line {
    char texture_name[ASSET_NAME_MAX];
    float center_x;
    float top_y;
    float alpha;
    float fade_start_alpha;
    float fade_target_alpha;
    unsigned long long fade_total_us;
    unsigned long long fade_elapsed_us;
    int id;
    int in_use;
};

struct scene_overlay {
    char texture_name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
    float size_fraction;
    float shake_x;
    float shake_y;
    unsigned long long grow_total_us;
    unsigned long long grow_elapsed_us;
    int id;
    int in_use;
};

struct scene_layers {
    struct scene_text_line text_lines[SCENE_TEXT_LINE_MAX];
    struct scene_overlay overlays[SCENE_OVERLAY_MAX];
};

void scene_layers_clear(struct scene_layers *layers);
void scene_layers_clear_overlays(struct scene_layers *layers);
void scene_layers_show_text(struct scene_layers *layers, int id, const char *texture_name, float center_x, float top_y, int fade_ticks);
void scene_layers_hide_text(struct scene_layers *layers, int id, int fade_ticks);
void scene_layers_hide_all_text(struct scene_layers *layers);
void scene_layers_finish_text_fade_ins(struct scene_layers *layers);
void scene_layers_show_overlay(struct scene_layers *layers, int id, const char *texture_name, float x, float y, float width, float height, int grow_ticks);
void scene_layers_hide_overlay(struct scene_layers *layers, int id);
void scene_layers_shake_overlay(struct scene_layers *layers, int id, float x, float y);
void scene_layers_advance(struct scene_layers *layers, unsigned long long elapsed_us);
int scene_layers_text_is_fading_out(const struct scene_layers *layers);

#endif
