#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "host_renderer.h"
#include "renderer.h"

#define TEXTURE_MAX 4096
#define CHANNELS 4
#define DXT1_BLOCK_BYTES 8
#define DXT5_BLOCK_BYTES 16
#define CONTRAST_FLOOR 0.06f
#define CONTRAST_GAIN 0.3f
#define DEGREES_TO_RADIANS 0.01745329252f
#define SNAPSHOT_TEXTURE (TEXTURE_MAX - 1)

struct host_texture {
    int in_use;
    int width;
    int height;
    int is_sharp;
    int has_format;
    enum renderer_compressed_format format;
    unsigned char *blocks;
    unsigned char *pixels;
};

static struct host_texture textures[TEXTURE_MAX];
static float screen[RENDERER_WIDTH * RENDERER_HEIGHT * CHANNELS];
static float snapshot[RENDERER_WIDTH * RENDERER_HEIGHT * CHANNELS];
static float *target = screen;
static unsigned int live_textures;

static unsigned int block_bytes_of(enum renderer_compressed_format format)
{
    return format == RENDERER_COMPRESSED_DXT1 ? DXT1_BLOCK_BYTES : DXT5_BLOCK_BYTES;
}

static unsigned int blocks_size(int width, int height, enum renderer_compressed_format format)
{
    return (unsigned int)((width + 3) / 4) * (unsigned int)((height + 3) / 4) * block_bytes_of(format);
}

static void expand_565(unsigned int color, int *rgb)
{
    rgb[0] = (int)((color >> 11) & 31u) * 255 / 31;
    rgb[1] = (int)((color >> 5) & 63u) * 255 / 63;
    rgb[2] = (int)(color & 31u) * 255 / 31;
}

static void decode_color_block(const unsigned char *block, int always_four, unsigned char colors[16][CHANNELS])
{
    unsigned int color0;
    unsigned int color1;
    unsigned int bits;
    int table[4][CHANNELS];
    int channel;
    int pixel;

    color0 = block[0] | ((unsigned int)block[1] << 8);
    color1 = block[2] | ((unsigned int)block[3] << 8);
    expand_565(color0, table[0]);
    expand_565(color1, table[1]);
    for (channel = 0; channel < 3; channel++) {
        if (always_four || color0 > color1) {
            table[2][channel] = (2 * table[0][channel] + table[1][channel]) / 3;
            table[3][channel] = (table[0][channel] + 2 * table[1][channel]) / 3;
        } else {
            table[2][channel] = (table[0][channel] + table[1][channel]) / 2;
            table[3][channel] = 0;
        }
    }
    table[0][3] = 255;
    table[1][3] = 255;
    table[2][3] = 255;
    table[3][3] = always_four || color0 > color1 ? 255 : 0;
    bits = block[4] | ((unsigned int)block[5] << 8) | ((unsigned int)block[6] << 16) | ((unsigned int)block[7] << 24);
    for (pixel = 0; pixel < 16; pixel++) {
        for (channel = 0; channel < CHANNELS; channel++) {
            colors[pixel][channel] = (unsigned char)table[(bits >> (2 * pixel)) & 3u][channel];
        }
    }
}

static void decode_alpha_block(const unsigned char *block, unsigned char colors[16][CHANNELS])
{
    unsigned long long bits;
    int table[8];
    int index;

    table[0] = block[0];
    table[1] = block[1];
    for (index = 2; index < 8; index++) {
        if (table[0] > table[1]) {
            table[index] = ((8 - index) * table[0] + (index - 1) * table[1]) / 7;
        } else if (index < 6) {
            table[index] = ((6 - index) * table[0] + (index - 1) * table[1]) / 5;
        } else {
            table[index] = index == 6 ? 0 : 255;
        }
    }
    bits = 0;
    for (index = 0; index < 6; index++) {
        bits = bits | ((unsigned long long)block[2 + index] << (8 * index));
    }
    for (index = 0; index < 16; index++) {
        colors[index][3] = (unsigned char)table[(bits >> (3 * index)) & 7u];
    }
}

