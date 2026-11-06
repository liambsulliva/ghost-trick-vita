#include <string.h>
#include <psp2/appmgr.h>
#include <psp2/audioout.h>
#include "system_state.h"

#define APP_STATE_SDK_VERSION 0x03600011

void system_state_read(struct system_state *state)
{
    SceAppMgrAppState app_state;

    memset(&app_state, 0, sizeof(app_state));
    if (_sceAppMgrGetAppState(&app_state, sizeof(app_state), APP_STATE_SDK_VERSION) < 0) {
        state->overlay = SYSTEM_STATE_UNREADABLE;
    } else {
        state->overlay = app_state.isSystemUiOverlaid;
    }
    state->bgm_adopt = sceAudioOutGetAdopt(SCE_AUDIO_OUT_PORT_TYPE_BGM);
}
