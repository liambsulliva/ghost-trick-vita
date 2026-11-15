#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run_log.h"
#include "smoke_run.h"
#include "text_lines.h"

#define WAIT_WORD "wait "
#define UNTIL_WORD "until "
#define EXPECT_WORD "expect "
#define STALL_WORD "stall "
#define NOT_WORD "not "
#define LOG_WORD "log"
#define NO_OFFSET 0xffffffffu
#define LOG_FOR_TOOLS_SMOKE_BEGIN "smoke begin name=%s"
#define LOG_FOR_TOOLS_SMOKE_OK "smoke ok name=%s frames=%u"
#define LOG_FOR_TOOLS_SMOKE_FAIL "smoke fail name=%s line=%d reason=%s frames=%u text=%s"

struct log_watch {
    char text[SMOKE_RUN_LINE_MAX];
    unsigned int hits;
    unsigned int taken;
};

static char flow_text[SMOKE_RUN_TEXT_MAX];
static unsigned int flow_size;
static unsigned int flow_offset;
static unsigned int counted_offset;
static char flow_name[SMOKE_RUN_NAME_MAX];
static int flow_is_active;
static int flow_line_number;
static unsigned int frames_in_flow;
static unsigned int frames_on_line;
static int flows_over;
static int flows_failed;
static smoke_probe probe_now;
static struct log_watch watches[SMOKE_RUN_WATCH_MAX];
static int watch_count;
static char stall_text[SMOKE_RUN_LINE_MAX];
static unsigned int stall_frames_most;
static unsigned int frames_since_stall_text;

struct condition {
    int is_negated;
    char word[32];
    const char *argument;
};

void smoke_run_use_probe(smoke_probe probe)
{
    probe_now = probe;
}

static void read_condition(const char *text, struct condition *condition)
{
    unsigned int length;

    condition->is_negated = 0;
    if (text_line_starts_with(text, NOT_WORD)) {
        condition->is_negated = 1;
        text = text + strlen(NOT_WORD);
    }
    length = 0;
    while (text[length] != '\0' && text[length] != ' ' && length < sizeof(condition->word) - 1) {
        condition->word[length] = text[length];
        length = length + 1;
    }
    condition->word[length] = '\0';
    condition->argument = text[length] == ' ' ? &text[length + 1] : &text[length];
}

static struct log_watch *watch_of(const char *text)
{
    int index;

    for (index = 0; index < watch_count; index++) {
        if (strcmp(watches[index].text, text) == 0) {
            return &watches[index];
        }
    }
    return NULL;
}

static const char *condition_text_of(const char *line)
{
    if (text_line_starts_with(line, EXPECT_WORD)) {
        return line + strlen(EXPECT_WORD);
    }
    if (text_line_starts_with(line, UNTIL_WORD)) {
        line = line + strlen(UNTIL_WORD);
        while (*line >= '0' && *line <= '9') {
            line = line + 1;
        }
        return *line == ' ' ? line + 1 : NULL;
    }
    return NULL;
}

static void watch_the_log_lines_the_flow_names(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    struct condition condition;
    const char *text;
    unsigned int offset;

    watch_count = 0;
    offset = 0;
    while (offset < flow_size) {
        offset = text_lines_read_next(flow_text, flow_size, offset, line, sizeof(line));
        text = condition_text_of(line);
        if (text == NULL) {
            continue;
        }
        read_condition(text, &condition);
        if (strcmp(condition.word, LOG_WORD) != 0 || watch_of(condition.argument) != NULL || watch_count >= SMOKE_RUN_WATCH_MAX) {
            continue;
        }
        snprintf(watches[watch_count].text, sizeof(watches[watch_count].text), "%s", condition.argument);
        watches[watch_count].hits = 0;
        watches[watch_count].taken = 0;
        watch_count = watch_count + 1;
    }
}

void smoke_run_begin(const char *name, const char *text, unsigned int text_size)
{
    if (text_size > SMOKE_RUN_TEXT_MAX) {
        text_size = SMOKE_RUN_TEXT_MAX;
    }
    memcpy(flow_text, text, text_size);
    flow_size = text_size;
    flow_offset = 0;
    counted_offset = NO_OFFSET;
    stall_frames_most = 0;
    frames_since_stall_text = 0;
    flow_line_number = 0;
    frames_in_flow = 0;
    frames_on_line = 0;
    snprintf(flow_name, sizeof(flow_name), "%s", name);
    watch_the_log_lines_the_flow_names();
    flow_is_active = 1;
    run_log_write(LOG_FOR_TOOLS_SMOKE_BEGIN, flow_name);
}

int smoke_run_is_active(void)
{
    return flow_is_active;
}

