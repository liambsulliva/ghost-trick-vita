#include <string.h>
#include "build_info.h"
#include "device_stats.h"
#include "input.h"
#include "keep_awake.h"

void keep_awake_tick(void)
{
}

void input_poll(struct input_state *state)
{
    memset(state, 0, sizeof(*state));
    state->pressed = INPUT_START;
}

void device_stats_read(struct device_stats *stats)
{
    memset(stats, 0, sizeof(*stats));
}

const char *build_info_id(void)
{
    return "player";
}

int sceKernelExitProcess(int result)
{
    return result;
}

int sceAppMgrLoadExec(const char *path, char * const arguments[], const void *options)
{
    (void)path;
    (void)arguments;
    (void)options;
    return 0;
}
