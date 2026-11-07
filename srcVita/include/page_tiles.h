#ifndef PAGE_TILES_H
#define PAGE_TILES_H

#include "asset_name.h"
#include "texture_cache.h"

#define PAGE_TILE_MOST 4
#define PAGE_TILE_SET_MOST 1536

struct page_tile {
    char name[ASSET_NAME_MAX];
    float left;
    float top;
    float width;
    float height;
    float draw_left;
    float draw_top;
    float draw_right;
    float draw_bottom;
};

struct page_tile_set {
    char page_name[ASSET_NAME_MAX];
    float page_width;
    float page_height;
    int tile_count;
    struct page_tile tiles[PAGE_TILE_MOST];
    struct cached_texture as_page;
};

struct page_tile_part {
    const struct page_tile *tile;
    struct renderer_rect screen_area;
    struct renderer_rect texture_area;
};

void page_tiles_clear(void);
int page_tiles_read_text(const char *text, unsigned int text_size);
struct page_tile_set *page_tiles_find(const char *page_name);
int page_tiles_dropped_count(void);
int page_tiles_cut(const struct page_tile_set *set, struct renderer_rect screen_area, struct renderer_rect texture_area, struct page_tile_part *parts);

#endif
