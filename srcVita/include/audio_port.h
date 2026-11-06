#ifndef AUDIO_PORT_H
#define AUDIO_PORT_H

#define AUDIO_PORT_BUFFER_ALIGNMENT 64

enum audio_port_kind {
    AUDIO_PORT_MUSIC,
    AUDIO_PORT_SOUND,
    AUDIO_PORT_KIND_COUNT
};

struct audio_port_facts {
    int rate;
    int grain;
    int mode;
    int adopt;
    int system_volume;
};

int audio_port_open(enum audio_port_kind kind, int rate, int frames_per_grain);
int audio_port_submit(enum audio_port_kind kind, const short *stereo_frames);
int audio_port_frames_waiting(enum audio_port_kind kind);
void audio_port_drain(enum audio_port_kind kind);
void audio_port_close(enum audio_port_kind kind);
void audio_port_read_facts(enum audio_port_kind kind, struct audio_port_facts *facts);

#endif
