#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_machine.h"
#include "script_tables.h"

#define FILE_NAME_MAX 512
#define TEXT_BYTES_MAX (1024 * 1024)

static FILE *rows_output;
static FILE *cues_output;
static struct script_machine machine;
static struct script_cue_list cue_lists[SCRIPT_SOURCE_MAX];
static int cue_list_count;
static int marks_to_stdout;

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
static void on_iris_created(const char *name, float x, float y, float radius) { CUE("iris_create %s %.1f %.1f %.1f", name, x, y, radius); }
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
static void on_marked(const char *what, const char *name) { CUE("mark %s %s", what, name); if (marks_to_stdout) { printf("mark %s %s\n", what, name); } }
static void on_talk(const struct script_talk_view *view) { CUE("talk slide=%.2f alpha=%.2f glyphs=%d cursor=%d open=%d", view->slide, view->text_alpha, view->glyphs_shown, view->cursor_is_on, view->page != NULL); }
static void on_effect_deleted(const char *name) { CUE("effect_deleted %s", name); }

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
    .marked = on_marked,
    .talk_changed = on_talk
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

static void add_every_script_of_the_chain(const char *folder)
{
    char path[FILE_NAME_MAX];
    char name[SCRIPT_SOURCE_NAME_MAX];
    char line[FILE_NAME_MAX];
    FILE *source;

    snprintf(path, sizeof(path), "%s/chain.txt", folder);
    source = fopen(path, "r");
    if (source == NULL) {
        printf("cannot read %s\n", path);
        exit(1);
    }
    while (fgets(line, sizeof(line), source) != NULL) {
        if (sscanf(line, "file %95s", name) == 1) {
            add_script(folder, name);
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
        } else if (strcmp(arguments[index], "--marks") == 0) {
            marks_to_stdout = 1;
        } else if (strcmp(arguments[index], "--rows") == 0) {
            rows_output = fopen(arguments[++index], "w");
        } else if (strcmp(arguments[index], "--cues") == 0) {
            cues_output = fopen(arguments[++index], "w");
        } else {
            printf("unknown argument %s\n", arguments[index]);
            return 2;
        }
    }
    if (!every_table_row_fits(folder)) {
        return 1;
    }
    script_machine_init(&machine, &recording_outputs, timing, NULL);
    machine.auto_press = auto_press;
    add_every_script_of_the_chain(folder);
    read_room_bounds(folder);
    read_talk_book(folder);
    if (auto_press != SCRIPT_AUTO_OFF) {
        read_route(folder);
    }
    if (!script_machine_start(&machine, "route")) {
        printf("cannot start route\n");
        return 1;
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
        script_machine_tick(&machine);
    }
    printf("ended tick=%u finished=%d fetches=%u pushes=%u\n", machine.tick, machine.is_finished, machine.fetch_total, machine.push_count);
    printf("state talk=%d open=%d pause=%d mode=%d tutorial=%d soul_keeps=%d objects=%d quiet=%u\n", (int)machine.talk.phase, machine.talk_is_open, machine.pause_bit, machine.mode_byte, machine.soul.tutorial_mode, machine.soul_state_keeps_pause, machine.object_count, machine.tick - machine.last_new_site_tick);
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
