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
#include "playback_speed.h"
#include "packed_file.h"
#include "scene_stage.h"
#include "pause_clock.h"
#include "script_bridge.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "script_machine.h"
#include "scene_view.h"
#include "read_ahead.h"
#include "game_save.h"
#include "game_settings.h"
#include "script_player.h"
#include "script_ui_view.h"
#include "script_view.h"
#include "script_tables.h"
#include "sound_engine.h"
#include "texture_cache.h"
#include "ds_units.h"

#define MICROSECONDS_PER_VM_TICK 16666ULL
#define AUDIO_MOST_SPEED_OUT_OF_256 (32 * PLAYBACK_SPEED_NORMAL)
#define AUDIO_SPEED_SMOOTHING 8
#define MOST_TICKS_PER_DRAWN_FRAME 2ULL
#define LONGEST_UPDATE_US 500000ULL
#define LONGEST_AUDIO_PRELOAD_US 10000000ULL
#define PRELOAD_PASS_MAX (TEXTURE_CACHE_ENTRY_MAX / BACKGROUND_READER_QUEUE_MAX)
#define CHAIN_FILE_NAME "chain"
#define FIXED_TABLE_COUNT 8
#define FILES_PER_SCRIPT 3
#define FAST_FORWARD_FRAME_US 16667ULL
#define FAST_FORWARD_FRAME_SLACK_US 600ULL
#define FAST_FORWARD_LEAST_BUDGET_US 2000ULL
#define FAST_FORWARD_MOST_BUDGET_US 13000ULL
#define FAST_FORWARD_BUDGET_SHRINK_US 1500ULL
#define FAST_FORWARD_BUDGET_GROWTH_US 250ULL
#define FAST_FORWARD_MOST_TICKS_PER_FRAME 32
#define FAST_FORWARD_TICKS_WHILE_DRAWN_ART_LOADS 1
#define FAST_FORWARD_SHARE_WHILE_READ_AHEAD_IS_BEHIND 4
#define READ_AHEAD_MOST_LOOKAHEAD_SCALE 8
#define FAST_FORWARD_REPORT_EVERY_US 1000000ULL
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
    unsigned char *ahead_bytes[SCRIPT_CHAIN_FILE_MAX];
    int ahead_bytes_count;
    struct script_talk_book *talk_book;
};

static struct script_input pad_input;
static struct script_input route_input;
static struct script_route_pad route_pad;
static char folder[FILE_PATH_MAX];
static char entry[ASSET_NAME_MAX];
static struct script_data *data;
static struct game_save checkpoint;
static int has_checkpoint;
static struct game_save pending_resume;
static int has_pending_resume;
static int autosaved_chapter_mark = -1;
static int newly_reached_chapter;

static void take_checkpoint(const char *name);
static enum script_player_phase phase;
static int is_available;
static int is_active;
static int is_paused;
static int read_step;
static int stall_breaker_is_on = 1;
static int auto_play_is_on;
static float next_chapter_card_age_ticks;
static int is_holding_for_art;
static unsigned long long art_wait_us;
static int art_wait_gave_up_was_logged;
static int latest_acted_route_step = -1;
static unsigned int latest_route_action_began_at;
static int preload_row;
static int preload_passes;
static int most_textures_loading;
static const char *preload_music_name;
static unsigned long long preload_started_us;
static unsigned long long time_owed_us;
static unsigned long long longest_tick_us;
static int ticks_per_frame_when_fast;
static int audio_speed_out_of_256 = PLAYBACK_SPEED_NORMAL;
static unsigned long long fast_forward_budget_us = FAST_FORWARD_LEAST_BUDGET_US;
static unsigned long long fast_forward_report_us;
static unsigned int fast_forward_ticks_since_report;
static unsigned int fast_forward_slowed_frames_since_report;
static char run_until_kind[16];
static char run_until_name[ASSET_NAME_MAX];
static unsigned int run_until_tick;

static void note_failure(const char *reason, const char *name)
{
    run_log_write("script problem reason=%s name=%s", reason, name);
}

