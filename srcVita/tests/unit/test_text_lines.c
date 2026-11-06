#include "test_support.h"
#include "text_lines.h"

static void test_text_lines_handle_endings_and_long_lines(void)
{
    const char text[] = "one\r\ntwo\n\nabcdefghij";
    char line[6];
    unsigned int offset;

    offset = text_lines_read_next(text, sizeof(text) - 1, 0, line, sizeof(line));
    CHECK_TEXT(line, "one");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "two");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "abcde");
    CHECK_INT(offset, sizeof(text) - 1);
}

static void test_text_line_starts_with_compares_only_the_start(void)
{
    CHECK(text_line_starts_with("frame a 1", "frame "));
    CHECK(!text_line_starts_with("frames a 1", "frame "));
    CHECK(!text_line_starts_with("fra", "frame "));
}

int main(void)
{
    test_text_lines_handle_endings_and_long_lines();
    test_text_line_starts_with_compares_only_the_start();
    return test_finish("unit/text_lines");
}
