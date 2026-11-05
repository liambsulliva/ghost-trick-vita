#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "asset_manifest.h"
#include "debug_font.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "renderer.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_view.h"
#include "test_pattern.h"
#include "texture_cache.h"
#include "timing.h"
#include "ui_list.h"
#include "viewer_app.h"

#define LIST_VISIBLE_ROWS 20
#define LIST_ROW_HEIGHT 22.0f
#define PLACEHOLDER_SIZE 64
#define PLACEHOLDER_TILE 8
#define UPLOADS_PER_FRAME 1
#define NO_PENDING_BACKGROUND -1
#define BROWSE_OBJECT_NAME "browse"
#define BROWSE_OBJECT_X 128.0f
#define BROWSE_OBJECT_Y 176.0f
#define BROWSE_OBJECT_LAYER 0

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color highlight = { 0.25f, 0.86f, 0.75f, 1.0f };
static const struct renderer_color panel = { 0.03f, 0.05f, 0.09f, 0.78f };

static char assets_path[FILE_PATH_MAX];
static struct asset_manifest *manifest;
static struct ui_list animation_list;
static renderer_texture placeholder_texture;
static char status_text[VIEWER_STATUS_TEXT_MAX];
static int current_background;
static int pending_background;
static unsigned long long pending_background_started_us;
static int current_animation_index;
static int hud_is_visible;
static int list_is_open;

static void make_asset_subfolder(char *path, const char *subfolder)
{
    file_path_join(path, assets_path, subfolder, "");
}

static void put_background_in_room(const char *name)
{
    const struct cached_texture *texture;

    texture = texture_cache_request(name);
    scene_player_clear_room();
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        scene_player_add_plate(name, 0.0f, 0.0f, 256.0f, 192.0f);
    } else {
        scene_player_add_plate(name, 0.0f, 0.0f, (float)texture->width / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL, (float)texture->height / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    }
    scene_player_set_camera(0.0f, 0.0f);
}

static void restore_browse_object(void)
{
    if (manifest->animation_count > 0) {
        scene_player_set_object(BROWSE_OBJECT_NAME, manifest->animation_names[current_animation_index], BROWSE_OBJECT_X, BROWSE_OBJECT_Y, BROWSE_OBJECT_LAYER, 0);
    }
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
    scene_player_stop_script();
    if (!scene_player_set_object(BROWSE_OBJECT_NAME, name, BROWSE_OBJECT_X, BROWSE_OBJECT_Y, BROWSE_OBJECT_LAYER, 0)) {
        return 0;
    }
    current_animation_index = index;
    ui_list_select(&animation_list, index);
    run_log_write("animation selected name=%s", name);
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
    scene_player_stop_script();
    pending_background = index;
    pending_background_started_us = timing_now_us();
    run_log_write("background selected name=%s", name);
    texture_cache_request(name);
    return 1;
}

