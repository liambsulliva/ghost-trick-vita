#include <stdio.h>
#include <string.h>
#include "script_narration.h"
#include "text_lines.h"

#define CUE_LINE_MAX 160
#define SHOW_AT_ONCE_OPTION 1
#define CUT_AT_END_OPTION 2
#define LEVEL_FULL 0x1f000
#define LEVEL_DIM_TOP 0x10000
#define DIM_SPEED 0x199a
#define GROUP_SPEED 0x800
#define LAST_GROUP_WAIT 50
#define ONE_GROUP_WAIT 31
#define WAIT_BEFORE_LIMIT_COUNT 31
#define TICKS_OF_TEXT_OUT 60

enum narration_state {
    STATE_DIM_IN = 0,
    STATE_DIM_OUT = 1,
    STATE_GROUP_IN = 2,
    STATE_TEXT_OUT = 3,
    STATE_HOLD = 4
};

static int read_page_line(const char *line, struct script_cue_list *cues)
{
    struct script_cue_page *page;
    int consumed;
    int position;
    int index;

    if (cues->page_count >= SCRIPT_CUE_PAGE_MAX || cues->message_count == 0) {
        return 0;
    }
    page = &cues->pages[cues->page_count];
    memset(page, 0, sizeof(*page));
    if (sscanf(line, "page %d %d %d%n", &page->limit, &page->options, &page->group_count, &consumed) != 3) {
        return 0;
    }
    if (page->group_count < 1 || page->group_count > SCRIPT_CUE_GROUP_MAX) {
        return 0;
    }
    position = consumed;
    for (index = 0; index < page->group_count; index++) {
        if (sscanf(&line[position], "%d%n", &page->group_hold[index], &consumed) != 1) {
            return 0;
        }
        position = position + consumed;
    }
    cues->page_count = cues->page_count + 1;
    cues->messages[cues->message_count - 1].page_count = cues->messages[cues->message_count - 1].page_count + 1;
    return 1;
}

static int read_message_line(const char *line, struct script_cue_list *cues)
{
    struct script_cue_message *message;

    if (cues->message_count >= SCRIPT_CUE_MESSAGE_MAX) {
        return 0;
    }
    message = &cues->messages[cues->message_count];
    memset(message, 0, sizeof(*message));
    if (sscanf(line, "message %31s", message->label) != 1) {
        return 0;
    }
    message->first_page = cues->page_count;
    cues->message_count = cues->message_count + 1;
    return 1;
}

