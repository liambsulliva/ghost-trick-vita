#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "page_tiles.h"
#include "iris_pattern.h"
#include "memory.h"
#include "run_log.h"
#include "scene_view.h"
#include "spotlight_mask.h"
#include "texture_cache.h"

#define OFF_SCREEN_MARGIN_DS_PIXELS 120.0f
#define READ_AHEAD_TICKS 120u
#define LOWER_THAN_ANY_LAYER INT_MIN
#define IRIS_TEXTURE_SIZE 256
#define MASK_RECT_COUNT_WORTH_LOGGING 600
#define MISSING_TEXT_MARKER_WIDTH 400.0f
#define MISSING_TEXT_MARKER_HEIGHT 52.0f

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
static struct renderer_color turned_for_the_ghost(struct renderer_color color, int ghost);
#define GHOST_SCANLINE_PITCH 3.0f
#define GHOST_SCANLINE_HEIGHT 1.0f
#define GHOST_SCANLINE_ALPHA 0.28f
#define GHOST_SCANLINE_ROWS_MAX 256
#define GHOST_SWEEP_HEIGHT 3.0f
#define GHOST_SWEEP_FRAMES 240u
static const struct renderer_color missile_sweep_color = { 0.2f, 1.0f, 0.2f, 0.55f };
static const struct renderer_color boss_sweep_color = { 0.15f, 0.35f, 1.0f, 0.55f };
static const struct renderer_color missing_texture_color = { 1.0f, 0.0f, 1.0f, 0.6f };
static const struct renderer_color ghost_copy_lavender = { 16.0f / 31.0f, 16.0f / 31.0f, 1.0f, 1.0f };

static renderer_texture iris_texture;
static int largest_logged_mask_rect_count;

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

static int area_is_on_screen(struct renderer_rect area)
{
    float left;
    float right;

    left = area.width < 0.0f ? area.x + area.width : area.x;
    right = area.width < 0.0f ? area.x : area.x + area.width;
    return right > 0.0f && left < (float)RENDERER_WIDTH && area.y + area.height > 0.0f && area.y < (float)RENDERER_HEIGHT;
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

static float view_start_centred_on_camera(float camera_start, float camera_length, float view_length)
{
    return camera_start - (view_length - camera_length) / 2.0f;
}

static float view_start_along_axis(float camera_start, float camera_length, float view_length, float art_start, float art_end)
{
    float start;

    if (art_end - art_start <= view_length) {
        return art_start - (view_length - (art_end - art_start)) / 2.0f;
    }
    start = view_start_centred_on_camera(camera_start, camera_length, view_length);
    if (start < art_start) {
        return art_start;
    }
    if (start + view_length > art_end) {
        return art_end - view_length;
    }
    return start;
}

static int only_beams_can_be_seen(const struct scene_effects *effects)
{
    return effects->mask_is_on && !effects->iris_is_on && !effects->mask_uses_cover_alpha;
}

static struct screen_view view_of_stage(const struct scene_stage *stage)
{
    struct screen_view view;

    view.art = art_around_camera(stage);
    if (only_beams_can_be_seen(&stage->effects)) {
        view.left = view_start_centred_on_camera(stage->camera_x, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_WIDTH_IN_DS_PIXELS);
        view.top = view_start_centred_on_camera(stage->camera_y, SCENE_VIEW_DS_SCREEN_HEIGHT, SCENE_VIEW_HEIGHT_IN_DS_PIXELS);
    } else {
        view.left = view_start_along_axis(stage->camera_x, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_WIDTH_IN_DS_PIXELS, view.art.left, view.art.right);
        view.top = view_start_along_axis(stage->camera_y, SCENE_VIEW_DS_SCREEN_HEIGHT, SCENE_VIEW_HEIGHT_IN_DS_PIXELS, view.art.top, view.art.bottom);
    }
    return view;
}

static struct screen_view view_for_objects(const struct scene_stage *stage, const struct screen_view *view)
{
    struct screen_view objects;

    objects = *view;
    objects.left = view->left - stage->object_origin_x;
    objects.top = view->top - stage->object_origin_y;
    return objects;
}

static struct renderer_rect world_area_to_screen(const struct screen_view *view, float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = (x - view->left) * VITA_PIXELS_PER_DS_PIXEL;
    area.y = (y - view->top) * VITA_PIXELS_PER_DS_PIXEL;
    area.width = width * VITA_PIXELS_PER_DS_PIXEL;
    area.height = height * VITA_PIXELS_PER_DS_PIXEL;
    return area;
}

static void draw_page_area(const char *texture_name, const struct cached_texture *texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color, int is_flat)
{
    const struct page_tile_set *tiles;
    struct page_tile_part parts[PAGE_TILE_MOST];
    int part_count;
    int part;

    tiles = page_tiles_find(texture_name);
    if (tiles == NULL) {
        if (is_flat) {
            renderer_draw_flat_sprite(texture->texture, screen_area, texture_area, color);
        } else {
            renderer_draw_sprite(texture->texture, screen_area, texture_area, color);
        }
        return;
    }
    part_count = page_tiles_cut(tiles, screen_area, texture_area, parts);
    for (part = 0; part < part_count; part++) {
        const struct cached_texture *tile_texture;

        tile_texture = texture_cache_use_or_start_loading(parts[part].tile->name);
        if (tile_texture == NULL || tile_texture->state != CACHED_TEXTURE_READY) {
            continue;
        }
        if (is_flat) {
            renderer_draw_flat_sprite(tile_texture->texture, parts[part].screen_area, parts[part].texture_area, color);
        } else {
            renderer_draw_sprite(tile_texture->texture, parts[part].screen_area, parts[part].texture_area, color);
        }
    }
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
        draw_page_area(texture_name, texture, screen_area, texture_area, white, 0);
    }
}

