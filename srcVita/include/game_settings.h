#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include "file.h"

#define GAME_SETTING_STEPS 10
#define GAME_FPS_FULL 60
#define GAME_FPS_CAPPED 30
#define GAME_SETTINGS_TEXT_MAX 256
#define GAME_SETTINGS_PATH FILE_DATA_FOLDER "/settings.txt"

enum game_visual_filter {
    GAME_FILTER_REMASTERED,
    GAME_FILTER_DS
};

enum game_text_speed {
    GAME_TEXT_SLOW,
    GAME_TEXT_NORMAL,
    GAME_TEXT_FAST,
    GAME_TEXT_SPEED_COUNT
};

enum game_music_type {
    GAME_MUSIC_ORIGINAL,
    GAME_MUSIC_ARRANGED
};

struct game_settings {
    int brightness;
    int master;
    int music;
    int effects;
    int music_type;
    int language;
    int unlock_all;
    int fps_cap;
    int high_contrast;
    int visual_filter;
    int text_speed;
};

void game_settings_defaults(struct game_settings *settings);
void game_settings_set_default_music(int music_type);
void game_settings_set_music_folders(const char *original_folder, const char *arranged_folder);
int game_settings_read_text(struct game_settings *settings, const char *text, unsigned int text_size);
unsigned int game_settings_write_text(const struct game_settings *settings, char *text, unsigned int text_size);
struct game_settings *game_settings_now(void);
void game_settings_load(const char *path);
int game_settings_save(const char *path);
void game_settings_apply(void);
int game_settings_volume_gain(int level);
float game_settings_darkness(void);
void game_settings_set_bundled_art(int has_remastered, int has_ds);
int game_settings_filter_is_pinned(void);
int game_settings_visual_filter(void);

#endif
