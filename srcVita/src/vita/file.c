#include <stdio.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include "file.h"
#include "memory.h"

#define NEW_FOLDER_MODE 0777
#define NEW_FILE_MODE 0666

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

static int write_with_flags(const char *path, const unsigned char *data, unsigned int size, int open_flags)
{
    SceUID fd;
    int written_bytes;

    fd = sceIoOpen(path, open_flags, NEW_FILE_MODE);
    if (fd < 0) {
        return FILE_ERR_OPEN;
    }
    written_bytes = sceIoWrite(fd, data, size);
    sceIoClose(fd);
    if (written_bytes != (int)size) {
        return FILE_ERR_WRITE;
    }
    return FILE_OK;
}

int file_write_whole(const char *path, const unsigned char *data, unsigned int size)
{
    return write_with_flags(path, data, size, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC);
}

int file_append(const char *path, const unsigned char *data, unsigned int size)
{
    return write_with_flags(path, data, size, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND);
}

int file_delete(const char *path)
{
    if (sceIoRemove(path) < 0) {
        return FILE_ERR_WRITE;
    }
    return FILE_OK;
}

int file_make_folder(const char *path)
{
    sceIoMkdir(path, NEW_FOLDER_MODE);
    return FILE_OK;
}
