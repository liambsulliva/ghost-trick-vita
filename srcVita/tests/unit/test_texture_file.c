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

int main(void)
{
    test_texture_header_accepts_a_valid_file();
    test_texture_header_rejects_bad_files();
    test_texture_header_checks_compressed_sizes();
    test_compressed_memory_is_charged_at_power_of_two_size();
    fake_files_reset();
    return test_finish("unit/texture_file");
}
