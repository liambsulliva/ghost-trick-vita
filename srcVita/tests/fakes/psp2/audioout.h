#ifndef FAKE_PSP2_AUDIOOUT_H
#define FAKE_PSP2_AUDIOOUT_H

typedef enum SceAudioOutPortType {
    SCE_AUDIO_OUT_PORT_TYPE_MAIN = 0,
    SCE_AUDIO_OUT_PORT_TYPE_BGM = 1,
    SCE_AUDIO_OUT_PORT_TYPE_VOICE = 2
} SceAudioOutPortType;

int sceAudioOutGetAdopt(SceAudioOutPortType type);

#endif
