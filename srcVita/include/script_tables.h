#ifndef SCRIPT_TABLES_H
#define SCRIPT_TABLES_H

#include "asset_name.h"
#include "script_machine.h"
#include "script_narration.h"
#include "ui_clips.h"

#define SCRIPT_CHAIN_FILE_MAX SCRIPT_SOURCE_MAX
#define SCRIPT_ROOM_PLATE_MAX 1024
#define SCRIPT_EFFECT_ROW_MAX 512
#define SCRIPT_AUDIO_ROW_MAX 1024
#define SCRIPT_TEXT_ROW_MAX 2048
#define SCRIPT_BALLOON_ROW_MAX 1024
#define SCRIPT_FLIPBOOK_MAX 8
#define SCRIPT_MOVIE_MAX 8
#define SCRIPT_CORE_LABEL_MAX 64
#define SCRIPT_FLIPBOOK_PAGE_MAX 3
#define SCRIPT_SOUL_STYLE_MAX 16
#define SCRIPT_BALLOON_PART_MAX 4
#define SCRIPT_MOTION_ROW_MAX 16384
#define SCRIPT_MOTION_BUCKET_COUNT 8192
#define SCRIPT_PRELOAD_ROW_MAX 2048
#define SCRIPT_START_COMMAND_MAX 8

enum script_preload_kind {
    SCRIPT_PRELOAD_ANIMATION,
    SCRIPT_PRELOAD_TEXTURE,
    SCRIPT_PRELOAD_MUSIC,
    SCRIPT_PRELOAD_SOUND
};

enum script_effect_kind {
    SCRIPT_EFFECT_CARD,
    SCRIPT_EFFECT_UI_CLIP,
    SCRIPT_EFFECT_DISSOLVE,
    SCRIPT_EFFECT_RING,
    SCRIPT_EFFECT_MOVIE,
    SCRIPT_EFFECT_GREY_WORLD,
    SCRIPT_EFFECT_PHONE_SPLIT,
    SCRIPT_EFFECT_RAIN
};

#define SCRIPT_FX_MOVIE_MAX 256
#define SCRIPT_FX_FRAME_MAX 16384
#define SCRIPT_FX_AFTER_END 0
#define SCRIPT_FX_AFTER_LOOP 1
#define SCRIPT_FX_AFTER_HOLD 2

struct script_fx_frame {
    int page;
    float u0;
    float v0;
    float u1;
    float v1;
    float corners[8];
};

struct script_fx_movie {
    char name[ASSET_NAME_MAX];
    float ticks_per_frame;
    int after_last;
    int loop_start;
    int sound_kind;
    int page_count;
    int first_frame;
    int frame_count;
};

struct script_effect_row {
    int id;
    enum script_effect_kind kind;
    char texture_name[ASSET_NAME_MAX];
    int ticks;
    float end_zoom;
    float width;
    float height;
    int only_in_soul_view;
};

struct script_flipbook {
    char name[ASSET_NAME_MAX];
    char pages[SCRIPT_FLIPBOOK_PAGE_MAX][ASSET_NAME_MAX];
    int page_count;
    int ticks_per_frame;
    int frame_count;
    int columns;
    int rows;
    float cell_width;
    float cell_height;
};

struct script_soul_style {
    char object_name[ASSET_NAME_MAX];
    unsigned int class_id;
    float red;
    float green;
    float blue;
    float alpha;
    int is_flat;
    int turns_with_ghost;
};

struct script_movie {
    char name[ASSET_NAME_MAX];
    int frame_count;
    int ticks_per_frame;
    unsigned int class_id;
    int sound_kind;
};

struct script_core_label {
    char object_name[ASSET_NAME_MAX];
    char name_texture[ASSET_NAME_MAX];
    char action_texture[ASSET_NAME_MAX];
    float name_width;
    float name_height;
    float action_width;
    float action_height;
};

struct script_room_plate {
    int room;
    char texture_name[ASSET_NAME_MAX];
    char soul_texture_name[ASSET_NAME_MAX];
    char missile_texture_name[ASSET_NAME_MAX];
    char boss_texture_name[ASSET_NAME_MAX];
    float x;
    float y;
    float width;
    float height;
};

struct script_audio_row {
    int id;
    char name[ASSET_NAME_MAX];
};

struct script_text_row {
    char label[SCRIPT_CUE_LABEL_MAX];
    int group;
    int id;
    char texture_name[ASSET_NAME_MAX];
    float center_x;
    float top_y;
};

struct script_balloon_part {
    char texture_name[ASSET_NAME_MAX];
    float dx;
    float dy;
    float width;
    float height;
    int after_ticks;
    int grow_ticks;
};

