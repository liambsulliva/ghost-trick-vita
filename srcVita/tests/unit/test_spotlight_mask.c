#include <stdlib.h>
#include "spotlight_mask.h"
#include "test_support.h"

static struct renderer_rect rects[SPOTLIGHT_MASK_RECT_MAX];
static unsigned char times_covered[RENDERER_HEIGHT][RENDERER_WIDTH];

static struct spotlight_triangle triangle_of(float apex_x, float apex_y, float base_x, float base_y, float half_width)
{
    struct spotlight_triangle triangle;

    triangle.apex_x = apex_x;
    triangle.apex_y = apex_y;
    triangle.base_x = base_x;
    triangle.base_y = base_y;
    triangle.half_width = half_width;
    return triangle;
}

static void paint_rects(int count)
{
    int column;
    int index;
    int row;

    memset(times_covered, 0, sizeof(times_covered));
    for (index = 0; index < count; index++) {
        for (row = (int)rects[index].y; row < (int)(rects[index].y + rects[index].height); row++) {
            for (column = (int)rects[index].x; column < (int)(rects[index].x + rects[index].width); column++) {
                times_covered[row][column] = times_covered[row][column] + 1;
            }
        }
    }
}

static int is_black(int column, int row)
{
    return times_covered[row][column] != 0;
}

static int rects_are_whole_pixels_inside_the_screen(int count)
{
    int index;

    for (index = 0; index < count; index++) {
        if (rects[index].x < 0.0f || rects[index].y < 0.0f) {
            return 0;
        }
        if (rects[index].x + rects[index].width > RENDERER_WIDTH || rects[index].y + rects[index].height > RENDERER_HEIGHT) {
            return 0;
        }
        if (rects[index].width <= 0.0f || rects[index].x != (float)(int)rects[index].x || rects[index].width != (float)(int)rects[index].width) {
            return 0;
        }
    }
    return 1;
}

static int first_column_at_or_past(float x)
{
    if (x <= 0.0f) {
        return 0;
    }
    return (int)(x + 0.5f);
}

static int triangle_lights_pixel(const struct spotlight_triangle *triangle, int column, int row)
{
    float progress;
    float centre;
    float half_width;

    progress = ((float)row + 0.5f - triangle->apex_y) / (triangle->base_y - triangle->apex_y);
    if (progress < 0.0f || progress > 1.0f) {
        return 0;
    }
    centre = triangle->apex_x + (triangle->base_x - triangle->apex_x) * progress;
    half_width = triangle->half_width * progress;
    return column >= first_column_at_or_past(centre - half_width) && column < first_column_at_or_past(centre + half_width);
}

static void check_painted_mask_matches_triangles(const struct spotlight_triangle *triangles, int triangle_count)
{
    int any_lit;
    int mismatches;
    int column;
    int index;
    int row;

    mismatches = 0;
    for (row = 0; row < RENDERER_HEIGHT; row++) {
        for (column = 0; column < RENDERER_WIDTH; column++) {
            any_lit = 0;
            for (index = 0; index < triangle_count; index++) {
                any_lit = any_lit || triangle_lights_pixel(&triangles[index], column, row);
            }
            if (times_covered[row][column] > 1 || is_black(column, row) == any_lit) {
                mismatches = mismatches + 1;
            }
        }
    }
    CHECK_INT(mismatches, 0);
}

static void test_no_triangles_means_one_rect_over_the_whole_screen(void)
{
    int count;

    count = spotlight_mask_black_rects(NULL, 0, rects);
    CHECK_INT(count, 1);
    CHECK(rects[0].x == 0.0f);
    CHECK(rects[0].y == 0.0f);
    CHECK(rects[0].width == RENDERER_WIDTH);
    CHECK(rects[0].height == RENDERER_HEIGHT);
}

static void test_one_upright_triangle_is_lit_between_its_edges_down_to_its_base(void)
{
    struct spotlight_triangle triangle;
    int count;

    triangle = triangle_of(100.0f, -100.0f, 100.0f, 300.0f, 80.0f);
    count = spotlight_mask_black_rects(&triangle, 1, rects);
    paint_rects(count);
    CHECK(rects_are_whole_pixels_inside_the_screen(count));
    check_painted_mask_matches_triangles(&triangle, 1);
    CHECK(!is_black(100, 0));
    CHECK(is_black(100 - 30, 0));
    CHECK(!is_black(100 - 70, 299));
    CHECK(is_black(100 - 90, 299));
    CHECK(is_black(100, 300));
    CHECK(is_black(100, RENDERER_HEIGHT - 1));
    CHECK(is_black(RENDERER_WIDTH - 1, 0));
}

static void test_an_edge_rounds_to_the_nearest_column(void)
{
    struct spotlight_triangle triangle;

    triangle = triangle_of(100.0f, 0.0f, 100.0f, 2.0f, 2.8f);
    CHECK_INT(spotlight_mask_black_rects(&triangle, 1, rects), 5);
    paint_rects(5);
    CHECK(is_black(98, 0));
    CHECK(!is_black(99, 0));
    CHECK(!is_black(100, 0));
    CHECK(is_black(101, 0));
    CHECK(is_black(97, 1));
    CHECK(!is_black(98, 1));
    CHECK(!is_black(101, 1));
    CHECK(is_black(102, 1));
}

