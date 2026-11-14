#include <stddef.h>
#include "background_task.h"
#include "fake_task.h"

static background_work pending_work;
static void *pending_argument;
static int tasks_are_held;

void fake_task_reset(void)
{
    pending_work = NULL;
    pending_argument = NULL;
    tasks_are_held = 0;
}

void fake_task_hold(int should_hold)
{
    tasks_are_held = should_hold;
}

int fake_task_is_pending(void)
{
    return pending_work != NULL;
}

void fake_task_run_pending(void)
{
    background_work work;

    work = pending_work;
    pending_work = NULL;
    if (work != NULL) {
        work(pending_argument);
    }
}

int background_task_start(background_work work, void *argument)
{
    if (pending_work != NULL) {
        return 0;
    }
    pending_work = work;
    pending_argument = argument;
    if (!tasks_are_held) {
        fake_task_run_pending();
    }
    return 1;
}

int background_task_is_running(void)
{
    return pending_work != NULL;
}

void background_task_wait(void)
{
    fake_task_run_pending();
}
