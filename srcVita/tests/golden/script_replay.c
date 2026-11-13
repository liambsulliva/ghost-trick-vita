#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_save.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_route_pad.h"
#include "script_tables.h"
#include "script_flags.h"

#define FILE_NAME_MAX 512
#define TEXT_BYTES_MAX (1024 * 1024)

static struct script_route_pad route_pad;
static struct script_input route_input;
static FILE *rows_output;
static FILE *cues_output;
static struct script_machine machine;
static struct script_cue_list cue_lists[SCRIPT_SOURCE_MAX];
static int cue_list_count;
static int marks_to_stdout;
#define STATE_SAVE_MAX 24

static const char *state_save_scripts[STATE_SAVE_MAX];
static const char *state_save_paths[STATE_SAVE_MAX];
static int state_was_saved[STATE_SAVE_MAX];
static int state_save_count;
static int soul_state_was_loaded;

static void print_fetch(const struct script_fetch *fetch)
{
    int index;

    if (rows_output == NULL) {
        return;
    }
    fprintf(rows_output, "%u\t%s\t%s\t%x\t%x\t", fetch->tick, fetch->vm_kind, fetch->file_name, fetch->offset, fetch->op);
    for (index = 0; index < fetch->word_count; index++) {
        fprintf(rows_output, "%s%04x", index == 0 ? "" : " ", fetch->words[index]);
    }
    fprintf(rows_output, "\t%d\t%s\n", fetch->condition, fetch->object_name);
}

#define CUE(...) do { if (cues_output != NULL) { fprintf(cues_output, "%u\t", machine.tick); fprintf(cues_output, __VA_ARGS__); fprintf(cues_output, "\n"); } } while (0)

static void on_stage_cleared(void) { CUE("stage_cleared"); }
static void on_room(int room, int x, int y) { CUE("room %d camera %d %d", room, x, y); }
static void on_camera(int x, int y) { CUE("camera %d %d", x, y); }
static void on_camera_moved(int x, int y) { (void)x; (void)y; }
static void on_actor(const struct script_actor_view *actor) { CUE("actor %s table=%x frame=%d into=%d x=%d y=%d level=%d visible=%d", actor->name, actor->table_offset, actor->frame_word_index, actor->ticks_into_frame, actor->x, actor->y, actor->level, actor->is_visible); }
static void on_brightness(int level) { CUE("brightness %d", level); }
static void on_mask_fade(int alive, int alpha) { CUE("mask_fade alive=%d alpha=%d", alive, alpha); }
static void on_beam_created(const char *name, float ax, float ay, float bx, float by, float width, float cover) { (void)cover; CUE("beam_create %s %.1f %.1f %.1f %.1f %.1f", name, ax, ay, bx, by, width); }
static void on_beam_width(const char *name, float width, int ticks) { CUE("beam_width %s %.1f ticks=%d", name, width, ticks); }
static void on_beam_base(const char *name, float x, float y, int ticks) { CUE("beam_base %s %.1f %.1f ticks=%d", name, x, y, ticks); }
static void on_beam_deleted(const char *name) { CUE("beam_delete %s", name); }
static void on_iris_created(const char *name, float x, float y, float radius, float alpha) { CUE("iris_create %s %.1f %.1f %.1f alpha=%.2f", name, x, y, radius, alpha); }
static void on_iris_alpha(const char *name, float alpha, int ticks) { CUE("iris_alpha %s %.2f ticks=%d", name, alpha, ticks); }
static void on_iris_radius(const char *name, float radius, int ticks) { CUE("iris_radius %s %.1f ticks=%d", name, radius, ticks); }
static void on_iris_deleted(const char *name) { CUE("iris_delete %s", name); }
static void on_effect(int id, const char *name, float x, float y) { CUE("effect %d %s %.1f %.1f", id, name, x, y); }
static void on_narration(const char *label, enum script_narration_event event, int group) { CUE("narration %s event=%d group=%d", label, (int)event, group); }
static void on_balloon_opened(int slot, const char *label, int kind, float x, float y) { CUE("balloon_open slot=%d label=%s kind=%d anchor %.2f %.2f", slot, label != NULL ? label : "-", kind, x, y); }
static void on_balloon_closed(int slot) { CUE("balloon_close slot=%d", slot); }
static void on_music_prepared(int id) { CUE("music_prepare %d", id); }
static void on_music_started(int id) { CUE("music_start %d", id); }
static void on_music_stopped(int fade) { CUE("music_stop %d", fade); }
static void on_sound(int kind) { CUE("sound %d", kind); }
static void on_soul_vision(int is_on, int with_effect) { (void)with_effect; CUE("soul_vision %d", is_on); }
static void write_state(const char *path, const char *script)
{
    static struct game_save save;
    static char text[GAME_SAVE_TEXT_MAX];
    unsigned int size;
    FILE *file;

    game_save_capture(&save, &machine, script);
    size = game_save_write_text(&save, text, sizeof(text));
    file = fopen(path, "w");
    if (file == NULL) {
        return;
    }
    fwrite(text, 1, size, file);
    fclose(file);
}

