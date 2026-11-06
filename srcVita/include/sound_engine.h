#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#define SOUND_RATE 44100
#define SOUND_GRAIN_FRAMES 512
#define SOUND_VOICE_MAX 4
#define SOUND_CLIP_MAX 128
#define SOUND_START_QUEUE_MAX 8

void sound_engine_init(const char *sound_folder);
void sound_request_prepare(const char *name);
int sound_prepare_is_done(void);
void sound_request_play(const char *name);
void sound_request_stop(void);
void sound_shutdown(void);
int sound_engine_step(void);

#endif