#define POSE_WAIT_FRAMES_WORTH_LOGGING 2

struct held_pose {
    char object_name[ASSET_NAME_MAX];
    char texture_name[ASSET_NAME_MAX];
    float world_left;
    float world_top;
    float width;
    float height;
    struct renderer_rect texture_area;
    int frames_waiting;
};

static struct renderer_color color_at_level(int level)
{
    struct renderer_color color;
    float shade;

    shade = level >= SCENE_COLOR_LEVEL_NORMAL ? 1.0f : (level <= 0 ? 0.0f : (float)level / (float)SCENE_COLOR_LEVEL_NORMAL);
    color = white;
    color.red = shade;
    color.green = shade;
    color.blue = shade;
    return color;
}

static int draw_when_ready(const char *texture_name, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color, int is_flat)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(texture_name);
    if (texture == NULL) {
        return 0;
    }
    if (texture->state == CACHED_TEXTURE_MISSING) {
        renderer_draw_filled_rect(screen_area, missing_texture_color);
        return 1;
    }
    if (texture->state != CACHED_TEXTURE_READY) {
        return 0;
    }
    draw_page_area(texture_name, texture, screen_area, texture_area, color, is_flat);
    return 1;
}

static int ghost_copy_alpha_level(const struct scene_stage *stage, int level)
{
    return stage->soul_mode_is_on ? level >> 1 : (level * 3) >> 2;
}

static int object_is_hidden(const struct scene_stage *stage, const struct scene_object *object)
{
    if (object->ghost_copy_level != SCENE_NO_GHOST_COPY) {
        return ghost_copy_alpha_level(stage, object->ghost_copy_level) <= 0;
    }
    return stage->soul_mode_is_on ? object->hides_in_soul_view : object->hides_in_living_view;
}

static struct renderer_color object_paint(const struct scene_stage *stage, const struct scene_object *object, int *is_flat)
{
    struct renderer_color color;

    *is_flat = 0;
    if (object->ghost_copy_level != SCENE_NO_GHOST_COPY) {
        color = ghost_copy_lavender;
        color.alpha = (float)ghost_copy_alpha_level(stage, object->ghost_copy_level) / (float)SCENE_GHOST_COPY_LEVEL_FULL;
        *is_flat = 1;
        return color;
    }
    if (stage->soul_mode_is_on && object->has_soul_tint) {
        color.red = object->soul_red;
        color.green = object->soul_green;
        color.blue = object->soul_blue;
        color.alpha = object->soul_alpha;
        if (object->soul_turns_with_ghost) {
            color = turned_for_the_ghost(color, scene_stage_soul_ghost());
        }
        *is_flat = object->soul_is_flat;
        return color;
    }
    return color_at_level(stage->soul_mode_is_on ? SCENE_COLOR_LEVEL_NORMAL : object->color_level);
}

