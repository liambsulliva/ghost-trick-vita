#ifndef SPOTLIGHT_MASK_H
#define SPOTLIGHT_MASK_H

#include "renderer.h"

#define SPOTLIGHT_MASK_TRIANGLE_MAX 4
#define SPOTLIGHT_MASK_BLACK_SPANS_PER_ROW_MAX (SPOTLIGHT_MASK_TRIANGLE_MAX + 1)
#define SPOTLIGHT_MASK_RECT_MAX (RENDERER_HEIGHT * SPOTLIGHT_MASK_BLACK_SPANS_PER_ROW_MAX)
#define SPOTLIGHT_MASK_RECTS_AROUND_AREA_MAX 4

struct spotlight_triangle {
    float apex_x;
    float apex_y;
    float base_x;
    float base_y;
    float half_width;
};

int spotlight_mask_black_rects(const struct spotlight_triangle *triangles, int triangle_count, struct renderer_rect *rects);
int spotlight_mask_rects_around_area(struct renderer_rect area, struct renderer_rect *rects);

#endif
