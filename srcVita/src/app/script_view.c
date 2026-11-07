#include <math.h>
#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_talk.h"
#include "script_outputs.h"
#include "script_ui_view.h"
#include "script_view.h"
#include "texture_cache.h"

#define VIEW_EFFECT_MAX 16
#define VIEW_EFFECT_NAME_MAX 32
#define BOX_BOTTOM_TEXTURE "talk_box_bottom"
#define BOX_TOP_TEXTURE "talk_box_top"
#define CURSOR_TEXTURE "talk_cursor"
#define BOX_BOTTOM_X 0.0f
#define BOX_BOTTOM_Y 412.0f
#define BOX_BOTTOM_WIDTH 960.0f
#define BOX_TOP_X 180.0f
#define BOX_TOP_Y 6.0f
#define BOX_TOP_WIDTH 600.0f
#define BOX_HEIGHT 132.0f
#define FACE_X 600.0f
#define FACE_Y 2.0f
#define FACE_WIDTH 360.0f
#define FACE_HEIGHT 540.0f
#define CURSOR_WIDTH 16.0f
#define CURSOR_HEIGHT 24.0f
#define CURSOR_MARGIN_RIGHT 24.0f
#define CURSOR_MARGIN_BOTTOM 12.0f
#define CURSOR_BOB_TICKS 30u
#define CURSOR_BOB_PIXELS 4.0f
#define RING_PULSE_TICKS 60u
#define RING_SMALLEST_SIZE 0.75f
#define VIEW_CORE_MARK_MAX 64
#define SOUL_BLOB_TEXTURE "soul_blob"
#define SOUL_GLOW_TEXTURE "soul_glow"
#define SOUL_ORB_TEXTURE "soul_orb"
#define SOUL_BLOB_DISC_FRACTION 0.59f
#define SOUL_ORB_SIZE 56.0f
#define SOUL_TWO_PI 6.2831853f
#define SOUL_ANIMATION_TICKS_PER_STEP 4u
#define SOUL_BASE_RISE 4.0f
#define SOUL_BASE_RADIUS 28.0f
#define SOUL_BASE_LUMP_COUNT 7
#define SOUL_BASE_LUMP_REACH 13.0f
#define SOUL_BASE_LUMP_JITTER 3.0f
#define SOUL_BASE_LUMP_RADIUS 19.0f
#define SOUL_WISP_COUNT 10u
#define SOUL_WISP_LIFE_STEPS 15u
#define SOUL_WISP_START_RISE 22.0f
#define SOUL_WISP_RISE 70.0f
#define SOUL_WISP_SWAY_TURNS 5.0f
#define SOUL_WISP_SWAY_NEAR 1.5f
#define SOUL_WISP_SWAY_FAR 13.0f
#define SOUL_WISP_RADIUS_LEAST 13.0f
#define SOUL_WISP_RADIUS_SPREAD 10.0f
#define SOUL_WISP_RADIUS_SMALLEST 1.0f
#define CORE_GLOW_SIZE 84.0f
#define CORE_ORB_SIZE 40.0f
#define TOOLTIP_RING_TEXTURE "soul_tooltip_ring"
#define TOOLTIP_NO_ACTION "-"
#define TOOLTIP_RING_SIZE 135.0f
#define TOOLTIP_UNKNOWN_OBJECT "?"
#define TOOLTIP_LINE_START 24.0f
#define TOOLTIP_LINE_END 135.0f
#define TOOLTIP_BEND_RISE 85.0f
#define TOOLTIP_BEND_STOPS_ABOVE_CORE 22.0f
#define TOOLTIP_BEND_LENGTH 120.0f
#define LIGHTNING_POINT_COUNT 36
#define LIGHTNING_WAVES 14.0f
#define LIGHTNING_WAVE_STEP 1.1f
#define LIGHTNING_HALF_TURN 3.14159265f
#define LIGHTNING_BEND 9.0f
#define LIGHTNING_EDGE_RADIUS 7.5f
#define LIGHTNING_MIDDLE_RADIUS 4.0f
#define LIGHTNING_LUMP_COUNT 16u
#define LIGHTNING_LUMP_LIFE_STEPS 12u
#define LIGHTNING_LUMP_SPREAD 34.0f
#define LIGHTNING_LUMP_RADIUS_LEAST 8.0f
#define LIGHTNING_LUMP_RADIUS_SPREAD 16.0f
#define TOOLTIP_LINE_THICKNESS 2.0f
#define FILM_STRIP_TEXTURE "film_strip"
#define FILM_CORNER_TEXTURE "film_corner"
#define FILM_TILE_SIZE 45.0f
#define FILM_FIRST_TILE_X -31.5f
#define FILM_TILE_ABOVE_SCREEN 5.8f
#define FILM_CORNER_WIDTH 45.0f
#define FILM_CORNER_WHOLE_HEIGHT 56.25f
#define FILM_CORNER_FIRST_V 0.1625f
#define FILM_CORNER_TOP 31.5f
#define FILM_CORNER_BEYOND_SCREEN 19.7f
#define MOVIE_FRAMES_LOADED_AHEAD 4
#define MOVIE_NAME_EXTRA 20
#define SWIRL_TEXTURE "swirl_back"
#define SWIRL_PARTICLE_TEXTURE "swirl_particle"
#define SWIRL_SIZE 1200.0f
#define SWIRL_TURN_PER_TICK 0.004f
#define SWIRL_PARTICLE_COUNT 14
#define SWIRL_PARTICLE_LIFE_TICKS 300.0f
#define SWIRL_PARTICLE_REACH 560.0f
#define SWIRL_PARTICLE_SIZE_LEAST 10.0f
#define SWIRL_PARTICLE_SIZE_SPREAD 16.0f
#define SCANLINE_COUNT 135
#define SCANLINE_SPACING 4.0f
#define SCANLINE_HEIGHT 2.0f
#define SOUL_MOTION_DOT_COUNT 14
#define SOUL_BEAM_TEXTURE "soul_beam"
#define SOUL_BEAM_HALF_WIDTH 16.0f
#define SOUL_SPARKS_FLIPBOOK "soul_sparks"
#define SOUL_SPARKS_SIZE 340.0f
#define SOUL_STREAM_LENGTH 0.7f
#define SOUL_STREAM_RADIUS 18.0f
#define SOUL_STREAM_RADIUS_LEAST 3.0f

