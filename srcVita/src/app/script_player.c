#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animation.h"
#include "animation_library.h"
#include "background_reader.h"
#include "clock.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "music_engine.h"
#include "run_log.h"
#include "next_chapter_card.h"
#include "scene_stage.h"
#include "script_bridge.h"
#include "script_machine.h"
#include "scene_view.h"
#include "read_ahead.h"
#include "script_player.h"
#include "script_ui_view.h"
#include "script_tables.h"
#include "sound_engine.h"
#include "texture_cache.h"
#include "ds_units.h"

#define MICROSECONDS_PER_VM_TICK 16666ULL
#define MOST_TICKS_PER_DRAWN_FRAME 2ULL
#define LONGEST_UPDATE_US 500000ULL
#define LONGEST_AUDIO_PRELOAD_US 10000000ULL
#define PRELOAD_PASS_MAX (TEXTURE_CACHE_ENTRY_MAX / BACKGROUND_READER_QUEUE_MAX)
#define CHAIN_FILE_NAME "chain"
#define FIXED_TABLE_COUNT 8
#define FILES_PER_SCRIPT 3
#define FAST_FORWARD_BUDGET_US 8000ULL
#define FAST_FORWARD_MOST_TICKS_PER_FRAME 8
#define FAST_FORWARD_TICKS_WHILE_ART_LOADS 1
#define ART_WAIT_MOST_US 30000000ULL
#define LOG_FOR_TOOLS_SCRIPT_READY "script ready name=%s preload_us=%llu texture_bytes=%u missing=%u"

enum script_player_phase {
    PHASE_IDLE,
    PHASE_READING,
    PHASE_LOADING_ANIMATIONS,
    PHASE_PRELOADING,
    PHASE_RUNNING,
    PHASE_NEXT_CHAPTER_CARD,
    PHASE_FINISHED
};

struct script_data {
    struct script_machine machine;
    struct script_tables tables;
    struct script_cue_list cues[SCRIPT_CHAIN_FILE_MAX];
    int cue_list_count;
    const struct script_cue_list *cues_of_next_file;
    unsigned char *file_bytes[SCRIPT_CHAIN_FILE_MAX];
    int file_bytes_count;
    struct script_talk_book *talk_book;
};

static int touch_was_down;
static int touch_is_dragging;
static float touch_last_x;
static float touch_last_y;
static char folder[FILE_PATH_MAX];
static char entry[ASSET_NAME_MAX];
static struct script_data *data;
static enum script_player_phase phase;
static int is_available;
static int is_active;
static int is_paused;
static int read_step;
static int auto_play_is_on;
static float next_chapter_card_age_ticks;
static int is_holding_for_art;
static unsigned long long art_wait_us;
static int art_wait_gave_up_was_logged;
static int latest_route_step = -1;
static unsigned int latest_route_step_began_at;
static int preload_row;
static int preload_passes;
static const char *preload_music_name;
static unsigned long long preload_started_us;
static unsigned long long time_owed_us;
static unsigned long long longest_tick_us;
static int ticks_per_frame_when_fast;
static char run_until_kind[16];
static char run_until_name[ASSET_NAME_MAX];
static unsigned int run_until_tick;

static void note_failure(const char *reason, const char *name)
{
    run_log_write("script problem reason=%s name=%s", reason, name);
}

static int read_named_file(const char *name, const char *extension, unsigned char **bytes, unsigned int *size)
{
    char path[FILE_PATH_MAX];
    unsigned long long started_us;
    int result;

    if (!file_path_join(path, folder, name, extension)) {
        return FILE_ERR_OPEN;
    }
    started_us = clock_now_us();
    result = file_read_whole(path, bytes, size);
    if (result == FILE_OK) {
        run_log_write("script read name=%s%s bytes=%u us=%llu", name, extension, *size, clock_now_us() - started_us);
    }
    return result;
}

typedef int (*text_table_reader)(struct script_tables *tables, const char *text, unsigned int text_size);

static void read_text_table(const char *name, const char *extension, text_table_reader reader, int is_required)
{
    unsigned char *bytes;
    unsigned int size;

    if (read_named_file(name, extension, &bytes, &size) != FILE_OK) {
        if (is_required) {
            note_failure("script_missing", name);
        }
        return;
    }
    if (!reader(&data->tables, (const char *)bytes, size)) {
        note_failure("script_table_invalid", name);
    }
    mem_free(bytes);
}

