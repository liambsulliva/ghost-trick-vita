#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "asset_manifest.h"
#include "debug_font.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "renderer.h"
#include "run_log.h"
#include "test_pattern.h"
#include "texture_cache.h"
#include "timing.h"
#include "ui_list.h"
#include "viewer_app.h"

#define SCENE_REFERENCE_WIDTH 1920.0f
#define SCENE_REFERENCE_HEIGHT 1080.0f
#define SCENE_SCREEN_WIDTH 960.0f
#define SCENE_SCREEN_HEIGHT 540.0f
#define SCENE_SCREEN_TOP 2.0f
#define LIST_VISIBLE_ROWS 20
#define LIST_ROW_HEIGHT 22.0f
#define PLACEHOLDER_SIZE 64
#define PLACEHOLDER_TILE 8
#define UPLOADS_PER_FRAME 1

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color highlight = { 0.25f, 0.86f, 0.75f, 1.0f };
static const struct renderer_color panel = { 0.03f, 0.05f, 0.09f, 0.78f };
static const struct renderer_color missing_texture_color = { 1.0f, 0.0f, 1.0f, 0.6f };

static char assets_path[FILE_PATH_MAX];
static struct asset_manifest *manifest;
static struct animation *current_animation;
static struct animation_player player;
static struct ui_list animation_list;
static renderer_texture placeholder_texture;
static char status_text[VIEWER_STATUS_TEXT_MAX];
static int current_background;
static int current_animation_index;
static int hud_is_visible;
static int list_is_open;

static struct renderer_rect whole_texture_area(void)
{
    struct renderer_rect area;

    area.x = 0.0f;
    area.y = 0.0f;
    area.width = 1.0f;
    area.height = 1.0f;
    return area;
}

static struct renderer_rect scene_area_to_screen(float x, float y, float width, float height)
{
    struct renderer_rect area;
    float scale_x;
    float scale_y;

    scale_x = SCENE_SCREEN_WIDTH / SCENE_REFERENCE_WIDTH;
    scale_y = SCENE_SCREEN_HEIGHT / SCENE_REFERENCE_HEIGHT;
    area.x = x * scale_x;
    area.y = SCENE_SCREEN_TOP + y * scale_y;
    area.width = width * scale_x;
    area.height = height * scale_y;
    return area;
}

static void log_animation_coverage(void)
{
    if (current_animation == NULL || player.animation == NULL) {
        return;
    }
    run_log_write("animation coverage name=%s frames_shown=%u frames_total=%u late_frames=%u", current_animation->name, animation_player_count_shown_frames(&player), current_animation->frame_count, player.late_frame_count);
}

static int texture_will_never_block_frame(const struct cached_texture *texture)
{
    return texture == NULL || texture->state != CACHED_TEXTURE_LOADING;
}

static int animation_frame_is_ready(const struct animation_frame *frame)
{
    return texture_will_never_block_frame(texture_cache_request(frame->texture_name));
}

static int load_animation_file(const char *name)
{
    char animation_folder[FILE_PATH_MAX];
    char path[FILE_PATH_MAX];
    unsigned char *text;
    unsigned int text_size;
    unsigned long long started_us;
    unsigned int frame;
    int result;

    if (!file_path_join(animation_folder, assets_path, "animations", "") || !file_path_join(path, animation_folder, name, ".anim")) {
        run_log_write("animation path_too_long name=%s", name);
        return 0;
    }
    result = file_read_whole(path, &text, &text_size);
    if (result != FILE_OK) {
        run_log_write("animation missing name=%s code=%d", name, result);
        return 0;
    }
    result = animation_parse((const char *)text, text_size, current_animation);
    mem_free(text);
    if (result != ANIMATION_OK) {
        run_log_write("animation invalid name=%s code=%d", name, result);
        return 0;
    }

    started_us = timing_now_us();
    texture_cache_load_now(current_animation->frames[0].texture_name);
    for (frame = 1; frame < current_animation->frame_count; frame++) {
        texture_cache_request(current_animation->frames[frame].texture_name);
    }
    run_log_write("animation loaded name=%s frames=%u first_frame_us=%llu", name, current_animation->frame_count, timing_now_us() - started_us);
    return 1;
}

int viewer_app_show_animation(const char *name)
{
    int index;

    if (manifest == NULL) {
        return 0;
    }
    index = asset_manifest_find_animation(manifest, name);
    if (index < 0) {
        run_log_write("animation not_in_manifest name=%s", name);
        return 0;
    }
    log_animation_coverage();
    player.animation = NULL;
    if (!load_animation_file(name)) {
        return 0;
    }
    current_animation_index = index;
    animation_player_start(&player, current_animation);
    ui_list_select(&animation_list, index);
    return 1;
}