static const char *art_under_the_screen_filter(const struct scene_stage *stage, const char *texture_name, char *grayscale_name)
{
    if (!stage->mono_mode_is_on || stage->soul_mode_is_on) {
        return texture_name;
    }
    if (snprintf(grayscale_name, ASSET_NAME_MAX, "%s" TEXTURE_CACHE_GRAYSCALE_SUFFIX, texture_name) >= ASSET_NAME_MAX) {
        return texture_name;
    }
    return grayscale_name;
}

static const char *plate_art_name(const struct scene_stage *stage, const struct scene_plate *plate, char *grayscale_name)
{
    if (stage->soul_mode_is_on && stage->soul_ghost == SCENE_GHOST_MISSILE && plate->missile_texture_name[0] != '\0') {
        return plate->missile_texture_name;
    }
    if (stage->soul_mode_is_on && stage->soul_ghost == SCENE_GHOST_BOSS && plate->boss_texture_name[0] != '\0') {
        return plate->boss_texture_name;
    }
    if (stage->soul_mode_is_on && plate->soul_texture_name[0] != '\0') {
        return plate->soul_texture_name;
    }
    return art_under_the_screen_filter(stage, plate->texture_name, grayscale_name);
}

static void draw_plates(const struct scene_stage *stage, const struct screen_view *view)
{
    const struct scene_plate *plate;
    int index;

    for (index = 0; index < stage->plate_count; index++) {
        char grayscale_name[ASSET_NAME_MAX];
        const char *wanted;
        const char *other;
        struct renderer_rect screen_area;
        struct renderer_rect texture_area;

        plate = &stage->plates[index];
        wanted = plate_art_name(stage, plate, grayscale_name);
        other = wanted == plate->texture_name ? plate->soul_texture_name : plate->texture_name;
        screen_area = world_area_to_screen(view, plate->x, plate->y, plate->width, plate->height);
        texture_area.x = 0.0f;
        texture_area.y = 0.0f;
        texture_area.width = 1.0f;
        texture_area.height = 1.0f;
        if (other[0] != '\0') {
            texture_cache_use_or_start_loading(other);
        }
        if (draw_when_ready(wanted, screen_area, texture_area, color_at_level(stage->soul_mode_is_on ? SCENE_COLOR_LEVEL_NORMAL : stage->background_color_level), 0) || other[0] == '\0') {
            continue;
        }
        draw_when_ready(other, screen_area, texture_area, color_at_level(stage->soul_mode_is_on ? SCENE_COLOR_LEVEL_NORMAL : stage->background_color_level), 0);
    }
}

static struct held_pose held_poses[SCENE_OBJECT_MAX];

static void read_art_ahead(const struct scene_stage *stage, const struct scene_object *object)
{
    const struct animation *animation;
    const char *last_name;
    char grayscale_name[ASSET_NAME_MAX];
    unsigned int ticks;
    unsigned int step;

    animation = object->player.animation;
    if (animation == NULL) {
        return;
    }
    ticks = 0;
    last_name = animation->frames[object->player.frame_index].texture_name;
    for (step = 1; step < animation->frame_count && ticks < READ_AHEAD_TICKS; step++) {
        const struct animation_frame *ahead;

        ahead = &animation->frames[(object->player.frame_index + step) % animation->frame_count];
        ticks = ticks + ahead->ticks;
        if (!animation_frame_is_blank(ahead) && strcmp(last_name, ahead->texture_name) != 0) {
            texture_cache_keep_or_start_reading_ahead(art_under_the_screen_filter(stage, ahead->texture_name, grayscale_name));
            last_name = ahead->texture_name;
        }
    }
}

