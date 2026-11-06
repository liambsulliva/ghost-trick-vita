#include <stdio.h>
#include <string.h>
#include "command_file.h"
#include "music_engine.h"
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
        if (index >= ASSET_NAME_MAX - 1) {
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

static int read_music_starve_command(const char *line, struct command *command)
{
    char extra;

    if (sscanf(line, "music_starve %d%c", &command->number, &extra) != 1) {
        return 0;
    }
    if (command->number <= 0 || command->number > MUSIC_STARVE_MS_MAX) {
        return 0;
    }
    command->kind = COMMAND_MUSIC_STARVE;
    return 1;
}

#ifdef FAST_FORWARD_BUILD
static int read_fast_forward_command(const char *line, struct command *command)
{
    char kind[16];
    char name[ASSET_NAME_MAX];

    if (strcmp(line, "auto_play") == 0 || strcmp(line, "auto_play off") == 0) {
        command->kind = COMMAND_AUTO_PLAY;
        command->number = strcmp(line, "auto_play") == 0;
        return 1;
    }
    if (sscanf(line, "ff %d", &command->number) == 1) {
        command->kind = COMMAND_FAST_FORWARD;
        return 1;
    }
    if (sscanf(line, "ff_until %15s %31s", kind, name) == 2) {
        command->kind = COMMAND_FAST_FORWARD_UNTIL;
        snprintf(command->name, sizeof(command->name), "%s", name);
        snprintf(command->detail, sizeof(command->detail), "%s", kind);
        return 1;
    }
    if (strcmp(line, "resume") == 0) {
        command->kind = COMMAND_RESUME;
        return 1;
    }
    if (strcmp(line, "skip") == 0) {
        command->kind = COMMAND_SKIP_AHEAD;
        return 1;
    }
    if (strcmp(line, "burst on") == 0 || strcmp(line, "burst off") == 0) {
        command->kind = COMMAND_BURST;
        command->number = strcmp(line, "burst on") == 0;
        return 1;
    }
    if (sscanf(line, "press %x", (unsigned int *)&command->number) == 1) {
        command->kind = COMMAND_PRESS;
        return 1;
    }
    return 0;
}
#endif

unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command)
{
    char line[COMMAND_LINE_MAX];

    command->kind = COMMAND_NONE;
    command->name[0] = '\0';
    command->number = 0;
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
    if (read_named_command(line, "play_script ", COMMAND_PLAY_SCRIPT, command)) {
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
    if (read_music_starve_command(line, command)) {
        return offset;
    }
#ifdef FAST_FORWARD_BUILD
    if (read_fast_forward_command(line, command)) {
        return offset;
    }
#endif
    command->kind = COMMAND_UNKNOWN;
    return offset;
}
