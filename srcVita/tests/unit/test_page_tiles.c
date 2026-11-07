#include <string.h>
#include "page_tiles.h"
#include "test_support.h"

#define TENTHS 10.0f

static const char two_by_two_page[] =
    "tile big 2840 2600 big_t00 0 0 2048 2048 0 0 2046 2046\n"
    "tile big 2840 2600 big_t10 2044 0 796 2048 2046 0 2840 2046\n"
    "tile big 2840 2600 big_t01 0 2044 2048 556 0 2046 2046 2600\n"
    "tile big 2840 2600 big_t11 2044 2044 796 556 2046 2046 2840 2600\n";

static int tenths(float value)
{
    return (int)(value * TENTHS + (value < 0.0f ? -0.5f : 0.5f));
}

static struct renderer_rect rect(float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    return area;
}

static void test_the_table_groups_tiles_under_their_page(void)
{
    const struct page_tile_set *set;

    page_tiles_clear();
    CHECK_INT(page_tiles_read_text(two_by_two_page, (unsigned int)strlen(two_by_two_page)), 4);
    set = page_tiles_find("big");
    CHECK(set != NULL);
    CHECK_INT(set->tile_count, 4);
    CHECK_INT((int)set->page_width, 2840);
    CHECK(page_tiles_find("big_t00") == NULL);
}

static void test_an_area_inside_one_tile_maps_to_that_tile_alone(void)
{
    struct page_tile_part parts[PAGE_TILE_MOST];
    int count;

    page_tiles_clear();
    page_tiles_read_text(two_by_two_page, (unsigned int)strlen(two_by_two_page));
    count = page_tiles_cut(page_tiles_find("big"), rect(10.0f, 20.0f, 100.0f, 50.0f), rect(2200.0f / 2840.0f, 2100.0f / 2600.0f, 284.0f / 2840.0f, 260.0f / 2600.0f), parts);
    CHECK_INT(count, 1);
    CHECK(strcmp(parts[0].tile->name, "big_t11") == 0);
    CHECK_INT(tenths(parts[0].screen_area.x), 100);
    CHECK_INT(tenths(parts[0].screen_area.width), 1000);
    CHECK_INT(tenths(parts[0].texture_area.x * 796.0f), 1560);
    CHECK_INT(tenths(parts[0].texture_area.width * 796.0f), 2840);
}

static void test_an_area_across_the_seam_splits_at_the_middle_of_the_overlap(void)
{
    struct page_tile_part parts[PAGE_TILE_MOST];
    int count;

    page_tiles_clear();
    page_tiles_read_text(two_by_two_page, (unsigned int)strlen(two_by_two_page));
    count = page_tiles_cut(page_tiles_find("big"), rect(0.0f, 0.0f, 200.0f, 100.0f), rect(1946.0f / 2840.0f, 0.0f, 200.0f / 2840.0f, 100.0f / 2600.0f), parts);
    CHECK_INT(count, 2);
    CHECK(strcmp(parts[0].tile->name, "big_t00") == 0);
    CHECK_INT(tenths(parts[0].screen_area.width), 1000);
    CHECK_INT(tenths(parts[1].screen_area.x), 1000);
    CHECK_INT(tenths(parts[1].screen_area.width), 1000);
    CHECK_INT(tenths(parts[1].texture_area.x * 796.0f), 20);
}

static void test_a_flipped_area_keeps_its_flip_in_each_part(void)
{
    struct page_tile_part parts[PAGE_TILE_MOST];
    int count;

    page_tiles_clear();
    page_tiles_read_text(two_by_two_page, (unsigned int)strlen(two_by_two_page));
    count = page_tiles_cut(page_tiles_find("big"), rect(0.0f, 0.0f, 200.0f, 100.0f), rect(2146.0f / 2840.0f, 0.0f, -200.0f / 2840.0f, 100.0f / 2600.0f), parts);
    CHECK_INT(count, 2);
    CHECK(strcmp(parts[0].tile->name, "big_t00") == 0);
    CHECK_INT(tenths(parts[0].screen_area.x), 1000);
    CHECK_INT(tenths(parts[0].screen_area.width), 1000);
    CHECK(parts[0].texture_area.width < 0.0f);
    CHECK(strcmp(parts[1].tile->name, "big_t10") == 0);
    CHECK_INT(tenths(parts[1].screen_area.x), 0);
    CHECK(parts[1].texture_area.width < 0.0f);
}

int main(void)
{
    test_the_table_groups_tiles_under_their_page();
    test_an_area_inside_one_tile_maps_to_that_tile_alone();
    test_an_area_across_the_seam_splits_at_the_middle_of_the_overlap();
    test_a_flipped_area_keeps_its_flip_in_each_part();
    return test_finish("unit/page_tiles");
}
