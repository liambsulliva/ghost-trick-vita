#include <stdio.h>
#include <string.h>
#include "page_tiles.h"
#include "background_reader.h"
#include "clock.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "crc32.h"
#include "texture_cache.h"
#include "texture_file.h"

#define READS_WAITING_MOST_WHEN_READING_AHEAD 2
#define MEMORY_KEPT_FREE_BYTES (92u * 1024u * 1024u)
#define BUDGET_MOST_BYTES (256u * 1024u * 1024u)
#define RETRY_AFTER_FRAMES 15u
#define LOAD_NOW_MOST_ROUNDS 64u
#define READ_AHEAD_SPARES_FRAMES_DRAWN_WITHIN 60u
#define READ_AHEAD_WANT_LASTS_FRAMES 2u
#define READ_AHEAD_LEAST_ROOM_BYTES (4u * 1024u * 1024u)
#define NAME_BUCKET_COUNT 4096u
#define NO_ENTRY (-1)

enum texture_wanter {
    WANTED_BY_DRAWING,
    WANTED_BY_READ_AHEAD
};

static struct cached_texture entries[TEXTURE_CACHE_ENTRY_MAX];
static int first_entry_in_bucket[NAME_BUCKET_COUNT];
static int next_entry_in_bucket[TEXTURE_CACHE_ENTRY_MAX];
static unsigned int read_ahead_checked_frame;
static int read_ahead_had_room;
static int read_ahead_waiting_reads;
static char folder[FILE_PATH_MAX];
static unsigned int bucket_count;
static unsigned int budget;
static unsigned int current_frame;
static unsigned int next_request_id;
static unsigned int reads_in_flight;
static int cache_full_was_logged;
static struct texture_cache_totals totals;

static struct cached_texture *start_loading(const char *name, enum texture_wanter wanter);

static unsigned int name_bucket(const char *name)
{
    return crc32_add(0u, (const unsigned char *)name, (unsigned int)strlen(name)) % NAME_BUCKET_COUNT;
}

static void forget_all_names(void)
{
    unsigned int bucket;
    int index;

    for (bucket = 0; bucket < NAME_BUCKET_COUNT; bucket++) {
        first_entry_in_bucket[bucket] = NO_ENTRY;
    }
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        next_entry_in_bucket[index] = NO_ENTRY;
    }
}

static void remember_name(struct cached_texture *entry)
{
    unsigned int bucket;
    int index;

    index = (int)(entry - entries);
    bucket = name_bucket(entry->name);
    next_entry_in_bucket[index] = first_entry_in_bucket[bucket];
    first_entry_in_bucket[bucket] = index;
}

static void stop_using_entry(struct cached_texture *entry)
{
    unsigned int bucket;
    int index;
    int *link;

    if (!entry->in_use) {
        return;
    }
    index = (int)(entry - entries);
    bucket = name_bucket(entry->name);
    for (link = &first_entry_in_bucket[bucket]; *link != NO_ENTRY; link = &next_entry_in_bucket[*link]) {
        if (*link == index) {
            *link = next_entry_in_bucket[index];
            break;
        }
    }
    next_entry_in_bucket[index] = NO_ENTRY;
    entry->in_use = 0;
}

static struct cached_texture *find_entry_by_name(const char *name)
{
    int index;

    for (index = first_entry_in_bucket[name_bucket(name)]; index != NO_ENTRY; index = next_entry_in_bucket[index]) {
        if (strcmp(entries[index].name, name) == 0) {
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
        totals.bytes_in_use = totals.bytes_in_use - entry->memory_bytes;
    }
    entry->texture = RENDERER_NO_TEXTURE;
    entry->memory_bytes = 0;
    stop_using_entry(entry);
}

static int is_wanted_ahead(const struct cached_texture *entry)
{
    return current_frame < entry->wanted_ahead_until_frame;
}

static int was_drawn_lately(const struct cached_texture *entry)
{
    return entry->was_drawn && current_frame - entry->last_used_frame < READ_AHEAD_SPARES_FRAMES_DRAWN_WITHIN;
}

static int may_evict(const struct cached_texture *entry, enum texture_wanter wanter)
{
    if (!entry->in_use || entry->state != CACHED_TEXTURE_READY || entry->last_used_frame == current_frame) {
        return 0;
    }
    return wanter == WANTED_BY_DRAWING || (!is_wanted_ahead(entry) && !was_drawn_lately(entry));
}

static int eviction_tier(const struct cached_texture *entry)
{
    if (was_drawn_lately(entry)) {
        return 2;
    }
    return is_wanted_ahead(entry) ? 1 : 0;
}

static int is_evicted_before(const struct cached_texture *entry, const struct cached_texture *other)
{
    if (eviction_tier(entry) != eviction_tier(other)) {
        return eviction_tier(entry) < eviction_tier(other);
    }
    return entry->last_used_frame < other->last_used_frame;
}

static struct cached_texture *find_entry_to_evict(enum texture_wanter wanter)
{
    struct cached_texture *first;
    int index;

