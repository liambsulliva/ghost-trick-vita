#include <stdio.h>
#include <string.h>
#include "script_route.h"
#include "text_lines.h"

#define ROUTE_LINE_MAX 384
#define ROUTE_LINE_SCAN "step %95s %15s %47s %d %d %159s"
#define PACING_LINE_SCAN "dwell %d %d"
#define SCENE_TIMEOUT_LINE_SCAN "scene_most_ticks %d"
#define SCENE_TIMEOUT_WORD "scene_most_ticks "
#define SCENE_LINE_SCAN "scene %95s"
#define ROUTE_ACTION_WORD_MAX 16
#define ROUTE_EMPTY_WORD "-"
#define WAIT_TOKEN_SEPARATOR '&'
#define WAIT_ARGUMENT_SEPARATOR ':'

static const char *const action_names[] = { "move", "trick", "ghost_on", "ghost_off", "touch", "tap_balloon", "camera", "phone_call", "wait", "dead_talk" };
static const char *const wait_word_names[] = { "act", "tutorial", "playable", "free", "view", "no_talk", "flag", "noflag", "core", "usable", "balloon", "camera_tutorial", "timeleap_armed", "ticks" };

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

static enum script_route_wait_word wait_word_named(const char *name)
{
    unsigned int index;

    for (index = 0; index < sizeof(wait_word_names) / sizeof(wait_word_names[0]); index++) {
        if (strcmp(name, wait_word_names[index]) == 0) {
            return (enum script_route_wait_word)index;
        }
    }
    return SCRIPT_ROUTE_WAIT_UNKNOWN;
}

static char *split_off_argument(char *text)
{
    char *separator;

    separator = strchr(text, WAIT_ARGUMENT_SEPARATOR);
    if (separator == NULL) {
        return NULL;
    }
    *separator = '\0';
    return separator + 1;
}

const char *script_route_read_wait_token(const char *wait, struct script_route_wait_token *token)
{
    const char *end;
    size_t length;
    char *first;
    char *second;

    end = strchr(wait, WAIT_TOKEN_SEPARATOR);
    length = end != NULL ? (size_t)(end - wait) : strlen(wait);
    if (length >= sizeof(token->text)) {
        length = sizeof(token->text) - 1u;
    }
    memcpy(token->text, wait, length);
    token->text[length] = '\0';
    first = split_off_argument(token->text);
    second = first != NULL ? split_off_argument(first) : NULL;
    token->first = first;
    token->second = second;
    token->word = wait_word_named(token->text);
    return end != NULL ? end + 1 : NULL;
}

static int wait_is_valid(const char *wait)
{
    struct script_route_wait_token token;

    while (wait != NULL && *wait != '\0') {
        wait = script_route_read_wait_token(wait, &token);
        if (token.word == SCRIPT_ROUTE_WAIT_UNKNOWN) {
            return 0;
        }
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
    route->scene_most_ticks = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (strncmp(line, SCENE_TIMEOUT_WORD, strlen(SCENE_TIMEOUT_WORD)) == 0) {
            if (sscanf(line, SCENE_TIMEOUT_LINE_SCAN, &route->scene_most_ticks) != 1 || route->scene_most_ticks < 0) {
                return 0;
            }
        } else if (line[0] != '\0' && !read_step(route, line)) {
            route->step_count = 0;
            return 0;
        }
    }
    return 1;
}

int script_route_read_scenes(struct script_route *route, const char *text, unsigned int text_size)
{
    char line[ROUTE_LINE_MAX];
    unsigned int offset;

    offset = 0;
    route->scene_count = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (route->scene_count >= SCRIPT_ROUTE_SCENE_MAX || sscanf(line, SCENE_LINE_SCAN, route->scenes[route->scene_count]) != 1) {
            route->scene_count = 0;
            return 0;
        }
        route->scene_count = route->scene_count + 1;
    }
    return 1;
}

int script_route_scene_index(const struct script_route *route, const char *script)
{
    int index;

    for (index = 0; index < route->scene_count; index++) {
        if (strcmp(route->scenes[index], script) == 0) {
            return index;
        }
    }
    return -1;
}

int script_route_read_pacing(struct script_route *route, const char *text, unsigned int text_size)
{
    char line[ROUTE_LINE_MAX];
    unsigned int offset;
    int step_index;
    int dwell_ticks;

    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (sscanf(line, PACING_LINE_SCAN, &step_index, &dwell_ticks) != 2 || step_index < 0 || step_index >= route->step_count || dwell_ticks < 0) {
            return 0;
        }
        route->steps[step_index].dwell_ticks = dwell_ticks;
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