struct script_balloon_row {
    int kind;
    char label[SCRIPT_CUE_LABEL_MAX];
    int delay_ticks;
    int part_count;
    struct script_balloon_part parts[SCRIPT_BALLOON_PART_MAX];
};

struct script_motion_row {
    char file_name[SCRIPT_SOURCE_NAME_MAX];
    unsigned int table_offset;
    char animation_name[ASSET_NAME_MAX];
    int records_per_frame;
};

struct script_preload_row {
    enum script_preload_kind kind;
    char name[ASSET_NAME_MAX];
};

struct script_tables {
    struct script_route route;
    struct ui_clips ui_clips;
    char next_chapter_card_texture[ASSET_NAME_MAX];
    enum script_timing timing;
    char entry_name[SCRIPT_SOURCE_NAME_MAX];
    int chapter_mark;
    char start_command_verbs[SCRIPT_START_COMMAND_MAX][ASSET_NAME_MAX];
    char start_command_args[SCRIPT_START_COMMAND_MAX][ASSET_NAME_MAX];
    int start_command_count;
    char file_names[SCRIPT_CHAIN_FILE_MAX][SCRIPT_SOURCE_NAME_MAX];
    int file_count;
    struct script_room_plate room_plates[SCRIPT_ROOM_PLATE_MAX];
    int room_plate_count;
    int rows_dropped;
    struct script_room_bounds room_bounds[SCRIPT_ROOM_ID_MAX];
    struct script_effect_row effect_rows[SCRIPT_EFFECT_ROW_MAX];
    int effect_row_count;
    struct script_flipbook flipbooks[SCRIPT_FLIPBOOK_MAX];
    int flipbook_count;
    struct script_fx_movie fx_movies[SCRIPT_FX_MOVIE_MAX];
    int fx_movie_count;
    struct script_fx_frame fx_frames[SCRIPT_FX_FRAME_MAX];
    int fx_frame_count;
    struct script_movie movies[SCRIPT_MOVIE_MAX];
    int movie_count;
    struct script_soul_style soul_styles[SCRIPT_SOUL_STYLE_MAX];
    int soul_style_count;
    struct script_core_label core_labels[SCRIPT_CORE_LABEL_MAX];
    int core_label_count;
    struct script_audio_row music_rows[SCRIPT_AUDIO_ROW_MAX];
    int music_row_count;
    struct script_audio_row sound_rows[SCRIPT_AUDIO_ROW_MAX];
    int sound_row_count;
    struct script_text_row text_rows[SCRIPT_TEXT_ROW_MAX];
    int text_row_count;
    struct script_balloon_row balloon_rows[SCRIPT_BALLOON_ROW_MAX];
    int balloon_row_count;
    struct script_motion_row motion_rows[SCRIPT_MOTION_ROW_MAX];
    int motion_row_count;
    int first_motion_row_in_bucket[SCRIPT_MOTION_BUCKET_COUNT];
    int next_motion_row_in_bucket[SCRIPT_MOTION_ROW_MAX];
    struct script_preload_row preload_rows[SCRIPT_PRELOAD_ROW_MAX];
    int preload_row_count;
};

void script_tables_clear(struct script_tables *tables);
int script_tables_read_chain(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_rooms(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_audio(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_text_lines(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_effects(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_balloons(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_preload(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_motions(struct script_tables *tables, const char *file_name, const char *text, unsigned int text_size);
int script_tables_has_motions(const struct script_tables *tables, const char *file_name);
const struct script_motion_row *script_tables_find_motion(const struct script_tables *tables, const char *file_name, unsigned int table_offset);
const struct script_fx_movie *script_tables_find_fx_movie(const struct script_tables *tables, const char *name);
const struct script_movie *script_tables_find_movie_of_class(const struct script_tables *tables, unsigned int class_id);
const struct script_movie *script_tables_find_movie(const struct script_tables *tables, const char *name);
const struct script_flipbook *script_tables_find_flipbook(const struct script_tables *tables, const char *name);
int script_tables_read_soul(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_route(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_scenes(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_route_pacing(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_ui_clips(struct script_tables *tables, const char *text, unsigned int text_size);
int script_tables_read_core_labels(struct script_tables *tables, const char *text, unsigned int text_size);
const struct script_core_label *script_tables_find_core_label(const struct script_tables *tables, const char *object_name);
const struct script_soul_style *script_tables_soul_style(const struct script_tables *tables, unsigned int class_id, int is_human, int has_core, int is_active, int is_dead);
const struct script_effect_row *script_tables_find_effect(const struct script_tables *tables, int id);
const char *script_tables_music_name(const struct script_tables *tables, int id);
const char *script_tables_sound_name(const struct script_tables *tables, int kind);
const struct script_balloon_row *script_tables_find_balloon(const struct script_tables *tables, const char *label, int kind);

#endif
