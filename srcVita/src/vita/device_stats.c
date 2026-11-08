#include <malloc.h>
#include <vitaGL.h>
#include "device_stats.h"
#include "memory.h"

void device_stats_read(struct device_stats *stats)
{
    stats->video_memory_free = (unsigned int)vglMemFree(VGL_MEM_VRAM);
    stats->video_memory_total = (unsigned int)vglMemTotal(VGL_MEM_VRAM);
    stats->main_memory_free = (unsigned int)vglMemFree(VGL_MEM_RAM);
    stats->main_memory_total = (unsigned int)vglMemTotal(VGL_MEM_RAM);
    stats->heap_used = (unsigned int)mallinfo().uordblks;
    stats->heap_size = MEMORY_HEAP_BYTES;
}
