#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "smoke_renderer.h"

#define TEXTURE_SLOT_COUNT 16384u
#define TEXTURE_SLOT_BITS 14u
#define CORNERS_PER_SHAPE 3
#define CONTRAST_FLOOR 0.06f
#define CONTRAST_GAIN 0.3f
#define DEGREES_TO_RADIANS 0.017453292f
#define SCREEN_PIXELS (RENDERER_WIDTH * RENDERER_HEIGHT)
#define BMP_HEADER_BYTES 54u
#define BYTES_PER_BLOCK_DXT1 8u
#define BYTES_PER_BLOCK_DXT5 16u

enum texture_kind {
    KIND_RGBA,
    KIND_DXT5,
    KIND_DXT1
};

enum shape_kind {
    SHAPE_TRIANGLE,
    SHAPE_FLAT_TRIANGLE,
    SHAPE_SNAPSHOT_TRIANGLE,
    SHAPE_CONTRAST
};

struct texture_slot {
    renderer_texture id;
    enum texture_kind kind;
    int width;
    int height;
    int is_sharp;
    int is_filled;
    unsigned char *stored;
    unsigned int stored_bytes;
    unsigned char *pixels;
};

struct shape {
    enum shape_kind kind;
    renderer_texture texture;
    struct renderer_vertex corners[CORNERS_PER_SHAPE];
    struct renderer_color tint;
};

struct shape_list {
    struct shape *shapes;
    unsigned int count;
    unsigned int room;
};

static struct texture_slot slots[TEXTURE_SLOT_COUNT];
static unsigned int serial;
static unsigned int live_textures;
static struct shape_list frame_shapes;
static struct shape_list snapshot_shapes;
static int snapshot_is_open;
static float *snapshot_pixels;
static unsigned int dead_draws;
static unsigned int unfilled_draws;
static unsigned int frames_drawn;
static unsigned int shapes_in_last_frame;

static struct texture_slot *slot_of(renderer_texture texture)
{
    struct texture_slot *slot;

    if (texture == RENDERER_NO_TEXTURE) {
        return NULL;
    }
    slot = &slots[texture & (TEXTURE_SLOT_COUNT - 1u)];
    return slot->id == texture ? slot : NULL;
}

static struct texture_slot *new_slot(int width, int height, enum texture_kind kind, unsigned int stored_bytes)
{
    unsigned int index;

    for (index = 1; index < TEXTURE_SLOT_COUNT; index++) {
        if (slots[index].id == RENDERER_NO_TEXTURE) {
            break;
        }
    }
    if (index == TEXTURE_SLOT_COUNT) {
        return NULL;
    }
    slots[index].stored = calloc(1, stored_bytes > 0 ? stored_bytes : 1);
    if (slots[index].stored == NULL) {
        return NULL;
    }
    serial = serial + 1u;
    slots[index].id = ((serial << TEXTURE_SLOT_BITS) | index) & 0x7fffffffu;
    slots[index].kind = kind;
    slots[index].width = width;
    slots[index].height = height;
    slots[index].is_sharp = 0;
    slots[index].is_filled = 0;
    slots[index].stored_bytes = stored_bytes;
    slots[index].pixels = NULL;
    live_textures = live_textures + 1u;
    return &slots[index];
}

static unsigned int block_bytes_of(int width, int height, enum texture_kind kind)
{
    unsigned int blocks;

    blocks = (((unsigned int)width + 3u) / 4u) * (((unsigned int)height + 3u) / 4u);
    return blocks * (kind == KIND_DXT1 ? BYTES_PER_BLOCK_DXT1 : BYTES_PER_BLOCK_DXT5);
}

int renderer_init(void)
{
    return 1;
}

void renderer_shutdown(void)
{
}

renderer_texture renderer_texture_create(const unsigned char *rgba_pixels, int width, int height)
{
    struct texture_slot *slot;

    slot = new_slot(width, height, KIND_RGBA, (unsigned int)width * (unsigned int)height * 4u);
    if (slot == NULL) {
        return RENDERER_NO_TEXTURE;
    }
    memcpy(slot->stored, rgba_pixels, slot->stored_bytes);
    slot->is_filled = 1;
    return slot->id;
}

static enum texture_kind kind_of(enum renderer_compressed_format format)
{
    return format == RENDERER_COMPRESSED_DXT1 ? KIND_DXT1 : KIND_DXT5;
}

