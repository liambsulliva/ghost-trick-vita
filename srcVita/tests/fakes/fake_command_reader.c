#include <stdio.h>
#include "command_reader.h"
#include "file.h"

static char commands_path[FILE_PATH_MAX];

int command_reader_start(const char *path)
{
    snprintf(commands_path, sizeof(commands_path), "%s", path);
    return 1;
}

int command_reader_take(unsigned char **text, unsigned int *size)
{
    if (file_read_whole(commands_path, text, size) != FILE_OK) {
        return 0;
    }
    file_delete(commands_path);
    return 1;
}
