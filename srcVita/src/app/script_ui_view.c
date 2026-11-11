#include <math.h>
#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_ui_view.h"
#include "texture_cache.h"

#define PLAYING_CLIP_MAX 4
#define DEGREES_TO_RADIANS 0.017453293f
#define BAND_CENTER_Y 272.0f
#define BAND_STRIPE_COUNT 29
#define REVEAL_STRIP_COUNT 32
#define REVEAL_RAMP_WIDTH 250.0f
#define HOURGLASS_TEXTURE "ui_hourglass"
#define HOURGLASS_TEXTURE_WIDTH 480.0f
#define HOURGLASS_TEXTURE_HEIGHT 1024.0f
#define HOURGLASS_LEVEL_COUNT 6
#define HOURGLASS_TOP_BOTTOM_EDGE -146.0f
#define HOURGLASS_BOTTOM_BOTTOM_EDGE 466.0f
#define HOURGLASS_LAST_LEVEL 1
#define HOURGLASS_FUNNEL_HEIGHT 168.0f
#define HOURGLASS_FUNNEL_CLIP_AT_LAST 50.0f
#define HOURGLASS_STREAM_WIDTH 28.8f
#define HOURGLASS_STREAM_HEIGHT 441.5f
#define HOURGLASS_STREAM_SPACING 418.0f
#define HOURGLASS_STREAM_TOP 15.0f
#define HOURGLASS_STREAM_FADE_END 135.0f
#define HOURGLASS_STREAM_STRIP_COUNT 12
#define HOURGLASS_STREAM_UNITS_PER_TICK 3.4833f
#define SIDE_HOURGLASS_X 900.0f
#define SIDE_HOURGLASS_Y 272.0f
#define SIDE_HOURGLASS_SCALE 0.225f
#define SIDE_DIM 0.4f
#define SIDE_LABEL_TEXTURE "side_label"
#define SIDE_LABEL_X 60.0f
#define SIDE_LABEL_Y 272.0f
#define SIDE_LABEL_LENGTH 312.0f
#define SIDE_LABEL_THICKNESS 39.5f
#define SIDE_LABEL_ROTATION -90.0f

#define REWIND_ROCK_X 405.0f
#define REWIND_ROCK_WIDTH 150.0f
#define REWIND_ROCK_HEIGHT 900.0f
#define REWIND_FAR_SPEED 0.75f
#define REWIND_MID_SPEED 1.25f
#define REWIND_NEAR_SPEED 1.875f
#define REWIND_BUTTON_TEXTURE "ui_select_button"
#define REWIND_FAR_ROCKS_TEXTURE "rewind_rocks_far"
#define REWIND_MID_ROCKS_TEXTURE "rewind_rocks_mid"
#define REWIND_NEAR_ROCKS_TEXTURE "rewind_rocks_near"
#define REWIND_BUTTON_X 331.0f
#define REWIND_BUTTON_Y 238.75f
#define REWIND_BUTTON_WIDTH 298.0f
#define REWIND_BUTTON_HEIGHT 69.0f
#define REWIND_BUTTON_CORNER 20.0f
#define REWIND_BUTTON_CORNER_UV 0.345f
#define REWIND_SLIDE_TICKS 20
#define REWIND_SLIDE_PIXELS 330.0f
#define REWIND_PRESS_FLASH_TICKS 12
#define REWIND_RESTING_BRIGHTNESS 0.88f
#define REWIND_PRESS_FADE_TICKS 60
#define REWIND_LABEL_WIDTH 210.0f
#define REWIND_LABEL_HEIGHT 36.0f
#define REWIND_LABEL_SHEET_Y 112.0f
#define REWIND_LABEL_SHEET_WIDTH 420.0f
#define REWIND_LABEL_SHEET_HEIGHT 72.0f
#define TOUCH_SLOP_PIXELS 6.0f
#define TAP_BALLOON_MAX 8
#define TAP_BALLOON_KEY_COUNT 5
#define TAP_BALLOON_CLOSE_TICKS 12
#define TAP_BALLOON_CLOSE_WIDEN 0.2f
#define TAP_ART_TEXTURE "ui_tap_balloon"
#define TAP_ART_WIDTH 512.0f
#define TAP_ART_HEIGHT 256.0f
#define TAP_OVAL_CELL_X 2.0f
#define TAP_OVAL_CELL_Y 2.0f
#define TAP_OVAL_CELL_WIDTH 225.0f
#define TAP_OVAL_CELL_HEIGHT 156.0f
#define TAP_OVAL_WITH_TAIL_CELL_HEIGHT 216.0f
#define TAP_OVAL_WIDTH 139.0f
#define TAP_OVAL_HEIGHT 74.0f
#define TAP_OVAL_WITH_TAIL_HEIGHT 124.0f
#define TAP_MARK_CELL_X 230.0f
#define TAP_MARK_CELL_Y 113.0f
#define TAP_MARK_CELL_SIZE 46.0f
#define TAP_MARK_SIZE 25.0f
#define TAP_MARK_SPACING 36.0f
#define TAP_MARKS_X 480.0f
#define TAP_MARKS_Y 52.5f
#define TAP_DOT_SIZE 12.0f
#define TAP_DOT_SPACING 28.0f
#define TAP_DOTS_ABOVE_MIDDLE 25.0f
#define TAP_DOTS_ABOVE_ANCHOR -66.0f
#define TAP_CONTENT_SHOWS_AT 10
#define TAP_GLYPH_RIGHT_OF_MIDDLE 34.0f
#define TAP_MARKS_GLYPH_BELOW_MIDDLE 20.0f
#define TAP_DOTS_GLYPH_BELOW_MIDDLE -8.0f
#define TAP_SQUARE_GLYPH_X 80.0f
#define TAP_TRIANGLE_GLYPH_X 120.0f
#define TAP_GLYPH_CELL 40.0f
#define TAP_GLYPH_SIZE 28.0f
#define NARRATION_ARROW_TEXTURE "narration_arrow"
#define NARRATION_ARROW_X 459.0f
#define NARRATION_ARROW_Y 504.0f
#define NARRATION_ARROW_WIDTH 41.0f
#define NARRATION_ARROW_HEIGHT 19.5f
#define NARRATION_ARROW_BOB_TICKS 50
#define NARRATION_ARROW_BOB_PIXELS 5.0f
#define PILL_ART_TEXTURE "hud_button_art"
#define PILL_SHEET_TEXTURE "hud_sheet"
#define PILL_ART_SIZE 512.0f
#define PILL_SHEET_WIDTH 512.0f
#define PILL_SHEET_HEIGHT 256.0f
#define PILL_LABEL_CELL_WIDTH 160.0f
#define PILL_SLIDE_STEPS 8
#define PILL_SLIDE_PIXELS_PER_STEP 11.25f
#define PILL_PRESSED_DROP 3.75f
#define PILL_PRESSED_GREEN 0.5216f
#define PILL_PULSE_PERIOD 80
#define PILL_PULSE_PHASE_SHIFT 70
#define PILL_PULSE_FALL_TICKS 30
#define PILL_PULSE_RISE_ENDS_AT 59
#define PILL_PULSE_DEPTH 0.1686f
#define PILL_RING_HEIGHT 37.5f
#define PILL_RING_TEXELS 78.0f
#define PILL_RING_CELL_Y 101.0f
#define PILL_RING_PRESSED_CELL_Y 180.0f
#define PILL_RING_LEFT_CELL_X 161.0f
#define PILL_RING_MIDDLE_CELL_X 211.0f
#define PILL_RING_RIGHT_CELL_X 221.0f
#define PILL_RING_CAP_TEXELS 60.0f
#define PILL_RING_MIDDLE_TEXELS 20.0f
#define PILL_RING_CAP_WIDTH 28.86f
#define PILL_RING_THICKNESS 7.7f
#define PILL_FILL_TOP 7.7f
#define PILL_FILL_HEIGHT 18.3f
#define PILL_ICON_SIZE 38.25f
#define PILL_ICON_TEXELS 80.0f
#define LARGE_ICON_TEXELS 92.0f
#define CHARACTER_PILL_WIDTH 69.75f
#define CHARACTER_PILL_X (480.0f - CHARACTER_PILL_WIDTH / 2.0f)
#define PILL_LABEL_WIDTH 80.0f
#define PILL_LABEL_HEIGHT 32.0f
#define PILL_LABEL_NUDGE_Y -2.0f
#define PILL_GLYPH_SIZE 20.0f
#define PILL_GLYPH_NUDGE_Y -2.0f
#define PILL_BOTTOM_Y 494.0f
#define PILL_TOP_Y 12.5f

