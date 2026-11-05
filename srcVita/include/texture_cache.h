#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include "renderer.h"

#define TEXTURE_CACHE_ENTRY_MAX 512
#define TEXTURE_NAME_MAX 96
#define TEXTURE_CACHE_DEFAULT_BUDGET_BYTES (64u * 1024u * 1024u)

enum cached_texture_state {
    CACHED_TEXTURE_LOADING,
    CACHED_TEXTURE_READY,
    CACHED_TEXTURE_MISSING
};

struct cached_texture {
    char name[TEXTURE_NAME_MAX];
    renderer_texture texture;
    enum cached_texture_state state;
    unsigned int request_id;
    unsigned int width;
    unsigned int height;
    unsigned int bytes;
    unsigned int last_used_frame;
    unsigned long long requested_at_us;
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
const struct cached_texture *texture_cache_request(const char *name);
const struct cached_texture *texture_cache_load_now(const char *name);
void texture_cache_upload_finished_reads(unsigned int upload_limit);
void texture_cache_read_totals(struct texture_cache_totals *totals);
void texture_cache_clear(void);

#endif
