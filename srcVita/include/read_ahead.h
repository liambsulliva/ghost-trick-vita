#ifndef READ_AHEAD_H
#define READ_AHEAD_H

#define READ_AHEAD_ENTRY_MAX 1536
#define READ_AHEAD_LOOKAHEAD_TICKS 240
#define READ_AHEAD_KEEP_BEHIND_TICKS 30

void read_ahead_clear(void);
int read_ahead_add_text(const char *script_name, const char *text, unsigned int text_size);
void read_ahead_note_mark(const char *what, const char *name, unsigned int tick);
int read_ahead_step(unsigned int tick);
void read_ahead_set_lookahead_scale(int scale);

#endif