struct tap_balloon {
    int is_open;
    int kind;
    int age_ticks;
    int closing_ticks;
    float anchor_x;
    float anchor_y;
};

struct pill_sheet_cell {
    float x;
    float y;
    float width;
    float height;
};

struct pill_layout {
    float x;
    float y;
    float width;
    int is_at_bottom;
    float fill_left;
    float fill_right;
    float icon_x;
    float icon_y;
    float icon_cell_x;
    float icon_cell_y;
    const struct pill_sheet_cell *label;
    float label_center_x;
    const struct pill_sheet_cell *glyph;
    float glyph_x;
    float glyph_width;
    float icon_texels;
};

static const struct pill_sheet_cell glyph_cross = { 0.0f, 0.0f, 40.0f, 40.0f };
static const struct pill_sheet_cell glyph_l = { 160.0f, 0.0f, 40.0f, 40.0f };
static const struct pill_sheet_cell glyph_r = { 200.0f, 0.0f, 40.0f, 40.0f };
static const struct pill_sheet_cell glyph_triangle = { 120.0f, 0.0f, 40.0f, 40.0f };
static const struct pill_sheet_cell glyph_select = { 312.0f, 0.0f, 84.0f, 40.0f };
static const struct pill_sheet_cell label_ghost = { 0.0f, 48.0f, 160.0f, 64.0f };
static const struct pill_sheet_cell label_trick = { 160.0f, 48.0f, 160.0f, 64.0f };
static const struct pill_sheet_cell label_back = { 320.0f, 48.0f, 160.0f, 64.0f };
static const struct pill_sheet_cell label_swap = { 0.0f, 192.0f, 224.0f, 64.0f };
static const struct pill_sheet_cell label_unlock = { 224.0f, 192.0f, 224.0f, 64.0f };

static const struct pill_layout pill_layouts[SCRIPT_PILL_COUNT] = {
    [SCRIPT_PILL_GHOST] = { 9.75f, PILL_BOTTOM_Y, 201.0f, 1, 29.25f, 165.0f, 163.5f, -2.25f, 0.0f, 101.0f, &label_ghost, 96.4f, &glyph_l, 9.5f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_BACK] = { 9.75f, PILL_BOTTOM_Y, 139.5f, 1, 29.25f, 132.0f, 0.0f, 0.0f, -1.0f, 0.0f, &label_back, 80.25f, &glyph_l, 9.5f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_TRICK] = { 750.75f, PILL_BOTTOM_Y, 199.5f, 1, 33.75f, 169.5f, -2.25f, -1.5f, 80.0f, 101.0f, &label_trick, 103.1f, &glyph_cross, 170.0f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_HOURGLASS] = { 880.5f, PILL_TOP_Y, 69.75f, 0, 30.0f, 41.0f, 0.0f, -1.0f, 0.0f, 181.0f, NULL, 0.0f, &glyph_r, 40.5f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_NOTEBOOK] = { 9.75f, PILL_TOP_Y, 86.0f, 0, 41.0f, 50.0f, 47.75f, -1.0f, 80.0f, 181.0f, NULL, 0.0f, &glyph_select, 4.0f, 42.0f, 0.0f },
    [SCRIPT_PILL_JOWD] = { CHARACTER_PILL_X, PILL_BOTTOM_Y, CHARACTER_PILL_WIDTH, 1, 30.0f, 41.0f, 0.0f, -1.0f, 0.0f, 261.0f, NULL, 0.0f, &glyph_triangle, 40.5f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_MISSILE] = { CHARACTER_PILL_X, PILL_BOTTOM_Y, CHARACTER_PILL_WIDTH, 1, 30.0f, 41.0f, 0.0f, -1.0f, 321.0f, 0.0f, NULL, 0.0f, &glyph_triangle, 40.5f, PILL_GLYPH_SIZE, LARGE_ICON_TEXELS },
    [SCRIPT_PILL_EXCHANGE] = { 750.75f, PILL_BOTTOM_Y, 199.5f, 1, 33.75f, 169.5f, 0.0f, 0.0f, -1.0f, 0.0f, &label_swap, 103.1f, &glyph_cross, 170.0f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_UNLOCK] = { 9.75f, PILL_BOTTOM_Y, 170.0f, 1, 29.25f, 162.5f, 0.0f, 0.0f, -1.0f, 0.0f, &label_unlock, 95.5f, &glyph_l, 9.5f, PILL_GLYPH_SIZE, 0.0f },
    [SCRIPT_PILL_CICEL] = { CHARACTER_PILL_X, PILL_BOTTOM_Y, CHARACTER_PILL_WIDTH, 1, 30.0f, 41.0f, 0.0f, -1.0f, 322.0f, 88.0f, NULL, 0.0f, &glyph_triangle, 40.5f, PILL_GLYPH_SIZE, LARGE_ICON_TEXELS },
};

struct hourglass_cell {
    float x;
    float y;
    float width;
    float height;
    float gui_width;
    float gui_height;
    float gui_top;
    float alpha;
    int flips_y;
};

struct playing_clip {
    const struct ui_clip *clip;
    int age_ticks;
    int level_from;
    int level_to;
};

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static int arrow_age_ticks;
static struct tap_balloon tap_balloons[TAP_BALLOON_MAX];
static const float tap_balloon_key_frames[TAP_BALLOON_KEY_COUNT] = { 0.0f, 5.0f, 10.0f, 15.0f, 19.0f };
static const float tap_balloon_widths[TAP_BALLOON_KEY_COUNT] = { 1.24f, 0.66f, 1.24f, 0.77f, 1.0f };
static const float tap_balloon_heights[TAP_BALLOON_KEY_COUNT] = { 0.0f, 1.42f, 0.64f, 1.21f, 1.0f };

