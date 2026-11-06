#ifndef FAKE_PSP2_APPMGR_H
#define FAKE_PSP2_APPMGR_H

typedef struct SceAppMgrAppState {
    unsigned int systemEventNum;
    unsigned int appEventNum;
    int isSystemUiOverlaid;
    unsigned char reserved[116];
} SceAppMgrAppState;

int _sceAppMgrGetAppState(SceAppMgrAppState *appState, unsigned int len, unsigned int version);

#endif
