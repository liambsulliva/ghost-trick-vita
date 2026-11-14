#ifndef TITLE_MENU_H
#define TITLE_MENU_H

#include "input.h"

#define TITLE_LOGO_NAME "title_logo"
#define TITLE_HUD_ART_NAME "hud_button_art"

enum title_menu_item {
    TITLE_ITEM_CHAPTERS,
    TITLE_ITEM_CONTINUE,
    TITLE_ITEM_ERASE,
    TITLE_ITEM_LANGUAGE,
    TITLE_ITEM_RECORD,
    TITLE_ITEM_NEW_GAME,
    TITLE_ITEM_OPTIONS,
    TITLE_MENU_ITEM_COUNT
};

enum title_menu_phase {
    TITLE_PHASE_CLOSED,
    TITLE_PHASE_WAITING_FOR_ART,
    TITLE_PHASE_SHOWING,
    TITLE_PHASE_PANNING_OUT,
    TITLE_PHASE_SUBMENU,
    TITLE_PHASE_PANNING_BACK,
    TITLE_PHASE_HIDING,
    TITLE_PHASE_LEAVING
};

enum title_menu_request {
    TITLE_REQUEST_NONE,
    TITLE_REQUEST_CONTINUE,
    TITLE_REQUEST_NEW_GAME,
    TITLE_REQUEST_CHAPTER
};

typedef int (*title_save_check_function)(void);

void title_menu_use(title_save_check_function has_save, title_save_check_function is_cleared, title_save_check_function erase_saves);
int title_menu_is_ready(void);
void title_menu_open(void);
void title_menu_close(void);
int title_menu_is_open(void);
int title_menu_shows(int item);
int title_menu_cursor(void);
enum title_menu_phase title_menu_phase(void);
int title_menu_dialog_is_open(void);
float title_menu_camera_x(void);
float title_menu_cover_alpha(void);
void title_menu_step(unsigned int pressed);
void title_menu_update(const struct input_state *input, unsigned long long elapsed_us);
enum title_menu_request title_menu_takes_request(int *chapter);
void title_menu_draw(void);

#endif