static const struct hourglass_cell glass_cells[] = {
    { 258.0f, 3.0f, 210.0f, 32.0f, 210.0f, 32.0f, 467.0f, 0.5f, 1 },
    { 259.0f, 33.0f, 210.0f, 1.0f, 210.0f, 319.0f, 148.0f, 0.5f, 0 },
    { 258.0f, 33.0f, 210.0f, 120.0f, 210.0f, 120.0f, 28.0f, 0.5f, 1 },
    { 258.0f, 152.0f, 210.0f, 1.0f, 210.0f, 55.0f, -27.0f, 0.5f, 0 },
    { 258.0f, 33.0f, 210.0f, 120.0f, 210.0f, 120.0f, -147.0f, 0.5f, 0 },
    { 259.0f, 33.0f, 210.0f, 1.0f, 210.0f, 320.0f, -467.0f, 0.5f, 0 },
    { 258.0f, 3.0f, 210.0f, 32.0f, 210.0f, 32.0f, -499.0f, 0.5f, 0 },
};

static const struct hourglass_cell outline_cells[] = {
    { 3.0f, 359.0f, 250.0f, 54.0f, 250.0f, 54.0f, 466.0f, 1.0f, 0 },
    { 3.0f, 359.0f, 250.0f, 1.0f, 250.0f, 302.0f, 164.0f, 1.0f, 0 },
    { 3.0f, 211.0f, 250.0f, 148.0f, 250.0f, 148.0f, 16.0f, 1.0f, 0 },
    { 3.0f, 210.0f, 250.0f, 1.0f, 250.0f, 32.0f, -16.0f, 1.0f, 0 },
    { 3.0f, 79.0f, 250.0f, 132.0f, 250.0f, 132.0f, -148.0f, 1.0f, 0 },
    { 3.0f, 82.0f, 250.0f, 1.0f, 250.0f, 294.0f, -442.0f, 1.0f, 0 },
    { 3.0f, 2.0f, 250.0f, 80.0f, 250.0f, 80.0f, -522.0f, 1.0f, 0 },
};

static const float top_sand_heights[HOURGLASS_LEVEL_COUNT] = { 0.0f, 0.0f, 0.0f, 70.0f, 140.0f, 210.0f };
static const float bottom_sand_heights[HOURGLASS_LEVEL_COUNT] = { 280.0f, 245.0f, 210.0f, 140.0f, 70.0f, 0.0f };
static const float stream_end_starts[HOURGLASS_LEVEL_COUNT] = { 0.0f, 135.0f, 160.0f, 160.0f, 230.0f, 300.0f };
static const float stream_end_lengths[HOURGLASS_LEVEL_COUNT] = { 0.0f, 15.0f, 25.0f, 100.0f, 100.0f, 100.0f };

static const struct script_tables *tables;
static struct playing_clip playing[PLAYING_CLIP_MAX];
static struct script_hud_view hud;
static float stream_phase;

static void advance_tap_balloons(void);
static int the_picture_is_covered(void);

void script_ui_view_clear(void)
{
    memset(playing, 0, sizeof(playing));
    memset(tap_balloons, 0, sizeof(tap_balloons));
    memset(&hud, 0, sizeof(hud));
    stream_phase = 0.0f;
}

void script_ui_view_stop_clips(void)
{
    memset(playing, 0, sizeof(playing));
}

void script_ui_view_start(const struct script_tables *new_tables)
{
    tables = new_tables;
    script_ui_view_clear();
}

void script_ui_view_clip_started(const char *name, int level_from, int level_to)
{
    const struct ui_clip *clip;
    int index;

    clip = tables != NULL ? ui_clips_find(&tables->ui_clips, name) : NULL;
    if (clip == NULL) {
        return;
    }
    for (index = 0; index < PLAYING_CLIP_MAX; index++) {
        if (playing[index].clip == NULL) {
            playing[index].clip = clip;
            playing[index].age_ticks = 0;
            playing[index].level_from = level_from;
            playing[index].level_to = level_to;
            return;
        }
    }
}

void script_ui_view_hud_changed(const struct script_hud_view *view)
{
    hud = *view;
}

static int element_has_a_texture(const struct ui_element *element)
{
    return element->texture_name[0] != '\0' && strcmp(element->texture_name, UI_CLIP_NO_TEXTURE) != 0;
}

static void keep_every_clip_texture_resident(void)
{
    int index;

    if (tables == NULL) {
        return;
    }
    for (index = 0; index < tables->ui_clips.element_count; index++) {
        if (element_has_a_texture(&tables->ui_clips.elements[index])) {
            (void)texture_cache_use_or_start_loading(tables->ui_clips.elements[index].texture_name);
        }
    }
}

static void keep_the_interface_art_resident(void)
{
    static const char *const resident_textures[] = { HOURGLASS_TEXTURE, SIDE_LABEL_TEXTURE, REWIND_BUTTON_TEXTURE, REWIND_FAR_ROCKS_TEXTURE, REWIND_MID_ROCKS_TEXTURE, REWIND_NEAR_ROCKS_TEXTURE, TAP_ART_TEXTURE, NARRATION_ARROW_TEXTURE, PILL_ART_TEXTURE, PILL_SHEET_TEXTURE };
    size_t index;

    for (index = 0; index < sizeof(resident_textures) / sizeof(resident_textures[0]); index++) {
        (void)texture_cache_use_or_start_loading(resident_textures[index]);
    }
    keep_every_clip_texture_resident();
}

void script_ui_view_advance(void)
{
    int index;

    arrow_age_ticks = arrow_age_ticks + 1;
    keep_the_interface_art_resident();
    advance_tap_balloons();
    for (index = 0; index < PLAYING_CLIP_MAX; index++) {
        if (playing[index].clip != NULL) {
            playing[index].age_ticks = playing[index].age_ticks + 1;
            if (playing[index].age_ticks > playing[index].clip->frame_count) {
                playing[index].clip = NULL;
            }
        }
    }
    if (hud.world_is_running) {
        stream_phase = stream_phase + HOURGLASS_STREAM_UNITS_PER_TICK;
        if (stream_phase >= HOURGLASS_STREAM_SPACING) {
            stream_phase = stream_phase - HOURGLASS_STREAM_SPACING;
        }
    } else {
        stream_phase = 0.0f;
    }
}

static const struct cached_texture *ready_texture(const char *name)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(name);
    return texture != NULL && texture->state == CACHED_TEXTURE_READY ? texture : NULL;
}

static struct renderer_rect whole_texture(void)
{
    struct renderer_rect area;

    area.x = 0.0f;
    area.y = 0.0f;
    area.width = 1.0f;
    area.height = 1.0f;
    return area;
}

static struct renderer_rect centered_area(float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = x - width / 2.0f;
    area.y = y - height / 2.0f;
    area.width = width;
    area.height = height;
    return area;
}

static void draw_turned_sprite(renderer_texture texture, float x, float y, float width, float height, float degrees, struct renderer_color tint)
{
    float along_x;
    float along_y;

    along_x = cosf(degrees * DEGREES_TO_RADIANS) * width / 2.0f;
    along_y = sinf(degrees * DEGREES_TO_RADIANS) * width / 2.0f;
    renderer_draw_beam(texture, x - along_x, y - along_y, x + along_x, y + along_y, height / 2.0f, tint);
}

