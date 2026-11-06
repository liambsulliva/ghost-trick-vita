#ifndef DEVICE_STATS_H
#define DEVICE_STATS_H

struct device_stats {
    unsigned int video_memory_free;
    unsigned int video_memory_total;
    unsigned int main_memory_free;
    unsigned int main_memory_total;
    unsigned int heap_used;
};

void device_stats_read(struct device_stats *stats);

#endif
