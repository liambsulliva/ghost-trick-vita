#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#define TEXTURE_CACHE_GRAYSCALE_SUFFIX "-mono"

#include "asset_name.h"
#include "renderer.h"

#define TEXTURE_CACHE_ENTRY_MAX 2048
#define TEXTURE_CACHE_DEFAULT_BUDGET_BYTES (128u * 1024u * 1024u)
#define TEXTURE_CACHE_READS_AT_ONCE 2u

enum cached_texture_state {
    CACHED_TEXTURE_LOADING,
    CACHED_TEXTURE_READY,
    CACHED_TEXTURE_MISSING
};

struct cached_texture {
    char name[ASSET_NAME_MAX];
    renderer_texture texture;
    enum cached_texture_state state;
    unsigned int request_id;
    unsigned int width;
    unsigned int height;
    unsigned int memory_bytes;
    unsigned int last_used_frame;
    unsigned long long requested_at_us;
    int read_is_started;
    int in_use;
};

struct texture_cache_totals {
    unsigned int loads;
    unsigned int evictions;
    unsigned int missing;
    unsigned int bytes_in_use;
    unsigned long long longest_wait_us;
    unsigned long long longest_upload_us;
};

void texture_cache_init(const char *texture_folder, unsigned int budget_bytes);
void texture_cache_begin_frame(void);
const struct cached_texture *texture_cache_use_or_start_loading(const char *name);
void texture_cache_keep_or_start_reading_ahead(const char *name);
unsigned int texture_cache_budget_for_free_memory(unsigned int free_bytes);
void texture_cache_set_budget(unsigned int budget_bytes);
const struct cached_texture *texture_cache_load_now(const char *name);
void texture_cache_upload_finished_reads(unsigned int upload_limit);
void texture_cache_drop_waiting_reads(void);
void texture_cache_drop_waiting_reads_left_behind(const char *name_prefix);
int texture_cache_loading_count(void);
void texture_cache_read_totals(struct texture_cache_totals *totals);
void texture_cache_clear(void);

#endif
