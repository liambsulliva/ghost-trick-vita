#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "loop_file.h"
#include "memory.h"

#define LOOP_TEXT_MAX 64

int loop_file_read(const char *folder, const char *name, unsigned int frame_count, unsigned int *loop_start, unsigned int *loop_last)
{
    char path[FILE_PATH_MAX];
    char text[LOOP_TEXT_MAX];
    unsigned char *data;
    unsigned int size;
    unsigned int start;
    unsigned int last;

    if (!file_path_join(path, folder, name, ".loop") || file_read_whole(path, &data, &size) != FILE_OK) {
        return LOOP_FILE_NONE;
    }
    if (size >= sizeof(text)) {
        size = sizeof(text) - 1;
    }
    memcpy(text, data, size);
    text[size] = '\0';
    mem_free(data);
    if (sscanf(text, "%u %u", &start, &last) != 2 || start > last || last >= frame_count) {
        return LOOP_FILE_BAD;
    }
    *loop_start = start;
    *loop_last = last;
    return LOOP_FILE_OK;
}
