#ifndef RENDERER_H
#define RENDERER_H

#define RENDERER_WIDTH 960
#define RENDERER_HEIGHT 544
#define RENDERER_NO_TEXTURE 0

typedef unsigned int renderer_texture;

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
void renderer_texture_destroy(renderer_texture texture);
void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint);
void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color);

#endif
