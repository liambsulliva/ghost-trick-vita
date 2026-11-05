#include "texture_file.h"

static unsigned int read_u32_little_endian(const unsigned char *bytes)
{
    unsigned int value;

    value = bytes[0];
    value = value | ((unsigned int)bytes[1] << 8);
    value = value | ((unsigned int)bytes[2] << 16);
    value = value | ((unsigned int)bytes[3] << 24);
    return value;
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
    if (header->format != TEXTURE_FILE_FORMAT_RGBA8888) {
        return TEXTURE_FILE_ERR_BAD_FORMAT;
    }
    if (header->width == 0 || header->height == 0) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (header->width > TEXTURE_FILE_MAX_DIMENSION || header->height > TEXTURE_FILE_MAX_DIMENSION) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (header->pixel_data_size != header->width * header->height * 4) {
        return TEXTURE_FILE_ERR_BAD_SIZE;
    }
    if (size - TEXTURE_FILE_HEADER_SIZE < header->pixel_data_size) {
        return TEXTURE_FILE_ERR_TOO_SHORT;
    }
    return TEXTURE_FILE_OK;
}
