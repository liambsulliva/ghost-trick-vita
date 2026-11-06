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
#define SOUL_CURSOR_TEXTURE "soul_ring"
#define SOUL_CURSOR_SIZE 120.0f

struct view_effect {
    int in_use;
    char name[VIEW_EFFECT_NAME_MAX];
    const struct script_effect_row *row;
    float x;
    float y;
    int age_ticks;
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
static float cursor_x;
static float cursor_y;

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

void script_view_cursor_moved(int is_shown, float x, float y)
{
    cursor_is_shown = is_shown;
    cursor_x = x;
    cursor_y = y;
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

static void draw_effect(const struct view_effect *effect)
{
    float screen_x;
    float screen_y;
    float pulse;
    float size;

    scene_view_world_to_screen(scene_stage_view(), effect->x, effect->y, &screen_x, &screen_y);
    pulse = (float)((unsigned int)effect->age_ticks % RING_PULSE_TICKS) / (float)RING_PULSE_TICKS;
    size = RING_SMALLEST_SIZE + (1.0f - RING_SMALLEST_SIZE) * pulse;
    draw_named_texture(effect->row->texture_name, screen_x - effect->row->width * size / 2.0f, screen_y - effect->row->height * size / 2.0f, effect->row->width * size, effect->row->height * size, white);
}

static void draw_effects(void)
{
    int index;

    for (index = 0; index < VIEW_EFFECT_MAX; index++) {
        if (effects[index].in_use && effects[index].row->kind == SCRIPT_EFFECT_RING) {
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
    area.x = page->face_is_flipped ? 0.0f : FACE_X;
    area.y = FACE_Y + (1.0f - talk.slide) * FACE_HEIGHT;
    area.width = FACE_WIDTH;
    area.height = FACE_HEIGHT;
    uv = whole_texture();
    if (page->face_is_flipped) {
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

static void draw_soul_cursor(void)
{
    float screen_x;
    float screen_y;
    float pulse;
    float size;

    if (!soul_is_on || !cursor_is_shown) {
        return;
    }
    scene_view_world_to_screen(scene_stage_view(), cursor_x, cursor_y, &screen_x, &screen_y);
    pulse = (float)(age_ticks % RING_PULSE_TICKS) / (float)RING_PULSE_TICKS;
    size = SOUL_CURSOR_SIZE * (RING_SMALLEST_SIZE + (1.0f - RING_SMALLEST_SIZE) * pulse);
    draw_named_texture(SOUL_CURSOR_TEXTURE, screen_x - size * 0.5f, screen_y - size * 0.5f, size, size, white);
}

void script_view_draw(void)
{
    draw_effects();
    draw_soul_cursor();
    if (talk_is_showing && talk.page != NULL && talk.book != NULL) {
        draw_talk();
    }
}