renderer_texture renderer_texture_create_compressed(const unsigned char *blocks, unsigned int block_bytes, int width, int height, enum renderer_compressed_format format)
{
    struct texture_slot *slot;

    slot = new_slot(width, height, kind_of(format), block_bytes_of(width, height, kind_of(format)));
    if (slot == NULL) {
        return RENDERER_NO_TEXTURE;
    }
    memcpy(slot->stored, blocks, block_bytes < slot->stored_bytes ? block_bytes : slot->stored_bytes);
    slot->is_filled = 1;
    return slot->id;
}

renderer_texture renderer_texture_create_compressed_empty(int width, int height, enum renderer_compressed_format format, void **blocks_out)
{
    struct texture_slot *slot;

    *blocks_out = NULL;
    slot = new_slot(width, height, kind_of(format), block_bytes_of(width, height, kind_of(format)));
    if (slot == NULL) {
        return RENDERER_NO_TEXTURE;
    }
    *blocks_out = slot;
    return slot->id;
}

void renderer_texture_fill_compressed(void *destination, const unsigned char *blocks, int width, int height, enum renderer_compressed_format format)
{
    struct texture_slot *slot;

    slot = destination;
    memcpy(slot->stored, blocks, block_bytes_of(width, height, kind_of(format)));
    slot->is_filled = 1;
    free(slot->pixels);
    slot->pixels = NULL;
}

void renderer_texture_destroy(renderer_texture texture)
{
    struct texture_slot *slot;

    slot = slot_of(texture);
    if (slot == NULL) {
        return;
    }
    free(slot->stored);
    free(slot->pixels);
    memset(slot, 0, sizeof(*slot));
    live_textures = live_textures - 1u;
}

void renderer_texture_set_sharp(renderer_texture texture)
{
    struct texture_slot *slot;

    slot = slot_of(texture);
    if (slot != NULL) {
        slot->is_sharp = 1;
    }
}

unsigned int renderer_live_texture_count(void)
{
    return live_textures;
}

static struct shape *next_shape(void)
{
    struct shape_list *list;
    struct shape *grown;

    list = snapshot_is_open ? &snapshot_shapes : &frame_shapes;
    if (list->count == list->room) {
        grown = realloc(list->shapes, (list->room == 0 ? 1024u : list->room * 2u) * sizeof(struct shape));
        if (grown == NULL) {
            return NULL;
        }
        list->shapes = grown;
        list->room = list->room == 0 ? 1024u : list->room * 2u;
    }
    list->count = list->count + 1u;
    return &list->shapes[list->count - 1u];
}

static void note_texture_use(renderer_texture texture)
{
    struct texture_slot *slot;

    if (texture == RENDERER_NO_TEXTURE) {
        return;
    }
    slot = slot_of(texture);
    if (slot == NULL) {
        dead_draws = dead_draws + 1u;
    } else if (!slot->is_filled) {
        unfilled_draws = unfilled_draws + 1u;
    }
}

static void add_triangle(enum shape_kind kind, renderer_texture texture, struct renderer_vertex first, struct renderer_vertex second, struct renderer_vertex third, struct renderer_color tint)
{
    struct shape *shape;

    shape = next_shape();
    if (shape == NULL) {
        return;
    }
    shape->kind = kind;
    shape->texture = texture;
    shape->corners[0] = first;
    shape->corners[1] = second;
    shape->corners[2] = third;
    shape->tint = tint;
}

static struct renderer_vertex corner(float x, float y, float u, float v)
{
    struct renderer_vertex vertex;

    vertex.x = x;
    vertex.y = y;
    vertex.u = u;
    vertex.v = v;
    return vertex;
}

static void add_quad(enum shape_kind kind, renderer_texture texture, struct renderer_vertex top_left, struct renderer_vertex top_right, struct renderer_vertex bottom_left, struct renderer_vertex bottom_right, struct renderer_color tint)
{
    add_triangle(kind, texture, top_left, top_right, bottom_left, tint);
    add_triangle(kind, texture, top_right, bottom_right, bottom_left, tint);
}

