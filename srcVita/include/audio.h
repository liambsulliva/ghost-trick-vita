#ifndef AUDIO_H
#define AUDIO_H

#define AUDIO_ERR_NOT_IMPLEMENTED -1

int audio_open(void);
int audio_play(const void *pcm, unsigned int size);
int audio_close(void);

#endif
