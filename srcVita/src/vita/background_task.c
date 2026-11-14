#include <psp2/kernel/threadmgr.h>
#include "background_task.h"

#define TASK_STACK_SIZE (256 * 1024)
#define TASK_PRIORITY 0x10000100

struct task_start {
    background_work work;
    void *argument;
};

static SceUID task_thread = -1;
static volatile int task_is_running;

static int task_body(SceSize argument_size, void *arguments)
{
    const struct task_start *start;

    (void)argument_size;
    start = arguments;
    start->work(start->argument);
    __sync_synchronize();
    task_is_running = 0;
    return 0;
}

static void forget_finished_thread(void)
{
    if (task_thread >= 0) {
        sceKernelWaitThreadEnd(task_thread, NULL, NULL);
        sceKernelDeleteThread(task_thread);
        task_thread = -1;
    }
}

int background_task_start(background_work work, void *argument)
{
    struct task_start start;

    if (task_is_running) {
        return 0;
    }
    forget_finished_thread();
    task_thread = sceKernelCreateThread("background_task", task_body, TASK_PRIORITY, TASK_STACK_SIZE, 0, SCE_KERNEL_CPU_MASK_USER_0, NULL);
    if (task_thread < 0) {
        return 0;
    }
    start.work = work;
    start.argument = argument;
    task_is_running = 1;
    if (sceKernelStartThread(task_thread, sizeof(start), &start) < 0) {
        task_is_running = 0;
        sceKernelDeleteThread(task_thread);
        task_thread = -1;
        return 0;
    }
    return 1;
}

int background_task_is_running(void)
{
    if (task_is_running) {
        return 1;
    }
    __sync_synchronize();
    return 0;
}

void background_task_wait(void)
{
    forget_finished_thread();
    task_is_running = 0;
}
