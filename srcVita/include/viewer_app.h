#ifndef VIEWER_APP_H
#define VIEWER_APP_H

#include "input.h"

#define VIEWER_STATUS_TEXT_MAX 128

void viewer_app_start(const char *folder);
void viewer_app_update(const struct input_state *input, unsigned long long elapsed_us);
void viewer_app_draw(void);
void viewer_app_stop(void);
void viewer_app_set_status_text(const char *text);
void viewer_app_set_hud_visible(int is_visible);
int viewer_app_show_background(const char *name);
int viewer_app_show_animation(const char *name);
int viewer_app_play_scene(const char *name);
int viewer_app_play_script(const char *name);
int viewer_app_has_assets(void);

#endif
