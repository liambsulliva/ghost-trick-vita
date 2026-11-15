#include <stddef.h>
#include "audio_port.h"
#include "audio_thread.h"
#include "smoke_audio.h"

#define STEPS_PER_FRAME_MOST 16
#define US_PER_SECOND 1000000ULL
#define QUIET_SAMPLE_MOST 8

struct smoke_port {
    int is_open;
    int rate;
    int grain_frames;
    unsigned long long queue_ends_us;
    unsigned long long grains;
    unsigned long long grains_with_sound;
};

static struct smoke_port ports[AUDIO_PORT_KIND_COUNT];
static audio_thread_step steps[AUDIO_THREAD_KIND_COUNT];
static const enum audio_port_kind port_of_thread[AUDIO_THREAD_KIND_COUNT] = { AUDIO_PORT_MUSIC, AUDIO_PORT_SOUND };
static unsigned long long pump_now_us;

int audio_port_open(enum audio_port_kind kind, int rate, int frames_per_grain)
{
    ports[kind].is_open = 1;
    ports[kind].rate = rate;
    ports[kind].grain_frames = frames_per_grain;
    ports[kind].queue_ends_us = 0;
    return (int)kind;
}

int audio_port_submit(enum audio_port_kind kind, const short *stereo_frames)
{
    struct smoke_port *port;
    int has_sound;
    int index;

    port = &ports[kind];
    has_sound = 0;
    for (index = 0; index < port->grain_frames * 2; index++) {
        if (stereo_frames[index] > QUIET_SAMPLE_MOST || stereo_frames[index] < -QUIET_SAMPLE_MOST) {
            has_sound = 1;
            break;
        }
    }
    port->grains = port->grains + 1;
    port->grains_with_sound = port->grains_with_sound + (unsigned long long)has_sound;
    if (port->queue_ends_us < pump_now_us) {
        port->queue_ends_us = pump_now_us;
    }
    if (port->rate > 0) {
        port->queue_ends_us = port->queue_ends_us + (unsigned long long)port->grain_frames * US_PER_SECOND / (unsigned long long)port->rate;
    }
    return 0;
}

void audio_port_drain(enum audio_port_kind kind)
{
    ports[kind].queue_ends_us = 0;
}

void audio_port_close(enum audio_port_kind kind)
{
    ports[kind].is_open = 0;
}

void audio_port_read_facts(enum audio_port_kind kind, struct audio_port_facts *facts)
{
    facts->rate = ports[kind].rate;
    facts->grain = ports[kind].grain_frames;
    facts->mode = 1;
    facts->adopt = 1;
}

int audio_thread_start(enum audio_thread_kind kind, audio_thread_step step)
{
    steps[kind] = step;
    return 0;
}

void audio_thread_wait_for_end(enum audio_thread_kind kind, unsigned int limit_us)
{
    int count;

    (void)limit_us;
    for (count = 0; count < STEPS_PER_FRAME_MOST && steps[kind] != NULL; count++) {
        if (!steps[kind]()) {
            steps[kind] = NULL;
        }
    }
}

void audio_thread_sleep_us(unsigned int duration_us)
{
    (void)duration_us;
}

void audio_thread_read_facts(struct audio_thread_facts *facts)
{
    facts->priority = 64;
    facts->cpu_mask = 0x40000;
    facts->cpu = 2;
    facts->arm_mhz = 333;
    facts->fpscr = 0;
}

void audio_mailbox_lock(void)
{
}

void audio_mailbox_unlock(void)
{
}

void smoke_audio_pump(unsigned long long now_us, unsigned long long frame_us)
{
    int kind;
    int count;

    pump_now_us = now_us;
    for (kind = 0; kind < AUDIO_THREAD_KIND_COUNT; kind++) {
        for (count = 0; count < STEPS_PER_FRAME_MOST && steps[kind] != NULL; count++) {
            unsigned long long grains_before;

            if (ports[port_of_thread[kind]].queue_ends_us >= now_us + frame_us) {
                break;
            }
            grains_before = ports[port_of_thread[kind]].grains;
            if (!steps[kind]()) {
                steps[kind] = NULL;
                break;
            }
            if (ports[port_of_thread[kind]].grains == grains_before) {
                break;
            }
        }
    }
}

unsigned long long smoke_audio_grains(enum audio_port_kind kind)
{
    return ports[kind].grains;
}

unsigned long long smoke_audio_grains_with_sound(enum audio_port_kind kind)
{
    return ports[kind].grains_with_sound;
}
