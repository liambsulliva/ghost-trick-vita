#ifndef SMOKE_RUN_H
#define SMOKE_RUN_H

#define SMOKE_RUN_TEXT_MAX 16384
#define SMOKE_RUN_NAME_MAX 64
#define SMOKE_RUN_LINE_MAX 160
#define SMOKE_RUN_WATCH_MAX 24
#define SMOKE_PROBE_UNKNOWN -1

typedef int (*smoke_probe)(const char *word, const char *argument);

void smoke_run_use_probe(smoke_probe probe);
void smoke_run_begin(const char *name, const char *text, unsigned int text_size);
int smoke_run_is_active(void);
void smoke_run_note_frame(void);
int smoke_run_next_command(char *line, unsigned int line_size);
void smoke_run_note_log_line(const char *line);
int smoke_run_flows_over(void);
int smoke_run_flows_failed(void);
const char *smoke_run_name(void);

#endif
