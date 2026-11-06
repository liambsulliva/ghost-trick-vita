#ifndef SCENE_PLAYER_H
#define SCENE_PLAYER_H

enum scene_playback_state {
    SCENE_PLAYER_IDLE,
    SCENE_PLAYER_PRELOADING,
    SCENE_PLAYER_RUNNING,
    SCENE_PLAYER_FINISHED
};

void scene_player_init(const char *scene_folder);
int scene_player_play(const char *scene_name);
void scene_player_update(unsigned long long elapsed_us);
void scene_player_stop(void);
void scene_player_skip_hold(void);
void scene_player_set_paused(int should_pause);
int scene_player_is_paused(void);
enum scene_playback_state scene_player_status(void);
int scene_player_next_step(void);
void scene_player_shutdown(void);

#endif
