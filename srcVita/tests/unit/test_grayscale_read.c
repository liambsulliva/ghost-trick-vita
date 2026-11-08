#include <string.h>
#include "background_reader.h"
#include "fakes/fake_texture_files.h"
#include "test_support.h"
#include "texture_file.h"

static void test_a_finished_read_turns_gray_and_keeps_its_alpha(void)
{
    unsigned char file[TEXTURE_FILE_HEADER_SIZE + 4];
    struct finished_read finished;

    memset(file, 0, sizeof(file));
    memcpy(file, "GTEX", 4);
    fake_files_write_u32(&file[4], 1);
    fake_files_write_u32(&file[8], 1);
    fake_files_write_u32(&file[12], TEXTURE_FILE_FORMAT_RGBA8888);
    fake_files_write_u32(&file[16], 4);
    file[TEXTURE_FILE_HEADER_SIZE] = 240;
    file[TEXTURE_FILE_HEADER_SIZE + 1] = 30;
    file[TEXTURE_FILE_HEADER_SIZE + 2] = 30;
    file[TEXTURE_FILE_HEADER_SIZE + 3] = 77;
    memset(&finished, 0, sizeof(finished));
    finished.data = file;
    finished.size = sizeof(file);
    background_reader_make_grayscale(&finished);
    CHECK_INT(file[TEXTURE_FILE_HEADER_SIZE], file[TEXTURE_FILE_HEADER_SIZE + 1]);
    CHECK_INT(file[TEXTURE_FILE_HEADER_SIZE + 1], file[TEXTURE_FILE_HEADER_SIZE + 2]);
    CHECK_INT(file[TEXTURE_FILE_HEADER_SIZE + 3], 77);
}

static void test_a_failed_read_is_left_alone(void)
{
    struct finished_read finished;

    memset(&finished, 0, sizeof(finished));
    background_reader_make_grayscale(&finished);
    CHECK(finished.data == NULL);
}

int main(void)
{
    test_a_finished_read_turns_gray_and_keeps_its_alpha();
    test_a_failed_read_is_left_alone();
    return test_finish("unit/grayscale_read");
}
