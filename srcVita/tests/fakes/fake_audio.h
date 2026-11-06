#ifndef FAKE_AUDIO_H
#define FAKE_AUDIO_H

#include "audio_port.h"
#include "audio_thread.h"

#define FAKE_AUDIO_PORT_RUNNING 0
#define FAKE_AUDIO_STEP_LIMIT 100000
#define FAKE_AUDIO_MUSIC_SHORTEST_BLOCK_US 70800ULL
#define FAKE_AUDIO_SOUND_SHORTEST_BLOCK_US 110ULL

void fake_audio_reset(void);
int fake_audio_thread_pump(enum audio_thread_kind kind, int step_count);
void fake_audio_thread_fail_start(enum audio_thread_kind kind, int code);
void fake_audio_port_fail_open(enum audio_port_kind kind, int code);
void fake_audio_port_fail_submit_after(enum audio_port_kind kind, int grain_count, int code);
void fake_audio_port_add_block_once_us(enum audio_port_kind kind, unsigned long long extra_us);
int fake_audio_port_is_open(enum audio_port_kind kind);
int fake_audio_port_open_count(enum audio_port_kind kind);
int fake_audio_port_rate(enum audio_port_kind kind);
int fake_audio_port_grain_frames(enum audio_port_kind kind);
int fake_audio_port_grain_count(enum audio_port_kind kind);
const short *fake_audio_port_grain(enum audio_port_kind kind, int grain_index);
const char *fake_audio_port_events(enum audio_port_kind kind);
void fake_audio_port_clear_events(enum audio_port_kind kind);
int fake_audio_thread_is_alive(enum audio_thread_kind kind);

#endif
