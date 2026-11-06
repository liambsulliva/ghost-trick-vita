#include <stdlib.h>
#include <string.h>
#include "fake_audio.h"
#include "fake_platform.h"

#define FAKE_SAMPLES_PER_FRAME 2
#define FAKE_EVENTS_MAX 4096
#define FAKE_US_PER_SECOND 1000000ULL
#define FAKE_FIRST_GRAIN_CAPACITY 64
#define FAKE_GRAIN_LIMIT 16384

struct fake_port {
    int open_failure_code;
    int submit_failure_code;
    int grains_until_submit_failure;
    unsigned long long extra_block_us;
    unsigned long long queue_ends_us;
    int is_open;
    int open_count;
    int rate;
    int grain_frames;
    int grain_count;
    int grain_capacity;
    short *grains;
    char events[FAKE_EVENTS_MAX];
};

struct fake_thread {
    audio_thread_step step;
    int start_failure_code;
    int is_alive;
};

static struct fake_port ports[AUDIO_PORT_KIND_COUNT];
static struct fake_thread threads[AUDIO_THREAD_KIND_COUNT];
static int mailbox_lock_depth;

static void add_event(struct fake_port *port, const char *event)
{
    if (strlen(port->events) + strlen(event) + 2 < sizeof(port->events)) {
        strcat(port->events, event);
        strcat(port->events, " ");
    }
}

void fake_audio_reset(void)
{
    int index;

    for (index = 0; index < AUDIO_PORT_KIND_COUNT; index++) {
        free(ports[index].grains);
    }
    memset(ports, 0, sizeof(ports));
    memset(threads, 0, sizeof(threads));
    mailbox_lock_depth = 0;
}

int fake_audio_thread_pump(enum audio_thread_kind kind, int step_count)
{
    int step;

    for (step = 0; step < step_count; step++) {
        if (!threads[kind].is_alive) {
            return step;
        }
        if (!threads[kind].step()) {
            threads[kind].is_alive = 0;
        }
    }
    return step_count;
}

void fake_audio_thread_fail_start(enum audio_thread_kind kind, int code)
{
    threads[kind].start_failure_code = code;
}

int fake_audio_thread_is_alive(enum audio_thread_kind kind)
{
    return threads[kind].is_alive;
}

void fake_audio_port_fail_open(enum audio_port_kind kind, int code)
{
    ports[kind].open_failure_code = code;
}

void fake_audio_port_fail_submit_after(enum audio_port_kind kind, int grain_count, int code)
{
    ports[kind].grains_until_submit_failure = grain_count;
    ports[kind].submit_failure_code = code;
}

void fake_audio_port_add_block_once_us(enum audio_port_kind kind, unsigned long long extra_us)
{
    ports[kind].extra_block_us = extra_us;
}

int fake_audio_port_is_open(enum audio_port_kind kind)
{
    return ports[kind].is_open;
}

int fake_audio_port_open_count(enum audio_port_kind kind)
{
    return ports[kind].open_count;
}

int fake_audio_port_rate(enum audio_port_kind kind)
{
    return ports[kind].rate;
}

int fake_audio_port_grain_frames(enum audio_port_kind kind)
{
    return ports[kind].grain_frames;
}

int fake_audio_port_grain_count(enum audio_port_kind kind)
{
    return ports[kind].grain_count;
}

const short *fake_audio_port_grain(enum audio_port_kind kind, int grain_index)
{
    return &ports[kind].grains[grain_index * ports[kind].grain_frames * FAKE_SAMPLES_PER_FRAME];
}

const char *fake_audio_port_events(enum audio_port_kind kind)
{
    return ports[kind].events;
}

void fake_audio_port_clear_events(enum audio_port_kind kind)
{
    ports[kind].events[0] = '\0';
}

int audio_port_open(enum audio_port_kind kind, int rate, int frames_per_grain)
{
    struct fake_port *port;

    port = &ports[kind];
    if (port->open_failure_code != 0) {
        add_event(port, "open_failed");
        return port->open_failure_code;
    }
    add_event(port, "open");
    port->is_open = 1;
    port->open_count = port->open_count + 1;
    port->rate = rate;
    port->grain_frames = frames_per_grain;
    port->queue_ends_us = 0;
    return (int)kind;
}

int audio_port_submit(enum audio_port_kind kind, const short *stereo_frames)
{
    struct fake_port *port;
    size_t grain_samples;
    unsigned long long now_us;
    unsigned long long grain_starts_us;
    unsigned long long grain_returns_us;

    port = &ports[kind];
    if (port->submit_failure_code != 0) {
        if (port->grains_until_submit_failure == 0) {
            add_event(port, "submit_failed");
            return port->submit_failure_code;
        }
        port->grains_until_submit_failure = port->grains_until_submit_failure - 1;
    }
    grain_samples = (size_t)port->grain_frames * FAKE_SAMPLES_PER_FRAME;
    if (port->grain_count >= FAKE_GRAIN_LIMIT) {
        abort();
    }
    if (port->grain_count == port->grain_capacity) {
        port->grain_capacity = port->grain_capacity == 0 ? FAKE_FIRST_GRAIN_CAPACITY : port->grain_capacity * 2;
        port->grains = realloc(port->grains, (size_t)port->grain_capacity * grain_samples * sizeof(short));
    }
    memcpy(&port->grains[(size_t)port->grain_count * grain_samples], stereo_frames, grain_samples * sizeof(short));
    port->grain_count = port->grain_count + 1;
    add_event(port, "submit");
    now_us = fake_clock_peek_us();
    grain_starts_us = port->queue_ends_us > now_us ? port->queue_ends_us : now_us;
    port->queue_ends_us = grain_starts_us + (unsigned long long)port->grain_frames * FAKE_US_PER_SECOND / (unsigned long long)port->rate;
    grain_returns_us = grain_starts_us + (kind == AUDIO_PORT_MUSIC ? FAKE_AUDIO_MUSIC_SHORTEST_BLOCK_US : FAKE_AUDIO_SOUND_SHORTEST_BLOCK_US);
    fake_clock_advance_us(grain_returns_us - now_us + port->extra_block_us);
    port->extra_block_us = 0;
    return 0;
}

void audio_port_drain(enum audio_port_kind kind)
{
    unsigned long long now_us;

    add_event(&ports[kind], "drain");
    now_us = fake_clock_peek_us();
    if (ports[kind].queue_ends_us > now_us) {
        fake_clock_advance_us(ports[kind].queue_ends_us - now_us);
    }
}

void audio_port_close(enum audio_port_kind kind)
{
    add_event(&ports[kind], "close");
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
    if (threads[kind].start_failure_code != 0) {
        return threads[kind].start_failure_code;
    }
    threads[kind].step = step;
    threads[kind].is_alive = 1;
    return 0;
}

void audio_thread_wait_for_end(enum audio_thread_kind kind, unsigned int limit_us)
{
    (void)limit_us;
    fake_audio_thread_pump(kind, FAKE_AUDIO_STEP_LIMIT);
}

void audio_thread_sleep_us(unsigned int duration_us)
{
    fake_clock_advance_us(duration_us);
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
    if (mailbox_lock_depth != 0) {
        abort();
    }
    mailbox_lock_depth = 1;
}

void audio_mailbox_unlock(void)
{
    if (mailbox_lock_depth != 1) {
        abort();
    }
    mailbox_lock_depth = 0;
}
