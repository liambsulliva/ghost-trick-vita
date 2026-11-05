#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "texture_cache.h"
#include "texture_file.h"
#include "timing.h"

static struct cached_texture entries[TEXTURE_CACHE_ENTRY_MAX];
static char folder[FILE_PATH_MAX];
static unsigned int budget;
static unsigned int current_frame;
static struct texture_cache_totals totals;

static struct cached_texture *find_entry(const char *name)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && strcmp(entries[index].name, name) == 0) {
            return &entries[index];
        }
    }
    return NULL;
}

static void release_entry(struct cached_texture *entry)
{
    if (entry->texture != RENDERER_NO_TEXTURE) {
        renderer_texture_destroy(entry->texture);
        totals.bytes_in_use = totals.bytes_in_use - entry->bytes;
    }
    entry->texture = RENDERER_NO_TEXTURE;
    entry->bytes = 0;
    entry->in_use = 0;
}

static struct cached_texture *find_least_recently_used(void)
{
    struct cached_texture *oldest;
    int index;

    oldest = NULL;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (!entries[index].in_use || entries[index].is_missing) {
            continue;
        }
        if (entries[index].last_used_frame == current_frame) {
            continue;
        }
        if (oldest == NULL || entries[index].last_used_frame < oldest->last_used_frame) {
            oldest = &entries[index];
        }
    }
    return oldest;
}

static void make_room_for(unsigned int bytes_needed)
{
    struct cached_texture *oldest;

    while (totals.bytes_in_use + bytes_needed > budget) {
        oldest = find_least_recently_used();
        if (oldest == NULL) {
            run_log_write("texture_cache over_budget bytes_in_use=%u needed=%u budget=%u", totals.bytes_in_use, bytes_needed, budget);
            return;
        }
        release_entry(oldest);
        totals.evictions = totals.evictions + 1;
    }
}

static struct cached_texture *claim_free_entry(void)
{
    struct cached_texture *oldest;
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (!entries[index].in_use) {
            return &entries[index];
        }
    }
    oldest = find_least_recently_used();
    if (oldest != NULL) {
        release_entry(oldest);
        totals.evictions = totals.evictions + 1;
    }
    return oldest;
}

static void remember_missing(struct cached_texture *entry, const char *name, const char *reason, int code)
{
    run_log_write("texture missing name=%s reason=%s code=%d", name, reason, code);
    snprintf(entry->name, sizeof(entry->name), "%s", name);
    entry->texture = RENDERER_NO_TEXTURE;
    entry->width = 0;
    entry->height = 0;
    entry->bytes = 0;
    entry->is_missing = 1;
    entry->in_use = 1;
    entry->last_used_frame = current_frame;
    totals.missing = totals.missing + 1;
}

static void load_into_entry(struct cached_texture *entry, const char *name)
{
    char path[FILE_PATH_MAX];
    unsigned char *file_data;
    unsigned int file_size;
    struct texture_file_header header;
    unsigned long long started_us;
    unsigned long long read_ended_us;
    unsigned long long elapsed_us;
    int result;

    started_us = timing_now_us();
    if (!file_path_join(path, folder, name, ".gtex")) {
        remember_missing(entry, name, "path", FILE_ERR_PATH);
        return;
    }
    result = file_read_whole(path, &file_data, &file_size);
    if (result != FILE_OK) {
        remember_missing(entry, name, "read", result);
        return;
    }
    read_ended_us = timing_now_us();
    result = texture_file_read_header(file_data, file_size, &header);
    if (result != TEXTURE_FILE_OK) {
        mem_free(file_data);
        remember_missing(entry, name, "header", result);
        return;
    }
    make_room_for(header.pixel_data_size);
    entry->texture = renderer_texture_create(&file_data[TEXTURE_FILE_HEADER_SIZE], (int)header.width, (int)header.height);
    mem_free(file_data);
    if (entry->texture == RENDERER_NO_TEXTURE) {
        remember_missing(entry, name, "upload", 0);
        return;
    }
    snprintf(entry->name, sizeof(entry->name), "%s", name);
    entry->width = header.width;
    entry->height = header.height;
    entry->bytes = header.pixel_data_size;
    entry->is_missing = 0;
    entry->in_use = 1;
    entry->last_used_frame = current_frame;
    totals.bytes_in_use = totals.bytes_in_use + entry->bytes;
    totals.loads = totals.loads + 1;

    elapsed_us = timing_now_us() - started_us;
    if (elapsed_us > totals.longest_load_us) {
        totals.longest_load_us = elapsed_us;
    }
    run_log_write("texture loaded name=%s width=%u height=%u bytes=%u read_us=%llu load_us=%llu", name, header.width, header.height, header.pixel_data_size, read_ended_us - started_us, elapsed_us);
}

void texture_cache_init(const char *texture_folder, unsigned int budget_bytes)
{
    memset(entries, 0, sizeof(entries));
    memset(&totals, 0, sizeof(totals));
    snprintf(folder, sizeof(folder), "%s", texture_folder);
    budget = budget_bytes;
    current_frame = 1;
}

void texture_cache_begin_frame(void)
{
    current_frame = current_frame + 1;
}

const struct cached_texture *texture_cache_get(const char *name)
{
    struct cached_texture *entry;

    entry = find_entry(name);
    if (entry != NULL) {
        entry->last_used_frame = current_frame;
        return entry;
    }
    entry = claim_free_entry();
    if (entry == NULL) {
        run_log_write("texture_cache full name=%s", name);
        return NULL;
    }
    load_into_entry(entry, name);
    return entry;
}

void texture_cache_read_totals(struct texture_cache_totals *totals_out)
{
    *totals_out = totals;
}

void texture_cache_clear(void)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use) {
            release_entry(&entries[index]);
        }
    }
}