static void add_sprite(enum shape_kind kind, renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    float left;
    float right;
    float top;
    float bottom;
    float u_left;
    float u_right;
    float v_top;
    float v_bottom;

    left = screen_area.x;
    right = screen_area.x + screen_area.width;
    top = screen_area.y;
    bottom = screen_area.y + screen_area.height;
    u_left = texture_area.x;
    u_right = texture_area.x + texture_area.width;
    v_top = texture_area.y;
    v_bottom = texture_area.y + texture_area.height;
    add_quad(kind, texture, corner(left, top, u_left, v_top), corner(right, top, u_right, v_top), corner(left, bottom, u_left, v_bottom), corner(right, bottom, u_right, v_bottom), tint);
}

void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    note_texture_use(texture);
    add_sprite(SHAPE_TRIANGLE, texture, screen_area, texture_area, tint);
}

void renderer_draw_flat_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    note_texture_use(texture);
    add_sprite(SHAPE_FLAT_TRIANGLE, texture, screen_area, texture_area, color);
}

void renderer_draw_beam(renderer_texture texture, float from_x, float from_y, float to_x, float to_y, float half_width, struct renderer_color tint)
{
    float length;
    float side_x;
    float side_y;

    length = sqrtf((to_x - from_x) * (to_x - from_x) + (to_y - from_y) * (to_y - from_y));
    if (length < 1.0f) {
        return;
    }
    note_texture_use(texture);
    side_x = -(to_y - from_y) / length * half_width;
    side_y = (to_x - from_x) / length * half_width;
    add_quad(SHAPE_TRIANGLE, texture, corner(from_x - side_x, from_y - side_y, 0.0f, 0.0f), corner(to_x - side_x, to_y - side_y, 1.0f, 0.0f), corner(from_x + side_x, from_y + side_y, 0.0f, 1.0f), corner(to_x + side_x, to_y + side_y, 1.0f, 1.0f), tint);
}

void renderer_draw_triangles(renderer_texture texture, const struct renderer_vertex *corners, int triangle_count, struct renderer_color tint)
{
    int index;

    note_texture_use(texture);
    for (index = 0; index < triangle_count; index++) {
        add_triangle(SHAPE_TRIANGLE, texture, corners[index * 3], corners[index * 3 + 1], corners[index * 3 + 2], tint);
    }
}

void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color)
{
    struct renderer_rect whole;

    whole.x = 0.0f;
    whole.y = 0.0f;
    whole.width = 1.0f;
    whole.height = 1.0f;
    add_sprite(SHAPE_TRIANGLE, RENDERER_NO_TEXTURE, screen_area, whole, color);
}

void renderer_draw_filled_rects(const struct renderer_rect *screen_areas, int count, struct renderer_color color)
{
    int index;

    for (index = 0; index < count; index++) {
        renderer_draw_filled_rect(screen_areas[index], color);
    }
}

void renderer_draw_snapshot(struct renderer_rect screen_area, float alpha)
{
    struct renderer_rect whole;
    struct renderer_color tint;

    whole.x = 0.0f;
    whole.y = 0.0f;
    whole.width = 1.0f;
    whole.height = 1.0f;
    tint.red = 1.0f;
    tint.green = 1.0f;
    tint.blue = 1.0f;
    tint.alpha = alpha;
    add_sprite(SHAPE_SNAPSHOT_TRIANGLE, RENDERER_NO_TEXTURE, screen_area, whole, tint);
}

void renderer_draw_turned_snapshot(float center_x, float center_y, float half_width, float half_height, float degrees, float alpha)
{
    struct renderer_color tint;
    float radians;
    float across_x;
    float across_y;
    float down_x;
    float down_y;

    tint.red = 1.0f;
    tint.green = 1.0f;
    tint.blue = 1.0f;
    tint.alpha = alpha;
    radians = degrees * DEGREES_TO_RADIANS;
    across_x = cosf(radians) * half_width;
    across_y = sinf(radians) * half_width;
    down_x = -sinf(radians) * half_height;
    down_y = cosf(radians) * half_height;
    add_quad(SHAPE_SNAPSHOT_TRIANGLE, RENDERER_NO_TEXTURE, corner(center_x - across_x - down_x, center_y - across_y - down_y, 0.0f, 0.0f), corner(center_x + across_x - down_x, center_y + across_y - down_y, 1.0f, 0.0f), corner(center_x - across_x + down_x, center_y - across_y + down_y, 0.0f, 1.0f), corner(center_x + across_x + down_x, center_y + across_y + down_y, 1.0f, 1.0f), tint);
}