static void decode_blocks(struct host_texture *texture)
{
    unsigned int bytes;
    int block_x;
    int block_y;

    bytes = block_bytes_of(texture->format);
    for (block_y = 0; block_y < (texture->height + 3) / 4; block_y++) {
        for (block_x = 0; block_x < (texture->width + 3) / 4; block_x++) {
            const unsigned char *block;
            unsigned char colors[16][CHANNELS];
            int pixel;

            block = &texture->blocks[((unsigned int)block_y * (unsigned int)((texture->width + 3) / 4) + (unsigned int)block_x) * bytes];
            if (texture->format == RENDERER_COMPRESSED_DXT1) {
                decode_color_block(block, 0, colors);
            } else {
                decode_color_block(block + 8, 1, colors);
                decode_alpha_block(block, colors);
            }
            for (pixel = 0; pixel < 16; pixel++) {
                int x;
                int y;

                x = block_x * 4 + pixel % 4;
                y = block_y * 4 + pixel / 4;
                if (x < texture->width && y < texture->height) {
                    memcpy(&texture->pixels[(y * texture->width + x) * CHANNELS], colors[pixel], CHANNELS);
                }
            }
        }
    }
}

static renderer_texture new_texture(int width, int height)
{
    renderer_texture handle;

    if (width <= 0 || height <= 0) {
        return RENDERER_NO_TEXTURE;
    }
    for (handle = 1; handle < SNAPSHOT_TEXTURE; handle++) {
        if (!textures[handle].in_use) {
            memset(&textures[handle], 0, sizeof(textures[handle]));
            textures[handle].pixels = calloc((size_t)width * (size_t)height, CHANNELS);
            if (textures[handle].pixels == NULL) {
                return RENDERER_NO_TEXTURE;
            }
            textures[handle].in_use = 1;
            textures[handle].width = width;
            textures[handle].height = height;
            live_textures = live_textures + 1u;
            return handle;
        }
    }
    return RENDERER_NO_TEXTURE;
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
    int index;

    target = screen;
    for (index = 0; index < RENDERER_WIDTH * RENDERER_HEIGHT; index++) {
        screen[index * CHANNELS] = clear_color.red;
        screen[index * CHANNELS + 1] = clear_color.green;
        screen[index * CHANNELS + 2] = clear_color.blue;
        screen[index * CHANNELS + 3] = 1.0f;
    }
}

void renderer_end_frame(void)
{
}

void renderer_set_vblanks_per_frame(int vblanks)
{
    (void)vblanks;
}

void renderer_set_vsync(int is_on)
{
    (void)is_on;
}

renderer_texture renderer_texture_create(const unsigned char *rgba_pixels, int width, int height)
{
    renderer_texture handle;

    handle = new_texture(width, height);
    if (handle != RENDERER_NO_TEXTURE) {
        memcpy(textures[handle].pixels, rgba_pixels, (size_t)width * (size_t)height * CHANNELS);
    }
    return handle;
}

renderer_texture renderer_texture_create_compressed_empty(int width, int height, enum renderer_compressed_format format, void **blocks_out)
{
    renderer_texture handle;

    *blocks_out = NULL;
    handle = new_texture(width, height);
    if (handle == RENDERER_NO_TEXTURE) {
        return handle;
    }
    textures[handle].has_format = 1;
    textures[handle].format = format;
    textures[handle].blocks = calloc(blocks_size(width, height, format), 1);
    *blocks_out = textures[handle].blocks;
    return handle;
}

void renderer_texture_fill_compressed(void *destination, const unsigned char *blocks, int width, int height, enum renderer_compressed_format format)
{
    memcpy(destination, blocks, blocks_size(width, height, format));
}

renderer_texture renderer_texture_create_compressed(const unsigned char *blocks, unsigned int block_bytes, int width, int height, enum renderer_compressed_format format)
{
    renderer_texture handle;
    void *destination;

    (void)block_bytes;
    handle = renderer_texture_create_compressed_empty(width, height, format, &destination);
    if (handle != RENDERER_NO_TEXTURE) {
        renderer_texture_fill_compressed(destination, blocks, width, height, format);
    }
    return handle;
}

void renderer_texture_destroy(renderer_texture texture)
{
    if (texture == RENDERER_NO_TEXTURE || texture >= SNAPSHOT_TEXTURE || !textures[texture].in_use) {
        return;
    }
    free(textures[texture].pixels);
    free(textures[texture].blocks);
    textures[texture].in_use = 0;
    live_textures = live_textures - 1u;
}

void renderer_texture_set_sharp(renderer_texture texture)
{
    if (texture < TEXTURE_MAX) {
        textures[texture].is_sharp = 1;
    }
}

