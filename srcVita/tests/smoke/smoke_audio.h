#ifndef SMOKE_AUDIO_H
#define SMOKE_AUDIO_H

#include "audio_port.h"

void smoke_audio_pump(unsigned long long now_us, unsigned long long frame_us);
unsigned long long smoke_audio_grains(enum audio_port_kind kind);
unsigned long long smoke_audio_grains_with_sound(enum audio_port_kind kind);

#endif
