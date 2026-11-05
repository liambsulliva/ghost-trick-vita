#ifndef LOG_SINK_H
#define LOG_SINK_H

void log_sink_start(const char *path);
void log_sink_write(const char *text, unsigned int size);
void log_sink_flush(void);

#endif
