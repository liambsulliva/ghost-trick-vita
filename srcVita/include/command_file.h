#ifndef COMMAND_FILE_H
#define COMMAND_FILE_H

#include "asset_name.h"

enum command_kind {
    COMMAND_NONE,
    COMMAND_CAPTURE,
    COMMAND_QUIT,
    COMMAND_SHOW_BACKGROUND,
    COMMAND_SHOW_ANIMATION,
    COMMAND_PLAY_SCENE,
    COMMAND_HUD_ON,
    COMMAND_HUD_OFF,
    COMMAND_MUSIC_STARVE,
    COMMAND_UNKNOWN
};

struct command {
    enum command_kind kind;
    char name[ASSET_NAME_MAX];
    int number;
};

int command_name_is_safe(const char *name);
unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command);

#endif