void renderer_raise_contrast_in(struct renderer_rect screen_area)
{
    struct shape *shape;

    shape = next_shape();
    if (shape == NULL) {
        return;
    }
    memset(shape, 0, sizeof(*shape));
    shape->kind = SHAPE_CONTRAST;
    shape->corners[0] = corner(screen_area.x, screen_area.y, 0.0f, 0.0f);
    shape->corners[1] = corner(screen_area.x + screen_area.width, screen_area.y + screen_area.height, 0.0f, 0.0f);
}

void renderer_raise_contrast(void)
{
    struct renderer_rect screen;

    screen.x = 0.0f;
    screen.y = 0.0f;
    screen.width = (float)RENDERER_WIDTH;
    screen.height = (float)RENDERER_HEIGHT;
    renderer_raise_contrast_in(screen);
}

void renderer_set_vblanks_per_frame(int vblanks)
{
    (void)vblanks;
}

void renderer_set_vsync(int is_on)
{
    (void)is_on;
}

static void expand_565(unsigned int color, int *rgb)
{
    rgb[0] = (int)((color >> 11) & 31u) * 255 / 31;
    rgb[1] = (int)((color >> 5) & 63u) * 255 / 63;
    rgb[2] = (int)(color & 31u) * 255 / 31;
}

static void decode_color_block(const unsigned char *block, int always_four, unsigned char out[16][4])
{
    unsigned int color0;
    unsigned int color1;
    unsigned int bits;
    int table[4][4];
    int channel;
    int pixel;

    color0 = (unsigned int)block[0] | ((unsigned int)block[1] << 8);
    color1 = (unsigned int)block[2] | ((unsigned int)block[3] << 8);
    bits = (unsigned int)block[4] | ((unsigned int)block[5] << 8) | ((unsigned int)block[6] << 16) | ((unsigned int)block[7] << 24);
    expand_565(color0, table[0]);
    expand_565(color1, table[1]);
    table[0][3] = 255;
    table[1][3] = 255;
    table[2][3] = 255;
    table[3][3] = 255;
    for (channel = 0; channel < 3; channel++) {
        if (always_four || color0 > color1) {
            table[2][channel] = (2 * table[0][channel] + table[1][channel]) / 3;
            table[3][channel] = (table[0][channel] + 2 * table[1][channel]) / 3;
        } else {
            table[2][channel] = (table[0][channel] + table[1][channel]) / 2;
            table[3][channel] = 0;
        }
    }
    if (!always_four && color0 <= color1) {
        table[3][3] = 0;
    }
    for (pixel = 0; pixel < 16; pixel++) {
        const int *entry;

        entry = table[(bits >> (2 * pixel)) & 3u];
        out[pixel][0] = (unsigned char)entry[0];
        out[pixel][1] = (unsigned char)entry[1];
        out[pixel][2] = (unsigned char)entry[2];
        out[pixel][3] = (unsigned char)entry[3];
    }
}

static void decode_alpha_block(const unsigned char *block, unsigned char out[16][4])
{
    unsigned long long bits;
    int table[8];
    int step;
    int pixel;

    table[0] = block[0];
    table[1] = block[1];
    for (step = 2; step < 8; step++) {
        if (table[0] > table[1]) {
            table[step] = ((8 - step) * table[0] + (step - 1) * table[1]) / 7;
        } else if (step < 6) {
            table[step] = ((6 - step) * table[0] + (step - 1) * table[1]) / 5;
        } else {
            table[step] = step == 6 ? 0 : 255;
        }
    }
    bits = 0;
    for (step = 0; step < 6; step++) {
        bits = bits | ((unsigned long long)block[2 + step] << (8 * step));
    }
    for (pixel = 0; pixel < 16; pixel++) {
        out[pixel][3] = (unsigned char)table[(bits >> (3 * pixel)) & 7u];
    }
}