    first = NULL;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (!may_evict(&entries[index], wanter)) {
            continue;
        }
        if (first == NULL || is_evicted_before(&entries[index], first)) {
            first = &entries[index];
        }
    }
    return first;
}

static void evict_one(struct cached_texture *entry)
{
    release_entry(entry);
    totals.evictions = totals.evictions + 1;
}

static int make_room_for(unsigned int bytes_needed, enum texture_wanter wanter)
{
    struct cached_texture *first;

    while (totals.bytes_in_use + bytes_needed > budget) {
        first = find_entry_to_evict(wanter);
        if (first == NULL) {
            if (wanter == WANTED_BY_DRAWING) {
                run_log_write("texture_cache over_budget bytes_in_use=%u needed=%u budget=%u", totals.bytes_in_use, bytes_needed, budget);
            }
            return 0;
        }
        evict_one(first);
    }
    return 1;
}

static struct cached_texture *claim_free_entry(enum texture_wanter wanter)
{
    struct cached_texture *first;
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (!entries[index].in_use) {
            return &entries[index];
        }
    }
    first = find_entry_to_evict(wanter);
    if (first != NULL) {
        evict_one(first);
    }
    return first;
}

static int read_ahead_has_room(void)
{
    return totals.bytes_in_use + READ_AHEAD_LEAST_ROOM_BYTES <= budget || find_entry_to_evict(WANTED_BY_READ_AHEAD) != NULL;
}

static void mark_missing(struct cached_texture *entry, const char *reason, int code)
{
    run_log_write("texture missing name=%s reason=%s code=%d", entry->name, reason, code);
    entry->texture = RENDERER_NO_TEXTURE;
    entry->width = 0;
    entry->height = 0;
    entry->memory_bytes = 0;
    entry->state = CACHED_TEXTURE_MISSING;
    totals.missing = totals.missing + 1;
}

static int is_power_of_two(unsigned int value)
{
    return value != 0u && (value & (value - 1u)) == 0u;
}

static void retry_later(struct cached_texture *entry, const char *reason)
{
    entry->retries = entry->retries + 1u;
    totals.retries = totals.retries + 1u;
    if (is_power_of_two(entry->retries)) {
        run_log_write("texture retry name=%s reason=%s retries=%u", entry->name, reason, entry->retries);
    }
    entry->state = CACHED_TEXTURE_LOADING;
    entry->read_is_started = 0;
    entry->retry_at_frame = current_frame + RETRY_AFTER_FRAMES;
}

static void free_texture_memory_for_a_retry(void)
{
    struct cached_texture *oldest;

    oldest = find_entry_to_evict(WANTED_BY_DRAWING);
    if (oldest != NULL) {
        evict_one(oldest);
    }
}

static int is_wanted_sooner(const struct cached_texture *entry, const struct cached_texture *other)
{
    if (entry->last_used_frame != other->last_used_frame) {
        return entry->last_used_frame > other->last_used_frame;
    }
    return entry->request_id < other->request_id;
}

static struct cached_texture *find_waiting_entry_wanted_soonest(void)
{
    struct cached_texture *soonest;
    int index;

    soonest = NULL;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (!entries[index].in_use || entries[index].state != CACHED_TEXTURE_LOADING || entries[index].read_is_started) {
            continue;
        }
        if ((int)(entries[index].retry_at_frame - current_frame) > 0) {
            continue;
        }
        if (soonest == NULL || is_wanted_sooner(&entries[index], soonest)) {
            soonest = &entries[index];
        }
    }
    return soonest;
}

