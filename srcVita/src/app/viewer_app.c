#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "asset_manifest.h"
#include "checker_pattern.h"
#include "clock.h"
#include "debug_font.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "music_engine.h"
#include "renderer.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_player.h"
#include "script_view.h"
#include "sound_engine.h"
#include "texture_cache.h"
#include "ui_list.h"
#include "viewer_app.h"

#define LIST_VISIBLE_ROWS 20
#define LIST_ROW_HEIGHT 22.0f
#define LIST_PANEL_X 40.0f
#define LIST_PANEL_Y 40.0f
#define LIST_PANEL_WIDTH 880.0f
#define LIST_PANEL_PADDING 16.0f
#define LIST_TEXT_X 52.0f
#define LIST_TEXT_Y 48.0f
#define HUD_BAR_HEIGHT 44.0f
#define HUD_TEXT_X 8.0f
#define HUD_FIRST_LINE_Y 6.0f
#define HUD_SECOND_LINE_Y 24.0f
#define TEXT_SCALE 2.0f
#define PLACEHOLDER_SIZE 64
#define PLACEHOLDER_TILE 8
#define UPLOADS_PER_FRAME 1
#define NO_PENDING_BACKGROUND -1
#define BROWSE_OBJECT_NAME "browse"
#define BROWSE_CENTRE_ACROSS_CAMERA 128.0f
#define BROWSE_CENTRE_DOWN_CAMERA 96.0f
#define BROWSE_OBJECT_LAYER 0
#define SCENE_END_FIRST_LINE "End of the chapter"
#define SCENE_END_SECOND_LINE "Press X to watch it again"
#define SCENE_END_FIRST_LINE_Y 240.0f
#define SCENE_END_SECOND_LINE_Y 280.0f
#define SCENE_END_TEXT_SCALE 3.0f
#define SCRIPT_ENTRY_NAME "route"

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color highlight = { 0.25f, 0.86f, 0.75f, 1.0f };
static const struct renderer_color panel = { 0.03f, 0.05f, 0.09f, 0.78f };

static char assets_folder[FILE_PATH_MAX];
static struct asset_manifest *manifest;
static struct ui_list animation_list;
static renderer_texture placeholder_texture;
static char status_text[VIEWER_STATUS_TEXT_MAX];
static int current_background_index;
static int pending_background_index;
static unsigned long long pending_background_started_us;
static int current_animation_index;
static int hud_is_visible;
static int list_is_open;
static int debug_controls_are_on;

static void asset_subfolder_path(char *path, const char *subfolder)
{
    file_path_join(path, assets_folder, subfolder, "");
}

static enum scene_playback_state playback_status(void)
{
    return script_player_is_active() ? script_player_status() : scene_player_status();
}

static int playback_is_paused(void)
{
    return script_player_is_active() ? script_player_is_paused() : scene_player_is_paused();
}

static void set_playback_paused(int should_pause)
{
    if (script_player_is_active()) {
        script_player_set_paused(should_pause);
    } else {
        scene_player_set_paused(should_pause);
    }
}

static void leave_scene_for_browsing(void)
{
    script_player_stop();
    scene_player_stop();
}

static void replace_room_with_background(const char *name)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(name);
    scene_stage_clear_room();
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        scene_stage_add_plate(name, 0.0f, 0.0f, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_DS_SCREEN_HEIGHT);
    } else {
        scene_stage_add_plate(name, 0.0f, 0.0f, (float)texture->width / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL, (float)texture->height / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    }
    scene_stage_set_camera(0.0f, 0.0f);
}

static const struct animation_frame *first_drawn_frame(const struct animation *animation)
{
    unsigned int index;

    for (index = 0; index < animation->frame_count; index++) {
        if (!animation_frame_is_blank(&animation->frames[index])) {
            return &animation->frames[index];
        }
    }
    return NULL;
}

static int show_browse_object(const char *name)
{
    const struct animation *animation;
    const struct animation_frame *frame;
    float x;
    float y;

    x = scene_stage_view()->camera_x + BROWSE_CENTRE_ACROSS_CAMERA;
    y = scene_stage_view()->camera_y + BROWSE_CENTRE_DOWN_CAMERA;
    animation = animation_library_load(name);
    if (animation != NULL) {
        frame = first_drawn_frame(animation);
        if (frame != NULL) {
            x = x - (frame->x + frame->width / 2.0f);
            y = y - (frame->y + frame->height / 2.0f);
        }
    }
    return scene_stage_set_object(BROWSE_OBJECT_NAME, name, x, y, BROWSE_OBJECT_LAYER, 0);
}

