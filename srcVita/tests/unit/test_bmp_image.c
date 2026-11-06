#include "bmp_image.h"
#include "test_support.h"

static void test_bmp_rows_are_padded_to_four_bytes(void)
{
    CHECK_INT(bmp_row_size(1), 4);
    CHECK_INT(bmp_row_size(4), 12);
    CHECK_INT(bmp_row_size(960), 2880);
    CHECK_INT(bmp_file_size(2, 2), 54 + 8 * 2);
}

static void test_bmp_header_describes_the_image(void)
{
    unsigned char header[BMP_HEADER_SIZE];

    bmp_write_header(header, 960, 544);
    CHECK_INT(header[0], 'B');
    CHECK_INT(header[1], 'M');
    CHECK_INT(header[18] + header[19] * 256, 960);
    CHECK_INT(header[22] + header[23] * 256, 544);
    CHECK_INT(header[26], 1);
    CHECK_INT(header[28], 24);
}

static void test_bmp_row_swaps_red_and_blue_and_drops_alpha(void)
{
    const unsigned char rgba_pixels[] = { 10, 20, 30, 255 };
    unsigned char row[4] = { 9, 9, 9, 9 };

    bmp_write_row_from_rgba(row, rgba_pixels, 1);
    CHECK_INT(row[0], 30);
    CHECK_INT(row[1], 20);
    CHECK_INT(row[2], 10);
    CHECK_INT(row[3], 0);
}

int main(void)
{
    test_bmp_rows_are_padded_to_four_bytes();
    test_bmp_header_describes_the_image();
    test_bmp_row_swaps_red_and_blue_and_drops_alpha();
    return test_finish("unit/bmp_image");
}
