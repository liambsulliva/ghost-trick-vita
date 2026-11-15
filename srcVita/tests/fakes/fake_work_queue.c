#include <stddef.h>
#include "fake_work_queue.h"
#include "work_queue.h"

struct queued_item {
    queued_work work;
    void *argument;
};

static struct queued_item items[WORK_QUEUE_MAX];
static int item_count;
static int items_are_held;

void fake_work_queue_reset(void)
{
    item_count = 0;
    items_are_held = 0;
}

void fake_work_queue_hold(int should_hold)
{
    items_are_held = should_hold;
}

int fake_work_queue_pending(void)
{
    return item_count;
}

void fake_work_queue_run_pending(void)
{
    struct queued_item item;
    int index;

    while (item_count > 0) {
        item = items[0];
        for (index = 1; index < item_count; index++) {
            items[index - 1] = items[index];
        }
        item_count = item_count - 1;
        item.work(item.argument);
    }
}

int work_queue_post(queued_work work, void *argument)
{
    if (item_count >= WORK_QUEUE_MAX) {
        return 0;
    }
    items[item_count].work = work;
    items[item_count].argument = argument;
    item_count = item_count + 1;
    if (!items_are_held) {
        fake_work_queue_run_pending();
    }
    return 1;
}

void work_queue_wait(void)
{
    fake_work_queue_run_pending();
}
