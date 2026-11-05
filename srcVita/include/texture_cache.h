#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include "renderer.h"

#define TEXTURE_CACHE_ENTRY_MAX 512
#define TEXTURE_NAME_MAX 96
#define TEXTURE_CACHE_DEFAULT_BUDGET_BYTES (64u * 1024u * 1024u)

struct cached_texture {
    char name[TEXTURE_NAME_MAX];
    renderer_texture texture;
    unsigned int width;
    unsigned int height;
    unsigned int bytes;
    unsigned int last_used_frame;
    int is_missing;
    int in_use;
};

struct texture_cache_totals {
    unsigned int loads;
    unsigned int evictions;
    unsigned int missing;
    unsigned int bytes_in_use;
    unsigned long long longest_load_us;
};

void texture_cache_init(const char *texture_folder, unsigned int budget_bytes);
void texture_cache_begin_frame(void);
const struct cached_texture *texture_cache_get(const char *name);
void texture_cache_read_totals(struct texture_cache_totals *totals);
void texture_cache_clear(void);

#endif