static const unsigned char *pixels_of(struct texture_slot *slot)
{
    unsigned char block_pixels[16][4];
    const unsigned char *block;
    int block_columns;
    int block_rows;
    int block_x;
    int block_y;
    int pixel;

    if (slot->kind == KIND_RGBA) {
        return slot->stored;
    }
    if (slot->pixels != NULL) {
        return slot->pixels;
    }
    slot->pixels = calloc((size_t)slot->width * (size_t)slot->height, 4);
    if (slot->pixels == NULL) {
        return NULL;
    }
    block_columns = (slot->width + 3) / 4;
    block_rows = (slot->height + 3) / 4;
    block = slot->stored;
    for (block_y = 0; block_y < block_rows; block_y++) {
        for (block_x = 0; block_x < block_columns; block_x++) {
            if (slot->kind == KIND_DXT5) {
                decode_color_block(block + 8, 1, block_pixels);
                decode_alpha_block(block, block_pixels);
                block = block + BYTES_PER_BLOCK_DXT5;
            } else {
                decode_color_block(block, 0, block_pixels);
                block = block + BYTES_PER_BLOCK_DXT1;
            }
            for (pixel = 0; pixel < 16; pixel++) {
                int x;
                int y;

                x = block_x * 4 + pixel % 4;
                y = block_y * 4 + pixel / 4;
                if (x < slot->width && y < slot->height) {
                    memcpy(&slot->pixels[((size_t)y * (size_t)slot->width + (size_t)x) * 4u], block_pixels[pixel], 4);
                }
            }
        }
    }
    return slot->pixels;
}

static void read_texel(const unsigned char *pixels, int width, int height, int x, int y, float *rgba)
{
    const unsigned char *texel;

    x = x < 0 ? 0 : (x >= width ? width - 1 : x);
    y = y < 0 ? 0 : (y >= height ? height - 1 : y);
    texel = &pixels[((size_t)y * (size_t)width + (size_t)x) * 4u];
    rgba[0] = (float)texel[0] / 255.0f;
    rgba[1] = (float)texel[1] / 255.0f;
    rgba[2] = (float)texel[2] / 255.0f;
    rgba[3] = (float)texel[3] / 255.0f;
}

static void sample_texture(struct texture_slot *slot, float u, float v, float *rgba)
{
    const unsigned char *pixels;
    float corners[4][4];
    float x;
    float y;
    float share_x;
    float share_y;
    int left;
    int top;
    int channel;

    pixels = pixels_of(slot);
    if (pixels == NULL) {
        rgba[0] = 1.0f;
        rgba[1] = 0.0f;
        rgba[2] = 1.0f;
        rgba[3] = 1.0f;
        return;
    }
    x = u * (float)slot->width;
    y = v * (float)slot->height;
    if (slot->is_sharp) {
        read_texel(pixels, slot->width, slot->height, (int)floorf(x), (int)floorf(y), rgba);
        return;
    }
    x = x - 0.5f;
    y = y - 0.5f;
    left = (int)floorf(x);
    top = (int)floorf(y);
    share_x = x - (float)left;
    share_y = y - (float)top;
    read_texel(pixels, slot->width, slot->height, left, top, corners[0]);
    read_texel(pixels, slot->width, slot->height, left + 1, top, corners[1]);
    read_texel(pixels, slot->width, slot->height, left, top + 1, corners[2]);
    read_texel(pixels, slot->width, slot->height, left + 1, top + 1, corners[3]);
    for (channel = 0; channel < 4; channel++) {
        rgba[channel] = (corners[0][channel] * (1.0f - share_x) + corners[1][channel] * share_x) * (1.0f - share_y) + (corners[2][channel] * (1.0f - share_x) + corners[3][channel] * share_x) * share_y;
    }
}

static void sample_snapshot(float u, float v, float *rgba)
{
    int x;
    int y;

    rgba[3] = 1.0f;
    if (snapshot_pixels == NULL) {
        rgba[0] = 0.0f;
        rgba[1] = 0.0f;
        rgba[2] = 0.0f;
        return;
    }
    x = (int)(u * (float)RENDERER_WIDTH);
    y = (int)(v * (float)RENDERER_HEIGHT);
    x = x < 0 ? 0 : (x >= RENDERER_WIDTH ? RENDERER_WIDTH - 1 : x);
    y = y < 0 ? 0 : (y >= RENDERER_HEIGHT ? RENDERER_HEIGHT - 1 : y);
    memcpy(rgba, &snapshot_pixels[(y * RENDERER_WIDTH + x) * 3], 3 * sizeof(float));
}

static float edge(const struct renderer_vertex *from, const struct renderer_vertex *to, float x, float y)
{
    return (to->x - from->x) * (y - from->y) - (to->y - from->y) * (x - from->x);
}

