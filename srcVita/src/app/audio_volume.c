#include "audio_volume.h"

static volatile int volumes[AUDIO_PORT_KIND_COUNT] = { AUDIO_VOLUME_FULL, AUDIO_VOLUME_FULL };

void audio_volume_set(enum audio_port_kind kind, int out_of_full)
{
    if ((unsigned int)kind >= (unsigned int)AUDIO_PORT_KIND_COUNT) {
        return;
    }
    volumes[kind] = out_of_full < 0 ? 0 : (out_of_full > AUDIO_VOLUME_FULL ? AUDIO_VOLUME_FULL : out_of_full);
}

int audio_volume_of(enum audio_port_kind kind)
{
    return (unsigned int)kind < (unsigned int)AUDIO_PORT_KIND_COUNT ? volumes[kind] : AUDIO_VOLUME_FULL;
}

void audio_volume_scale(enum audio_port_kind kind, short *samples, unsigned int sample_count)
{
    unsigned int index;
    int volume;

    volume = audio_volume_of(kind);
    if (volume >= AUDIO_VOLUME_FULL) {
        return;
    }
    for (index = 0; index < sample_count; index++) {
        samples[index] = (short)((int)samples[index] * volume / AUDIO_VOLUME_FULL);
    }
}
