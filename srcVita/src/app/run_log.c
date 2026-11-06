#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "log_sink.h"
#include "run_log.h"

#define LOG_FOR_TOOLS_LAUNCH_MARKER "--- launch"

static unsigned long long log_start_us;

void run_log_open(const char *path)
{
    log_start_us = clock_now_us();
    log_sink_start(path);
    run_log_write(LOG_FOR_TOOLS_LAUNCH_MARKER);
}

void run_log_write(const char *format, ...)
{
    char message[RUN_LOG_LINE_MAX];
    char line[RUN_LOG_LINE_MAX + 32];
    unsigned long long elapsed_ms;
    int line_length;
    va_list arguments;

    va_start(arguments, format);
    vsnprintf(message, sizeof(message), format, arguments);
    va_end(arguments);

    elapsed_ms = (clock_now_us() - log_start_us) / 1000;
    line_length = snprintf(line, sizeof(line), "[%7llu.%03llu] %s\n", elapsed_ms / 1000, elapsed_ms % 1000, message);
    if (line_length < 0) {
        return;
    }
    if (line_length >= (int)sizeof(line)) {
        line_length = (int)sizeof(line) - 1;
    }
    log_sink_write(line, (unsigned int)line_length);
}

void run_log_flush(void)
{
    log_sink_flush();
}
