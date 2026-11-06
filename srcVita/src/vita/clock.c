#include <psp2/kernel/processmgr.h>
#include "clock.h"

unsigned long long clock_now_us(void)
{
    return sceKernelGetProcessTimeWide();
}
