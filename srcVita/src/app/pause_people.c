#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset_name.h"
#include "file.h"
#include "file_path.h"
#include "input.h"
#include "memory.h"
#include "pause_people.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"
#include "script_machine.h"
#include "text_lines.h"
#include "texture_cache.h"

#define TABLE_PATH "pause/people.txt"
#define THUMB_SHEET "pause_people_thumbs"
#define SPOT_NAME "pause_people_spot"
#define LINE_MAX 512
#define ID_MOST SCRIPT_NOTE_ID_COUNT
#define NOTES_MOST 512
#define THUMBS_MOST 96
#define FIRST_MOST 8
#define DEFAULT_LEVEL 10
#define PEOPLE_LIST 1
#define PLACES_LIST 2
#define TAB_LEFT 300.0f
#define TAB_STEP 190.0f
#define TAB_TOP 12.0f
#define TAB_WIDTH 170.0f
#define TAB_HEIGHT 40.0f
#define SHOULDER_L_X 262.0f
#define SHOULDER_R_X 698.0f
#define SHOULDER_HALF_WIDTH 22.0f
#define SHOULDER_HALF_HEIGHT 14.0f
#define SHOULDER_GLYPH_SIZE 36.0f
#define TAB_TEXT_Y 32.0f
#define FILM_TOP 64.0f
#define FILM_HEIGHT 102.0f
#define SPROCKET_FIRST 6.0f
#define SPROCKET_STEP 22.0f
#define SPROCKET_WIDTH 11.0f
#define SPROCKET_HEIGHT 7.0f
#define SPROCKET_TOP_INSET 5.0f
#define SPROCKET_BOTTOM_INSET 12.0f
#define THUMB_WIDTH 104.0f
#define THUMB_HEIGHT 78.0f
#define THUMB_TOP 76.0f
#define FRAME_SPILL 6.0f
#define SPOT_LEFT 26.0f
#define SPOT_TOP 178.0f
#define ART_LEFT 60.0f
#define ART_TOP 190.0f
#define ART_WIDTH 372.0f
#define ART_HEIGHT 280.0f
#define ART_EDGE 3.0f
#define CARD_LEFT 486.0f
#define CARD_TOP 178.0f
#define CARD_WIDTH 448.0f
#define BAND_HEIGHT 48.0f
#define BOX_TOP 236.0f
#define BOX_HEIGHT 246.0f

struct note_card {
    int list;
    int id;
    int level;
    char texture[ASSET_NAME_MAX];
};

struct thumb {
    char name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

static int order[ID_MOST];
static int order_count;
static unsigned char skips[2][ID_MOST];
static int firsts[2][FIRST_MOST];
static int first_counts[2];
static int place_count;
static char arts[2][ID_MOST][ASSET_NAME_MAX];
static struct note_card cards[NOTES_MOST];
static int card_count;
static struct thumb thumbs[THUMBS_MOST];
static int thumb_count;
static int is_loaded;
static const unsigned char *note_levels;
static int rows[2][ID_MOST];
static int levels[2][ID_MOST];
static int counts[2];
static int tab;
static int selected[2];
static int first_slot[2];

static int read_numbers(const char *text, int *numbers, int most)
{
    char *end;
    long value;
    int count;

    count = 0;
    while (count < most) {
        value = strtol(text, &end, 10);
        if (end == text) {
            break;
        }
        numbers[count] = (int)value;
        count = count + 1;
        text = end;
    }
    return count;
}

static void read_skips(const char *text, unsigned char *flags)
{
    int numbers[ID_MOST];
    int count;
    int index;

    count = read_numbers(text, numbers, ID_MOST);
    for (index = 0; index < count; index++) {
        if (numbers[index] > 0 && numbers[index] < ID_MOST) {
            flags[numbers[index]] = 1;
        }
    }
}

static void read_art(int side, const char *text)
{
    int id;
    char name[ASSET_NAME_MAX];

    if (sscanf(text, "%d %95s", &id, name) == 2 && id > 0 && id < ID_MOST) {
        snprintf(arts[side][id], ASSET_NAME_MAX, "%s", name);
    }
}

static void read_card(const char *text)
{
    struct note_card card;

    if (card_count >= NOTES_MOST || sscanf(text, "%d %d %d %95s", &card.list, &card.id, &card.level, card.texture) != 4) {
        return;
    }
    cards[card_count] = card;
    card_count = card_count + 1;
}

static void read_thumb(const char *text)
{
    struct thumb thumb;

    if (thumb_count >= THUMBS_MOST || sscanf(text, "%95s %f %f %f %f", thumb.name, &thumb.x, &thumb.y, &thumb.width, &thumb.height) != 5) {
        return;
    }
    thumbs[thumb_count] = thumb;
    thumb_count = thumb_count + 1;
}

static void forget_table(void)
{
    order_count = 0;
    memset(skips, 0, sizeof(skips));
    first_counts[0] = 0;
    first_counts[1] = 0;
    place_count = 0;
    memset(arts, 0, sizeof(arts));
    card_count = 0;
    thumb_count = 0;
    is_loaded = 0;
}

int pause_people_read_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    unsigned int offset;

