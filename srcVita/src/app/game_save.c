#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "game_save.h"
#include "memory.h"
#include "record_reveal.h"
#include "run_log.h"
#include "save_icon.h"
#include "text_lines.h"
#include "work_queue.h"

#define LINE_MAX (4 * SCRIPT_FLAG_BYTES)
#define NOTE_BYTES (SCRIPT_NOTE_LIST_COUNT * SCRIPT_NOTE_ID_COUNT)
#define SAVE_JOB_MAX 4

enum save_job_kind {
    SAVE_JOB_NEWEST,
    SAVE_JOB_SLOT,
    SAVE_JOB_CHAPTER
};

struct save_job {
    volatile int is_busy;
    enum save_job_kind kind;
    int place;
    struct game_save save;
};

static struct save_job jobs[SAVE_JOB_MAX];
static int next_job;

void game_save_capture(struct game_save *save, const struct script_machine *machine, const char *script)
{
    memset(save, 0, sizeof(*save));
    snprintf(save->script, sizeof(save->script), "%s", script != NULL ? script : "");
    save->chapter_mark = machine->chapter_mark;
    save->phone_book_place = machine->phone_book_place;
    save->fate_level = machine->fate_level;
    save->pills_registered = machine->soul.pills_registered;
    save->trick_is_on = machine->soul.trick_is_on;
    save->rollback_is_on = machine->soul.rollback_is_on;
    save->has_soul_state = 1;
    memcpy(save->game_flags, machine->flags.game, sizeof(save->game_flags));
    memcpy(save->chapter_flags, machine->flags.chapter, sizeof(save->chapter_flags));
    memcpy(save->game_bank_saved, machine->flags.game_bank_saved, sizeof(save->game_bank_saved));
    memcpy(save->note_levels, machine->note_levels, sizeof(save->note_levels));
}

static unsigned int add_text(char *text, unsigned int text_size, unsigned int used, const char *format, int number)
{
    int written;

    if (used >= text_size) {
        return used;
    }
    written = snprintf(text + used, text_size - used, format, number);
    return written < 0 ? text_size : used + (unsigned int)written;
}

static unsigned int add_bytes(char *text, unsigned int text_size, unsigned int used, const char *name, const unsigned char *bytes, unsigned int count)
{
    static const char digits[] = "0123456789abcdef";
    unsigned int index;
    unsigned int name_length;

    name_length = (unsigned int)strlen(name);
    if (used + name_length + 2u * count + 2u >= text_size) {
        return text_size;
    }
    memcpy(text + used, name, name_length);
    used = used + name_length;
    text[used++] = ' ';
    for (index = 0; index < count; index++) {
        text[used++] = digits[bytes[index] >> 4];
        text[used++] = digits[bytes[index] & 15u];
    }
    text[used++] = '\n';
    text[used] = '\0';
    return used;
}

unsigned int game_save_write_text(const struct game_save *save, char *text, unsigned int text_size)
{
    unsigned int used;
    int written;

    if (text_size == 0) {
        return 0;
    }
    written = snprintf(text, text_size, "script %s\n", save->script);
    used = written < 0 ? text_size : (unsigned int)written;
    used = add_text(text, text_size, used, "serial %d\n", (int)save->serial);
    used = add_text(text, text_size, used, "chapter_mark %d\n", save->chapter_mark);
    used = add_text(text, text_size, used, "phone_book_place %d\n", save->phone_book_place);
    used = add_text(text, text_size, used, "fate_level %d\n", save->fate_level);
    if (save->game_cleared) {
        used = add_text(text, text_size, used, "game_cleared %d\n", 1);
    }
    if (save->has_soul_state) {
        used = add_text(text, text_size, used, "pills_registered %d\n", (int)save->pills_registered);
        used = add_text(text, text_size, used, "trick_is_on %d\n", save->trick_is_on);
        used = add_text(text, text_size, used, "rollback_is_on %d\n", save->rollback_is_on);
    }
    used = add_bytes(text, text_size, used, "game_flags", save->game_flags, sizeof(save->game_flags));
    used = add_bytes(text, text_size, used, "chapter_flags", save->chapter_flags, sizeof(save->chapter_flags));
    used = add_bytes(text, text_size, used, "game_bank_saved", save->game_bank_saved, sizeof(save->game_bank_saved));
    used = add_bytes(text, text_size, used, "note_levels", &save->note_levels[0][0], NOTE_BYTES);
    return used < text_size ? used : 0;
}

static int read_bytes(const char *hex, unsigned char *bytes, unsigned int count)
{
    unsigned int index;
    unsigned int value;

    for (index = 0; index < count; index++) {
        if (sscanf(hex + 2 * index, "%2x", &value) != 1) {
            return 0;
        }
        bytes[index] = (unsigned char)value;
    }
    return 1;
}

