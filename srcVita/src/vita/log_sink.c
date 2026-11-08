#include <stdio.h>
#include <string.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/threadmgr.h>
#include "file.h"
#include "log_sink.h"

#define QUEUE_SIZE (128 * 1024)
#define WRITER_STACK_SIZE (32 * 1024)
#define WRITER_PRIORITY 0x10000100
#define WRITER_SLEEP_US 50000
#define FLUSH_POLL_US 1000
#define DROPPED_NOTE_MAX 64
#define LOG_FILE_MOST_BYTES (128u * 1024u * 1024u)
#define EARLIER_PART_SUFFIX ".earlier"

static char sink_path[FILE_PATH_MAX];
static char earlier_part_path[FILE_PATH_MAX + sizeof(EARLIER_PART_SUFFIX)];
static unsigned long long bytes_in_file;
static char queue[QUEUE_SIZE];
static char writer_buffer[QUEUE_SIZE];
static unsigned int queued_bytes;
static unsigned int dropped_bytes;
static SceUID queue_lock;
static int writer_is_busy;
static int writer_is_running;

static unsigned long long size_of_file(const char *path)
{
    SceIoStat status;

    if (sceIoGetstat(path, &status) < 0) {
        return 0;
    }
    return (unsigned long long)status.st_size;
}

static void move_the_full_file_aside(void)
{
    char note[DROPPED_NOTE_MAX];
    int note_size;

    sceIoRemove(earlier_part_path);
    sceIoRename(sink_path, earlier_part_path);
    note_size = snprintf(note, sizeof(note), "log continued_after_bytes=%llu\n", bytes_in_file);
    file_append(sink_path, (const unsigned char *)note, (unsigned int)note_size);
    bytes_in_file = (unsigned long long)note_size;
}

static void append_to_file(const char *text, unsigned int size)
{
    if (bytes_in_file + size > LOG_FILE_MOST_BYTES) {
        move_the_full_file_aside();
    }
    file_append(sink_path, (const unsigned char *)text, size);
    bytes_in_file = bytes_in_file + size;
}

static void write_batch_if_any(void)
{
    unsigned int batch_size;
    unsigned int dropped;
    char note[DROPPED_NOTE_MAX];
    int note_size;

    sceKernelLockMutex(queue_lock, 1, NULL);
    batch_size = queued_bytes;
    dropped = dropped_bytes;
    writer_is_busy = batch_size > 0 || dropped > 0;
    if (batch_size > 0) {
        memcpy(writer_buffer, queue, batch_size);
    }
    queued_bytes = 0;
    dropped_bytes = 0;
    sceKernelUnlockMutex(queue_lock, 1);

    if (batch_size > 0) {
        append_to_file(writer_buffer, batch_size);
    }
    if (dropped > 0) {
        note_size = snprintf(note, sizeof(note), "log dropped_bytes=%u\n", dropped);
        append_to_file(note, (unsigned int)note_size);
    }
    sceKernelLockMutex(queue_lock, 1, NULL);
    writer_is_busy = 0;
    sceKernelUnlockMutex(queue_lock, 1);
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
    snprintf(earlier_part_path, sizeof(earlier_part_path), "%s%s", path, EARLIER_PART_SUFFIX);
    sceIoRemove(earlier_part_path);
    bytes_in_file = size_of_file(sink_path);
    queued_bytes = 0;
    dropped_bytes = 0;
    writer_is_busy = 0;
    queue_lock = sceKernelCreateMutex("log_queue", 0, 0, NULL);
    thread = sceKernelCreateThread("log_writer", writer_thread, WRITER_PRIORITY, WRITER_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_2, NULL);
    writer_is_running = thread >= 0 && sceKernelStartThread(thread, 0, NULL) >= 0;
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
    int is_done;

    if (!writer_is_running) {
        return;
    }
    is_done = 0;
    while (!is_done) {
        sceKernelLockMutex(queue_lock, 1, NULL);
        is_done = queued_bytes == 0 && !writer_is_busy;
        sceKernelUnlockMutex(queue_lock, 1);
        if (!is_done) {
            sceKernelDelayThread(FLUSH_POLL_US);
        }
    }
}
