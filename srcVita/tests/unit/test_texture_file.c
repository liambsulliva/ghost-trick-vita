#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_texture_files.h"
#include "test_support.h"
#include "texture_file.h"

static void test_texture_header_accepts_a_valid_file(void)
{
    const unsigned char *file;
    unsigned int size;
    struct texture_file_header header;

    fake_files_reset();
    fake_files_add_rgba_texture("t.gtex", 4, 2);
    file = fake_files_contents("t.gtex", &size);
    CHECK_INT(texture_file_read_header(file, size, &header), TEXTURE_FILE_OK);
    CHECK_INT(header.width, 4);
    CHECK_INT(header.height, 2);
    CHECK_INT(header.pixel_data_size, 32);
}

static void test_texture_header_rejects_bad_files(void)
{
    unsigned char file[TEXTURE_FILE_HEADER_SIZE + 16];
    struct texture_file_header header;

    memset(file, 0, sizeof(file));
    CHECK_INT(texture_file_read_header(file, 3, &header), TEXTURE_FILE_ERR_TOO_SHORT);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_MAGIC);

    memcpy(file, "GTEX", 4);
    fake_files_write_u32(&file[4], 2);
    fake_files_write_u32(&file[8], 2);
    fake_files_write_u32(&file[12], 99);
    fake_files_write_u32(&file[16], 16);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_FORMAT);

    fake_files_write_u32(&file[12], TEXTURE_FILE_FORMAT_RGBA8888);
    fake_files_write_u32(&file[16], 15);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);

    fake_files_write_u32(&file[16], 16);
    CHECK_INT(texture_file_read_header(file, sizeof(file) - 1, &header), TEXTURE_FILE_ERR_TOO_SHORT);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    fake_files_write_u32(&file[4], 0);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);

    fake_files_write_u32(&file[4], 65536);
    fake_files_write_u32(&file[8], 65536);
    fake_files_write_u32(&file[16], 0);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
}

static void test_texture_header_checks_compressed_sizes(void)
{
    unsigned char file[TEXTURE_FILE_HEADER_SIZE + 64];
    struct texture_file_header header;

    memset(file, 0, sizeof(file));
    memcpy(file, "GTEX", 4);
    fake_files_write_u32(&file[4], 8);
    fake_files_write_u32(&file[8], 8);
    fake_files_write_u32(&file[12], TEXTURE_FILE_FORMAT_DXT5);
    fake_files_write_u32(&file[16], 64);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    fake_files_write_u32(&file[12], TEXTURE_FILE_FORMAT_DXT1);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
    fake_files_write_u32(&file[16], 32);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    fake_files_write_u32(&file[4], 6);
    fake_files_write_u32(&file[16], 16);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
}

static void test_compressed_memory_is_charged_at_power_of_two_size(void)
{
    struct texture_file_header header;

    header.width = 720;
    header.height = 540;
    header.format = TEXTURE_FILE_FORMAT_DXT1;
    header.pixel_data_size = 194400;
    CHECK_INT(texture_file_memory_bytes(&header), 524288);

    header.width = 2880;
    header.height = 2880;
    header.format = TEXTURE_FILE_FORMAT_DXT5;
    header.pixel_data_size = 8294400;
    CHECK_INT(texture_file_memory_bytes(&header), 16777216);

    header.width = 16;
    header.height = 8;
    header.pixel_data_size = 128;
    CHECK_INT(texture_file_memory_bytes(&header), 128);

    header.width = 3;
    header.height = 3;
    header.format = TEXTURE_FILE_FORMAT_RGBA8888;
    header.pixel_data_size = 36;
    CHECK_INT(texture_file_memory_bytes(&header), 36);
}

#define GRAY_6_AS_565 0x3186u
#define GRAY_22_AS_565 0xb5b6u
#define PURE_RED_565 0xf800u
#define NEARLY_RED_565 0xf000u
#define PURE_GREEN_565 0x07e0u

static void write_dxt1_block(unsigned char *block, unsigned int first, unsigned int second, unsigned int indices)
{
    block[0] = (unsigned char)(first & 0xffu);
    block[1] = (unsigned char)(first >> 8);
    block[2] = (unsigned char)(second & 0xffu);
    block[3] = (unsigned char)(second >> 8);
    block[4] = (unsigned char)(indices & 0xffu);
    block[5] = (unsigned char)((indices >> 8) & 0xffu);
    block[6] = (unsigned char)((indices >> 16) & 0xffu);
    block[7] = (unsigned char)(indices >> 24);
}

static unsigned int u16_of(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8);
}

static struct texture_file_header header_of_one_block(unsigned int format, unsigned int bytes)
{
    struct texture_file_header header;

    header.width = 4;
    header.height = 4;
    header.format = format;
    header.pixel_data_size = bytes;
    return header;
}

