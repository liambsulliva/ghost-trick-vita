#include <stdio.h>
#include <string.h>
#include "crc32.h"
#include "script_tables.h"

#define SOUL_CORE_CLASS_ID 0x4080f0cdu
#define SOUL_ACTIVE_STYLE_NAME "active"
#define SOUL_DEAD_STYLE_NAME "dead"
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

    if (sscanf(line, "next_chapter_card " NAME_SCAN, tables->next_chapter_card_texture) == 1) {
        return 1;
    }
    if (sscanf(line, "entry " NAME_SCAN, tables->entry_name) == 1) {
        return 1;
    }
    if (sscanf(line, "chapter_mark %d", &tables->chapter_mark) == 1) {
        return tables->chapter_mark >= 0;
    }
    if (text_line_starts_with(line, "start_command ")) {
        if (tables->start_command_count >= SCRIPT_START_COMMAND_MAX) {
            return 0;
        }
        if (sscanf(line, "start_command " NAME_SCAN " " NAME_SCAN, tables->start_command_verbs[tables->start_command_count], tables->start_command_args[tables->start_command_count]) != 2) {
            return 0;
        }
        tables->start_command_count = tables->start_command_count + 1;
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

static int read_room_size_line(struct script_tables *tables, const char *line)
{
    int room;
    int width;
    int height;

    if (sscanf(line, "room %d %d %d", &room, &width, &height) != 3 || room < 0 || room >= SCRIPT_ROOM_ID_MAX) {
        return 0;
    }
    tables->room_bounds[room].width = width;
    tables->room_bounds[room].height = height;
    return 1;
}

static int read_room_band_line(struct script_tables *tables, const char *line)
{
    struct script_room_bounds *bounds;
    int room;
    int key;
    int value;

    if (sscanf(line, "band %d %d %d", &room, &key, &value) != 3 || room < 0 || room >= SCRIPT_ROOM_ID_MAX) {
        return 0;
    }
    bounds = &tables->room_bounds[room];
    if (bounds->band_count >= SCRIPT_CAMERA_BAND_MAX) {
        return 0;
    }
    bounds->band_keys[bounds->band_count] = key;
    bounds->band_values[bounds->band_count] = value;
    bounds->band_count = bounds->band_count + 1;
    return 1;
}

static int read_room_line(struct script_tables *tables, const char *line)
{
    struct script_room_plate *plate;

    if (text_line_starts_with(line, "band ")) {
        return read_room_band_line(tables, line);
    }
    if (text_line_starts_with(line, "room ")) {
        return read_room_size_line(tables, line);
    }
    if (tables->room_plate_count >= SCRIPT_ROOM_PLATE_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    plate = &tables->room_plates[tables->room_plate_count];
    snprintf(plate->missile_texture_name, sizeof(plate->missile_texture_name), "-");
    snprintf(plate->boss_texture_name, sizeof(plate->boss_texture_name), "-");
    if (sscanf(line, "plate %d " NAME_SCAN " %f %f %f %f " NAME_SCAN " " NAME_SCAN " " NAME_SCAN, &plate->room, plate->texture_name, &plate->x, &plate->y, &plate->width, &plate->height, plate->soul_texture_name, plate->missile_texture_name, plate->boss_texture_name) < 6) {
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
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
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

static int read_movie_line(struct script_tables *tables, const char *line)
{
    struct script_movie *movie;

    if (tables->movie_count >= SCRIPT_MOVIE_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    movie = &tables->movies[tables->movie_count];
    movie->sound_kind = -1;
    if (sscanf(line, "movie " NAME_SCAN " %d %d %x %d", movie->name, &movie->frame_count, &movie->ticks_per_frame, &movie->class_id, &movie->sound_kind) < 4) {
        return 0;
    }
    tables->movie_count = tables->movie_count + 1;
    return 1;
}

static int read_fx_movie_line(struct script_tables *tables, const char *line)
{
    struct script_fx_movie *movie;
    int status;

    if (tables->fx_movie_count >= SCRIPT_FX_MOVIE_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    movie = &tables->fx_movies[tables->fx_movie_count];
    if (sscanf(line, "fxmovie " NAME_SCAN " %f %d %d %d %d", movie->name, &movie->ticks_per_frame, &movie->after_last, &movie->loop_start, &movie->sound_kind, &movie->page_count) != 6) {
        return 0;
    }
    movie->first_frame = tables->fx_frame_count;
    movie->frame_count = 0;
    movie->first_jump = tables->fx_jump_count;
    movie->jump_count = 0;
    for (status = 0; status < SCRIPT_FX_STATUS_MAX; status++) {
        movie->status_frames[status] = SCRIPT_FX_NO_FRAME;
    }
    tables->fx_movie_count = tables->fx_movie_count + 1;
    return 1;
}

static int read_staff_page_line(struct script_tables *tables, const char *line)
{
    int page;
    int screen_count;

    if (sscanf(line, "staffpage %d %d", &page, &screen_count) != 2 || screen_count <= 0) {
        return 0;
    }
    if (page < 0 || page >= SCRIPT_STAFF_PAGE_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    tables->staff_page_screens[page] = screen_count;
    return 1;
}

static int read_fx_jump_line(struct script_tables *tables, const char *line)
{
    struct script_fx_jump *jump;

    if (tables->fx_movie_count == 0) {
        return 0;
    }
    if (tables->fx_jump_count >= SCRIPT_FX_JUMP_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    jump = &tables->fx_jumps[tables->fx_jump_count];
    if (sscanf(line, "fxjump %d %d %d %d", &jump->frame, &jump->target, &jump->condition, &jump->value) != 4) {
        return 0;
    }
    tables->fx_jump_count = tables->fx_jump_count + 1;
    tables->fx_movies[tables->fx_movie_count - 1].jump_count = tables->fx_movies[tables->fx_movie_count - 1].jump_count + 1;
    return 1;
}

static int read_fx_status_line(struct script_tables *tables, const char *line)
{
    int status;
    int frame;

    if (tables->fx_movie_count == 0 || sscanf(line, "fxstatus %d %d", &status, &frame) != 2 || frame < 0) {
        return 0;
    }
    if (status < 0 || status >= SCRIPT_FX_STATUS_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    tables->fx_movies[tables->fx_movie_count - 1].status_frames[status] = frame;
    return 1;
}

static int read_fx_frame_line(struct script_tables *tables, const char *line)
{
    struct script_fx_frame *frame;
    float *c;

    if (tables->fx_movie_count == 0) {
        return 0;
    }
    if (tables->fx_frame_count >= SCRIPT_FX_FRAME_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    frame = &tables->fx_frames[tables->fx_frame_count];
    c = frame->corners;
    if (sscanf(line, "fxframe %d %f %f %f %f %f %f %f %f %f %f %f %f", &frame->page, &frame->u0, &frame->v0, &frame->u1, &frame->v1, &c[0], &c[1], &c[2], &c[3], &c[4], &c[5], &c[6], &c[7]) != 13) {
        return 0;
    }
    tables->fx_frame_count = tables->fx_frame_count + 1;
    tables->fx_movies[tables->fx_movie_count - 1].frame_count = tables->fx_movies[tables->fx_movie_count - 1].frame_count + 1;
    return 1;
}

static int read_flipbook_line(struct script_tables *tables, const char *line)
{
    struct script_flipbook *book;
    int found;

    if (tables->flipbook_count >= SCRIPT_FLIPBOOK_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    book = &tables->flipbooks[tables->flipbook_count];
    memset(book, 0, sizeof(*book));
    found = sscanf(line, "flipbook " NAME_SCAN " %d %d %d %d %f %f " NAME_SCAN " " NAME_SCAN " " NAME_SCAN, book->name, &book->ticks_per_frame, &book->frame_count, &book->columns, &book->rows, &book->cell_width, &book->cell_height, book->pages[0], book->pages[1], book->pages[2]);
    if (found < 8) {
        return 0;
    }
    book->page_count = found - 7;
    tables->flipbook_count = tables->flipbook_count + 1;
    return 1;
}

static int read_effect_line(struct script_tables *tables, const char *line)
{
    struct script_effect_row *row;
    char kind[16];

    if (text_line_starts_with(line, "flipbook ")) {
        return read_flipbook_line(tables, line);
    }
    if (text_line_starts_with(line, "movie ")) {
        return read_movie_line(tables, line);
    }
    if (text_line_starts_with(line, "fxmovie ")) {
        return read_fx_movie_line(tables, line);
    }
    if (text_line_starts_with(line, "fxframe ")) {
        return read_fx_frame_line(tables, line);
    }
    if (text_line_starts_with(line, "staffpage ")) {
        return read_staff_page_line(tables, line);
    }
    if (text_line_starts_with(line, "fxjump ")) {
        return read_fx_jump_line(tables, line);
    }
    if (text_line_starts_with(line, "fxstatus ")) {
        return read_fx_status_line(tables, line);
    }

    if (tables->effect_row_count >= SCRIPT_EFFECT_ROW_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    row = &tables->effect_rows[tables->effect_row_count];
    memset(row, 0, sizeof(*row));
    if (sscanf(line, "effect %d %15s " NAME_SCAN " %d %f %f %f %d", &row->id, kind, row->texture_name, &row->ticks, &row->end_zoom, &row->width, &row->height, &row->only_in_soul_view) < 7) {
        return 0;
    }
    if (strcmp(kind, "card") == 0) {
        row->kind = SCRIPT_EFFECT_CARD;
    } else if (strcmp(kind, "ui_clip") == 0) {
        row->kind = SCRIPT_EFFECT_UI_CLIP;
    } else if (strcmp(kind, "dissolve") == 0) {
        row->kind = SCRIPT_EFFECT_DISSOLVE;
    } else if (strcmp(kind, "grey_world") == 0) {
        row->kind = SCRIPT_EFFECT_GREY_WORLD;
    } else if (strcmp(kind, "phone_split") == 0) {
        row->kind = SCRIPT_EFFECT_PHONE_SPLIT;
    } else if (strcmp(kind, "rain") == 0) {
        row->kind = SCRIPT_EFFECT_RAIN;
    } else if (strcmp(kind, "movie") == 0) {
        row->kind = SCRIPT_EFFECT_MOVIE;
    } else if (strcmp(kind, "ring") == 0) {
        row->kind = SCRIPT_EFFECT_RING;
    } else {
        return 0;
    }
    tables->effect_row_count = tables->effect_row_count + 1;
    return 1;
}

int script_tables_read_effects(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_effect_line);
}

const struct script_fx_movie *script_tables_find_fx_movie(const struct script_tables *tables, const char *name)
{
    int index;

    for (index = 0; index < tables->fx_movie_count; index++) {
        if (strcmp(tables->fx_movies[index].name, name) == 0) {
            return &tables->fx_movies[index];
        }
    }
    return NULL;
}

const struct script_movie *script_tables_find_movie_of_class(const struct script_tables *tables, unsigned int class_id)
{
    int index;

    for (index = 0; index < tables->movie_count; index++) {
        if (tables->movies[index].class_id == class_id) {
            return &tables->movies[index];
        }
    }
    return NULL;
}

const struct script_movie *script_tables_find_movie(const struct script_tables *tables, const char *name)
{
    int index;

    for (index = 0; index < tables->movie_count; index++) {
        if (strcmp(tables->movies[index].name, name) == 0) {
            return &tables->movies[index];
        }
    }
    return NULL;
}

const struct script_flipbook *script_tables_find_flipbook(const struct script_tables *tables, const char *name)
{
    int index;

    for (index = 0; index < tables->flipbook_count; index++) {
        if (strcmp(tables->flipbooks[index].name, name) == 0) {
            return &tables->flipbooks[index];
        }
    }
    return NULL;
}

static int read_trickless_line(struct script_tables *tables, const char *line)
{
    int core_info;

    if (sscanf(line, "trickless %d", &core_info) != 1) {
        return 0;
    }
    if (tables->trickless_core_info_count >= SCRIPT_TRICKLESS_CORE_INFO_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    tables->trickless_core_infos[tables->trickless_core_info_count] = core_info;
    tables->trickless_core_info_count = tables->trickless_core_info_count + 1;
    return 1;
}

static int read_soul_line(struct script_tables *tables, const char *line)
{
    struct script_soul_style *style;

    if (read_trickless_line(tables, line)) {
        return 1;
    }
    if (tables->soul_style_count >= SCRIPT_SOUL_STYLE_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    style = &tables->soul_styles[tables->soul_style_count];
    memset(style, 0, sizeof(*style));
    style->turns_with_ghost = 0;
    if (sscanf(line, "class %x %f %f %f %f %d %d", &style->class_id, &style->red, &style->green, &style->blue, &style->alpha, &style->is_flat, &style->turns_with_ghost) >= 6 || sscanf(line, "object " NAME_SCAN " %f %f %f %f %d %d", style->object_name, &style->red, &style->green, &style->blue, &style->alpha, &style->is_flat, &style->turns_with_ghost) >= 6) {
        tables->soul_style_count = tables->soul_style_count + 1;
    }
    return 1;
}

static int read_core_label_line(struct script_tables *tables, const char *line)
{
    struct script_core_label *label;

    if (tables->core_label_count >= SCRIPT_CORE_LABEL_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
    }
    label = &tables->core_labels[tables->core_label_count];
    memset(label, 0, sizeof(*label));
    if (sscanf(line, "label " NAME_SCAN " " NAME_SCAN " %f %f " NAME_SCAN " %f %f", label->object_name, label->name_texture, &label->name_width, &label->name_height, label->action_texture, &label->action_width, &label->action_height) != 7) {
        return 0;
    }
    tables->core_label_count = tables->core_label_count + 1;
    return 1;
}

int script_tables_read_core_labels(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_core_label_line);
}

const struct script_core_label *script_tables_find_core_label(const struct script_tables *tables, const char *object_name)
{
    int index;

    for (index = 0; index < tables->core_label_count; index++) {
        if (strcmp(tables->core_labels[index].object_name, object_name) == 0) {
            return &tables->core_labels[index];
        }
    }
    return NULL;
}

int script_tables_read_soul(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return read_all_lines(tables, text, text_size, read_soul_line);
}

int script_tables_read_ui_clips(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return ui_clips_read(&tables->ui_clips, text, text_size);
}

int script_tables_read_route(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return script_route_read(&tables->route, text, text_size);
}

int script_tables_read_route_pacing(struct script_tables *tables, const char *text, unsigned int text_size)
{
    return script_route_read_pacing(&tables->route, text, text_size);
}

const struct script_soul_style *script_tables_soul_style(const struct script_tables *tables, unsigned int class_id, int is_human, int has_core, int is_active, int is_dead)
{
    unsigned int wanted_class;
    int index;

    wanted_class = has_core && !is_human ? SOUL_CORE_CLASS_ID : class_id;
    for (index = 0; index < tables->soul_style_count; index++) {
        const struct script_soul_style *style;

        style = &tables->soul_styles[index];
        if (is_active && strcmp(style->object_name, SOUL_ACTIVE_STYLE_NAME) == 0) {
            return style;
        }
        if (!is_active && is_dead && strcmp(style->object_name, SOUL_DEAD_STYLE_NAME) == 0) {
            return style;
        }
        if (!is_active && !is_dead && style->object_name[0] == '\0' && style->class_id == wanted_class) {
            return style;
        }
    }
    return NULL;
}

const struct script_effect_row *script_tables_find_effect(const struct script_tables *tables, int id)
{
    int index;

    for (index = 0; index < tables->effect_row_count; index++) {
        if (tables->effect_rows[index].id == id) {
            return &tables->effect_rows[index];
        }
    }
    return NULL;
}

static int read_balloon_row_line(struct script_tables *tables, const char *line)
{
    struct script_balloon_row *row;

    if (tables->balloon_row_count >= SCRIPT_BALLOON_ROW_MAX) {
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
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
        tables->rows_dropped = tables->rows_dropped + 1;
        return 1;
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

static unsigned int motion_bucket(const char *file_name, unsigned int table_offset)
{
    return crc32_add(table_offset, (const unsigned char *)file_name, (unsigned int)strlen(file_name)) % SCRIPT_MOTION_BUCKET_COUNT;
}

static void index_motion_row(struct script_tables *tables, int row_index)
{
    const struct script_motion_row *row;
    unsigned int bucket;

    row = &tables->motion_rows[row_index];
    if (script_tables_find_motion(tables, row->file_name, row->table_offset) != NULL) {
        return;
    }
    bucket = motion_bucket(row->file_name, row->table_offset);
    tables->next_motion_row_in_bucket[row_index] = tables->first_motion_row_in_bucket[bucket];
    tables->first_motion_row_in_bucket[bucket] = row_index + 1;
}

int script_tables_has_motions(const struct script_tables *tables, const char *file_name)
{
    int index;

    for (index = 0; index < tables->motion_row_count; index++) {
        if (strcmp(tables->motion_rows[index].file_name, file_name) == 0) {
            return 1;
        }
    }
    return 0;
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
            tables->rows_dropped = tables->rows_dropped + 1;
            return 1;
        }
        row = &tables->motion_rows[tables->motion_row_count];
        if (sscanf(line, "motion %x " NAME_SCAN " %d", &row->table_offset, row->animation_name, &row->records_per_frame) != 3) {
            return 0;
        }
        snprintf(row->file_name, sizeof(row->file_name), "%s", file_name);
        index_motion_row(tables, tables->motion_row_count);
        tables->motion_row_count = tables->motion_row_count + 1;
    }
    return 1;
}

const struct script_motion_row *script_tables_find_motion(const struct script_tables *tables, const char *file_name, unsigned int table_offset)
{
    int linked;

    for (linked = tables->first_motion_row_in_bucket[motion_bucket(file_name, table_offset)]; linked != 0; linked = tables->next_motion_row_in_bucket[linked - 1]) {
        if (tables->motion_rows[linked - 1].table_offset == table_offset && strcmp(tables->motion_rows[linked - 1].file_name, file_name) == 0) {
            return &tables->motion_rows[linked - 1];
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
