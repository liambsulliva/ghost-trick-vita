#include <math.h>
#include <stddef.h>
#include "ds_units.h"
#include "swirl_waves.h"
#include "texture_cache.h"

#define NATIVE_TURN_STEPS 65536.0f
#define NATIVE_TURN_PER_TICK 32.0f
#define TWO_PI 6.2831853f

float swirl_waves_radians(unsigned int ticks_on)
{
    return fmodf((float)ticks_on * NATIVE_TURN_PER_TICK, NATIVE_TURN_STEPS) / NATIVE_TURN_STEPS * TWO_PI;
}

struct renderer_rect swirl_waves_cell(const struct script_flipbook *book, unsigned int ticks_on)
{
    struct renderer_rect cell;
    int frame;

    frame = (int)(ticks_on / (unsigned int)book->ticks_per_frame) % book->frame_count;
    cell.x = (float)(frame % book->columns) * book->cell_width;
    cell.y = (float)(frame / book->columns) * book->cell_height;
    cell.width = book->cell_width;
    cell.height = book->cell_height;
    return cell;
}

static struct renderer_vertex turned_corner(float center_x, float center_y, float x, float y, float cosine, float sine, float u, float v)
{
    struct renderer_vertex vertex;

    vertex.x = center_x + x * cosine - y * sine;
    vertex.y = center_y + x * sine + y * cosine;
    vertex.u = u;
    vertex.v = v;
    return vertex;
}

void swirl_waves_corners(float center_x, float center_y, float tile_side, float radians, struct renderer_rect cell, struct renderer_vertex *corners)
{
    float cosine;
    float sine;
    int row;
    int column;

    cosine = cosf(radians);
    sine = sinf(radians);
    for (row = 0; row < SWIRL_WAVES_TILES_ACROSS; row++) {
        for (column = 0; column < SWIRL_WAVES_TILES_ACROSS; column++) {
            struct renderer_vertex top_left;
            struct renderer_vertex top_right;
            struct renderer_vertex bottom_left;
            struct renderer_vertex bottom_right;
            struct renderer_vertex *tile;
            float left;
            float top;

            left = ((float)column - (float)SWIRL_WAVES_TILES_ACROSS / 2.0f) * tile_side;
            top = ((float)row - (float)SWIRL_WAVES_TILES_ACROSS / 2.0f) * tile_side;
            top_left = turned_corner(center_x, center_y, left, top, cosine, sine, cell.x, cell.y);
            top_right = turned_corner(center_x, center_y, left + tile_side, top, cosine, sine, cell.x + cell.width, cell.y);
            bottom_left = turned_corner(center_x, center_y, left, top + tile_side, cosine, sine, cell.x, cell.y + cell.height);
            bottom_right = turned_corner(center_x, center_y, left + tile_side, top + tile_side, cosine, sine, cell.x + cell.width, cell.y + cell.height);
            tile = &corners[(row * SWIRL_WAVES_TILES_ACROSS + column) * 6];
            tile[0] = top_left;
            tile[1] = top_right;
            tile[2] = bottom_left;
            tile[3] = top_right;
            tile[4] = bottom_right;
            tile[5] = bottom_left;
        }
    }
}

int swirl_waves_draw(const struct script_flipbook *book, unsigned int ticks_on)
{
    static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
    const struct cached_texture *texture;
    struct renderer_vertex corners[SWIRL_WAVES_TRIANGLES * 3];
    float tile_side;

    if (book == NULL || book->page_count <= 0 || book->ticks_per_frame <= 0 || book->frame_count <= 0 || book->columns <= 0) {
        return 0;
    }
    texture = texture_cache_use_or_start_loading(book->pages[0]);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return 0;
    }
    tile_side = (float)texture->width * book->cell_width * VITA_PIXELS_PER_DS_PIXEL;
    swirl_waves_corners((float)RENDERER_WIDTH / 2.0f, (float)RENDERER_HEIGHT / 2.0f, tile_side, swirl_waves_radians(ticks_on), swirl_waves_cell(book, ticks_on), corners);
    renderer_draw_triangles(texture->texture, corners, SWIRL_WAVES_TRIANGLES, white);
    return 1;
}
