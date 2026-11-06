#ifndef SCRIPT_ROUTE_H
#define SCRIPT_ROUTE_H

#define SCRIPT_ROUTE_STEP_MAX 256
#define SCRIPT_ROUTE_SCRIPT_NAME_MAX 96
#define SCRIPT_ROUTE_NAME_MAX 48
#define SCRIPT_ROUTE_NO_SLOT 0

enum script_route_action {
    SCRIPT_ROUTE_MOVE,
    SCRIPT_ROUTE_TRICK,
    SCRIPT_ROUTE_GHOST_ON,
    SCRIPT_ROUTE_GHOST_OFF,
    SCRIPT_ROUTE_TAP_LEFT,
    SCRIPT_ROUTE_TAP_RIGHT,
    SCRIPT_ROUTE_WAIT
};

enum script_route_wait {
    SCRIPT_ROUTE_WAIT_NOTHING,
    SCRIPT_ROUTE_WAIT_PLAYABLE,
    SCRIPT_ROUTE_WAIT_TUTORIAL_TOUCH,
    SCRIPT_ROUTE_WAIT_TUTORIAL_MOVE,
    SCRIPT_ROUTE_WAIT_ACT,
    SCRIPT_ROUTE_WAIT_CORE
};

struct script_route_step {
    char script[SCRIPT_ROUTE_SCRIPT_NAME_MAX];
    enum script_route_action action;
    char object[SCRIPT_ROUTE_NAME_MAX];
    int slot;
    enum script_route_wait wait;
    char wait_name[SCRIPT_ROUTE_NAME_MAX];
    int dwell_ticks;
};

struct script_route {
    struct script_route_step steps[SCRIPT_ROUTE_STEP_MAX];
    int step_count;
};

int script_route_read(struct script_route *route, const char *text, unsigned int text_size);
int script_route_first_step_of_script(const struct script_route *route, const char *script);
const char *script_route_action_name(enum script_route_action action);

#endif
