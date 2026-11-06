#include <psp2/kernel/processmgr.h>
#include "keep_awake.h"

#define KEEP_AWAKE_TICK_KIND SCE_KERNEL_POWER_TICK_DEFAULT

void keep_awake_tick(void)
{
    sceKernelPowerTick(KEEP_AWAKE_TICK_KIND);
}
