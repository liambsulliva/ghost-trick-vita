#include <math.h>
#include <stdio.h>
#include <string.h>
#include "renderer.h"
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

void script_ui_view_clear(void)
{
    memset(playing, 0, sizeof(playing));
    memset(&hud, 0, sizeof(hud));
    stream_phase = 0.0f;
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

void script_ui_view_advance(void)
{
    int index;

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

static void draw_side_hud(void)
{
    const struct cached_texture *label;
    struct renderer_color tint;
    float dim;

    if (!hud.side_is_on) {
        return;
    }
    dim = hud.world_is_running ? 1.0f : SIDE_DIM;
    tint.red = dim;
    tint.green = dim;
    tint.blue = dim;
    tint.alpha = 1.0f;
    label = ready_texture(SIDE_LABEL_TEXTURE);
    if (label != NULL) {
        draw_turned_sprite(label->texture, SIDE_LABEL_X, SIDE_LABEL_Y, SIDE_LABEL_LENGTH, SIDE_LABEL_THICKNESS, SIDE_LABEL_ROTATION, tint);
    }
    if (hud.fate_level > 0) {
        draw_hourglass(SIDE_HOURGLASS_X, SIDE_HOURGLASS_Y, SIDE_HOURGLASS_SCALE, hud.fate_level, hud.fate_level, 1.0f, stream_phase, tint);
    }
}

void script_ui_view_draw(void)
{
    int index;

    if (tables == NULL) {
        return;
    }
    if (!hud.is_hidden_for_movie) {
        draw_side_hud();
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
