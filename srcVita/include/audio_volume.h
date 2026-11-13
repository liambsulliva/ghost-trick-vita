#ifndef AUDIO_VOLUME_H
#define AUDIO_VOLUME_H

#include "audio_port.h"

#define AUDIO_VOLUME_FULL 256

void audio_volume_set(enum audio_port_kind kind, int out_of_full);
int audio_volume_of(enum audio_port_kind kind);
void audio_volume_scale(enum audio_port_kind kind, short *samples, unsigned int sample_count);

#endif