struct view_effect {
    int in_use;
    char name[VIEW_EFFECT_NAME_MAX];
    const struct script_effect_row *row;
    float x;
    float y;
    int age_ticks;
};

struct view_core_mark {
    float x;
    float y;
    unsigned int marked_at_tick;
    int is_set;
    int is_dead;
};

static const struct renderer_color movie_black = { 0.0f, 0.0f, 0.0f, 1.0f };
static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
static const struct renderer_color text_colors[SCRIPT_TALK_COLOR_COUNT] = {
    { 0.0f, 0.0f, 0.0f, 1.0f },
    { 0.902f, 0.22f, 0.141f, 1.0f },
    { 0.173f, 0.588f, 0.878f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f }
};

static const struct script_tables *tables;
static struct script_talk_view talk;
static int talk_is_showing;
static int soul_is_on;
static struct view_effect effects[VIEW_EFFECT_MAX];
static unsigned int age_ticks;
static int cursor_is_shown;
static int cursor_motion;
static const struct script_core_label *tooltip_label;
static float tooltip_alpha;
static float flash_alpha;
static int film_frame_is_on;
static int swirl_is_on;
static const struct script_movie *playing_movie;
static unsigned int movie_started_at_tick;
static int movie_last_ready_frame;
static int sparks_elapsed_ticks;
static float sparks_x;
static float sparks_y;
static float cursor_x;
static float cursor_y;
static float cursor_from_x;
static float cursor_from_y;
static struct view_core_mark core_marks[VIEW_CORE_MARK_MAX];

void script_view_start(const struct script_tables *new_tables)
{
    tables = new_tables;
    talk_is_showing = 0;
    soul_is_on = 0;
    cursor_is_shown = 0;
    age_ticks = 0;
    memset(effects, 0, sizeof(effects));
    scene_stage_set_soul_mode(0);
}

void script_view_talk_changed(const struct script_talk_view *view)
{
    if (view == NULL || view->page == NULL) {
        talk_is_showing = 0;
        return;
    }
    talk = *view;
    talk_is_showing = 1;
}

void script_view_soul_changed(int is_on)
{
    soul_is_on = is_on;
    scene_stage_set_soul_mode(is_on);
}

void script_view_cursor_moved(int is_shown, float x, float y, float from_x, float from_y, int motion)
{
    cursor_is_shown = is_shown;
    cursor_x = x;
    cursor_y = y;
    cursor_from_x = from_x;
    cursor_from_y = from_y;
    cursor_motion = motion;
}

void script_view_tooltip_changed(const char *object_name, float alpha)
{
    tooltip_alpha = alpha;
    tooltip_label = NULL;
    if (object_name != NULL && tables != NULL) {
        tooltip_label = script_tables_find_core_label(tables, object_name);
        if (tooltip_label == NULL) {
            tooltip_label = script_tables_find_core_label(tables, TOOLTIP_UNKNOWN_OBJECT);
        }
    }
}

void script_view_film_frame_changed(int is_on)
{
    film_frame_is_on = is_on;
}

void script_view_swirl_changed(int is_on)
{
    swirl_is_on = is_on;
}

void script_view_movie_started(const char *name)
{
    playing_movie = script_tables_find_movie(tables, name);
    movie_started_at_tick = age_ticks;
    movie_last_ready_frame = -1;
}

void script_view_soul_flash_changed(float alpha)
{
    flash_alpha = alpha;
}

