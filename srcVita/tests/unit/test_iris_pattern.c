#include "iris_pattern.h"
#include "test_support.h"

#define SIZE 64
#define MIDDLE (SIZE / 2)

static unsigned char pixels[SIZE * SIZE * 4];

static int alpha_at(int x, int y)
{
    return pixels[(y * SIZE + x) * 4 + 3];
}

static void test_the_middle_is_clear_up_to_half_the_radius(void)
{
    iris_pattern_fill(pixels, SIZE);
    CHECK_INT(alpha_at(MIDDLE, MIDDLE), 0);
    CHECK_INT(alpha_at(MIDDLE + 14, MIDDLE), 0);
    CHECK(alpha_at(MIDDLE + 18, MIDDLE) > 0);
}

static void test_the_edge_falls_in_a_straight_line_from_clear_to_solid(void)
{
    int middle_of_soft_part;

    iris_pattern_fill(pixels, SIZE);
    middle_of_soft_part = alpha_at(MIDDLE + 23, MIDDLE);
    CHECK(middle_of_soft_part > 110);
    CHECK(middle_of_soft_part < 150);
}

static void test_the_whole_border_is_solid_so_it_meets_the_black_rects_with_no_seam(void)
{
    int mismatches;
    int index;

    iris_pattern_fill(pixels, SIZE);
    mismatches = 0;
    for (index = 0; index < SIZE; index++) {
        mismatches = mismatches + (alpha_at(index, 0) != 255) + (alpha_at(index, SIZE - 1) != 255) + (alpha_at(0, index) != 255) + (alpha_at(SIZE - 1, index) != 255);
    }
    CHECK_INT(mismatches, 0);
}

static void test_it_is_round_and_grows_only_outward(void)
{
    int index;

    iris_pattern_fill(pixels, SIZE);
    CHECK_INT(alpha_at(MIDDLE + 20, MIDDLE), alpha_at(MIDDLE, MIDDLE + 20));
    CHECK_INT(alpha_at(MIDDLE + 20, MIDDLE), alpha_at(MIDDLE - 21, MIDDLE));
    for (index = 1; index < MIDDLE; index++) {
        CHECK(alpha_at(MIDDLE + index, MIDDLE) >= alpha_at(MIDDLE + index - 1, MIDDLE));
    }
}

static void test_every_pixel_is_black(void)
{
    int mismatches;
    int index;

    iris_pattern_fill(pixels, SIZE);
    mismatches = 0;
    for (index = 0; index < SIZE * SIZE; index++) {
        mismatches = mismatches + (pixels[index * 4] != 0) + (pixels[index * 4 + 1] != 0) + (pixels[index * 4 + 2] != 0);
    }
    CHECK_INT(mismatches, 0);
}

int main(void)
{
    test_the_middle_is_clear_up_to_half_the_radius();
    test_the_edge_falls_in_a_straight_line_from_clear_to_solid();
    test_the_whole_border_is_solid_so_it_meets_the_black_rects_with_no_seam();
    test_it_is_round_and_grows_only_outward();
    test_every_pixel_is_black();
    return test_finish("unit/iris_pattern");
}
