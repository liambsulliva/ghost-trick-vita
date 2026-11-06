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
    int uses_hdr_endpoint_mode;
} astc_block_description;

typedef struct {
    uint64_t normal_blocks;
    uint64_t void_extent_ldr_blocks;
    uint64_t void_extent_hdr_blocks;
    uint64_t illegal_blocks;
    uint64_t hdr_endpoint_blocks;
    uint64_t partition_count_blocks[5];
    uint64_t endpoint_mode_partitions[16];
    uint64_t dual_plane_blocks;
    uint64_t grid_size_blocks[13][13];
} astc_block_statistics;

void astc_describe_block(const uint8_t *block, int block_width, int block_height, astc_block_description *description);

astc_block_kind astc_decode_block(const uint8_t *block, int block_width, int block_height, int use_srgb, uint16_t *texels_rgba16);

astc_block_kind astc_decode_block_hdr(const uint8_t *block, int block_width, int block_height, float *texels_rgba);

void astc_decode_image_hdr(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, float *pixels_rgba);

void astc_decode_image(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, int use_srgb, uint8_t *pixels_rgba8);

void astc_decode_image_16bit(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, int use_srgb, uint16_t *pixels_rgba16);

void astc_add_block_statistics(const uint8_t *blocks, size_t block_count, int block_width, int block_height, astc_block_statistics *statistics);

#endif
