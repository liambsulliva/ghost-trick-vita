#ifndef PAUSE_CHAPTERS_H
#define PAUSE_CHAPTERS_H

#include "game_save.h"

#define PAUSE_CHAPTERS_STAYS 0
#define PAUSE_CHAPTERS_LEAVES 1
#define PAUSE_CHAPTERS_PLAYS 2
#define PAUSE_CHAPTERS_MOST GAME_SAVE_CHAPTER_COUNT

typedef int (*pause_chapter_reached_function)(int chapter);

void pause_chapters_use(pause_chapter_reached_function chapter_is_reached);
void pause_chapters_enter(const char *warning_key);
int pause_chapters_step(unsigned int pressed);
unsigned int pause_chapters_touch(int lands, int lifts, float x, float y);
int pause_chapters_selected(void);
int pause_chapters_dialog_is_open(void);
void pause_chapters_draw(float x_offset);
void pause_chapters_draw_bar(float x_offset);
void pause_chapters_draw_dialog(float x_offset);

#endif
