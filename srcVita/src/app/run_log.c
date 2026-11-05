#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "run_log.h"
#include "timing.h"

static char log_path[FILE_PATH_MAX];
static unsigned long long log_start_us;
static unsigned int log_line_count;

void run_log_open(const char *path)
{
    snprintf(log_path, sizeof(log_path), "%s", path);
    log_start_us = timing_now_us();
    log_line_count = 0;
    run_log_write("--- launch");
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

    elapsed_ms = (timing_now_us() - log_start_us) / 1000;
    line_length = snprintf(line, sizeof(line), "[%7llu.%03llu] %s\n", elapsed_ms / 1000, elapsed_ms % 1000, message);
    if (line_length < 0) {
        return;
    }
    if (line_length >= (int)sizeof(line)) {
        line_length = (int)sizeof(line) - 1;
    }
    file_append(log_path, (const unsigned char *)line, (unsigned int)line_length);
    log_line_count = log_line_count + 1;
}

unsigned int run_log_lines_written(void)
{
    return log_line_count;
}