void script_view_soul_sparks_changed(int elapsed_ticks, float x, float y)
{
    sparks_elapsed_ticks = elapsed_ticks;
    sparks_x = x;
    sparks_y = y;
}

void script_view_core_marked(int object_index, float x, float y, int is_dead)
{
    if (object_index >= 0 && object_index < VIEW_CORE_MARK_MAX) {
        core_marks[object_index].is_dead = is_dead;
        core_marks[object_index].x = x;
        core_marks[object_index].y = y;
        core_marks[object_index].marked_at_tick = age_ticks;
        core_marks[object_index].is_set = 1;
    }
}

static struct view_effect *find_free_effect(void)
{
    int index;

    for (index = 0; index < VIEW_EFFECT_MAX; index++) {
        if (!effects[index].in_use) {
            return &effects[index];
        }
    }
    return NULL;
}

void script_view_effect_spawned(const struct script_effect_row *row, const char *name, float x, float y)
{
    struct view_effect *effect;

    effect = find_free_effect();
    if (effect == NULL) {
        return;
    }
    effect->in_use = 1;
    snprintf(effect->name, sizeof(effect->name), "%s", name);
    effect->row = row;
    effect->x = x;
    effect->y = y;
    effect->age_ticks = 0;
}

void script_view_effect_deleted(const char *name)
{
    int index;

    for (index = 0; index < VIEW_EFFECT_MAX; index++) {
        if (effects[index].in_use && (name[0] == '*' || strcmp(effects[index].name, name) == 0)) {
            effects[index].in_use = 0;
        }
    }
}

void script_view_advance(void)
{
    int index;

    script_ui_view_advance();
    age_ticks = age_ticks + 1;
    for (index = 0; index < VIEW_EFFECT_MAX; index++) {
        if (effects[index].in_use) {
            effects[index].age_ticks = effects[index].age_ticks + 1;
            if (effects[index].row->ticks > 0 && effects[index].age_ticks >= effects[index].row->ticks) {
                effects[index].in_use = 0;
            }
        }
    }
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

static void draw_named_texture(const char *name, float x, float y, float width, float height, struct renderer_color tint)
{
    const struct cached_texture *texture;
    struct renderer_rect area;

    texture = texture_cache_use_or_start_loading(name);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    renderer_draw_sprite(texture->texture, area, whole_texture(), tint);
}

static void draw_flipbook(const struct script_flipbook *book, unsigned int age, float center_x, float center_y, float width, float height, struct renderer_color tint)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    struct renderer_rect uv;
    int frame;
    int cells;
    int page;
    int cell;

    if (book->ticks_per_frame <= 0 || book->frame_count <= 0 || book->columns <= 0 || book->rows <= 0) {
        return;
    }
    frame = (int)(age / (unsigned int)book->ticks_per_frame) % book->frame_count;
    cells = book->columns * book->rows;
    page = frame / cells;
    cell = frame % cells;
    if (page >= book->page_count) {
        return;
    }
    texture = texture_cache_use_or_start_loading(book->pages[page]);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    area.x = center_x - width / 2.0f;
    area.y = center_y - height / 2.0f;
    area.width = width;
    area.height = height;
    uv.x = (float)(cell % book->columns) * book->cell_width;
    uv.y = (float)(cell / book->columns) * book->cell_height;
    uv.width = book->cell_width;
    uv.height = book->cell_height;
    renderer_draw_sprite(texture->texture, area, uv, tint);
}

static void draw_effect(const struct view_effect *effect)
{
    const struct script_flipbook *book;
    float screen_x;
    float screen_y;
    float pulse;
    float size;

    scene_view_world_to_screen(scene_stage_view(), effect->x, effect->y, &screen_x, &screen_y);
    book = script_tables_find_flipbook(tables, effect->row->texture_name);
    if (book != NULL) {
        draw_flipbook(book, (unsigned int)effect->age_ticks, screen_x, screen_y, effect->row->width, effect->row->height, white);
        return;
    }
    pulse = (float)((unsigned int)effect->age_ticks % RING_PULSE_TICKS) / (float)RING_PULSE_TICKS;
    size = RING_SMALLEST_SIZE + (1.0f - RING_SMALLEST_SIZE) * pulse;
    draw_named_texture(effect->row->texture_name, screen_x - effect->row->width * size / 2.0f, screen_y - effect->row->height * size / 2.0f, effect->row->width * size, effect->row->height * size, white);
}

static void draw_effects(void)
{
    int index;

    for (index = 0; index < VIEW_EFFECT_MAX; index++) {
        if (effects[index].in_use && effects[index].row->kind == SCRIPT_EFFECT_RING && (soul_is_on || !effects[index].row->only_in_soul_view)) {
            draw_effect(&effects[index]);
        }
    }
}