int viewer_app_play_scene(const char *name)
{
    pending_background = NO_PENDING_BACKGROUND;
    return scene_player_play(name);
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
    run_log_write("manifest loaded scenes=%d backgrounds=%d animations=%d", manifest->scene_count, manifest->background_count, manifest->animation_count);
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

static void start_browsing_first_assets(void)
{
    if (manifest->background_count > 0) {
        texture_cache_load_now(manifest->background_names[0]);
        put_background_in_room(manifest->background_names[0]);
    }
    if (manifest->animation_count > 0) {
        viewer_app_show_animation(manifest->animation_names[0]);
    }
}

void viewer_app_start(const char *assets_folder)
{
    char texture_folder[FILE_PATH_MAX];
    char animation_folder[FILE_PATH_MAX];
    char scene_folder[FILE_PATH_MAX];

    snprintf(assets_path, sizeof(assets_path), "%s", assets_folder);
    make_asset_subfolder(texture_folder, "textures");
    make_asset_subfolder(animation_folder, "animations");
    make_asset_subfolder(scene_folder, "scenes");
    manifest = NULL;
    current_background = 0;
    pending_background = NO_PENDING_BACKGROUND;
    current_animation_index = 0;
    hud_is_visible = 1;
    list_is_open = 0;
    status_text[0] = '\0';

    texture_cache_init(texture_folder, TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    animation_library_init(animation_folder);
    scene_player_init(scene_folder);
    if (!debug_font_init()) {
        run_log_write("font failed");
    }
    create_placeholder_texture();
    load_manifest();
    if (manifest == NULL) {
        ui_list_init(&animation_list, 0, LIST_VISIBLE_ROWS);
        return;
    }
    ui_list_init(&animation_list, manifest->animation_count, LIST_VISIBLE_ROWS);
    if (manifest->scene_count > 0) {
        hud_is_visible = 0;
        scene_player_play(manifest->scene_names[0]);
        return;
    }
    start_browsing_first_assets();
}

static void step_background(int direction)
{
    int starting_from;
    int next;

    if (manifest == NULL || manifest->background_count == 0) {
        return;
    }
    starting_from = current_background;
    if (pending_background != NO_PENDING_BACKGROUND) {
        starting_from = pending_background;
    }
    next = (starting_from + direction + manifest->background_count) % manifest->background_count;
    viewer_app_show_background(manifest->background_names[next]);
}

static void switch_to_pending_background_when_ready(void)
{
    const struct cached_texture *background;
    const char *name;

    if (pending_background == NO_PENDING_BACKGROUND) {
        return;
    }
    name = manifest->background_names[pending_background];
    background = texture_cache_request(name);
    if (background == NULL || background->state == CACHED_TEXTURE_LOADING) {
        return;
    }
    current_background = pending_background;
    pending_background = NO_PENDING_BACKGROUND;
    put_background_in_room(name);
    restore_browse_object();
    run_log_write("background shown name=%s wait_us=%llu", name, timing_now_us() - pending_background_started_us);
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
        scene_player_set_paused(!scene_player_is_paused());
    }
    if (input->pressed & INPUT_TRIANGLE) {
        hud_is_visible = !hud_is_visible;
    }
    if ((input->pressed & INPUT_R) && manifest != NULL && manifest->animation_count > 0) {
        list_is_open = 1;
    }
    if ((input->pressed & INPUT_L) && manifest != NULL && manifest->scene_count > 0) {
        viewer_app_play_scene(manifest->scene_names[0]);
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
    switch_to_pending_background_when_ready();
    scene_player_update(elapsed_us);
}

void viewer_app_set_status_text(const char *text)
{
    snprintf(status_text, sizeof(status_text), "%s", text);
}

void viewer_app_set_hud_visible(int is_visible)
{
    hud_is_visible = is_visible;
}

static void draw_no_assets_screen(void)
{
    struct renderer_rect screen_area;
    struct renderer_rect texture_area;

    screen_area.x = 0.0f;
    screen_area.y = 0.0f;
    screen_area.width = RENDERER_WIDTH;
    screen_area.height = RENDERER_HEIGHT;
    texture_area.x = 0.0f;
    texture_area.y = 0.0f;
    texture_area.width = 1.0f;
    texture_area.height = 1.0f;
    renderer_draw_sprite(placeholder_texture, screen_area, texture_area, white);
}

static void write_second_hud_line(char *line, unsigned int line_size)
{
    const struct scene_picture *picture;
    int index;

    if (manifest == NULL) {
        snprintf(line, line_size, "no assets in %s", assets_path);
        return;
    }
    if (scene_player_status() == SCENE_PLAYER_PRELOADING) {
        snprintf(line, line_size, "loading %d textures", texture_cache_loading_count());
        return;
    }
    if (scene_player_status() == SCENE_PLAYER_RUNNING) {
        snprintf(line, line_size, "scene step %d", scene_player_current_step());
        return;
    }
    picture = scene_player_picture();
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (picture->objects[index].in_use && picture->objects[index].player.animation != NULL) {
            snprintf(line, line_size, "%s %u/%u%s", picture->objects[index].player.animation->name, picture->objects[index].player.frame_index + 1, picture->objects[index].player.animation->frame_count, scene_player_is_paused() ? " paused" : "");
            return;
        }
    }
    snprintf(line, line_size, "no animation");
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
    write_second_hud_line(line, sizeof(line));
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
    if (manifest == NULL) {
        draw_no_assets_screen();
    } else if (scene_player_status() != SCENE_PLAYER_PRELOADING) {
        scene_view_draw(scene_player_picture());
    }
    if (hud_is_visible || scene_player_status() == SCENE_PLAYER_PRELOADING) {
        draw_hud();
    }
    if (list_is_open) {
        draw_list();
    }
}

void viewer_app_stop(void)
{
    scene_player_shutdown();
    animation_library_clear();
    texture_cache_clear();
    debug_font_shutdown();
    if (placeholder_texture != RENDERER_NO_TEXTURE) {
        renderer_texture_destroy(placeholder_texture);
    }
    placeholder_texture = RENDERER_NO_TEXTURE;
    mem_free(manifest);
    manifest = NULL;
}

int viewer_app_has_assets(void)
{
    return manifest != NULL;
}