static void fill_triangle(float *canvas, const struct shape *shape)
{
    const struct renderer_vertex *corners;
    struct texture_slot *slot;
    float area;
    float least_x;
    float most_x;
    float least_y;
    float most_y;
    int x;
    int y;
    int index;

    corners = shape->corners;
    area = edge(&corners[0], &corners[1], corners[2].x, corners[2].y);
    if (area == 0.0f) {
        return;
    }
    slot = shape->kind == SHAPE_SNAPSHOT_TRIANGLE ? NULL : slot_of(shape->texture);
    least_x = most_x = corners[0].x;
    least_y = most_y = corners[0].y;
    for (index = 1; index < CORNERS_PER_SHAPE; index++) {
        least_x = corners[index].x < least_x ? corners[index].x : least_x;
        most_x = corners[index].x > most_x ? corners[index].x : most_x;
        least_y = corners[index].y < least_y ? corners[index].y : least_y;
        most_y = corners[index].y > most_y ? corners[index].y : most_y;
    }
    least_x = least_x < 0.0f ? 0.0f : least_x;
    least_y = least_y < 0.0f ? 0.0f : least_y;
    most_x = most_x > (float)RENDERER_WIDTH ? (float)RENDERER_WIDTH : most_x;
    most_y = most_y > (float)RENDERER_HEIGHT ? (float)RENDERER_HEIGHT : most_y;
    for (y = (int)floorf(least_y); y < (int)ceilf(most_y); y++) {
        for (x = (int)floorf(least_x); x < (int)ceilf(most_x); x++) {
            float weights[CORNERS_PER_SHAPE];
            float texel[4];
            float source[4];
            float centre_x;
            float centre_y;
            float u;
            float v;
            float *pixel;

            centre_x = (float)x + 0.5f;
            centre_y = (float)y + 0.5f;
            weights[0] = edge(&corners[1], &corners[2], centre_x, centre_y) / area;
            weights[1] = edge(&corners[2], &corners[0], centre_x, centre_y) / area;
            weights[2] = edge(&corners[0], &corners[1], centre_x, centre_y) / area;
            if (weights[0] < 0.0f || weights[1] < 0.0f || weights[2] < 0.0f) {
                continue;
            }
            u = weights[0] * corners[0].u + weights[1] * corners[1].u + weights[2] * corners[2].u;
            v = weights[0] * corners[0].v + weights[1] * corners[1].v + weights[2] * corners[2].v;
            if (shape->kind == SHAPE_SNAPSHOT_TRIANGLE) {
                sample_snapshot(u, v, texel);
            } else if (slot != NULL) {
                sample_texture(slot, u, v, texel);
            } else if (shape->texture == RENDERER_NO_TEXTURE) {
                texel[0] = texel[1] = texel[2] = texel[3] = 1.0f;
            } else {
                texel[0] = 1.0f;
                texel[1] = 0.0f;
                texel[2] = 1.0f;
                texel[3] = 1.0f;
            }
            if (shape->kind == SHAPE_FLAT_TRIANGLE) {
                source[0] = shape->tint.red;
                source[1] = shape->tint.green;
                source[2] = shape->tint.blue;
            } else {
                source[0] = texel[0] * shape->tint.red;
                source[1] = texel[1] * shape->tint.green;
                source[2] = texel[2] * shape->tint.blue;
            }
            source[3] = texel[3] * shape->tint.alpha;
            source[3] = source[3] < 0.0f ? 0.0f : (source[3] > 1.0f ? 1.0f : source[3]);
            pixel = &canvas[(y * RENDERER_WIDTH + x) * 3];
            pixel[0] = pixel[0] * (1.0f - source[3]) + source[0] * source[3];
            pixel[1] = pixel[1] * (1.0f - source[3]) + source[1] * source[3];
            pixel[2] = pixel[2] * (1.0f - source[3]) + source[2] * source[3];
        }
    }
}

