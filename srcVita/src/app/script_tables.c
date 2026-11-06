#include <stdio.h>
#include <string.h>
#include "script_tables.h"
#include "text_lines.h"

#define TABLE_LINE_MAX 320
#define NAME_SCAN ASSET_NAME_SCAN
#define NO_LABEL "-"

typedef int (*table_line_reader)(struct script_tables *tables, const char *line);

static int read_all_lines(struct script_tables *tables, const char *text, unsigned int text_size, table_line_reader read_line)
{
    char line[TABLE_LINE_MAX];
    unsigned int offset;

    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] != '\0' && !read_line(tables, line)) {
            return 0;
        }
    }
    return 1;
}

void script_tables_clear(struct script_tables *tables)
{
    memset(tables, 0, sizeof(*tables));
}

static int read_chain_line(struct script_tables *tables, const char *line)
{
    char word[ASSET_NAME_MAX];

    if (sscanf(line, "stop " NAME_SCAN, tables->stop_act) == 1) {
        return 1;
    }
    if (sscanf(line, "card " NAME_SCAN, tables->card_texture) == 1) {
        return 1;
    }
    if (sscanf(line, "timing " NAME_SCAN, word) == 1) {
        if (strcmp(word, "ds") == 0) {
            tables->timing = SCRIPT_TIMING_DS;
            return 1;
        }
        if (strcmp(word, "remaster") == 0) {
            tables->timing = SCRIPT_TIMING_REMASTER;
            return 1;
        }
        return 0;
    }
    if (text_line_starts_with(line, "file ")) {
        if (tables->file_count >= SCRIPT_CHAIN_FILE_MAX) {
            return 0;
        }
        if (sscanf(line, "file " NAME_SCAN, tables->file_names[tables->file_count]) != 1) {
            return 0;
        }
        tables->file_count = tables->file_count + 1;
        return 1;
    }
    return 0;
}

int script_tables_read_chain(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_chain_line);
}

static int read_room_line(struct script_tables *tables, const char *line)
{
    struct script_room_plate *plate;

    if (tables->room_plate_count >= SCRIPT_ROOM_PLATE_MAX) {
        return 0;
    }
    plate = &tables->room_plates[tables->room_plate_count];
    if (sscanf(line, "plate %d " NAME_SCAN " %f %f %f %f", &plate->room, plate->texture_name, &plate->x, &plate->y, &plate->width, &plate->height) != 6) {
        return 0;
    }
    tables->room_plate_count = tables->room_plate_count + 1;
    return 1;
}

int script_tables_read_rooms(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_room_line);
}

static int read_audio_line(struct script_tables *tables, const char *line)
{
    struct script_audio_row *row;

    if (text_line_starts_with(line, "music ")) {
        if (tables->music_row_count >= SCRIPT_AUDIO_ROW_MAX) {
            return 0;
        }
        row = &tables->music_rows[tables->music_row_count];
        if (sscanf(line, "music %d " NAME_SCAN, &row->id, row->name) != 2) {
            return 0;
        }
        tables->music_row_count = tables->music_row_count + 1;
        return 1;
    }
    if (tables->sound_row_count >= SCRIPT_AUDIO_ROW_MAX) {
        return 0;
    }
    row = &tables->sound_rows[tables->sound_row_count];
    if (sscanf(line, "sound %d " NAME_SCAN, &row->id, row->name) != 2) {
        return 0;
    }
    tables->sound_row_count = tables->sound_row_count + 1;
    return 1;
}

int script_tables_read_audio(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_audio_line);
}

static int read_text_line(struct script_tables *tables, const char *line)
{
    struct script_text_row *row;

    if (tables->text_row_count >= SCRIPT_TEXT_ROW_MAX) {
        return 0;
    }
    row = &tables->text_rows[tables->text_row_count];
    if (sscanf(line, "line %31s %d %d " NAME_SCAN " %f %f", row->label, &row->group, &row->id, row->texture_name, &row->center_x, &row->top_y) != 6) {
        return 0;
    }
    tables->text_row_count = tables->text_row_count + 1;
    return 1;
}

int script_tables_read_text_lines(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_text_line);
}

static int read_balloon_row_line(struct script_tables *tables, const char *line)
{
    struct script_balloon_row *row;

    if (tables->balloon_row_count >= SCRIPT_BALLOON_ROW_MAX) {
        return 0;
    }
    row = &tables->balloon_rows[tables->balloon_row_count];
    memset(row, 0, sizeof(*row));
    if (sscanf(line, "balloon %d %31s %d", &row->kind, row->label, &row->delay_ticks) != 3) {
        return 0;
    }
    if (strcmp(row->label, NO_LABEL) == 0) {
        row->label[0] = '\0';
    }
    tables->balloon_row_count = tables->balloon_row_count + 1;
    return 1;
}