static void draw_glyph(const struct cached_texture *font, const struct script_talk_glyph *glyph, float x, float y, struct renderer_color tint)
{
    struct renderer_rect screen_area;
    struct renderer_rect texture_area;

    if (glyph->width == 0) {
        return;
    }
    screen_area.x = x + (float)glyph->left;
    screen_area.y = y + (float)glyph->top;
    screen_area.width = (float)glyph->width;
    screen_area.height = (float)glyph->height;
    texture_area.x = (float)glyph->atlas_x / (float)font->width;
    texture_area.y = (float)glyph->atlas_y / (float)font->height;
    texture_area.width = (float)glyph->width / (float)font->width;
    texture_area.height = (float)glyph->height / (float)font->height;
    renderer_draw_sprite(font->texture, screen_area, texture_area, tint);
}

static int draw_talk_line(const struct cached_texture *font, const struct script_talk_book *book, const struct script_talk_line *line, float slide_y, float text_alpha, int glyphs_left)
{
    float x;
    int index;

    x = (float)line->x;
    for (index = 0; index < line->glyph_count && glyphs_left > 0; index++) {
        unsigned short code;
        const struct script_talk_glyph *glyph;
        struct renderer_color tint;

        code = book->text[line->first_text + index];
        glyph = &book->glyphs[code & SCRIPT_TALK_GLYPH_ID_MASK];
        tint = text_colors[(code >> SCRIPT_TALK_COLOR_SHIFT) & (SCRIPT_TALK_COLOR_COUNT - 1)];
        tint.alpha = text_alpha;
        draw_glyph(font, glyph, x, (float)line->y + slide_y, tint);
        x = x + (float)glyph->advance;
        glyphs_left = glyphs_left - 1;
    }
    return glyphs_left;
}

static void draw_talk_text(const struct script_talk_view *view, float slide_y)
{
    const struct cached_texture *font;
    const struct script_talk_page *page;
    int glyphs_left;
    int index;

    font = texture_cache_use_or_start_loading(view->book->font_texture);
    if (font == NULL || font->state != CACHED_TEXTURE_READY) {
        return;
    }
    page = view->page;
    glyphs_left = view->glyphs_shown;
    for (index = 0; index < page->line_count && glyphs_left > 0; index++) {
        glyphs_left = draw_talk_line(font, view->book, &view->book->lines[page->first_line + index], slide_y, view->text_alpha, glyphs_left);
    }
}

static void draw_face(const struct script_talk_page *page)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    struct renderer_rect uv;

    texture = texture_cache_use_or_start_loading(page->face_texture);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    area.x = page->face_is_flipped ? FACE_X : 0.0f;
    area.y = FACE_Y + (1.0f - talk.slide) * FACE_HEIGHT;
    area.width = FACE_WIDTH;
    area.height = FACE_HEIGHT;
    uv = whole_texture();
    if (!page->face_is_flipped) {
        uv.x = 1.0f;
        uv.width = -1.0f;
    }
    renderer_draw_sprite(texture->texture, area, uv, white);
}

static void draw_talk(void)
{
    const struct script_talk_page *page;
    float slide_y;
    float box_y;

    page = talk.page;
    slide_y = (1.0f - talk.slide) * (BOX_HEIGHT + BOX_TOP_Y);
    if (page->is_top_box) {
        slide_y = -slide_y;
        box_y = BOX_TOP_Y + slide_y;
        draw_named_texture(BOX_TOP_TEXTURE, BOX_TOP_X, box_y, BOX_TOP_WIDTH, BOX_HEIGHT, white);
    } else {
        box_y = BOX_BOTTOM_Y + slide_y;
        if (page->has_face) {
            draw_face(page);
        }
        draw_named_texture(BOX_BOTTOM_TEXTURE, BOX_BOTTOM_X, box_y, BOX_BOTTOM_WIDTH, BOX_HEIGHT, white);
    }
    draw_talk_text(&talk, slide_y);
    if (talk.cursor_is_on) {
        float bob;

        bob = (age_ticks % CURSOR_BOB_TICKS) < CURSOR_BOB_TICKS / 2u ? 0.0f : CURSOR_BOB_PIXELS;
        draw_named_texture(CURSOR_TEXTURE, (page->is_top_box ? BOX_TOP_X + BOX_TOP_WIDTH : BOX_BOTTOM_WIDTH) - CURSOR_MARGIN_RIGHT, box_y + BOX_HEIGHT - CURSOR_HEIGHT - CURSOR_MARGIN_BOTTOM + bob, CURSOR_WIDTH, CURSOR_HEIGHT, white);
    }
}

static void draw_centered(const char *texture_name, float center_x, float center_y, float size, struct renderer_color tint)
{
    draw_named_texture(texture_name, center_x - size / 2.0f, center_y - size / 2.0f, size, size, tint);
}

