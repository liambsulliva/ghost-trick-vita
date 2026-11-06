#define system_state_read real_system_state_read
#include "../../src/vita/system_state.c"
#undef system_state_read
#include "../../src/vita/keep_awake.c"
#include "test_support.h"

#define NOT_CALLED -1
#define EXPECTED_APP_STATE_SDK_VERSION 0x03600011u
#define APP_STATE_READ_FAILURE ((int)0x80010058)
#define BGM_ADOPT 7

static int last_tick_kind = NOT_CALLED;
static int tick_count;
static int app_state_result;
static int overlay_flag;
static unsigned int last_state_size;
static unsigned int last_state_version;
static int last_adopt_port = NOT_CALLED;

int sceKernelPowerTick(SceKernelPowerTickType type)
{
    last_tick_kind = (int)type;
    tick_count = tick_count + 1;
    return 0;
}

int _sceAppMgrGetAppState(SceAppMgrAppState *appState, unsigned int len, unsigned int version)
{
    last_state_size = len;
    last_state_version = version;
    if (app_state_result >= 0) {
        appState->isSystemUiOverlaid = overlay_flag;
    }
    return app_state_result;
}

int sceAudioOutGetAdopt(SceAudioOutPortType type)
{
    last_adopt_port = (int)type;
    return BGM_ADOPT;
}

static void test_a_tick_cancels_every_timer_including_dimming(void)
{
    keep_awake_tick();
    CHECK_INT(tick_count, 1);
    CHECK_INT(last_tick_kind, SCE_KERNEL_POWER_TICK_DEFAULT);
}

static void test_the_state_comes_from_the_app_manager_and_the_bgm_port(void)
{
    struct system_state state;

    overlay_flag = 1;
    app_state_result = 0;
    real_system_state_read(&state);
    CHECK_INT(state.overlay, 1);
    CHECK_INT(state.bgm_adopt, BGM_ADOPT);
    CHECK_INT(last_adopt_port, SCE_AUDIO_OUT_PORT_TYPE_BGM);
    CHECK_INT(last_state_size, sizeof(SceAppMgrAppState));
    CHECK_INT(last_state_version, EXPECTED_APP_STATE_SDK_VERSION);
}

static void test_a_failed_read_says_so_rather_than_claiming_no_overlay(void)
{
    struct system_state state;

    app_state_result = APP_STATE_READ_FAILURE;
    real_system_state_read(&state);
    CHECK_INT(state.overlay, SYSTEM_STATE_UNREADABLE);
    CHECK_INT(state.bgm_adopt, BGM_ADOPT);
}

int main(void)
{
    test_a_tick_cancels_every_timer_including_dimming();
    test_the_state_comes_from_the_app_manager_and_the_bgm_port();
    test_a_failed_read_says_so_rather_than_claiming_no_overlay();
    return test_finish("unit/vita_calls");
}