static void draw_stretched_cell(const struct cached_texture *texture, const struct hourglass_cell *cell, float center_x, float center_y, float scale, struct renderer_color tint)
{
    struct renderer_rect area;
    struct renderer_rect part;

    area.x = center_x - cell->gui_width * scale / 2.0f;
    area.y = center_y + cell->gui_top * scale;
    area.width = cell->gui_width * scale;
    area.height = cell->gui_height * scale;
    part.x = cell->x / HOURGLASS_TEXTURE_WIDTH;
    part.width = cell->width / HOURGLASS_TEXTURE_WIDTH;
    part.y = (cell->flips_y ? cell->y + cell->height : cell->y) / HOURGLASS_TEXTURE_HEIGHT;
    part.height = (cell->flips_y ? -cell->height : cell->height) / HOURGLASS_TEXTURE_HEIGHT;
    tint.alpha = tint.alpha * cell->alpha;
    renderer_draw_sprite(texture->texture, area, part, tint);
}

static float lerp(float from, float to, float progress)
{
    return from + (to - from) * progress;
}

static int clamped_level(int level)
{
    if (level < 0) {
        return 0;
    }
    return level >= HOURGLASS_LEVEL_COUNT ? HOURGLASS_LEVEL_COUNT - 1 : level;
}

static void draw_sand_part(const struct cached_texture *texture, float cell_x, float cell_y, float cell_height, float gui_top, float gui_height, float center_x, float center_y, float scale, struct renderer_color tint)
{
    struct hourglass_cell cell;

    cell.x = cell_x;
    cell.y = cell_y;
    cell.width = 206.0f;
    cell.height = cell_height;
    cell.gui_width = 206.0f;
    cell.gui_height = gui_height;
    cell.gui_top = gui_top;
    cell.alpha = 1.0f;
    cell.flips_y = 0;
    draw_stretched_cell(texture, &cell, center_x, center_y, scale, tint);
}

static float stream_opacity(float gui_y, float end_start, float end_length)
{
    if (gui_y < HOURGLASS_STREAM_TOP || gui_y > end_start + end_length) {
        return 0.0f;
    }
    if (gui_y < HOURGLASS_STREAM_FADE_END) {
        return (gui_y - HOURGLASS_STREAM_TOP) / (HOURGLASS_STREAM_FADE_END - HOURGLASS_STREAM_TOP);
    }
    if (gui_y > end_start && end_length > 0.0f) {
        return 1.0f - (gui_y - end_start) / end_length;
    }
    return 1.0f;
}

static void draw_stream(const struct cached_texture *texture, float center_x, float center_y, float scale, float end_start, float end_length, float phase, struct renderer_color tint)
{
    float strip_height;
    int strip;

    strip_height = (end_start + end_length - HOURGLASS_STREAM_TOP) / (float)HOURGLASS_STREAM_STRIP_COUNT;
    for (strip = 0; strip < HOURGLASS_STREAM_STRIP_COUNT; strip++) {
        struct renderer_rect area;
        struct renderer_rect part;
        struct renderer_color strip_tint;
        float gui_y;
        float along;

        gui_y = HOURGLASS_STREAM_TOP + (float)strip * strip_height;
        along = fmodf(gui_y - phase + HOURGLASS_STREAM_SPACING * 4.0f, HOURGLASS_STREAM_SPACING);
        area.x = center_x - HOURGLASS_STREAM_WIDTH * scale / 2.0f;
        area.y = center_y + gui_y * scale;
        area.width = HOURGLASS_STREAM_WIDTH * scale;
        area.height = strip_height * scale;
        part.x = 434.0f / HOURGLASS_TEXTURE_WIDTH;
        part.width = 36.0f / HOURGLASS_TEXTURE_WIDTH;
        part.y = (470.0f + along / HOURGLASS_STREAM_HEIGHT * 552.0f) / HOURGLASS_TEXTURE_HEIGHT;
        part.height = (strip_height / HOURGLASS_STREAM_HEIGHT * 552.0f) / HOURGLASS_TEXTURE_HEIGHT;
        strip_tint = tint;
        strip_tint.alpha = tint.alpha * stream_opacity(gui_y + strip_height / 2.0f, end_start, end_length);
        if (strip_tint.alpha > 0.0f) {
            renderer_draw_sprite(texture->texture, area, part, strip_tint);
        }
    }
}

static void draw_hourglass(float center_x, float center_y, float scale, int level_from, int level_to, float progress, float phase, struct renderer_color tint)
{
    const struct cached_texture *texture;
    float top_height;
    float bottom_height;
    float end_start;
    float end_length;
    float funnel_clip;
    unsigned int index;

    texture = ready_texture(HOURGLASS_TEXTURE);
    if (texture == NULL || tint.alpha <= 0.0f) {
        return;
    }
    level_from = clamped_level(level_from);
    level_to = clamped_level(level_to);
    top_height = lerp(top_sand_heights[level_from], top_sand_heights[level_to], progress);
    bottom_height = lerp(bottom_sand_heights[level_from], bottom_sand_heights[level_to], progress);
    end_start = lerp(stream_end_starts[level_from], stream_end_starts[level_to], progress);
    end_length = lerp(stream_end_lengths[level_from], stream_end_lengths[level_to], progress);
    funnel_clip = lerp(level_from <= HOURGLASS_LAST_LEVEL ? HOURGLASS_FUNNEL_CLIP_AT_LAST : 0.0f, level_to <= HOURGLASS_LAST_LEVEL ? HOURGLASS_FUNNEL_CLIP_AT_LAST : 0.0f, progress);
    for (index = 0; index < sizeof(glass_cells) / sizeof(glass_cells[0]); index++) {
        draw_stretched_cell(texture, &glass_cells[index], center_x, center_y, scale, tint);
    }
    if (level_from > 0 || level_to > 0) {
        draw_stream(texture, center_x, center_y, scale, end_start, end_length, phase, tint);
    }
    draw_sand_part(texture, 259.0f, 434.0f, 32.0f, HOURGLASS_BOTTOM_BOTTOM_EDGE, 32.0f, center_x, center_y, scale, tint);
    draw_sand_part(texture, 259.0f, 433.0f, 1.0f, HOURGLASS_BOTTOM_BOTTOM_EDGE - bottom_height, bottom_height, center_x, center_y, scale, tint);
    draw_sand_part(texture, 259.0f, 365.0f, 68.0f, HOURGLASS_BOTTOM_BOTTOM_EDGE - bottom_height - 68.0f, 68.0f, center_x, center_y, scale, tint);
    if (level_from > 0 || level_to > 0) {
        draw_sand_part(texture, 259.0f, 193.0f + funnel_clip, HOURGLASS_FUNNEL_HEIGHT - funnel_clip, HOURGLASS_TOP_BOTTOM_EDGE + funnel_clip, HOURGLASS_FUNNEL_HEIGHT - funnel_clip, center_x, center_y, scale, tint);
    }
    if (top_height > 0.0f) {
        draw_sand_part(texture, 259.0f, 193.0f, 1.0f, HOURGLASS_TOP_BOTTOM_EDGE - top_height, top_height, center_x, center_y, scale, tint);
        draw_sand_part(texture, 259.0f, 159.0f, 36.0f, HOURGLASS_TOP_BOTTOM_EDGE - top_height - 36.0f, 36.0f, center_x, center_y, scale, tint);
    }
    for (index = 0; index < sizeof(outline_cells) / sizeof(outline_cells[0]); index++) {
        draw_stretched_cell(texture, &outline_cells[index], center_x, center_y, scale, tint);
    }
}

