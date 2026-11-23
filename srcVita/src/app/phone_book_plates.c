#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset_language.h"
#include "asset_name.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "pause_widgets.h"
#include "phone_book_plates.h"
#include "run_log.h"
#include "text_lines.h"

#define TABLE_PATH "pause/phone_book.txt"
#define LINE_MAX 256
#define PLATES_MOST 128
#define REVEALS (PHONE_BOOK_PLATES_LEVELS - 1)
#define NO_REVEAL 0xff
#define SELECTED_STATE 1
#define PROMPTS_MOST 4
#define PROMPT_NAME_MAX 16
#define MISSING_CHOICE_LOGGED (1u << 30)
#define MISSING_PROMPT_LOGGED (1u << 31)

struct place_row {
    int is_known;
    int resources[PHONE_BOOK_PLATES_LEVELS];
    int reveals[REVEALS];
};

struct prompt_row {
    char name[PROMPT_NAME_MAX];
    int resource;
};

static char sheet[ASSET_NAME_MAX];
static struct phone_book_plate plates[PLATES_MOST];
static int plate_count;
static struct place_row places[PHONE_BOOK_PLATES_PLACE_MOST];
static int place_count;
static int choices[PHONE_BOOK_PLATES_CHOICES];
static struct prompt_row prompts[PROMPTS_MOST];
static int prompt_count;
static unsigned int missing_logged;

static void forget_table(void)
{
    sheet[0] = '\0';
    plate_count = 0;
    place_count = 0;
    memset(places, 0, sizeof(places));
    memset(choices, 0, sizeof(choices));
    memset(prompts, 0, sizeof(prompts));
    prompt_count = 0;
    missing_logged = 0;
}

static void read_plate(const char *text)
{
    struct phone_book_plate plate;
    int x;
    int y;
    int width;
    int height;
    int left;
    int top;

    if (plate_count >= PLATES_MOST || sscanf(text, "%d %d %d %d %d %d %d", &plate.resource, &x, &y, &width, &height, &left, &top) != 7 || width <= 0 || height <= 0) {
        return;
    }
    plate.x = (float)x;
    plate.y = (float)y;
    plate.width = (float)width;
    plate.height = (float)height;
    plate.left = (float)left;
    plate.top = (float)top;
    plates[plate_count] = plate;
    plate_count = plate_count + 1;
}

static void read_place(const char *text)
{
    struct place_row row;
    int place;

    if (sscanf(text, "%d %d %d %d %d %d", &place, &row.resources[0], &row.resources[1], &row.resources[2], &row.reveals[0], &row.reveals[1]) != 6 || place < 0 || place >= PHONE_BOOK_PLATES_PLACE_MOST) {
        return;
    }
    row.is_known = 1;
    places[place] = row;
    place_count = place_count + 1;
}

static void read_choice(const char *text)
{
    int kind;
    int resource;

    if (sscanf(text, "%d %d", &kind, &resource) != 2 || kind < 0 || kind >= PHONE_BOOK_PLATES_CHOICES || resource <= 0) {
        return;
    }
    choices[kind] = resource;
}

static void read_prompt(const char *text)
{
    struct prompt_row row;
    char name[PROMPT_NAME_MAX];

    if (prompt_count >= PROMPTS_MOST || sscanf(text, "%15s %d", name, &row.resource) != 2 || row.resource <= 0) {
        return;
    }
    snprintf(row.name, sizeof(row.name), "%s", name);
    prompts[prompt_count] = row;
    prompt_count = prompt_count + 1;
}

int phone_book_plates_read_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    unsigned int offset;

    forget_table();
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (text_line_starts_with(line, "sheet ")) {
            snprintf(sheet, sizeof(sheet), "%s", line + 6);
        } else if (text_line_starts_with(line, "plate ")) {
            read_plate(line + 6);
        } else if (text_line_starts_with(line, "place ")) {
            read_place(line + 6);
        } else if (text_line_starts_with(line, "choice ")) {
            read_choice(line + 7);
        } else if (text_line_starts_with(line, "prompt ")) {
            read_prompt(line + 7);
        }
    }
    if (sheet[0] == '\0' || plate_count == 0) {
        forget_table();
    }
    return place_count;
}

