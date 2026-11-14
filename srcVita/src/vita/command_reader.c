#include <stdio.h>
#include <psp2/kernel/threadmgr.h>
#include "command_reader.h"
#include "file.h"

#define READER_STACK_SIZE (16 * 1024)
#define READER_PRIORITY 0x10000100
#define READER_PERIOD_US 100000

static char commands_path[FILE_PATH_MAX];
static SceUID handoff_lock;
static unsigned char *waiting_text;
static unsigned int waiting_size;
static int reader_started;

static int reader_thread(SceSize argument_size, void *arguments)
{
    unsigned char *text;
    unsigned int size;
    int has_room;

    (void)argument_size;
    (void)arguments;
    while (1) {
        sceKernelDelayThread(READER_PERIOD_US);
        sceKernelLockMutex(handoff_lock, 1, NULL);
        has_room = waiting_text == NULL;
        sceKernelUnlockMutex(handoff_lock, 1);
        if (!has_room || file_read_whole(commands_path, &text, &size) != FILE_OK) {
            continue;
        }
        file_delete(commands_path);
        sceKernelLockMutex(handoff_lock, 1, NULL);
        waiting_text = text;
        waiting_size = size;
        sceKernelUnlockMutex(handoff_lock, 1);
    }
    return 0;
}

int command_reader_start(const char *path)
{
    SceUID thread;

    if (reader_started) {
        return 1;
    }
    snprintf(commands_path, sizeof(commands_path), "%s", path);
    handoff_lock = sceKernelCreateMutex("command_handoff", 0, 0, NULL);
    thread = sceKernelCreateThread("command_reader", reader_thread, READER_PRIORITY, READER_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_1, NULL);
    if (handoff_lock < 0 || thread < 0 || sceKernelStartThread(thread, 0, NULL) < 0) {
        return 0;
    }
    reader_started = 1;
    return 1;
}

int command_reader_take(unsigned char **text, unsigned int *size)
{
    if (!reader_started) {
        return 0;
    }
    sceKernelLockMutex(handoff_lock, 1, NULL);
    *text = waiting_text;
    *size = waiting_size;
    waiting_text = NULL;
    waiting_size = 0;
    sceKernelUnlockMutex(handoff_lock, 1);
    return *text != NULL;
}
