#ifndef SCRIPT_PLAYER_H
#define SCRIPT_PLAYER_H

#include "input.h"
#include "loading_screen.h"
#include "scene_player.h"


void script_player_init(const char *scripts_folder);
int script_player_is_available(void);
void script_player_play(const char *entry_name);
void script_player_update(unsigned long long elapsed_us);
void script_player_stop(void);
void script_player_set_paused(int should_pause);
int script_player_is_paused(void);
int script_player_is_active(void);
const unsigned char *script_player_note_levels(void);
int script_player_save_manual(void);
int script_player_takes_new_chapter(void);
int script_player_takes_record_request(void);
struct game_save;
void script_player_resume(const struct game_save *save);
int script_player_is_showing_next_chapter_card(void);
enum scene_playback_state script_player_status(void);
void script_player_skip_ahead(void);
int script_player_is_holding_for_art(void);
void script_player_load_counts(struct loading_counts *counts);
void script_player_input_frame(const struct input_state *frame);
void script_player_set_auto_play(int is_on);
void script_player_set_stall_breaker(int is_on);
void script_player_set_fast_forward(int ticks_per_frame);
void script_player_run_until(const char *kind, const char *name);
void script_player_note_mark(const char *what, const char *name);
unsigned int script_player_tick(void);
int script_player_acted_route_step(void);
unsigned int script_player_route_action_began_at(void);
void script_player_shutdown(void);

#endif
