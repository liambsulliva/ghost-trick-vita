#ifndef SCRIPT_PLAYER_H
#define SCRIPT_PLAYER_H

#include "scene_player.h"

#define SCRIPT_PLAYER_LINE_MAX 160

void script_player_init(const char *scripts_folder);
int script_player_is_available(void);
void script_player_play(const char *entry_name);
void script_player_update(unsigned long long elapsed_us);
void script_player_stop(void);
void script_player_set_paused(int should_pause);
int script_player_is_paused(void);
int script_player_is_active(void);
enum scene_playback_state script_player_status(void);
const char *script_player_halt_line(void);
unsigned int script_player_tick(void);
void script_player_shutdown(void);

#endif
