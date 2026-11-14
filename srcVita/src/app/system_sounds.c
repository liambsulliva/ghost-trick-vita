#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "sound_engine.h"
#include "system_sounds.h"

#define AUDIO_NAMES_FILE "audio_names"
#define LINE_MAX 128

struct system_sound_row {
    int kind;
    char name[SYSTEM_SOUND_NAME_MAX];
};

static struct system_sound_row rows[SYSTEM_SOUND_ROW_MAX];
static int row_count;

static void read_line(const char *line)
{
    struct system_sound_row *row;
    char name[LINE_MAX];
    int kind;

    if (row_count >= SYSTEM_SOUND_ROW_MAX || sscanf(line, "sound %d %127s", &kind, name) != 2 || strlen(name) >= SYSTEM_SOUND_NAME_MAX) {
        return;
    }
    row = &rows[row_count];
    row->kind = kind;
    snprintf(row->name, sizeof(row->name), "%s", name);
    row_count = row_count + 1;
}

int system_sounds_load(const char *scripts_folder)
{
    char path[FILE_PATH_MAX];
    char line[LINE_MAX];
    unsigned char *bytes;
    unsigned int size;
    unsigned int start;
    unsigned int end;
    unsigned int length;

    row_count = 0;
    if (!file_path_join(path, scripts_folder, AUDIO_NAMES_FILE, ".txt") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        run_log_write("system_sounds missing");
        return 0;
    }
    start = 0;
    while (start < size) {
        end = start;
        while (end < size && bytes[end] != '\n') {
            end = end + 1;
        }
        length = end - start < LINE_MAX - 1 ? end - start : LINE_MAX - 1;
        memcpy(line, bytes + start, length);
        line[length] = '\0';
        read_line(line);
        start = end + 1;
    }
    mem_free(bytes);
    return row_count;
}

const char *system_sounds_name(int kind)
{
    int index;

    for (index = 0; index < row_count; index++) {
        if (rows[index].kind == kind) {
            return rows[index].name;
        }
    }
    return NULL;
}

void system_sounds_play(int kind)
{
    const char *name;

    name = system_sounds_name(kind);
    if (name == NULL) {
        run_log_write("system_sounds no_row kind=%d", kind);
        return;
    }
    sound_request_play(name);
}
