#include <string.h>
#include "command_file.h"

#define COMMAND_LINE_MAX 128

static int character_is_safe_in_name(char character)
{
    if (character >= 'a' && character <= 'z') {
        return 1;
    }
    if (character >= 'A' && character <= 'Z') {
        return 1;
    }
    if (character >= '0' && character <= '9') {
        return 1;
    }
    return character == '_' || character == '-';
}

int command_name_is_safe(const char *name)
{
    unsigned int index;

    if (name[0] == '\0') {
        return 0;
    }
    for (index = 0; name[index] != '\0'; index++) {
        if (index >= COMMAND_NAME_MAX - 1) {
            return 0;
        }
        if (!character_is_safe_in_name(name[index])) {
            return 0;
        }
    }
    return 1;
}

static unsigned int copy_one_line(const char *text, unsigned int text_size, unsigned int offset, char *line)
{
    unsigned int length;

    length = 0;
    while (offset < text_size && text[offset] != '\n') {
        if (text[offset] != '\r' && length < COMMAND_LINE_MAX - 1) {
            line[length] = text[offset];
            length = length + 1;
        }
        offset = offset + 1;
    }
    line[length] = '\0';
    if (offset < text_size) {
        offset = offset + 1;
    }
    return offset;
}

unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command)
{
    static const char capture_prefix[] = "capture ";
    char line[COMMAND_LINE_MAX];
    unsigned int prefix_length;

    command->kind = COMMAND_NONE;
    command->name[0] = '\0';
    if (offset >= text_size) {
        return offset;
    }
    offset = copy_one_line(text, text_size, offset, line);
    prefix_length = sizeof(capture_prefix) - 1;
    if (line[0] == '\0') {
        return offset;
    }
    if (strcmp(line, "quit") == 0) {
        command->kind = COMMAND_QUIT;
        return offset;
    }
    if (strncmp(line, capture_prefix, prefix_length) == 0 && command_name_is_safe(&line[prefix_length])) {
        command->kind = COMMAND_CAPTURE;
        strcpy(command->name, &line[prefix_length]);
        return offset;
    }
    command->kind = COMMAND_UNKNOWN;
    return offset;
}
