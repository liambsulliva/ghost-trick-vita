#ifndef BACKGROUND_TASK_H
#define BACKGROUND_TASK_H

typedef void (*background_work)(void *argument);

int background_task_start(background_work work, void *argument);
int background_task_is_running(void);
void background_task_wait(void);

#endif