static void draw_object(const struct scene_stage *stage, const struct screen_view *view, const struct scene_object *object)
{
    const struct animation_frame *frame;
    struct held_pose *held;
    struct renderer_color paint;
    int paint_is_flat;
    struct renderer_rect texture_area;
    char grayscale_name[ASSET_NAME_MAX];
    const char *wanted;
    float left;

    if (object_is_hidden(stage, object)) {
        return;
    }
    frame = animation_player_current_frame(&object->player);
    if (frame == NULL || animation_frame_is_blank(frame)) {
        return;
    }
    held = &held_poses[object - stage->objects];
    paint = object_paint(stage, object, &paint_is_flat);
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
    if (!area_is_on_screen(world_area_to_screen(view, left - OFF_SCREEN_MARGIN_DS_PIXELS, object->y + frame->y - OFF_SCREEN_MARGIN_DS_PIXELS, frame->width + OFF_SCREEN_MARGIN_DS_PIXELS * 2.0f, frame->height + OFF_SCREEN_MARGIN_DS_PIXELS * 2.0f))) {
        return;
    }
    read_art_ahead(stage, object);
    wanted = art_under_the_screen_filter(stage, frame->texture_name, grayscale_name);
    if (draw_when_ready(wanted, world_area_to_screen(view, left, object->y + frame->y, frame->width, frame->height), texture_area, paint, paint_is_flat)) {
        if (held->frames_waiting >= POSE_WAIT_FRAMES_WORTH_LOGGING && strcmp(held->object_name, object->name) == 0) {
            run_log_write("pose held object=%s frames=%d waited_for=%s", object->name, held->frames_waiting, wanted);
        }
        held->frames_waiting = 0;
        snprintf(held->object_name, sizeof(held->object_name), "%s", object->name);
        snprintf(held->texture_name, sizeof(held->texture_name), "%s", wanted);
        held->world_left = left;
        held->world_top = object->y + frame->y;
        held->width = frame->width;
        held->height = frame->height;
        held->texture_area = texture_area;
        return;
    }
    if (strcmp(held->object_name, object->name) == 0) {
        held->frames_waiting = held->frames_waiting + 1;
        draw_when_ready(held->texture_name, world_area_to_screen(view, held->world_left, held->world_top, held->width, held->height), held->texture_area, paint, paint_is_flat);
    }
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
                draw_object(stage, view, &stage->objects[index]);
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

static float world_x_to_screen(const struct screen_view *view, float x)
{
    return (x - view->left) * VITA_PIXELS_PER_DS_PIXEL;
}

static float world_y_to_screen(const struct screen_view *view, float y)
{
    return (y - view->top) * VITA_PIXELS_PER_DS_PIXEL;
}

static int find_beam_triangles_on_screen(const struct scene_effects *effects, const struct screen_view *view, struct spotlight_triangle *triangles)
{
    const struct scene_beam *beam;
    int count;
    int index;

    count = 0;
    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        beam = &effects->beams[index];
        if (beam->in_use) {
            triangles[count].apex_x = world_x_to_screen(view, beam->apex_x);
            triangles[count].apex_y = world_y_to_screen(view, beam->apex_y);
            triangles[count].base_x = world_x_to_screen(view, beam->base_x.value);
            triangles[count].base_y = world_y_to_screen(view, beam->base_y.value);
            triangles[count].half_width = beam->half_width.value * VITA_PIXELS_PER_DS_PIXEL;
            count = count + 1;
        }
    }
    return count;
}

static void log_mask_rect_count_when_large(int rect_count)
{
    if (rect_count > MASK_RECT_COUNT_WORTH_LOGGING && rect_count > largest_logged_mask_rect_count) {
        largest_logged_mask_rect_count = rect_count;
        run_log_write("scene mask_rects count=%d", rect_count);
    }
}

static void draw_mask(const struct scene_effects *effects, const struct screen_view *view)
{
    static struct renderer_rect rects[SPOTLIGHT_MASK_RECT_MAX];
    struct spotlight_triangle triangles[SCENE_BEAM_MAX];
    int triangle_count;
    int rect_count;

    triangle_count = find_beam_triangles_on_screen(effects, view, triangles);
    rect_count = spotlight_mask_black_rects(triangles, triangle_count, rects);
    log_mask_rect_count_when_large(rect_count);
    if (!effects->mask_uses_cover_alpha || effects->mask_is_opaque) {
        renderer_draw_filled_rects(rects, rect_count, black);
    } else {
        struct renderer_color cover;

        cover = black;
        cover.alpha = scene_effects_strongest_beam_cover(effects);
        if (cover.alpha > 0.0f) {
            renderer_draw_filled_rects(rects, rect_count, cover);
        }
    }
    if (effects->mask_uses_cover_alpha && effects->mask_fade_alpha > 0.0f) {
        struct renderer_color fade;

        fade = effects->mask_fade_is_white ? white : black;
        fade.alpha = effects->mask_fade_alpha;
        renderer_draw_filled_rects(rects, rect_count, fade);
    }
}