unsigned int renderer_live_texture_count(void)
{
    return live_textures;
}

static void texel(const struct host_texture *texture, int x, int y, float *rgba)
{
    const unsigned char *source;
    int channel;

    x = x < 0 ? 0 : (x >= texture->width ? texture->width - 1 : x);
    y = y < 0 ? 0 : (y >= texture->height ? texture->height - 1 : y);
    source = &texture->pixels[(y * texture->width + x) * CHANNELS];
    for (channel = 0; channel < CHANNELS; channel++) {
        rgba[channel] = (float)source[channel] / 255.0f;
    }
}

static void sample(renderer_texture handle, float u, float v, float *rgba)
{
    const struct host_texture *texture;
    float corners[4][CHANNELS];
    float x;
    float y;
    float share_x;
    float share_y;
    int channel;

    if (handle == RENDERER_NO_TEXTURE) {
        rgba[0] = rgba[1] = rgba[2] = rgba[3] = 1.0f;
        return;
    }
    if (handle == SNAPSHOT_TEXTURE) {
        int pixel_x;
        int pixel_y;

        pixel_x = (int)(u * RENDERER_WIDTH);
        pixel_y = (int)((1.0f - v) * RENDERER_HEIGHT);
        pixel_x = pixel_x < 0 ? 0 : (pixel_x >= RENDERER_WIDTH ? RENDERER_WIDTH - 1 : pixel_x);
        pixel_y = pixel_y < 0 ? 0 : (pixel_y >= RENDERER_HEIGHT ? RENDERER_HEIGHT - 1 : pixel_y);
        memcpy(rgba, &snapshot[(pixel_y * RENDERER_WIDTH + pixel_x) * CHANNELS], sizeof(float) * CHANNELS);
        rgba[3] = 1.0f;
        return;
    }
    texture = &textures[handle];
    if (!texture->in_use) {
        rgba[0] = rgba[1] = rgba[2] = rgba[3] = 0.0f;
        return;
    }
    x = u * (float)texture->width - 0.5f;
    y = v * (float)texture->height - 0.5f;
    if (texture->is_sharp) {
        texel(texture, (int)floorf(x + 0.5f), (int)floorf(y + 0.5f), rgba);
        return;
    }
    texel(texture, (int)floorf(x), (int)floorf(y), corners[0]);
    texel(texture, (int)floorf(x) + 1, (int)floorf(y), corners[1]);
    texel(texture, (int)floorf(x), (int)floorf(y) + 1, corners[2]);
    texel(texture, (int)floorf(x) + 1, (int)floorf(y) + 1, corners[3]);
    share_x = x - floorf(x);
    share_y = y - floorf(y);
    for (channel = 0; channel < CHANNELS; channel++) {
        rgba[channel] = (corners[0][channel] * (1.0f - share_x) + corners[1][channel] * share_x) * (1.0f - share_y) + (corners[2][channel] * (1.0f - share_x) + corners[3][channel] * share_x) * share_y;
    }
}

static void make_ready(renderer_texture handle)
{
    if (handle != RENDERER_NO_TEXTURE && handle < SNAPSHOT_TEXTURE && textures[handle].in_use && textures[handle].has_format) {
        decode_blocks(&textures[handle]);
    }
}

static void blend(int x, int y, const float *rgba)
{
    float *pixel;
    int channel;

    if (rgba[3] <= 0.0f) {
        return;
    }
    pixel = &target[(y * RENDERER_WIDTH + x) * CHANNELS];
    for (channel = 0; channel < 3; channel++) {
        pixel[channel] = rgba[channel] * rgba[3] + pixel[channel] * (1.0f - rgba[3]);
    }
}

static float edge(const struct renderer_vertex *from, const struct renderer_vertex *to, float x, float y)
{
    return (to->x - from->x) * (y - from->y) - (to->y - from->y) * (x - from->x);
}

