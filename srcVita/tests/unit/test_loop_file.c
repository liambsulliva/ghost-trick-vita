#include <string.h>
#include "fakes/fake_platform.h"
#include "loop_file.h"
#include "test_support.h"

#define FRAMES 1000

static int read_this(const char *text, unsigned int *loop_start, unsigned int *loop_last)
{
    int result;

    fake_files_reset();
    fake_memory_reset();
    if (text != NULL) {
        fake_files_add("sounds/hum.loop", (const unsigned char *)text, (unsigned int)strlen(text));
    }
    *loop_start = 7;
    *loop_last = 7;
    result = loop_file_read("sounds", "hum", FRAMES, loop_start, loop_last);
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    return result;
}

static void test_a_sound_with_no_loop_file_has_no_loop(void)
{
    unsigned int loop_start;
    unsigned int loop_last;

    CHECK_INT(read_this(NULL, &loop_start, &loop_last), LOOP_FILE_NONE);
}

static void test_a_loop_may_end_on_the_last_frame(void)
{
    unsigned int loop_start;
    unsigned int loop_last;

    CHECK_INT(read_this("2 999\n", &loop_start, &loop_last), LOOP_FILE_OK);
    CHECK_INT(loop_start, 2);
    CHECK_INT(loop_last, FRAMES - 1);
    CHECK_INT(read_this("0 0\n", &loop_start, &loop_last), LOOP_FILE_OK);
}

static void test_a_loop_at_or_past_the_frame_count_is_bad_and_leaves_the_points_alone(void)
{
    static const char *const bad_files[] = { "2 1000\n", "2 1001\n", "0 4000000000\n", "1000 1000\n", "500 499\n", "words\n", "5\n" };
    unsigned int loop_start;
    unsigned int loop_last;
    unsigned int index;

    for (index = 0; index < sizeof(bad_files) / sizeof(bad_files[0]); index++) {
        CHECK_INT(read_this(bad_files[index], &loop_start, &loop_last), LOOP_FILE_BAD);
        CHECK_INT(loop_start, 7);
        CHECK_INT(loop_last, 7);
    }
}

int main(void)
{
    test_a_sound_with_no_loop_file_has_no_loop();
    test_a_loop_may_end_on_the_last_frame();
    test_a_loop_at_or_past_the_frame_count_is_bad_and_leaves_the_points_alone();
    return test_finish("unit/loop_file");
}