static void test_a_triangle_with_no_width_lights_nothing(void)
{
    struct spotlight_triangle triangle;

    triangle = triangle_of(100.0f, 0.0f, 100.0f, 300.0f, 0.0f);
    CHECK_INT(spotlight_mask_black_rects(&triangle, 1, rects), 1);
    CHECK(rects[0].height == RENDERER_HEIGHT);
}

static void test_a_triangle_with_no_height_lights_nothing(void)
{
    struct spotlight_triangle triangle;

    triangle = triangle_of(100.0f, 50.5f, 100.0f, 50.5f, 40.0f);
    CHECK_INT(spotlight_mask_black_rects(&triangle, 1, rects), 1);
    CHECK(rects[0].width == RENDERER_WIDTH);
}

static void test_a_slanted_triangle_follows_its_centre_line(void)
{
    struct spotlight_triangle triangle;
    int count;

    triangle = triangle_of(100.0f, 0.0f, 500.0f, 400.0f, 40.0f);
    count = spotlight_mask_black_rects(&triangle, 1, rects);
    paint_rects(count);
    check_painted_mask_matches_triangles(&triangle, 1);
    CHECK(!is_black(300, 200));
    CHECK(is_black(100, 200));
    CHECK(is_black(500, 200));
}

static void test_overlapping_triangles_light_their_union_with_no_black_between(void)
{
    struct spotlight_triangle triangles[2];
    int count;

    triangles[0] = triangle_of(200.0f, 0.0f, 300.0f, 400.0f, 80.0f);
    triangles[1] = triangle_of(400.0f, 0.0f, 300.0f, 400.0f, 80.0f);
    count = spotlight_mask_black_rects(triangles, 2, rects);
    paint_rects(count);
    CHECK(rects_are_whole_pixels_inside_the_screen(count));
    check_painted_mask_matches_triangles(triangles, 2);
    CHECK(!is_black(300, 399));
    CHECK(is_black(300, 100));
    CHECK(!is_black(230, 100));
    CHECK(!is_black(370, 100));
}

static void test_a_triangle_inside_another_adds_nothing(void)
{
    struct spotlight_triangle triangles[2];
    int count;

    triangles[0] = triangle_of(300.0f, 0.0f, 300.0f, 400.0f, 200.0f);
    triangles[1] = triangle_of(300.0f, 100.0f, 300.0f, 200.0f, 10.0f);
    count = spotlight_mask_black_rects(triangles, 2, rects);
    paint_rects(count);
    check_painted_mask_matches_triangles(triangles, 2);
}

static void test_triangles_listed_right_to_left_give_the_same_mask(void)
{
    struct spotlight_triangle triangles[3];
    int count;

    triangles[0] = triangle_of(700.0f, 0.0f, 700.0f, 500.0f, 60.0f);
    triangles[1] = triangle_of(400.0f, 0.0f, 400.0f, 500.0f, 60.0f);
    triangles[2] = triangle_of(100.0f, 0.0f, 100.0f, 500.0f, 60.0f);
    count = spotlight_mask_black_rects(triangles, 3, rects);
    paint_rects(count);
    check_painted_mask_matches_triangles(triangles, 3);
    CHECK(rects_are_whole_pixels_inside_the_screen(count));
}

static void test_a_triangle_reaching_past_the_screen_is_cut_at_its_edges(void)
{
    struct spotlight_triangle triangle;
    int count;

    triangle = triangle_of(480.0f, -2000.0f, 480.0f, 3000.0f, 5000.0f);
    count = spotlight_mask_black_rects(&triangle, 1, rects);
    CHECK_INT(count, 0);
    triangle = triangle_of(-500.0f, 0.0f, -500.0f, 400.0f, 100.0f);
    CHECK_INT(spotlight_mask_black_rects(&triangle, 1, rects), 1);
    triangle = triangle_of(1500.0f, 0.0f, 1500.0f, 400.0f, 100.0f);
    CHECK_INT(spotlight_mask_black_rects(&triangle, 1, rects), 1);
    CHECK(rects[0].width == RENDERER_WIDTH);
}

static void test_rows_with_the_same_span_share_one_rect(void)
{
    struct spotlight_triangle triangle;
    int count;

    triangle = triangle_of(100.0f, -100000.0f, 100.0f, 200.0f, 100000.0f);
    count = spotlight_mask_black_rects(&triangle, 1, rects);
    paint_rects(count);
    CHECK(count < 3);
    CHECK(is_black(100, 200));
    CHECK(!is_black(100, 199));
}

