#ifndef SCRIPT_ROUTE_H
#define SCRIPT_ROUTE_H

#define SCRIPT_ROUTE_STEP_MAX 4096
#define SCRIPT_ROUTE_SCENE_MAX 512
#define SCRIPT_ROUTE_SCRIPT_NAME_MAX 96
#define SCRIPT_ROUTE_NAME_MAX 48
#define SCRIPT_ROUTE_WAIT_MAX 160

enum script_route_action {
    SCRIPT_ROUTE_MOVE,
    SCRIPT_ROUTE_TRICK,
    SCRIPT_ROUTE_GHOST_ON,
    SCRIPT_ROUTE_GHOST_OFF,
    SCRIPT_ROUTE_TOUCH,
    SCRIPT_ROUTE_TAP_BALLOON,
    SCRIPT_ROUTE_CAMERA,
    SCRIPT_ROUTE_PHONE_CALL,
    SCRIPT_ROUTE_WAIT,
    SCRIPT_ROUTE_DEAD_TALK,
    SCRIPT_ROUTE_CHOOSE,
    SCRIPT_ROUTE_CHARACTER
};

enum script_route_wait_word {
    SCRIPT_ROUTE_WAIT_ACT,
    SCRIPT_ROUTE_WAIT_TUTORIAL,
    SCRIPT_ROUTE_WAIT_PLAYABLE,
    SCRIPT_ROUTE_WAIT_FREE,
    SCRIPT_ROUTE_WAIT_VIEW,
    SCRIPT_ROUTE_WAIT_NO_TALK,
    SCRIPT_ROUTE_WAIT_FLAG,
    SCRIPT_ROUTE_WAIT_NOFLAG,
    SCRIPT_ROUTE_WAIT_CORE,
    SCRIPT_ROUTE_WAIT_USABLE,
    SCRIPT_ROUTE_WAIT_BALLOON,
    SCRIPT_ROUTE_WAIT_CAMERA_TUTORIAL,
    SCRIPT_ROUTE_WAIT_TIMELEAP_ARMED,
    SCRIPT_ROUTE_WAIT_TICKS,
    SCRIPT_ROUTE_WAIT_CHOICE,
    SCRIPT_ROUTE_WAIT_REACH,
    SCRIPT_ROUTE_WAIT_UNKNOWN
};

struct script_route_wait_token {
    enum script_route_wait_word word;
    char text[SCRIPT_ROUTE_WAIT_MAX];
    const char *first;
    const char *second;
};

struct script_route_step {
    char script[SCRIPT_ROUTE_SCRIPT_NAME_MAX];
    enum script_route_action action;
    char object[SCRIPT_ROUTE_NAME_MAX];
    int slot;
    int dwell_ticks;
    char wait[SCRIPT_ROUTE_WAIT_MAX];
};

struct script_route {
    struct script_route_step steps[SCRIPT_ROUTE_STEP_MAX];
    int step_count;
    int scene_most_ticks;
    char scenes[SCRIPT_ROUTE_SCENE_MAX][SCRIPT_ROUTE_SCRIPT_NAME_MAX];
    int scene_count;
    unsigned int steps_text_crc;
    int pacing_is_stale;
};

int script_route_read(struct script_route *route, const char *text, unsigned int text_size);
int script_route_read_pacing(struct script_route *route, const char *text, unsigned int text_size);
int script_route_read_scenes(struct script_route *route, const char *text, unsigned int text_size);
int script_route_scene_index(const struct script_route *route, const char *script);
int script_route_first_step_of_script(const struct script_route *route, const char *script);
const char *script_route_action_name(enum script_route_action action);
int script_route_wait_has_token(const char *wait, const char *token);
const char *script_route_read_wait_token(const char *wait, struct script_route_wait_token *token);

#endif