static void restore_browse_object(void)
{
    if (manifest->animation_count > 0) {
        show_browse_object(manifest->animation_names[current_animation_index]);
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
    leave_scene_for_browsing();
    texture_cache_drop_waiting_reads();
    if (animation_library_is_full()) {
        scene_stage_clear_room();
        animation_library_clear();
        if (manifest->background_count > 0) {
            replace_room_with_background(manifest->background_names[current_background_index]);
        }
    }
    if (!show_browse_object(name)) {
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
    leave_scene_for_browsing();
    pending_background_index = index;
    pending_background_started_us = clock_now_us();
    run_log_write("background selected name=%s", name);
    texture_cache_use_or_start_loading(name);
    return 1;
}

int viewer_app_play_scene(const char *name)
{
    pending_background_index = NO_PENDING_BACKGROUND;
    script_player_stop();
    return scene_player_play(name);
}

int viewer_app_play_script(const char *name)
{
    pending_background_index = NO_PENDING_BACKGROUND;
    scene_player_stop();
    script_player_play(name);
    return 1;
}

static void load_manifest(void)
{
    char path[FILE_PATH_MAX];
    unsigned char *text;
    unsigned int text_size;
    int result;

    if (!file_path_join(path, assets_folder, "manifest", ".txt")) {
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
    checker_pattern_fill(pixels, PLACEHOLDER_SIZE, PLACEHOLDER_TILE);
    placeholder_texture = renderer_texture_create(pixels, PLACEHOLDER_SIZE, PLACEHOLDER_SIZE);
    mem_free(pixels);
}

static void start_browsing_first_assets(void)
{
    if (manifest->background_count > 0) {
        texture_cache_load_now(manifest->background_names[0]);
        replace_room_with_background(manifest->background_names[0]);
    }
    if (manifest->animation_count > 0) {
        viewer_app_show_animation(manifest->animation_names[0]);
    }
}

static void play_opening(void)
{
    if (script_player_is_available()) {
        viewer_app_play_script(SCRIPT_ENTRY_NAME);
    } else {
        viewer_app_play_scene(manifest->scene_names[0]);
    }
}

void viewer_app_start(const char *folder)
{
    char texture_folder[FILE_PATH_MAX];
    char animation_folder[FILE_PATH_MAX];
    char scene_folder[FILE_PATH_MAX];
    char music_folder[FILE_PATH_MAX];
    char sound_folder[FILE_PATH_MAX];
    char script_folder[FILE_PATH_MAX];

    snprintf(assets_folder, sizeof(assets_folder), "%s", folder);
    asset_subfolder_path(texture_folder, "textures");
    asset_subfolder_path(animation_folder, "animations");
    asset_subfolder_path(scene_folder, "scenes");
    asset_subfolder_path(music_folder, "music");
    asset_subfolder_path(sound_folder, "sounds");
    asset_subfolder_path(script_folder, "scripts");
    manifest = NULL;
    current_background_index = 0;
    pending_background_index = NO_PENDING_BACKGROUND;
    current_animation_index = 0;
    hud_is_visible = 1;
    list_is_open = 0;
    debug_controls_are_on = 1;
    status_text[0] = '\0';

    texture_cache_init(texture_folder, TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    animation_library_init(animation_folder);
    scene_player_init(scene_folder);
    script_player_init(script_folder);
    music_engine_init(music_folder);
    sound_engine_init(sound_folder);
    if (!debug_font_init()) {
        run_log_write("font failed");
    }
    create_placeholder_texture();
    scene_view_start();
    load_manifest();
    if (manifest == NULL) {
        ui_list_init(&animation_list, 0, LIST_VISIBLE_ROWS);
        return;
    }
    ui_list_init(&animation_list, manifest->animation_count, LIST_VISIBLE_ROWS);
    if (script_player_is_available() || manifest->scene_count > 0) {
        hud_is_visible = 0;
        debug_controls_are_on = 0;
        play_opening();
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
    starting_from = current_background_index;
    if (pending_background_index != NO_PENDING_BACKGROUND) {
        starting_from = pending_background_index;
    }
    next = (starting_from + direction + manifest->background_count) % manifest->background_count;
    viewer_app_show_background(manifest->background_names[next]);
}

static void switch_to_pending_background_when_ready(void)
{
    const struct cached_texture *background;
    const char *name;

    if (pending_background_index == NO_PENDING_BACKGROUND) {
        return;
    }
    name = manifest->background_names[pending_background_index];
    background = texture_cache_use_or_start_loading(name);
    if (background == NULL || background->state == CACHED_TEXTURE_LOADING) {
        return;
    }
    current_background_index = pending_background_index;
    pending_background_index = NO_PENDING_BACKGROUND;
    replace_room_with_background(name);
    restore_browse_object();
    run_log_write("background shown name=%s wait_us=%llu", name, clock_now_us() - pending_background_started_us);
}

static void step_animation(int direction)
{
    int next;
    int tries;

    if (manifest == NULL || manifest->animation_count == 0) {
        return;
    }
    next = current_animation_index;
    for (tries = 0; tries < manifest->animation_count; tries++) {
        next = (next + direction + manifest->animation_count) % manifest->animation_count;
        if (viewer_app_show_animation(manifest->animation_names[next])) {
            return;
        }
    }
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

static int opening_can_play(void)
{
    return manifest != NULL && (manifest->scene_count > 0 || script_player_is_available());
}

static void set_debug_controls(int should_be_on)
{
    debug_controls_are_on = should_be_on;
    hud_is_visible = should_be_on;
    list_is_open = 0;
    run_log_write("debug controls %s", should_be_on ? "on" : "off");
    if (should_be_on) {
        return;
    }
    set_playback_paused(0);
    if (playback_status() == SCENE_PLAYER_IDLE && opening_can_play()) {
        play_opening();
    }
}

static void replay_what_finished(void)
{
    if (script_player_is_active()) {
        viewer_app_play_script(SCRIPT_ENTRY_NAME);
    } else {
        viewer_app_play_scene(manifest->scene_names[0]);
    }
}

static int axis_of(unsigned int buttons, unsigned int negative, unsigned int positive)
{
    if (buttons & positive) {
        return 1;
    }
    return (buttons & negative) ? -1 : 0;
}

static void send_script_directions(const struct input_state *input)
{
    int pressed_x;
    int pressed_y;

    pressed_x = axis_of(input->pressed, INPUT_LEFT, INPUT_RIGHT);
    pressed_y = axis_of(input->pressed, INPUT_UP, INPUT_DOWN);
    if (pressed_x != 0 || pressed_y != 0) {
        script_player_direction(pressed_x, pressed_y);
    }
    script_player_held_direction(axis_of(input->held, INPUT_LEFT, INPUT_RIGHT), axis_of(input->held, INPUT_UP, INPUT_DOWN));
}

static void update_player_controls(const struct input_state *input)
{
    if ((input->pressed & (INPUT_L | INPUT_R)) && (input->held & (INPUT_L | INPUT_R)) == (INPUT_L | INPUT_R)) {
        script_player_skip_ahead();
        return;
    }
    if (script_player_is_active()) {
        send_script_directions(input);
        if (input->pressed & (INPUT_TRIANGLE | INPUT_CIRCLE)) {
            script_player_ghost_button();
        }
        if (input->pressed & INPUT_L) {
            script_player_tap_balloon(0);
        }
        if (input->pressed & INPUT_R) {
            script_player_tap_balloon(1);
        }
    }
    if (!(input->pressed & INPUT_CROSS)) {
        return;
    }
    if (playback_status() == SCENE_PLAYER_FINISHED && opening_can_play()) {
        replay_what_finished();
        return;
    }
    if (script_player_is_active()) {
        script_player_press();
        return;
    }
    scene_player_skip_hold();
}

static void update_browse_controls(const struct input_state *input)
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
}

static void update_general_controls(const struct input_state *input)
{
    if (input->pressed & INPUT_CROSS) {
        set_playback_paused(!playback_is_paused());
    }
    if (input->pressed & INPUT_TRIANGLE) {
        hud_is_visible = !hud_is_visible;
    }
    if ((input->pressed & INPUT_R) && manifest != NULL && manifest->animation_count > 0) {
        list_is_open = 1;
    }
    if ((input->pressed & INPUT_L) && opening_can_play()) {
        play_opening();
    }
}

void viewer_app_update(const struct input_state *input, unsigned long long elapsed_us)
{
    texture_cache_upload_finished_reads(UPLOADS_PER_FRAME);
    texture_cache_begin_frame();
    if (input->pressed != 0) {
        run_log_write("input pressed buttons=0x%04x", input->pressed);
    }
    if (input->pressed & INPUT_SELECT) {
        set_debug_controls(!debug_controls_are_on);
    }
    if (list_is_open) {
        update_list(input);
    } else if (debug_controls_are_on) {
        update_browse_controls(input);
        update_general_controls(input);
    } else {
        update_player_controls(input);
    }
    switch_to_pending_background_when_ready();
    if (script_player_is_active()) {
        script_player_update(elapsed_us);
    } else {
        scene_player_update(elapsed_us);
    }
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

static void write_first_object_line(char *line, unsigned int line_size)
{
    const struct scene_stage *stage;
    const struct animation_player *player;
    int index;

    stage = scene_stage_view();
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        player = &stage->objects[index].player;
        if (stage->objects[index].in_use && player->animation != NULL) {
            snprintf(line, line_size, "%s %u/%u%s", player->animation->name, player->frame_index + 1, player->animation->frame_count, playback_is_paused() ? " paused" : "");
            return;
        }
    }
    snprintf(line, line_size, "no animation");
}

static void write_second_hud_line(char *line, unsigned int line_size)
{
    if (manifest == NULL) {
        snprintf(line, line_size, "no assets in %s", assets_folder);
        return;
    }
    if (playback_status() == SCENE_PLAYER_PRELOADING) {
        snprintf(line, line_size, "loading %d textures", texture_cache_loading_count());
        return;
    }
    if (script_player_is_active() && playback_status() == SCENE_PLAYER_RUNNING) {
        snprintf(line, line_size, "script tick %u%s", script_player_tick(), playback_is_paused() ? " paused" : "");
        return;
    }
    if (playback_status() == SCENE_PLAYER_RUNNING) {
        snprintf(line, line_size, "scene step %d%s", scene_player_next_step(), scene_player_is_paused() ? " paused" : "");
        return;
    }
    write_first_object_line(line, line_size);
}

static void draw_hud(void)
{
    struct renderer_rect bar;
    char line[FILE_PATH_MAX + ASSET_NAME_MAX];

    bar.x = 0.0f;
    bar.y = 0.0f;
    bar.width = RENDERER_WIDTH;
    bar.height = HUD_BAR_HEIGHT;
    renderer_draw_filled_rect(bar, panel);
    debug_font_draw_text(HUD_TEXT_X, HUD_FIRST_LINE_Y, TEXT_SCALE, highlight, status_text);
    write_second_hud_line(line, sizeof(line));
    debug_font_draw_text(HUD_TEXT_X, HUD_SECOND_LINE_Y, TEXT_SCALE, white, line);
}

static void draw_list(void)
{
    struct renderer_rect area;
    int item;
    int row;

    area.x = LIST_PANEL_X;
    area.y = LIST_PANEL_Y;
    area.width = LIST_PANEL_WIDTH;
    area.height = LIST_ROW_HEIGHT * LIST_VISIBLE_ROWS + LIST_PANEL_PADDING;
    renderer_draw_filled_rect(area, panel);
    for (item = 0; item < animation_list.item_count; item++) {
        row = ui_list_row_for_item(&animation_list, item);
        if (row >= 0) {
            debug_font_draw_text(LIST_TEXT_X, LIST_TEXT_Y + LIST_ROW_HEIGHT * row, TEXT_SCALE, item == animation_list.selected ? highlight : white, manifest->animation_names[item]);
        }
    }
}

static void draw_centred_scene_end_line(float y, const char *text)
{
    float width;

    width = (float)strlen(text) * DEBUG_FONT_CELL_WIDTH * SCENE_END_TEXT_SCALE;
    debug_font_draw_text(((float)RENDERER_WIDTH - width) / 2.0f, y, SCENE_END_TEXT_SCALE, white, text);
}

static void draw_game_picture(void)
{
    scene_view_draw_world(scene_stage_view());
    if (script_player_is_active()) {
        script_view_draw_on_world();
    }
    scene_view_draw_covers(scene_stage_view());
    if (script_player_is_active()) {
        script_view_draw_on_covers();
    }
}

void viewer_app_draw(void)
{
    if (manifest == NULL) {
        draw_no_assets_screen();
    } else if (playback_status() != SCENE_PLAYER_PRELOADING) {
        if (script_player_is_active() && script_view_wants_snapshot()) {
            if (renderer_snapshot_begin()) {
                draw_game_picture();
                renderer_snapshot_end();
            }
            script_view_snapshot_taken();
        }
        draw_game_picture();
    }
    if (playback_status() == SCENE_PLAYER_FINISHED && !debug_controls_are_on && !scene_layers_text_is_fading_out(scene_stage_layers())) {
        draw_centred_scene_end_line(SCENE_END_FIRST_LINE_Y, SCENE_END_FIRST_LINE);
        draw_centred_scene_end_line(SCENE_END_SECOND_LINE_Y, SCENE_END_SECOND_LINE);
    }
    if (hud_is_visible || playback_status() == SCENE_PLAYER_PRELOADING) {
        draw_hud();
    }
    if (list_is_open) {
        draw_list();
    }
}

void viewer_app_stop(void)
{
    script_player_shutdown();
    scene_player_shutdown();
    music_shutdown();
    sound_shutdown();
    animation_library_clear();
    texture_cache_clear();
    debug_font_shutdown();
    if (placeholder_texture != RENDERER_NO_TEXTURE) {
        renderer_texture_destroy(placeholder_texture);
    }
    placeholder_texture = RENDERER_NO_TEXTURE;
    scene_view_stop();
    mem_free(manifest);
    manifest = NULL;
}

int viewer_app_has_assets(void)
{
    return manifest != NULL;
}