static void raise_contrast_on(float *canvas, const struct shape *shape)
{
    int left;
    int top;
    int right;
    int bottom;
    int x;
    int y;
    int channel;

    left = (int)shape->corners[0].x;
    top = (int)shape->corners[0].y;
    right = (int)shape->corners[1].x;
    bottom = (int)shape->corners[1].y;
    left = left < 0 ? 0 : left;
    top = top < 0 ? 0 : top;
    right = right > RENDERER_WIDTH ? RENDERER_WIDTH : right;
    bottom = bottom > RENDERER_HEIGHT ? RENDERER_HEIGHT : bottom;
    for (y = top; y < bottom; y++) {
        for (x = left; x < right; x++) {
            for (channel = 0; channel < 3; channel++) {
                float value;

                value = canvas[(y * RENDERER_WIDTH + x) * 3 + channel] - CONTRAST_FLOOR;
                value = value < 0.0f ? 0.0f : value;
                value = value * (1.0f + CONTRAST_GAIN);
                canvas[(y * RENDERER_WIDTH + x) * 3 + channel] = value > 1.0f ? 1.0f : value;
            }
        }
    }
}

static void paint(float *canvas, const struct shape_list *list)
{
    unsigned int index;

    memset(canvas, 0, SCREEN_PIXELS * 3 * sizeof(float));
    for (index = 0; index < list->count; index++) {
        if (list->shapes[index].kind == SHAPE_CONTRAST) {
            raise_contrast_on(canvas, &list->shapes[index]);
        } else {
            fill_triangle(canvas, &list->shapes[index]);
        }
    }
}

int renderer_snapshot_begin(void)
{
    snapshot_is_open = 1;
    snapshot_shapes.count = 0;
    return 1;
}

void renderer_snapshot_end(void)
{
    snapshot_is_open = 0;
    if (snapshot_pixels == NULL) {
        snapshot_pixels = malloc(SCREEN_PIXELS * 3 * sizeof(float));
    }
    if (snapshot_pixels != NULL) {
        paint(snapshot_pixels, &snapshot_shapes);
    }
}

void renderer_begin_frame(struct renderer_color clear_color)
{
    (void)clear_color;
    frame_shapes.count = 0;
}

void renderer_end_frame(void)
{
    frames_drawn = frames_drawn + 1u;
    shapes_in_last_frame = frame_shapes.count;
}

static void write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)value;
    bytes[1] = (unsigned char)(value >> 8);
    bytes[2] = (unsigned char)(value >> 16);
    bytes[3] = (unsigned char)(value >> 24);
}

int smoke_renderer_save_bmp(const char *disk_path)
{
    unsigned char header[BMP_HEADER_BYTES];
    unsigned char *row;
    float *canvas;
    FILE *output;
    int x;
    int y;

    canvas = malloc(SCREEN_PIXELS * 3 * sizeof(float));
    row = malloc(RENDERER_WIDTH * 3);
    output = fopen(disk_path, "wb");
    if (canvas == NULL || row == NULL || output == NULL) {
        free(canvas);
        free(row);
        if (output != NULL) {
            fclose(output);
        }
        return 0;
    }
    paint(canvas, &frame_shapes);
    memset(header, 0, sizeof(header));
    header[0] = 'B';
    header[1] = 'M';
    write_u32(&header[2], BMP_HEADER_BYTES + SCREEN_PIXELS * 3u);
    write_u32(&header[10], BMP_HEADER_BYTES);
    write_u32(&header[14], 40u);
    write_u32(&header[18], RENDERER_WIDTH);
    write_u32(&header[22], RENDERER_HEIGHT);
    header[26] = 1;
    header[28] = 24;
    write_u32(&header[34], SCREEN_PIXELS * 3u);
    fwrite(header, 1, sizeof(header), output);
    for (y = RENDERER_HEIGHT - 1; y >= 0; y--) {
        for (x = 0; x < RENDERER_WIDTH; x++) {
            const float *pixel;

            pixel = &canvas[(y * RENDERER_WIDTH + x) * 3];
            row[x * 3] = (unsigned char)(pixel[2] * 255.0f + 0.5f);
            row[x * 3 + 1] = (unsigned char)(pixel[1] * 255.0f + 0.5f);
            row[x * 3 + 2] = (unsigned char)(pixel[0] * 255.0f + 0.5f);
        }
        fwrite(row, 1, RENDERER_WIDTH * 3, output);
    }
    fclose(output);
    free(canvas);
    free(row);
    return 1;
}

unsigned int smoke_renderer_draws_of_dead_textures(void)
{
    return dead_draws;
}

unsigned int smoke_renderer_draws_of_unfilled_textures(void)
{
    return unfilled_draws;
}

unsigned int smoke_renderer_frames_drawn(void)
{
    return frames_drawn;
}

unsigned int smoke_renderer_shapes_in_last_frame(void)
{
    return shapes_in_last_frame;
}