static void draw_band(float scale_y, struct renderer_color color)
{
    struct renderer_rect area;
    int stripe;

    area.x = 0.0f;
    area.width = (float)RENDERER_WIDTH;
    area.y = BAND_CENTER_Y - 112.0f * scale_y;
    area.height = 23.0f * scale_y;
    renderer_draw_filled_rect(area, color);
    for (stripe = 0; stripe < BAND_STRIPE_COUNT; stripe++) {
        area.y = BAND_CENTER_Y + scale_y * (-84.5f + 6.0f * (float)stripe) - 1.5f * scale_y;
        area.height = 3.0f * scale_y;
        renderer_draw_filled_rect(area, color);
    }
    area.y = BAND_CENTER_Y + 88.0f * scale_y;
    area.height = 23.0f * scale_y;
    renderer_draw_filled_rect(area, color);
}

static float window_opacity(float x, float window_start)
{
    if (x < window_start - REVEAL_RAMP_WIDTH || x > window_start + REVEAL_RAMP_WIDTH * 2.0f) {
        return 0.0f;
    }
    if (x < window_start) {
        return (x - (window_start - REVEAL_RAMP_WIDTH)) / REVEAL_RAMP_WIDTH;
    }
    if (x > window_start + REVEAL_RAMP_WIDTH) {
        return 1.0f - (x - (window_start + REVEAL_RAMP_WIDTH)) / REVEAL_RAMP_WIDTH;
    }
    return 1.0f;
}

static void draw_reveal(const struct cached_texture *texture, float x, float y, float width, float height, float window_start, float alpha)
{
    int strip;

    for (strip = 0; strip < REVEAL_STRIP_COUNT; strip++) {
        struct renderer_rect area;
        struct renderer_rect part;
        struct renderer_color tint;
        float fraction;
        float step;

        step = 1.0f / (float)REVEAL_STRIP_COUNT;
        fraction = (float)strip * step;
        area.x = x - width / 2.0f + fraction * width;
        area.y = y - height / 2.0f;
        area.width = step * width;
        area.height = height;
        part.x = fraction;
        part.y = 0.0f;
        part.width = step;
        part.height = 1.0f;
        tint = white;
        tint.alpha = alpha * window_opacity(area.x + area.width / 2.0f, window_start);
        if (tint.alpha > 0.0f) {
            renderer_draw_sprite(texture->texture, area, part, tint);
        }
    }
}

static void draw_element(const struct ui_element *element, const struct playing_clip *clip)
{
    const struct ui_clips *clips;
    struct renderer_color tint;
    const struct cached_texture *texture;
    float frame;
    float x;
    float y;
    float width;
    float height;
    float rotation;

    clips = &tables->ui_clips;
    frame = (float)clip->age_ticks;
    tint.red = element->red;
    tint.green = element->green;
    tint.blue = element->blue;
    tint.alpha = ui_element_value(clips, element, UI_PROP_ALPHA, frame, 1.0f);
    if (tint.alpha <= 0.0f) {
        return;
    }
    x = ui_element_value(clips, element, UI_PROP_X, frame, 0.0f);
    y = ui_element_value(clips, element, UI_PROP_Y, frame, 0.0f);
    width = ui_element_value(clips, element, UI_PROP_WIDTH, frame, 0.0f);
    height = ui_element_value(clips, element, UI_PROP_HEIGHT, frame, 0.0f);
    rotation = ui_element_value(clips, element, UI_PROP_ROTATION, frame, 0.0f);
    switch (element->kind) {
    case UI_ELEMENT_RECT:
        renderer_draw_filled_rect(centered_area(x, y, width, height), tint);
        break;
    case UI_ELEMENT_BAND:
        draw_band(height, tint);
        break;
    case UI_ELEMENT_HOURGLASS:
        draw_hourglass(x, y, width, clip->level_from, clip->level_to, ui_element_value(clips, element, UI_PROP_WINDOW, frame, 1.0f), 0.0f, tint);
        break;
    case UI_ELEMENT_REVEAL:
        texture = ready_texture(element->texture_name);
        if (texture != NULL) {
            draw_reveal(texture, x, y, width, height, ui_element_value(clips, element, UI_PROP_WINDOW, frame, 0.0f), tint.alpha);
        }
        break;
    default:
        texture = ready_texture(element->texture_name);
        if (texture == NULL) {
            break;
        }
        if (rotation != 0.0f) {
            draw_turned_sprite(texture->texture, x, y, width, height, rotation, tint);
        } else {
            renderer_draw_sprite(texture->texture, centered_area(x, y, width, height), whole_texture(), tint);
        }
        break;
    }
}

static void draw_side_hud(float screen_fade)
{
    const struct cached_texture *label;
    struct renderer_color tint;
    float dim;

    if (!hud.side_is_on || screen_fade >= 1.0f) {
        return;
    }
    dim = hud.world_is_running ? 1.0f : SIDE_DIM;
    tint.red = dim;
    tint.green = dim;
    tint.blue = dim;
    tint.alpha = screen_fade > 0.0f ? 1.0f - screen_fade : 1.0f;
    label = ready_texture(SIDE_LABEL_TEXTURE);
    if (label != NULL) {
        draw_turned_sprite(label->texture, SIDE_LABEL_X, SIDE_LABEL_Y, SIDE_LABEL_LENGTH, SIDE_LABEL_THICKNESS, SIDE_LABEL_ROTATION, tint);
    }
    if (hud.fate_level > 0) {
        draw_hourglass(SIDE_HOURGLASS_X, SIDE_HOURGLASS_Y, SIDE_HOURGLASS_SCALE, hud.fate_level, hud.fate_level, 1.0f, stream_phase, tint);
    }
}

static void draw_rock_layer(const char *name, float pixels_per_tick, float alpha)
{
    const struct cached_texture *rocks;
    struct renderer_rect area;
    struct renderer_color tint;
    float scroll;

    rocks = ready_texture(name);
    if (rocks == NULL) {
        return;
    }
    scroll = fmodf((float)hud.rewind_select_ticks * pixels_per_tick, REWIND_ROCK_HEIGHT);
    tint = white;
    tint.alpha = alpha;
    area.x = REWIND_ROCK_X;
    area.width = REWIND_ROCK_WIDTH;
    area.height = REWIND_ROCK_HEIGHT;
    area.y = -scroll;
    renderer_draw_sprite(rocks->texture, area, whole_texture(), tint);
    area.y = area.y + REWIND_ROCK_HEIGHT;
    renderer_draw_sprite(rocks->texture, area, whole_texture(), tint);
}

static void draw_slice(const struct cached_texture *art, float x, float y, float width, float height, float u, float v, float cell_width, float cell_height, struct renderer_color tint)
{
    struct renderer_rect area;
    struct renderer_rect part;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    part.x = u;
    part.y = v;
    part.width = cell_width;
    part.height = cell_height;
    renderer_draw_sprite(art->texture, area, part, tint);
}

