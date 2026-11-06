#include "checker_pattern.h"
#include "test_support.h"

static void test_checker_pattern_alternates_tiles(void)
{
    unsigned char pixels[4 * 4 * 4];

    checker_pattern_fill(pixels, 4, 2);
    CHECK_INT(pixels[0], 40);
    CHECK_INT(pixels[2 * 4], 255);
    CHECK_INT(pixels[(2 * 4 + 2) * 4], 40);
    CHECK_INT(pixels[3], 255);
}

int main(void)
{
    test_checker_pattern_alternates_tiles();
    return test_finish("unit/checker_pattern");
}