int phone_book_plates_load(const char *assets_folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    int count;

    forget_table();
    if (!asset_language_join(path, assets_folder, TABLE_PATH) || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    count = phone_book_plates_read_text((const char *)bytes, size);
    mem_free(bytes);
    return count;
}

int phone_book_plates_is_loaded(void)
{
    return place_count > 0;
}

int phone_book_plates_level(int place, int stored_note_level)
{
    int level;
    int reveal;

    if (place < 0 || place >= PHONE_BOOK_PLATES_PLACE_MOST || !places[place].is_known) {
        return -1;
    }
    level = 0;
    for (reveal = 0; reveal < REVEALS; reveal++) {
        if (places[place].reveals[reveal] != NO_REVEAL && stored_note_level > 0 && stored_note_level - 1 >= places[place].reveals[reveal]) {
            level = reveal + 1;
        }
    }
    return level;
}

static const struct phone_book_plate *plate_of(int resource)
{
    int index;

    for (index = 0; index < plate_count; index++) {
        if (plates[index].resource == resource) {
            return &plates[index];
        }
    }
    return NULL;
}

const struct phone_book_plate *phone_book_plates_find(int place, int stored_note_level, int is_selected)
{
    int level;
    int resource;

    level = phone_book_plates_level(place, stored_note_level);
    if (level < 0) {
        return NULL;
    }
    resource = places[place].resources[level] + (is_selected ? SELECTED_STATE : 0);
    return plate_of(resource);
}

static void draw_plate(const struct phone_book_plate *plate, float x, float y, float scale)
{
    pause_draw_from(sheet, plate->x, plate->y, plate->width, plate->height, x + plate->left * scale, y + plate->top * scale, plate->width * scale, plate->height * scale, 1.0f);
}

void phone_book_plates_draw(int place, int stored_note_level, int is_selected, float x, float y, float scale)
{
    const struct phone_book_plate *plate;

    plate = phone_book_plates_find(place, stored_note_level, is_selected);
    if (plate == NULL) {
        if (place >= 0 && place < PHONE_BOOK_PLATES_PLACE_MOST && (missing_logged & (1u << place)) == 0) {
            missing_logged = missing_logged | (1u << place);
            run_log_write("phone_book plate missing place=%d note_level=%d", place, stored_note_level);
        }
        return;
    }
    draw_plate(plate, x, y, scale);
}

const struct phone_book_plate *phone_book_plates_find_choice(int kind, int is_selected)
{
    if (!phone_book_plates_is_loaded() || kind < 0 || kind >= PHONE_BOOK_PLATES_CHOICES || choices[kind] == 0) {
        return NULL;
    }
    return plate_of(choices[kind] + (is_selected ? SELECTED_STATE : 0));
}

const struct phone_book_plate *phone_book_plates_find_prompt(const char *name)
{
    int index;

    if (!phone_book_plates_is_loaded()) {
        return NULL;
    }
    for (index = 0; index < prompt_count; index++) {
        if (strcmp(prompts[index].name, name) == 0) {
            return plate_of(prompts[index].resource);
        }
    }
    return NULL;
}

void phone_book_plates_draw_choice(int kind, int is_selected, float x, float y, float scale)
{
    const struct phone_book_plate *plate;

    plate = phone_book_plates_find_choice(kind, is_selected);
    if (plate == NULL) {
        if ((missing_logged & MISSING_CHOICE_LOGGED) == 0) {
            missing_logged = missing_logged | MISSING_CHOICE_LOGGED;
            run_log_write("phone_book plate missing choice=%d", kind);
        }
        return;
    }
    draw_plate(plate, x, y, scale);
}

void phone_book_plates_draw_prompt(const char *name, float x, float y, float scale)
{
    const struct phone_book_plate *plate;

    plate = phone_book_plates_find_prompt(name);
    if (plate == NULL) {
        if ((missing_logged & MISSING_PROMPT_LOGGED) == 0) {
            missing_logged = missing_logged | MISSING_PROMPT_LOGGED;
            run_log_write("phone_book plate missing prompt=%s", name);
        }
        return;
    }
    draw_plate(plate, x, y, scale);
}