static void draw_select_button(float x, float y, float alpha, float brightness)
{
    const struct cached_texture *art;
    struct renderer_color tint;
    float corner;
    float inner;
    float far_x;
    float far_y;

    art = ready_texture(REWIND_BUTTON_TEXTURE);
    if (art == NULL) {
        return;
    }
    tint.red = brightness;
    tint.green = brightness;
    tint.blue = brightness;
    tint.alpha = alpha;
    corner = REWIND_BUTTON_CORNER;
    inner = REWIND_BUTTON_CORNER_UV;
    far_x = x + REWIND_BUTTON_WIDTH - corner;
    far_y = y + REWIND_BUTTON_HEIGHT - corner;
    draw_slice(art, x, y, corner, corner, 0.0f, 0.0f, inner, inner, tint);
    draw_slice(art, x + corner, y, REWIND_BUTTON_WIDTH - corner * 2.0f, corner, inner, 0.0f, 1.0f - inner * 2.0f, inner, tint);
    draw_slice(art, far_x, y, corner, corner, 1.0f - inner, 0.0f, inner, inner, tint);
    draw_slice(art, x, y + corner, corner, REWIND_BUTTON_HEIGHT - corner * 2.0f, 0.0f, inner, inner, 1.0f - inner * 2.0f, tint);
    draw_slice(art, x + corner, y + corner, REWIND_BUTTON_WIDTH - corner * 2.0f, REWIND_BUTTON_HEIGHT - corner * 2.0f, inner, inner, 1.0f - inner * 2.0f, 1.0f - inner * 2.0f, tint);
    draw_slice(art, far_x, y + corner, corner, REWIND_BUTTON_HEIGHT - corner * 2.0f, 1.0f - inner, inner, inner, 1.0f - inner * 2.0f, tint);
    draw_slice(art, x, far_y, corner, corner, 0.0f, 1.0f - inner, inner, inner, tint);
    draw_slice(art, x + corner, far_y, REWIND_BUTTON_WIDTH - corner * 2.0f, corner, inner, 1.0f - inner, 1.0f - inner * 2.0f, inner, tint);
    draw_slice(art, far_x, far_y, corner, corner, 1.0f - inner, 1.0f - inner, inner, inner, tint);
}

static void draw_rewind_select(void)
{
    const struct cached_texture *sheet;
    struct renderer_rect area;
    struct renderer_color tint;
    float alpha;
    float slide;
    float brightness;
    float button_x;

    if (hud.rewind_select_ticks == 0) {
        return;
    }
    alpha = 1.0f;
    brightness = REWIND_RESTING_BRIGHTNESS;
    if (hud.rewind_press_ticks > 0) {
        brightness = hud.rewind_press_ticks < REWIND_PRESS_FLASH_TICKS ? 1.0f : REWIND_RESTING_BRIGHTNESS;
        alpha = 1.0f - (float)hud.rewind_press_ticks / (float)REWIND_PRESS_FADE_TICKS;
        if (alpha < 0.0f) {
            alpha = 0.0f;
        }
    }
    draw_rock_layer(REWIND_FAR_ROCKS_TEXTURE, REWIND_FAR_SPEED, alpha);
    draw_rock_layer(REWIND_MID_ROCKS_TEXTURE, REWIND_MID_SPEED, alpha);
    draw_rock_layer(REWIND_NEAR_ROCKS_TEXTURE, REWIND_NEAR_SPEED, alpha);
    slide = 1.0f - (float)hud.rewind_select_ticks / (float)REWIND_SLIDE_TICKS;
    if (slide < 0.0f) {
        slide = 0.0f;
    }
    button_x = REWIND_BUTTON_X + slide * REWIND_SLIDE_PIXELS;
    draw_select_button(button_x, REWIND_BUTTON_Y, alpha, brightness);
    sheet = ready_texture(PILL_SHEET_TEXTURE);
    if (sheet != NULL) {
        struct renderer_rect part;

        area.x = button_x + (REWIND_BUTTON_WIDTH - REWIND_LABEL_WIDTH) / 2.0f;
        area.y = REWIND_BUTTON_Y + (REWIND_BUTTON_HEIGHT - REWIND_LABEL_HEIGHT) / 2.0f;
        area.width = REWIND_LABEL_WIDTH;
        area.height = REWIND_LABEL_HEIGHT;
        part.x = 0.0f;
        part.y = REWIND_LABEL_SHEET_Y / PILL_SHEET_HEIGHT;
        part.width = REWIND_LABEL_SHEET_WIDTH / PILL_SHEET_WIDTH;
        part.height = REWIND_LABEL_SHEET_HEIGHT / PILL_SHEET_HEIGHT;
        tint = white;
        tint.alpha = alpha;
        renderer_draw_sprite(sheet->texture, area, part, tint);
    }
}

static float keyed_value(const float *values, float frame)
{
    int index;

    for (index = 0; index < TAP_BALLOON_KEY_COUNT - 1; index++) {
        if (frame <= tap_balloon_key_frames[index + 1]) {
            float share;

            share = (frame - tap_balloon_key_frames[index]) / (tap_balloon_key_frames[index + 1] - tap_balloon_key_frames[index]);
            return values[index] + (values[index + 1] - values[index]) * share;
        }
    }
    return values[TAP_BALLOON_KEY_COUNT - 1];
}

static void draw_tap_balloon_art(const struct cached_texture *art, float x, float y, float width, float height, float cell_x, float cell_y, float cell_width, float cell_height, struct renderer_color tint)
{
    draw_slice(art, x - width / 2.0f, y - height / 2.0f, width, height, cell_x / TAP_ART_WIDTH, cell_y / TAP_ART_HEIGHT, cell_width / TAP_ART_WIDTH, cell_height / TAP_ART_HEIGHT, tint);
}

static void tap_balloon_center(const struct tap_balloon *balloon, float *x, float *y)
{
    if (balloon->kind == SCRIPT_BALLOON_KIND_TAP_MARKS) {
        *x = TAP_MARKS_X;
        *y = TAP_MARKS_Y;
        return;
    }
    scene_view_world_to_screen(scene_stage_view(), balloon->anchor_x, balloon->anchor_y, x, y);
    *y = *y + TAP_DOTS_ABOVE_ANCHOR;
}

