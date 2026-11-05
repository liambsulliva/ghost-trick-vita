#include "bmp_image.h"
#include "command_file.h"
#include "fakes/fake_platform.h"
#include "file.h"
#include "file_path.h"
#include "frame_stats.h"
#include "run_log.h"
#include "test_pattern.h"
#include "test_support.h"
#include "timing.h"

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

static void test_bmp_rows_are_padded_to_four_bytes(void)
{
    CHECK_INT(bmp_row_size(1), 4);
    CHECK_INT(bmp_row_size(4), 12);
    CHECK_INT(bmp_row_size(960), 2880);
    CHECK_INT(bmp_file_size(2, 2), 54 + 8 * 2);
}

static void test_bmp_header_describes_the_image(void)
{
    unsigned char header[BMP_HEADER_SIZE];

    bmp_write_header(header, 960, 544);
    CHECK_INT(header[0], 'B');
    CHECK_INT(header[1], 'M');
    CHECK_INT(header[18] + header[19] * 256, 960);
    CHECK_INT(header[22] + header[23] * 256, 544);
    CHECK_INT(header[26], 1);
    CHECK_INT(header[28], 24);
}

static void test_bmp_row_swaps_red_and_blue_and_drops_alpha(void)
{
    const unsigned char rgba_pixels[] = { 10, 20, 30, 255 };
    unsigned char row[4] = { 9, 9, 9, 9 };

    bmp_write_row_from_rgba(row, rgba_pixels, 1);
    CHECK_INT(row[0], 30);
    CHECK_INT(row[1], 20);
    CHECK_INT(row[2], 10);
    CHECK_INT(row[3], 0);
}

static void test_command_names_only_allow_plain_characters(void)
{
    char too_long[COMMAND_NAME_MAX + 2];

    CHECK(command_name_is_safe("first_frames-2"));
    CHECK(!command_name_is_safe(""));
    CHECK(!command_name_is_safe("../secret"));
    CHECK(!command_name_is_safe("a b"));
    memset(too_long, 'a', sizeof(too_long) - 1);
    too_long[sizeof(too_long) - 1] = '\0';
    CHECK(!command_name_is_safe(too_long));
}

static void test_command_file_parses_each_line_in_order(void)
{
    const char text[] = "capture one\r\n\nquit\nbogus\ncapture ../two\nshow_background room\nshow_animation walk_1";
    unsigned int size;
    unsigned int offset;
    struct command command;

    size = sizeof(text) - 1;
    offset = command_parse_next_line(text, size, 0, &command);
    CHECK_INT(command.kind, COMMAND_CAPTURE);
    CHECK_TEXT(command.name, "one");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_NONE);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_QUIT);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_UNKNOWN);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_UNKNOWN);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_SHOW_BACKGROUND);
    CHECK_TEXT(command.name, "room");
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_SHOW_ANIMATION);
    CHECK_TEXT(command.name, "walk_1");
    CHECK_INT(offset, size);
    offset = command_parse_next_line(text, size, offset, &command);
    CHECK_INT(command.kind, COMMAND_NONE);
    CHECK_INT(offset, size);
}

static void test_run_log_stamps_time_and_appends(void)
{
    const unsigned char *contents;
    unsigned int size;

    fake_files_reset();
    fake_clock_set_us(5000000);
    run_log_open("log.txt");
    fake_clock_advance_us(1500000);
    run_log_write("hello %d", 7);
    CHECK_INT(run_log_lines_written(), 2);
    contents = fake_files_contents("log.txt", &size);
    CHECK(contents != NULL);
    CHECK_HAS_TEXT((const char *)contents, "--- launch\n");
    CHECK_HAS_TEXT((const char *)contents, "[      1.500] hello 7\n");
}

