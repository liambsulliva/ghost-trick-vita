#include "run_log.h"
#include "system_state.h"
#include "system_watch.h"

#define LOG_FOR_TOOLS_SYSTEM_STATE "system state overlay=%d bgm_adopt=%d"

static struct system_state last_logged;

static void log_state(const struct system_state *state)
{
    last_logged = *state;
    run_log_write(LOG_FOR_TOOLS_SYSTEM_STATE, state->overlay, state->bgm_adopt);
}

void system_watch_start(void)
{
    struct system_state state;

    system_state_read(&state);
    log_state(&state);
}

void system_watch_poll(void)
{
    struct system_state state;

    system_state_read(&state);
    if (state.overlay != last_logged.overlay || state.bgm_adopt != last_logged.bgm_adopt) {
        log_state(&state);
    }
}