static void draw_tap_balloon(const struct tap_balloon *balloon)
{
    const struct cached_texture *art;
    const struct cached_texture *sheet;
    struct renderer_color black;
    struct renderer_rect glyph;
    struct renderer_rect part;
    float frame;
    float width_scale;
    float height_scale;
    float x;
    float y;
    float full_height;
    int is_marks;
    int index;

    art = ready_texture(TAP_ART_TEXTURE);
    sheet = ready_texture(PILL_SHEET_TEXTURE);
    if (art == NULL || sheet == NULL) {
        return;
    }
    is_marks = balloon->kind == SCRIPT_BALLOON_KIND_TAP_MARKS;
    frame = (float)balloon->age_ticks;
    width_scale = keyed_value(tap_balloon_widths, frame);
    height_scale = keyed_value(tap_balloon_heights, frame);
    if (balloon->closing_ticks > 0) {
        float closed;

        closed = (float)balloon->closing_ticks / (float)TAP_BALLOON_CLOSE_TICKS;
        width_scale = 1.0f + closed * TAP_BALLOON_CLOSE_WIDEN;
        height_scale = 1.0f - closed;
    }
    tap_balloon_center(balloon, &x, &y);
    full_height = is_marks ? TAP_OVAL_HEIGHT : TAP_OVAL_WITH_TAIL_HEIGHT;
    draw_tap_balloon_art(art, x, y, TAP_OVAL_WIDTH * width_scale, full_height * height_scale, TAP_OVAL_CELL_X, TAP_OVAL_CELL_Y, TAP_OVAL_CELL_WIDTH, is_marks ? TAP_OVAL_CELL_HEIGHT : TAP_OVAL_WITH_TAIL_CELL_HEIGHT, white);
    if (balloon->age_ticks < TAP_CONTENT_SHOWS_AT || balloon->closing_ticks > 0) {
        return;
    }
    black.red = 0.0f;
    black.green = 0.0f;
    black.blue = 0.0f;
    black.alpha = 1.0f;
    for (index = -1; index <= 1; index++) {
        if (is_marks) {
            draw_tap_balloon_art(art, x + (float)index * TAP_MARK_SPACING, y, TAP_MARK_SIZE, TAP_MARK_SIZE, TAP_MARK_CELL_X, TAP_MARK_CELL_Y, TAP_MARK_CELL_SIZE, TAP_MARK_CELL_SIZE, white);
        } else {
            draw_tap_balloon_art(art, x + (float)index * TAP_DOT_SPACING, y - TAP_DOTS_ABOVE_MIDDLE, TAP_DOT_SIZE, TAP_DOT_SIZE, TAP_OVAL_CELL_X, TAP_OVAL_CELL_Y, TAP_OVAL_CELL_WIDTH, TAP_OVAL_CELL_HEIGHT, black);
        }
    }
    glyph.x = x + TAP_GLYPH_RIGHT_OF_MIDDLE;
    glyph.y = y + (is_marks ? TAP_MARKS_GLYPH_BELOW_MIDDLE : TAP_DOTS_GLYPH_BELOW_MIDDLE);
    glyph.width = TAP_GLYPH_SIZE;
    glyph.height = TAP_GLYPH_SIZE;
    part.x = (is_marks ? TAP_SQUARE_GLYPH_X : TAP_TRIANGLE_GLYPH_X) / PILL_SHEET_WIDTH;
    part.y = 0.0f;
    part.width = TAP_GLYPH_CELL / PILL_SHEET_WIDTH;
    part.height = TAP_GLYPH_CELL / PILL_SHEET_HEIGHT;
    renderer_draw_sprite(sheet->texture, glyph, part, white);
}

static void draw_tap_balloons(void)
{
    int index;

    if (hud.soul_view_is_on || hud.picture_is_hidden || the_picture_is_covered()) {
        return;
    }
    for (index = 0; index < TAP_BALLOON_MAX; index++) {
        if (tap_balloons[index].is_open && tap_balloons[index].age_ticks > 0) {
            draw_tap_balloon(&tap_balloons[index]);
        }
    }
}

static int point_is_in_box(float x, float y, float left, float top, float width, float height)
{
    return x >= left - TOUCH_SLOP_PIXELS && x <= left + width + TOUCH_SLOP_PIXELS && y >= top - TOUCH_SLOP_PIXELS && y <= top + height + TOUCH_SLOP_PIXELS;
}

static int the_picture_is_covered(void)
{
    const struct scene_stage *stage;

    stage = scene_stage_view();
    return stage->card_texture[0] != '\0' || stage->fade_amount >= 1.0f;
}

int script_ui_view_pill_at(float x, float y)
{
    int index;

    if (hud.is_hidden_for_movie || hud.picture_is_hidden || the_picture_is_covered()) {
        return -1;
    }
    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        const struct script_pill_view *pill;

        pill = &hud.pills[index];
        if (pill->is_on_screen && pill->slide_step == PILL_SLIDE_STEPS && !pill->is_pressed && point_is_in_box(x, y, pill_layouts[index].x, pill_layouts[index].y, pill_layouts[index].width, PILL_RING_HEIGHT)) {
            return index;
        }
    }
    return -1;
}

int script_ui_view_tap_balloon_kind_at(float x, float y)
{
    int index;

    if (hud.soul_view_is_on || hud.picture_is_hidden || the_picture_is_covered()) {
        return 0;
    }
    for (index = 0; index < TAP_BALLOON_MAX; index++) {
        const struct tap_balloon *balloon;
        float center_x;
        float center_y;

        balloon = &tap_balloons[index];
        if (!balloon->is_open || balloon->closing_ticks > 0) {
            continue;
        }
        tap_balloon_center(balloon, &center_x, &center_y);
        if (point_is_in_box(x, y, center_x - TAP_OVAL_WIDTH / 2.0f, center_y - TAP_OVAL_HEIGHT / 2.0f, TAP_OVAL_WIDTH, TAP_OVAL_HEIGHT)) {
            return balloon->kind;
        }
    }
    return 0;
}

int script_ui_view_rewind_button_is_at(float x, float y)
{
    return hud.rewind_select_ticks > 0 && point_is_in_box(x, y, REWIND_BUTTON_X, REWIND_BUTTON_Y, REWIND_BUTTON_WIDTH, REWIND_BUTTON_HEIGHT);
}

void script_ui_view_tap_balloon_opened(int slot, int kind, float anchor_x, float anchor_y)
{
    if (slot < 0 || slot >= TAP_BALLOON_MAX) {
        return;
    }
    tap_balloons[slot].is_open = 1;
    tap_balloons[slot].kind = kind;
    tap_balloons[slot].age_ticks = 0;
    tap_balloons[slot].closing_ticks = 0;
    tap_balloons[slot].anchor_x = anchor_x;
    tap_balloons[slot].anchor_y = anchor_y;
}

void script_ui_view_tap_balloon_closed(int slot)
{
    if (slot >= 0 && slot < TAP_BALLOON_MAX && tap_balloons[slot].is_open && tap_balloons[slot].closing_ticks == 0) {
        tap_balloons[slot].closing_ticks = 1;
    }
}

static void advance_tap_balloons(void)
{
    int index;

    for (index = 0; index < TAP_BALLOON_MAX; index++) {
        struct tap_balloon *balloon;

        balloon = &tap_balloons[index];
        if (!balloon->is_open || (!hud.world_is_running && balloon->closing_ticks == 0)) {
            continue;
        }
        balloon->age_ticks = balloon->age_ticks + 1;
        if (balloon->closing_ticks > 0) {
            balloon->closing_ticks = balloon->closing_ticks + 1;
            if (balloon->closing_ticks > TAP_BALLOON_CLOSE_TICKS) {
                balloon->is_open = 0;
            }
        }
    }
}

static void draw_narration_arrow(void)
{
    const struct cached_texture *arrow;
    struct renderer_rect area;
    int phase;

    if (!hud.narration_arrow_is_on) {
        arrow_age_ticks = 0;
        return;
    }
    arrow = ready_texture(NARRATION_ARROW_TEXTURE);
    if (arrow == NULL) {
        return;
    }
    phase = arrow_age_ticks % NARRATION_ARROW_BOB_TICKS;
    if (phase > NARRATION_ARROW_BOB_TICKS / 2) {
        phase = NARRATION_ARROW_BOB_TICKS - phase;
    }
    area.x = NARRATION_ARROW_X;
    area.y = NARRATION_ARROW_Y + (float)phase / (float)(NARRATION_ARROW_BOB_TICKS / 2) * NARRATION_ARROW_BOB_PIXELS;
    area.width = NARRATION_ARROW_WIDTH;
    area.height = NARRATION_ARROW_HEIGHT;
    renderer_draw_sprite(arrow->texture, area, whole_texture(), white);
}