int game_save_read_text(struct game_save *save, const char *text, unsigned int text_size)
{
    static char line[LINE_MAX];
    unsigned int offset;
    char *value;
    int is_whole;
    int has_flags;

    memset(save, 0, sizeof(*save));
    is_whole = 1;
    has_flags = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        value = strchr(line, ' ');
        if (value == NULL) {
            continue;
        }
        *value = '\0';
        value = value + 1;
        if (strcmp(line, "script") == 0) {
            snprintf(save->script, sizeof(save->script), "%s", value);
        } else if (strcmp(line, "serial") == 0) {
            save->serial = (unsigned int)strtoul(value, NULL, 10);
        } else if (strcmp(line, "chapter_mark") == 0) {
            save->chapter_mark = atoi(value);
        } else if (strcmp(line, "phone_book_place") == 0) {
            save->phone_book_place = atoi(value);
        } else if (strcmp(line, "fate_level") == 0) {
            save->fate_level = atoi(value);
        } else if (strcmp(line, "game_cleared") == 0) {
            save->game_cleared = atoi(value) != 0;
        } else if (strcmp(line, "pills_registered") == 0) {
            save->pills_registered = (unsigned int)strtoul(value, NULL, 10);
            save->has_soul_state = 1;
        } else if (strcmp(line, "trick_is_on") == 0) {
            save->trick_is_on = atoi(value);
        } else if (strcmp(line, "rollback_is_on") == 0) {
            save->rollback_is_on = atoi(value);
        } else if (strcmp(line, "game_flags") == 0) {
            is_whole = is_whole && read_bytes(value, save->game_flags, sizeof(save->game_flags));
            has_flags = 1;
        } else if (strcmp(line, "chapter_flags") == 0) {
            is_whole = is_whole && read_bytes(value, save->chapter_flags, sizeof(save->chapter_flags));
        } else if (strcmp(line, "game_bank_saved") == 0) {
            is_whole = is_whole && read_bytes(value, save->game_bank_saved, sizeof(save->game_bank_saved));
        } else if (strcmp(line, "note_levels") == 0) {
            is_whole = is_whole && read_bytes(value, &save->note_levels[0][0], NOTE_BYTES);
        }
    }
    return is_whole && has_flags;
}

void game_save_apply_before_start(const struct game_save *save, struct script_machine *machine)
{
    machine->chapter_mark = save->chapter_mark;
    machine->phone_book_place = save->phone_book_place;
    script_soul_set_fate_level(machine, save->fate_level);
    memcpy(machine->flags.game, save->game_flags, sizeof(save->game_flags));
    memcpy(machine->flags.chapter, save->chapter_flags, sizeof(save->chapter_flags));
    memcpy(machine->flags.game_bank_saved, save->game_bank_saved, sizeof(save->game_bank_saved));
    memcpy(machine->note_levels, save->note_levels, sizeof(save->note_levels));
}

void game_save_apply_after_start(const struct game_save *save, struct script_machine *machine)
{
    if (!save->has_soul_state) {
        return;
    }
    machine->soul.pills_registered = save->pills_registered;
    machine->soul.trick_is_on = save->trick_is_on;
    machine->soul.rollback_is_on = save->rollback_is_on;
}

static const char *game_save_path(enum game_save_slot slot)
{
    return slot == GAME_SAVE_AUTO ? GAME_SAVE_AUTO_PATH : GAME_SAVE_MANUAL_PATH;
}

static void chapter_path(char *path, unsigned int path_size, int chapter)
{
    snprintf(path, path_size, "%s/chapter_%02d.txt", FILE_DATA_FOLDER, chapter);
}

