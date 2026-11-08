#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include "renderer.h"
#include "scene_stage.h"
#include "ds_units.h"

#define SCENE_VIEW_DS_SCREEN_WIDTH 256.0f
#define SCENE_VIEW_DS_SCREEN_HEIGHT 192.0f
#define SCENE_VIEW_WIDTH_IN_DS_PIXELS ((float)RENDERER_WIDTH / VITA_PIXELS_PER_DS_PIXEL)
#define SCENE_VIEW_HEIGHT_IN_DS_PIXELS ((float)RENDERER_HEIGHT / VITA_PIXELS_PER_DS_PIXEL)
#define SCENE_VIEW_CARD_WIDTH 720.0f
#define SCENE_VIEW_CARD_HEIGHT 540.0f
#define SCENE_VIEW_DISSOLVE_ANCHOR_X ((float)RENDERER_WIDTH / 2.0f)
#define SCENE_VIEW_DISSOLVE_ANCHOR_Y (((float)RENDERER_HEIGHT + SCENE_VIEW_DS_SCREEN_HEIGHT * VITA_PIXELS_PER_DS_PIXEL) / 2.0f)

void scene_view_start(void);
void scene_view_stop(void);
int scene_view_visible_art_is_loading(const struct scene_stage *stage);
void scene_view_screen_to_world(const struct scene_stage *stage, float screen_x, float screen_y, float *x, float *y);
void scene_view_world_to_screen(const struct scene_stage *stage, float x, float y, float *screen_x, float *screen_y);
void scene_view_forget_held_poses(void);
void scene_view_draw_world(const struct scene_stage *stage);
void scene_view_draw_covers(const struct scene_stage *stage);
int scene_view_snapshot_stage(const struct scene_stage *stage);

#endif