static float pill_pulse_brightness(void)
{
    int phase;

    phase = (hud.pulse_ticks + PILL_PULSE_PHASE_SHIFT) % PILL_PULSE_PERIOD;
    if (phase < PILL_PULSE_FALL_TICKS) {
        return 1.0f - PILL_PULSE_DEPTH * (float)phase / (float)PILL_PULSE_FALL_TICKS;
    }
    if (phase < PILL_PULSE_RISE_ENDS_AT) {
        return 1.0f - PILL_PULSE_DEPTH * (float)(PILL_PULSE_RISE_ENDS_AT - phase) / (float)(PILL_PULSE_RISE_ENDS_AT - PILL_PULSE_FALL_TICKS);
    }
    return 1.0f;
}

static void draw_art_cell(const struct cached_texture *art, float x, float y, float width, float height, float cell_x, float cell_y, float cell_width, float cell_height)
{
    struct renderer_rect area;
    struct renderer_rect part;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    part.x = cell_x / PILL_ART_SIZE;
    part.y = cell_y / PILL_ART_SIZE;
    part.width = cell_width / PILL_ART_SIZE;
    part.height = cell_height / PILL_ART_SIZE;
    renderer_draw_sprite(art->texture, area, part, white);
}

static void draw_sheet_cell(const struct cached_texture *sheet, const struct pill_sheet_cell *cell, float x, float y, float width, float height)
{
    struct renderer_rect area;
    struct renderer_rect part;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    part.x = cell->x / PILL_SHEET_WIDTH;
    part.y = cell->y / PILL_SHEET_HEIGHT;
    part.width = cell->width / PILL_SHEET_WIDTH;
    part.height = cell->height / PILL_SHEET_HEIGHT;
    renderer_draw_sprite(sheet->texture, area, part, white);
}

static float pill_screen_y(const struct pill_layout *layout, int slide_step)
{
    float away;

    away = (float)(PILL_SLIDE_STEPS - slide_step) * PILL_SLIDE_PIXELS_PER_STEP;
    return layout->is_at_bottom ? layout->y + away : layout->y - away;
}

static void draw_pill(const struct pill_layout *layout, const struct script_pill_view *view, const struct cached_texture *art, const struct cached_texture *sheet)
{
    struct renderer_rect fill;
    struct renderer_color fill_color;
    float x;
    float y;
    float ring_cell_y;
    float brightness;

    x = layout->x;
    y = pill_screen_y(layout, view->slide_step) + (view->is_pressed ? PILL_PRESSED_DROP : 0.0f);
    ring_cell_y = view->is_pressed ? PILL_RING_PRESSED_CELL_Y : PILL_RING_CELL_Y;
    fill.x = x + PILL_RING_THICKNESS;
    fill.y = y + PILL_FILL_TOP;
    fill.width = layout->width - PILL_RING_THICKNESS * 2.0f;
    fill.height = PILL_FILL_HEIGHT;
    renderer_draw_filled_rect(fill, white);
    brightness = pill_pulse_brightness();
    fill_color.red = view->is_pressed ? 1.0f : brightness;
    fill_color.green = view->is_pressed ? PILL_PRESSED_GREEN : 0.0f;
    fill_color.blue = 0.0f;
    fill_color.alpha = 1.0f;
    fill.x = x + layout->fill_left;
    fill.width = layout->fill_right - layout->fill_left;
    renderer_draw_filled_rect(fill, fill_color);
    draw_art_cell(art, x, y, PILL_RING_CAP_WIDTH, PILL_RING_HEIGHT, PILL_RING_LEFT_CELL_X, ring_cell_y, PILL_RING_CAP_TEXELS, PILL_RING_TEXELS);
    draw_art_cell(art, x + PILL_RING_CAP_WIDTH, y, layout->width - PILL_RING_CAP_WIDTH * 2.0f, PILL_RING_HEIGHT, PILL_RING_MIDDLE_CELL_X, ring_cell_y, PILL_RING_MIDDLE_TEXELS, PILL_RING_TEXELS);
    draw_art_cell(art, x + layout->width - PILL_RING_CAP_WIDTH, y, PILL_RING_CAP_WIDTH, PILL_RING_HEIGHT, PILL_RING_RIGHT_CELL_X, ring_cell_y, PILL_RING_CAP_TEXELS, PILL_RING_TEXELS);
    if (layout->icon_cell_x >= 0.0f) {
        draw_art_cell(art, x + layout->icon_x, y + layout->icon_y, PILL_ICON_SIZE, PILL_ICON_SIZE, layout->icon_cell_x, layout->icon_cell_y, layout->icon_texels > 0.0f ? layout->icon_texels : PILL_ICON_TEXELS, layout->icon_texels > 0.0f ? layout->icon_texels : PILL_ICON_TEXELS);
    }
    if (layout->label != NULL) {
        float label_width;

        label_width = PILL_LABEL_WIDTH * layout->label->width / PILL_LABEL_CELL_WIDTH;
        draw_sheet_cell(sheet, layout->label, x + layout->label_center_x - label_width / 2.0f, y + PILL_RING_HEIGHT / 2.0f - PILL_LABEL_HEIGHT / 2.0f + PILL_LABEL_NUDGE_Y, label_width, PILL_LABEL_HEIGHT);
    }
    draw_sheet_cell(sheet, layout->glyph, x + layout->glyph_x, y + PILL_RING_HEIGHT / 2.0f - PILL_GLYPH_SIZE / 2.0f + PILL_GLYPH_NUDGE_Y, layout->glyph_width, PILL_GLYPH_SIZE);
}

static void draw_pills(void)
{
    const struct cached_texture *art;
    const struct cached_texture *sheet;
    int index;

    art = ready_texture(PILL_ART_TEXTURE);
    sheet = ready_texture(PILL_SHEET_TEXTURE);
    if (art == NULL || sheet == NULL) {
        return;
    }
    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        if (hud.pills[index].is_on_screen) {
            draw_pill(&pill_layouts[index], &hud.pills[index], art, sheet);
        }
    }
}

void script_ui_view_draw_under_talk(float screen_fade)
{
    if (tables == NULL || hud.is_hidden_for_movie) {
        return;
    }
    draw_side_hud(screen_fade);
}

void script_ui_view_draw(void)
{
    int index;

    if (tables == NULL) {
        return;
    }
    if (!hud.is_hidden_for_movie) {
        draw_tap_balloons();
        draw_rewind_select();
        if (!hud.picture_is_hidden && !the_picture_is_covered()) {
            draw_pills();
        }
        draw_narration_arrow();
    }
    for (index = 0; index < PLAYING_CLIP_MAX; index++) {
        if (playing[index].clip != NULL) {
            int element;

            for (element = 0; element < playing[index].clip->element_count; element++) {
                draw_element(&tables->ui_clips.elements[playing[index].clip->first_element + element], &playing[index]);
            }
        }
    }
}
