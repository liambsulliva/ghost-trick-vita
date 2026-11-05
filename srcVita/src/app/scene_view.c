#include <stddef.h>
#include "renderer.h"
#include "scene_view.h"
#include "texture_cache.h"

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color missing_texture_color = { 1.0f, 0.0f, 1.0f, 0.6f };
static const struct renderer_color plain_bar_color = { 0.02f, 0.11f, 0.12f, 1.0f };

static struct renderer_rect whole_texture_area(void)
{
    struct renderer_rect area;

    area.x = 0.0f;
    area.y = 0.0f;
    area.width = 1.0f;
    area.height = 1.0f;
    return area;
}

static struct renderer_rect world_area_to_screen(const struct scene_picture *picture, float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = SCENE_VIEW_PLAYFIELD_X + (x - picture->camera_x) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.y = SCENE_VIEW_PLAYFIELD_Y + (y - picture->camera_y) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.width = width * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    area.height = height * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    return area;
}

static void draw_texture_or_placeholder(const char *texture_name, struct renderer_rect screen_area, struct renderer_rect texture_area)
{
    const struct cached_texture *texture;

    texture = texture_cache_request(texture_name);
    if (texture == NULL || texture->state == CACHED_TEXTURE_MISSING) {
        renderer_draw_filled_rect(screen_area, missing_texture_color);
        return;
    }
    if (texture->state == CACHED_TEXTURE_READY) {
        renderer_draw_sprite(texture->texture, screen_area, texture_area, white);
    }
}

static void draw_plates(const struct scene_picture *picture)
{
    const struct scene_plate *plate;
    int index;

    for (index = 0; index < picture->plate_count; index++) {
        plate = &picture->plates[index];
        draw_texture_or_placeholder(plate->texture_name, world_area_to_screen(picture, plate->x, plate->y, plate->width, plate->height), whole_texture_area());
    }
}

static void draw_object(const struct scene_picture *picture, const struct scene_object *object)
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
    draw_texture_or_placeholder(frame->texture_name, world_area_to_screen(picture, left, object->y + frame->y, frame->width, frame->height), texture_area);
}

static int find_lowest_layer_above(const struct scene_picture *picture, int layer, int *found_layer)
{
    int index;
    int found;

    found = 0;
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (!picture->objects[index].in_use || picture->objects[index].layer <= layer) {
            continue;
        }
        if (!found || picture->objects[index].layer < *found_layer) {
            *found_layer = picture->objects[index].layer;
            found = 1;
        }
    }
    return found;
}

static void draw_objects_in_layer_order(const struct scene_picture *picture)
{
    int layer;
    int next_layer;
    int index;

    layer = -2147483647;
    while (find_lowest_layer_above(picture, layer, &next_layer)) {
        layer = next_layer;
        for (index = 0; index < SCENE_OBJECT_MAX; index++) {
            if (picture->objects[index].in_use && picture->objects[index].layer == layer) {
                draw_object(picture, &picture->objects[index]);
            }
        }
    }
}

static struct renderer_rect playfield_area(void)
{
    struct renderer_rect area;

    area.x = SCENE_VIEW_PLAYFIELD_X;
    area.y = SCENE_VIEW_PLAYFIELD_Y;
    area.width = SCENE_VIEW_PLAYFIELD_WIDTH;
    area.height = SCENE_VIEW_PLAYFIELD_HEIGHT;
    return area;
}

static void draw_side_bar(const char *texture_name, float x)
{
    struct renderer_rect area;

    area.x = x;
    area.y = 0.0f;
    area.width = SCENE_VIEW_PLAYFIELD_X;
    area.height = RENDERER_HEIGHT;
    if (texture_name[0] == '\0') {
        renderer_draw_filled_rect(area, plain_bar_color);
        return;
    }
    draw_texture_or_placeholder(texture_name, area, whole_texture_area());
}

static void draw_frame_bands(void)
{
    struct renderer_color black;
    struct renderer_rect band;

    black.red = 0.0f;
    black.green = 0.0f;
    black.blue = 0.0f;
    black.alpha = 1.0f;
    band.x = SCENE_VIEW_PLAYFIELD_X;
    band.width = SCENE_VIEW_PLAYFIELD_WIDTH;
    band.height = SCENE_VIEW_PLAYFIELD_Y;
    band.y = 0.0f;
    renderer_draw_filled_rect(band, black);
    band.y = SCENE_VIEW_PLAYFIELD_Y + SCENE_VIEW_PLAYFIELD_HEIGHT;
    renderer_draw_filled_rect(band, black);
}

void scene_view_draw(const struct scene_picture *picture)
{
    struct renderer_color fade_color;

    draw_plates(picture);
    draw_objects_in_layer_order(picture);
    if (picture->card_texture[0] != '\0') {
        draw_texture_or_placeholder(picture->card_texture, playfield_area(), whole_texture_area());
    }
    if (picture->fade_amount > 0.0f) {
        fade_color.red = 0.0f;
        fade_color.green = 0.0f;
        fade_color.blue = 0.0f;
        fade_color.alpha = picture->fade_amount;
        renderer_draw_filled_rect(playfield_area(), fade_color);
    }
    draw_frame_bands();
    draw_side_bar(picture->left_bar_texture, 0.0f);
    draw_side_bar(picture->right_bar_texture, SCENE_VIEW_PLAYFIELD_X + SCENE_VIEW_PLAYFIELD_WIDTH);
}
