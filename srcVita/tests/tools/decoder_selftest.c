#include "astc_decode.h"
#include "etc_decode.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;
static int checks = 0;

static void expect(int condition, const char *name)
{
    checks++;
    if (!condition) {
        failures++;
        printf("FAILED: %s\n", name);
    }
}

static void put_bits(uint8_t *block, int first_bit, int bit_count, uint32_t value)
{
    for (int bit = 0; bit < bit_count; bit++) {
        int position = first_bit + bit;
        block[position / 8] = (uint8_t)((block[position / 8] & ~(1 << (position % 8))) | (((value >> bit) & 1) << (position % 8)));
    }
}

static void build_void_extent(uint8_t *block, int is_hdr, int reserved_bits, int low_s, int high_s, int low_t, int high_t)
{
    memset(block, 0, 16);
    put_bits(block, 0, 9, 0x1FC);
    put_bits(block, 9, 1, (uint32_t)is_hdr);
    put_bits(block, 10, 2, (uint32_t)reserved_bits);
    put_bits(block, 12, 13, (uint32_t)low_s);
    put_bits(block, 25, 13, (uint32_t)high_s);
    put_bits(block, 38, 13, (uint32_t)low_t);
    put_bits(block, 51, 13, (uint32_t)high_t);
    put_bits(block, 64, 16, 0x1111);
    put_bits(block, 80, 16, 0x2222);
    put_bits(block, 96, 16, 0x3333);
    put_bits(block, 112, 16, 0x4444);
}

static int all_texels_equal(const uint16_t *texels, int texel_count, int red, int green, int blue, int alpha)
{
    for (int texel = 0; texel < texel_count; texel++) {
        if (texels[texel * 4] != red || texels[texel * 4 + 1] != green || texels[texel * 4 + 2] != blue || texels[texel * 4 + 3] != alpha) {
            return 0;
        }
    }
    return 1;
}

static void build_direct_rgb_block(uint8_t *block, const int *color_values, int all_weights_set)
{
    memset(block, 0, 16);
    put_bits(block, 0, 11, 0xE1);
    put_bits(block, 11, 2, 0);
    put_bits(block, 13, 4, 8);
    for (int index = 0; index < 6; index++) {
        put_bits(block, 17 + index * 8, 8, (uint32_t)color_values[index]);
    }
    if (all_weights_set) {
        put_bits(block, 128 - 25, 25, 0x1FFFFFF);
    }
}

static void test_void_extents(void)
{
    uint8_t block[16];
    uint16_t texels[ASTC_MAX_BLOCK_TEXELS * 4];
    build_void_extent(block, 0, 3, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_VOID_EXTENT_LDR, "LDR void extent kind");
    expect(all_texels_equal(texels, 36, 0x1111, 0x2222, 0x3333, 0x4444), "LDR void extent color");
    expect(block[0] == 0xFC && block[1] == 0xFD, "LDR void extent first bytes");
    build_void_extent(block, 0, 3, 0, 100, 0, 100);
    expect(astc_decode_block(block, 10, 5, 0, texels) == ASTC_BLOCK_VOID_EXTENT_LDR, "constrained void extent is legal");
    expect(all_texels_equal(texels, 50, 0x1111, 0x2222, 0x3333, 0x4444), "constrained void extent color");
    build_void_extent(block, 0, 3, 100, 100, 0, 100);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "void extent low S equal to high S is illegal");
    expect(all_texels_equal(texels, 36, 0xFFFF, 0, 0xFFFF, 0xFFFF), "illegal void extent gives magenta");
    build_void_extent(block, 0, 3, 0, 100, 200, 100);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "void extent low T above high T is illegal");
    build_void_extent(block, 0, 1, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "void extent reserved bit 11 clear is illegal");
    build_void_extent(block, 0, 2, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "void extent reserved bit 10 clear is illegal");
    build_void_extent(block, 1, 3, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_VOID_EXTENT_HDR, "HDR void extent kind");
    expect(all_texels_equal(texels, 36, 0xFFFF, 0, 0xFFFF, 0xFFFF), "HDR void extent is an error color in the LDR path");
    put_bits(block, 64, 16, 0x3C00);
    put_bits(block, 80, 16, 0x4000);
    put_bits(block, 96, 16, 0x3800);
    put_bits(block, 112, 16, 0x3C00);
    float floats[ASTC_MAX_BLOCK_TEXELS * 4];
    expect(astc_decode_block_hdr(block, 4, 4, floats) == ASTC_BLOCK_VOID_EXTENT_HDR, "HDR void extent kind in the HDR path");
    expect(floats[0] == 1.0f && floats[1] == 2.0f && floats[2] == 0.5f && floats[3] == 1.0f, "HDR void extent half floats");
}

