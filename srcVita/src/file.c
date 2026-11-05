#include <stdio.h>
#include <psp2/io/fcntl.h>
#include "file.h"
#include "memory.h"

int file_read_whole(const char *path, unsigned char **data, unsigned int *size)
{
    SceUID fd;
    SceOff length;
    int read_bytes;
    unsigned char *buffer;

    fd = sceIoOpen(path, SCE_O_RDONLY, 0);
    if (fd < 0) {
        return FILE_ERR_OPEN;
    }
    length = sceIoLseek(fd, 0, SCE_SEEK_END);
    if (length <= 0) {
        sceIoClose(fd);
        return FILE_ERR_SIZE;
    }
    if (sceIoLseek(fd, 0, SCE_SEEK_SET) < 0) {
        sceIoClose(fd);
        return FILE_ERR_READ;
    }
    buffer = mem_alloc((unsigned int)length);
    if (buffer == NULL) {
        sceIoClose(fd);
        return FILE_ERR_MEMORY;
    }
    read_bytes = sceIoRead(fd, buffer, (unsigned int)length);
    sceIoClose(fd);
    if (read_bytes != (int)length) {
        mem_free(buffer);
        return FILE_ERR_READ;
    }
    *data = buffer;
    *size = (unsigned int)length;
    return FILE_OK;
}

int file_read_app(const char *name, unsigned char **data, unsigned int *size)
{
    char path[FILE_PATH_MAX];
    int written;

    written = snprintf(path, sizeof(path), "%s%s", FILE_APP_PREFIX, name);
    if (written < 0 || written >= (int)sizeof(path)) {
        return FILE_ERR_PATH;
    }
    return file_read_whole(path, data, size);
}
