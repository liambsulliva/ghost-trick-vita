#include "fakes/fake_platform.h"
#include "run_log.h"
#include "test_support.h"

static void test_run_log_stamps_time_and_appends(void)
{
    const unsigned char *contents;
    unsigned int size;

    fake_files_reset();
    fake_clock_set_us(5000000);
    run_log_open("log.txt");
    fake_clock_advance_us(1500000);
    run_log_write("hello %d", 7);
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

int main(void)
{
    test_run_log_stamps_time_and_appends();
    test_run_log_keeps_earlier_runs();
    test_run_log_cuts_oversized_lines_safely();
    test_run_log_survives_write_failures();
    fake_files_reset();
    return test_finish("unit/run_log");
}