static void test_run_log_keeps_earlier_runs(void)
{
    const unsigned char *contents;
    unsigned int size;

    fake_files_reset();
    fake_files_add("log.txt", (const unsigned char *)"old run\n", 8);
    run_log_open("log.txt");
    contents = fake_files_contents("log.txt", &size);
    CHECK_HAS_TEXT((const char *)contents, "old run\n");
    CHECK_HAS_TEXT((const char *)contents, "--- launch\n");
}

static void test_run_log_cuts_oversized_lines_safely(void)
{
    char long_text[RUN_LOG_LINE_MAX * 2];
    const unsigned char *contents;
    unsigned int size;

    fake_files_reset();
    memset(long_text, 'x', sizeof(long_text) - 1);
    long_text[sizeof(long_text) - 1] = '\0';
    run_log_open("log.txt");
    run_log_write("%s", long_text);
    contents = fake_files_contents("log.txt", &size);
    CHECK(contents != NULL);
    CHECK(size < RUN_LOG_LINE_MAX + 64);
}

static void test_run_log_survives_write_failures(void)
{
    fake_files_reset();
    fake_files_fail_writes(1);
    run_log_open("log.txt");
    run_log_write("still running");
    CHECK(!fake_files_exists("log.txt"));
    fake_files_fail_writes(0);
}

static void test_checker_pattern_alternates_tiles(void)
{
    unsigned char pixels[4 * 4 * 4];

    test_pattern_fill_checker(pixels, 4, 2);
    CHECK_INT(pixels[0], 40);
    CHECK_INT(pixels[2 * 4], 255);
    CHECK_INT(pixels[(2 * 4 + 2) * 4], 40);
    CHECK_INT(pixels[3], 255);
}

static void test_fixed_step_counts_steps_and_drops_long_stalls(void)
{
    struct fixed_step step;

    fixed_step_init(&step, 1000);
    CHECK_INT(fixed_step_feed(&step, 999), 0);
    CHECK_INT(fixed_step_feed(&step, 1), 1);
    CHECK_INT(fixed_step_feed(&step, 2500), 2);
    CHECK_INT(step.accumulator_us, 500);
    CHECK_INT(fixed_step_feed(&step, 100000), FIXED_STEP_MAX_PER_FEED);
    CHECK_INT(step.accumulator_us, 0);
}

static void test_paths_join_and_refuse_to_overflow(void)
{
    char path[FILE_PATH_MAX];
    char long_name[FILE_PATH_MAX];

    CHECK(file_path_join(path, "a/b", "c", ".txt"));
    CHECK_TEXT(path, "a/b/c.txt");
    memset(long_name, 'x', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    CHECK(!file_path_join(path, "a", long_name, ""));
    CHECK_TEXT(path, "");
}

static void test_frame_timer_measures_elapsed_time(void)
{
    struct frame_timer timer;

    fake_clock_set_us(1000);
    frame_timer_start(&timer);
    fake_clock_advance_us(16667);
    CHECK_INT(frame_timer_tick(&timer), 16667);
    fake_clock_advance_us(1);
    CHECK_INT(frame_timer_tick(&timer), 1);
}

int main(void)
{
    test_frame_stats_summarizes_one_window();
    test_frame_stats_only_counts_frames_over_the_slow_limit();
    test_frame_stats_window_fills_after_one_second();
    test_frame_stats_format_with_no_frames_reports_zero();
    test_bmp_rows_are_padded_to_four_bytes();
    test_bmp_header_describes_the_image();
    test_bmp_row_swaps_red_and_blue_and_drops_alpha();
    test_command_names_only_allow_plain_characters();
    test_command_file_parses_each_line_in_order();
    test_run_log_stamps_time_and_appends();
    test_run_log_keeps_earlier_runs();
    test_run_log_cuts_oversized_lines_safely();
    test_run_log_survives_write_failures();
    test_checker_pattern_alternates_tiles();
    test_fixed_step_counts_steps_and_drops_long_stalls();
    test_frame_timer_measures_elapsed_time();
    test_paths_join_and_refuse_to_overflow();
    fake_files_reset();
    return test_finish("unit/foundations");
}