static int point_is_within_clear_part_of_iris(float x, float y, float center_x, float center_y, float clear_radius)
{
    float from_center_x;
    float from_center_y;

    from_center_x = x - center_x;
    from_center_y = y - center_y;
    return from_center_x * from_center_x + from_center_y * from_center_y <= clear_radius * clear_radius;
}

static int iris_clears_whole_screen(float center_x, float center_y, float radius)
{
    float clear_radius;

    clear_radius = radius * IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS;
    return point_is_within_clear_part_of_iris(0.0f, 0.0f, center_x, center_y, clear_radius)
        && point_is_within_clear_part_of_iris(RENDERER_WIDTH, 0.0f, center_x, center_y, clear_radius)
        && point_is_within_clear_part_of_iris(0.0f, RENDERER_HEIGHT, center_x, center_y, clear_radius)
        && point_is_within_clear_part_of_iris(RENDERER_WIDTH, RENDERER_HEIGHT, center_x, center_y, clear_radius);
}

static void draw_iris(const struct scene_effects *effects, const struct screen_view *view)
{
    struct renderer_rect rects[SPOTLIGHT_MASK_RECTS_AROUND_AREA_MAX];
    struct renderer_rect circle;
    float center_x;
    float center_y;
    float radius;
    struct renderer_color edge;
    struct renderer_color cover;

    center_x = world_x_to_screen(view, effects->iris_x.value);
    center_y = world_y_to_screen(view, effects->iris_y.value);
    radius = effects->iris_radius.value * VITA_PIXELS_PER_DS_PIXEL;
    if (effects->iris_alpha.value <= 0.0f || iris_clears_whole_screen(center_x, center_y, radius)) {
        return;
    }
    edge = white;
    edge.alpha = effects->iris_alpha.value;
    cover = black;
    cover.alpha = effects->iris_alpha.value;
    circle.x = center_x - radius;
    circle.y = center_y - radius;
    circle.width = radius * 2.0f;
    circle.height = radius * 2.0f;
    if (iris_texture != RENDERER_NO_TEXTURE) {
        renderer_draw_sprite(iris_texture, circle, whole_texture_area(), edge);
    }
    renderer_draw_filled_rects(rects, spotlight_mask_rects_around_area(circle, rects), cover);
}

static void draw_mask_or_iris(const struct scene_effects *effects, const struct screen_view *view)
{
    if (effects->iris_is_on) {
        draw_iris(effects, view);
    } else if (effects->mask_is_on) {
        draw_mask(effects, view);
    }
}

static void draw_dim(const struct scene_effects *effects)
{
    struct renderer_color dim_color;

    if (effects->brightness.value < 1.0f) {
        dim_color = black;
        dim_color.alpha = 1.0f - effects->brightness.value;
        renderer_draw_filled_rect(whole_screen_area(), dim_color);
    }
}

static void draw_flash(const struct scene_effects *effects)
{
    struct renderer_color flash_color;

    if (effects->flash_level.value > 0.0f) {
        flash_color = white;
        flash_color.alpha = effects->flash_level.value;
        renderer_draw_filled_rect(whole_screen_area(), flash_color);
    }
}

void scene_view_start(void)
{
    unsigned char *pixels;

    iris_texture = RENDERER_NO_TEXTURE;
    pixels = mem_alloc(IRIS_TEXTURE_SIZE * IRIS_TEXTURE_SIZE * 4);
    if (pixels != NULL) {
        iris_pattern_fill(pixels, IRIS_TEXTURE_SIZE);
        iris_texture = renderer_texture_create(pixels, IRIS_TEXTURE_SIZE, IRIS_TEXTURE_SIZE);
        mem_free(pixels);
    }
    if (iris_texture == RENDERER_NO_TEXTURE) {
        run_log_write("scene iris_texture_failed");
    }
}

void scene_view_stop(void)
{
    if (iris_texture != RENDERER_NO_TEXTURE) {
        renderer_texture_destroy(iris_texture);
    }
    iris_texture = RENDERER_NO_TEXTURE;
}

