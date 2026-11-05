#include "bmp_image.h"

#define BMP_INFO_HEADER_SIZE 40
#define BMP_PLANE_COUNT 1
#define BMP_BITS_PER_PIXEL 24

static void write_u16_little_endian(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
}

static void write_u32_little_endian(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
    bytes[2] = (unsigned char)((value >> 16) & 0xff);
    bytes[3] = (unsigned char)((value >> 24) & 0xff);
}

unsigned int bmp_row_size(unsigned int width)
{
    unsigned int unpadded_size;

    unpadded_size = width * BMP_BYTES_PER_PIXEL;
    return (unpadded_size + 3) / 4 * 4;
}

unsigned int bmp_file_size(unsigned int width, unsigned int height)
{
    return BMP_HEADER_SIZE + bmp_row_size(width) * height;
}

void bmp_write_header(unsigned char *header, unsigned int width, unsigned int height)
{
    unsigned int index;

    for (index = 0; index < BMP_HEADER_SIZE; index++) {
        header[index] = 0;
    }
    header[0] = 'B';
    header[1] = 'M';
    write_u32_little_endian(&header[2], bmp_file_size(width, height));
    write_u32_little_endian(&header[10], BMP_HEADER_SIZE);
    write_u32_little_endian(&header[14], BMP_INFO_HEADER_SIZE);
    write_u32_little_endian(&header[18], width);
    write_u32_little_endian(&header[22], height);
    write_u16_little_endian(&header[26], BMP_PLANE_COUNT);
    write_u16_little_endian(&header[28], BMP_BITS_PER_PIXEL);
    write_u32_little_endian(&header[34], bmp_row_size(width) * height);
}

void bmp_write_row_from_rgba(unsigned char *row, const unsigned char *rgba_pixels, unsigned int width)
{
    unsigned int pixel;
    unsigned int padded_size;
    unsigned int index;

    for (pixel = 0; pixel < width; pixel++) {
        row[pixel * 3] = rgba_pixels[pixel * 4 + 2];
        row[pixel * 3 + 1] = rgba_pixels[pixel * 4 + 1];
        row[pixel * 3 + 2] = rgba_pixels[pixel * 4];
    }
    padded_size = bmp_row_size(width);
    for (index = width * BMP_BYTES_PER_PIXEL; index < padded_size; index++) {
        row[index] = 0;
    }
}
