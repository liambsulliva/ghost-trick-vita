#ifndef FAKE_PSP2_GXM_H
#define FAKE_PSP2_GXM_H

#define SCE_GXM_MULTISAMPLE_NONE 0

typedef struct SceGxmRenderTargetParams {
    unsigned int flags;
    unsigned short width;
    unsigned short height;
    unsigned short scenesPerFrame;
    unsigned short multisampleMode;
    unsigned int multisampleLocations;
    int driverMemBlock;
} SceGxmRenderTargetParams;

int sceGxmGetRenderTargetMemSize(const SceGxmRenderTargetParams *params, unsigned int *driver_memory_size);

#endif
