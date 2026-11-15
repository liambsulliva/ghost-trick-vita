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

struct place_row {
    int is_known;
    int resources[PHONE_BOOK_PLATES_LEVELS];
    int reveals[REVEALS];
};

static char sheet[ASSET_NAME_MAX];
static struct phone_book_plate plates[PLATES_MOST];
static int plate_count;
static struct place_row places[PHONE_BOOK_PLATES_PLACE_MOST];
static int place_count;
static unsigned int missing_logged;

static void forget_table(void)
{
    sheet[0] = '\0';
    plate_count = 0;
    place_count = 0;
    memset(places, 0, sizeof(places));
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

const struct phone_book_plate *phone_book_plates_find(int place, int stored_note_level, int is_selected)
{
    int level;
    int resource;
    int index;

    level = phone_book_plates_level(place, stored_note_level);
    if (level < 0) {
        return NULL;
    }
    resource = places[place].resources[level] + (is_selected ? SELECTED_STATE : 0);
    for (index = 0; index < plate_count; index++) {
        if (plates[index].resource == resource) {
            return &plates[index];
        }
    }
    return NULL;
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
    pause_draw_from(sheet, plate->x, plate->y, plate->width, plate->height, x + plate->left * scale, y + plate->top * scale, plate->width * scale, plate->height * scale, 1.0f);
}