int script_cues_parse(const char *text, unsigned int text_size, struct script_cue_list *cues)
{
    char line[CUE_LINE_MAX];
    unsigned int offset;

    cues->message_count = 0;
    cues->page_count = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (text_line_starts_with(line, "message ")) {
            if (!read_message_line(line, cues)) {
                return 0;
            }
        } else if (text_line_starts_with(line, "page ")) {
            if (!read_page_line(line, cues)) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

const struct script_cue_message *script_cues_find(const struct script_cue_list *cues, const char *label)
{
    int index;

    if (cues == NULL) {
        return NULL;
    }
    for (index = 0; index < cues->message_count; index++) {
        if (strcmp(cues->messages[index].label, label) == 0) {
            return &cues->messages[index];
        }
    }
    return NULL;
}

int script_cue_message_waits_for_button(const struct script_cue_list *cues, const struct script_cue_message *message)
{
    int index;

    for (index = 0; index < message->page_count; index++) {
        if (cues->pages[message->first_page + index].limit == 0) {
            return 1;
        }
    }
    return message->page_count == 0;
}

static void load_page(struct script_narration *narration, int page_index)
{
    const struct script_cue_page *page;
    int index;

    page = &narration->cues->pages[narration->message->first_page + page_index];
    narration->page = page_index;
    narration->group_count = page->group_count;
    narration->limit = page->limit;
    narration->options = page->options;
    if (narration->timing == SCRIPT_TIMING_REMASTER) {
        narration->options = narration->options & ~SHOW_AT_ONCE_OPTION;
    }
    memset(narration->wait, 0, sizeof(narration->wait));
    for (index = 0; index < SCRIPT_CUE_GROUP_MAX; index++) {
        narration->group_hold[index] = page->group_hold[index];
    }
    if (page->group_count <= 1) {
        narration->wait[0] = ONE_GROUP_WAIT;
        return;
    }
    for (index = 0; index < page->group_count - 1; index++) {
        narration->wait[index] = (page->group_count * ONE_GROUP_WAIT - LAST_GROUP_WAIT) / (page->group_count - 1);
    }
    narration->wait[page->group_count - 1] = LAST_GROUP_WAIT;
}

void script_narration_start(struct script_narration *narration, const struct script_cue_list *cues, const struct script_cue_message *message, enum script_timing timing)
{
    memset(narration, 0, sizeof(*narration));
    narration->cues = cues;
    narration->message = message;
    narration->timing = timing;
    narration->is_running = 1;
    narration->state = STATE_DIM_IN;
    narration->speed = DIM_SPEED;
    load_page(narration, 0);
}

static void tell(const struct script_narration *narration, script_narration_report report, void *listener, enum script_narration_event event, int group)
{
    report(listener, narration->message->label, event, group);
}

static void begin_group_in(struct script_narration *narration)
{
    narration->state = STATE_GROUP_IN;
    narration->timer = 0;
    narration->level = 0;
    narration->speed = GROUP_SPEED;
    narration->group_was_reported = 0;
}

static void step_dim_in(struct script_narration *narration, script_narration_report report, void *listener)
{
    int index;

    if (narration->options & SHOW_AT_ONCE_OPTION) {
        tell(narration, report, listener, SCRIPT_NARRATION_DIM_FULL, 0);
        for (index = 0; index < narration->group_count; index++) {
            tell(narration, report, listener, SCRIPT_NARRATION_GROUP_IN, index);
        }
        narration->state = STATE_GROUP_IN;
        narration->timer = 0;
        narration->speed = 0;
        narration->level = LEVEL_FULL;
        narration->group = narration->group_count;
        narration->group_was_reported = 1;
        return;
    }
    if (narration->level > LEVEL_DIM_TOP) {
        begin_group_in(narration);
    }
}

static void step_group_in(struct script_narration *narration, script_narration_report report, void *listener)
{
    int hold;

    narration->timer = narration->timer + 1;
    hold = narration->group < SCRIPT_CUE_GROUP_MAX ? narration->group_hold[narration->group] : 0;
    if (narration->timer > hold) {
        narration->timer = hold;
        if (!narration->group_was_reported) {
            narration->group_was_reported = 1;
            tell(narration, report, listener, SCRIPT_NARRATION_GROUP_IN, narration->group);
        }
    } else {
        narration->level = 0;
    }
    if (narration->level == LEVEL_FULL) {
        narration->counter = 0;
        narration->count_to_limit = 0;
        narration->speed = 0;
        narration->state = STATE_HOLD;
    }
}

static int step_hold(struct script_narration *narration, script_narration_report report, void *listener)
{
    int group_is_done;
    int is_last_page;

    group_is_done = 0;
    narration->counter = narration->counter + 1;
    if (narration->group >= narration->group_count) {
        if (narration->counter >= ONE_GROUP_WAIT) {
            narration->counter = WAIT_BEFORE_LIMIT_COUNT;
            narration->count_to_limit = narration->count_to_limit + 1;
            if (narration->count_to_limit > narration->limit) {
                narration->count_to_limit = narration->limit;
            }
            if (narration->count_to_limit >= narration->limit && narration->limit != 0) {
                group_is_done = 1;
            }
        }
    } else if (narration->counter >= narration->wait[narration->group]) {
        group_is_done = 1;
    }
    if (!group_is_done) {
        return 0;
    }
    narration->counter = 0;
    narration->count_to_limit = 0;
    narration->group = narration->group + 1;
    if (narration->group >= narration->group_count + 1) {
        is_last_page = narration->page >= narration->message->page_count - 1;
        if (is_last_page && (narration->options & CUT_AT_END_OPTION)) {
            tell(narration, report, listener, SCRIPT_NARRATION_CUT, 0);
            return 1;
        }
        narration->speed = -GROUP_SPEED;
        narration->state = STATE_TEXT_OUT;
        tell(narration, report, listener, SCRIPT_NARRATION_TEXT_OUT, 0);
    } else if (narration->group >= narration->group_count) {
        narration->speed = 0;
    } else {
        begin_group_in(narration);
    }
    return 0;
}

static void step_text_out(struct script_narration *narration, script_narration_report report, void *listener)
{
    narration->counter = narration->counter + 1;
    if (narration->counter <= TICKS_OF_TEXT_OUT) {
        return;
    }
    narration->counter = 0;
    narration->count_to_limit = 0;
    if (narration->page >= narration->message->page_count - 1) {
        narration->state = STATE_DIM_OUT;
        narration->level = LEVEL_DIM_TOP;
        narration->speed = -DIM_SPEED;
        tell(narration, report, listener, SCRIPT_NARRATION_DIM_OUT, 0);
        return;
    }
    load_page(narration, narration->page + 1);
    narration->group = 0;
    narration->state = STATE_GROUP_IN;
    narration->timer = 0;
    narration->speed = GROUP_SPEED;
    narration->group_was_reported = 0;
}

int script_narration_step(struct script_narration *narration, script_narration_report report, void *listener)
{
    int is_done;

    if (!narration->is_running) {
        return 0;
    }
    if (narration->state == STATE_DIM_IN && narration->level == 0 && !(narration->options & SHOW_AT_ONCE_OPTION)) {
        tell(narration, report, listener, SCRIPT_NARRATION_DIM_IN, 0);
    }
    if (narration->state != STATE_HOLD) {
        narration->level = narration->level + narration->speed;
        if (narration->level > LEVEL_FULL) {
            narration->level = LEVEL_FULL;
        }
        if (narration->level < 0) {
            narration->level = 0;
        }
    }
    is_done = 0;
    switch (narration->state) {
    case STATE_DIM_IN:
        step_dim_in(narration, report, listener);
        break;
    case STATE_DIM_OUT:
        is_done = narration->level == 0;
        break;
    case STATE_GROUP_IN:
        step_group_in(narration, report, listener);
        break;
    case STATE_TEXT_OUT:
        step_text_out(narration, report, listener);
        break;
    default:
        is_done = step_hold(narration, report, listener);
        break;
    }
    if (is_done) {
        narration->is_running = 0;
    }
    return is_done;
}