static void fill_triangle(renderer_texture texture, const struct renderer_vertex *corners, struct renderer_color tint, int is_flat)
{
    float area;
    float least_x;
    float most_x;
    float least_y;
    float most_y;
    int x;
    int y;
    int index;

    area = edge(&corners[0], &corners[1], corners[2].x, corners[2].y);
    if (area == 0.0f) {
        return;
    }
    least_x = most_x = corners[0].x;
    least_y = most_y = corners[0].y;
    for (index = 1; index < 3; index++) {
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
            float weights[3];
            float rgba[CHANNELS];
            float centre_x;
            float centre_y;

            centre_x = (float)x + 0.5f;
            centre_y = (float)y + 0.5f;
            weights[0] = edge(&corners[1], &corners[2], centre_x, centre_y) / area;
            weights[1] = edge(&corners[2], &corners[0], centre_x, centre_y) / area;
            weights[2] = edge(&corners[0], &corners[1], centre_x, centre_y) / area;
            if (weights[0] < 0.0f || weights[1] < 0.0f || weights[2] < 0.0f) {
                continue;
            }
            sample(texture, weights[0] * corners[0].u + weights[1] * corners[1].u + weights[2] * corners[2].u, weights[0] * corners[0].v + weights[1] * corners[1].v + weights[2] * corners[2].v, rgba);
            if (is_flat) {
                rgba[0] = tint.red;
                rgba[1] = tint.green;
                rgba[2] = tint.blue;
            } else {
                rgba[0] = rgba[0] * tint.red;
                rgba[1] = rgba[1] * tint.green;
                rgba[2] = rgba[2] * tint.blue;
            }
            rgba[3] = rgba[3] * tint.alpha;
            blend(x, y, rgba);
        }
    }
}

static void set_corner(struct renderer_vertex *corner, float x, float y, float u, float v)
{
    corner->x = x;
    corner->y = y;
    corner->u = u;
    corner->v = v;
}

static void fill_quad(renderer_texture texture, const struct renderer_vertex *quad, struct renderer_color tint, int is_flat)
{
    struct renderer_vertex second[3];

    make_ready(texture);
    fill_triangle(texture, quad, tint, is_flat);
    second[0] = quad[1];
    second[1] = quad[3];
    second[2] = quad[2];
    fill_triangle(texture, second, tint, is_flat);
}

static void fill_sprite(renderer_texture texture, struct renderer_rect area, struct renderer_rect part, struct renderer_color tint, int is_flat)
{
    struct renderer_vertex quad[4];

    set_corner(&quad[0], area.x, area.y, part.x, part.y);
    set_corner(&quad[1], area.x + area.width, area.y, part.x + part.width, part.y);
    set_corner(&quad[2], area.x, area.y + area.height, part.x, part.y + part.height);
    set_corner(&quad[3], area.x + area.width, area.y + area.height, part.x + part.width, part.y + part.height);
    fill_quad(texture, quad, tint, is_flat);
}

void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    fill_sprite(texture, screen_area, texture_area, tint, 0);
}

void renderer_draw_flat_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    fill_sprite(texture, screen_area, texture_area, color, 1);
}

void renderer_draw_beam(renderer_texture texture, float from_x, float from_y, float to_x, float to_y, float half_width, struct renderer_color tint)
{
    struct renderer_vertex quad[4];
    float length;
    float side_x;
    float side_y;

    length = sqrtf((to_x - from_x) * (to_x - from_x) + (to_y - from_y) * (to_y - from_y));
    if (length < 1.0f) {
        return;
    }
    side_x = -(to_y - from_y) / length * half_width;
    side_y = (to_x - from_x) / length * half_width;
    set_corner(&quad[0], from_x - side_x, from_y - side_y, 0.0f, 0.0f);
    set_corner(&quad[1], to_x - side_x, to_y - side_y, 1.0f, 0.0f);
    set_corner(&quad[2], from_x + side_x, from_y + side_y, 0.0f, 1.0f);
    set_corner(&quad[3], to_x + side_x, to_y + side_y, 1.0f, 1.0f);
    fill_quad(texture, quad, tint, 0);
}

void renderer_draw_triangles(renderer_texture texture, const struct renderer_vertex *corners, int triangle_count, struct renderer_color tint)
{
    int index;

    make_ready(texture);
    for (index = 0; index < triangle_count; index++) {
        fill_triangle(texture, &corners[index * 3], tint, 0);
    }
}

void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color)
{
    struct renderer_rect whole;

    whole.x = 0.0f;
    whole.y = 0.0f;
    whole.width = 1.0f;
    whole.height = 1.0f;
    fill_sprite(RENDERER_NO_TEXTURE, screen_area, whole, color, 0);
}

