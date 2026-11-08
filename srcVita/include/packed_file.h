#ifndef PACKED_FILE_H
#define PACKED_FILE_H

#define PACKED_FILE_HEADER_SIZE 8u

int packed_file_is_packed(const unsigned char *bytes, unsigned int size);
unsigned int packed_file_unpacked_size(const unsigned char *bytes);
int packed_file_unpack(const unsigned char *bytes, unsigned int size, unsigned char *unpacked, unsigned int unpacked_size);
int packed_file_inflate(const unsigned char *input, unsigned int input_size, unsigned char *output, unsigned int output_size);

#endif
