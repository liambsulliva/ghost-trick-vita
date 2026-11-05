#ifndef FRAME_STATS_H
#define FRAME_STATS_H

#define FRAME_STATS_WINDOW_US 1000000ULL
#define FRAME_STATS_SLOW_FRAME_US 20000ULL
#define FRAME_STATS_TEXT_MAX 256

struct frame_stats {
    unsigned int frame_count;
    unsigned int slow_frame_count;
    unsigned long long total_frame_us;
    unsigned long long shortest_frame_us;
    unsigned long long longest_frame_us;
    unsigned long long total_update_us;
    unsigned long long total_draw_us;
    unsigned long long total_present_us;
};

void frame_stats_reset(struct frame_stats *stats);
void frame_stats_add_frame(struct frame_stats *stats, unsigned long long frame_us, unsigned long long update_us, unsigned long long draw_us, unsigned long long present_us);
int frame_stats_window_is_full(const struct frame_stats *stats);
void frame_stats_format(const struct frame_stats *stats, char *text, unsigned int text_size);

#endif
