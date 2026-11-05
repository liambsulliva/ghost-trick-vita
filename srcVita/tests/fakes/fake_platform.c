#include <stdlib.h>
#include <string.h>
#include "fake_platform.h"
#include "file.h"
#include "memory.h"
#include "timing.h"

#define FAKE_FILE_COUNT 64

struct fake_file {
    char path[FILE_PATH_MAX];
    unsigned char *data;
    unsigned int size;
    int in_use;
};

static struct fake_file fake_files[FAKE_FILE_COUNT];
static unsigned long long fake_now_us;
static int writes_should_fail;

void fake_clock_set_us(unsigned long long now_us)
{
    fake_now_us = now_us;
}

void fake_clock_advance_us(unsigned long long elapsed_us)
{
    fake_now_us = fake_now_us + elapsed_us;
}

unsigned long long timing_now_us(void)
{
    return fake_now_us;
}

void *mem_alloc(unsigned int size)
{
    if (size == 0) {
        return NULL;
    }
    return malloc(size);
}

void mem_free(void *block)
{
    free(block);
}

static struct fake_file *find_file(const char *path)
{
    int index;

    for (index = 0; index < FAKE_FILE_COUNT; index++) {
        if (fake_files[index].in_use && strcmp(fake_files[index].path, path) == 0) {
            return &fake_files[index];
        }
    }
    return NULL;
}

static struct fake_file *find_free_file(void)
{
    int index;

    for (index = 0; index < FAKE_FILE_COUNT; index++) {
        if (!fake_files[index].in_use) {
            return &fake_files[index];
        }
    }
    return NULL;
}

void fake_files_reset(void)
{
    int index;

    for (index = 0; index < FAKE_FILE_COUNT; index++) {
        if (fake_files[index].in_use) {
            free(fake_files[index].data);
        }
        fake_files[index].in_use = 0;
        fake_files[index].data = NULL;
        fake_files[index].size = 0;
    }
    writes_should_fail = 0;
}

void fake_files_fail_writes(int should_fail)
{
    writes_should_fail = should_fail;
}

void fake_files_add(const char *path, const unsigned char *data, unsigned int size)
{
    file_write_whole(path, data, size);
}

const unsigned char *fake_files_contents(const char *path, unsigned int *size)
{
    struct fake_file *file;

    file = find_file(path);
    if (file == NULL) {
        *size = 0;
        return NULL;
    }
    *size = file->size;
    return file->data;
}

int fake_files_exists(const char *path)
{
    return find_file(path) != NULL;
}

int file_read_whole(const char *path, unsigned char **data, unsigned int *size)
{
    struct fake_file *file;
    unsigned char *copy;

    file = find_file(path);
    if (file == NULL) {
        return FILE_ERR_OPEN;
    }
    if (file->size == 0) {
        return FILE_ERR_SIZE;
    }
    copy = malloc(file->size);
    if (copy == NULL) {
        return FILE_ERR_MEMORY;
    }
    memcpy(copy, file->data, file->size);
    *data = copy;
    *size = file->size;
    return FILE_OK;
}

int file_read_app(const char *name, unsigned char **data, unsigned int *size)
{
    return file_read_whole(name, data, size);
}

int file_write_whole(const char *path, const unsigned char *data, unsigned int size)
{
    struct fake_file *file;

    if (writes_should_fail) {
        return FILE_ERR_WRITE;
    }
    file = find_file(path);
    if (file == NULL) {
        file = find_free_file();
        if (file == NULL) {
            return FILE_ERR_OPEN;
        }
        strncpy(file->path, path, FILE_PATH_MAX - 1);
        file->path[FILE_PATH_MAX - 1] = '\0';
        file->in_use = 1;
        file->data = NULL;
    }
    free(file->data);
    file->data = malloc(size + 1);
    memcpy(file->data, data, size);
    file->data[size] = 0;
    file->size = size;
    return FILE_OK;
}

int file_append(const char *path, const unsigned char *data, unsigned int size)
{
    struct fake_file *file;
    unsigned char *combined;

    if (writes_should_fail) {
        return FILE_ERR_WRITE;
    }
    file = find_file(path);
    if (file == NULL) {
        return file_write_whole(path, data, size);
    }
    combined = malloc(file->size + size + 1);
    memcpy(combined, file->data, file->size);
    memcpy(&combined[file->size], data, size);
    combined[file->size + size] = 0;
    free(file->data);
    file->data = combined;
    file->size = file->size + size;
    return FILE_OK;
}

int file_delete(const char *path)
{
    struct fake_file *file;

    file = find_file(path);
    if (file == NULL) {
        return FILE_ERR_OPEN;
    }
    free(file->data);
    file->data = NULL;
    file->size = 0;
    file->in_use = 0;
    return FILE_OK;
}

int file_make_folder(const char *path)
{
    (void)path;
    return FILE_OK;
}
