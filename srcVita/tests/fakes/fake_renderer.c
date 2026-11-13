#include <string.h>
#include "fake_renderer.h"

#define FAKE_TEXTURE_MAX 8192

static struct fake_draw draws[FAKE_DRAW_MAX];
static int draw_count;
static struct fake_draw snapshot_draws[FAKE_DRAW_MAX];
static int snapshot_draw_count;
static int snapshot_is_open;
static int snapshots_taken;
static int snapshots_should_fail;
static struct renderer_rect batched_rects[FAKE_BATCHED_RECT_MAX];
static int batched_rect_count;
static int texture_widths[FAKE_TEXTURE_MAX];
static int texture_is_alive[FAKE_TEXTURE_MAX];
static int textures_created;
static int uploads_should_fail;
static int vblanks_per_frame = 1;

static struct fake_draw *record_draw(renderer_texture texture, int is_filled_rect, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color);

void fake_renderer_reset(void)
{
    vblanks_per_frame = 1;
    memset(texture_widths, 0, sizeof(texture_widths));
    memset(texture_is_alive, 0, sizeof(texture_is_alive));
    textures_created = 0;
    draw_count = 0;
    batched_rect_count = 0;
    uploads_should_fail = 0;
    snapshot_draw_count = 0;
    snapshot_is_open = 0;
    snapshots_taken = 0;
    snapshots_should_fail = 0;
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

void fake_renderer_fail_snapshots(int should_fail)
{
    snapshots_should_fail = should_fail;
}

int fake_renderer_snapshots_taken(void)
{
    return snapshots_taken;
}

int fake_renderer_snapshot_is_open(void)
{
    return snapshot_is_open;
}

int fake_renderer_snapshot_draw_count(void)
{
    return snapshot_draw_count;
}

const struct fake_draw *fake_renderer_snapshot_draw(int index)
{
    return &snapshot_draws[index];
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

void renderer_set_vblanks_per_frame(int vblanks)
{
    vblanks_per_frame = vblanks;
}

void renderer_raise_contrast(void)
{
}

void renderer_raise_contrast_in(struct renderer_rect screen_area)
{
    struct fake_draw *draw;
    struct renderer_rect no_area;
    struct renderer_color no_color;

    memset(&no_area, 0, sizeof(no_area));
    memset(&no_color, 0, sizeof(no_color));
    draw = record_draw(RENDERER_NO_TEXTURE, 0, screen_area, no_area, no_color);
    if (draw != NULL) {
        draw->is_contrast = 1;
    }
}

int fake_renderer_vblanks_per_frame(void)
{
    return vblanks_per_frame;
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

unsigned int renderer_live_texture_count(void)
{
    return (unsigned int)fake_renderer_textures_alive();
}

static struct fake_draw *next_free_draw(void)
{
    if (snapshot_is_open) {
        if (snapshot_draw_count >= FAKE_DRAW_MAX) {
            return NULL;
        }
        snapshot_draw_count = snapshot_draw_count + 1;
        return &snapshot_draws[snapshot_draw_count - 1];
    }
    if (draw_count >= FAKE_DRAW_MAX) {
        return NULL;
    }
    draw_count = draw_count + 1;
    return &draws[draw_count - 1];
}

static struct fake_draw *record_draw(renderer_texture texture, int is_filled_rect, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    struct fake_draw *draw;

    draw = next_free_draw();
    if (draw == NULL) {
        return NULL;
    }
    draw->texture = texture;
    draw->is_filled_rect = is_filled_rect;
    draw->is_rect_batch = 0;
    draw->batched_rect_count = 0;
    draw->first_batched_rect = 0;
    draw->screen_area = screen_area;
    draw->texture_area = texture_area;
    draw->color = color;
    draw->is_snapshot = 0;
    draw->is_contrast = 0;
    return draw;
}

void renderer_draw_beam(renderer_texture texture, float from_x, float from_y, float to_x, float to_y, float half_width, struct renderer_color tint)
{
    struct renderer_rect screen_area;
    struct renderer_rect texture_area;

    screen_area.x = from_x;
    screen_area.y = from_y - half_width;
    screen_area.width = to_x - from_x;
    screen_area.height = (to_y - from_y) + 2.0f * half_width;
    texture_area.x = 0.0f;
    texture_area.y = 0.0f;
    texture_area.width = 1.0f;
    texture_area.height = 1.0f;
    renderer_draw_sprite(texture, screen_area, texture_area, tint);
}

void renderer_draw_flat_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    renderer_draw_sprite(texture, screen_area, texture_area, color);
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
    struct fake_draw *draw;
    struct renderer_rect no_area;
    int index;

    memset(&no_area, 0, sizeof(no_area));
    draw = record_draw(RENDERER_NO_TEXTURE, 0, no_area, no_area, color);
    if (draw == NULL || batched_rect_count + count > FAKE_BATCHED_RECT_MAX) {
        return;
    }
    draw->is_rect_batch = 1;
    draw->batched_rect_count = count;
    draw->first_batched_rect = batched_rect_count;
    for (index = 0; index < count; index++) {
        batched_rects[batched_rect_count] = screen_areas[index];
        batched_rect_count = batched_rect_count + 1;
    }
}

int renderer_snapshot_begin(void)
{
    if (snapshots_should_fail) {
        return 0;
    }
    snapshots_taken = snapshots_taken + 1;
    snapshot_is_open = 1;
    snapshot_draw_count = 0;
    return 1;
}

void renderer_snapshot_end(void)
{
    snapshot_is_open = 0;
}

void renderer_draw_turned_snapshot(float center_x, float center_y, float half_width, float half_height, float degrees, float alpha)
{
    struct renderer_rect area;

    (void)degrees;
    area.x = center_x - half_width;
    area.y = center_y - half_height;
    area.width = half_width * 2.0f;
    area.height = half_height * 2.0f;
    renderer_draw_snapshot(area, alpha);
}

void renderer_draw_triangles(renderer_texture texture, const struct renderer_vertex *corners, int triangle_count, struct renderer_color tint)
{
    struct renderer_rect no_area;

    (void)corners;
    (void)triangle_count;
    memset(&no_area, 0, sizeof(no_area));
    (void)record_draw(texture, 0, no_area, no_area, tint);
}

void renderer_draw_snapshot(struct renderer_rect screen_area, float alpha)
{
    struct fake_draw *draw;
    struct renderer_rect no_area;
    struct renderer_color tint;

    memset(&no_area, 0, sizeof(no_area));
    tint.red = 1.0f;
    tint.green = 1.0f;
    tint.blue = 1.0f;
    tint.alpha = alpha;
    draw = record_draw(RENDERER_NO_TEXTURE, 0, screen_area, no_area, tint);
    if (draw != NULL) {
        draw->is_snapshot = 1;
    }
}
