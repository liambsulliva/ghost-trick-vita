#include "test_pattern.h"

void test_pattern_fill_checker(unsigned char *rgba_pixels, int size, int tile_size)
{
    int x;
    int y;
    int index;
    int tile_is_light;

    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            index = (y * size + x) * 4;
            tile_is_light = ((x / tile_size) + (y / tile_size)) % 2;
            if (tile_is_light) {
                rgba_pixels[index] = 255;
                rgba_pixels[index + 1] = 200;
                rgba_pixels[index + 2] = 80;
            } else {
                rgba_pixels[index] = 40;
                rgba_pixels[index + 1] = 60;
                rgba_pixels[index + 2] = 120;
            }
            rgba_pixels[index + 3] = 255;
        }
    }
}