static int name_asks_for_grayscale(const char *name)
{
    size_t name_length;
    size_t suffix_length;

    name_length = strlen(name);
    suffix_length = strlen(TEXTURE_CACHE_GRAYSCALE_SUFFIX);
    return name_length > suffix_length && strcmp(&name[name_length - suffix_length], TEXTURE_CACHE_GRAYSCALE_SUFFIX) == 0;
}

static int join_texture_path(char *path, const char *file_name)
{
    char bucket_folder[FILE_PATH_MAX];
    unsigned int bucket;
    int written;

    if (bucket_count == 0u) {
        return file_path_join(path, folder, file_name, ".gtex");
    }
    bucket = crc32_add(0u, (const unsigned char *)file_name, (unsigned int)strlen(file_name)) % bucket_count;
    written = snprintf(bucket_folder, sizeof(bucket_folder), "%s/%02x", folder, bucket);
    if (written < 0 || (size_t)written >= sizeof(bucket_folder)) {
        path[0] = '\0';
        return 0;
    }
    return file_path_join(path, bucket_folder, file_name, ".gtex");
}

static void start_read(struct cached_texture *entry)
{
    char path[FILE_PATH_MAX];
    char file_name[ASSET_NAME_MAX];

    snprintf(file_name, sizeof(file_name), "%s", entry->name);
    if (name_asks_for_grayscale(file_name)) {
        file_name[strlen(file_name) - strlen(TEXTURE_CACHE_GRAYSCALE_SUFFIX)] = '\0';
    }
    if (!join_texture_path(path, file_name)) {
        mark_missing(entry, "path_too_long", 0);
        return;
    }
    if (!background_reader_submit(entry->request_id, path, name_asks_for_grayscale(entry->name))) {
        retry_later(entry, "read_queue_full");
        return;
    }
    entry->read_is_started = 1;
    reads_in_flight = reads_in_flight + 1;
}

static void start_waiting_reads(void)
{
    struct cached_texture *entry;

    while (reads_in_flight < TEXTURE_CACHE_READS_AT_ONCE) {
        entry = find_waiting_entry_wanted_soonest();
        if (entry == NULL) {
            return;
        }
        start_read(entry);
    }
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
    unsigned int memory_bytes;
    int result;

    if (finished->result == FILE_ERR_MEMORY) {
        retry_later(entry, "heap");
        return;
    }
    if (finished->result != FILE_OK) {
        mark_missing(entry, "read", finished->result);
        return;
    }
    result = texture_file_read_header(finished->data, finished->size, &header);
    if (result != TEXTURE_FILE_OK) {
        mark_missing(entry, "header", result);
        return;
    }
    memory_bytes = texture_file_memory_bytes(&header);
    upload_started_us = clock_now_us();
    if (!make_room_for(memory_bytes, entry->was_drawn ? WANTED_BY_DRAWING : WANTED_BY_READ_AHEAD)) {
        if (!entry->was_drawn) {
            totals.read_ahead_dropped = totals.read_ahead_dropped + 1u;
            stop_using_entry(entry);
            return;
        }
    }
    entry->texture = create_texture_from_file(&header, &finished->data[TEXTURE_FILE_HEADER_SIZE]);
    if (entry->texture == RENDERER_NO_TEXTURE) {
        free_texture_memory_for_a_retry();
        retry_later(entry, "gpu_memory");
        return;
    }
    entry->width = header.width;
    entry->height = header.height;
    entry->memory_bytes = memory_bytes;
    entry->state = CACHED_TEXTURE_READY;
    totals.bytes_in_use = totals.bytes_in_use + entry->memory_bytes;
    totals.loads = totals.loads + 1;

    upload_us = clock_now_us() - upload_started_us;
    wait_us = clock_now_us() - entry->requested_at_us;
    if (upload_us > totals.longest_upload_us) {
        totals.longest_upload_us = upload_us;
    }
    if (wait_us > totals.longest_wait_us) {
        totals.longest_wait_us = wait_us;
    }
    run_log_write("texture loaded name=%s width=%u height=%u file_bytes=%u memory=%u wait_us=%llu upload_us=%llu open_us=%llu read_us=%llu unpack_us=%llu", entry->name, header.width, header.height, header.pixel_data_size, memory_bytes, wait_us, upload_us, finished->open_us, finished->read_us, finished->unpack_us);
}