static void read_script_bytes(const char *name)
{
    unsigned char *bytes;
    unsigned int size;

    if (read_named_file(name, ".lmg", &bytes, &size) != FILE_OK) {
        note_failure("script_missing", name);
        return;
    }
    data->file_bytes[data->file_bytes_count] = bytes;
    data->file_bytes_count = data->file_bytes_count + 1;
    if (!script_machine_add_source(&data->machine, name, bytes, size, data->cues_of_next_file)) {
        note_failure("script_invalid", name);
    }
}

static void read_script_cues(const char *name)
{
    unsigned char *bytes;
    unsigned int size;

    data->cues_of_next_file = NULL;
    if (read_named_file(name, ".cues", &bytes, &size) != FILE_OK) {
        return;
    }
    if (!script_cues_parse((const char *)bytes, size, &data->cues[data->cue_list_count])) {
        note_failure("cues_invalid", name);
    }
    mem_free(bytes);
    data->cues_of_next_file = &data->cues[data->cue_list_count];
    data->cue_list_count = data->cue_list_count + 1;
}

static void read_script_motions(const char *name)
{
    unsigned char *bytes;
    unsigned int size;

    if (read_named_file(name, ".motions", &bytes, &size) != FILE_OK) {
        return;
    }
    if (!script_tables_read_motions(&data->tables, name, (const char *)bytes, size)) {
        note_failure("motions_invalid", name);
    }
    mem_free(bytes);
    if (read_named_file(name, ".ahead", &bytes, &size) == FILE_OK) {
        read_ahead_add_text(name, (const char *)bytes, size);
        mem_free(bytes);
    }
}

static void load_script_animations(const char *name)
{
    unsigned char *bytes;
    unsigned int size;
    unsigned long long started_us;

    started_us = clock_now_us();
    if (read_named_file(name, ".anims", &bytes, &size) != FILE_OK) {
        return;
    }
    run_log_write("script animations name=%s added=%d us=%llu", name, animation_library_add_bundle((const char *)bytes, size), clock_now_us() - started_us);
    mem_free(bytes);
}

static void begin_loading_animations(void);

static void read_talk_book(void)
{
    unsigned char *bytes;
    unsigned int size;

    if (read_named_file("talk", ".txt", &bytes, &size) != FILE_OK) {
        return;
    }
    data->talk_book = mem_alloc(sizeof(struct script_talk_book));
    if (data->talk_book == NULL) {
        note_failure("talk_out_of_memory", "talk");
    } else if (!script_talk_book_parse((const char *)bytes, size, data->talk_book)) {
        note_failure("talk_invalid", "talk");
        mem_free(data->talk_book);
        data->talk_book = NULL;
    }
    mem_free(bytes);
    data->machine.talk_book = data->talk_book;
}

static void read_next_file(void)
{
    int script_index;

    switch (read_step) {
    case 0:
        read_text_table(CHAIN_FILE_NAME, ".txt", script_tables_read_chain, 1);
        break;
    case 1:
        read_text_table("rooms", ".txt", script_tables_read_rooms, 0);
        break;
    case 2:
        read_text_table("audio_names", ".txt", script_tables_read_audio, 0);
        break;
    case 3:
        read_text_table("text_lines", ".txt", script_tables_read_text_lines, 0);
        break;
    case 4:
        read_text_table("balloons", ".txt", script_tables_read_balloons, 0);
        break;
    case 5:
        read_text_table(CHAIN_FILE_NAME, ".preload", script_tables_read_preload, 0);
        break;
    case 6:
        read_text_table("effects", ".txt", script_tables_read_effects, 0);
        read_text_table("soul", ".txt", script_tables_read_soul, 0);
        read_text_table("core_labels", ".txt", script_tables_read_core_labels, 0);
        read_text_table("route_steps", ".txt", script_tables_read_route, 0);
        read_text_table("route_pacing", ".txt", script_tables_read_route_pacing, 0);
        read_text_table("ui_clips", ".txt", script_tables_read_ui_clips, 0);
        break;
    case 7:
        read_talk_book();
        break;
    default:
        script_index = (read_step - FIXED_TABLE_COUNT) / FILES_PER_SCRIPT;
        if (script_index >= data->tables.file_count) {
            run_log_write("script chain files=%d timing=%s", data->tables.file_count, data->tables.timing == SCRIPT_TIMING_DS ? "ds" : "remaster");
            begin_loading_animations();
            return;
        }
        switch ((read_step - FIXED_TABLE_COUNT) % FILES_PER_SCRIPT) {
        case 0:
            read_script_cues(data->tables.file_names[script_index]);
            break;
        case 1:
            read_script_bytes(data->tables.file_names[script_index]);
            break;
        default:
            read_script_motions(data->tables.file_names[script_index]);
            break;
        }
        break;
    }
    read_step = read_step + 1;
}

