#include <stdio.h>
#include <string.h>
#include "page_tiles.h"
#include "text_lines.h"

#define PAGE_TILE_LINE_MAX 256
#define PAGE_TILE_LINE_SCAN "tile " ASSET_NAME_SCAN " %f %f " ASSET_NAME_SCAN " %f %f %f %f %f %f %f %f"

static struct page_tile_set sets[PAGE_TILE_SET_MOST];
static int set_count;

void page_tiles_clear(void)
{
    memset(sets, 0, sizeof(sets));
    set_count = 0;
}

struct page_tile_set *page_tiles_find(const char *page_name)
{
    int index;

    for (index = 0; index < set_count; index++) {
        if (strcmp(sets[index].page_name, page_name) == 0) {
            return &sets[index];
        }
    }
    return NULL;
}

static struct page_tile_set *set_for_page(const char *page_name, float page_width, float page_height)
{
    struct page_tile_set *set;

    set = page_tiles_find(page_name);
    if (set != NULL) {
        return set;
    }
    if (set_count >= PAGE_TILE_SET_MOST) {
        return NULL;
    }
    set = &sets[set_count];
    set_count = set_count + 1;
    memset(set, 0, sizeof(*set));
    snprintf(set->page_name, sizeof(set->page_name), "%s", page_name);
    snprintf(set->as_page.name, sizeof(set->as_page.name), "%s", page_name);
    set->page_width = page_width;
    set->page_height = page_height;
    set->as_page.width = (unsigned int)page_width;
    set->as_page.height = (unsigned int)page_height;
    set->as_page.state = CACHED_TEXTURE_LOADING;
    set->as_page.texture = RENDERER_NO_TEXTURE;
    return set;
}

int page_tiles_read_text(const char *text, unsigned int text_size)
{
    char line[PAGE_TILE_LINE_MAX];
    char page_name[ASSET_NAME_MAX];
    struct page_tile tile;
    struct page_tile_set *set;
    float page_width;
    float page_height;
    unsigned int offset;
    int tile_count;

    tile_count = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        memset(&tile, 0, sizeof(tile));
        if (sscanf(line, PAGE_TILE_LINE_SCAN, page_name, &page_width, &page_height, tile.name, &tile.left, &tile.top, &tile.width, &tile.height, &tile.draw_left, &tile.draw_top, &tile.draw_right, &tile.draw_bottom) != 12) {
            continue;
        }
        set = set_for_page(page_name, page_width, page_height);
        if (set == NULL || set->tile_count >= PAGE_TILE_MOST || tile.width <= 0.0f || tile.height <= 0.0f) {
            continue;
        }
        set->tiles[set->tile_count] = tile;
        set->tile_count = set->tile_count + 1;
        tile_count = tile_count + 1;
    }
    return tile_count;
}

static int cut_span(float page_start, float page_end, float draw_start, float draw_end, float screen_start, float screen_size, float *screen_cut_start, float *screen_cut_size, float *page_cut_start, float *page_cut_end)
{
    float low;
    float high;
    float span;

    span = page_end - page_start;
    if (span == 0.0f) {
        return 0;
    }
    low = page_start < page_end ? page_start : page_end;
    high = page_start < page_end ? page_end : page_start;
    if (low < draw_start) {
        low = draw_start;
    }
    if (high > draw_end) {
        high = draw_end;
    }
    if (high <= low) {
        return 0;
    }
    if (span > 0.0f) {
        *page_cut_start = low;
        *page_cut_end = high;
    } else {
        *page_cut_start = high;
        *page_cut_end = low;
    }
    *screen_cut_start = screen_start + screen_size * (*page_cut_start - page_start) / span;
    *screen_cut_size = screen_size * (*page_cut_end - *page_cut_start) / span;
    return 1;
}

int page_tiles_cut(const struct page_tile_set *set, struct renderer_rect screen_area, struct renderer_rect texture_area, struct page_tile_part *parts)
{
    float page_left;
    float page_right;
    float page_top;
    float page_bottom;
    int count;
    int index;

    page_left = texture_area.x * set->page_width;
    page_right = (texture_area.x + texture_area.width) * set->page_width;
    page_top = texture_area.y * set->page_height;
    page_bottom = (texture_area.y + texture_area.height) * set->page_height;
    count = 0;
    for (index = 0; index < set->tile_count; index++) {
        const struct page_tile *tile;
        float cut_left;
        float cut_right;
        float cut_top;
        float cut_bottom;
        struct page_tile_part part;

        tile = &set->tiles[index];
        if (!cut_span(page_left, page_right, tile->draw_left, tile->draw_right, screen_area.x, screen_area.width, &part.screen_area.x, &part.screen_area.width, &cut_left, &cut_right)) {
            continue;
        }
        if (!cut_span(page_top, page_bottom, tile->draw_top, tile->draw_bottom, screen_area.y, screen_area.height, &part.screen_area.y, &part.screen_area.height, &cut_top, &cut_bottom)) {
            continue;
        }
        part.tile = tile;
        part.texture_area.x = (cut_left - tile->left) / tile->width;
        part.texture_area.width = (cut_right - cut_left) / tile->width;
        part.texture_area.y = (cut_top - tile->top) / tile->height;
        part.texture_area.height = (cut_bottom - cut_top) / tile->height;
        parts[count] = part;
        count = count + 1;
    }
    return count;
}