static void handle_finished_read(const struct finished_read *finished)
{
    struct cached_texture *entry;

    reads_in_flight = reads_in_flight - 1;
    entry = find_entry_by_request(finished->request_id);
    if (entry != NULL) {
        upload_read_data(entry, finished);
    }
    mem_free(finished->data);
    start_waiting_reads();
}

void texture_cache_init(const char *texture_folder, unsigned int budget_bytes)
{
    memset(entries, 0, sizeof(entries));
    forget_all_names();
    read_ahead_checked_frame = 0;
    memset(&totals, 0, sizeof(totals));
    snprintf(folder, sizeof(folder), "%s", texture_folder);
    budget = budget_bytes;
    current_frame = 1;
    next_request_id = 1;
    reads_in_flight = 0;
    cache_full_was_logged = 0;
    if (!background_reader_start()) {
        run_log_write("reader thread_failed");
    }
}

static int waiting_read_count(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && !entries[index].read_is_started) {
            count = count + 1;
        }
    }
    return count;
}

void texture_cache_keep_or_start_reading_ahead(const char *name)
{
    struct cached_texture *entry;
    const struct page_tile_set *tiles;

    tiles = page_tiles_find(name);
    if (tiles != NULL) {
        int tile;

        for (tile = 0; tile < tiles->tile_count; tile++) {
            texture_cache_keep_or_start_reading_ahead(tiles->tiles[tile].name);
        }
        return;
    }
    entry = find_entry_by_name(name);
    if (entry != NULL) {
        entry->wanted_ahead_until_frame = current_frame + READ_AHEAD_WANT_LASTS_FRAMES;
        return;
    }
    if (read_ahead_checked_frame != current_frame) {
        read_ahead_checked_frame = current_frame;
        read_ahead_waiting_reads = waiting_read_count();
        read_ahead_had_room = read_ahead_has_room();
    }
    if (read_ahead_waiting_reads >= READS_WAITING_MOST_WHEN_READING_AHEAD || !read_ahead_had_room) {
        return;
    }
    if (start_loading(name, WANTED_BY_READ_AHEAD) != NULL) {
        read_ahead_waiting_reads = read_ahead_waiting_reads + 1;
        read_ahead_had_room = read_ahead_has_room();
    }
}

unsigned int texture_cache_budget_for_free_memory(unsigned int free_bytes)
{
    unsigned int budget_bytes;

    budget_bytes = free_bytes > MEMORY_KEPT_FREE_BYTES ? free_bytes - MEMORY_KEPT_FREE_BYTES : 0u;
    if (budget_bytes < TEXTURE_CACHE_DEFAULT_BUDGET_BYTES) {
        return TEXTURE_CACHE_DEFAULT_BUDGET_BYTES;
    }
    return budget_bytes > BUDGET_MOST_BYTES ? BUDGET_MOST_BYTES : budget_bytes;
}

void texture_cache_set_bucket_count(unsigned int count)
{
    bucket_count = count;
}

void texture_cache_set_budget(unsigned int budget_bytes)
{
    budget = budget_bytes;
    run_log_write("texture_cache budget=%u", budget);
}

void texture_cache_begin_frame(void)
{
    current_frame = current_frame + 1;
    start_waiting_reads();
}

static const struct cached_texture *use_or_start_loading_tiles(struct page_tile_set *tiles)
{
    int tile;
    int ready_count;
    int missing_count;

    ready_count = 0;
    missing_count = 0;
    for (tile = 0; tile < tiles->tile_count; tile++) {
        const struct cached_texture *entry;

        entry = texture_cache_use_or_start_loading(tiles->tiles[tile].name);
        if (entry == NULL) {
            return NULL;
        }
        if (entry->state == CACHED_TEXTURE_READY) {
            ready_count = ready_count + 1;
        } else if (entry->state == CACHED_TEXTURE_MISSING) {
            missing_count = missing_count + 1;
        }
    }
    tiles->as_page.state = CACHED_TEXTURE_LOADING;
    if (missing_count > 0) {
        tiles->as_page.state = CACHED_TEXTURE_MISSING;
    } else if (ready_count == tiles->tile_count) {
        tiles->as_page.state = CACHED_TEXTURE_READY;
    }
    return &tiles->as_page;
}