static void test_grayscale_averages_rgba_and_keeps_alpha(void)
{
    unsigned char pixel[4] = { 255, 0, 0, 128 };
    struct texture_file_header header;

    header = header_of_one_block(TEXTURE_FILE_FORMAT_RGBA8888, 4);
    texture_file_make_grayscale(&header, pixel);
    CHECK_INT(pixel[0], 53);
    CHECK_INT(pixel[1], 53);
    CHECK_INT(pixel[2], 53);
    CHECK_INT(pixel[3], 128);
}

static void test_grayscale_leaves_dxt5_alpha_and_indices_alone(void)
{
    unsigned char block[16];
    struct texture_file_header header;
    int index;

    for (index = 0; index < 8; index++) {
        block[index] = (unsigned char)(index + 1);
    }
    write_dxt1_block(&block[8], PURE_RED_565, PURE_GREEN_565, 0xe4u);
    header = header_of_one_block(TEXTURE_FILE_FORMAT_DXT5, 16);
    texture_file_make_grayscale(&header, block);
    for (index = 0; index < 8; index++) {
        CHECK_INT(block[index], index + 1);
    }
    CHECK_INT(u16_of(&block[8]), GRAY_6_AS_565);
    CHECK_INT(u16_of(&block[10]), GRAY_22_AS_565);
    CHECK_INT(block[12], 0xe4);
}

static void test_grayscale_keeps_a_four_color_dxt1_block_opaque(void)
{
    unsigned char block[8];
    struct texture_file_header header;

    write_dxt1_block(block, PURE_RED_565, PURE_GREEN_565, 0xe4u);
    header = header_of_one_block(TEXTURE_FILE_FORMAT_DXT1, 8);
    texture_file_make_grayscale(&header, block);
    CHECK_INT(u16_of(block), GRAY_22_AS_565);
    CHECK_INT(u16_of(&block[2]), GRAY_6_AS_565);
    CHECK_INT(block[4], 0xb1);
}

static void test_grayscale_never_leaves_a_transparent_index_in_an_opaque_dxt1_block(void)
{
    unsigned char block[8];
    struct texture_file_header header;

    write_dxt1_block(block, PURE_RED_565, NEARLY_RED_565, 0xe4u);
    header = header_of_one_block(TEXTURE_FILE_FORMAT_DXT1, 8);
    texture_file_make_grayscale(&header, block);
    CHECK_INT(u16_of(block), GRAY_6_AS_565);
    CHECK_INT(u16_of(&block[2]), GRAY_6_AS_565);
    CHECK_INT(block[4], 0);
}

static void test_grayscale_keeps_a_three_color_dxt1_block_transparent(void)
{
    unsigned char block[8];
    struct texture_file_header header;

    write_dxt1_block(block, PURE_GREEN_565, PURE_RED_565, 0xe4u);
    header = header_of_one_block(TEXTURE_FILE_FORMAT_DXT1, 8);
    texture_file_make_grayscale(&header, block);
    CHECK_INT(u16_of(block), GRAY_6_AS_565);
    CHECK_INT(u16_of(&block[2]), GRAY_22_AS_565);
    CHECK_INT(block[4], 0xe1);
}

static void write_u32_at(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)((value >> 8) & 0xffu);
    bytes[2] = (unsigned char)((value >> 16) & 0xffu);
    bytes[3] = (unsigned char)(value >> 24);
}

static unsigned int make_packed_header(unsigned char *file, unsigned int format, unsigned int pixel_bytes)
{
    memcpy(file, "GTEX", 4);
    write_u32_at(&file[4], 4);
    write_u32_at(&file[8], 8);
    write_u32_at(&file[12], format | TEXTURE_FILE_PACKED_FLAG);
    write_u32_at(&file[16], pixel_bytes);
    return TEXTURE_FILE_HEADER_SIZE;
}

static void test_unpacks_literal_and_repeated_blocks(void)
{
    unsigned char file[128];
    unsigned char out[TEXTURE_FILE_HEADER_SIZE + 32];
    unsigned int offset;
    int index;

    memset(file, 0, sizeof(file));
    offset = make_packed_header(file, TEXTURE_FILE_FORMAT_DXT1, 32);
    write_u32_at(&file[offset], 2);
    offset = offset + 4;
    for (index = 0; index < 16; index++) {
        file[offset + (unsigned int)index] = (unsigned char)(index + 1);
    }
    offset = offset + 16;
    write_u32_at(&file[offset], 0x80000000u | 2u);
    write_u32_at(&file[offset + 4], 0);
    offset = offset + 8;
    CHECK(texture_file_is_packed(file, offset));
    CHECK_INT(texture_file_unpacked_size(file), TEXTURE_FILE_HEADER_SIZE + 32);
    CHECK(texture_file_unpack(file, offset, out, sizeof(out)));
    CHECK(!texture_file_is_packed(out, sizeof(out)));
    CHECK_INT(out[12], TEXTURE_FILE_FORMAT_DXT1);
    CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE], 1);
    CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + 15], 16);
    CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + 16], 1);
    CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + 31], 16);
}

