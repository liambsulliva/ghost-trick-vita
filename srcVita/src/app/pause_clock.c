#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "pause_clock.h"
#include "pause_widgets.h"
#include "text_lines.h"

#define TABLE_PATH "pause/clock.txt"
#define SHEET_NAME "pause_clock"
#define NAME_MAX 64
#define LINE_MAX 160
#define GLYPH_MAX 16
#define PLACE_MAX 32
#define SCENE_MAX 256
#define MINUTES_PER_HOUR 60
#define HOURS_PER_HALF_DAY 12
#define HOURS_PER_DAY 24
#define FIRST_CHAPTER 1
#define CLOCK_LEFT 24.0f
#define CLOCK_TOP 6.0f
#define PLACE_GAP -10.0f
#define AM_PM_GAP 8.0f
#define GLYPH_OVERLAP 6.0f

struct clock_cell {
    char name[NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

struct clock_scene {
    int chapter;
    char name[NAME_MAX];
    int minutes;
};

static struct clock_cell glyphs[GLYPH_MAX];
static int glyph_count;
static struct clock_cell places[PLACE_MAX];
static int place_count;
static struct clock_scene scenes[SCENE_MAX];
static int scene_count;
static int shown_minutes = PAUSE_CLOCK_NO_TIME;
static int shown_place = -1;

static int read_cell(const char *rest, struct clock_cell *cell)
{
    return sscanf(rest, "%63s %f %f %f %f", cell->name, &cell->x, &cell->y, &cell->width, &cell->height) == 5;
}

static void read_line(const char *line)
{
    if (text_line_starts_with(line, "glyph ") && glyph_count < GLYPH_MAX && read_cell(line + 6, &glyphs[glyph_count])) {
        glyph_count = glyph_count + 1;
    } else if (text_line_starts_with(line, "place ") && place_count < PLACE_MAX && read_cell(line + 6, &places[place_count])) {
        place_count = place_count + 1;
    } else if (text_line_starts_with(line, "scene ") && scene_count < SCENE_MAX && sscanf(line + 6, "%d %63s %d", &scenes[scene_count].chapter, scenes[scene_count].name, &scenes[scene_count].minutes) == 3) {
        scene_count = scene_count + 1;
    }
}

int pause_clock_read_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    unsigned int at;

    glyph_count = 0;
    place_count = 0;
    scene_count = 0;
    shown_minutes = PAUSE_CLOCK_NO_TIME;
    shown_place = -1;
    at = 0;
    while (at < text_size) {
        at = text_lines_read_next(text, text_size, at, line, sizeof(line));
        read_line(line);
    }
    return glyph_count > 0 ? scene_count : 0;
}

int pause_clock_load(const char *assets_folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    int count;

    if (!file_path_join(path, assets_folder, TABLE_PATH, "") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        pause_clock_read_text("", 0);
        return 0;
    }
    count = pause_clock_read_text((const char *)bytes, size);
    mem_free(bytes);
    return count;
}

static int chapter_has_times(int chapter)
{
    int index;

    for (index = 0; index < scene_count; index++) {
        if (scenes[index].chapter == chapter) {
            return 1;
        }
    }
    return 0;
}

void pause_clock_note_scene(int chapter_mark, const char *script)
{
    int chapter;
    int index;

    chapter = chapter_mark < FIRST_CHAPTER ? FIRST_CHAPTER : chapter_mark;
    for (index = 0; index < scene_count; index++) {
        if (scenes[index].chapter == chapter && strcmp(scenes[index].name, script) == 0) {
            shown_minutes = scenes[index].minutes;
            return;
        }
    }
    if (!chapter_has_times(chapter)) {
        shown_minutes = PAUSE_CLOCK_NO_TIME;
    }
}

static int place_index(const char *key)
{
    int index;

    for (index = 0; index < place_count; index++) {
        if (strcmp(places[index].name, key) == 0) {
            return index;
        }
    }
    return -1;
}

void pause_clock_note_plate(const char *plate_name)
{
    char key[NAME_MAX];
    int stage;
    int part;
    int found;

    if (sscanf(plate_name, "st%2d-%2d", &stage, &part) == 2) {
        snprintf(key, sizeof(key), "stage%02d_%02d", stage, part);
        found = place_index(key);
        if (found >= 0) {
            shown_place = found;
            return;
        }
    }
    if (sscanf(plate_name, "st%2d", &stage) == 1) {
        snprintf(key, sizeof(key), "stage%02d_00", stage);
        found = place_index(key);
        if (found >= 0) {
            shown_place = found;
        }
    }
}

int pause_clock_minutes(void)
{
    return shown_minutes;
}

const char *pause_clock_place(void)
{
    return shown_place >= 0 ? places[shown_place].name : NULL;
}

void pause_clock_time_text(int minutes, char *text, unsigned int text_size)
{
    int hour;

    if (minutes < 0) {
        snprintf(text, text_size, "%s", "");
        return;
    }
    hour = (minutes / MINUTES_PER_HOUR) % HOURS_PER_DAY;
    snprintf(text, text_size, "%d:%02d %s", hour % HOURS_PER_HALF_DAY == 0 ? HOURS_PER_HALF_DAY : hour % HOURS_PER_HALF_DAY, minutes % MINUTES_PER_HOUR, hour < HOURS_PER_HALF_DAY ? "AM" : "PM");
}

static const struct clock_cell *glyph_named(const char *name)
{
    int index;

    for (index = 0; index < glyph_count; index++) {
        if (strcmp(glyphs[index].name, name) == 0) {
            return &glyphs[index];
        }
    }
    return NULL;
}

static const struct clock_cell *glyph_for(const char *text)
{
    char name[8];

    if (text[0] >= '0' && text[0] <= '9') {
        snprintf(name, sizeof(name), "d%c", text[0]);
        return glyph_named(name);
    }
    if (text[0] == ':') {
        return glyph_named("colon");
    }
    if (strncmp(text, "AM", 2) == 0) {
        return glyph_named("am");
    }
    if (strncmp(text, "PM", 2) == 0) {
        return glyph_named("pm");
    }
    return NULL;
}

static float draw_cell(const struct clock_cell *cell, float x, float y, float alpha)
{
    pause_draw_from(SHEET_NAME, cell->x, cell->y, cell->width, cell->height, x, y, cell->width, cell->height, alpha);
    return x + cell->width - GLYPH_OVERLAP;
}

void pause_clock_draw(float alpha)
{
    char text[PAUSE_CLOCK_TEXT_MAX];
    const struct clock_cell *cell;
    float x;
    float bottom;
    unsigned int at;

    bottom = CLOCK_TOP;
    pause_clock_time_text(shown_minutes, text, sizeof(text));
    x = CLOCK_LEFT;
    for (at = 0; text[at] != '\0'; at++) {
        if (text[at] == ' ') {
            x = x + AM_PM_GAP;
            continue;
        }
        cell = glyph_for(&text[at]);
        if (cell == NULL) {
            continue;
        }
        x = draw_cell(cell, x, CLOCK_TOP, alpha);
        bottom = CLOCK_TOP + cell->height;
        if (text[at] == 'A' || text[at] == 'P') {
            at = at + 1;
        }
    }
    if (shown_place >= 0) {
        draw_cell(&places[shown_place], CLOCK_LEFT, bottom == CLOCK_TOP ? CLOCK_TOP : bottom + PLACE_GAP, alpha);
    }
}
