#ifndef FAKE_WORK_QUEUE_H
#define FAKE_WORK_QUEUE_H

void fake_work_queue_reset(void);
void fake_work_queue_hold(int should_hold);
int fake_work_queue_pending(void);
void fake_work_queue_run_pending(void);

#endif