static void test_unpack_copies_overlapping_runs_forward(void)
{
    unsigned char file[64];
    unsigned char out[TEXTURE_FILE_HEADER_SIZE + 32];
    unsigned int offset;

    memset(file, 0, sizeof(file));
    offset = make_packed_header(file, TEXTURE_FILE_FORMAT_DXT1, 32);
    write_u32_at(&file[offset], 1);
    offset = offset + 4;
    file[offset] = 7;
    offset = offset + 8;
    write_u32_at(&file[offset], 0x80000000u | 3u);
    write_u32_at(&file[offset + 4], 0);
    offset = offset + 8;
    CHECK(texture_file_unpack(file, offset, out, sizeof(out)));
    CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + 24], 7);
}

static void test_unpack_repeats_a_two_unit_pattern_with_an_odd_tail(void)
{
    unsigned char file[96];
    unsigned char out[TEXTURE_FILE_HEADER_SIZE + 56];
    unsigned int offset;
    int unit;

    memset(file, 0, sizeof(file));
    offset = make_packed_header(file, TEXTURE_FILE_FORMAT_DXT1, 56);
    write_u32_at(&file[offset], 2);
    offset = offset + 4;
    file[offset] = 3;
    file[offset + 8] = 9;
    offset = offset + 16;
    write_u32_at(&file[offset], 0x80000000u | 5u);
    write_u32_at(&file[offset + 4], 0);
    offset = offset + 8;
    CHECK(texture_file_unpack(file, offset, out, sizeof(out)));
    for (unit = 0; unit < 7; unit++) {
        CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + unit * 8], unit % 2 == 0 ? 3 : 9);
    }
}

static void test_unpack_copies_a_distant_run_in_one_piece(void)
{
    unsigned char file[96];
    unsigned char out[TEXTURE_FILE_HEADER_SIZE + 40];
    unsigned int offset;
    int unit;

    memset(file, 0, sizeof(file));
    offset = make_packed_header(file, TEXTURE_FILE_FORMAT_DXT1, 40);
    write_u32_at(&file[offset], 3);
    offset = offset + 4;
    file[offset] = 1;
    file[offset + 8] = 2;
    file[offset + 16] = 4;
    offset = offset + 24;
    write_u32_at(&file[offset], 0x80000000u | 2u);
    write_u32_at(&file[offset + 4], 0);
    offset = offset + 8;
    CHECK(texture_file_unpack(file, offset, out, sizeof(out)));
    for (unit = 0; unit < 5; unit++) {
        CHECK_INT(out[TEXTURE_FILE_HEADER_SIZE + unit * 8], unit < 3 ? (1 << unit) : (1 << (unit - 3)));
    }
}

static void test_unpack_rejects_broken_files(void)
{
    unsigned char file[64];
    unsigned char out[TEXTURE_FILE_HEADER_SIZE + 32];
    unsigned int offset;

    memset(file, 0, sizeof(file));
    offset = make_packed_header(file, TEXTURE_FILE_FORMAT_DXT1, 32);
    write_u32_at(&file[offset], 9);
    CHECK(!texture_file_unpack(file, offset + 4, out, sizeof(out)));
    write_u32_at(&file[offset], 0x80000000u | 1u);
    write_u32_at(&file[offset + 4], 5);
    CHECK(!texture_file_unpack(file, offset + 8, out, sizeof(out)));
    write_u32_at(&file[offset], 1);
    CHECK(!texture_file_unpack(file, offset + 2, out, sizeof(out)));
    memcpy(file, "NOPE", 4);
    CHECK(!texture_file_is_packed(file, sizeof(file)));
}

int main(void)
{
    test_unpacks_literal_and_repeated_blocks();
    test_unpack_copies_overlapping_runs_forward();
    test_unpack_repeats_a_two_unit_pattern_with_an_odd_tail();
    test_unpack_copies_a_distant_run_in_one_piece();
    test_unpack_rejects_broken_files();
    test_grayscale_averages_rgba_and_keeps_alpha();
    test_grayscale_leaves_dxt5_alpha_and_indices_alone();
    test_grayscale_keeps_a_four_color_dxt1_block_opaque();
    test_grayscale_never_leaves_a_transparent_index_in_an_opaque_dxt1_block();
    test_grayscale_keeps_a_three_color_dxt1_block_transparent();
    test_texture_header_accepts_a_valid_file();
    test_texture_header_rejects_bad_files();
    test_texture_header_checks_compressed_sizes();
    test_compressed_memory_is_charged_at_power_of_two_size();
    fake_files_reset();
    return test_finish("unit/texture_file");
}