static void test_illegal_blocks(void)
{
    uint8_t block[16];
    uint16_t texels[ASTC_MAX_BLOCK_TEXELS * 4];
    memset(block, 0, 16);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "all zero block uses a reserved mode");
    expect(all_texels_equal(texels, 36, 0xFFFF, 0, 0xFFFF, 0xFFFF), "reserved mode gives magenta");
    memset(block, 0, 16);
    put_bits(block, 0, 11, (1u << 7) | (3u << 7) | (2u << 5) | (1u << 2));
    put_bits(block, 0, 11, (3u << 7) | (2u << 5) | (1u << 2));
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "reserved mode with bits 8 and 7 set and bit 6 set");
    memset(block, 0, 16);
    put_bits(block, 0, 11, (1u << 2) | (0u << 7) | (0u << 5));
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "grid twelve wide does not fit a 6x6 block");
    expect(astc_decode_block(block, 10, 5, 0, texels) == ASTC_BLOCK_ILLEGAL, "grid twelve wide does not fit a 10x5 block");
    memset(block, 0, 16);
    put_bits(block, 0, 11, (1u << 10) | (2u << 7) | (3u << 5) | (1u << 3));
    astc_block_description description;
    astc_describe_block(block, 12, 12, &description);
    expect(description.kind == ASTC_BLOCK_ILLEGAL, "a 9x8 grid has more than 64 weights and is illegal");
    memset(block, 0, 16);
    put_bits(block, 0, 11, 0xE1 | (1u << 10));
    put_bits(block, 11, 2, 3);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "dual plane with four partitions is illegal");
    memset(block, 0, 16);
    put_bits(block, 0, 11, 0xE1);
    put_bits(block, 11, 2, 3);
    put_bits(block, 23, 2, 0);
    put_bits(block, 25, 4, 12);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "more than 18 color values is illegal");
    memset(block, 0, 16);
    put_bits(block, 0, 11, 834);
    put_bits(block, 11, 2, 2);
    put_bits(block, 23, 2, 0);
    put_bits(block, 25, 4, 8);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "too few color bits for the color values is illegal");
    memset(block, 0, 16);
    put_bits(block, 0, 11, 0x1 | (0u << 2) | (0u << 4) | (0u << 5) | (0u << 7));
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_ILLEGAL, "fewer than 24 weight bits is illegal");
}

static void test_direct_rgb_block(void)
{
    uint8_t block[16];
    uint16_t texels[ASTC_MAX_BLOCK_TEXELS * 4];
    const int ascending[6] = {10, 200, 20, 210, 30, 220};
    build_direct_rgb_block(block, ascending, 0);
    expect(astc_decode_block(block, 6, 6, 0, texels) == ASTC_BLOCK_NORMAL, "hand built block is legal");
    expect(all_texels_equal(texels, 36, 10 * 257, 20 * 257, 30 * 257, 65535), "weight zero selects endpoint zero");
    build_direct_rgb_block(block, ascending, 1);
    astc_decode_block(block, 6, 6, 0, texels);
    expect(all_texels_equal(texels, 36, 200 * 257, 210 * 257, 220 * 257, 65535), "weight one selects endpoint one");
    astc_decode_block(block, 6, 6, 1, texels);
    expect(all_texels_equal(texels, 36, (200 << 8) | 0x80, (210 << 8) | 0x80, (220 << 8) | 0x80, 65535), "sRGB endpoints expand with 0x80");
    const int descending[6] = {200, 10, 210, 20, 220, 30};
    build_direct_rgb_block(block, descending, 0);
    astc_decode_block(block, 6, 6, 0, texels);
    expect(all_texels_equal(texels, 36, ((10 + 30) >> 1) * 257, ((20 + 30) >> 1) * 257, 30 * 257, 65535), "descending sums swap and blue contract");
}

