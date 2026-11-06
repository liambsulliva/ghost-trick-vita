#ifndef MUSIC_ENGINE_H
#define MUSIC_ENGINE_H

#define MUSIC_RATE 44100
#define MUSIC_GRAIN_FRAMES 4096
#define MUSIC_QUICK_FADE_TICKS 5
#define MUSIC_STARVE_MS_MAX 5000

void music_engine_init(const char *music_folder);
void music_request_prepare(const char *name);
int music_prepare_is_done(const char *name);
void music_request_play(const char *name, int is_looping);
void music_request_stop(int fade_ticks);
void music_request_starve(int milliseconds);
void music_shutdown(void);
int music_engine_step(void);

#endif
