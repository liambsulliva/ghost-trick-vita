#include <stdio.h>
#include <string.h>
#include "background_reader.h"
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
static unsigned int next_request_id;
static struct texture_cache_totals totals;

static struct cached_texture *find_entry_by_name(const char *name)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && strcmp(entries[index].name, name) == 0) {
            return &entries[index];
        }
    }
    return NULL;
}

static struct cached_texture *find_entry_by_request(unsigned int request_id)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && entries[index].request_id == request_id) {
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
        if (!entries[index].in_use || entries[index].state != CACHED_TEXTURE_READY) {
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

static void evict_one(struct cached_texture *entry)
{
    release_entry(entry);
    totals.evictions = totals.evictions + 1;
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
        evict_one(oldest);
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
        evict_one(oldest);
    }
    return oldest;
}

static void mark_missing(struct cached_texture *entry, const char *reason, int code)
{
    run_log_write("texture missing name=%s reason=%s code=%d", entry->name, reason, code);
    entry->texture = RENDERER_NO_TEXTURE;
    entry->width = 0;
    entry->height = 0;
    entry->bytes = 0;
    entry->state = CACHED_TEXTURE_MISSING;
    totals.missing = totals.missing + 1;
}

static renderer_texture create_texture_from_file(const struct texture_file_header *header, const unsigned char *pixel_data)
{
    int width;
    int height;

    width = (int)header->width;
    height = (int)header->height;
    if (header->format == TEXTURE_FILE_FORMAT_DXT5) {
        return renderer_texture_create_compressed(pixel_data, header->pixel_data_size, width, height, RENDERER_COMPRESSED_DXT5);
    }
    if (header->format == TEXTURE_FILE_FORMAT_DXT1) {
        return renderer_texture_create_compressed(pixel_data, header->pixel_data_size, width, height, RENDERER_COMPRESSED_DXT1);
    }
    return renderer_texture_create(pixel_data, width, height);
}

static void upload_read_data(struct cached_texture *entry, const struct finished_read *finished)
{
    struct texture_file_header header;
    unsigned long long upload_started_us;
    unsigned long long upload_us;
    unsigned long long wait_us;
    int result;

    if (finished->result != FILE_OK) {
        mark_missing(entry, "read", finished->result);
        return;
    }
    result = texture_file_read_header(finished->data, finished->size, &header);
    if (result != TEXTURE_FILE_OK) {
        mark_missing(entry, "header", result);
        return;
    }
    make_room_for(header.pixel_data_size);
    upload_started_us = timing_now_us();
    entry->texture = create_texture_from_file(&header, &finished->data[TEXTURE_FILE_HEADER_SIZE]);
    if (entry->texture == RENDERER_NO_TEXTURE) {
        mark_missing(entry, "upload", 0);
        return;
    }
    entry->width = header.width;
    entry->height = header.height;
    entry->bytes = header.pixel_data_size;
    entry->state = CACHED_TEXTURE_READY;
    totals.bytes_in_use = totals.bytes_in_use + entry->bytes;
    totals.loads = totals.loads + 1;

    upload_us = timing_now_us() - upload_started_us;
    wait_us = timing_now_us() - entry->requested_at_us;
    if (upload_us > totals.longest_upload_us) {
        totals.longest_upload_us = upload_us;
    }
    if (wait_us > totals.longest_wait_us) {
        totals.longest_wait_us = wait_us;
    }
    run_log_write("texture loaded name=%s width=%u height=%u bytes=%u wait_us=%llu upload_us=%llu", entry->name, header.width, header.height, header.pixel_data_size, wait_us, upload_us);
}

static void handle_finished_read(const struct finished_read *finished)
{
    struct cached_texture *entry;

    entry = find_entry_by_request(finished->request_id);
    if (entry != NULL) {
        upload_read_data(entry, finished);
    }
    mem_free(finished->data);
}

void texture_cache_init(const char *texture_folder, unsigned int budget_bytes)
{
    memset(entries, 0, sizeof(entries));
    memset(&totals, 0, sizeof(totals));
    snprintf(folder, sizeof(folder), "%s", texture_folder);
    budget = budget_bytes;
    current_frame = 1;
    next_request_id = 1;
    background_reader_start();
}

void texture_cache_begin_frame(void)
{
    current_frame = current_frame + 1;
}

const struct cached_texture *texture_cache_request(const char *name)
{
    struct cached_texture *entry;
    char path[FILE_PATH_MAX];

    entry = find_entry_by_name(name);
    if (entry != NULL) {
        entry->last_used_frame = current_frame;
        return entry;
    }
    if (!file_path_join(path, folder, name, ".gtex")) {
        run_log_write("texture path_too_long name=%s", name);
        return NULL;
    }
    entry = claim_free_entry();
    if (entry == NULL) {
        run_log_write("texture_cache full name=%s", name);
        return NULL;
    }
    if (!background_reader_submit(next_request_id, path)) {
        return NULL;
    }
    snprintf(entry->name, sizeof(entry->name), "%s", name);
    entry->state = CACHED_TEXTURE_LOADING;
    entry->request_id = next_request_id;
    entry->texture = RENDERER_NO_TEXTURE;
    entry->bytes = 0;
    entry->width = 0;
    entry->height = 0;
    entry->last_used_frame = current_frame;
    entry->requested_at_us = timing_now_us();
    entry->in_use = 1;
    next_request_id = next_request_id + 1;
    return entry;
}

const struct cached_texture *texture_cache_load_now(const char *name)
{
    const struct cached_texture *entry;
    struct finished_read finished;

    entry = texture_cache_request(name);
    while (entry != NULL && entry->state == CACHED_TEXTURE_LOADING) {
        background_reader_wait_for_finished(&finished);
        handle_finished_read(&finished);
    }
    return entry;
}

void texture_cache_upload_finished_reads(unsigned int upload_limit)
{
    struct finished_read finished;
    unsigned int uploads;

    uploads = 0;
    while (uploads < upload_limit && background_reader_take_finished(&finished)) {
        handle_finished_read(&finished);
        uploads = uploads + 1;
    }
}

void texture_cache_read_totals(struct texture_cache_totals *totals_out)
{
    *totals_out = totals;
}

int texture_cache_loading_count(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING) {
            count = count + 1;
        }
    }
    return count;
}

void texture_cache_clear(void)
{
    struct finished_read finished;
    int index;

    while (texture_cache_loading_count() > 0) {
        background_reader_wait_for_finished(&finished);
        handle_finished_read(&finished);
    }
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use) {
            release_entry(&entries[index]);
        }
    }
}
