#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H

#define BMP_HEADER_SIZE 54
#define BMP_BYTES_PER_PIXEL 3

unsigned int bmp_row_size(unsigned int width);
unsigned int bmp_file_size(unsigned int width, unsigned int height);
void bmp_write_header(unsigned char *header, unsigned int width, unsigned int height);
void bmp_write_row_from_rgba(unsigned char *row, const unsigned char *rgba_pixels, unsigned int width);

#endif