static void save_state_when_due(const char *what, const char *name)
{
    int index;

    if (strcmp(what, "script_begun") != 0) {
        return;
    }
    for (index = 0; index < state_save_count; index++) {
        if (!state_was_saved[index] && strcmp(name, state_save_scripts[index]) == 0) {
            state_was_saved[index] = 1;
            write_state(state_save_paths[index], name);
        }
    }
}

static struct game_save loaded_save;

static int load_state(const char *path)
{
    static char text[GAME_SAVE_TEXT_MAX];
    size_t size;
    FILE *file;

    file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }
    size = fread(text, 1, sizeof(text), file);
    fclose(file);
    if (!game_save_read_text(&loaded_save, text, (unsigned int)size)) {
        return 0;
    }
    game_save_apply_before_start(&loaded_save, &machine);
    soul_state_was_loaded = loaded_save.has_soul_state;
    return 1;
}

static struct script_flags flags_at_last_step;
static void note_flag_changes(void)
{
    char changes[1024];
    size_t used;
    unsigned int flag;

    used = 0;
    changes[0] = '\0';
    for (flag = 0; flag < 2u * SCRIPT_FLAG_COUNT && used + 16 < sizeof(changes); flag++) {
        int now;

        if (!script_flags_is_known(flag)) {
            continue;
        }
        now = script_flags_read(&machine.flags, flag);
        if (now != script_flags_read(&flags_at_last_step, flag)) {
            used = used + (size_t)snprintf(changes + used, sizeof(changes) - used, " %c0x%x", now ? '+' : '-', flag);
        }
    }
    flags_at_last_step = machine.flags;
    if (used > 0) {
        CUE("flags%s", changes);
        if (marks_to_stdout) {
            printf("flags%s\n", changes);
        }
    }
}
static void print_cores(void)
{
    int index;

    for (index = 0; index < machine.object_count; index++) {
        const struct script_actor *actor;

        actor = &machine.objects[index].actor;
        if (!actor->exists || (!actor->has_core && !actor->has_second_core)) {
            continue;
        }
        printf("core %s at %d,%d second %d,%d usable %d%d visible %d hidden %d%d dead %d\n", machine.objects[index].name, actor->x + actor->core_x, actor->y + actor->core_y, actor->has_second_core ? actor->x + actor->second_core_x : 0, actor->has_second_core ? actor->y + actor->second_core_y : 0, actor->has_core && !(actor->disabled_cores & 1), actor->has_second_core && !(actor->disabled_cores & 2), actor->is_visible, actor->is_hard_hidden, actor->is_frozen_and_hidden, machine.objects[index].is_dead);
    }
}

static const char *traced_core_name;
static int traced_core_x = -100000;
static int traced_core_y = -100000;
#define WATCH_EVERY_TICKS 20

static const char *watched_actor_names;

static void note_watched_actors(void)
{
    int index;

    if (watched_actor_names == NULL || machine.tick % WATCH_EVERY_TICKS != 0) {
        return;
    }
    printf("watch %u", machine.tick);
    for (index = 0; index < machine.object_count; index++) {
        const struct script_actor *actor;
        const char *found;
        size_t length;

        actor = &machine.objects[index].actor;
        length = strlen(machine.objects[index].name);
        found = strstr(watched_actor_names, machine.objects[index].name);
        if (found == NULL || (found[length] != ',' && found[length] != '\0') || (found != watched_actor_names && found[-1] != ',') || machine.objects[index].is_deleted) {
            continue;
        }
        printf(" %s %d,%d f%d r%d,%d", machine.objects[index].name, actor->x + actor->points[0][0], actor->y + actor->points[0][1], actor->is_flipped ? 1 : 0, actor->counters[0], actor->counters[4]);
    }
    printf("\n");
}