    forget_table();
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (text_line_starts_with(line, "order ")) {
            order_count = read_numbers(line + 6, order, ID_MOST);
        } else if (text_line_starts_with(line, "skip_people ")) {
            read_skips(line + 12, skips[PAUSE_PEOPLE_TAB_PEOPLE]);
        } else if (text_line_starts_with(line, "skip_places ")) {
            read_skips(line + 12, skips[PAUSE_PEOPLE_TAB_PLACES]);
        } else if (text_line_starts_with(line, "first_people ")) {
            first_counts[PAUSE_PEOPLE_TAB_PEOPLE] = read_numbers(line + 13, firsts[PAUSE_PEOPLE_TAB_PEOPLE], FIRST_MOST);
        } else if (text_line_starts_with(line, "first_places ")) {
            first_counts[PAUSE_PEOPLE_TAB_PLACES] = read_numbers(line + 13, firsts[PAUSE_PEOPLE_TAB_PLACES], FIRST_MOST);
        } else if (text_line_starts_with(line, "place_count ")) {
            place_count = atoi(line + 12);
        } else if (text_line_starts_with(line, "face ")) {
            read_art(PAUSE_PEOPLE_TAB_PEOPLE, line + 5);
        } else if (text_line_starts_with(line, "place ")) {
            read_art(PAUSE_PEOPLE_TAB_PLACES, line + 6);
        } else if (text_line_starts_with(line, "note ")) {
            read_card(line + 5);
        } else if (text_line_starts_with(line, "thumb ")) {
            read_thumb(line + 6);
        }
    }
    if (place_count >= ID_MOST) {
        place_count = ID_MOST - 1;
    }
    is_loaded = order_count > 0 && card_count > 0 && thumb_count > 0;
    return is_loaded ? card_count : 0;
}

