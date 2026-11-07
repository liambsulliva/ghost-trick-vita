#include <string.h>
#include "texture_file.h"

#define DXT_BLOCK_SIZE_IN_PIXELS 4
#define PACKED_MATCH_FLAG 0x80000000u
#define DXT5_BYTES_PER_BLOCK 16
#define DXT1_BYTES_PER_BLOCK 8

#define DXT1_INDEX_LOW_BITS 0x55555555u
#define DXT1_INDEX_HIGH_BITS 0xaaaaaaaau
#define LUMA_RED_WEIGHT 54u
#define LUMA_GREEN_WEIGHT 183u
#define LUMA_BLUE_WEIGHT 19u
#define LUMA_WEIGHT_SHIFT 8u
#define RGBA_BYTES_PER_PIXEL 4u

static unsigned int read_u16_little_endian(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8);
}

static void write_u16_little_endian(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)(value >> 8);
}

static void write_u32_little_endian(unsigned char *bytes, unsigned int value)
{
    write_u16_little_endian(bytes, value & 0xffffu);
    write_u16_little_endian(&bytes[2], value >> 16);
}

static unsigned int read_u32_little_endian(const unsigned char *bytes)
{
    unsigned int value;

    value = bytes[0];
    value = value | ((unsigned int)bytes[1] << 8);
    value = value | ((unsigned int)bytes[2] << 16);
    value = value | ((unsigned int)bytes[3] << 24);
    return value;
}

static int format_is_known(unsigned int format)
{
    return format == TEXTURE_FILE_FORMAT_RGBA8888 || format == TEXTURE_FILE_FORMAT_DXT5 || format == TEXTURE_FILE_FORMAT_DXT1;
}

static int dimensions_are_multiples_of_block(const struct texture_file_header *header)
{
    return header->width % DXT_BLOCK_SIZE_IN_PIXELS == 0 && header->height % DXT_BLOCK_SIZE_IN_PIXELS == 0;
}

static unsigned int expected_pixel_data_size(const struct texture_file_header *header)
{
    unsigned int block_count;

    block_count = (header->width / DXT_BLOCK_SIZE_IN_PIXELS) * (header->height / DXT_BLOCK_SIZE_IN_PIXELS);
    if (header->format == TEXTURE_FILE_FORMAT_DXT5) {
        return block_count * DXT5_BYTES_PER_BLOCK;
    }
    if (header->format == TEXTURE_FILE_FORMAT_DXT1) {
        return block_count * DXT1_BYTES_PER_BLOCK;
    }
    return header->width * header->height * 4;
}

static unsigned int round_up_to_power_of_two(unsigned int value)
{
    unsigned int rounded;

    rounded = 1;
    while (rounded < value) {
        rounded = rounded * 2;
    }
    return rounded;
}

static unsigned int luma_of(unsigned int red, unsigned int green, unsigned int blue)
{
    return (red * LUMA_RED_WEIGHT + green * LUMA_GREEN_WEIGHT + blue * LUMA_BLUE_WEIGHT) >> LUMA_WEIGHT_SHIFT;
}

static unsigned int gray_565_of(unsigned int color)
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int gray;

    red = (color >> 11) & 0x1fu;
    green = (color >> 5) & 0x3fu;
    blue = color & 0x1fu;
    gray = luma_of((red << 3) | (red >> 2), (green << 2) | (green >> 4), (blue << 3) | (blue >> 2)) >> 3;
    return (gray << 11) | (((gray << 1) | (gray >> 4)) << 5) | gray;
}

static void make_color_block_gray(unsigned char *block)
{
    write_u16_little_endian(block, gray_565_of(read_u16_little_endian(block)));
    write_u16_little_endian(&block[2], gray_565_of(read_u16_little_endian(&block[2])));
}

static unsigned int swap_first_two_palette_entries(unsigned int indices)
{
    unsigned int first_two;

    first_two = ~(indices & DXT1_INDEX_HIGH_BITS) >> 1 & DXT1_INDEX_LOW_BITS;
    return indices ^ first_two;
}

static void make_dxt1_block_gray(unsigned char *block)
{
    unsigned int first;
    unsigned int second;
    unsigned int gray_first;
    unsigned int gray_second;
    unsigned int indices;
    int has_four_colors;

    first = read_u16_little_endian(block);
    second = read_u16_little_endian(&block[2]);
    has_four_colors = first > second;
    gray_first = gray_565_of(first);
    gray_second = gray_565_of(second);
    indices = read_u32_little_endian(&block[4]);
    if (has_four_colors && gray_first == gray_second) {
        indices = 0;
    } else if (has_four_colors && gray_first < gray_second) {
        indices = indices ^ DXT1_INDEX_LOW_BITS;
    } else if (!has_four_colors && gray_first > gray_second) {
        indices = swap_first_two_palette_entries(indices);
    }
    if ((has_four_colors && gray_first < gray_second) || (!has_four_colors && gray_first > gray_second)) {
        write_u16_little_endian(block, gray_second);
        write_u16_little_endian(&block[2], gray_first);
    } else {
        write_u16_little_endian(block, gray_first);
        write_u16_little_endian(&block[2], gray_second);
    }
    write_u32_little_endian(&block[4], indices);
}

