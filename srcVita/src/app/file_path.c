#include <string.h>
#include "file.h"
#include "file_path.h"

int file_path_join(char *path, const char *folder, const char *name, const char *extension)
{
    unsigned int needed_size;

    needed_size = (unsigned int)(strlen(folder) + 1 + strlen(name) + strlen(extension) + 1);
    if (needed_size > FILE_PATH_MAX) {
        path[0] = '\0';
        return 0;
    }
    strcpy(path, folder);
    strcat(path, "/");
    strcat(path, name);
    strcat(path, extension);
    return 1;
}
