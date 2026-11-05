#ifndef RUN_LOG_H
#define RUN_LOG_H

#define RUN_LOG_LINE_MAX 512

void run_log_open(const char *path);
void run_log_write(const char *format, ...);
void run_log_flush(void);
unsigned int run_log_lines_written(void);

#endif