static void draw_core_marks(void)
{
    static const struct renderer_color dead_glow = { 1.0f, 0.72f, 0.25f, 0.85f };
    static const struct renderer_color dead_orb = { 1.0f, 1.0f, 0.86f, 1.0f };
    static const struct renderer_color glow = { 0.13f, 0.39f, 0.75f, 0.9f };
    static const struct renderer_color orb = { 0.73f, 0.74f, 0.94f, 1.0f };
    int index;

    for (index = 0; index < VIEW_CORE_MARK_MAX; index++) {
        float screen_x;
        float screen_y;

        if (!core_marks[index].is_set || age_ticks - core_marks[index].marked_at_tick > 1u) {
            continue;
        }
        scene_view_world_to_screen(scene_stage_view(), core_marks[index].x, core_marks[index].y, &screen_x, &screen_y);
        draw_centered(SOUL_GLOW_TEXTURE, screen_x, screen_y, CORE_GLOW_SIZE, core_marks[index].is_dead ? dead_glow : glow);
        draw_centered(SOUL_ORB_TEXTURE, screen_x, screen_y, CORE_ORB_SIZE, core_marks[index].is_dead ? dead_orb : orb);
    }
}

static void draw_blob(float center_x, float center_y, float radius)
{
    float size;

    size = 2.0f * radius / SOUL_BLOB_DISC_FRACTION;
    draw_named_texture(SOUL_BLOB_TEXTURE, center_x - size / 2.0f, center_y - size / 2.0f, size, size, white);
}

static float seeded_fraction(unsigned int seed, unsigned int salt)
{
    unsigned int value;

    value = (seed * 31u + salt) * 2654435761u;
    value = value ^ (value >> 13);
    return (float)((value >> 8) & 1023u) / 1023.0f;
}

static void draw_soul_base(float x, float y, unsigned int step)
{
    int index;

    draw_blob(x, y - SOUL_BASE_RISE, SOUL_BASE_RADIUS);
    for (index = 0; index < SOUL_BASE_LUMP_COUNT; index++) {
        float angle;
        float reach;

        angle = (float)index * SOUL_TWO_PI / (float)SOUL_BASE_LUMP_COUNT + seeded_fraction(step, (unsigned int)index) * 0.5f;
        reach = SOUL_BASE_LUMP_REACH + seeded_fraction(step, (unsigned int)index + 16u) * SOUL_BASE_LUMP_JITTER;
        draw_blob(x + cosf(angle) * reach, y - SOUL_BASE_RISE + sinf(angle) * reach, SOUL_BASE_LUMP_RADIUS);
    }
}

static void draw_soul_wisp(float x, float y, unsigned int step)
{
    unsigned int index;

    for (index = 0; index < SOUL_WISP_COUNT; index++) {
        unsigned int age;
        unsigned int cycle;
        unsigned int seed;
        float along;
        float rise;
        float sway;
        float radius;

        age = step + index * SOUL_WISP_LIFE_STEPS / SOUL_WISP_COUNT;
        cycle = age / SOUL_WISP_LIFE_STEPS;
        seed = index * 97u + cycle * 13u;
        along = (float)(age % SOUL_WISP_LIFE_STEPS) / (float)SOUL_WISP_LIFE_STEPS;
        rise = SOUL_WISP_START_RISE + SOUL_WISP_RISE * along * (0.75f + 0.5f * seeded_fraction(seed, 2u));
        sway = sinf(seeded_fraction(seed, 3u) * SOUL_TWO_PI + along * SOUL_WISP_SWAY_TURNS) * (SOUL_WISP_SWAY_NEAR + SOUL_WISP_SWAY_FAR * along);
        radius = (SOUL_WISP_RADIUS_LEAST + seeded_fraction(seed, 1u) * SOUL_WISP_RADIUS_SPREAD) * (1.0f - along);
        if (radius >= SOUL_WISP_RADIUS_SMALLEST) {
            draw_blob(x + sway, y - rise, radius);
        }
    }
}

static void draw_soul_body(float x, float y)
{
    unsigned int step;

    step = age_ticks / SOUL_ANIMATION_TICKS_PER_STEP;
    draw_soul_wisp(x, y, step);
    draw_soul_base(x, y, step);
}

static void draw_lightning(float from_x, float from_y, float to_x, float to_y);
static void draw_lightning_stream(float from_x, float from_y, float to_x, float to_y);
static void draw_soul_beam(float from_x, float from_y, float to_x, float to_y);

static void draw_soul_motion(float screen_x, float screen_y)
{
    float other_x;
    float other_y;
    int index;

    if (cursor_motion == SCRIPT_SOUL_RESTING) {
        return;
    }
    scene_view_world_to_screen(scene_stage_view(), cursor_from_x, cursor_from_y, &other_x, &other_y);
    if (cursor_motion == SCRIPT_SOUL_LIGHTNING) {
        draw_lightning_stream(screen_x, screen_y, other_x, other_y);
        draw_lightning(screen_x, screen_y, other_x, other_y);
        return;
    }
    if (cursor_motion == SCRIPT_SOUL_BEAM) {
        draw_soul_beam(screen_x, screen_y, other_x, other_y);
        return;
    }
    for (index = 1; index <= SOUL_MOTION_DOT_COUNT; index++) {
        float along;

        along = (float)index / (float)SOUL_MOTION_DOT_COUNT;
        draw_blob(screen_x + (other_x - screen_x) * along * SOUL_STREAM_LENGTH, screen_y + (other_y - screen_y) * along * SOUL_STREAM_LENGTH, SOUL_STREAM_RADIUS * (1.0f - along) + SOUL_STREAM_RADIUS_LEAST);
    }
}

