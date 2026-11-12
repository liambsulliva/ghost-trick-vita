#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "game_save.h"
#include "memory.h"
#include "run_log.h"
#include "text_lines.h"

#define LINE_MAX (4 * SCRIPT_FLAG_BYTES)
#define NOTE_BYTES (SCRIPT_NOTE_LIST_COUNT * SCRIPT_NOTE_ID_COUNT)

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

int game_save_store_as_newest(struct game_save *save, enum game_save_slot slot)
{
    static struct game_save newest;

    save->serial = game_save_load_newest(&newest) ? newest.serial + 1u : 1u;
    return game_save_store(save, slot);
}

const char *game_save_path(enum game_save_slot slot)
{
    return slot == GAME_SAVE_AUTO ? GAME_SAVE_AUTO_PATH : GAME_SAVE_MANUAL_PATH;
}

int game_save_store(const struct game_save *save, enum game_save_slot slot)
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

int game_save_load(struct game_save *save, enum game_save_slot slot)
{
    unsigned char *bytes;
    unsigned int size;
    int result;

    memset(save, 0, sizeof(*save));
    if (file_read_whole(game_save_path(slot), &bytes, &size) != FILE_OK) {
        return 0;
    }
    result = game_save_read_text(save, (const char *)bytes, size) && save->script[0] != '\0';
    mem_free(bytes);
    return result;
}

int game_save_load_newest(struct game_save *save)
{
    static struct game_save other;
    int has_auto;
    int has_manual;

    has_auto = game_save_load(save, GAME_SAVE_AUTO);
    has_manual = game_save_load(&other, GAME_SAVE_MANUAL);
    if (has_manual && (!has_auto || other.serial > save->serial)) {
        *save = other;
        return 1;
    }
    if (!has_auto) {
        memset(save, 0, sizeof(*save));
    }
    return has_auto;
}

int game_save_erase_all(void)
{
    int erased;

    erased = file_delete(GAME_SAVE_AUTO_PATH) == FILE_OK;
    erased = (file_delete(GAME_SAVE_MANUAL_PATH) == FILE_OK) || erased;
    run_log_write("save erased=%d", erased);
    return erased;
}
