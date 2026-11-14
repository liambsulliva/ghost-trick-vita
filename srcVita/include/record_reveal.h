#ifndef RECORD_REVEAL_H
#define RECORD_REVEAL_H

#include "file.h"

#define RECORD_REVEAL_PATH FILE_DATA_FOLDER "/record_reveal.txt"
#define RECORD_REVEAL_RULES_MOST 8

void record_reveal_forget_rules(void);
int record_reveal_read_rule(const char *text);
int record_reveal_load(const char *path);
int record_reveal_stage(void);
int record_reveal_watch(const char *talk_label, const unsigned char *note_levels, const char *path);
void record_reveal_forget(const char *path);

#endif