static void draw_label_text(const char *texture_name, float width, float height, float x, float y, struct renderer_color tint)
{
    draw_named_texture(texture_name, x, y, width, height, tint);
}

static void draw_tooltip_line(float x, float y, float width, float height, struct renderer_color tint)
{
    struct renderer_rect line;

    line.x = x;
    line.y = y;
    line.width = width;
    line.height = height;
    renderer_draw_filled_rect(line, tint);
}

static void draw_tooltip(float screen_x, float screen_y)
{
    struct renderer_color tint;
    float line_end;

    if (tooltip_label == NULL) {
        return;
    }
    tint = white;
    tint.alpha = tooltip_alpha;
    draw_centered(TOOLTIP_RING_TEXTURE, screen_x, screen_y, TOOLTIP_RING_SIZE, tint);
    if (strcmp(tooltip_label->action_texture, TOOLTIP_NO_ACTION) == 0) {
        float corner_y;

        corner_y = screen_y - TOOLTIP_BEND_RISE;
        draw_tooltip_line(screen_x, corner_y, TOOLTIP_LINE_THICKNESS, TOOLTIP_BEND_RISE - TOOLTIP_BEND_STOPS_ABOVE_CORE, tint);
        draw_tooltip_line(screen_x - TOOLTIP_BEND_LENGTH, corner_y, TOOLTIP_BEND_LENGTH + TOOLTIP_LINE_THICKNESS, TOOLTIP_LINE_THICKNESS, tint);
        draw_label_text(tooltip_label->name_texture, tooltip_label->name_width, tooltip_label->name_height, screen_x - TOOLTIP_BEND_LENGTH, corner_y - tooltip_label->name_height, tint);
        return;
    }
    line_end = screen_x + TOOLTIP_LINE_END;
    draw_tooltip_line(screen_x + TOOLTIP_LINE_START, screen_y - TOOLTIP_LINE_THICKNESS / 2.0f, TOOLTIP_LINE_END - TOOLTIP_LINE_START, TOOLTIP_LINE_THICKNESS, tint);
    draw_label_text(tooltip_label->name_texture, tooltip_label->name_width, tooltip_label->name_height, line_end - tooltip_label->name_width, screen_y - tooltip_label->name_height, tint);
    draw_label_text(tooltip_label->action_texture, tooltip_label->action_width, tooltip_label->action_height, line_end - tooltip_label->action_width, screen_y, tint);
}

static void draw_flat_dot(float center_x, float center_y, float radius, struct renderer_color color)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    float size;

    texture = texture_cache_use_or_start_loading(SOUL_BLOB_TEXTURE);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    size = 2.0f * radius / SOUL_BLOB_DISC_FRACTION;
    area.x = center_x - size / 2.0f;
    area.y = center_y - size / 2.0f;
    area.width = size;
    area.height = size;
    renderer_draw_flat_sprite(texture->texture, area, whole_texture(), color);
}

static void draw_lightning(float from_x, float from_y, float to_x, float to_y)
{
    static const struct renderer_color edge = { 0.05f, 0.62f, 0.12f, 1.0f };
    static const struct renderer_color middle = { 0.93f, 0.95f, 0.08f, 1.0f };
    unsigned int step;
    float length;
    float side_x;
    float side_y;
    int pass;

    step = age_ticks / SOUL_ANIMATION_TICKS_PER_STEP;
    length = sqrtf((to_x - from_x) * (to_x - from_x) + (to_y - from_y) * (to_y - from_y));
    if (length < 1.0f) {
        return;
    }
    side_x = -(to_y - from_y) / length;
    side_y = (to_x - from_x) / length;
    for (pass = 0; pass < 2; pass++) {
        int index;

        for (index = 0; index <= LIGHTNING_POINT_COUNT; index++) {
            float along;
            float bend;

            along = (float)index / (float)LIGHTNING_POINT_COUNT;
            bend = sinf(along * LIGHTNING_WAVES + (float)step * LIGHTNING_WAVE_STEP) * sinf(along * LIGHTNING_HALF_TURN) * LIGHTNING_BEND;
            draw_flat_dot(from_x + (to_x - from_x) * along + side_x * bend, from_y + (to_y - from_y) * along + side_y * bend, pass == 0 ? LIGHTNING_EDGE_RADIUS : LIGHTNING_MIDDLE_RADIUS, pass == 0 ? edge : middle);
        }
    }
}