static int load_from(struct game_save *save, const char *path)
{
    unsigned char *bytes;
    unsigned int size;
    int result;

    memset(save, 0, sizeof(*save));
    if (file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    result = game_save_read_text(save, (const char *)bytes, size) && save->script[0] != '\0';
    mem_free(bytes);
    return result;
}

static int store_now(const struct game_save *save, enum game_save_slot slot)
{
    static char text[GAME_SAVE_TEXT_MAX];
    unsigned int size;
    int result;

    size = game_save_write_text(save, text, sizeof(text));
    (void)file_make_folder(FILE_DATA_FOLDER);
    result = size > 0 && file_write_whole(game_save_path(slot), (const unsigned char *)text, size) == FILE_OK;
    run_log_write("save stored slot=%d script=%s serial=%u ok=%d", (int)slot, save->script, save->serial, result);
    return result;
}

static int load_newest_now(struct game_save *save)
{
    static struct game_save other;
    int has_auto;
    int has_manual;

    has_auto = load_from(save, game_save_path(GAME_SAVE_AUTO));
    has_manual = load_from(&other, game_save_path(GAME_SAVE_MANUAL));
    if (has_manual && (!has_auto || other.serial > save->serial)) {
        *save = other;
        return 1;
    }
    if (!has_auto) {
        memset(save, 0, sizeof(*save));
    }
    return has_auto;
}

static int store_as_newest_now(struct game_save *save, enum game_save_slot slot)
{
    static struct game_save newest;

    if (load_newest_now(&newest)) {
        save->serial = newest.serial + 1u;
        save->game_cleared = save->game_cleared || newest.game_cleared;
    } else {
        save->serial = 1u;
    }
    return store_now(save, slot);
}

static int store_chapter_now(const struct game_save *save, int chapter)
{
    static char text[GAME_SAVE_TEXT_MAX];
    char path[64];
    unsigned int size;
    int result;

    chapter_path(path, sizeof(path), chapter);
    size = game_save_write_text(save, text, sizeof(text));
    (void)file_make_folder(FILE_DATA_FOLDER);
    result = size > 0 && file_write_whole(path, (const unsigned char *)text, size) == FILE_OK;
    run_log_write("save chapter=%d script=%s ok=%d", chapter, save->script, result);
    return result;
}

static void run_job(void *argument)
{
    struct save_job *job;

    job = argument;
    if (job->kind == SAVE_JOB_NEWEST) {
        (void)store_as_newest_now(&job->save, (enum game_save_slot)job->place);
    } else if (job->kind == SAVE_JOB_SLOT) {
        (void)store_now(&job->save, (enum game_save_slot)job->place);
    } else {
        (void)store_chapter_now(&job->save, job->place);
    }
    __sync_synchronize();
    job->is_busy = 0;
}

static int queue_job(enum save_job_kind kind, const struct game_save *save, int place)
{
    struct save_job *job;

    job = &jobs[next_job];
    if (job->is_busy) {
        work_queue_wait();
    }
    next_job = (next_job + 1) % SAVE_JOB_MAX;
    job->kind = kind;
    job->place = place;
    job->save = *save;
    job->is_busy = 1;
    save_icon_show();
    if (!work_queue_post(run_job, job)) {
        work_queue_wait();
        run_job(job);
    }
    return 1;
}

int game_save_store_as_newest(const struct game_save *save, enum game_save_slot slot)
{
    return queue_job(SAVE_JOB_NEWEST, save, (int)slot);
}

static int game_save_store(const struct game_save *save, enum game_save_slot slot)
{
    return queue_job(SAVE_JOB_SLOT, save, (int)slot);
}

int game_save_store_chapter(const struct game_save *save, int chapter)
{
    if (chapter < 1 || chapter > GAME_SAVE_CHAPTER_COUNT) {
        return 0;
    }
    return queue_job(SAVE_JOB_CHAPTER, save, chapter);
}

int game_save_is_writing(void)
{
    int index;

    for (index = 0; index < SAVE_JOB_MAX; index++) {
        if (jobs[index].is_busy) {
            return 1;
        }
    }
    return 0;
}

void game_save_wait(void)
{
    work_queue_wait();
}

int game_save_load(struct game_save *save, enum game_save_slot slot)
{
    work_queue_wait();
    return load_from(save, game_save_path(slot));
}

int game_save_load_chapter(struct game_save *save, int chapter)
{
    char path[64];

    if (chapter < 1 || chapter > GAME_SAVE_CHAPTER_COUNT) {
        memset(save, 0, sizeof(*save));
        return 0;
    }
    chapter_path(path, sizeof(path), chapter);
    work_queue_wait();
    return load_from(save, path);
}

int game_save_for_chapter_select(struct game_save *save, int chapter)
{
    memset(save, 0, sizeof(*save));
    if (chapter < 1 || chapter > GAME_SAVE_CHAPTER_COUNT) {
        return 0;
    }
    snprintf(save->script, sizeof(save->script), "%s", GAME_SAVE_CHAPTER_SELECT_SCRIPT);
    save->chapter_mark = chapter - 1;
    return 1;
}

int game_save_load_newest(struct game_save *save)
{
    work_queue_wait();
    return load_newest_now(save);
}

int game_save_mark_cleared(void)
{
    static struct game_save save;
    int slot;
    int marked;

    marked = 0;
    for (slot = 0; slot < GAME_SAVE_SLOT_COUNT; slot++) {
        if (game_save_load(&save, (enum game_save_slot)slot) && !save.game_cleared) {
            save.game_cleared = 1;
            marked = game_save_store(&save, (enum game_save_slot)slot) || marked;
        }
    }
    run_log_write("save game_cleared marked=%d", marked);
    return marked;
}

int game_save_erase_all(void)
{
    char path[64];
    int chapter;
    int erased;

    work_queue_wait();
    erased = file_delete(GAME_SAVE_AUTO_PATH) == FILE_OK;
    erased = (file_delete(GAME_SAVE_MANUAL_PATH) == FILE_OK) || erased;
    for (chapter = 1; chapter <= GAME_SAVE_CHAPTER_COUNT; chapter++) {
        chapter_path(path, sizeof(path), chapter);
        erased = (file_delete(path) == FILE_OK) || erased;
    }
    record_reveal_forget(RECORD_REVEAL_PATH);
    run_log_write("save erased=%d", erased);
    return erased;
}
