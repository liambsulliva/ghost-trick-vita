#include <stdlib.h>
#include <string.h>
#include "fake_platform.h"
#include "fake_texture_files.h"
#include "texture_file.h"

void fake_files_write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
    bytes[2] = (unsigned char)((value >> 16) & 0xff);
    bytes[3] = (unsigned char)((value >> 24) & 0xff);
}

static void add_texture(const char *path, unsigned int width, unsigned int height, unsigned int format, unsigned int pixel_bytes)
{
    unsigned char *file;

    file = calloc(1, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    memcpy(file, "GTEX", 4);
    fake_files_write_u32(&file[4], width);
    fake_files_write_u32(&file[8], height);
    fake_files_write_u32(&file[12], format);
    fake_files_write_u32(&file[16], pixel_bytes);
    fake_files_add(path, file, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    free(file);
}

void fake_files_add_rgba_texture(const char *path, unsigned int width, unsigned int height)
{
    add_texture(path, width, height, TEXTURE_FILE_FORMAT_RGBA8888, width * height * 4);
}

void fake_files_add_dxt5_texture(const char *path, unsigned int width, unsigned int height)
{
    add_texture(path, width, height, TEXTURE_FILE_FORMAT_DXT5, (width / 4) * (height / 4) * 16);
}