void renderer_draw_filled_rects(const struct renderer_rect *screen_areas, int count, struct renderer_color color)
{
    int index;

    for (index = 0; index < count; index++) {
        renderer_draw_filled_rect(screen_areas[index], color);
    }
}

int renderer_snapshot_begin(void)
{
    int index;

    target = snapshot;
    for (index = 0; index < RENDERER_WIDTH * RENDERER_HEIGHT * CHANNELS; index++) {
        snapshot[index] = index % CHANNELS == 3 ? 1.0f : 0.0f;
    }
    return 1;
}

void renderer_snapshot_end(void)
{
    target = screen;
}

void renderer_draw_snapshot(struct renderer_rect screen_area, float alpha)
{
    struct renderer_rect bottom_row_first;
    struct renderer_color tint;

    bottom_row_first.x = 0.0f;
    bottom_row_first.y = 1.0f;
    bottom_row_first.width = 1.0f;
    bottom_row_first.height = -1.0f;
    tint.red = tint.green = tint.blue = 1.0f;
    tint.alpha = alpha;
    fill_sprite(SNAPSHOT_TEXTURE, screen_area, bottom_row_first, tint, 0);
}

void renderer_draw_turned_snapshot(float center_x, float center_y, float half_width, float half_height, float degrees, float alpha)
{
    struct renderer_vertex quad[4];
    struct renderer_color tint;
    float across_x;
    float across_y;
    float down_x;
    float down_y;

    across_x = cosf(degrees * DEGREES_TO_RADIANS) * half_width;
    across_y = sinf(degrees * DEGREES_TO_RADIANS) * half_width;
    down_x = -sinf(degrees * DEGREES_TO_RADIANS) * half_height;
    down_y = cosf(degrees * DEGREES_TO_RADIANS) * half_height;
    tint.red = tint.green = tint.blue = 1.0f;
    tint.alpha = alpha;
    set_corner(&quad[0], center_x - across_x - down_x, center_y - across_y - down_y, 0.0f, 1.0f);
    set_corner(&quad[1], center_x + across_x - down_x, center_y + across_y - down_y, 1.0f, 1.0f);
    set_corner(&quad[2], center_x - across_x + down_x, center_y - across_y + down_y, 0.0f, 0.0f);
    set_corner(&quad[3], center_x + across_x + down_x, center_y + across_y + down_y, 1.0f, 0.0f);
    fill_quad(SNAPSHOT_TEXTURE, quad, tint, 0);
}

void renderer_raise_contrast_in(struct renderer_rect area)
{
    int x;
    int y;
    int channel;

    for (y = (int)area.y < 0 ? 0 : (int)area.y; y < (int)(area.y + area.height) && y < RENDERER_HEIGHT; y++) {
        for (x = (int)area.x < 0 ? 0 : (int)area.x; x < (int)(area.x + area.width) && x < RENDERER_WIDTH; x++) {
            for (channel = 0; channel < 3; channel++) {
                float value;

                value = target[(y * RENDERER_WIDTH + x) * CHANNELS + channel] - CONTRAST_FLOOR;
                value = value < 0.0f ? 0.0f : value;
                value = value + value * CONTRAST_GAIN;
                target[(y * RENDERER_WIDTH + x) * CHANNELS + channel] = value > 1.0f ? 1.0f : value;
            }
        }
    }
}

void renderer_raise_contrast(void)
{
    struct renderer_rect whole;

    whole.x = 0.0f;
    whole.y = 0.0f;
    whole.width = (float)RENDERER_WIDTH;
    whole.height = (float)RENDERER_HEIGHT;
    renderer_raise_contrast_in(whole);
}

int host_renderer_save_screen(const char *path)
{
    FILE *image;
    int index;

    image = fopen(path, "wb");
    if (image == NULL) {
        return 0;
    }
    fprintf(image, "P6\n%d %d\n255\n", RENDERER_WIDTH, RENDERER_HEIGHT);
    for (index = 0; index < RENDERER_WIDTH * RENDERER_HEIGHT; index++) {
        int channel;

        for (channel = 0; channel < 3; channel++) {
            float value;

            value = screen[index * CHANNELS + channel];
            value = value < 0.0f ? 0.0f : (value > 1.0f ? 1.0f : value);
            fputc((int)(value * 255.0f + 0.5f), image);
        }
    }
    fclose(image);
    return 1;
}
