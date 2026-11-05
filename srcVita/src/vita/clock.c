#include <psp2/kernel/processmgr.h>
#include "timing.h"

unsigned long long timing_now_us(void)
{
    return sceKernelGetProcessTimeWide();
}