int pause_people_load(const char *assets_folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    int count;

    forget_table();
    if (!file_path_join(path, assets_folder, TABLE_PATH, "") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    count = pause_people_read_text((const char *)bytes, size);
    mem_free(bytes);
    return count;
}

int pause_people_is_loaded(void)
{
    return is_loaded;
}

void pause_people_use_notes(const unsigned char *levels_by_list)
{
    note_levels = levels_by_list;
}

const unsigned char *pause_people_every_note(void)
{
    static unsigned char every[SCRIPT_NOTE_LIST_COUNT * SCRIPT_NOTE_ID_COUNT];

    memset(every, 0xff, sizeof(every));
    return every;
}

static int level_of(int list, int id)
{
    unsigned char stored;

    if (note_levels == NULL || id <= 0 || id >= ID_MOST) {
        return -1;
    }
    stored = note_levels[list * SCRIPT_NOTE_ID_COUNT + id];
    return stored == 0 ? -1 : (int)stored - 1;
}

static void add_row(int side, int id, int level)
{
    if (counts[side] >= ID_MOST) {
        return;
    }
    rows[side][counts[side]] = id;
    levels[side][counts[side]] = level;
    counts[side] = counts[side] + 1;
}

static void build_lists(void)
{
    int side;
    int index;
    int level;

    counts[PAUSE_PEOPLE_TAB_PEOPLE] = 0;
    counts[PAUSE_PEOPLE_TAB_PLACES] = 0;
    for (index = 0; index < order_count; index++) {
        level = level_of(PEOPLE_LIST, order[index]);
        if (level >= 0 && order[index] < ID_MOST && !skips[PAUSE_PEOPLE_TAB_PEOPLE][order[index]]) {
            add_row(PAUSE_PEOPLE_TAB_PEOPLE, order[index], level);
        }
    }
    for (index = 1; index <= place_count; index++) {
        level = level_of(PLACES_LIST, index);
        if (level >= 0 && !skips[PAUSE_PEOPLE_TAB_PLACES][index]) {
            add_row(PAUSE_PEOPLE_TAB_PLACES, index, level);
        }
    }
    for (side = 0; side < 2; side++) {
        if (counts[side] == 0) {
            for (index = 0; index < first_counts[side]; index++) {
                add_row(side, firsts[side][index], DEFAULT_LEVEL);
            }
        }
    }
}

void pause_people_enter(void)
{
    build_lists();
    tab = PAUSE_PEOPLE_TAB_PEOPLE;
    selected[0] = 0;
    selected[1] = 0;
    first_slot[0] = 0;
    first_slot[1] = 0;
    run_log_write("pause people people=%d places=%d", counts[0], counts[1]);
}

static void keep_in_strip(void)
{
    if (selected[tab] < first_slot[tab]) {
        first_slot[tab] = selected[tab];
    }
    if (selected[tab] >= first_slot[tab] + PAUSE_PEOPLE_SLOTS) {
        first_slot[tab] = selected[tab] - PAUSE_PEOPLE_SLOTS + 1;
    }
}

static void select_entry(int index)
{
    if (index < 0 || index >= counts[tab]) {
        return;
    }
    selected[tab] = index;
    keep_in_strip();
}

static void switch_tab(void)
{
    tab = tab == PAUSE_PEOPLE_TAB_PEOPLE ? PAUSE_PEOPLE_TAB_PLACES : PAUSE_PEOPLE_TAB_PEOPLE;
}

int pause_people_step(unsigned int pressed)
{
    if (pressed & INPUT_CIRCLE) {
        return PAUSE_PEOPLE_LEAVES;
    }
    if (pressed & (INPUT_L | INPUT_R)) {
        switch_tab();
    } else if (pressed & INPUT_LEFT) {
        select_entry(selected[tab] - 1);
    } else if (pressed & INPUT_RIGHT) {
        select_entry(selected[tab] + 1);
    }
    return PAUSE_PEOPLE_STAYS;
}

static float slot_left(int slot)
{
    float gap;

    gap = ((float)RENDERER_WIDTH - PAUSE_PEOPLE_SLOTS * THUMB_WIDTH) / (PAUSE_PEOPLE_SLOTS + 1);
    return floorf(gap + (float)slot * (THUMB_WIDTH + gap) + 0.5f);
}

static int is_inside(float x, float y, float left, float top, float width, float height)
{
    return x >= left && x < left + width && y >= top && y < top + height;
}

unsigned int pause_people_touch(int lands, float x, float y)
{
    int slot;

    if (!lands) {
        return 0;
    }
    if (pause_is_on_back_pill(x, y)) {
        return INPUT_CIRCLE;
    }
    for (slot = 0; slot < 2; slot++) {
        if (is_inside(x, y, TAB_LEFT + (float)slot * TAB_STEP, TAB_TOP, TAB_WIDTH, TAB_HEIGHT)) {
            tab = slot;
            return 0;
        }
    }
    if (is_inside(x, y, SHOULDER_L_X - SHOULDER_HALF_WIDTH, TAB_TEXT_Y - SHOULDER_HALF_HEIGHT, 2.0f * SHOULDER_HALF_WIDTH, 2.0f * SHOULDER_HALF_HEIGHT) || is_inside(x, y, SHOULDER_R_X - SHOULDER_HALF_WIDTH, TAB_TEXT_Y - SHOULDER_HALF_HEIGHT, 2.0f * SHOULDER_HALF_WIDTH, 2.0f * SHOULDER_HALF_HEIGHT)) {
        switch_tab();
        return 0;
    }
    for (slot = 0; slot < PAUSE_PEOPLE_SLOTS; slot++) {
        if (is_inside(x, y, slot_left(slot), THUMB_TOP, THUMB_WIDTH, THUMB_HEIGHT)) {
            select_entry(first_slot[tab] + slot);
            return 0;
        }
    }
    return 0;
}

int pause_people_tab(void)
{
    return tab;
}

int pause_people_selected(void)
{
    return selected[tab];
}

int pause_people_first_slot(void)
{
    return first_slot[tab];
}

int pause_people_count(int which)
{
    return which == 0 || which == 1 ? counts[which] : 0;
}

int pause_people_entry(int which, int index)
{
    return (which == 0 || which == 1) && index >= 0 && index < counts[which] ? rows[which][index] : 0;
}

int pause_people_level(int which, int index)
{
    return (which == 0 || which == 1) && index >= 0 && index < counts[which] ? levels[which][index] : -1;
}

static const char *card_for(int side, int id, int level)
{
    const struct note_card *best;
    const struct note_card *lowest;
    int list;
    int index;

    list = side == PAUSE_PEOPLE_TAB_PEOPLE ? PEOPLE_LIST : PLACES_LIST;
    best = NULL;
    lowest = NULL;
    for (index = 0; index < card_count; index++) {
        const struct note_card *card = &cards[index];

        if (card->list != list || card->id != id) {
            continue;
        }
        if (lowest == NULL || card->level < lowest->level) {
            lowest = card;
        }
        if (card->level <= level && (best == NULL || card->level > best->level)) {
            best = card;
        }
    }
    best = best != NULL ? best : lowest;
    return best != NULL ? best->texture : NULL;
}

static const char *art_for(int side, int id)
{
    if (id <= 0 || id >= ID_MOST || arts[side][id][0] == '\0') {
        return NULL;
    }
    return arts[side][id];
}

const char *pause_people_card(void)
{
    if (counts[tab] == 0) {
        return NULL;
    }
    return card_for(tab, rows[tab][selected[tab]], levels[tab][selected[tab]]);
}

const char *pause_people_art(void)
{
    if (counts[tab] == 0) {
        return NULL;
    }
    return art_for(tab, rows[tab][selected[tab]]);
}

static const struct thumb *thumb_named(const char *name)
{
    int index;

    if (name == NULL) {
        name = "unknown";
    }
    for (index = 0; index < thumb_count; index++) {
        if (strcmp(thumbs[index].name, name) == 0) {
            return &thumbs[index];
        }
    }
    return NULL;
}

static void draw_thumb(const char *name, float x, float y, float width, float height)
{
    const struct thumb *thumb;

    thumb = thumb_named(name);
    if (thumb == NULL) {
        thumb = thumb_named("unknown");
    }
    if (thumb != NULL) {
        pause_draw_from(THUMB_SHEET, thumb->x, thumb->y, thumb->width, thumb->height, x, y, width, height, 1.0f);
    }
}

static void draw_whole_texture(const char *name, float x, float y)
{
    const struct cached_texture *texture;

    if (name == NULL) {
        return;
    }
    texture = texture_cache_use_or_start_loading(name);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    pause_draw_from(name, 0.0f, 0.0f, (float)texture->width, (float)texture->height, x, y, (float)texture->width, (float)texture->height, 1.0f);
}

static struct renderer_rect area(float x, float y, float width, float height)
{
    struct renderer_rect rect;

    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    return rect;
}

static struct renderer_color color_of(int red, int green, int blue)
{
    struct renderer_color color;

    color.red = (float)red / 255.0f;
    color.green = (float)green / 255.0f;
    color.blue = (float)blue / 255.0f;
    color.alpha = 1.0f;
    return color;
}

static void draw_tabs(float x_offset)
{
    int index;
    float left;

    for (index = 0; index < 2; index++) {
        left = x_offset + TAB_LEFT + (float)index * TAB_STEP;
        pause_draw_slice(index == tab ? "tab_selected" : "tab", left, TAB_TOP, TAB_WIDTH, TAB_HEIGHT, 1.0f);
        pause_draw_text(index == 0 ? "bar.people_tab" : "bar.phone_tab", left + TAB_WIDTH / 2.0f, TAB_TEXT_Y, PAUSE_ANCHOR_CENTRE, 1.0f);
    }
    pause_draw_glyph("l", x_offset + SHOULDER_L_X, TAB_TEXT_Y, SHOULDER_GLYPH_SIZE, 1.0f);
    pause_draw_glyph("r", x_offset + SHOULDER_R_X, TAB_TEXT_Y, SHOULDER_GLYPH_SIZE, 1.0f);
}

static void draw_strip(float x_offset)
{
    float x;
    int slot;
    int index;

    renderer_draw_filled_rect(area(x_offset, FILM_TOP, (float)RENDERER_WIDTH, FILM_HEIGHT), color_of(8, 8, 10));
    for (x = SPROCKET_FIRST; x < (float)RENDERER_WIDTH; x += SPROCKET_STEP) {
        pause_draw_slice("sprocket", x_offset + x, FILM_TOP + SPROCKET_TOP_INSET, SPROCKET_WIDTH, SPROCKET_HEIGHT, 1.0f);
        pause_draw_slice("sprocket", x_offset + x, FILM_TOP + FILM_HEIGHT - SPROCKET_BOTTOM_INSET, SPROCKET_WIDTH, SPROCKET_HEIGHT, 1.0f);
    }
    for (slot = 0; slot < PAUSE_PEOPLE_SLOTS; slot++) {
        index = first_slot[tab] + slot;
        draw_thumb(index < counts[tab] ? art_for(tab, rows[tab][index]) : NULL, x_offset + slot_left(slot), THUMB_TOP, THUMB_WIDTH, THUMB_HEIGHT);
    }
    if (counts[tab] > 0) {
        draw_thumb("frame", x_offset + slot_left(selected[tab] - first_slot[tab]) - FRAME_SPILL, THUMB_TOP - FRAME_SPILL, THUMB_WIDTH + 2.0f * FRAME_SPILL, THUMB_HEIGHT + 2.0f * FRAME_SPILL);
    }
}

static void draw_detail(float x_offset)
{
    struct renderer_color edge;

    draw_whole_texture(SPOT_NAME, x_offset + SPOT_LEFT, SPOT_TOP);
    edge = color_of(140, 20, 20);
    renderer_draw_filled_rect(area(x_offset + ART_LEFT - ART_EDGE, ART_TOP - ART_EDGE, ART_WIDTH + 2.0f * ART_EDGE, ART_EDGE), edge);
    renderer_draw_filled_rect(area(x_offset + ART_LEFT - ART_EDGE, ART_TOP + ART_HEIGHT, ART_WIDTH + 2.0f * ART_EDGE, ART_EDGE), edge);
    renderer_draw_filled_rect(area(x_offset + ART_LEFT - ART_EDGE, ART_TOP, ART_EDGE, ART_HEIGHT), edge);
    renderer_draw_filled_rect(area(x_offset + ART_LEFT + ART_WIDTH, ART_TOP, ART_EDGE, ART_HEIGHT), edge);
    draw_whole_texture(pause_people_art(), x_offset + ART_LEFT, ART_TOP);
    renderer_draw_filled_rect(area(x_offset + CARD_LEFT, CARD_TOP, CARD_WIDTH, BAND_HEIGHT), color_of(0xd3, 0x76, 0x3a));
    pause_draw_slice("note_box", x_offset + CARD_LEFT, BOX_TOP, CARD_WIDTH, BOX_HEIGHT, 1.0f);
    draw_whole_texture(pause_people_card(), x_offset + CARD_LEFT, CARD_TOP);
}

void pause_people_draw(float x_offset)
{
    draw_tabs(x_offset);
    draw_strip(x_offset);
    draw_detail(x_offset);
}

void pause_people_draw_bar(float x_offset)
{
    pause_draw_bar(x_offset, "bar.people_hint", NULL, NULL);
}
