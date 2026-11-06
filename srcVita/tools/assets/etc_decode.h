#ifndef ETC_DECODE_H
#define ETC_DECODE_H

#include <stddef.h>
#include <stdint.h>

enum {
    ETC2_RGB8_BLOCK_BYTES = 8,
    EAC_R11_BLOCK_BYTES = 8,
    EAC_RG11_BLOCK_BYTES = 16
};

typedef enum {
    ETC2_MODE_INDIVIDUAL = 0,
    ETC2_MODE_DIFFERENTIAL = 1,
    ETC2_MODE_T = 2,
    ETC2_MODE_H = 3,
    ETC2_MODE_PLANAR = 4
} etc2_block_mode;

etc2_block_mode etc2_rgb8_decode_block(const uint8_t *block, uint8_t *texels_rgb8);

void eac_r11_decode_block(const uint8_t *block, uint16_t *texels_11bit);

void etc2_rgb8_decode_image(const uint8_t *blocks, int image_width, int image_height, uint8_t *pixels_rgba8, uint64_t *mode_counts);

void eac_r11_decode_image(const uint8_t *blocks, int image_width, int image_height, uint16_t *pixels_11bit, int pixel_stride, int block_stride);

#endif
