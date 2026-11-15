#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "clock.h"
#include "file.h"
#include "fakes/fake_platform.h"
#include "host_platform.h"
#include "log_sink.h"
#include "memory.h"

#define DEVICE_PREFIX "ux0:"
#define HOST_PATH_MAX 1024

static unsigned long long now_us;
static char data_folder[HOST_PATH_MAX];
static char log_path[HOST_PATH_MAX];

void host_clock_advance_us(unsigned long long elapsed_us)
{
    now_us = now_us + elapsed_us;
}

void host_files_keep_device_files_in(const char *folder)
{
    snprintf(data_folder, sizeof(data_folder), "%s", folder);
}

unsigned long long clock_now_us(void)
{
    return now_us;
}

static void host_path_of(const char *path, char *host_path)
{
    char *colon;

    if (strncmp(path, DEVICE_PREFIX, strlen(DEVICE_PREFIX)) != 0) {
        snprintf(host_path, HOST_PATH_MAX, "%s", path);
        return;
    }
    snprintf(host_path, HOST_PATH_MAX, "%s/%s", data_folder, path);
    colon = strchr(host_path + strlen(data_folder), ':');
    if (colon != NULL) {
        *colon = '_';
    }
}

static void make_folders_above(const char *host_path)
{
    char folder[HOST_PATH_MAX];
    char *slash;

    snprintf(folder, sizeof(folder), "%s", host_path);
    for (slash = strchr(folder + 1, '/'); slash != NULL; slash = strchr(slash + 1, '/')) {
        *slash = '\0';
        mkdir(folder, 0755);
        *slash = '/';
    }
}

void log_sink_start(const char *path)
{
    host_path_of(path, log_path);
}

void log_sink_write(const char *text, unsigned int size)
{
    FILE *log;

    make_folders_above(log_path);
    log = fopen(log_path, "ab");
    if (log != NULL) {
        fwrite(text, 1, size, log);
        fclose(log);
    }
}

void log_sink_flush(void)
{
}

void *mem_alloc(unsigned int size)
{
    return size == 0 ? NULL : malloc(size);
}

void mem_free(void *block)
{
    free(block);
}

static unsigned char *make_with_malloc(void *context, unsigned int size)
{
    (void)context;
    return mem_alloc(size);
}

int file_read_whole_into(const char *path, const struct file_buffers *buffers, unsigned char **data, unsigned int *size, unsigned long long *open_us)
{
    char host_path[HOST_PATH_MAX];
    unsigned char *buffer;
    FILE *source;
    long length;

    *open_us = 0;
    host_path_of(path, host_path);
    source = fopen(host_path, "rb");
    if (source == NULL) {
        if (getenv("HOST_FILES_TRACE") != NULL) {
            fprintf(stderr, "no file %s\n", host_path);
        }
        return FILE_ERR_OPEN;
    }
    fseek(source, 0, SEEK_END);
    length = ftell(source);
    fseek(source, 0, SEEK_SET);
    if (length <= 0) {
        fclose(source);
        return FILE_ERR_SIZE;
    }
    buffer = buffers->make(buffers->context, (unsigned int)length);
    if (buffer == NULL) {
        fclose(source);
        return FILE_ERR_MEMORY;
    }
    if (fread(buffer, 1, (size_t)length, source) != (size_t)length) {
        fclose(source);
        if (buffers->give_back != NULL) {
            buffers->give_back(buffers->context, buffer);
        }
        return FILE_ERR_READ;
    }
    fclose(source);
    *data = buffer;
    *size = (unsigned int)length;
    return FILE_OK;
}

int file_read_whole(const char *path, unsigned char **data, unsigned int *size)
{
    struct file_buffers buffers;
    unsigned long long open_us;

    buffers.make = make_with_malloc;
    buffers.give_back = NULL;
    buffers.context = NULL;
    return file_read_whole_into(path, &buffers, data, size, &open_us);
}

static int write_with_mode(const char *path, const unsigned char *data, unsigned int size, const char *mode)
{
    char host_path[HOST_PATH_MAX];
    FILE *target;

    host_path_of(path, host_path);
    make_folders_above(host_path);
    target = fopen(host_path, mode);
    if (target == NULL) {
        return FILE_ERR_OPEN;
    }
    if (fwrite(data, 1, size, target) != size) {
        fclose(target);
        return FILE_ERR_WRITE;
    }
    fclose(target);
    return FILE_OK;
}

int file_write_whole(const char *path, const unsigned char *data, unsigned int size)
{
    return write_with_mode(path, data, size, "wb");
}

int file_append(const char *path, const unsigned char *data, unsigned int size)
{
    return write_with_mode(path, data, size, "ab");
}

int file_delete(const char *path)
{
    char host_path[HOST_PATH_MAX];

    host_path_of(path, host_path);
    return remove(host_path) == 0 ? FILE_OK : FILE_ERR_OPEN;
}

int file_make_folder(const char *path)
{
    char host_path[HOST_PATH_MAX];
    char inside[HOST_PATH_MAX + 2];

    host_path_of(path, host_path);
    snprintf(inside, sizeof(inside), "%s/.", host_path);
    make_folders_above(inside);
    return FILE_OK;
}

int file_list_folder(const char *path, file_name_visitor visit, void *context)
{
    char host_path[HOST_PATH_MAX];
    struct dirent *entry;
    DIR *folder;

    host_path_of(path, host_path);
    folder = opendir(host_path);
    if (folder == NULL) {
        return FILE_ERR_OPEN;
    }
    for (entry = readdir(folder); entry != NULL; entry = readdir(folder)) {
        if (entry->d_name[0] != '.') {
            visit(context, entry->d_name);
        }
    }
    closedir(folder);
    return FILE_OK;
}

void fake_clock_advance_us(unsigned long long elapsed_us)
{
    (void)elapsed_us;
}

unsigned long long fake_clock_peek_us(void)
{
    return now_us;
}