static void request_audio(void)
{
    int index;

    preload_music_name = NULL;
    for (index = 0; index < data->tables.preload_row_count; index++) {
        const struct script_preload_row *row;

        row = &data->tables.preload_rows[index];
        if (row->kind == SCRIPT_PRELOAD_MUSIC && preload_music_name == NULL) {
            preload_music_name = row->name;
            music_request_prepare(row->name);
        }
        if (row->kind == SCRIPT_PRELOAD_SOUND) {
            sound_request_prepare(row->name);
        }
    }
}

static enum script_auto_play wanted_auto_play(void)
{
    if (ticks_per_frame_when_fast > 0) {
        return SCRIPT_AUTO_FAST;
    }
    return auto_play_is_on ? SCRIPT_AUTO_PACED : SCRIPT_AUTO_OFF;
}

static void begin_loading_animations(void)
{
    const struct script_movie *time_jump;

    script_bridge_start(&data->tables);
    data->machine.timing = data->tables.timing;
    data->machine.chapter_mark = data->tables.chapter_mark;
    if (data->tables.entry_name[0] != '\0') {
        snprintf(entry, sizeof(entry), "%s", data->tables.entry_name);
    }
    data->machine.auto_press = wanted_auto_play();
    data->machine.route = data->tables.route.step_count > 0 ? &data->tables.route : NULL;
    memcpy(data->machine.room_bounds, data->tables.room_bounds, sizeof(data->machine.room_bounds));
    time_jump = script_tables_find_movie(&data->tables, "time_jump");
    data->machine.time_jump_movie_ticks = time_jump != NULL ? time_jump->frame_count * time_jump->ticks_per_frame : 0;
    request_audio();
    preload_row = 0;
    preload_passes = 0;
    if (data->tables.rows_dropped > 0) {
        run_log_write("script problem reason=table_rows_dropped count=%d", data->tables.rows_dropped);
    }
    phase = PHASE_LOADING_ANIMATIONS;
}

static void load_next_animation(void)
{
    while (preload_row < data->tables.preload_row_count) {
        const struct script_preload_row *row;

        row = &data->tables.preload_rows[preload_row];
        preload_row = preload_row + 1;
        if (row->kind == SCRIPT_PRELOAD_ANIMATION) {
            if (animation_library_load(row->name) == NULL) {
                note_failure("animation_missing", row->name);
            }
            return;
        }
    }
    phase = PHASE_PRELOADING;
}

static void request_textures(void)
{
    int index;

    for (index = 0; index < data->tables.preload_row_count; index++) {
        const struct script_preload_row *row;
        const struct animation *animation;

        row = &data->tables.preload_rows[index];
        if (row->kind == SCRIPT_PRELOAD_TEXTURE) {
            texture_cache_use_or_start_loading(row->name);
        } else if (row->kind == SCRIPT_PRELOAD_ANIMATION) {
            animation = animation_library_load(row->name);
            if (animation != NULL) {
                scene_stage_request_animation_textures(animation);
            }
        }
    }
}

static int audio_is_ready(void)
{
    int is_ready;

    is_ready = sound_prepare_is_done() && (preload_music_name == NULL || music_prepare_is_done(preload_music_name));
    if (!is_ready && clock_now_us() - preload_started_us > LONGEST_AUDIO_PRELOAD_US) {
        run_log_write("script audio_wait_gave_up name=%s", entry);
        return 1;
    }
    return is_ready;
}