static int unpack_if_packed(const char *name, const char *extension, unsigned char **bytes, unsigned int *size)
{
    unsigned char *unpacked;
    unsigned int unpacked_size;
    unsigned long long started_us;

    if (!packed_file_is_packed(*bytes, *size)) {
        return FILE_OK;
    }
    started_us = clock_now_us();
    unpacked_size = packed_file_unpacked_size(*bytes);
    unpacked = mem_alloc(unpacked_size + 1u);
    if (unpacked == NULL) {
        mem_free(*bytes);
        return FILE_ERR_MEMORY;
    }
    if (!packed_file_unpack(*bytes, *size, unpacked, unpacked_size)) {
        run_log_write("script problem reason=unpack_failed name=%s%s", name, extension);
        mem_free(unpacked);
        mem_free(*bytes);
        return FILE_ERR_READ;
    }
    unpacked[unpacked_size] = '\0';
    mem_free(*bytes);
    *bytes = unpacked;
    *size = unpacked_size;
    run_log_write("script unpacked name=%s%s bytes=%u us=%llu", name, extension, unpacked_size, clock_now_us() - started_us);
    return FILE_OK;
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
    if (result != FILE_OK) {
        return result;
    }
    run_log_write("script read name=%s%s bytes=%u us=%llu", name, extension, *size, clock_now_us() - started_us);
    return unpack_if_packed(name, extension, bytes, size);
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
    if (data->ahead_bytes_count < SCRIPT_CHAIN_FILE_MAX && read_named_file(name, ".ahead", &bytes, &size) == FILE_OK) {
        data->ahead_bytes[data->ahead_bytes_count] = bytes;
        data->ahead_bytes_count = data->ahead_bytes_count + 1;
        read_ahead_add_text(name, (const char *)bytes, size);
    }
}

#define LOADED_BUNDLE_MAX 64

static char loaded_bundle_names[LOADED_BUNDLE_MAX][ASSET_NAME_MAX];
static int loaded_bundle_evictions[LOADED_BUNDLE_MAX];
static int loaded_bundle_count;
static int next_loaded_bundle_slot;

static enum script_text_speed talk_speed_of(int text_speed)
{
    if (text_speed == GAME_TEXT_SLOW) {
        return SCRIPT_TEXT_SLOW;
    }
    if (text_speed == GAME_TEXT_FAST) {
        return SCRIPT_TEXT_FAST;
    }
    return SCRIPT_TEXT_NORMAL;
}

static void forget_loaded_bundles(void)
{
    loaded_bundle_count = 0;
    next_loaded_bundle_slot = 0;
}

static int bundle_is_still_whole(const char *name)
{
    int index;

    for (index = 0; index < loaded_bundle_count; index++) {
        if (strcmp(loaded_bundle_names[index], name) == 0) {
            return loaded_bundle_evictions[index] == animation_library_evicted_count();
        }
    }
    return 0;
}

static void remember_loaded_bundle(const char *name)
{
    int index;

    for (index = 0; index < loaded_bundle_count; index++) {
        if (strcmp(loaded_bundle_names[index], name) == 0) {
            loaded_bundle_evictions[index] = animation_library_evicted_count();
            return;
        }
    }
    index = next_loaded_bundle_slot;
    next_loaded_bundle_slot = (next_loaded_bundle_slot + 1) % LOADED_BUNDLE_MAX;
    if (loaded_bundle_count < LOADED_BUNDLE_MAX) {
        loaded_bundle_count = loaded_bundle_count + 1;
    }
    snprintf(loaded_bundle_names[index], sizeof(loaded_bundle_names[index]), "%s", name);
    loaded_bundle_evictions[index] = animation_library_evicted_count();
}