static void note_traced_core(void)
{
    int index;

    if (traced_core_name == NULL) {
        return;
    }
    for (index = 0; index < machine.object_count; index++) {
        const struct script_actor *actor;

        actor = &machine.objects[index].actor;
        if (strcmp(machine.objects[index].name, traced_core_name) != 0 || !actor->has_core) {
            continue;
        }
        if (actor->x + actor->core_x != traced_core_x || actor->y + actor->core_y != traced_core_y) {
            traced_core_x = actor->x + actor->core_x;
            traced_core_y = actor->y + actor->core_y;
            CUE("core_at %s %d,%d soul_view=%d", traced_core_name, traced_core_x, traced_core_y, machine.soul.is_on);
        }
        return;
    }
}

static void on_marked(const char *what, const char *name) { CUE("mark %s %s", what, name); if (marks_to_stdout) { printf("mark %s %s\n", what, name); if (strcmp(what, "route_hop_beyond_reach") == 0) { print_cores(); } } save_state_when_due(what, name); if (strcmp(what, "route_step_done") == 0) { note_flag_changes(); } }
static void on_talk(const struct script_talk_view *view) { CUE("talk slide=%.2f alpha=%.2f glyphs=%d cursor=%d open=%d", view->slide, view->text_alpha, view->glyphs_shown, view->cursor_is_on, view->page != NULL); }
static void on_effect_deleted(const char *name) { CUE("effect_deleted %s", name); }
static void on_screen_filter(int mode) { CUE("screen_filter %d", mode); }
static unsigned char second_core_was_marked[SCRIPT_OBJECT_MAX];
static int pills_on_screen_before = -1;
static void on_film_frame(int is_on) { CUE("film_frame %d", is_on); }
static void on_hud(const struct script_hud_view *view) { int mask = 0; int index; for (index = 0; index < SCRIPT_PILL_COUNT; index++) { if (view->pills[index].is_on_screen) { mask |= 1 << index; } } if (mask != pills_on_screen_before) { pills_on_screen_before = mask; CUE("pills_on_screen %d", mask); } }
static void on_core_marked(int object_index, int slot, float x, float y, int is_dead, float opacity) { (void)x; (void)y; (void)is_dead; (void)opacity; if (slot == 1 && object_index >= 0 && object_index < SCRIPT_OBJECT_MAX && !second_core_was_marked[object_index]) { second_core_was_marked[object_index] = 1; CUE("second_core_marked %s", machine.objects[object_index].name); } }

static const struct script_outputs recording_outputs = {
    .fetched = print_fetch,
    .stage_cleared = on_stage_cleared,
    .room_loaded = on_room,
    .camera_cut = on_camera,
    .camera_moved = on_camera_moved,
    .actor_changed = on_actor,
    .brightness_changed = on_brightness,
    .mask_fade_changed = on_mask_fade,
    .beam_created = on_beam_created,
    .beam_width_ramped = on_beam_width,
    .beam_base_ramped = on_beam_base,
    .beam_deleted = on_beam_deleted,
    .iris_created = on_iris_created,
    .iris_radius_ramped = on_iris_radius,
    .iris_alpha_ramped = on_iris_alpha,
    .iris_deleted = on_iris_deleted,
    .effect_spawned = on_effect,
    .effect_deleted = on_effect_deleted,
    .narration_changed = on_narration,
    .balloon_opened = on_balloon_opened,
    .balloon_closed = on_balloon_closed,
    .music_prepared = on_music_prepared,
    .music_started = on_music_started,
    .music_stopped = on_music_stopped,
    .sound_played = on_sound,
    .soul_vision_changed = on_soul_vision,
    .screen_filter_changed = on_screen_filter,
    .marked = on_marked,
    .talk_changed = on_talk,
    .soul_core_marked = on_core_marked,
    .film_frame_changed = on_film_frame,
    .hud_changed = on_hud
};

static unsigned char *read_file(const char *path, unsigned int *size)
{
    unsigned char *bytes;
    FILE *source;
    long length;

    source = fopen(path, "rb");
    if (source == NULL) {
        return NULL;
    }
    fseek(source, 0, SEEK_END);
    length = ftell(source);
    fseek(source, 0, SEEK_SET);
    bytes = malloc((size_t)length + 1);
    if (fread(bytes, 1, (size_t)length, source) != (size_t)length) {
        exit(1);
    }
    fclose(source);
    *size = (unsigned int)length;
    return bytes;
}