static void start_running(void)
{
    struct texture_cache_totals totals;
    int command;

    texture_cache_read_totals(&totals);
    run_log_write(LOG_FOR_TOOLS_SCRIPT_READY, entry, clock_now_us() - preload_started_us, totals.bytes_in_use, totals.missing);
    if (!script_machine_start(&data->machine, entry)) {
        note_failure("script_entry_missing", entry);
        phase = PHASE_FINISHED;
        return;
    }
    for (command = 0; command < data->tables.start_command_count; command++) {
        script_soul_run_command(&data->machine, data->tables.start_command_verbs[command], data->tables.start_command_args[command]);
    }
    time_owed_us = 0;
    longest_tick_us = 0;
    phase = PHASE_RUNNING;
}

static void finish_preloading_when_ready(void)
{
    if (texture_cache_loading_count() > 0) {
        return;
    }
    if (!audio_is_ready()) {
        return;
    }
    if (preload_passes < PRELOAD_PASS_MAX) {
        preload_passes = preload_passes + 1;
        request_textures();
        if (texture_cache_loading_count() > 0) {
            return;
        }
    } else {
        run_log_write("script does_not_fit_in_cache name=%s", entry);
    }
    start_running();
}

static void finish_run(void)
{
    phase = PHASE_FINISHED;
    music_request_stop(MUSIC_QUICK_FADE_TICKS);
    run_log_write("script finished name=%s tick=%u", entry, data->machine.tick);
    run_log_write("script tick_us_max=%llu", longest_tick_us);
    if (data->tables.next_chapter_card_texture[0] != '\0') {
        phase = PHASE_NEXT_CHAPTER_CARD;
        next_chapter_card_age_ticks = 0.0f;
        scene_stage_show_card(data->tables.next_chapter_card_texture);
        scene_stage_set_fade(next_chapter_card_fade(next_chapter_card_age_ticks));
    }
}

static void step_next_chapter_card(unsigned long long elapsed_us)
{
    next_chapter_card_age_ticks = next_chapter_card_age_ticks + (float)elapsed_us / (float)MICROSECONDS_PER_VM_TICK;
    scene_stage_set_fade(next_chapter_card_fade(next_chapter_card_age_ticks));
    if (next_chapter_card_is_over(next_chapter_card_age_ticks)) {
        scene_stage_hide_card();
        phase = PHASE_FINISHED;
    }
}

static int run_one_tick(void)
{
    unsigned long long started_us;
    unsigned long long tick_us;

    started_us = clock_now_us();
    script_machine_tick(&data->machine);
    tick_us = clock_now_us() - started_us;
    if (tick_us > longest_tick_us) {
        longest_tick_us = tick_us;
    }
    script_bridge_finish_tick();
    if (data->machine.is_finished) {
        finish_run();
        return 0;
    }
    return 1;
}

static int reached_the_run_until_point(void)
{
    if (run_until_kind[0] == '\0') {
        return 0;
    }
    if (strcmp(run_until_kind, "tick") == 0) {
        return data->machine.tick >= run_until_tick;
    }
    return strcmp(run_until_kind, "reached") == 0;
}

static void stop_at_the_run_until_point(void)
{
    run_log_write("script reached tick=%u point=%s", data->machine.tick, run_until_name);
    texture_cache_drop_waiting_reads();
    run_until_kind[0] = '\0';
    ticks_per_frame_when_fast = 0;
    data->machine.auto_press = wanted_auto_play();
    script_bridge_set_quiet(0);
    is_paused = 1;
}

static int fast_ticks_the_art_allows(void)
{
    if (texture_cache_loading_count() > 0) {
        return FAST_FORWARD_TICKS_WHILE_ART_LOADS;
    }
    return ticks_per_frame_when_fast;
}

static void run_ticks_as_fast_as_the_frame_allows(void)
{
    unsigned long long started_us;
    int ticks;
    int ticks_allowed;

    started_us = clock_now_us();
    ticks_allowed = fast_ticks_the_art_allows();
    for (ticks = 0; ticks < ticks_allowed; ticks++) {
        if (!run_one_tick()) {
            return;
        }
        if (reached_the_run_until_point()) {
            stop_at_the_run_until_point();
            return;
        }
        if (clock_now_us() - started_us > FAST_FORWARD_BUDGET_US) {
            return;
        }
    }
}

