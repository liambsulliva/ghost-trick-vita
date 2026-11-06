#ifndef ASTC_DECODE_H
#define ASTC_DECODE_H

#include <stddef.h>
#include <stdint.h>

enum {
    ASTC_BLOCK_BYTES = 16,
    ASTC_MAX_BLOCK_TEXELS = 144
};

typedef enum {
    ASTC_BLOCK_NORMAL = 0,
    ASTC_BLOCK_VOID_EXTENT_LDR = 1,
    ASTC_BLOCK_VOID_EXTENT_HDR = 2,
    ASTC_BLOCK_ILLEGAL = 3
} astc_block_kind;

typedef struct {
    astc_block_kind kind;
    int grid_width;
    int grid_height;
    int is_dual_plane;
    int plane2_component;
    int weight_level_count;
    int weight_bit_count;
    int partition_count;
    int partition_seed;
    int endpoint_modes[4];
    int color_value_count;
    int color_level_count;
    int color_first_bit;
} astc_block_description;

void astc_describe_block(const uint8_t *block, int block_width, int block_height, astc_block_description *description);

astc_block_kind astc_decode_block(const uint8_t *block, int block_width, int block_height, int use_srgb, uint16_t *texels_rgba16);

void astc_decode_image(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, int use_srgb, uint8_t *pixels_rgba8);

#endif
