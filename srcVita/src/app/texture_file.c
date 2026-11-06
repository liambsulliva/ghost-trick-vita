#include "texture_file.h"

#define DXT_BLOCK_SIZE_IN_PIXELS 4
#define DXT5_BYTES_PER_BLOCK 16
#define DXT1_BYTES_PER_BLOCK 8

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