void smoke_run_note_frame(void)
{
    if (flow_is_active) {
        frames_in_flow = frames_in_flow + 1u;
        frames_on_line = frames_on_line + 1u;
        frames_since_stall_text = frames_since_stall_text + 1u;
    }
}

void smoke_run_note_log_line(const char *line)
{
    int index;

    if (!flow_is_active) {
        return;
    }
    for (index = 0; index < watch_count; index++) {
        if (strstr(line, watches[index].text) != NULL) {
            watches[index].hits = watches[index].hits + 1u;
        }
    }
    if (stall_frames_most > 0 && strstr(line, stall_text) != NULL) {
        frames_since_stall_text = 0;
    }
}

static void end_flow(void)
{
    flow_is_active = 0;
    flows_over = flows_over + 1;
}

static void fail(const char *reason, const char *line)
{
    flows_failed = flows_failed + 1;
    end_flow();
    run_log_write(LOG_FOR_TOOLS_SMOKE_FAIL, flow_name, flow_line_number, reason, frames_in_flow, line);
}

static int condition_holds(const struct condition *condition, int takes_the_log_line)
{
    struct log_watch *watch;
    int holds;

    if (strcmp(condition->word, LOG_WORD) == 0) {
        watch = watch_of(condition->argument);
        holds = watch != NULL && watch->hits > watch->taken;
        if (holds && takes_the_log_line && !condition->is_negated) {
            watch->taken = watch->taken + 1u;
        }
    } else {
        holds = probe_now != NULL ? probe_now(condition->word, condition->argument) : SMOKE_PROBE_UNKNOWN;
        if (holds == SMOKE_PROBE_UNKNOWN) {
            return SMOKE_PROBE_UNKNOWN;
        }
    }
    return condition->is_negated ? !holds : holds;
}

static void watch_for_a_stall(const char *line)
{
    const char *text;

    stall_frames_most = (unsigned int)strtoul(line + strlen(STALL_WORD), NULL, 10);
    text = line + strlen(STALL_WORD);
    while (*text >= '0' && *text <= '9') {
        text = text + 1;
    }
    snprintf(stall_text, sizeof(stall_text), "%s", *text == ' ' ? text + 1 : text);
    frames_since_stall_text = 0;
}

static int step_line(const char *line)
{
    struct condition condition;
    unsigned int frames_most;
    int holds;

    if (text_line_starts_with(line, STALL_WORD)) {
        watch_for_a_stall(line);
        return 1;
    }
    if (text_line_starts_with(line, WAIT_WORD)) {
        return frames_on_line > (unsigned int)strtoul(line + strlen(WAIT_WORD), NULL, 10);
    }
    read_condition(condition_text_of(line), &condition);
    holds = condition_holds(&condition, text_line_starts_with(line, UNTIL_WORD));
    if (holds == SMOKE_PROBE_UNKNOWN) {
        fail("unknown_condition", line);
        return 0;
    }
    if (text_line_starts_with(line, EXPECT_WORD)) {
        if (!holds) {
            fail("expect", line);
        }
        return holds;
    }
    frames_most = (unsigned int)strtoul(line + strlen(UNTIL_WORD), NULL, 10);
    if (!holds && frames_on_line > frames_most) {
        fail("timeout", line);
    }
    return holds;
}

static int line_is_the_runners(const char *line)
{
    return text_line_starts_with(line, WAIT_WORD) || text_line_starts_with(line, STALL_WORD) || condition_text_of(line) != NULL;
}

int smoke_run_next_command(char *line, unsigned int line_size)
{
    unsigned int next_offset;

    while (flow_is_active) {
        if (stall_frames_most > 0 && frames_since_stall_text > stall_frames_most) {
            fail("stalled", stall_text);
            return 0;
        }
        if (flow_offset >= flow_size) {
            end_flow();
            run_log_write(LOG_FOR_TOOLS_SMOKE_OK, flow_name, frames_in_flow);
            return 0;
        }
        next_offset = text_lines_read_next(flow_text, flow_size, flow_offset, line, line_size);
        if (line[0] != '\0' && line_is_the_runners(line)) {
            if (flow_offset != counted_offset) {
                flow_line_number = flow_line_number + 1;
                counted_offset = flow_offset;
            }
            if (!step_line(line)) {
                return 0;
            }
            flow_offset = next_offset;
            frames_on_line = 0;
            continue;
        }
        flow_offset = next_offset;
        frames_on_line = 0;
        if (line[0] != '\0') {
            flow_line_number = flow_line_number + 1;
            return 1;
        }
    }
    return 0;
}

int smoke_run_flows_over(void)
{
    return flows_over;
}

int smoke_run_flows_failed(void)
{
    return flows_failed;
}

const char *smoke_run_name(void)
{
    return flow_name;
}
