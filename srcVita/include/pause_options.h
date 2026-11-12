#ifndef PAUSE_OPTIONS_H
#define PAUSE_OPTIONS_H

#define PAUSE_OPTIONS_STAYS 0
#define PAUSE_OPTIONS_LEAVES 1

void pause_options_show_unlock(int is_shown);
void pause_options_enter(void);
int pause_options_step(unsigned int pressed);
unsigned int pause_options_touch(int lands, int lifts, int is_down, float x, float y);
void pause_options_tick(void);
void pause_options_leave(void);
int pause_options_selected_row(void);
int pause_options_is_on_controls_page(void);
int pause_options_dropdown_is_open(void);
int pause_options_dialog_is_open(void);
void pause_options_draw(float x_offset);
void pause_options_draw_bar(float x_offset);
void pause_options_draw_dialog(float x_offset);

#endif