static void add_script(const char *folder, const char *name)
{
    char path[FILE_NAME_MAX];
    const struct script_cue_list *cues;
    unsigned char *bytes;
    unsigned int size;

    cues = NULL;
    snprintf(path, sizeof(path), "%s/%s.cues", folder, name);
    bytes = read_file(path, &size);
    if (bytes != NULL) {
        if (!script_cues_parse((const char *)bytes, size, &cue_lists[cue_list_count])) {
            printf("bad cues in %s\n", path);
            exit(1);
        }
        cues = &cue_lists[cue_list_count];
        cue_list_count = cue_list_count + 1;
    }
    snprintf(path, sizeof(path), "%s/%s.lmg", folder, name);
    bytes = read_file(path, &size);
    if (bytes == NULL || !script_machine_add_source(&machine, name, bytes, size, cues)) {
        printf("cannot read %s\n", path);
        exit(1);
    }
}

#define SCRIPT_FLAG_CHANGE_MAX 64
static char chain_entry[SCRIPT_SOURCE_NAME_MAX] = "route";
static char start_verbs[SCRIPT_START_COMMAND_MAX][SCRIPT_SOURCE_NAME_MAX];
static char start_args[SCRIPT_START_COMMAND_MAX][SCRIPT_SOURCE_NAME_MAX];
static int start_count;

static void add_every_script_of_the_chain(const char *folder)
{
    char path[FILE_NAME_MAX];
    char name[SCRIPT_SOURCE_NAME_MAX];
    char line[FILE_NAME_MAX];
    FILE *source;
    int chapter_mark;

    snprintf(path, sizeof(path), "%s/chain.txt", folder);
    source = fopen(path, "r");
    if (source == NULL) {
        printf("cannot read %s\n", path);
        exit(1);
    }
    while (fgets(line, sizeof(line), source) != NULL) {
        if (sscanf(line, "file %95s", name) == 1) {
            add_script(folder, name);
        } else if (sscanf(line, "entry %95s", name) == 1) {
            snprintf(chain_entry, sizeof(chain_entry), "%s", name);
        } else if (sscanf(line, "chapter_mark %d", &chapter_mark) == 1) {
            machine.chapter_mark = chapter_mark;
        } else if (start_count < SCRIPT_START_COMMAND_MAX && sscanf(line, "start_command %95s %95s", start_verbs[start_count], start_args[start_count]) == 2) {
            start_count = start_count + 1;
        }
    }
    fclose(source);
}

static struct script_route replay_route;

static void read_route(const char *folder)
{
    char path[FILE_NAME_MAX];
    static char text[SCRIPT_ROUTE_STEP_MAX * 256];
    FILE *source;
    size_t size;

    snprintf(path, sizeof(path), "%s/route_steps.txt", folder);
    source = fopen(path, "r");
    if (source == NULL) {
        return;
    }
    size = fread(text, 1, sizeof(text), source);
    fclose(source);
    if (!script_route_read(&replay_route, text, (unsigned int)size)) {
        printf("route_steps.txt is invalid\n");
        return;
    }
    snprintf(path, sizeof(path), "%s/route_pacing.txt", folder);
    source = fopen(path, "r");
    if (source != NULL) {
        size = fread(text, 1, sizeof(text), source);
        fclose(source);
        if (!script_route_read_pacing(&replay_route, text, (unsigned int)size)) {
            printf("route_pacing.txt is invalid\n");
        } else if (replay_route.pacing_is_stale) {
            printf("route_pacing.txt was baked for another route; using each step's pace\n");
        }
    }
    snprintf(path, sizeof(path), "%s/scenes.txt", folder);
    source = fopen(path, "r");
    if (source != NULL) {
        size = fread(text, 1, sizeof(text), source);
        fclose(source);
        if (!script_route_read_scenes(&replay_route, text, (unsigned int)size)) {
            printf("scenes.txt is invalid\n");
        }
    }
    machine.route = &replay_route;
}

#define MOTIONS_SUFFIX ".motions"

static struct script_tables tables_for_the_size_check;

