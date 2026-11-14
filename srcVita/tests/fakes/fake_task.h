#ifndef FAKE_TASK_H
#define FAKE_TASK_H

void fake_task_reset(void);
void fake_task_hold(int should_hold);
int fake_task_is_pending(void);
void fake_task_run_pending(void);

#endif
