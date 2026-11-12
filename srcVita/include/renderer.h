#ifndef RENDERER_H
#define RENDERER_H

#define RENDERER_WIDTH 960
#define RENDERER_HEIGHT 544
#define RENDERER_NO_TEXTURE 0

typedef unsigned int renderer_texture;

enum renderer_compressed_format {
    RENDERER_COMPRESSED_DXT5,
    RENDERER_COMPRESSED_DXT1
};

struct renderer_rect {
    float x;
    float y;
    float width;
    float height;
};

struct renderer_color {
    float red;
    float green;
    float blue;
    float alpha;
};

int renderer_init(void);
void renderer_shutdown(void);
void renderer_begin_frame(struct renderer_color clear_color);
void renderer_end_frame(void);
renderer_texture renderer_texture_create(const unsigned char *rgba_pixels, int width, int height);
renderer_texture renderer_texture_create_compressed(const unsigned char *blocks, unsigned int block_bytes, int width, int height, enum renderer_compressed_format format);
void renderer_texture_destroy(renderer_texture texture);
unsigned int renderer_live_texture_count(void);
void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint);
void renderer_draw_beam(renderer_texture texture, float from_x, float from_y, float to_x, float to_y, float half_width, struct renderer_color tint);
void renderer_draw_flat_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color);
struct renderer_vertex {
    float x;
    float y;
    float u;
    float v;
};

void renderer_draw_triangles(renderer_texture texture, const struct renderer_vertex *corners, int triangle_count, struct renderer_color tint);
void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color);
void renderer_draw_filled_rects(const struct renderer_rect *screen_areas, int count, struct renderer_color color);
int renderer_snapshot_begin(void);
void renderer_snapshot_end(void);
void renderer_draw_snapshot(struct renderer_rect screen_area, float alpha);
void renderer_set_vblanks_per_frame(int vblanks);
void renderer_raise_contrast(void);
int renderer_low_resolution_begin(void);
void renderer_low_resolution_end(void);
void renderer_draw_turned_snapshot(float center_x, float center_y, float half_width, float half_height, float degrees, float alpha);

#endif