int viewer_app_show_background(const char *name)
{
    int index;

    if (manifest == NULL) {
        return 0;
    }
    index = asset_manifest_find_background(manifest, name);
    if (index < 0) {
        run_log_write("background not_in_manifest name=%s", name);
        return 0;
    }
    current_background = index;
    run_log_write("background selected name=%s", name);
    texture_cache_load_now(name);
    return 1;
}

static void load_manifest(void)
{
    char path[FILE_PATH_MAX];
    unsigned char *text;
    unsigned int text_size;
    int result;

    if (!file_path_join(path, assets_path, "manifest", ".txt")) {
        run_log_write("manifest path_too_long");
        return;
    }
    result = file_read_whole(path, &text, &text_size);
    if (result != FILE_OK) {
        run_log_write("manifest missing path=%s code=%d", path, result);
        return;
    }
    manifest = mem_alloc(sizeof(struct asset_manifest));
    if (manifest == NULL) {
        mem_free(text);
        run_log_write("manifest out_of_memory");
        return;
    }
    result = asset_manifest_parse((const char *)text, text_size, manifest);
    mem_free(text);
    if (result != ASSET_MANIFEST_OK) {
        run_log_write("manifest invalid code=%d", result);
        mem_free(manifest);
        manifest = NULL;
        return;
    }
    run_log_write("manifest loaded backgrounds=%d animations=%d", manifest->background_count, manifest->animation_count);
}

static void create_placeholder_texture(void)
{
    unsigned char *pixels;

    pixels = mem_alloc(PLACEHOLDER_SIZE * PLACEHOLDER_SIZE * 4);
    if (pixels == NULL) {
        return;
    }
    test_pattern_fill_checker(pixels, PLACEHOLDER_SIZE, PLACEHOLDER_TILE);
    placeholder_texture = renderer_texture_create(pixels, PLACEHOLDER_SIZE, PLACEHOLDER_SIZE);
    mem_free(pixels);
}

