#ifndef SWIRL_WAVES_H
#define SWIRL_WAVES_H

#include "renderer.h"
#include "script_tables.h"

#define SWIRL_WAVES_FLIPBOOK "swirl_waves"
#define SWIRL_WAVES_TILES_ACROSS 3
#define SWIRL_WAVES_TRIANGLES (SWIRL_WAVES_TILES_ACROSS * SWIRL_WAVES_TILES_ACROSS * 2)

float swirl_waves_radians(unsigned int ticks_on);
struct renderer_rect swirl_waves_cell(const struct script_flipbook *book, unsigned int ticks_on);
void swirl_waves_corners(float center_x, float center_y, float tile_side, float radians, struct renderer_rect cell, struct renderer_vertex *corners);
int swirl_waves_draw(const struct script_flipbook *book, unsigned int ticks_on);

#endif
