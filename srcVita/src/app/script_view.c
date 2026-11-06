#include <math.h>
#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_talk.h"
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
#define SCANLINE_COUNT 135
#define SCANLINE_SPACING 4.0f
#define SCANLINE_HEIGHT 2.0f
#define DRAG_DOT_COUNT 10
#define DRAG_DOT_SIZE 30.0f

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
};

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
static int cursor_is_dragging;
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

void script_view_cursor_moved(int is_shown, float x, float y, float from_x, float from_y, int is_dragging)
{
    cursor_is_shown = is_shown;
    cursor_x = x;
    cursor_y = y;
    cursor_from_x = from_x;
    cursor_from_y = from_y;
    cursor_is_dragging = is_dragging;
}

void script_view_core_marked(int object_index, float x, float y)
{
    if (object_index >= 0 && object_index < VIEW_CORE_MARK_MAX) {
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
        draw_centered(SOUL_GLOW_TEXTURE, screen_x, screen_y, CORE_GLOW_SIZE, glow);
        draw_centered(SOUL_ORB_TEXTURE, screen_x, screen_y, CORE_ORB_SIZE, orb);
    }
}

static void draw_drag_line(void)
{
    static const struct renderer_color dot = { 0.45f, 0.9f, 1.0f, 0.9f };
    int index;

    if (!cursor_is_dragging) {
        return;
    }
    for (index = 0; index < DRAG_DOT_COUNT; index++) {
        float along;
        float screen_x;
        float screen_y;

        along = (float)index / (float)DRAG_DOT_COUNT;
        scene_view_world_to_screen(scene_stage_view(), cursor_from_x + (cursor_x - cursor_from_x) * along, cursor_from_y + (cursor_y - cursor_from_y) * along, &screen_x, &screen_y);
        draw_centered(SOUL_ORB_TEXTURE, screen_x, screen_y, DRAG_DOT_SIZE, dot);
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
    int index;

    for (index = 0; index < SOUL_WISP_COUNT; index++) {
        unsigned int age;
        unsigned int cycle;
        unsigned int seed;
        float along;
        float rise;
        float sway;
        float radius;

        age = step + (unsigned int)index * SOUL_WISP_LIFE_STEPS / SOUL_WISP_COUNT;
        cycle = age / SOUL_WISP_LIFE_STEPS;
        seed = (unsigned int)index * 97u + cycle * 13u;
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
    draw_drag_line();
    scene_view_world_to_screen(scene_stage_view(), cursor_x, cursor_y, &screen_x, &screen_y);
    draw_soul_body(screen_x, screen_y);
    draw_centered(SOUL_ORB_TEXTURE, screen_x, screen_y, SOUL_ORB_SIZE, orb);
}

void script_view_draw(void)
{
    draw_effects();
    draw_soul();
    if (talk_is_showing && talk.page != NULL && talk.book != NULL) {
        draw_talk();
    }
}
