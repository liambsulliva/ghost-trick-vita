#include <stdio.h>
#include "frame_stats.h"

static unsigned long long average_us(unsigned long long total_us, unsigned int count)
{
    if (count == 0) {
        return 0;
    }
    return total_us / count;
}

void frame_stats_reset(struct frame_stats *stats)
{
    stats->frame_count = 0;
    stats->slow_frame_count = 0;
    stats->total_frame_us = 0;
    stats->shortest_frame_us = 0;
    stats->longest_frame_us = 0;
    stats->total_update_us = 0;
    stats->total_draw_us = 0;
    stats->total_present_us = 0;
}

void frame_stats_add_frame(struct frame_stats *stats, unsigned long long frame_us, unsigned long long update_us, unsigned long long draw_us, unsigned long long present_us)
{
    if (stats->frame_count == 0 || frame_us < stats->shortest_frame_us) {
        stats->shortest_frame_us = frame_us;
    }
    if (frame_us > stats->longest_frame_us) {
        stats->longest_frame_us = frame_us;
    }
    if (frame_us > FRAME_STATS_SLOW_FRAME_US) {
        stats->slow_frame_count = stats->slow_frame_count + 1;
    }
    stats->frame_count = stats->frame_count + 1;
    stats->total_frame_us = stats->total_frame_us + frame_us;
    stats->total_update_us = stats->total_update_us + update_us;
    stats->total_draw_us = stats->total_draw_us + draw_us;
    stats->total_present_us = stats->total_present_us + present_us;
}

int frame_stats_window_is_full(const struct frame_stats *stats)
{
    return stats->total_frame_us >= FRAME_STATS_WINDOW_US;
}

void frame_stats_format(const struct frame_stats *stats, char *text, unsigned int text_size)
{
    unsigned long long frames_per_second_x10;

    frames_per_second_x10 = 0;
    if (stats->total_frame_us > 0) {
        frames_per_second_x10 = (unsigned long long)stats->frame_count * 10000000ULL / stats->total_frame_us;
    }
    snprintf(text, text_size,
        "stats fps=%llu.%llu frames=%u slow=%u frame_us_min=%llu frame_us_avg=%llu frame_us_max=%llu update_us=%llu draw_us=%llu present_us=%llu",
        frames_per_second_x10 / 10,
        frames_per_second_x10 % 10,
        stats->frame_count,
        stats->slow_frame_count,
        stats->shortest_frame_us,
        average_us(stats->total_frame_us, stats->frame_count),
        stats->longest_frame_us,
        average_us(stats->total_update_us, stats->frame_count),
        average_us(stats->total_draw_us, stats->frame_count),
        average_us(stats->total_present_us, stats->frame_count));
}