static void run_ticks(unsigned long long elapsed_us)
{
    if (ticks_per_frame_when_fast > 0) {
        time_owed_us = 0;
        run_ticks_as_fast_as_the_frame_allows();
        return;
    }
    time_owed_us = time_owed_us + elapsed_us;
    if (time_owed_us > MICROSECONDS_PER_VM_TICK * MOST_TICKS_PER_DRAWN_FRAME) {
        time_owed_us = MICROSECONDS_PER_VM_TICK * MOST_TICKS_PER_DRAWN_FRAME;
    }
    while (time_owed_us >= MICROSECONDS_PER_VM_TICK) {
        time_owed_us = time_owed_us - MICROSECONDS_PER_VM_TICK;
        if (!run_one_tick()) {
            return;
        }
    }
}

static void free_data(void)
{
    int index;

    if (data == NULL) {
        return;
    }
    for (index = 0; index < data->file_bytes_count; index++) {
        mem_free(data->file_bytes[index]);
    }
    mem_free(data->talk_book);
    mem_free(data);
    data = NULL;
}

void script_player_init(const char *scripts_folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;

    snprintf(folder, sizeof(folder), "%s", scripts_folder);
    phase = PHASE_IDLE;
    is_active = 0;
    is_paused = 0;
    data = NULL;
    is_available = 0;
    if (file_path_join(path, folder, CHAIN_FILE_NAME, ".txt") && file_read_whole(path, &bytes, &size) == FILE_OK) {
        mem_free(bytes);
        is_available = 1;
    }
}

int script_player_is_available(void)
{
    return is_available;
}

void script_player_play(const char *entry_name)
{
    snprintf(entry, sizeof(entry), "%s", entry_name);
    latest_route_step = -1;
    latest_route_step_began_at = 0;
    free_data();
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    animation_library_clear();
    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    scene_effects_reset(scene_stage_effects());
    music_request_stop(MUSIC_QUICK_FADE_TICKS);
    sound_request_stop();
    is_active = 1;
    is_paused = 0;
    read_step = 0;
    preload_started_us = clock_now_us();
    run_log_write("script started name=%s", entry);
    data = mem_alloc(sizeof(struct script_data));
    if (data == NULL) {
        note_failure("script_out_of_memory", entry);
        phase = PHASE_FINISHED;
        return;
    }
    memset(data, 0, sizeof(*data));
    script_tables_clear(&data->tables);
    read_ahead_clear();
    script_machine_init(&data->machine, script_bridge_outputs(), SCRIPT_TIMING_DS, NULL);
    phase = PHASE_READING;
}

static int art_gate_holds(unsigned long long elapsed_us)
{
    is_holding_for_art = 0;
    if (data->machine.movie_ticks_left > 0 || !scene_view_visible_art_is_loading(scene_stage_view())) {
        art_wait_us = 0;
        return 0;
    }
    if (art_wait_us >= ART_WAIT_MOST_US) {
        if (!art_wait_gave_up_was_logged) {
            run_log_write("script art_wait_gave_up us=%llu tick=%u", art_wait_us, data->machine.tick);
            art_wait_gave_up_was_logged = 1;
        }
        return 0;
    }
    art_wait_gave_up_was_logged = 0;
    art_wait_us = art_wait_us + elapsed_us;
    is_holding_for_art = 1;
    return 1;
}

int script_player_is_holding_for_art(void)
{
    return is_holding_for_art;
}

void script_player_update(unsigned long long elapsed_us)
{
    if (phase == PHASE_READING) {
        read_next_file();
        return;
    }
    if (phase == PHASE_LOADING_ANIMATIONS) {
        load_next_animation();
        return;
    }
    if (phase == PHASE_PRELOADING) {
        finish_preloading_when_ready();
        return;
    }
    if (is_paused || phase == PHASE_IDLE) {
        return;
    }
    if (elapsed_us > LONGEST_UPDATE_US) {
        run_log_write("script time_dropped us=%llu", elapsed_us - LONGEST_UPDATE_US);
        elapsed_us = LONGEST_UPDATE_US;
    }
    if (phase == PHASE_RUNNING) {
        read_ahead_step(data->machine.tick);
        if (!art_gate_holds(elapsed_us)) {
            run_ticks(elapsed_us);
            if (phase == PHASE_RUNNING && data->machine.movie_ticks_left == 0 && art_wait_us < ART_WAIT_MOST_US && scene_view_visible_art_is_loading(scene_stage_view())) {
                is_holding_for_art = 1;
            }
        }
        return;
    }
    if (phase == PHASE_NEXT_CHAPTER_CARD) {
        step_next_chapter_card(elapsed_us);
        return;
    }
    scene_effects_advance(scene_stage_effects(), elapsed_us);
    scene_layers_advance(scene_stage_layers(), elapsed_us);
}

