#include "etc_decode.h"

static const int base_intensity_table[8][4] = {
    {2, 8, -2, -8},
    {5, 17, -5, -17},
    {9, 29, -9, -29},
    {13, 42, -13, -42},
    {18, 60, -18, -60},
    {24, 80, -24, -80},
    {33, 106, -33, -106},
    {47, 183, -47, -183}
};

static const int distance_table[8] = {3, 6, 11, 16, 23, 32, 41, 64};

static int clamp_to_byte(int value)
{
    if (value < 0) {
        return 0;
    }
    return value > 255 ? 255 : value;
}

static int expand_4_to_8(int value)
{
    return (value << 4) | value;
}

static int expand_5_to_8(int value)
{
    return (value << 3) | (value >> 2);
}

static int expand_6_to_8(int value)
{
    return (value << 2) | (value >> 4);
}

static int expand_7_to_8(int value)
{
    return (value << 1) | (value >> 6);
}

static int sign_extend_3_bits(int value)
{
    return (value & 4) ? value - 8 : value;
}

static int pixel_selector(const uint8_t *block, int x, int y)
{
    int bit_index = x * 4 + y;
    int high_word = (block[4] << 8) | block[5];
    int low_word = (block[6] << 8) | block[7];
    return (((high_word >> bit_index) & 1) << 1) | ((low_word >> bit_index) & 1);
}

static void write_texel(uint8_t *texels_rgb8, int x, int y, int red, int green, int blue)
{
    uint8_t *texel = texels_rgb8 + (x + y * 4) * 3;
    texel[0] = (uint8_t)clamp_to_byte(red);
    texel[1] = (uint8_t)clamp_to_byte(green);
    texel[2] = (uint8_t)clamp_to_byte(blue);
}

static void decode_subblock_mode(const uint8_t *block, const int base_colors[2][3], uint8_t *texels_rgb8)
{
    int table_indices[2] = {(block[3] >> 5) & 7, (block[3] >> 2) & 7};
    int is_flipped = block[3] & 1;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int subblock = is_flipped ? (y >= 2) : (x >= 2);
            int modifier = base_intensity_table[table_indices[subblock]][pixel_selector(block, x, y)];
            write_texel(texels_rgb8, x, y, base_colors[subblock][0] + modifier, base_colors[subblock][1] + modifier, base_colors[subblock][2] + modifier);
        }
    }
}

static void decode_paint_color_mode(const uint8_t *block, const int paint_colors[4][3], uint8_t *texels_rgb8)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            const int *color = paint_colors[pixel_selector(block, x, y)];
            write_texel(texels_rgb8, x, y, color[0], color[1], color[2]);
        }
    }
}

static void decode_t_mode(const uint8_t *block, uint8_t *texels_rgb8)
{
    int red1 = (((block[0] >> 3) & 3) << 2) | (block[0] & 3);
    int green1 = block[1] >> 4;
    int blue1 = block[1] & 0xF;
    int red2 = block[2] >> 4;
    int green2 = block[2] & 0xF;
    int blue2 = block[3] >> 4;
    int distance = distance_table[(((block[3] >> 2) & 3) << 1) | (block[3] & 1)];
    int base_red = expand_4_to_8(red2);
    int base_green = expand_4_to_8(green2);
    int base_blue = expand_4_to_8(blue2);
    int paint_colors[4][3] = {
        {expand_4_to_8(red1), expand_4_to_8(green1), expand_4_to_8(blue1)},
        {base_red + distance, base_green + distance, base_blue + distance},
        {base_red, base_green, base_blue},
        {base_red - distance, base_green - distance, base_blue - distance}
    };
    decode_paint_color_mode(block, paint_colors, texels_rgb8);
}

static void decode_h_mode(const uint8_t *block, uint8_t *texels_rgb8)
{
    int red1 = (block[0] >> 3) & 0xF;
    int green1 = ((block[0] & 7) << 1) | ((block[1] >> 4) & 1);
    int blue1 = (block[1] & 8) | ((block[1] & 3) << 1) | (block[2] >> 7);
    int red2 = (block[2] >> 3) & 0xF;
    int green2 = ((block[2] & 7) << 1) | (block[3] >> 7);
    int blue2 = (block[3] >> 3) & 0xF;
    int color1 = expand_4_to_8(red1) << 16 | expand_4_to_8(green1) << 8 | expand_4_to_8(blue1);
    int color2 = expand_4_to_8(red2) << 16 | expand_4_to_8(green2) << 8 | expand_4_to_8(blue2);
    int distance_index = (((block[3] >> 2) & 1) << 2) | ((block[3] & 1) << 1) | (color1 >= color2 ? 1 : 0);
    int distance = distance_table[distance_index];
    int red_a = expand_4_to_8(red1);
    int green_a = expand_4_to_8(green1);
    int blue_a = expand_4_to_8(blue1);
    int red_b = expand_4_to_8(red2);
    int green_b = expand_4_to_8(green2);
    int blue_b = expand_4_to_8(blue2);
    int paint_colors[4][3] = {
        {red_a + distance, green_a + distance, blue_a + distance},
        {red_a - distance, green_a - distance, blue_a - distance},
        {red_b + distance, green_b + distance, blue_b + distance},
        {red_b - distance, green_b - distance, blue_b - distance}
    };
    decode_paint_color_mode(block, paint_colors, texels_rgb8);
}

