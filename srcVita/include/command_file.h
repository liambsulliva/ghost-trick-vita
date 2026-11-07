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
    COMMAND_PLAY_SCRIPT,
    COMMAND_HUD_ON,
    COMMAND_HUD_OFF,
    COMMAND_MUSIC_STARVE,
    COMMAND_AUTO_PLAY,
    COMMAND_FAST_FORWARD,
    COMMAND_FAST_FORWARD_UNTIL,
    COMMAND_RESUME,
    COMMAND_SKIP_AHEAD,
    COMMAND_PRESS,
    COMMAND_BURST,
    COMMAND_TOUCH,
    COMMAND_DRAG,
    COMMAND_DRAG_SOUL,
    COMMAND_UNKNOWN
};

struct command {
    enum command_kind kind;
    char name[ASSET_NAME_MAX];
    char detail[16];
    int number;
    int touch_x;
    int touch_y;
    int touch_end_x;
    int touch_end_y;
    int delay_frames;
};

int command_name_is_safe(const char *name);
unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command);

#endif