static void read_table_for_the_size_check(const char *folder, const char *file_name, int (*reader)(struct script_tables *, const char *, unsigned int))
{
    char path[FILE_NAME_MAX];
    FILE *source;
    char *text;
    long size;

    snprintf(path, sizeof(path), "%s/%s", folder, file_name);
    source = fopen(path, "rb");
    if (source == NULL) {
        return;
    }
    fseek(source, 0, SEEK_END);
    size = ftell(source);
    fseek(source, 0, SEEK_SET);
    text = malloc((size_t)size + 1u);
    if (text != NULL && fread(text, 1, (size_t)size, source) == (size_t)size) {
        text[size] = '\0';
        reader(&tables_for_the_size_check, text, (unsigned int)size);
    }
    free(text);
    fclose(source);
}

static void read_motions_for_the_size_check(const char *folder)
{
    char path[FILE_NAME_MAX];
    DIR *listing;
    struct dirent *entry;

    listing = opendir(folder);
    if (listing == NULL) {
        return;
    }
    while ((entry = readdir(listing)) != NULL) {
        size_t length;
        FILE *source;
        char *text;
        long size;

        length = strlen(entry->d_name);
        if (length <= strlen(MOTIONS_SUFFIX) || strcmp(&entry->d_name[length - strlen(MOTIONS_SUFFIX)], MOTIONS_SUFFIX) != 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", folder, entry->d_name);
        source = fopen(path, "rb");
        if (source == NULL) {
            continue;
        }
        fseek(source, 0, SEEK_END);
        size = ftell(source);
        fseek(source, 0, SEEK_SET);
        text = malloc((size_t)size + 1u);
        if (text != NULL && fread(text, 1, (size_t)size, source) == (size_t)size) {
            script_tables_read_motions(&tables_for_the_size_check, entry->d_name, text, (unsigned int)size);
        }
        free(text);
        fclose(source);
    }
    closedir(listing);
}

static int every_table_row_fits(const char *folder)
{
    read_table_for_the_size_check(folder, "chain.txt", script_tables_read_chain);
    read_table_for_the_size_check(folder, "rooms.txt", script_tables_read_rooms);
    read_table_for_the_size_check(folder, "audio_names.txt", script_tables_read_audio);
    read_table_for_the_size_check(folder, "text_lines.txt", script_tables_read_text_lines);
    read_table_for_the_size_check(folder, "balloons.txt", script_tables_read_balloons);
    read_table_for_the_size_check(folder, "chain.preload", script_tables_read_preload);
    read_table_for_the_size_check(folder, "effects.txt", script_tables_read_effects);
    read_table_for_the_size_check(folder, "soul.txt", script_tables_read_soul);
    read_table_for_the_size_check(folder, "core_labels.txt", script_tables_read_core_labels);
    read_table_for_the_size_check(folder, "ui_clips.txt", script_tables_read_ui_clips);
    read_motions_for_the_size_check(folder);
    if (tables_for_the_size_check.rows_dropped > 0) {
        printf("table rows dropped %d\n", tables_for_the_size_check.rows_dropped);
        return 0;
    }
    return 1;
}

static void read_room_bounds(const char *folder)
{
    char path[FILE_NAME_MAX];
    char line[FILE_NAME_MAX];
    FILE *source;
    int room;
    int width;
    int height;

    snprintf(path, sizeof(path), "%s/rooms.txt", folder);
    source = fopen(path, "r");
    if (source == NULL) {
        return;
    }
    while (fgets(line, sizeof(line), source) != NULL) {
        if (sscanf(line, "room %d %d %d", &room, &width, &height) == 3 && room >= 0 && room < SCRIPT_ROOM_ID_MAX) {
            machine.room_bounds[room].width = width;
            machine.room_bounds[room].height = height;
        }
    }
    fclose(source);
}

static void read_talk_book(const char *folder)
{
    char path[FILE_NAME_MAX];
    struct script_talk_book *book;
    unsigned char *bytes;
    unsigned int size;

    snprintf(path, sizeof(path), "%s/talk.txt", folder);
    bytes = read_file(path, &size);
    if (bytes == NULL) {
        return;
    }
    book = malloc(sizeof(*book));
    if (book != NULL && script_talk_book_parse((const char *)bytes, size, book)) {
        machine.talk_book = book;
    } else {
        printf("talk book is invalid\n");
    }
}

int main(int argument_count, char **arguments)
{
    const char *folder;
    unsigned int stop_tick;
    enum script_timing timing;
    unsigned int press_ticks[16];
    unsigned int talk_close_ticks[16];
    int press_count;
    int talk_close_count;
    int index;
    int press;
    int auto_press;
    int stall_breaker_is_off = 0;
    const char *state_load_path;
    const char *start_override;
    int chapter_mark_override;
    unsigned int flag_changes[SCRIPT_FLAG_CHANGE_MAX];
    int flag_values[SCRIPT_FLAG_CHANGE_MAX];
    int flag_change_count;

    state_load_path = NULL;
    start_override = NULL;
    chapter_mark_override = -1;
    flag_change_count = 0;
    folder = "extracted/vita_assets/chapter1_opening/scripts";
    stop_tick = 100000;
    timing = SCRIPT_TIMING_DS;
    press_count = 0;
    talk_close_count = 0;
    auto_press = 0;
    for (index = 1; index < argument_count; index++) {
        if (strcmp(arguments[index], "--scripts") == 0) {
            folder = arguments[++index];
        } else if (strcmp(arguments[index], "--timing") == 0) {
            index = index + 1;
            timing = strcmp(arguments[index], "ds") == 0 ? SCRIPT_TIMING_DS : SCRIPT_TIMING_REMASTER;
        } else if (strcmp(arguments[index], "--trace-core") == 0) {
            traced_core_name = arguments[++index];
        } else if (strcmp(arguments[index], "--watch") == 0) {
            watched_actor_names = arguments[++index];
        } else if (strcmp(arguments[index], "--stop-tick") == 0) {
            stop_tick = (unsigned int)atoi(arguments[++index]);
        } else if (strcmp(arguments[index], "--press-at") == 0) {
            press_ticks[press_count] = (unsigned int)atoi(arguments[++index]);
            press_count = press_count + 1;
        } else if (strcmp(arguments[index], "--close-talk-at") == 0) {
            talk_close_ticks[talk_close_count] = (unsigned int)atoi(arguments[++index]);
            talk_close_count = talk_close_count + 1;
        } else if (strcmp(arguments[index], "--auto-press") == 0) {
            auto_press = SCRIPT_AUTO_FAST;
        } else if (strcmp(arguments[index], "--auto-paced") == 0) {
            auto_press = SCRIPT_AUTO_PACED;
        } else if (strcmp(arguments[index], "--no-stall-breaker") == 0) {
            stall_breaker_is_off = 1;
        } else if (strcmp(arguments[index], "--marks") == 0) {
            marks_to_stdout = 1;
        } else if (strcmp(arguments[index], "--rows") == 0) {
            rows_output = fopen(arguments[++index], "w");
        } else if (strcmp(arguments[index], "--cues") == 0) {
            cues_output = fopen(arguments[++index], "w");
        } else if (strcmp(arguments[index], "--save-state-at") == 0 && index + 2 < argument_count) {
            if (state_save_count < STATE_SAVE_MAX) {
                state_save_scripts[state_save_count] = arguments[index + 1];
                state_save_paths[state_save_count] = arguments[index + 2];
                state_save_count = state_save_count + 1;
            }
            index = index + 2;
        } else if (strcmp(arguments[index], "--load-state") == 0) {
            state_load_path = arguments[++index];
        } else if (strcmp(arguments[index], "--start") == 0) {
            start_override = arguments[++index];
        } else if (strcmp(arguments[index], "--chapter-mark") == 0) {
            chapter_mark_override = atoi(arguments[++index]);
        } else if ((strcmp(arguments[index], "--set-flag") == 0 || strcmp(arguments[index], "--clear-flag") == 0) && flag_change_count < SCRIPT_FLAG_CHANGE_MAX) {
            flag_values[flag_change_count] = strcmp(arguments[index], "--set-flag") == 0;
            flag_changes[flag_change_count] = (unsigned int)strtoul(arguments[++index], NULL, 16);
            flag_change_count = flag_change_count + 1;
        } else {
            printf("unknown argument %s\n", arguments[index]);
            return 2;
        }
    }
    if (!every_table_row_fits(folder)) {
        return 1;
    }
    script_machine_init(&machine, &recording_outputs, timing, NULL);
    script_machine_start_from_black(&machine);
    machine.auto_press = SCRIPT_AUTO_OFF;
    machine.route_is_driving = auto_press != SCRIPT_AUTO_OFF;
    machine.stall_breaker_is_off = stall_breaker_is_off;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    add_every_script_of_the_chain(folder);
    read_room_bounds(folder);
    read_talk_book(folder);
    if (auto_press != SCRIPT_AUTO_OFF) {
        read_route(folder);
    }
    if (state_load_path != NULL && !load_state(state_load_path)) {
        printf("cannot read state %s\n", state_load_path);
        return 1;
    }
    if (chapter_mark_override >= 0) {
        machine.chapter_mark = chapter_mark_override;
    }
    for (index = 0; index < flag_change_count; index++) {
        script_flags_write_known(&machine.flags, flag_changes[index], flag_values[index]);
    }
    if (start_override != NULL) {
        snprintf(chain_entry, sizeof(chain_entry), "%s", start_override);
    }
    if (!script_machine_start(&machine, chain_entry)) {
        printf("cannot start %s\n", chain_entry);
        return 1;
    }
    if (soul_state_was_loaded) {
        game_save_apply_after_start(&loaded_save, &machine);
    }
    for (index = 0; index < start_count; index++) {
        script_soul_run_command(&machine, start_verbs[index], start_args[index]);
    }
    while (machine.tick <= stop_tick && !machine.is_finished) {
        for (press = 0; press < press_count; press++) {
            if (press_ticks[press] == machine.tick) {
                script_machine_note_press(&machine);
            }
        }
        for (press = 0; press < talk_close_count; press++) {
            if (talk_close_ticks[press] == machine.tick) {
                script_talk_close_now(&machine);
            }
        }
        if (machine.route_is_driving) {
            struct input_state frame;

            script_route_pad_frame(&route_pad, &machine, &frame);
            script_input_frame(&route_input, &machine, &frame);
        }
        script_machine_tick(&machine);
        note_traced_core();
        note_watched_actors();
    }
    if (marks_to_stdout) {
        print_cores();
    }
    printf("ended tick=%u finished=%d fetches=%u pushes=%u\n", machine.tick, machine.is_finished, machine.fetch_total, machine.push_count);
    printf("state talk=%d open=%d pause=%d mode=%d tutorial=%d soul_keeps=%d objects=%d quiet=%u\n", (int)machine.talk.phase, machine.talk_is_open, machine.pause_bit, machine.mode_byte, machine.soul.tutorial_mode, machine.soul_state_keeps_pause, machine.object_count, machine.tick - machine.last_new_site_tick);
    printf("soul view=%d pills=%u rewind_select=%u rewind_press=%u movie_left=%d time_jump_at=%lld time_jump=%s input_off=%d dead_talk=%d choice=%d narration=%d\n", machine.soul.is_on, machine.soul.pills_registered, (unsigned int)machine.soul.rewind_select_ticks, (unsigned int)machine.soul.rewind_press_ticks, (int)machine.movie_ticks_left, machine.soul.time_jump_event.target != NULL ? machine.soul.time_jump_event.at : -1LL, machine.soul.time_jump_event.target != NULL ? machine.soul.time_jump_event.target : "-", machine.soul.input_is_off, machine.soul.dead_talk_is_on, machine.choice.is_open, machine.narration.is_running);
    printf("soul_held view_change=%d view_demo=%d demo_soul=%d film=%d pill_out=%d tutorial=%d chain=%d pick=%d drag=%d\n", (int)machine.soul.view_change_ticks_left, machine.soul.view_demo_is_on, machine.demo_soul_is_on, machine.film_frame_is_on, machine.soul.ghost_pill_is_held_out, machine.soul.tutorial_holds_input, (int)machine.soul.chain.state, machine.soul.chain_pick_object, (int)machine.soul.drag_ticks_left);
    for (index = 0; index < machine.object_count; index++) {
        int handler;
        for (handler = 0; handler < machine.objects[index].handler_count; handler++) {
            if (machine.objects[index].handlers[handler].is_busy) {
                printf("busy %s %s|%s wait=%u pc=%x\n", machine.objects[index].name, machine.objects[index].handlers[handler].verb, machine.objects[index].handlers[handler].arg, machine.objects[index].handlers[handler].vm.wait_ticks, machine.objects[index].handlers[handler].vm.pc * 2 + 0x34);
            }
        }
    }
    if (rows_output != NULL) {
        fclose(rows_output);
    }
    if (cues_output != NULL) {
        fclose(cues_output);
    }
    return 0;
}
