#include <stdio.h>
#include <string.h>
#include <psp2/kernel/threadmgr.h>
#include "file.h"
#include "log_sink.h"

#define QUEUE_SIZE (128 * 1024)
#define WRITER_STACK_SIZE (32 * 1024)
#define WRITER_PRIORITY 0x10000100
#define WRITER_SLEEP_US 50000
#define FLUSH_POLL_US 1000
#define DROPPED_NOTE_MAX 64

static char sink_path[FILE_PATH_MAX];
static char queue[QUEUE_SIZE];
static char writer_buffer[QUEUE_SIZE];
static unsigned int queued_bytes;
static unsigned int dropped_bytes;
static SceUID queue_lock;
static volatile int writer_is_busy;

static void write_batch_if_any(void)
{
    unsigned int batch_size;
    unsigned int dropped;
    char note[DROPPED_NOTE_MAX];
    int note_size;

    sceKernelLockMutex(queue_lock, 1, NULL);
    batch_size = queued_bytes;
    dropped = dropped_bytes;
    if (batch_size > 0) {
        writer_is_busy = 1;
        memcpy(writer_buffer, queue, batch_size);
    }
    queued_bytes = 0;
    dropped_bytes = 0;
    sceKernelUnlockMutex(queue_lock, 1);

    if (batch_size > 0) {
        file_append(sink_path, (const unsigned char *)writer_buffer, batch_size);
        writer_is_busy = 0;
    }
    if (dropped > 0) {
        note_size = snprintf(note, sizeof(note), "log dropped_bytes=%u\n", dropped);
        file_append(sink_path, (const unsigned char *)note, (unsigned int)note_size);
    }
}

static int writer_thread(SceSize argument_size, void *arguments)
{
    (void)argument_size;
    (void)arguments;
    while (1) {
        write_batch_if_any();
        sceKernelDelayThread(WRITER_SLEEP_US);
    }
    return 0;
}

void log_sink_start(const char *path)
{
    SceUID thread;

    snprintf(sink_path, sizeof(sink_path), "%s", path);
    queued_bytes = 0;
    dropped_bytes = 0;
    writer_is_busy = 0;
    queue_lock = sceKernelCreateMutex("log_queue", 0, 0, NULL);
    thread = sceKernelCreateThread("log_writer", writer_thread, WRITER_PRIORITY, WRITER_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_2, NULL);
    if (thread >= 0) {
        sceKernelStartThread(thread, 0, NULL);
    }
}

void log_sink_write(const char *text, unsigned int size)
{
    sceKernelLockMutex(queue_lock, 1, NULL);
    if (queued_bytes + size <= QUEUE_SIZE) {
        memcpy(&queue[queued_bytes], text, size);
        queued_bytes = queued_bytes + size;
    } else {
        dropped_bytes = dropped_bytes + size;
    }
    sceKernelUnlockMutex(queue_lock, 1);
}

void log_sink_flush(void)
{
    while (queued_bytes > 0 || writer_is_busy) {
        sceKernelDelayThread(FLUSH_POLL_US);
    }
}
