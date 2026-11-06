#ifndef FAKE_RENDERER_H
#define FAKE_RENDERER_H

#include "renderer.h"

#define FAKE_DRAW_MAX 4096
#define FAKE_BATCHED_RECT_MAX 16384

struct fake_draw {
    renderer_texture texture;
    int is_filled_rect;
    int is_rect_batch;
    int batched_rect_count;
    int first_batched_rect;
    struct renderer_rect screen_area;
    struct renderer_rect texture_area;
    struct renderer_color color;
    int is_snapshot;
};

void fake_renderer_reset(void);
void fake_renderer_clear_draws(void);
int fake_renderer_draw_count(void);
const struct fake_draw *fake_renderer_draw(int index);
const struct renderer_rect *fake_renderer_batched_rects(const struct fake_draw *draw);
int fake_renderer_textures_alive(void);
int fake_renderer_textures_created(void);
int fake_renderer_texture_width(renderer_texture texture);
void fake_renderer_fail_uploads(int should_fail);
void fake_renderer_fail_snapshots(int should_fail);
int fake_renderer_snapshots_taken(void);
int fake_renderer_snapshot_is_open(void);
int fake_renderer_snapshot_draw_count(void);
const struct fake_draw *fake_renderer_snapshot_draw(int index);

#endif
