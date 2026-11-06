#include <string.h>
#include "fake_renderer.h"

#define FAKE_TEXTURE_MAX 2048

static struct fake_draw draws[FAKE_DRAW_MAX];
static int draw_count;
static struct renderer_rect batched_rects[FAKE_BATCHED_RECT_MAX];
static int batched_rect_count;
static int texture_widths[FAKE_TEXTURE_MAX];
static int texture_is_alive[FAKE_TEXTURE_MAX];
static int textures_created;
static int uploads_should_fail;

void fake_renderer_reset(void)
{
    memset(texture_widths, 0, sizeof(texture_widths));
    memset(texture_is_alive, 0, sizeof(texture_is_alive));
    textures_created = 0;
    draw_count = 0;
    batched_rect_count = 0;
    uploads_should_fail = 0;
}

void fake_renderer_clear_draws(void)
{
    draw_count = 0;
    batched_rect_count = 0;
}

int fake_renderer_draw_count(void)
{
    return draw_count;
}

const struct fake_draw *fake_renderer_draw(int index)
{
    return &draws[index];
}

const struct renderer_rect *fake_renderer_batched_rects(const struct fake_draw *draw)
{
    return &batched_rects[draw->first_batched_rect];
}

int fake_renderer_textures_created(void)
{
    return textures_created;
}

int fake_renderer_textures_alive(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < FAKE_TEXTURE_MAX; index++) {
        if (texture_is_alive[index]) {
            count = count + 1;
        }
    }
    return count;
}

int fake_renderer_texture_width(renderer_texture texture)
{
    return texture_widths[texture];
}

void fake_renderer_fail_uploads(int should_fail)
{
    uploads_should_fail = should_fail;
}

int renderer_init(void)
{
    return 1;
}

void renderer_shutdown(void)
{
}

void renderer_begin_frame(struct renderer_color clear_color)
{
    (void)clear_color;
    draw_count = 0;
    batched_rect_count = 0;
}

void renderer_end_frame(void)
{
}

renderer_texture renderer_texture_create(const unsigned char *rgba_pixels, int width, int height)
{
    (void)rgba_pixels;
    (void)height;
    if (uploads_should_fail) {
        return RENDERER_NO_TEXTURE;
    }
    textures_created = textures_created + 1;
    texture_widths[textures_created] = width;
    texture_is_alive[textures_created] = 1;
    return (renderer_texture)textures_created;
}

renderer_texture renderer_texture_create_compressed(const unsigned char *blocks, unsigned int block_bytes, int width, int height, enum renderer_compressed_format format)
{
    (void)block_bytes;
    (void)format;
    return renderer_texture_create(blocks, width, height);
}

void renderer_texture_destroy(renderer_texture texture)
{
    texture_is_alive[texture] = 0;
}

static void record_draw(renderer_texture texture, int is_filled_rect, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    if (draw_count >= FAKE_DRAW_MAX) {
        return;
    }
    draws[draw_count].texture = texture;
    draws[draw_count].is_filled_rect = is_filled_rect;
    draws[draw_count].is_rect_batch = 0;
    draws[draw_count].batched_rect_count = 0;
    draws[draw_count].first_batched_rect = 0;
    draws[draw_count].screen_area = screen_area;
    draws[draw_count].texture_area = texture_area;
    draws[draw_count].color = color;
    draw_count = draw_count + 1;
}

void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    record_draw(texture, 0, screen_area, texture_area, tint);
}

void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color)
{
    struct renderer_rect no_texture_area;

    memset(&no_texture_area, 0, sizeof(no_texture_area));
    record_draw(RENDERER_NO_TEXTURE, 1, screen_area, no_texture_area, color);
}

void renderer_draw_filled_rects(const struct renderer_rect *screen_areas, int count, struct renderer_color color)
{
    struct renderer_rect no_area;
    int index;

    memset(&no_area, 0, sizeof(no_area));
    record_draw(RENDERER_NO_TEXTURE, 0, no_area, no_area, color);
    if (batched_rect_count + count > FAKE_BATCHED_RECT_MAX) {
        return;
    }
    draws[draw_count - 1].is_rect_batch = 1;
    draws[draw_count - 1].batched_rect_count = count;
    draws[draw_count - 1].first_batched_rect = batched_rect_count;
    for (index = 0; index < count; index++) {
        batched_rects[batched_rect_count] = screen_areas[index];
        batched_rect_count = batched_rect_count + 1;
    }
}
