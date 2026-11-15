#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "fake_platform.h"
#include "file.h"
#include "log_sink.h"
#include "memory.h"

#define FAKE_FILE_COUNT 4096

struct fake_file {
    char path[FILE_PATH_MAX];
    unsigned char *data;
    unsigned int size;
    int in_use;
};

static struct fake_file fake_files[FAKE_FILE_COUNT];
static unsigned long long fake_now_us;
static unsigned long long tick_per_read_us;
static unsigned int largest_block_bytes;
static int blocks_in_use;
static int writes_should_fail;
#define MISSED_PATH_MAX 256

static char missed_paths[MISSED_PATH_MAX][FILE_PATH_MAX];
static int read_misses;
static int folder_listing_fails;
static char watched_folder[FILE_PATH_MAX];
static int watched_reads;
static char fake_log_path[FILE_PATH_MAX];

void fake_clock_set_us(unsigned long long now_us)
{
    fake_now_us = now_us;
}

void fake_clock_advance_us(unsigned long long elapsed_us)
{
    fake_now_us = fake_now_us + elapsed_us;
}

void fake_clock_set_tick_per_read_us(unsigned long long tick_us)
{
    tick_per_read_us = tick_us;
}

unsigned long long fake_clock_peek_us(void)
{
    return fake_now_us;
}

unsigned long long clock_now_us(void)
{
    unsigned long long now_us;

    now_us = fake_now_us;
    fake_now_us = fake_now_us + tick_per_read_us;
    return now_us;
}

void log_sink_start(const char *path)
{
    strncpy(fake_log_path, path, FILE_PATH_MAX - 1);
    fake_log_path[FILE_PATH_MAX - 1] = '\0';
}

void log_sink_write(const char *text, unsigned int size)
{
    file_append(fake_log_path, (const unsigned char *)text, size);
}

void log_sink_flush(void)
{
}

void fake_memory_reset(void)
{
    largest_block_bytes = 0;
    blocks_in_use = 0;
}

void fake_memory_set_largest_block(unsigned int bytes)
{
    largest_block_bytes = bytes;
}

int fake_memory_blocks_in_use(void)
{
    return blocks_in_use;
}

void *mem_alloc(unsigned int size)
{
    void *block;

    if (size == 0 || (largest_block_bytes > 0 && size > largest_block_bytes)) {
        return NULL;
    }
    block = malloc(size);
    if (block != NULL) {
        blocks_in_use = blocks_in_use + 1;
    }
    return block;
}

void mem_free(void *block)
{
    if (block != NULL) {
        blocks_in_use = blocks_in_use - 1;
    }
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
    read_misses = 0;
    folder_listing_fails = 0;
    watched_folder[0] = '\0';
    watched_reads = 0;
}

static void note_read(const char *path)
{
    if (watched_folder[0] != '\0' && strncmp(path, watched_folder, strlen(watched_folder)) == 0) {
        watched_reads = watched_reads + 1;
    }
}

void fake_files_watch_reads_under(const char *folder)
{
    snprintf(watched_folder, sizeof(watched_folder), "%s", folder);
    watched_reads = 0;
}

int fake_files_watched_reads(void)
{
    return watched_reads;
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

int file_read_whole_into(const char *path, const struct file_buffers *buffers, unsigned char **data, unsigned int *size, unsigned long long *open_us)
{
    struct fake_file *file;
    unsigned char *buffer;

    *open_us = 0;
    note_read(path);
    file = find_file(path);
    if (file == NULL) {
        if (read_misses < MISSED_PATH_MAX) {
            snprintf(missed_paths[read_misses], FILE_PATH_MAX, "%s", path);
            read_misses = read_misses + 1;
        }
        return FILE_ERR_OPEN;
    }
    if (file->size == 0) {
        return FILE_ERR_SIZE;
    }
    buffer = buffers->make(buffers->context, file->size);
    if (buffer == NULL) {
        return FILE_ERR_MEMORY;
    }
    memcpy(buffer, file->data, file->size);
    *data = buffer;
    *size = file->size;
    return FILE_OK;
}

int file_read_whole(const char *path, unsigned char **data, unsigned int *size)
{
    struct fake_file *file;
    unsigned char *copy;

    note_read(path);
    file = find_file(path);
    if (file == NULL) {
        if (read_misses < MISSED_PATH_MAX) {
            snprintf(missed_paths[read_misses], FILE_PATH_MAX, "%s", path);
            read_misses = read_misses + 1;
        }
        return FILE_ERR_OPEN;
    }
    if (file->size == 0) {
        return FILE_ERR_SIZE;
    }
    copy = mem_alloc(file->size);
    if (copy == NULL) {
        return FILE_ERR_MEMORY;
    }
    memcpy(copy, file->data, file->size);
    *data = copy;
    *size = file->size;
    return FILE_OK;
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

int fake_files_read_misses_under(const char *folder)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < read_misses; index++) {
        if (strncmp(missed_paths[index], folder, strlen(folder)) == 0) {
            count = count + 1;
        }
    }
    return count;
}

void fake_files_fail_folder_listing(int should_fail)
{
    folder_listing_fails = should_fail;
}

int file_list_folder(const char *path, file_name_visitor visit, void *context)
{
    unsigned int path_length;
    int index;

    note_read(path);
    if (folder_listing_fails) {
        return FILE_ERR_OPEN;
    }
    path_length = (unsigned int)strlen(path);
    for (index = 0; index < FAKE_FILE_COUNT; index++) {
        const char *name;

        if (!fake_files[index].in_use || strncmp(fake_files[index].path, path, path_length) != 0 || fake_files[index].path[path_length] != '/') {
            continue;
        }
        name = &fake_files[index].path[path_length + 1];
        if (strchr(name, '/') == NULL) {
            visit(context, name);
        }
    }
    return FILE_OK;
}
