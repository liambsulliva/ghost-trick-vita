#include <psp2/kernel/threadmgr.h>
#include "work_queue.h"

#define QUEUE_STACK_SIZE (64 * 1024)
#define QUEUE_PRIORITY 0x10000100
#define QUEUE_WAIT_STEP_US 1000

struct queued_item {
    queued_work work;
    void *argument;
};

static struct queued_item items[WORK_QUEUE_MAX];
static int first_item;
static int item_count;
static volatile int unfinished_count;
static SceUID queue_lock = -1;
static SceUID items_waiting = -1;
static int queue_started;
static int queue_failed;

static int queue_thread(SceSize argument_size, void *arguments)
{
    struct queued_item item;

    (void)argument_size;
    (void)arguments;
    while (1) {
        if (sceKernelWaitSema(items_waiting, 1, NULL) < 0) {
            continue;
        }
        sceKernelLockMutex(queue_lock, 1, NULL);
        item = items[first_item];
        first_item = (first_item + 1) % WORK_QUEUE_MAX;
        item_count = item_count - 1;
        sceKernelUnlockMutex(queue_lock, 1);
        item.work(item.argument);
        __sync_synchronize();
        __sync_fetch_and_sub(&unfinished_count, 1);
    }
    return 0;
}

static int start_queue(void)
{
    SceUID thread;

    if (queue_started) {
        return 1;
    }
    if (queue_failed) {
        return 0;
    }
    queue_lock = sceKernelCreateMutex("work_queue", 0, 0, NULL);
    items_waiting = sceKernelCreateSema("work_queue_items", 0, 0, WORK_QUEUE_MAX, NULL);
    thread = sceKernelCreateThread("work_queue", queue_thread, QUEUE_PRIORITY, QUEUE_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_0, NULL);
    if (queue_lock < 0 || items_waiting < 0 || thread < 0 || sceKernelStartThread(thread, 0, NULL) < 0) {
        queue_failed = 1;
        return 0;
    }
    queue_started = 1;
    return 1;
}

int work_queue_post(queued_work work, void *argument)
{
    int has_room;

    if (!start_queue()) {
        return 0;
    }
    sceKernelLockMutex(queue_lock, 1, NULL);
    has_room = item_count < WORK_QUEUE_MAX;
    if (has_room) {
        items[(first_item + item_count) % WORK_QUEUE_MAX].work = work;
        items[(first_item + item_count) % WORK_QUEUE_MAX].argument = argument;
        item_count = item_count + 1;
        __sync_fetch_and_add(&unfinished_count, 1);
    }
    sceKernelUnlockMutex(queue_lock, 1);
    if (has_room) {
        sceKernelSignalSema(items_waiting, 1);
    }
    return has_room;
}

void work_queue_wait(void)
{
    while (unfinished_count > 0) {
        sceKernelDelayThread(QUEUE_WAIT_STEP_US);
    }
    __sync_synchronize();
}
