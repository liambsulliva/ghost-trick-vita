#ifndef FAKE_PSP2_APPUTIL_H
#define FAKE_PSP2_APPUTIL_H

typedef struct SceAppUtilInitParam {
    unsigned int workBufSize;
    unsigned char reserved[60];
} SceAppUtilInitParam;

typedef struct SceAppUtilBootParam {
    unsigned int attr;
    unsigned int appVersion;
    unsigned char reserved[32];
} SceAppUtilBootParam;

int sceAppUtilInit(SceAppUtilInitParam *initParam, SceAppUtilBootParam *bootParam);
int sceAppUtilSystemParamGetInt(unsigned int paramId, int *value);

#endif