void script_player_stop(void)
{
    if (!is_active) {
        return;
    }
    music_request_stop(MUSIC_QUICK_FADE_TICKS);
    sound_request_stop();
    scene_stage_set_fade(0.0f);
    scene_stage_hide_card();
    scene_effects_reset(scene_stage_effects());
    scene_layers_clear(scene_stage_layers());
    free_data();
    phase = PHASE_IDLE;
    is_active = 0;
    is_paused = 0;
}

void script_player_set_paused(int should_pause)
{
    if (should_pause != is_paused) {
        run_log_write("script %s", should_pause ? "paused" : "resumed");
    }
    is_paused = should_pause;
}

int script_player_is_paused(void)
{
    return is_paused;
}

int script_player_is_active(void)
{
    return is_active;
}

int script_player_is_showing_next_chapter_card(void)
{
    return phase == PHASE_NEXT_CHAPTER_CARD;
}

enum scene_playback_state script_player_status(void)
{
    switch (phase) {
    case PHASE_IDLE:
        return SCENE_PLAYER_IDLE;
    case PHASE_READING:
    case PHASE_LOADING_ANIMATIONS:
    case PHASE_PRELOADING:
        return SCENE_PLAYER_PRELOADING;
    case PHASE_RUNNING:
    case PHASE_NEXT_CHAPTER_CARD:
        return SCENE_PLAYER_RUNNING;
    default:
        return SCENE_PLAYER_FINISHED;
    }
}

void script_player_skip_ahead(void)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        run_log_write("script skip_ahead tick=%u", data->machine.tick);
        script_machine_skip_ahead(&data->machine);
    }
}

void script_player_press(void)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_machine_note_press(&data->machine);
    }
}

void script_player_ghost_button(void)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_note_ghost(&data->machine);
    }
}

static void press_the_pill(struct script_machine *machine, int pill)
{
    switch (pill) {
    case SCRIPT_PILL_GHOST:
    case SCRIPT_PILL_BACK:
        script_soul_note_ghost(machine);
        break;
    case SCRIPT_PILL_TRICK:
        script_machine_note_press(machine);
        break;
    default:
        script_soul_note_pill(machine, (enum script_pill_id)pill);
        break;
    }
}

static void touch_began(struct script_machine *machine, float x, float y)
{
    int pill;
    int balloon_kind;

    touch_is_dragging = 0;
    pill = script_ui_view_pill_at(x, y);
    balloon_kind = script_ui_view_tap_balloon_kind_at(x, y);
    run_log_write("touch began x=%d y=%d tick=%u pill=%d balloon=%d wants_tap=%d", (int)x, (int)y, machine->tick, pill, balloon_kind, script_soul_wants_a_tap(machine));
    if (pill >= 0) {
        press_the_pill(machine, pill);
        return;
    }
    if (balloon_kind != 0) {
        script_soul_note_tap(machine, balloon_kind == SCRIPT_BALLOON_KIND_TAP_DOTS ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT);
        return;
    }
    if (machine->soul.rewind_select_ticks > 0) {
        if (script_ui_view_rewind_button_is_at(x, y)) {
            script_machine_note_press(machine);
        }
        return;
    }
    if (script_soul_wants_a_tap(machine)) {
        script_machine_note_press(machine);
        return;
    }
    touch_is_dragging = 1;
    touch_last_x = x;
    touch_last_y = y;
}

static void touch_dragged(struct script_machine *machine, float x, float y)
{
    float world_x;
    float world_y;

    if (machine->soul.is_on) {
        scene_view_screen_to_world(scene_stage_view(), x, y, &world_x, &world_y);
        script_soul_set_touch(machine, 1, world_x, world_y);
    } else {
        script_soul_drag_camera(machine, (touch_last_x - x) / VITA_PIXELS_PER_DS_PIXEL, (touch_last_y - y) / VITA_PIXELS_PER_DS_PIXEL);
    }
    touch_last_x = x;
    touch_last_y = y;
}