static void load_script_animations(const char *name)
{
    unsigned char *bytes;
    unsigned int size;
    unsigned long long started_us;

    if (bundle_is_still_whole(name)) {
        run_log_write("script animations name=%s kept", name);
        return;
    }
    if (data != NULL && !script_tables_has_motions(&data->tables, name)) {
        run_log_write("script animations name=%s none", name);
        return;
    }
    started_us = clock_now_us();
    if (read_named_file(name, ".anims", &bytes, &size) != FILE_OK) {
        return;
    }
    run_log_write("script animations name=%s added=%d us=%llu", name, animation_library_add_bundle((const char *)bytes, size), clock_now_us() - started_us);
    mem_free(bytes);
    remember_loaded_bundle(name);
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
        if (data->tables.route.pacing_is_stale) {
            run_log_write("script route_pacing_stale using each step's pace");
        }
        read_text_table("scenes", ".txt", script_tables_read_scenes, 0);
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

static void apply_auto_play(void)
{
    data->machine.auto_press = SCRIPT_AUTO_OFF;
    data->machine.route_is_driving = auto_play_is_on;
    data->machine.stall_breaker_is_off = !stall_breaker_is_on;
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
    if (has_pending_resume) {
        snprintf(entry, sizeof(entry), "%s", pending_resume.script);
        game_save_apply_before_start(&pending_resume, &data->machine);
        autosaved_chapter_mark = pending_resume.chapter_mark;
        run_log_write("save resumed script=%s chapter_mark=%d", pending_resume.script, pending_resume.chapter_mark);
    }
    apply_auto_play();
    data->machine.route = data->tables.route.step_count > 0 ? &data->tables.route : NULL;
    memcpy(data->machine.room_bounds, data->tables.room_bounds, sizeof(data->machine.room_bounds));
    data->machine.trickless_core_infos = data->tables.trickless_core_infos;
    data->machine.trickless_core_info_count = data->tables.trickless_core_info_count;
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
    most_textures_loading = 0;
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
    if (has_pending_resume) {
        game_save_apply_after_start(&pending_resume, &data->machine);
        has_pending_resume = 0;
        take_checkpoint(entry);
    }
    for (command = 0; command < SCRIPT_STAFF_PAGE_MAX; command++) {
        script_staff_roll_set_screens(&data->machine.staff_roll, command, data->tables.staff_page_screens[command]);
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
    if (data->machine.route_is_driving) {
        struct input_state frame;

        script_route_pad_frame(&route_pad, &data->machine, &frame);
        script_input_frame(&route_input, &data->machine, &frame);
    }
    data->machine.text_speed = talk_speed_of(game_settings_now()->text_speed);
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
    apply_auto_play();
    audio_speed_out_of_256 = PLAYBACK_SPEED_NORMAL;
    playback_speed_set(PLAYBACK_SPEED_NORMAL);
    is_paused = 1;
}

static int fast_ticks_the_art_allows(void)
{
    int share;

    if (texture_cache_drawn_loading_count() > 0) {
        return FAST_FORWARD_TICKS_WHILE_DRAWN_ART_LOADS;
    }
    if (texture_cache_read_ahead_is_behind()) {
        share = ticks_per_frame_when_fast / FAST_FORWARD_SHARE_WHILE_READ_AHEAD_IS_BEHIND;
        return share > 1 ? share : 1;
    }
    return ticks_per_frame_when_fast;
}

static void fit_the_fast_forward_budget_to_the_frame(unsigned long long frame_us)
{
    if (frame_us > FAST_FORWARD_FRAME_US + FAST_FORWARD_FRAME_SLACK_US) {
        fast_forward_budget_us = fast_forward_budget_us > FAST_FORWARD_LEAST_BUDGET_US + FAST_FORWARD_BUDGET_SHRINK_US ? fast_forward_budget_us - FAST_FORWARD_BUDGET_SHRINK_US : FAST_FORWARD_LEAST_BUDGET_US;
    } else if (fast_forward_budget_us + FAST_FORWARD_BUDGET_GROWTH_US <= FAST_FORWARD_MOST_BUDGET_US) {
        fast_forward_budget_us = fast_forward_budget_us + FAST_FORWARD_BUDGET_GROWTH_US;
    }
}

static void report_fast_forward_speed(unsigned long long frame_us, int ticks_run, int ticks_allowed)
{
    fast_forward_report_us = fast_forward_report_us + frame_us;
    fast_forward_ticks_since_report = fast_forward_ticks_since_report + (unsigned int)ticks_run;
    if (ticks_allowed < ticks_per_frame_when_fast) {
        fast_forward_slowed_frames_since_report = fast_forward_slowed_frames_since_report + 1u;
    }
    if (fast_forward_report_us < FAST_FORWARD_REPORT_EVERY_US) {
        return;
    }
    run_log_write("script fast_forward_speed ticks_per_second=%llu budget_us=%llu slowed_for_art_frames=%u", (unsigned long long)fast_forward_ticks_since_report * 1000000ULL / fast_forward_report_us, fast_forward_budget_us, fast_forward_slowed_frames_since_report);
    fast_forward_report_us = 0;
    fast_forward_ticks_since_report = 0;
    fast_forward_slowed_frames_since_report = 0;
}

static void follow_the_tick_rate_with_audio(unsigned long long frame_us, int ticks_run)
{
    int frame_speed;

    if (frame_us == 0) {
        return;
    }
    frame_speed = (int)((unsigned long long)ticks_run * MICROSECONDS_PER_VM_TICK * PLAYBACK_SPEED_NORMAL / frame_us);
    if (frame_speed > AUDIO_MOST_SPEED_OUT_OF_256) {
        frame_speed = AUDIO_MOST_SPEED_OUT_OF_256;
    }
    audio_speed_out_of_256 = (audio_speed_out_of_256 * (AUDIO_SPEED_SMOOTHING - 1) + frame_speed) / AUDIO_SPEED_SMOOTHING;
    playback_speed_set(audio_speed_out_of_256);
}

static void run_ticks_as_fast_as_the_frame_allows(unsigned long long frame_us)
{
    unsigned long long started_us;
    int ticks_run;
    int ticks_allowed;

    fit_the_fast_forward_budget_to_the_frame(frame_us);
    started_us = clock_now_us();
    ticks_allowed = fast_ticks_the_art_allows();
    ticks_run = 0;
    while (ticks_run < ticks_allowed) {
        if (!run_one_tick()) {
            break;
        }
        ticks_run = ticks_run + 1;
        if (reached_the_run_until_point()) {
            stop_at_the_run_until_point();
            break;
        }
        if (clock_now_us() - started_us > fast_forward_budget_us) {
            break;
        }
    }
    report_fast_forward_speed(frame_us, ticks_run, ticks_allowed);
    follow_the_tick_rate_with_audio(frame_us, ticks_run);
}

static void run_ticks(unsigned long long elapsed_us)
{
    if (ticks_per_frame_when_fast > 0) {
        time_owed_us = 0;
        run_ticks_as_fast_as_the_frame_allows(elapsed_us);
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
    read_ahead_clear();
    for (index = 0; index < data->file_bytes_count; index++) {
        mem_free(data->file_bytes[index]);
    }
    for (index = 0; index < data->ahead_bytes_count; index++) {
        mem_free(data->ahead_bytes[index]);
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
    latest_acted_route_step = -1;
    latest_route_action_began_at = 0;
    free_data();
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    animation_library_clear();
    forget_loaded_bundles();
    scene_stage_hide_card();
    scene_stage_set_fade(1.0f);
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
    script_machine_start_from_black(&data->machine);
    phase = PHASE_READING;
}

static int shown_art_is_loading(void)
{
    if (script_view_movie_covers_the_screen()) {
        return script_view_movie_is_waiting_for_frames();
    }
    return scene_view_visible_art_is_loading(scene_stage_view()) || script_bridge_balloon_art_is_loading();
}

static int art_gate_holds(unsigned long long elapsed_us)
{
    is_holding_for_art = 0;
    if (!shown_art_is_loading()) {
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

void script_player_load_counts(struct loading_counts *counts)
{
    int loading;

    counts->done = 0;
    counts->total = 0;
    if (data == NULL) {
        counts->stage = LOADING_DONE;
    } else if (phase == PHASE_READING) {
        counts->stage = LOADING_READING;
        counts->done = read_step;
        counts->total = FIXED_TABLE_COUNT + data->tables.file_count * FILES_PER_SCRIPT;
    } else if (phase == PHASE_LOADING_ANIMATIONS) {
        counts->stage = LOADING_ANIMATIONS;
        counts->done = preload_row;
        counts->total = data->tables.preload_row_count;
    } else if (phase == PHASE_PRELOADING) {
        counts->stage = LOADING_TEXTURES;
        loading = texture_cache_loading_count();
        if (loading > most_textures_loading) {
            most_textures_loading = loading;
        }
        counts->done = most_textures_loading > 0 ? most_textures_loading - loading : 1;
        counts->total = most_textures_loading > 0 ? most_textures_loading : 1;
    } else {
        counts->stage = LOADING_DONE;
    }
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
            if (phase == PHASE_RUNNING && art_wait_us < ART_WAIT_MOST_US && shown_art_is_loading()) {
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

const unsigned char *script_player_note_levels(void)
{
    return data == NULL ? NULL : &data->machine.note_levels[0][0];
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

void script_player_input_frame(const struct input_state *frame)
{
    if (phase == PHASE_RUNNING && data != NULL) {
        script_input_frame(&pad_input, &data->machine, frame);
    }
}

void script_player_set_stall_breaker(int is_on)
{
    stall_breaker_is_on = is_on;
    if (data != NULL) {
        apply_auto_play();
    }
    run_log_write("script stall_breaker on=%d", is_on);
}

void script_player_set_auto_play(int is_on)
{
    auto_play_is_on = is_on;
    if (data != NULL) {
        apply_auto_play();
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
        apply_auto_play();
    }
    audio_speed_out_of_256 = PLAYBACK_SPEED_NORMAL;
    playback_speed_set(PLAYBACK_SPEED_NORMAL);
    read_ahead_set_lookahead_scale(ticks_per_frame > READ_AHEAD_MOST_LOOKAHEAD_SCALE ? READ_AHEAD_MOST_LOOKAHEAD_SCALE : (ticks_per_frame > 1 ? ticks_per_frame : 1));
    fast_forward_budget_us = FAST_FORWARD_LEAST_BUDGET_US;
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

static void take_checkpoint(const char *name)
{
    if (data == NULL || has_pending_resume) {
        return;
    }
    game_save_capture(&checkpoint, &data->machine, name);
    has_checkpoint = 1;
    if (autosaved_chapter_mark >= 0 && checkpoint.chapter_mark > autosaved_chapter_mark) {
        (void)game_save_store_as_newest(&checkpoint, GAME_SAVE_AUTO);
        (void)game_save_store_chapter(&checkpoint, checkpoint.chapter_mark);
        newly_reached_chapter = checkpoint.chapter_mark;
    }
    if (checkpoint.chapter_mark > autosaved_chapter_mark) {
        autosaved_chapter_mark = checkpoint.chapter_mark;
    }
}

int script_player_takes_record_request(void)
{
    if (data == NULL || phase != PHASE_RUNNING || !data->machine.record_is_wanted) {
        return 0;
    }
    data->machine.record_is_wanted = 0;
    return 1;
}

int script_player_takes_new_chapter(void)
{
    int chapter;

    chapter = newly_reached_chapter;
    newly_reached_chapter = 0;
    return chapter;
}

int script_player_save_manual(void)
{
    if (!has_checkpoint) {
        run_log_write("save refused reason=no_checkpoint");
        return 0;
    }
    return game_save_store_as_newest(&checkpoint, GAME_SAVE_MANUAL);
}

void script_player_resume(const struct game_save *save)
{
    pending_resume = *save;
    has_pending_resume = 1;
    has_checkpoint = 0;
    script_player_play(save->script);
}

void script_player_note_mark(const char *what, const char *name)
{
    if (data != NULL) {
        read_ahead_note_mark(what, name, data->machine.tick);
    }
    if (strcmp(what, "script_begun") == 0) {
        load_script_animations(name);
        take_checkpoint(name);
        pause_clock_note_scene(data != NULL ? data->machine.chapter_mark : 0, name);
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

static void remember_latest_route_action(void)
{
    if (data != NULL && data->machine.soul.route_acted_step >= 0) {
        latest_acted_route_step = data->machine.soul.route_acted_step;
        latest_route_action_began_at = data->machine.soul.route_action_began_at;
    }
}

int script_player_acted_route_step(void)
{
    remember_latest_route_action();
    return latest_acted_route_step;
}

unsigned int script_player_route_action_began_at(void)
{
    remember_latest_route_action();
    return latest_route_action_began_at;
}

void script_player_shutdown(void)
{
    free_data();
    phase = PHASE_IDLE;
    is_active = 0;
}
