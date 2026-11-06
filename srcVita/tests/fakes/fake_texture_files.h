#ifndef FAKE_TEXTURE_FILES_H
#define FAKE_TEXTURE_FILES_H

void fake_files_write_u32(unsigned char *bytes, unsigned int value);
void fake_files_add_rgba_texture(const char *path, unsigned int width, unsigned int height);
void fake_files_add_dxt5_texture(const char *path, unsigned int width, unsigned int height);

#endif