void script_player_touch(int is_down, float x, float y)
{
    if (phase != PHASE_RUNNING || data == NULL || data->machine.auto_press != SCRIPT_AUTO_OFF) {
        touch_was_down = 0;
        return;
    }
    if (is_down && !touch_was_down) {
        touch_began(&data->machine, x, y);
    }
    if (is_down && touch_is_dragging) {
        touch_dragged(&data->machine, x, y);
    }
    if (!is_down && touch_was_down) {
        touch_is_dragging = 0;
        script_soul_set_touch(&data->machine, 0, 0.0f, 0.0f);
    }
    touch_was_down = is_down;
}

void script_player_back_button(void)
{
    if (phase == PHASE_RUNNING && data != NULL && data->machine.soul.is_on) {
        script_soul_note_ghost(&data->machine);
    }
}

void script_player_hourglass_button(void)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_note_pill(&data->machine, SCRIPT_PILL_HOURGLASS);
    }
}

void script_player_notebook_button(void)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_note_pill(&data->machine, SCRIPT_PILL_NOTEBOOK);
    }
}

void script_player_tap_balloon(int is_right)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_note_tap(&data->machine, is_right ? SCRIPT_TAP_GROUP_RIGHT : SCRIPT_TAP_GROUP_LEFT);
    }
}

void script_player_direction(int direction_x, int direction_y)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_note_direction(&data->machine, direction_x, direction_y);
    }
}

void script_player_held_direction(float held_x, float held_y)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_soul_set_held(&data->machine, held_x, held_y);
    }
}

void script_player_set_auto_play(int is_on)
{
    auto_play_is_on = is_on;
    if (data != NULL) {
        data->machine.auto_press = wanted_auto_play();
    }
    run_log_write("script auto_play on=%d", is_on);
}

void script_player_set_fast_forward(int ticks_per_frame)
{
    if (ticks_per_frame > FAST_FORWARD_MOST_TICKS_PER_FRAME) {
        ticks_per_frame = FAST_FORWARD_MOST_TICKS_PER_FRAME;
    }
    ticks_per_frame_when_fast = ticks_per_frame;
    if (data != NULL) {
        data->machine.auto_press = wanted_auto_play();
    }
    script_bridge_set_quiet(ticks_per_frame > 0);
    if (ticks_per_frame > 0) {
        is_paused = 0;
    }
    run_log_write("script fast_forward ticks_per_frame=%d", ticks_per_frame);
}

void script_player_run_until(const char *kind, const char *name)
{
    snprintf(run_until_kind, sizeof(run_until_kind), "%s", kind);
    snprintf(run_until_name, sizeof(run_until_name), "%s", name);
    run_until_tick = 0;
    if (strcmp(kind, "tick") == 0) {
        run_until_tick = (unsigned int)strtoul(name, NULL, 10);
    }
}

static int route_step_number_is(const char *step_name, const char *wanted)
{
    return strtoul(step_name, NULL, 10) == strtoul(wanted, NULL, 10) && step_name[0] >= '0' && step_name[0] <= '9';
}

void script_player_note_mark(const char *what, const char *name)
{
    if (data != NULL) {
        read_ahead_note_mark(what, name, data->machine.tick);
    }
    if (strcmp(what, "script_begun") == 0) {
        load_script_animations(name);
    }
    if (strcmp(run_until_kind, "step") == 0 && strcmp(what, "route_step_done") == 0 && route_step_number_is(name, run_until_name)) {
        snprintf(run_until_kind, sizeof(run_until_kind), "reached");
        return;
    }
    if ((strcmp(run_until_kind, "script") == 0 && strcmp(what, "script_begun") == 0 && strcmp(run_until_name, name) == 0) || (strcmp(run_until_kind, "act") == 0 && strcmp(what, "act_begun") == 0 && strcmp(run_until_name, name) == 0)) {
        snprintf(run_until_kind, sizeof(run_until_kind), "reached");
    }
}

unsigned int script_player_tick(void)
{
    return data != NULL ? data->machine.tick : 0;
}

static void remember_latest_route_step(void)
{
    if (data != NULL && data->machine.soul.route_step >= 0) {
        latest_route_step = data->machine.soul.route_step;
        latest_route_step_began_at = data->machine.soul.route_step_began_at;
    }
}

int script_player_route_step(void)
{
    remember_latest_route_step();
    return latest_route_step;
}

unsigned int script_player_route_step_began_at(void)
{
    remember_latest_route_step();
    return latest_route_step_began_at;
}

void script_player_shutdown(void)
{
    free_data();
    phase = PHASE_IDLE;
    is_active = 0;
}
