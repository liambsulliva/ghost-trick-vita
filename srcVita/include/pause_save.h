#ifndef PAUSE_SAVE_H
#define PAUSE_SAVE_H

#define PAUSE_SAVE_STAYS 0
#define PAUSE_SAVE_LEAVES 1
#define PAUSE_SAVE_RESUMES 2
#define PAUSE_SAVE_AUTO_PANEL 0
#define PAUSE_SAVE_MANUAL_PANEL 1

typedef int (*pause_save_now_function)(void);
typedef int (*pause_slot_has_data_function)(int slot);

void pause_save_use(pause_save_now_function save_now, pause_slot_has_data_function slot_has_data);
int pause_save_now(void);
void pause_save_enter(void);
int pause_save_step(unsigned int pressed);
unsigned int pause_save_touch(int lands, int lifts, float x, float y);
int pause_save_selected(void);
int pause_save_dialog_is_open(void);
void pause_save_draw(float x_offset);
void pause_save_draw_bar(float x_offset);
void pause_save_draw_dialog(float x_offset);

#endif