static void test_the_two_beam_layouts_of_the_opening_stay_well_under_the_rect_limit(void)
{
    struct spotlight_triangle apart[2];
    struct spotlight_triangle wide[2];
    int count;

    apart[0] = triangle_of(157.5f, -176.0f, 157.5f, 496.0f, 84.375f);
    apart[1] = triangle_of(562.5f, -176.0f, 562.5f, 496.0f, 84.375f);
    count = spotlight_mask_black_rects(apart, 2, rects);
    CHECK(count < 400);
    wide[0] = triangle_of(75.0f, -900.0f, 423.75f, 446.0f, 168.75f);
    wide[1] = triangle_of(885.0f, -900.0f, 536.25f, 446.0f, 168.75f);
    count = spotlight_mask_black_rects(wide, 2, rects);
    CHECK(count < 400);
}

static void test_random_triangles_always_match_the_slow_answer(void)
{
    struct spotlight_triangle triangles[SPOTLIGHT_MASK_TRIANGLE_MAX];
    int round_number;
    int triangle_count;
    int count;
    int index;

    srand(1234);
    for (round_number = 0; round_number < 40; round_number++) {
        triangle_count = rand() % (SPOTLIGHT_MASK_TRIANGLE_MAX + 1);
        for (index = 0; index < triangle_count; index++) {
            triangles[index] = triangle_of((float)(rand() % 1400 - 200), (float)(rand() % 400 - 300), (float)(rand() % 1400 - 200), (float)(rand() % 700 + 1), (float)(rand() % 400));
        }
        count = spotlight_mask_black_rects(triangles, triangle_count, rects);
        CHECK(count <= SPOTLIGHT_MASK_RECT_MAX);
        paint_rects(count);
        CHECK(rects_are_whole_pixels_inside_the_screen(count));
        check_painted_mask_matches_triangles(triangles, triangle_count);
    }
}

static void paint_rects_around(struct renderer_rect area)
{
    paint_rects(spotlight_mask_rects_around_area(area, rects));
}

static struct renderer_rect area_of(float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    return area;
}

static void test_rects_around_an_area_cover_everything_else_once(void)
{
    int column;
    int mismatches;
    int row;
    int inside;

    paint_rects_around(area_of(100.0f, 50.0f, 200.0f, 100.0f));
    mismatches = 0;
    for (row = 0; row < RENDERER_HEIGHT; row++) {
        for (column = 0; column < RENDERER_WIDTH; column++) {
            inside = column >= 100 && column < 300 && row >= 50 && row < 150;
            if (times_covered[row][column] != (inside ? 0 : 1)) {
                mismatches = mismatches + 1;
            }
        }
    }
    CHECK_INT(mismatches, 0);
}

static void test_an_area_of_no_size_leaves_nothing_uncovered(void)
{
    int count;

    count = spotlight_mask_rects_around_area(area_of(200.0f, 100.0f, 0.0f, 0.0f), rects);
    CHECK_INT(count, 2);
    paint_rects(count);
    CHECK(is_black(200, 100));
    CHECK(is_black(0, 0));
    CHECK(is_black(RENDERER_WIDTH - 1, RENDERER_HEIGHT - 1));
}

static void test_an_area_past_the_screen_is_cut_at_its_edges(void)
{
    CHECK_INT(spotlight_mask_rects_around_area(area_of(-100.0f, -100.0f, 2000.0f, 2000.0f), rects), 0);
    paint_rects_around(area_of(-100.0f, -100.0f, 200.0f, 200.0f));
    CHECK(!is_black(50, 50));
    CHECK(is_black(100, 50));
    CHECK(is_black(50, 100));
    CHECK(rects_are_whole_pixels_inside_the_screen(spotlight_mask_rects_around_area(area_of(900.0f, 500.0f, 500.0f, 500.0f), rects)));
    CHECK_INT(spotlight_mask_rects_around_area(area_of(2000.0f, 0.0f, 100.0f, 100.0f), rects), 2);
    paint_rects(2);
    CHECK(is_black(RENDERER_WIDTH - 1, 50));
}

int main(void)
{
    test_no_triangles_means_one_rect_over_the_whole_screen();
    test_one_upright_triangle_is_lit_between_its_edges_down_to_its_base();
    test_an_edge_rounds_to_the_nearest_column();
    test_a_triangle_with_no_width_lights_nothing();
    test_a_triangle_with_no_height_lights_nothing();
    test_a_slanted_triangle_follows_its_centre_line();
    test_overlapping_triangles_light_their_union_with_no_black_between();
    test_a_triangle_inside_another_adds_nothing();
    test_triangles_listed_right_to_left_give_the_same_mask();
    test_a_triangle_reaching_past_the_screen_is_cut_at_its_edges();
    test_rows_with_the_same_span_share_one_rect();
    test_the_two_beam_layouts_of_the_opening_stay_well_under_the_rect_limit();
    test_random_triangles_always_match_the_slow_answer();
    test_rects_around_an_area_cover_everything_else_once();
    test_an_area_of_no_size_leaves_nothing_uncovered();
    test_an_area_past_the_screen_is_cut_at_its_edges();
    return test_finish("unit/spotlight_mask");
}