static void decode_planar_mode(const uint8_t *block, uint8_t *texels_rgb8)
{
    int red_origin = expand_6_to_8((block[0] >> 1) & 0x3F);
    int green_origin = expand_7_to_8(((block[0] & 1) << 6) | ((block[1] >> 1) & 0x3F));
    int blue_origin = expand_6_to_8(((block[1] & 1) << 5) | (block[2] & 0x18) | ((block[2] & 3) << 1) | (block[3] >> 7));
    int red_horizontal = expand_6_to_8((((block[3] >> 2) & 0x1F) << 1) | (block[3] & 1));
    int green_horizontal = expand_7_to_8((block[4] >> 1) & 0x7F);
    int blue_horizontal = expand_6_to_8(((block[4] & 1) << 5) | (block[5] >> 3));
    int red_vertical = expand_6_to_8(((block[5] & 7) << 3) | (block[6] >> 5));
    int green_vertical = expand_7_to_8(((block[6] & 0x1F) << 2) | (block[7] >> 6));
    int blue_vertical = expand_6_to_8(block[7] & 0x3F);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int red = (x * (red_horizontal - red_origin) + y * (red_vertical - red_origin) + 4 * red_origin + 2) >> 2;
            int green = (x * (green_horizontal - green_origin) + y * (green_vertical - green_origin) + 4 * green_origin + 2) >> 2;
            int blue = (x * (blue_horizontal - blue_origin) + y * (blue_vertical - blue_origin) + 4 * blue_origin + 2) >> 2;
            write_texel(texels_rgb8, x, y, red, green, blue);
        }
    }
}

etc2_block_mode etc2_rgb8_decode_block(const uint8_t *block, uint8_t *texels_rgb8)
{
    int is_differential = (block[3] >> 1) & 1;
    int base_colors[2][3];
    if (!is_differential) {
        for (int channel = 0; channel < 3; channel++) {
            base_colors[0][channel] = expand_4_to_8(block[channel] >> 4);
            base_colors[1][channel] = expand_4_to_8(block[channel] & 0xF);
        }
        decode_subblock_mode(block, base_colors, texels_rgb8);
        return ETC2_MODE_INDIVIDUAL;
    }
    int base[3];
    int second[3];
    for (int channel = 0; channel < 3; channel++) {
        base[channel] = block[channel] >> 3;
        second[channel] = base[channel] + sign_extend_3_bits(block[channel] & 7);
    }
    if (second[0] < 0 || second[0] > 31) {
        decode_t_mode(block, texels_rgb8);
        return ETC2_MODE_T;
    }
    if (second[1] < 0 || second[1] > 31) {
        decode_h_mode(block, texels_rgb8);
        return ETC2_MODE_H;
    }
    if (second[2] < 0 || second[2] > 31) {
        decode_planar_mode(block, texels_rgb8);
        return ETC2_MODE_PLANAR;
    }
    for (int channel = 0; channel < 3; channel++) {
        base_colors[0][channel] = expand_5_to_8(base[channel]);
        base_colors[1][channel] = expand_5_to_8(second[channel]);
    }
    decode_subblock_mode(block, base_colors, texels_rgb8);
    return ETC2_MODE_DIFFERENTIAL;
}

void etc2_rgb8_decode_image(const uint8_t *blocks, int image_width, int image_height, uint8_t *pixels_rgba8)
{
    int columns = (image_width + 3) / 4;
    int rows = (image_height + 3) / 4;
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            uint8_t texels[4 * 4 * 3];
            etc2_rgb8_decode_block(blocks + (size_t)(row * columns + column) * ETC2_RGB8_BLOCK_BYTES, texels);
            for (int y = 0; y < 4 && row * 4 + y < image_height; y++) {
                for (int x = 0; x < 4 && column * 4 + x < image_width; x++) {
                    uint8_t *pixel = pixels_rgba8 + ((size_t)(row * 4 + y) * image_width + column * 4 + x) * 4;
                    const uint8_t *texel = texels + (x + y * 4) * 3;
                    pixel[0] = texel[0];
                    pixel[1] = texel[1];
                    pixel[2] = texel[2];
                    pixel[3] = 255;
                }
            }
        }
    }
}
