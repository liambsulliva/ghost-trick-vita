#ifndef FAKE_READER_H
#define FAKE_READER_H

void fake_reader_reset(void);
void fake_reader_hold_reads(int should_hold);
int fake_reader_pending_count(void);
void fake_reader_release_reads(int count);
void fake_reader_refuse_submits(int should_refuse);
void fake_reader_fail_start(int should_fail);
void fake_reader_run_out_of_memory_on_reads(int count);
const char *fake_reader_pending_path(int index);

#endif
