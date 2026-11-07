#ifndef SCRIPT_PLAYER_H
#define SCRIPT_PLAYER_H

#include "scene_player.h"


void script_player_init(const char *scripts_folder);
int script_player_is_available(void);
void script_player_play(const char *entry_name);
void script_player_update(unsigned long long elapsed_us);
void script_player_stop(void);
void script_player_set_paused(int should_pause);
int script_player_is_paused(void);
int script_player_is_active(void);
enum scene_playback_state script_player_status(void);
void script_player_skip_ahead(void);
int script_player_is_holding_for_art(void);
void script_player_press(void);
void script_player_ghost_button(void);
void script_player_touch(int is_down, float x, float y);
void script_player_back_button(void);
void script_player_hourglass_button(void);
void script_player_notebook_button(void);
void script_player_tap_balloon(int is_right);
void script_player_direction(int direction_x, int direction_y);
void script_player_held_direction(int held_x, int held_y);
void script_player_set_auto_play(int is_on);
void script_player_set_fast_forward(int ticks_per_frame);
void script_player_run_until(const char *kind, const char *name);
void script_player_note_mark(const char *what, const char *name);
unsigned int script_player_tick(void);
void script_player_shutdown(void);

#endif
