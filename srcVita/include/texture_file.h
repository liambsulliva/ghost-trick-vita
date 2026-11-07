#ifndef TEXTURE_FILE_H
#define TEXTURE_FILE_H

#define TEXTURE_FILE_HEADER_SIZE 20
#define TEXTURE_FILE_PACKED_FLAG 0x100u
#define TEXTURE_FILE_MAX_DIMENSION 4096

#define TEXTURE_FILE_OK 0
#define TEXTURE_FILE_ERR_TOO_SHORT -1
#define TEXTURE_FILE_ERR_BAD_MAGIC -2
#define TEXTURE_FILE_ERR_BAD_FORMAT -3
#define TEXTURE_FILE_ERR_BAD_SIZE -4

enum texture_file_format {
    TEXTURE_FILE_FORMAT_RGBA8888 = 0,
    TEXTURE_FILE_FORMAT_DXT5 = 1,
    TEXTURE_FILE_FORMAT_DXT1 = 2
};

struct texture_file_header {
    unsigned int width;
    unsigned int height;
    unsigned int format;
    unsigned int pixel_data_size;
};

int texture_file_read_header(const unsigned char *data, unsigned int size, struct texture_file_header *header);
int texture_file_is_packed(const unsigned char *data, unsigned int size);
int texture_file_unpack(const unsigned char *packed, unsigned int packed_size, unsigned char *unpacked, unsigned int unpacked_size);
unsigned int texture_file_unpacked_size(const unsigned char *packed);
unsigned int texture_file_memory_bytes(const struct texture_file_header *header);
void texture_file_make_grayscale(const struct texture_file_header *header, unsigned char *pixel_data);

#endif