static void draw_lightning_stream(float from_x, float from_y, float to_x, float to_y)
{
    unsigned int step;
    unsigned int index;

    step = age_ticks / SOUL_ANIMATION_TICKS_PER_STEP;
    for (index = 0; index < LIGHTNING_LUMP_COUNT; index++) {
        unsigned int age;
        unsigned int seed;
        float along;

        age = step + index * LIGHTNING_LUMP_LIFE_STEPS / LIGHTNING_LUMP_COUNT;
        seed = index * 53u + (age / LIGHTNING_LUMP_LIFE_STEPS) * 11u;
        along = (float)(age % LIGHTNING_LUMP_LIFE_STEPS) / (float)LIGHTNING_LUMP_LIFE_STEPS;
        draw_blob(from_x + (to_x - from_x) * along + (seeded_fraction(seed, 4u) - 0.5f) * LIGHTNING_LUMP_SPREAD, from_y + (to_y - from_y) * along + (seeded_fraction(seed, 5u) - 0.5f) * LIGHTNING_LUMP_SPREAD, LIGHTNING_LUMP_RADIUS_LEAST + seeded_fraction(seed, 6u) * LIGHTNING_LUMP_RADIUS_SPREAD);
    }
}

static void draw_possession_sparks(void)
{
    const struct script_flipbook *sparks;
    float screen_x;
    float screen_y;

    sparks = script_tables_find_flipbook(tables, SOUL_SPARKS_FLIPBOOK);
    if (sparks_elapsed_ticks <= 0 || sparks == NULL) {
        return;
    }
    scene_view_world_to_screen(scene_stage_view(), sparks_x, sparks_y, &screen_x, &screen_y);
    draw_flipbook(sparks, (unsigned int)sparks_elapsed_ticks, screen_x, screen_y, SOUL_SPARKS_SIZE, SOUL_SPARKS_SIZE, white);
}

static void draw_soul_beam(float from_x, float from_y, float to_x, float to_y)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(SOUL_BEAM_TEXTURE);
    if (texture != NULL && texture->state == CACHED_TEXTURE_READY) {
        renderer_draw_beam(texture->texture, from_x, from_y, to_x, to_y, SOUL_BEAM_HALF_WIDTH, white);
    }
}

static void draw_scanlines(void)
{
    static const struct renderer_color line_color = { 0.0f, 0.0f, 0.0f, 0.16f };
    struct renderer_rect lines[SCANLINE_COUNT];
    int index;

    for (index = 0; index < SCANLINE_COUNT; index++) {
        lines[index].x = 0.0f;
        lines[index].y = (float)index * SCANLINE_SPACING;
        lines[index].width = (float)RENDERER_WIDTH;
        lines[index].height = SCANLINE_HEIGHT;
    }
    renderer_draw_filled_rects(lines, SCANLINE_COUNT, line_color);
}

static void draw_soul(void)
{
    static const struct renderer_color orb = { 0.73f, 0.74f, 0.94f, 1.0f };
    float screen_x;
    float screen_y;

    if (!soul_is_on || tables == NULL) {
        return;
    }
    draw_scanlines();
    draw_core_marks();
    if (!cursor_is_shown) {
        return;
    }
    scene_view_world_to_screen(scene_stage_view(), cursor_x, cursor_y, &screen_x, &screen_y);
    draw_soul_motion(screen_x, screen_y);
    draw_soul_body(screen_x, screen_y);
    draw_centered(SOUL_ORB_TEXTURE, screen_x, screen_y, SOUL_ORB_SIZE, orb);
    draw_tooltip(screen_x, screen_y);
    draw_possession_sparks();
    if (flash_alpha > 0.0f) {
        struct renderer_rect whole_screen;
        struct renderer_color flash;

        whole_screen.x = 0.0f;
        whole_screen.y = 0.0f;
        whole_screen.width = (float)RENDERER_WIDTH;
        whole_screen.height = (float)RENDERER_HEIGHT;
        flash = white;
        flash.alpha = flash_alpha;
        renderer_draw_filled_rect(whole_screen, flash);
    }
}

static void draw_swirl(void)
{
    const struct cached_texture *texture;
    float angle;
    float reach_x;
    float reach_y;
    float center_x;
    float center_y;
    int index;

    if (!swirl_is_on) {
        return;
    }
    texture = texture_cache_use_or_start_loading(SWIRL_TEXTURE);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    center_x = (float)RENDERER_WIDTH / 2.0f;
    center_y = (float)RENDERER_HEIGHT / 2.0f;
    angle = (float)age_ticks * SWIRL_TURN_PER_TICK;
    reach_x = cosf(angle) * SWIRL_SIZE / 2.0f;
    reach_y = sinf(angle) * SWIRL_SIZE / 2.0f;
    renderer_draw_beam(texture->texture, center_x - reach_x, center_y - reach_y, center_x + reach_x, center_y + reach_y, SWIRL_SIZE / 2.0f, white);
    for (index = 0; index < SWIRL_PARTICLE_COUNT; index++) {
        float phase;
        float direction;
        float distance;
        float size;

        phase = (float)age_ticks / SWIRL_PARTICLE_LIFE_TICKS + seeded_fraction((unsigned int)index, 9u);
        phase = phase - floorf(phase);
        direction = seeded_fraction((unsigned int)index, 10u) * SOUL_TWO_PI + (float)age_ticks * SWIRL_TURN_PER_TICK;
        distance = phase * SWIRL_PARTICLE_REACH;
        size = SWIRL_PARTICLE_SIZE_LEAST + seeded_fraction((unsigned int)index, 11u) * SWIRL_PARTICLE_SIZE_SPREAD;
        draw_centered(SWIRL_PARTICLE_TEXTURE, center_x + cosf(direction) * distance, center_y + sinf(direction) * distance, size, white);
    }
}