static int read_balloon_part_line(struct script_tables *tables, const char *line)
{
    struct script_balloon_row *row;
    struct script_balloon_part *part;

    if (tables->balloon_row_count == 0) {
        return 0;
    }
    row = &tables->balloon_rows[tables->balloon_row_count - 1];
    if (row->part_count >= SCRIPT_BALLOON_PART_MAX) {
        return 0;
    }
    part = &row->parts[row->part_count];
    if (sscanf(line, "part " NAME_SCAN " %f %f %f %f %d %d", part->texture_name, &part->dx, &part->dy, &part->width, &part->height, &part->after_ticks, &part->grow_ticks) != 7) {
        return 0;
    }
    row->part_count = row->part_count + 1;
    return 1;
}

static int read_balloon_line(struct script_tables *tables, const char *line)
{
    if (text_line_starts_with(line, "balloon ")) {
        return read_balloon_row_line(tables, line);
    }
    return read_balloon_part_line(tables, line);
}

int script_tables_read_balloons(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_balloon_line);
}

static int read_preload_line(struct script_tables *tables, const char *line)
{
    struct script_preload_row *row;
    char word[16];

    if (tables->preload_row_count >= SCRIPT_PRELOAD_ROW_MAX) {
        return 0;
    }
    row = &tables->preload_rows[tables->preload_row_count];
    if (sscanf(line, "%15s " NAME_SCAN, word, row->name) != 2) {
        return 0;
    }
    if (strcmp(word, "animation") == 0) {
        row->kind = SCRIPT_PRELOAD_ANIMATION;
    } else if (strcmp(word, "texture") == 0) {
        row->kind = SCRIPT_PRELOAD_TEXTURE;
    } else if (strcmp(word, "music") == 0) {
        row->kind = SCRIPT_PRELOAD_MUSIC;
    } else if (strcmp(word, "sound") == 0) {
        row->kind = SCRIPT_PRELOAD_SOUND;
    } else {
        return 0;
    }
    tables->preload_row_count = tables->preload_row_count + 1;
    return 1;
}

int script_tables_read_preload(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_preload_line);
}

int script_tables_read_motions(struct script_tables *tables, const char *file_name, const char *text, unsigned int text_size)
{
    char line[TABLE_LINE_MAX];
    unsigned int offset;

    offset = 0;
    while (offset < text_size) {
        struct script_motion_row *row;

        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (tables->motion_row_count >= SCRIPT_MOTION_ROW_MAX) {
            return 0;
        }
        row = &tables->motion_rows[tables->motion_row_count];
        if (sscanf(line, "motion %x " NAME_SCAN " %d", &row->table_offset, row->animation_name, &row->records_per_frame) != 3) {
            return 0;
        }
        snprintf(row->file_name, sizeof(row->file_name), "%s", file_name);
        tables->motion_row_count = tables->motion_row_count + 1;
    }
    return 1;
}

const struct script_motion_row *script_tables_find_motion(const struct script_tables *tables, const char *file_name, unsigned int table_offset)
{
    int index;

    for (index = 0; index < tables->motion_row_count; index++) {
        if (tables->motion_rows[index].table_offset == table_offset && strcmp(tables->motion_rows[index].file_name, file_name) == 0) {
            return &tables->motion_rows[index];
        }
    }
    return NULL;
}

static const char *find_audio_name(const struct script_audio_row *rows, int count, int id)
{
    int index;

    for (index = 0; index < count; index++) {
        if (rows[index].id == id) {
            return rows[index].name;
        }
    }
    return NULL;
}

const char *script_tables_music_name(const struct script_tables *tables, int id)
{
    return find_audio_name(tables->music_rows, tables->music_row_count, id);
}

const char *script_tables_sound_name(const struct script_tables *tables, int kind)
{
    return find_audio_name(tables->sound_rows, tables->sound_row_count, kind);
}

const struct script_balloon_row *script_tables_find_balloon(const struct script_tables *tables, const char *label, int kind)
{
    int index;

    for (index = 0; index < tables->balloon_row_count; index++) {
        const struct script_balloon_row *row;

        row = &tables->balloon_rows[index];
        if (label != NULL && strcmp(row->label, label) == 0) {
            return row;
        }
        if (label == NULL && row->label[0] == '\0' && row->kind == kind) {
            return row;
        }
    }
    return NULL;
}
