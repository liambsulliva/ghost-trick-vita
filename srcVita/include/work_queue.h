#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#define WORK_QUEUE_MAX 16

typedef void (*queued_work)(void *argument);

int work_queue_post(queued_work work, void *argument);
int work_queue_is_busy(void);
void work_queue_wait(void);

#endif