void viewer_app_start(const char *assets_folder)
{
    char texture_folder[FILE_PATH_MAX];

    snprintf(assets_path, sizeof(assets_path), "%s", assets_folder);
    file_path_join(texture_folder, assets_folder, "textures", "");
    manifest = NULL;
    player.animation = NULL;
    current_background = 0;
    current_animation_index = 0;
    hud_is_visible = 1;
    list_is_open = 0;
    status_text[0] = '\0';

    texture_cache_init(texture_folder, TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    if (!debug_font_init()) {
        run_log_write("font failed");
    }
    create_placeholder_texture();
    current_animation = mem_alloc(sizeof(struct animation));
    if (current_animation == NULL) {
        run_log_write("animation out_of_memory");
        return;
    }
    load_manifest();
    if (manifest == NULL) {
        ui_list_init(&animation_list, 0, LIST_VISIBLE_ROWS);
        return;
    }
    ui_list_init(&animation_list, manifest->animation_count, LIST_VISIBLE_ROWS);
    if (manifest->background_count > 0) {
        viewer_app_show_background(manifest->background_names[0]);
    }
    if (manifest->animation_count > 0) {
        viewer_app_show_animation(manifest->animation_names[0]);
    }
}

static void step_background(int direction)
{
    int next;

    if (manifest == NULL || manifest->background_count == 0) {
        return;
    }
    next = (current_background + direction + manifest->background_count) % manifest->background_count;
    viewer_app_show_background(manifest->background_names[next]);
}

static void step_animation(int direction)
{
    int next;

    if (manifest == NULL || manifest->animation_count == 0) {
        return;
    }
    next = (current_animation_index + direction + manifest->animation_count) % manifest->animation_count;
    viewer_app_show_animation(manifest->animation_names[next]);
}

static void update_list(const struct input_state *input)
{
    if (input->pressed & INPUT_UP) {
        ui_list_move_selection(&animation_list, -1);
    }
    if (input->pressed & INPUT_DOWN) {
        ui_list_move_selection(&animation_list, 1);
    }
    if (input->pressed & INPUT_CROSS) {
        viewer_app_show_animation(manifest->animation_names[animation_list.selected]);
        list_is_open = 0;
    }
    if (input->pressed & (INPUT_CIRCLE | INPUT_R)) {
        list_is_open = 0;
    }
}

static void update_scene_controls(const struct input_state *input)
{
    if (input->pressed & INPUT_LEFT) {
        step_background(-1);
    }
    if (input->pressed & INPUT_RIGHT) {
        step_background(1);
    }
    if (input->pressed & INPUT_UP) {
        step_animation(-1);
    }
    if (input->pressed & INPUT_DOWN) {
        step_animation(1);
    }
    if (input->pressed & INPUT_CROSS) {
        player.is_paused = !player.is_paused;
    }
    if (input->pressed & INPUT_TRIANGLE) {
        hud_is_visible = !hud_is_visible;
    }
    if ((input->pressed & INPUT_R) && manifest != NULL && manifest->animation_count > 0) {
        list_is_open = 1;
    }
}

void viewer_app_update(const struct input_state *input, unsigned long long elapsed_us)
{
    texture_cache_begin_frame();
    texture_cache_upload_finished_reads(UPLOADS_PER_FRAME);
    if (list_is_open) {
        update_list(input);
    } else {
        update_scene_controls(input);
    }
    animation_player_advance(&player, elapsed_us, animation_frame_is_ready);
}

void viewer_app_set_status_text(const char *text)
{
    snprintf(status_text, sizeof(status_text), "%s", text);
}

static void draw_background(void)
{
    const struct cached_texture *background;
    struct renderer_rect whole_screen;

    whole_screen = scene_area_to_screen(0.0f, 0.0f, SCENE_REFERENCE_WIDTH, SCENE_REFERENCE_HEIGHT);
    if (manifest == NULL || manifest->background_count == 0) {
        renderer_draw_sprite(placeholder_texture, whole_screen, whole_texture_area(), white);
        return;
    }
    background = texture_cache_request(manifest->background_names[current_background]);
    if (background == NULL || background->state == CACHED_TEXTURE_MISSING) {
        renderer_draw_filled_rect(whole_screen, missing_texture_color);
        return;
    }
    if (background->state == CACHED_TEXTURE_READY) {
        renderer_draw_sprite(background->texture, whole_screen, whole_texture_area(), white);
    }
}

static void draw_animation_frame(void)
{
    const struct animation_frame *frame;
    const struct cached_texture *texture;
    struct renderer_rect area;

    frame = animation_player_current_frame(&player);
    if (frame == NULL) {
        return;
    }
    area = scene_area_to_screen(frame->x, frame->y, frame->width, frame->height);
    texture = texture_cache_request(frame->texture_name);
    if (texture == NULL || texture->state == CACHED_TEXTURE_MISSING) {
        renderer_draw_filled_rect(area, missing_texture_color);
        return;
    }
    if (texture->state == CACHED_TEXTURE_READY) {
        renderer_draw_sprite(texture->texture, area, whole_texture_area(), white);
    }
}

static void draw_hud(void)
{
    struct renderer_rect bar;
    char line[FILE_PATH_MAX + ANIMATION_NAME_MAX];

    bar.x = 0.0f;
    bar.y = 0.0f;
    bar.width = RENDERER_WIDTH;
    bar.height = 44.0f;
    renderer_draw_filled_rect(bar, panel);
    debug_font_draw_text(8.0f, 6.0f, 2.0f, highlight, status_text);
    if (player.animation != NULL) {
        snprintf(line, sizeof(line), "%s %u/%u%s", player.animation->name, player.frame_index + 1, player.animation->frame_count, player.is_paused ? " paused" : "");
    } else if (manifest == NULL) {
        snprintf(line, sizeof(line), "no assets in %s", assets_path);
    } else {
        snprintf(line, sizeof(line), "no animation");
    }
    debug_font_draw_text(8.0f, 24.0f, 2.0f, white, line);
}

static void draw_list(void)
{
    struct renderer_rect area;
    int item;
    int row;

    area.x = 40.0f;
    area.y = 40.0f;
    area.width = 880.0f;
    area.height = LIST_ROW_HEIGHT * LIST_VISIBLE_ROWS + 16.0f;
    renderer_draw_filled_rect(area, panel);
    for (item = 0; item < animation_list.item_count; item++) {
        row = ui_list_row_for_item(&animation_list, item);
        if (row >= 0) {
            debug_font_draw_text(52.0f, 48.0f + LIST_ROW_HEIGHT * row, 2.0f, item == animation_list.selected ? highlight : white, manifest->animation_names[item]);
        }
    }
}

void viewer_app_draw(void)
{
    draw_background();
    draw_animation_frame();
    if (hud_is_visible) {
        draw_hud();
    }
    if (list_is_open) {
        draw_list();
    }
}

void viewer_app_stop(void)
{
    log_animation_coverage();
    texture_cache_clear();
    debug_font_shutdown();
    if (placeholder_texture != RENDERER_NO_TEXTURE) {
        renderer_texture_destroy(placeholder_texture);
    }
    placeholder_texture = RENDERER_NO_TEXTURE;
    mem_free(manifest);
    manifest = NULL;
    mem_free(current_animation);
    current_animation = NULL;
    player.animation = NULL;
}

int viewer_app_has_assets(void)
{
    return manifest != NULL;
}
