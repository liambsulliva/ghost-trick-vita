#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_machine.h"

#define FILE_NAME_MAX 512
#define TEXT_BYTES_MAX (1024 * 1024)

static FILE *rows_output;
static FILE *cues_output;
static struct script_machine machine;
static struct script_cue_list cue_lists[SCRIPT_SOURCE_MAX];
static int cue_list_count;

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
static void on_actor(const struct script_actor_view *actor) { CUE("actor %s table=%x frame=%d into=%d x=%d y=%d level=%d visible=%d", actor->name, actor->table_offset, actor->frame_word_index, actor->ticks_into_frame, actor->x, actor->y, actor->level, actor->is_visible); }
static void on_brightness(int level) { CUE("brightness %d", level); }
static void on_mask_fade(int alive, int alpha) { CUE("mask_fade alive=%d alpha=%d", alive, alpha); }
static void on_beam_created(const char *name, float ax, float ay, float bx, float by, float width) { CUE("beam_create %s %.1f %.1f %.1f %.1f %.1f", name, ax, ay, bx, by, width); }
static void on_beam_width(const char *name, float width, int ticks) { CUE("beam_width %s %.1f ticks=%d", name, width, ticks); }
static void on_beam_base(const char *name, float x, float y, int ticks) { CUE("beam_base %s %.1f %.1f ticks=%d", name, x, y, ticks); }
static void on_beam_deleted(const char *name) { CUE("beam_delete %s", name); }
static void on_iris_created(const char *name, float x, float y, float radius) { CUE("iris_create %s %.1f %.1f %.1f", name, x, y, radius); }
static void on_iris_radius(const char *name, float radius, int ticks) { CUE("iris_radius %s %.1f ticks=%d", name, radius, ticks); }
static void on_iris_deleted(const char *name) { CUE("iris_delete %s", name); }
static void on_effect(int id, const char *name) { CUE("effect %d %s", id, name); }
static void on_narration(const char *label, enum script_narration_event event, int group) { CUE("narration %s event=%d group=%d", label, (int)event, group); }
static void on_balloon_opened(int slot, const char *label, int kind, float x, float y) { CUE("balloon_open slot=%d label=%s kind=%d anchor %.2f %.2f", slot, label != NULL ? label : "-", kind, x, y); }
static void on_balloon_closed(int slot) { CUE("balloon_close slot=%d", slot); }
static void on_music_prepared(int id) { CUE("music_prepare %d", id); }
static void on_music_started(int id) { CUE("music_start %d", id); }
static void on_music_stopped(int fade) { CUE("music_stop %d", fade); }
static void on_sound(int kind) { CUE("sound %d", kind); }
static void on_soul_vision(int is_on) { CUE("soul_vision %d", is_on); }
static void on_marked(const char *what, const char *name) { CUE("mark %s %s", what, name); }

static const struct script_outputs recording_outputs = {
    print_fetch, on_stage_cleared, on_room, on_camera, on_actor, on_brightness, on_mask_fade,
    on_beam_created, on_beam_width, on_beam_base, on_beam_deleted, on_iris_created, on_iris_radius, on_iris_deleted,
    on_effect, on_narration, on_balloon_opened, on_balloon_closed, on_music_prepared, on_music_started, on_music_stopped,
    on_sound, on_soul_vision, on_marked
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

int main(int argument_count, char **arguments)
{
    const char *folder;
    const char *stop_act;
    unsigned int stop_tick;
    enum script_timing timing;
    int index;

    folder = "extracted/vita_assets/chapter1_opening/scripts";
    stop_act = "gameAct020";
    stop_tick = 100000;
    timing = SCRIPT_TIMING_DS;
    for (index = 1; index < argument_count; index++) {
        if (strcmp(arguments[index], "--scripts") == 0) {
            folder = arguments[++index];
        } else if (strcmp(arguments[index], "--timing") == 0) {
            index = index + 1;
            timing = strcmp(arguments[index], "ds") == 0 ? SCRIPT_TIMING_DS : SCRIPT_TIMING_REMASTER;
        } else if (strcmp(arguments[index], "--stop-tick") == 0) {
            stop_tick = (unsigned int)atoi(arguments[++index]);
        } else if (strcmp(arguments[index], "--ignore-stop") == 0) {
            stop_act = NULL;
        } else if (strcmp(arguments[index], "--rows") == 0) {
            rows_output = fopen(arguments[++index], "w");
        } else if (strcmp(arguments[index], "--cues") == 0) {
            cues_output = fopen(arguments[++index], "w");
        } else {
            printf("unknown argument %s\n", arguments[index]);
            return 2;
        }
    }
    script_machine_init(&machine, &recording_outputs, timing, stop_act);
    add_script(folder, "route");
    add_script(folder, "st01-st01_root");
    add_script(folder, "st01-st01_game000_Expand");
    if (!script_machine_start(&machine, "route")) {
        printf("cannot start route\n");
        return 1;
    }
    while (machine.tick <= stop_tick && !machine.is_finished && !machine.halt.is_halted) {
        script_machine_tick(&machine);
    }
    printf("ended tick=%u finished=%d fetches=%u\n", machine.tick, machine.is_finished, machine.fetch_total);
    if (machine.halt.is_halted) {
        printf("halted reason=%s detail=%s op=0x%x offset=0x%x tick=%u file=%s object=%s\n", machine.halt.reason, machine.halt.detail, machine.halt.op, machine.halt.offset, machine.halt.tick, machine.halt.file_name, machine.halt.object_name);
    }
    if (rows_output != NULL) {
        fclose(rows_output);
    }
    if (cues_output != NULL) {
        fclose(cues_output);
    }
    return 0;
}
