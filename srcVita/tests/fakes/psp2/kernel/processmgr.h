#ifndef FAKE_PSP2_KERNEL_PROCESSMGR_H
#define FAKE_PSP2_KERNEL_PROCESSMGR_H

typedef enum SceKernelPowerTickType {
    SCE_KERNEL_POWER_TICK_DEFAULT = 0,
    SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND = 1
} SceKernelPowerTickType;

int sceKernelPowerTick(SceKernelPowerTickType type);
int sceKernelExitProcess(int result);

#endif
