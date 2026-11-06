#include "command_file.h"
#include "test_support.h"

static void test_command_names_only_allow_plain_characters(void)
{
    char too_long[ASSET_NAME_MAX + 2];

    CHECK(command_name_is_safe("first_frames-2"));
    CHECK(!command_name_is_safe(""));
    CHECK(!command_name_is_safe("../secret"));
    CHECK(!command_name_is_safe("a b"));
    memset(too_long, 'a', sizeof(too_long) - 1);
    too_long[sizeof(too_long) - 1] = '\0';
    CHECK(!command_name_is_safe(too_long));
}

static void test_command_file_parses_each_line_in_order(void)
{
    const char text[] = "capture one\r\n\nquit\nbogus\ncapture ../two\nshow_background room\nshow_animation walk_1\nplay_scene opening\nhud on\nhud off";
    unsigned int size;
    unsigned int offset;
    struct command command;

    size = sizeof(text) - 1;
    offset = command_parse_next_line(text, size, 0, &command);
    CHECK_INT(command.kind, COMMAND_CAPTURE);
    CHECK_TEXT(command.name, "one");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_NONE);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_QUIT);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_UNKNOWN);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_UNKNOWN);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_SHOW_BACKGROUND);
    CHECK_TEXT(command.name, "room");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_SHOW_ANIMATION);
    CHECK_TEXT(command.name, "walk_1");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_PLAY_SCENE);
    CHECK_TEXT(command.name, "opening");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_HUD_ON);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_HUD_OFF);
    CHECK_INT(offset, size);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_NONE);
    CHECK_INT(offset, size);
}

int main(void)
{
    test_command_names_only_allow_plain_characters();
    test_command_file_parses_each_line_in_order();
    return test_finish("unit/command_file");
}
