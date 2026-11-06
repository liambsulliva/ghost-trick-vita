#include <stddef.h>
#include <psp2/audioout.h>
#include "audio_port.h"

static int port_numbers[AUDIO_PORT_KIND_COUNT];

static SceAudioOutPortType port_type_for(enum audio_port_kind kind)
{
    return kind == AUDIO_PORT_MUSIC ? SCE_AUDIO_OUT_PORT_TYPE_BGM : SCE_AUDIO_OUT_PORT_TYPE_VOICE;
}

int audio_port_open(enum audio_port_kind kind, int rate, int frames_per_grain)
{
    int result;

    result = sceAudioOutOpenPort(port_type_for(kind), frames_per_grain, rate, SCE_AUDIO_OUT_MODE_STEREO);
    if (result >= 0) {
        port_numbers[kind] = result;
    }
    return result;
}

int audio_port_submit(enum audio_port_kind kind, const short *stereo_frames)
{
    return sceAudioOutOutput(port_numbers[kind], stereo_frames);
}

void audio_port_drain(enum audio_port_kind kind)
{
    sceAudioOutOutput(port_numbers[kind], NULL);
}

void audio_port_close(enum audio_port_kind kind)
{
    sceAudioOutReleasePort(port_numbers[kind]);
}

void audio_port_read_facts(enum audio_port_kind kind, struct audio_port_facts *facts)
{
    int port;

    port = port_numbers[kind];
    facts->rate = sceAudioOutGetConfig(port, SCE_AUDIO_OUT_CONFIG_TYPE_FREQ);
    facts->grain = sceAudioOutGetConfig(port, SCE_AUDIO_OUT_CONFIG_TYPE_LEN);
    facts->mode = sceAudioOutGetConfig(port, SCE_AUDIO_OUT_CONFIG_TYPE_MODE);
    facts->adopt = sceAudioOutGetAdopt(port_type_for(kind));
}
