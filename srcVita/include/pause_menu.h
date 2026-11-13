#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "input.h"

#define PAUSE_MENU_ITEM_COUNT 6
#define PAUSE_MENU_ROWS 3
#define PAUSE_CAT_FRAMES_MOST 512
#define PAUSE_CAT_TICKS_MOST 1024
#define PAUSE_CAT_PAGES_MOST 16
#define PAUSE_TICK_US 16667ull
#define PAUSE_TICKS_PER_UPDATE_MOST 4

enum pause_menu_item {
    PAUSE_ITEM_RESUME,
    PAUSE_ITEM_SAVE,
    PAUSE_ITEM_PEOPLE,
    PAUSE_ITEM_TITLE,
    PAUSE_ITEM_CHAPTERS,
    PAUSE_ITEM_OPTIONS
};

enum pause_menu_phase {
    PAUSE_PHASE_CLOSED,
    PAUSE_PHASE_DARKENING,
    PAUSE_PHASE_WAITING_FOR_ART,
    PAUSE_PHASE_SHOWING,
    PAUSE_PHASE_PANNING_OUT,
    PAUSE_PHASE_SUBMENU,
    PAUSE_PHASE_PANNING_BACK,
    PAUSE_PHASE_HIDING,
    PAUSE_PHASE_BLACK_BEFORE_GAME,
    PAUSE_PHASE_BRIGHTENING
};

struct pause_cat_frame {
    int page;
    float x;
    float y;
    float width;
    float height;
    int left;
    int top;
};

struct pause_cat_step {
    int first;
    int second;
    float weight;
};

struct pause_label {
    float x;
    float y;
    float width;
    float height;
};

int pause_menu_read_cat_text(const char *text, unsigned int text_size);
int pause_menu_read_track_text(const char *text, unsigned int text_size);
int pause_menu_read_label_text(const char *text, unsigned int text_size);
int pause_menu_load(const char *assets_folder);
int pause_menu_is_ready(void);

void pause_cat_track_restart(void);
struct pause_cat_step pause_cat_track_next(void);
const struct pause_cat_frame *pause_cat_frame_at(int row);
int pause_cat_page_count(void);

void pause_menu_open(void);
int pause_menu_is_open(void);
int pause_menu_takes_title_request(void);
int pause_menu_takes_chapter_request(void);
int pause_menu_shows_game(void);
void pause_menu_update(const struct input_state *input, unsigned long long elapsed_us);
void pause_menu_step(unsigned int pressed);
enum pause_menu_phase pause_menu_phase(void);
int pause_menu_cursor(void);
float pause_menu_camera_x(void);
float pause_menu_label_alpha(void);
float pause_menu_black_alpha(void);
void pause_menu_draw(void);

#endif
