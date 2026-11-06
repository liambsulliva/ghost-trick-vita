#include "frame_stats.h"
#include "test_support.h"

static void test_frame_stats_summarizes_one_window(void)
{
    struct frame_stats stats;
    char text[FRAME_STATS_TEXT_MAX];

    frame_stats_reset(&stats);
    CHECK(!frame_stats_window_is_full(&stats));
    frame_stats_add_frame(&stats, 16000, 100, 200, 15000);
    frame_stats_add_frame(&stats, 24000, 300, 400, 20000);
    CHECK_INT(stats.frame_count, 2);
    CHECK_INT(stats.slow_frame_count, 1);
    CHECK_INT(stats.shortest_frame_us, 16000);
    CHECK_INT(stats.longest_frame_us, 24000);

    frame_stats_format(&stats, text, sizeof(text));
    CHECK_HAS_TEXT(text, "fps=50.0");
    CHECK_HAS_TEXT(text, "frame_us_avg=20000");
    CHECK_HAS_TEXT(text, "update_us=200");
    CHECK_HAS_TEXT(text, "present_us=17500");
}

static void test_frame_stats_only_counts_frames_over_the_slow_limit(void)
{
    struct frame_stats stats;

    frame_stats_reset(&stats);
    frame_stats_add_frame(&stats, 16000, 0, 0, 0);
    frame_stats_add_frame(&stats, 17000, 0, 0, 0);
    frame_stats_add_frame(&stats, FRAME_STATS_SLOW_FRAME_US, 0, 0, 0);
    CHECK_INT(stats.slow_frame_count, 0);
    frame_stats_add_frame(&stats, FRAME_STATS_SLOW_FRAME_US + 1, 0, 0, 0);
    CHECK_INT(stats.slow_frame_count, 1);
}

static void test_frame_stats_window_fills_after_one_second(void)
{
    struct frame_stats stats;
    int frame;

    frame_stats_reset(&stats);
    for (frame = 0; frame < 59; frame++) {
        frame_stats_add_frame(&stats, 16667, 0, 0, 0);
    }
    CHECK(!frame_stats_window_is_full(&stats));
    frame_stats_add_frame(&stats, 16667, 0, 0, 0);
    CHECK(frame_stats_window_is_full(&stats));
}

static void test_frame_stats_format_with_no_frames_reports_zero(void)
{
    struct frame_stats stats;
    char text[FRAME_STATS_TEXT_MAX];

    frame_stats_reset(&stats);
    frame_stats_format(&stats, text, sizeof(text));
    CHECK_HAS_TEXT(text, "fps=0.0");
    CHECK_HAS_TEXT(text, "frames=0");
}

int main(void)
{
    test_frame_stats_summarizes_one_window();
    test_frame_stats_only_counts_frames_over_the_slow_limit();
    test_frame_stats_window_fills_after_one_second();
    test_frame_stats_format_with_no_frames_reports_zero();
    return test_finish("unit/frame_stats");
}
