#include "spotlight_mask.h"

struct pixel_span {
    int left;
    int right;
};

struct rect_builder {
    struct renderer_rect *rects;
    int count;
    int open_rects[SPOTLIGHT_MASK_BLACK_SPANS_PER_ROW_MAX];
    int open_count;
    int next_open_rects[SPOTLIGHT_MASK_BLACK_SPANS_PER_ROW_MAX];
    int next_open_count;
};

static int nearest_column_on_screen(float x)
{
    if (x <= 0.0f) {
        return 0;
    }
    if (x >= (float)RENDERER_WIDTH) {
        return RENDERER_WIDTH;
    }
    return (int)(x + 0.5f);
}

static int find_lit_span_of_triangle(const struct spotlight_triangle *triangle, float row_centre_y, struct pixel_span *span)
{
    float progress;
    float centre;
    float half_width;

    if (triangle->base_y == triangle->apex_y) {
        return 0;
    }
    progress = (row_centre_y - triangle->apex_y) / (triangle->base_y - triangle->apex_y);
    if (progress < 0.0f || progress > 1.0f) {
        return 0;
    }
    centre = triangle->apex_x + (triangle->base_x - triangle->apex_x) * progress;
    half_width = triangle->half_width * progress;
    span->left = nearest_column_on_screen(centre - half_width);
    span->right = nearest_column_on_screen(centre + half_width);
    return span->left < span->right;
}

static void insert_span_keeping_order(struct pixel_span *spans, int count, struct pixel_span span)
{
    int index;

    index = count;
    while (index > 0 && spans[index - 1].left > span.left) {
        spans[index] = spans[index - 1];
        index = index - 1;
    }
    spans[index] = span;
}

static int find_lit_spans_in_row(const struct spotlight_triangle *triangles, int triangle_count, float row_centre_y, struct pixel_span *spans)
{
    struct pixel_span span;
    int count;
    int index;

    count = 0;
    for (index = 0; index < triangle_count; index++) {
        if (find_lit_span_of_triangle(&triangles[index], row_centre_y, &span)) {
            insert_span_keeping_order(spans, count, span);
            count = count + 1;
        }
    }
    return count;
}

static int find_open_rect_with_span(const struct rect_builder *builder, int left, int right)
{
    const struct renderer_rect *rect;
    int index;

    for (index = 0; index < builder->open_count; index++) {
        rect = &builder->rects[builder->open_rects[index]];
        if ((int)rect->x == left && (int)rect->width == right - left) {
            return builder->open_rects[index];
        }
    }
    return -1;
}

static void add_black_span(struct rect_builder *builder, int left, int right, int row)
{
    struct renderer_rect *rect;
    int rect_index;

    rect_index = find_open_rect_with_span(builder, left, right);
    if (rect_index >= 0) {
        builder->rects[rect_index].height = builder->rects[rect_index].height + 1.0f;
    } else {
        rect_index = builder->count;
        rect = &builder->rects[rect_index];
        rect->x = (float)left;
        rect->y = (float)row;
        rect->width = (float)(right - left);
        rect->height = 1.0f;
        builder->count = builder->count + 1;
    }
    builder->next_open_rects[builder->next_open_count] = rect_index;
    builder->next_open_count = builder->next_open_count + 1;
}

static void finish_row(struct rect_builder *builder)
{
    int index;

    for (index = 0; index < builder->next_open_count; index++) {
        builder->open_rects[index] = builder->next_open_rects[index];
    }
    builder->open_count = builder->next_open_count;
    builder->next_open_count = 0;
}

int spotlight_mask_black_rects(const struct spotlight_triangle *triangles, int triangle_count, struct renderer_rect *rects)
{
    struct pixel_span lit_spans[SPOTLIGHT_MASK_TRIANGLE_MAX];
    struct rect_builder builder;
    int lit_count;
    int covered_up_to;
    int index;
    int row;

    builder.rects = rects;
    builder.count = 0;
    builder.open_count = 0;
    builder.next_open_count = 0;
    for (row = 0; row < RENDERER_HEIGHT; row++) {
        lit_count = find_lit_spans_in_row(triangles, triangle_count, (float)row + 0.5f, lit_spans);
        covered_up_to = 0;
        for (index = 0; index < lit_count; index++) {
            if (lit_spans[index].left > covered_up_to) {
                add_black_span(&builder, covered_up_to, lit_spans[index].left, row);
            }
            if (lit_spans[index].right > covered_up_to) {
                covered_up_to = lit_spans[index].right;
            }
        }
        if (covered_up_to < RENDERER_WIDTH) {
            add_black_span(&builder, covered_up_to, RENDERER_WIDTH, row);
        }
        finish_row(&builder);
    }
    return builder.count;
}

static float keep_between(float value, float lowest, float highest)
{
    if (value < lowest) {
        return lowest;
    }
    if (value > highest) {
        return highest;
    }
    return value;
}

static int add_rect_if_visible(struct renderer_rect *rects, int count, float x, float y, float width, float height)
{
    if (width <= 0.0f || height <= 0.0f) {
        return count;
    }
    rects[count].x = x;
    rects[count].y = y;
    rects[count].width = width;
    rects[count].height = height;
    return count + 1;
}

int spotlight_mask_rects_around_area(struct renderer_rect area, struct renderer_rect *rects)
{
    float left;
    float right;
    float top;
    float bottom;
    int count;

    left = keep_between(area.x, 0.0f, RENDERER_WIDTH);
    right = keep_between(area.x + area.width, 0.0f, RENDERER_WIDTH);
    top = keep_between(area.y, 0.0f, RENDERER_HEIGHT);
    bottom = keep_between(area.y + area.height, 0.0f, RENDERER_HEIGHT);
    count = 0;
    count = add_rect_if_visible(rects, count, 0.0f, 0.0f, RENDERER_WIDTH, top);
    count = add_rect_if_visible(rects, count, 0.0f, bottom, RENDERER_WIDTH, RENDERER_HEIGHT - bottom);
    count = add_rect_if_visible(rects, count, 0.0f, top, left, bottom - top);
    count = add_rect_if_visible(rects, count, right, top, RENDERER_WIDTH - right, bottom - top);
    return count;
}