static void draw_film_corner(float x, int is_at_left, int is_at_bottom)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    struct renderer_rect part;

    texture = texture_cache_use_or_start_loading(FILM_CORNER_TEXTURE);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    area.x = x;
    area.width = FILM_CORNER_WIDTH;
    area.height = FILM_CORNER_WHOLE_HEIGHT * (1.0f - FILM_CORNER_FIRST_V);
    area.y = is_at_bottom ? (float)RENDERER_HEIGHT - FILM_CORNER_TOP - area.height : FILM_CORNER_TOP;
    part.x = is_at_left ? 1.0f : 0.0f;
    part.width = is_at_left ? -1.0f : 1.0f;
    part.y = is_at_bottom ? 1.0f : FILM_CORNER_FIRST_V;
    part.height = is_at_bottom ? FILM_CORNER_FIRST_V - 1.0f : 1.0f - FILM_CORNER_FIRST_V;
    renderer_draw_sprite(texture->texture, area, part, white);
}

static void draw_film_frame(void)
{
    float left_corner_x;
    float right_corner_x;
    float tile_x;

    if (!film_frame_is_on) {
        return;
    }
    for (tile_x = FILM_FIRST_TILE_X; tile_x < (float)RENDERER_WIDTH; tile_x += FILM_TILE_SIZE) {
        draw_named_texture(FILM_STRIP_TEXTURE, tile_x, -FILM_TILE_ABOVE_SCREEN, FILM_TILE_SIZE, FILM_TILE_SIZE, white);
        draw_named_texture(FILM_STRIP_TEXTURE, tile_x, (float)RENDERER_HEIGHT - FILM_TILE_SIZE + FILM_TILE_ABOVE_SCREEN, FILM_TILE_SIZE, FILM_TILE_SIZE, white);
    }
    left_corner_x = -FILM_CORNER_BEYOND_SCREEN;
    right_corner_x = (float)RENDERER_WIDTH + FILM_CORNER_BEYOND_SCREEN - FILM_CORNER_WIDTH;
    draw_film_corner(left_corner_x, 1, 0);
    draw_film_corner(right_corner_x, 0, 0);
    draw_film_corner(left_corner_x, 1, 1);
    draw_film_corner(right_corner_x, 0, 1);
}

static int movie_frame_is_ready(int frame, const struct cached_texture **texture)
{
    char name[ASSET_NAME_MAX + MOVIE_NAME_EXTRA];

    snprintf(name, sizeof(name), "movie_%s_%03d", playing_movie->name, frame);
    *texture = texture_cache_use_or_start_loading(name);
    return *texture != NULL && (*texture)->state == CACHED_TEXTURE_READY;
}

static void draw_movie(void)
{
    const struct cached_texture *texture;
    struct renderer_rect area;
    int frame;
    int ahead;

    if (playing_movie == NULL) {
        return;
    }
    frame = (int)((age_ticks - movie_started_at_tick) / (unsigned int)playing_movie->ticks_per_frame);
    if (frame >= playing_movie->frame_count) {
        playing_movie = NULL;
        return;
    }
    for (ahead = 1; ahead <= MOVIE_FRAMES_LOADED_AHEAD && frame + ahead < playing_movie->frame_count; ahead++) {
        movie_frame_is_ready(frame + ahead, &texture);
    }
    area.x = 0.0f;
    area.y = 0.0f;
    area.width = (float)RENDERER_WIDTH;
    area.height = (float)RENDERER_HEIGHT;
    if (movie_frame_is_ready(frame, &texture)) {
        movie_last_ready_frame = frame;
    } else if (movie_last_ready_frame < 0 || !movie_frame_is_ready(movie_last_ready_frame, &texture)) {
        renderer_draw_filled_rect(area, movie_black);
        return;
    }
    renderer_draw_sprite(texture->texture, area, whole_texture(), white);
}

void script_view_draw_on_world(void)
{
    draw_effects();
    draw_swirl();
    draw_film_frame();
    draw_soul();
}

void script_view_draw_on_covers(void)
{
    if (talk_is_showing && talk.page != NULL && talk.book != NULL) {
        draw_talk();
    }
    script_ui_view_draw();
    draw_movie();
}
