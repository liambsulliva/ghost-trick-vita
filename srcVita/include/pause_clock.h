#ifndef PAUSE_CLOCK_H
#define PAUSE_CLOCK_H

#define PAUSE_CLOCK_NO_TIME (-1)
#define PAUSE_CLOCK_TEXT_MAX 16

int pause_clock_read_text(const char *text, unsigned int text_size);
int pause_clock_load(const char *assets_folder);
void pause_clock_note_scene(int chapter_mark, const char *script);
void pause_clock_note_plate(const char *plate_name);
int pause_clock_minutes(void);
const char *pause_clock_place(void);
void pause_clock_time_text(int minutes, char *text, unsigned int text_size);
void pause_clock_draw(float alpha);

#endif