static uint64_t random_state = 0x9E3779B97F4A7C15ull;

static uint64_t next_random(void)
{
    random_state ^= random_state << 13;
    random_state ^= random_state >> 7;
    random_state ^= random_state << 17;
    return random_state;
}

static void fill_random(uint8_t *bytes, int count)
{
    for (int index = 0; index < count; index++) {
        if (index % 8 == 0) {
            uint64_t value = next_random();
            memcpy(bytes + index, &value, (size_t)(count - index < 8 ? count - index : 8));
        }
    }
}

static void fuzz_decoders(long block_count)
{
    static const int footprints[3][2] = {{6, 6}, {10, 5}, {4, 4}};
    uint8_t block[16];
    uint16_t texels16[ASTC_MAX_BLOCK_TEXELS * 4];
    float floats[ASTC_MAX_BLOCK_TEXELS * 4];
    uint64_t legal_blocks = 0;
    uint64_t illegal_blocks = 0;
    for (long index = 0; index < block_count; index++) {
        fill_random(block, 16);
        if ((index & 3) == 0) {
            put_bits(block, 0, 9, 0x1FC);
        }
        const int *footprint = footprints[index % 3];
        astc_block_kind kind = astc_decode_block(block, footprint[0], footprint[1], (int)(index >> 2) & 1, texels16);
        astc_decode_block_hdr(block, footprint[0], footprint[1], floats);
        if (kind == ASTC_BLOCK_ILLEGAL) {
            illegal_blocks++;
        } else {
            legal_blocks++;
        }
        uint8_t etc_texels[48];
        uint16_t eac_texels[16];
        etc2_rgb8_decode_block(block, etc_texels);
        eac_r11_decode_block(block, eac_texels);
    }
    printf("fuzzed %ld random blocks: %llu legal, %llu illegal\n", block_count, (unsigned long long)legal_blocks, (unsigned long long)illegal_blocks);
    int width = 37;
    int height = 23;
    size_t bytes = (size_t)((width + 3) / 4) * (size_t)((height + 3) / 4) * 16 + 16;
    uint8_t *random_blocks = malloc(bytes);
    uint8_t *pixels = malloc((size_t)width * (size_t)height * 4);
    uint16_t *pixels16 = malloc((size_t)width * (size_t)height * 8);
    float *pixels_float = malloc((size_t)width * (size_t)height * 16);
    fill_random(random_blocks, (int)bytes);
    astc_decode_image(random_blocks, width, height, 6, 6, 0, pixels);
    astc_decode_image(random_blocks, width, height, 10, 5, 1, pixels);
    astc_decode_image_16bit(random_blocks, width, height, 4, 4, 0, pixels16);
    astc_decode_image_hdr(random_blocks, width, height, 4, 4, pixels_float);
    etc2_rgb8_decode_image(random_blocks, width, height, pixels, NULL);
    eac_r11_decode_image(random_blocks, width, height, pixels16, 1, 8);
    eac_r11_decode_image(random_blocks, width, height, pixels16, 1, 16);
    free(random_blocks);
    free(pixels);
    free(pixels16);
    free(pixels_float);
}

int main(int argument_count, char **arguments)
{
    long fuzz_count = argument_count > 1 ? atol(arguments[1]) : 2000000;
    test_void_extents();
    test_illegal_blocks();
    test_direct_rgb_block();
    fuzz_decoders(fuzz_count);
    printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