static struct cached_texture *start_loading(const char *name, enum texture_wanter wanter)
{
    struct cached_texture *entry;

    entry = claim_free_entry(wanter);
    if (entry == NULL) {
        if (!cache_full_was_logged) {
            run_log_write("texture_cache full name=%s", name);
            cache_full_was_logged = 1;
        }
        return NULL;
    }
    cache_full_was_logged = 0;
    snprintf(entry->name, sizeof(entry->name), "%s", name);
    entry->state = CACHED_TEXTURE_LOADING;
    entry->request_id = next_request_id;
    next_request_id = next_request_id + 1;
    entry->read_is_started = 0;
    entry->texture = RENDERER_NO_TEXTURE;
    entry->memory_bytes = 0;
    entry->width = 0;
    entry->height = 0;
    entry->was_drawn = wanter == WANTED_BY_DRAWING;
    entry->last_used_frame = entry->was_drawn ? current_frame : 0u;
    entry->wanted_ahead_until_frame = entry->was_drawn ? 0u : current_frame + READ_AHEAD_WANT_LASTS_FRAMES;
    entry->requested_at_us = clock_now_us();
    entry->retry_at_frame = current_frame;
    entry->retries = 0u;
    entry->in_use = 1;
    remember_name(entry);
    start_waiting_reads();
    return entry;
}

const struct cached_texture *texture_cache_use_or_start_loading(const char *name)
{
    struct cached_texture *entry;
    struct page_tile_set *tiles;

    tiles = page_tiles_find(name);
    if (tiles != NULL) {
        return use_or_start_loading_tiles(tiles);
    }
    entry = find_entry_by_name(name);
    if (entry != NULL) {
        entry->last_used_frame = current_frame;
        entry->was_drawn = 1;
        return entry;
    }
    return start_loading(name, WANTED_BY_DRAWING);
}

static void start_retries_now(void)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && !entries[index].read_is_started) {
            entries[index].retry_at_frame = current_frame;
        }
    }
    start_waiting_reads();
}

const struct cached_texture *texture_cache_load_now(const char *name)
{
    const struct cached_texture *entry;
    struct finished_read finished;
    unsigned int rounds;

    entry = texture_cache_use_or_start_loading(name);
    for (rounds = 0; entry != NULL && entry->state == CACHED_TEXTURE_LOADING; rounds++) {
        if (rounds >= LOAD_NOW_MOST_ROUNDS) {
            struct cached_texture *given_up;

            given_up = find_entry_by_name(name);
            if (given_up != NULL) {
                mark_missing(given_up, "load_now_gave_up", (int)rounds);
            }
            return entry;
        }
        if (reads_in_flight == 0) {
            start_retries_now();
        }
        if (reads_in_flight > 0) {
            background_reader_wait_for_finished(&finished);
            handle_finished_read(&finished);
        }
        entry = texture_cache_use_or_start_loading(name);
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

void texture_cache_drop_waiting_reads(void)
{
    int index;

    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && !entries[index].read_is_started) {
            stop_using_entry(&entries[index]);
        }
    }
}

void texture_cache_drop_waiting_reads_left_behind(const char *name_prefix)
{
    size_t prefix_length;
    int index;

    prefix_length = strlen(name_prefix);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && !entries[index].read_is_started && entries[index].last_used_frame != current_frame && strncmp(entries[index].name, name_prefix, prefix_length) == 0) {
            stop_using_entry(&entries[index]);
        }
    }
}

void texture_cache_read_totals(struct texture_cache_totals *totals_out)
{
    *totals_out = totals;
}

int texture_cache_drawn_loading_count(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use && entries[index].state == CACHED_TEXTURE_LOADING && entries[index].was_drawn) {
            count = count + 1;
        }
    }
    return count;
}

int texture_cache_read_ahead_is_behind(void)
{
    return waiting_read_count() >= READS_WAITING_MOST_WHEN_READING_AHEAD;
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

    texture_cache_drop_waiting_reads();
    while (reads_in_flight > 0) {
        background_reader_wait_for_finished(&finished);
        handle_finished_read(&finished);
    }
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        if (entries[index].in_use) {
            release_entry(&entries[index]);
        }
    }
}