void texture_file_make_grayscale(const struct texture_file_header *header, unsigned char *pixel_data)
{
    unsigned int offset;

    if (header->format == TEXTURE_FILE_FORMAT_DXT5) {
        for (offset = 0; offset < header->pixel_data_size; offset += DXT5_BYTES_PER_BLOCK) {
            make_color_block_gray(&pixel_data[offset + DXT5_BYTES_PER_BLOCK - DXT1_BYTES_PER_BLOCK]);
        }
    } else if (header->format == TEXTURE_FILE_FORMAT_DXT1) {
        for (offset = 0; offset < header->pixel_data_size; offset += DXT1_BYTES_PER_BLOCK) {
            make_dxt1_block_gray(&pixel_data[offset]);
        }
    } else {
        for (offset = 0; offset < header->pixel_data_size; offset += RGBA_BYTES_PER_PIXEL) {
            unsigned int gray;

            gray = luma_of(pixel_data[offset], pixel_data[offset + 1], pixel_data[offset + 2]);
            pixel_data[offset] = (unsigned char)gray;
            pixel_data[offset + 1] = (unsigned char)gray;
            pixel_data[offset + 2] = (unsigned char)gray;
        }
    }
}

static unsigned int bytes_per_packed_unit(unsigned int format)
{
    if (format == TEXTURE_FILE_FORMAT_DXT5) {
        return DXT5_BYTES_PER_BLOCK;
    }
    if (format == TEXTURE_FILE_FORMAT_DXT1) {
        return DXT1_BYTES_PER_BLOCK;
    }
    return RGBA_BYTES_PER_PIXEL;
}

int texture_file_is_packed(const unsigned char *data, unsigned int size)
{
    return size >= TEXTURE_FILE_HEADER_SIZE && memcmp(data, "GTEX", 4) == 0 && (read_u32_little_endian(&data[12]) & TEXTURE_FILE_PACKED_FLAG) != 0;
}

unsigned int texture_file_unpacked_size(const unsigned char *packed)
{
    return TEXTURE_FILE_HEADER_SIZE + read_u32_little_endian(&packed[16]);
}

static void copy_repeating_run(unsigned char *destination, const unsigned char *source, unsigned int distance, unsigned int bytes)
{
    unsigned int copied;

    copied = distance < bytes ? distance : bytes;
    memcpy(destination, source, copied);
    while (copied < bytes) {
        unsigned int chunk;

        chunk = copied < bytes - copied ? copied : bytes - copied;
        memcpy(&destination[copied], destination, chunk);
        copied = copied + chunk;
    }
}

int texture_file_unpack(const unsigned char *packed, unsigned int packed_size, unsigned char *unpacked, unsigned int unpacked_size)
{
    unsigned int format;
    unsigned int unit_size;
    unsigned int offset;
    unsigned int written;

    if (!texture_file_is_packed(packed, packed_size) || unpacked_size != texture_file_unpacked_size(packed)) {
        return 0;
    }
    format = read_u32_little_endian(&packed[12]) & ~TEXTURE_FILE_PACKED_FLAG;
    if (!format_is_known(format)) {
        return 0;
    }
    unit_size = bytes_per_packed_unit(format);
    memcpy(unpacked, packed, TEXTURE_FILE_HEADER_SIZE);
    write_u32_little_endian(&unpacked[12], format);
    offset = TEXTURE_FILE_HEADER_SIZE;
    written = TEXTURE_FILE_HEADER_SIZE;
    while (offset < packed_size) {
        unsigned int token;
        unsigned int bytes;

        if (packed_size - offset < 4u) {
            return 0;
        }
        token = read_u32_little_endian(&packed[offset]);
        offset = offset + 4u;
        bytes = (token & ~PACKED_MATCH_FLAG) * unit_size;
        if (bytes > unpacked_size - written) {
            return 0;
        }
        if (token & PACKED_MATCH_FLAG) {
            unsigned int source;

            if (packed_size - offset < 4u) {
                return 0;
            }
            source = read_u32_little_endian(&packed[offset]) * unit_size + TEXTURE_FILE_HEADER_SIZE;
            offset = offset + 4u;
            if (source >= written) {
                return 0;
            }
            copy_repeating_run(&unpacked[written], &unpacked[source], written - source, bytes);
        } else {
            if (bytes > packed_size - offset) {
                return 0;
            }
            memcpy(&unpacked[written], &packed[offset], bytes);
            offset = offset + bytes;
        }
        written = written + bytes;
    }
    return written == unpacked_size;
}

unsigned int texture_file_memory_bytes(const struct texture_file_header *header)
{
    struct texture_file_header allocated;

    if (header->format == TEXTURE_FILE_FORMAT_RGBA8888) {
        return header->pixel_data_size;
    }
    allocated = *header;
    allocated.width = round_up_to_power_of_two(header->width);
    allocated.height = round_up_to_power_of_two(header->height);
    return expected_pixel_data_size(&allocated);
}

int texture_file_read_header(const unsigned char *data, unsigned int size, struct texture_file_header *header)
{
    if (size < TEXTURE_FILE_HEADER_SIZE) {
        return TEXTURE_FILE_ERR_TOO_SHORT;
    }
    if (data[0] != 'G' || data[1] != 'T' || data[2] != 'E' || data[3] != 'X') {
        return TEXTURE_FILE_ERR_BAD_MAGIC;
    }
    header->width = read_u32_little_endian(&data[4]);
    header->height = read_u32_little_endian(&data[8]);
    header->format = read_u32_little_endian(&data[12]);
    header->pixel_data_size = read_u32_little_endian(&data[16]);
    if (!format_is_known(header->format)) {
        return TEXTURE_FILE_ERR_BAD_FORMAT;
    }
    if (header->width == 0 || header->height == 0) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (header->width > TEXTURE_FILE_MAX_DIMENSION || header->height > TEXTURE_FILE_MAX_DIMENSION) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (header->format != TEXTURE_FILE_FORMAT_RGBA8888 && !dimensions_are_multiples_of_block(header)) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (header->pixel_data_size != expected_pixel_data_size(header)) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (size - TEXTURE_FILE_HEADER_SIZE < header->pixel_data_size) {
        return TEXTURE_FILE_ERR_TOO_SHORT;
    }
    return TEXTURE_FILE_OK;
}
