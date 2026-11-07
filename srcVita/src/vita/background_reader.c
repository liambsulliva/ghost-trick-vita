#include <string.h>
#include <psp2/kernel/threadmgr.h>
#include "clock.h"
#include "memory.h"
#include "texture_file.h"
#include "background_reader.h"
#include "file.h"

#define READER_STACK_SIZE (64 * 1024)
#define READER_PRIORITY 0x10000100

struct read_request {
    unsigned int request_id;
    char path[FILE_PATH_MAX];
    int wants_grayscale;
};

static struct read_request requests[BACKGROUND_READER_QUEUE_MAX];
static struct finished_read finished_reads[BACKGROUND_READER_QUEUE_MAX];
static unsigned int next_request_to_add;
static unsigned int next_request_to_read;
static unsigned int next_finished_to_add;
static unsigned int next_finished_to_take;
static unsigned int reads_in_flight;
static SceUID queue_lock;
static SceUID work_available;
static SceUID read_finished;
static int reader_started;

static int unpack_texture(struct finished_read *finished)
{
    unsigned char *unpacked;
    unsigned int unpacked_size;
    int was_unpacked;

    unpacked_size = texture_file_unpacked_size(finished->data);
    unpacked = mem_alloc(unpacked_size);
    if (unpacked == NULL) {
        mem_free(finished->data);
        finished->data = NULL;
        return FILE_ERR_MEMORY;
    }
    was_unpacked = texture_file_unpack(finished->data, finished->size, unpacked, unpacked_size);
    mem_free(finished->data);
    if (!was_unpacked) {
        mem_free(unpacked);
        finished->data = NULL;
        return FILE_ERR_READ;
    }
    finished->data = unpacked;
    finished->size = unpacked_size;
    return FILE_OK;
}

static int reader_thread(SceSize argument_size, void *arguments)
{
    struct read_request request;
    struct finished_read finished;
    unsigned long long started_us;

    (void)argument_size;
    (void)arguments;
    while (1) {
        sceKernelWaitSema(work_available, 1, NULL);

        sceKernelLockMutex(queue_lock, 1, NULL);
        request = requests[next_request_to_read];
        next_request_to_read = (next_request_to_read + 1) % BACKGROUND_READER_QUEUE_MAX;
        sceKernelUnlockMutex(queue_lock, 1);

        finished.request_id = request.request_id;
        finished.data = NULL;
        finished.size = 0;
        finished.unpack_us = 0;
        started_us = clock_now_us();
        finished.result = file_read_whole_timing_open(request.path, &finished.data, &finished.size, &finished.open_us);
        finished.read_us = clock_now_us() - started_us - finished.open_us;
        if (finished.result == FILE_OK && texture_file_is_packed(finished.data, finished.size)) {
            started_us = clock_now_us();
            finished.result = unpack_texture(&finished);
            finished.unpack_us = clock_now_us() - started_us;
        }
        if (finished.result == FILE_OK && request.wants_grayscale) {
            started_us = clock_now_us();
            background_reader_make_grayscale(&finished);
            finished.unpack_us = finished.unpack_us + clock_now_us() - started_us;
        }

        sceKernelLockMutex(queue_lock, 1, NULL);
        finished_reads[next_finished_to_add] = finished;
        next_finished_to_add = (next_finished_to_add + 1) % BACKGROUND_READER_QUEUE_MAX;
        sceKernelUnlockMutex(queue_lock, 1);

        sceKernelSignalSema(read_finished, 1);
    }
    return 0;
}

int background_reader_start(void)
{
    SceUID thread;

    if (reader_started) {
        return 1;
    }
    queue_lock = sceKernelCreateMutex("reader_queue", 0, 0, NULL);
    work_available = sceKernelCreateSema("reader_work", 0, 0, BACKGROUND_READER_QUEUE_MAX, NULL);
    read_finished = sceKernelCreateSema("reader_done", 0, 0, BACKGROUND_READER_QUEUE_MAX, NULL);
    thread = sceKernelCreateThread("file_reader", reader_thread, READER_PRIORITY, READER_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_1, NULL);
    if (queue_lock < 0 || work_available < 0 || read_finished < 0 || thread < 0) {
        return 0;
    }
    if (sceKernelStartThread(thread, 0, NULL) < 0) {
        return 0;
    }
    reader_started = 1;
    return 1;
}

int background_reader_submit(unsigned int request_id, const char *path, int wants_grayscale)
{
    if (!reader_started || reads_in_flight >= BACKGROUND_READER_QUEUE_MAX || strlen(path) >= FILE_PATH_MAX) {
        return 0;
    }
    reads_in_flight = reads_in_flight + 1;

    sceKernelLockMutex(queue_lock, 1, NULL);
    requests[next_request_to_add].request_id = request_id;
    strcpy(requests[next_request_to_add].path, path);
    requests[next_request_to_add].wants_grayscale = wants_grayscale;
    next_request_to_add = (next_request_to_add + 1) % BACKGROUND_READER_QUEUE_MAX;
    sceKernelUnlockMutex(queue_lock, 1);

    sceKernelSignalSema(work_available, 1);
    return 1;
}

static void pop_finished(struct finished_read *finished)
{
    sceKernelLockMutex(queue_lock, 1, NULL);
    *finished = finished_reads[next_finished_to_take];
    next_finished_to_take = (next_finished_to_take + 1) % BACKGROUND_READER_QUEUE_MAX;
    sceKernelUnlockMutex(queue_lock, 1);
    reads_in_flight = reads_in_flight - 1;
}

int background_reader_take_finished(struct finished_read *finished)
{
    if (sceKernelPollSema(read_finished, 1) < 0) {
        return 0;
    }
    pop_finished(finished);
    return 1;
}

void background_reader_wait_for_finished(struct finished_read *finished)
{
    sceKernelWaitSema(read_finished, 1, NULL);
    pop_finished(finished);
}
