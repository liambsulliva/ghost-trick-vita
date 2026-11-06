#include <limits.h>
#include <stddef.h>
#include "scene_view.h"
#include "texture_cache.h"

#define LOWER_THAN_ANY_LAYER INT_MIN

struct world_rect {
    float left;
    float top;
    float right;
    float bottom;
};

struct screen_view {
    float left;
    float top;
    struct world_rect art;
};

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color black = { 0.0f, 0.0f, 0.0f, 1.0f };
static const struct renderer_color missing_texture_color = { 1.0f, 0.0f, 1.0f, 0.6f };

static struct renderer_rect whole_texture_area(void)
{
    struct renderer_rect area;

    area.x = 0.0f;
    area.y = 0.0f;
    area.width = 1.0f;
    area.height = 1.0f;
    return area;
}

static struct renderer_rect whole_screen_area(void)
{
    struct renderer_rect area;

    area.x = 0.0f;
    area.y = 0.0f;
    area.width = RENDERER_WIDTH;
    area.height = RENDERER_HEIGHT;
    return area;
}

static int plate_overlaps_camera(const struct scene_stage *stage, const struct scene_plate *plate)
{
    return plate->x < stage->camera_x + SCENE_VIEW_DS_SCREEN_WIDTH
        && plate->x + plate->width > stage->camera_x
        && plate->y < stage->camera_y + SCENE_VIEW_DS_SCREEN_HEIGHT
        && plate->y + plate->height > stage->camera_y;
}

static void grow_rect_to_include_plate(struct world_rect *rect, const struct scene_plate *plate)
{
    if (plate->x < rect->left) {
        rect->left = plate->x;
    }
    if (plate->y < rect->top) {
        rect->top = plate->y;
    }
    if (plate->x + plate->width > rect->right) {
        rect->right = plate->x + plate->width;
    }
    if (plate->y + plate->height > rect->bottom) {
        rect->bottom = plate->y + plate->height;
    }
}

static struct world_rect rect_of_plate(const struct scene_plate *plate)
{
    struct world_rect rect;

    rect.left = plate->x;
    rect.top = plate->y;
    rect.right = plate->x + plate->width;
    rect.bottom = plate->y + plate->height;
    return rect;
}

static struct world_rect rect_of_camera(const struct scene_stage *stage)
{
    struct world_rect rect;

    rect.left = stage->camera_x;
    rect.top = stage->camera_y;
    rect.right = stage->camera_x + SCENE_VIEW_DS_SCREEN_WIDTH;
    rect.bottom = stage->camera_y + SCENE_VIEW_DS_SCREEN_HEIGHT;
    return rect;
}

static struct world_rect art_around_camera(const struct scene_stage *stage)
{
    struct world_rect art;
    int found_plate;
    int index;

    art = rect_of_camera(stage);
    found_plate = 0;
    for (index = 0; index < stage->plate_count; index++) {
        if (!plate_overlaps_camera(stage, &stage->plates[index])) {
            continue;
        }
        if (found_plate) {
            grow_rect_to_include_plate(&art, &stage->plates[index]);
        } else {
            art = rect_of_plate(&stage->plates[index]);
            found_plate = 1;
        }
    }
    return art;
}

static float view_start_along_axis(float camera_start, float camera_length, float view_length, float art_start, float art_end)
{
    float start;

    if (art_end - art_start <= view_length) {
        return art_start - (view_length - (art_end - art_start)) / 2.0f;
    }
    start = camera_start - (view_length - camera_length) / 2.0f;
    if (start < art_start) {
        return art_start;
    }
    if (start + view_length > art_end) {
        return art_end - view_length;
    }
    return start;
}

static struct screen_view view_of_stage(const struct scene_stage *stage)
{
    struct screen_view view;

    view.art = art_around_camera(stage);
    view.left = view_start_along_axis(stage->camera_x, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_WIDTH_IN_DS_PIXELS, view.art.left, view.art.right);
    view.top = view_start_along_axis(stage->camera_y, SCENE_VIEW_DS_SCREEN_HEIGHT, SCENE_VIEW_HEIGHT_IN_DS_PIXELS, view.art.top, view.art.bottom);
    return view;
}

static struct renderer_rect world_area_to_screen(const struct screen_view *view, float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = (x - view->left) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.y = (y - view->top) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.width = width * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.height = height * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    return area;
}

static void draw_texture_or_placeholder(const char *texture_name, struct renderer_rect screen_area, struct renderer_rect texture_area)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(texture_name);
    if (texture == NULL) {
        return;
    }
    if (texture->state == CACHED_TEXTURE_MISSING) {
        renderer_draw_filled_rect(screen_area, missing_texture_color);
        return;
    }
    if (texture->state == CACHED_TEXTURE_READY) {
        renderer_draw_sprite(texture->texture, screen_area, texture_area, white);
    }
}

