#include <string.h>
#include "command_file.h"
#include "text_lines.h"

#define COMMAND_LINE_MAX 160

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

static int read_named_command(const char *line, const char *prefix, enum command_kind kind, struct command *command)
{
    unsigned int prefix_length;

    prefix_length = (unsigned int)strlen(prefix);
    if (strncmp(line, prefix, prefix_length) != 0) {
        return 0;
    }
    if (!command_name_is_safe(&line[prefix_length])) {
        return 0;
    }
    command->kind = kind;
    strcpy(command->name, &line[prefix_length]);
    return 1;
}

unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command)
{
    char line[COMMAND_LINE_MAX];

    command->kind = COMMAND_NONE;
    command->name[0] = '\0';
    if (offset >= text_size) {
        return offset;
    }
    offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
    if (line[0] == '\0') {
        return offset;
    }
    if (strcmp(line, "quit") == 0) {
        command->kind = COMMAND_QUIT;
        return offset;
    }
    if (strcmp(line, "hud on") == 0) {
        command->kind = COMMAND_HUD_ON;
        return offset;
    }
    if (strcmp(line, "hud off") == 0) {
        command->kind = COMMAND_HUD_OFF;
        return offset;
    }
    if (read_named_command(line, "play_scene ", COMMAND_PLAY_SCENE, command)) {
        return offset;
    }
    if (read_named_command(line, "capture ", COMMAND_CAPTURE, command)) {
        return offset;
    }
    if (read_named_command(line, "show_background ", COMMAND_SHOW_BACKGROUND, command)) {
        return offset;
    }
    if (read_named_command(line, "show_animation ", COMMAND_SHOW_ANIMATION, command)) {
        return offset;
    }
    command->kind = COMMAND_UNKNOWN;
    return offset;
}
