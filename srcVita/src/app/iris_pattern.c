#include <math.h>
#include "iris_pattern.h"

static unsigned char alpha_at_distance(float distance_as_share_of_radius)
{
    float solidness;

    solidness = (distance_as_share_of_radius - IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS) / (1.0f - IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS);
    if (solidness <= 0.0f) {
        return 0;
    }
    if (solidness >= 1.0f) {
        return 255;
    }
    return (unsigned char)(solidness * 255.0f + 0.5f);
}

void iris_pattern_fill(unsigned char *rgba_pixels, int size)
{
    float middle;
    float solid_from;
    float from_middle_x;
    float from_middle_y;
    int index;
    int x;
    int y;

    middle = (float)size / 2.0f;
    solid_from = middle - 1.0f;
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            index = (y * size + x) * 4;
            from_middle_x = (float)x + 0.5f - middle;
            from_middle_y = (float)y + 0.5f - middle;
            rgba_pixels[index] = 0;
            rgba_pixels[index + 1] = 0;
            rgba_pixels[index + 2] = 0;
            rgba_pixels[index + 3] = alpha_at_distance(sqrtf(from_middle_x * from_middle_x + from_middle_y * from_middle_y) / solid_from);
        }
    }
}