static void draw_plates(const struct scene_stage *stage, const struct screen_view *view)
{
    const struct scene_plate *plate;
    int index;

    for (index = 0; index < stage->plate_count; index++) {
        plate = &stage->plates[index];
        draw_texture_or_placeholder(plate->texture_name, world_area_to_screen(view, plate->x, plate->y, plate->width, plate->height), whole_texture_area());
    }
}

static void draw_object(const struct screen_view *view, const struct scene_object *object)
{
    const struct animation_frame *frame;
    struct renderer_rect texture_area;
    float left;

    frame = animation_player_current_frame(&object->player);
    if (frame == NULL || animation_frame_is_blank(frame)) {
        return;
    }
    texture_area.x = frame->u;
    texture_area.y = frame->v;
    texture_area.width = frame->u_size;
    texture_area.height = frame->v_size;
    left = object->x + frame->x;
    if (object->is_flipped) {
        left = object->x - frame->x - frame->width;
        texture_area.x = frame->u + frame->u_size;
        texture_area.width = -frame->u_size;
    }
    draw_texture_or_placeholder(frame->texture_name, world_area_to_screen(view, left, object->y + frame->y, frame->width, frame->height), texture_area);
}

static int find_lowest_layer_above(const struct scene_stage *stage, int layer, int *found_layer)
{
    int index;
    int found;

    found = 0;
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (!stage->objects[index].in_use || stage->objects[index].layer <= layer) {
            continue;
        }
        if (!found || stage->objects[index].layer < *found_layer) {
            *found_layer = stage->objects[index].layer;
            found = 1;
        }
    }
    return found;
}

static void draw_objects_in_layer_order(const struct scene_stage *stage, const struct screen_view *view)
{
    int layer;
    int next_layer;
    int index;

    layer = LOWER_THAN_ANY_LAYER;
    while (find_lowest_layer_above(stage, layer, &next_layer)) {
        layer = next_layer;
        for (index = 0; index < SCENE_OBJECT_MAX; index++) {
            if (stage->objects[index].in_use && stage->objects[index].layer == layer) {
                draw_object(view, &stage->objects[index]);
            }
        }
    }
}

static void fill_black_when_visible(float x, float y, float width, float height)
{
    struct renderer_rect area;

    if (width <= 0.0f || height <= 0.0f) {
        return;
    }
    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    renderer_draw_filled_rect(area, black);
}

static void draw_black_around_art(const struct screen_view *view)
{
    struct renderer_rect art;
    float art_screen_right;
    float art_screen_bottom;

    art = world_area_to_screen(view, view->art.left, view->art.top, view->art.right - view->art.left, view->art.bottom - view->art.top);
    art_screen_right = art.x + art.width;
    art_screen_bottom = art.y + art.height;
    fill_black_when_visible(0.0f, 0.0f, RENDERER_WIDTH, art.y);
    fill_black_when_visible(0.0f, art_screen_bottom, RENDERER_WIDTH, RENDERER_HEIGHT - art_screen_bottom);
    fill_black_when_visible(0.0f, art.y, art.x, art.height);
    fill_black_when_visible(art_screen_right, art.y, RENDERER_WIDTH - art_screen_right, art.height);
}

static void draw_card(const char *texture_name)
{
    struct renderer_rect area;

    renderer_draw_filled_rect(whole_screen_area(), black);
    area.x = (RENDERER_WIDTH - SCENE_VIEW_CARD_WIDTH) / 2.0f;
    area.y = (RENDERER_HEIGHT - SCENE_VIEW_CARD_HEIGHT) / 2.0f;
    area.width = SCENE_VIEW_CARD_WIDTH;
    area.height = SCENE_VIEW_CARD_HEIGHT;
    draw_texture_or_placeholder(texture_name, area, whole_texture_area());
}

void scene_view_draw(const struct scene_stage *stage)
{
    struct screen_view view;
    struct renderer_color fade_color;

    view = view_of_stage(stage);
    draw_plates(stage, &view);
    draw_objects_in_layer_order(stage, &view);
    draw_black_around_art(&view);
    if (stage->card_texture[0] != '\0') {
        draw_card(stage->card_texture);
    }
    if (stage->fade_amount > 0.0f) {
        fade_color = black;
        fade_color.alpha = stage->fade_amount;
        renderer_draw_filled_rect(whole_screen_area(), fade_color);
    }
}
