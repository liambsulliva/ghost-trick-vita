#ifndef MUSIC_ENGINE_H
#define MUSIC_ENGINE_H

#include "asset_name.h"

#define MUSIC_VOLUME_FULL 256

#define MUSIC_RATE 44100
#define MUSIC_GRAIN_FRAMES 4096
#define MUSIC_QUICK_FADE_TICKS 5
#define MUSIC_STARVE_MS_MAX 5000

struct music_memory {
    char track_name[ASSET_NAME_MAX];
    int should_play;
    int is_looping;
};

void music_engine_init(const char *music_folder);
void music_engine_use_folder(const char *music_folder);
void music_request_prepare(const char *name);
int music_prepare_is_done(const char *name);
void music_request_play(const char *name, int is_looping);
void music_request_play_fading_in(const char *name, int is_looping, int fade_in_ticks);
void music_request_stop(int fade_ticks);
void music_request_volume(int volume_out_of_256, int ticks);
void music_request_starve(int milliseconds);
void music_remember(struct music_memory *memory);
void music_restore(const struct music_memory *memory, int fade_ticks);
void music_shutdown(void);

#endif
