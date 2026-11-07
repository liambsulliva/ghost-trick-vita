#include <stdio.h>
#include <string.h>
#include "script_route.h"
#include "text_lines.h"

#define ROUTE_LINE_MAX 384
#define ROUTE_LINE_SCAN "step %95s %15s %47s %d %d %159s"
#define ROUTE_ACTION_WORD_MAX 16
#define ROUTE_EMPTY_WORD "-"
#define WAIT_TOKEN_SEPARATOR '&'

static const char *const action_names[] = { "move", "trick", "ghost_on", "ghost_off", "touch", "tap_balloon", "camera", "phone_call", "wait" };
static const char *const known_wait_words[] = { "act", "tutorial", "playable", "free", "view", "no_talk", "flag", "noflag", "core", "usable", "balloon", "camera_tutorial", "timeleap_armed", "ticks" };

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

static int wait_word_is_known(const char *token, size_t token_length)
{
    unsigned int index;

    for (index = 0; index < sizeof(known_wait_words) / sizeof(known_wait_words[0]); index++) {
        size_t word_length;

        word_length = strlen(known_wait_words[index]);
        if (token_length >= word_length && strncmp(token, known_wait_words[index], word_length) == 0 && (token_length == word_length || token[word_length] == ':')) {
            return 1;
        }
    }
    return 0;
}

static int wait_is_valid(const char *wait)
{
    while (*wait != '\0') {
        const char *end;

        end = strchr(wait, WAIT_TOKEN_SEPARATOR);
        if (end == NULL) {
            end = wait + strlen(wait);
        }
        if (!wait_word_is_known(wait, (size_t)(end - wait))) {
            return 0;
        }
        wait = *end == '\0' ? end : end + 1;
    }
    return 1;
}

static int read_step(struct script_route *route, const char *line)
{
    struct script_route_step *step;
    char action_word[ROUTE_ACTION_WORD_MAX];

    if (route->step_count >= SCRIPT_ROUTE_STEP_MAX) {
        return 0;
    }
    step = &route->steps[route->step_count];
    if (sscanf(line, ROUTE_LINE_SCAN, step->script, action_word, step->object, &step->slot, &step->dwell_ticks, step->wait) != 6) {
        return 0;
    }
    if (!read_action(action_word, &step->action)) {
        return 0;
    }
    if (strcmp(step->object, ROUTE_EMPTY_WORD) == 0) {
        step->object[0] = '\0';
    }
    if (strcmp(step->wait, ROUTE_EMPTY_WORD) == 0) {
        step->wait[0] = '\0';
    }
    if (!wait_is_valid(step->wait)) {
        return 0;
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

int script_route_wait_has_token(const char *wait, const char *token)
{
    size_t token_length;

    token_length = strlen(token);
    while (*wait != '\0') {
        const char *end;

        end = strchr(wait, WAIT_TOKEN_SEPARATOR);
        if (end == NULL) {
            end = wait + strlen(wait);
        }
        if ((size_t)(end - wait) == token_length && strncmp(wait, token, token_length) == 0) {
            return 1;
        }
        wait = *end == '\0' ? end : end + 1;
    }
    return 0;
}
