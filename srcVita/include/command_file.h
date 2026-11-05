#ifndef COMMAND_FILE_H
#define COMMAND_FILE_H

#define COMMAND_NAME_MAX 64

enum command_kind {
    COMMAND_NONE,
    COMMAND_CAPTURE,
    COMMAND_QUIT,
    COMMAND_UNKNOWN
};

struct command {
    enum command_kind kind;
    char name[COMMAND_NAME_MAX];
};

int command_name_is_safe(const char *name);
unsigned int command_parse_next_line(const char *text, unsigned int text_size, unsigned int offset, struct command *command);

#endif
