#include <stdio.h>
#include <string.h>
#include "script_route.h"
#include "text_lines.h"

#define ROUTE_LINE_MAX 256
#define ROUTE_LINE_SCAN "step %95s %15s %47s %d %63s %d"
#define ROUTE_WORD_MAX 64
#define ROUTE_ACTION_WORD_MAX 16
#define ROUTE_EMPTY_WORD "-"
#define ACT_WAIT_PREFIX "act:"
#define CORE_WAIT_PREFIX "core:"

static const char *const action_names[] = { "move", "trick", "ghost_on", "ghost_off", "tap_left", "tap_right", "wait" };

static int read_action(const char *word, enum script_route_action *action)
{
    unsigned int index;

    for (index = 0; index < sizeof(action_names) / sizeof(action_names[0]); index++) {
        if (strcmp(word, action_names[index]) == 0) {
            *action = (enum script_route_action)index;
            return 1;
        }
    }
    return 0;
}

static int read_wait(const char *word, struct script_route_step *step)
{
    step->wait_name[0] = '\0';
    if (strcmp(word, ROUTE_EMPTY_WORD) == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_NOTHING;
    } else if (strcmp(word, "playable") == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_PLAYABLE;
    } else if (strcmp(word, "tutorial:touch") == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_TUTORIAL_TOUCH;
    } else if (strcmp(word, "tutorial:move") == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_TUTORIAL_MOVE;
    } else if (strncmp(word, ACT_WAIT_PREFIX, strlen(ACT_WAIT_PREFIX)) == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_ACT;
        return snprintf(step->wait_name, sizeof(step->wait_name), "%s", &word[strlen(ACT_WAIT_PREFIX)]) < (int)sizeof(step->wait_name);
    } else if (strncmp(word, CORE_WAIT_PREFIX, strlen(CORE_WAIT_PREFIX)) == 0) {
        step->wait = SCRIPT_ROUTE_WAIT_CORE;
        return snprintf(step->wait_name, sizeof(step->wait_name), "%s", &word[strlen(CORE_WAIT_PREFIX)]) < (int)sizeof(step->wait_name);
    } else {
        return 0;
    }
    return 1;
}

static int read_step(struct script_route *route, const char *line)
{
    struct script_route_step *step;
    char action_word[ROUTE_ACTION_WORD_MAX];
    char wait_word[ROUTE_WORD_MAX];

    if (route->step_count >= SCRIPT_ROUTE_STEP_MAX) {
        return 0;
    }
    step = &route->steps[route->step_count];
    if (sscanf(line, ROUTE_LINE_SCAN, step->script, action_word, step->object, &step->slot, wait_word, &step->dwell_ticks) != 6) {
        return 0;
    }
    if (!read_action(action_word, &step->action) || !read_wait(wait_word, step)) {
        return 0;
    }
    if (strcmp(step->object, ROUTE_EMPTY_WORD) == 0) {
        step->object[0] = '\0';
    }
    route->step_count = route->step_count + 1;
    return 1;
}

int script_route_read(struct script_route *route, const char *text, unsigned int text_size)
{
    char line[ROUTE_LINE_MAX];
    unsigned int offset;

    offset = 0;
    route->step_count = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] != '\0' && !read_step(route, line)) {
            return 0;
        }
    }
    return 1;
}

int script_route_first_step_of_script(const struct script_route *route, const char *script)
{
    int index;

    for (index = 0; index < route->step_count; index++) {
        if (strcmp(route->steps[index].script, script) == 0) {
            return index;
        }
    }
    return -1;
}

const char *script_route_action_name(enum script_route_action action)
{
    return action_names[action];
}
