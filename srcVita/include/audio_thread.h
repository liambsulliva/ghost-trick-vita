#ifndef AUDIO_THREAD_H
#define AUDIO_THREAD_H

enum audio_thread_kind {
    AUDIO_THREAD_MUSIC,
    AUDIO_THREAD_SOUND,
    AUDIO_THREAD_KIND_COUNT
};

typedef int (*audio_thread_step)(void);

struct audio_thread_facts {
    int priority;
    unsigned int cpu_mask;
    int cpu;
    int arm_mhz;
    unsigned int fpscr;
};

int audio_thread_start(enum audio_thread_kind kind, audio_thread_step step);
void audio_thread_wait_for_end(enum audio_thread_kind kind, unsigned int limit_us);
void audio_thread_sleep_us(unsigned int duration_us);
void audio_thread_read_facts(struct audio_thread_facts *facts);
void audio_mailbox_lock(void);
void audio_mailbox_unlock(void);

#endif