static void draw_overlays(const struct scene_stage *stage, const struct screen_view *view)
{
    const struct scene_overlay *overlay;
    float width;
    float height;
    int index;

    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        overlay = &stage->layers.overlays[index];
        if (!overlay->in_use) {
            continue;
        }
        width = overlay->width * overlay->size_fraction;
        height = overlay->height * overlay->size_fraction;
        draw_texture_or_placeholder(overlay->texture_name, world_area_to_screen(view, overlay->shake_x + overlay->x + (overlay->width - width) / 2.0f, overlay->shake_y + overlay->y + (overlay->height - height) / 2.0f, width, height), whole_texture_area());
    }
}

static void draw_text_line(const struct scene_text_line *line)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    struct renderer_color tint;

    texture = texture_cache_use_or_start_loading(line->texture_name);
    if (texture == NULL || texture->state == CACHED_TEXTURE_MISSING) {
        area.x = line->center_x - MISSING_TEXT_MARKER_WIDTH / 2.0f;
        area.y = line->top_y;
        area.width = MISSING_TEXT_MARKER_WIDTH;
        area.height = MISSING_TEXT_MARKER_HEIGHT;
        renderer_draw_filled_rect(area, missing_texture_color);
        return;
    }
    if (texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    area.x = line->center_x - (float)texture->width / 2.0f;
    area.y = line->top_y;
    area.width = (float)texture->width;
    area.height = (float)texture->height;
    tint = white;
    tint.alpha = line->alpha;
    renderer_draw_sprite(texture->texture, area, whole_texture_area(), tint);
}

static void draw_text_lines(const struct scene_stage *stage)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (stage->layers.text_lines[index].in_use && stage->layers.text_lines[index].alpha > 0.0f) {
            draw_text_line(&stage->layers.text_lines[index]);
        }
    }
}

static void draw_world_layers(const struct scene_stage *stage)
{
    struct screen_view view;

    struct screen_view objects_view;

    view = view_of_stage(stage);
    objects_view = view_for_objects(stage, &view);
    draw_plates(stage, &view);
    draw_objects_in_layer_order(stage, &objects_view);
    draw_black_around_art(&view);
}

static void draw_layers_over_the_world(const struct scene_stage *stage)
{
    struct screen_view view;

    view = view_of_stage(stage);
    view = view_for_objects(stage, &view);
    draw_mask_or_iris(&stage->effects, &view);
    if (!stage->soul_mode_is_on) {
        draw_overlays(stage, &view);
    }
    draw_dim(&stage->effects);
}

static void draw_dissolve(const struct scene_effects *effects)
{
    struct renderer_rect area;
    float progress;
    float zoom;

    if (!effects->dissolve_is_on) {
        return;
    }
    progress = effects->dissolve_progress.value;
    zoom = 1.0f + (effects->dissolve_end_zoom - 1.0f) * progress;
    area.x = SCENE_VIEW_DISSOLVE_ANCHOR_X * (1.0f - zoom);
    area.y = SCENE_VIEW_DISSOLVE_ANCHOR_Y * (1.0f - zoom);
    area.width = RENDERER_WIDTH * zoom;
    area.height = RENDERER_HEIGHT * zoom;
    renderer_draw_snapshot(area, 1.0f - progress);
}

int scene_view_snapshot_stage(const struct scene_stage *stage)
{
    if (!renderer_snapshot_begin()) {
        return 0;
    }
    draw_world_layers(stage);
    draw_layers_over_the_world(stage);
    renderer_snapshot_end();
    return 1;
}

static int texture_is_still_loading(const char *texture_name)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(texture_name);
    return texture == NULL || texture->state == CACHED_TEXTURE_LOADING;
}

int scene_view_visible_art_is_loading(const struct scene_stage *stage)
{
    struct screen_view view;
    struct screen_view objects_view;
    char grayscale_name[ASSET_NAME_MAX];
    int is_loading;
    int index;

    is_loading = 0;
    view = view_of_stage(stage);
    objects_view = view_for_objects(stage, &view);
    for (index = 0; index < stage->plate_count; index++) {
        const struct scene_plate *plate;

        plate = &stage->plates[index];
        if (area_is_on_screen(world_area_to_screen(&view, plate->x, plate->y, plate->width, plate->height)) && texture_is_still_loading(plate_art_name(stage, plate, grayscale_name))) {
            is_loading = 1;
        }
    }
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        const struct scene_object *object;
        const struct animation_frame *frame;

        object = &stage->objects[index];
        if (!object->in_use) {
            continue;
        }
        frame = animation_player_current_frame(&object->player);
        if (frame == NULL || animation_frame_is_blank(frame)) {
            continue;
        }
        if (object_is_hidden(stage, object)) {
            continue;
        }
        if (area_is_on_screen(world_area_to_screen(&objects_view, object->is_flipped ? object->x - frame->x - frame->width : object->x + frame->x, object->y + frame->y, frame->width, frame->height)) && texture_is_still_loading(art_under_the_screen_filter(stage, frame->texture_name, grayscale_name))) {
            is_loading = 1;
        }
    }
    return is_loading;
}

void scene_view_screen_to_world(const struct scene_stage *stage, float screen_x, float screen_y, float *x, float *y)
{
    struct screen_view view;

    view = view_of_stage(stage);
    view = view_for_objects(stage, &view);
    *x = screen_x / VITA_PIXELS_PER_DS_PIXEL + view.left;
    *y = screen_y / VITA_PIXELS_PER_DS_PIXEL + view.top;
}

void scene_view_world_to_screen(const struct scene_stage *stage, float x, float y, float *screen_x, float *screen_y)
{
    struct screen_view view;

    view = view_of_stage(stage);
    view = view_for_objects(stage, &view);
    *screen_x = (x - view.left) * VITA_PIXELS_PER_DS_PIXEL;
    *screen_y = (y - view.top) * VITA_PIXELS_PER_DS_PIXEL;
}

void scene_view_forget_held_poses(void)
{
    memset(held_poses, 0, sizeof(held_poses));
}

void scene_view_draw_world(const struct scene_stage *stage)
{
    draw_world_layers(stage);
}

static struct renderer_color turned_for_the_ghost(struct renderer_color color, int ghost)
{
    struct renderer_color turned;

    turned = color;
    if (ghost == SCENE_GHOST_MISSILE) {
        turned.red = color.blue;
        turned.green = color.red;
        turned.blue = color.green;
    } else if (ghost == SCENE_GHOST_BOSS) {
        turned.red = color.green;
        turned.green = color.blue;
        turned.blue = color.red;
    }
    return turned;
}

static void draw_ghost_scanlines(const struct scene_stage *stage)
{
    static unsigned int sweep_frame;
    struct renderer_rect screen;
    struct renderer_rect rows[GHOST_SCANLINE_ROWS_MAX];
    struct renderer_rect sweep;
    struct renderer_color shade;
    struct renderer_color glow;
    int count;
    float y;

    if (!stage->soul_mode_is_on || (stage->soul_ghost != SCENE_GHOST_MISSILE && stage->soul_ghost != SCENE_GHOST_BOSS)) {
        return;
    }
    screen = whole_screen_area();
    count = 0;
    for (y = screen.y; y < screen.y + screen.height && count < GHOST_SCANLINE_ROWS_MAX; y = y + GHOST_SCANLINE_PITCH) {
        rows[count].x = screen.x;
        rows[count].y = y;
        rows[count].width = screen.width;
        rows[count].height = GHOST_SCANLINE_HEIGHT;
        count = count + 1;
    }
    shade = black;
    shade.alpha = GHOST_SCANLINE_ALPHA;
    renderer_draw_filled_rects(rows, count, shade);
    sweep_frame = sweep_frame + 1u;
    sweep.x = screen.x;
    sweep.width = screen.width;
    sweep.height = GHOST_SWEEP_HEIGHT;
    sweep.y = screen.y + (float)(sweep_frame % GHOST_SWEEP_FRAMES) * screen.height / (float)GHOST_SWEEP_FRAMES;
    glow = stage->soul_ghost == SCENE_GHOST_MISSILE ? missile_sweep_color : boss_sweep_color;
    renderer_draw_filled_rect(sweep, glow);
}

void scene_view_draw_covers(const struct scene_stage *stage)
{
    struct renderer_color fade_color;

    draw_ghost_scanlines(stage);
    draw_layers_over_the_world(stage);
    draw_dissolve(&stage->effects);
    draw_text_lines(stage);
    draw_flash(&stage->effects);
    if (stage->card_texture[0] != '\0') {
        draw_card(stage->card_texture);
    }
    if (stage->fade_amount > 0.0f) {
        fade_color = black;
        fade_color.alpha = stage->fade_amount;
        renderer_draw_filled_rect(whole_screen_area(), fade_color);
    }
}
