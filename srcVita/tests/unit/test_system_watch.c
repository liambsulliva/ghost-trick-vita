#include "fakes/fake_platform.h"
#include "fakes/fake_system_state.h"
#include "run_log.h"
#include "system_watch.h"
#include "test_support.h"

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static int count_in_log(const char *wanted)
{
    const char *found;
    int count;

    count = 0;
    found = strstr(log_text(), wanted);
    while (found != NULL) {
        count = count + 1;
        found = strstr(found + 1, wanted);
    }
    return count;
}

static void set_up(int overlay, int bgm_adopt)
{
    fake_files_reset();
    fake_clock_set_us(1000);
    run_log_open("run.log");
    fake_system_state_set(overlay, bgm_adopt);
    system_watch_start();
}

static void test_the_state_is_logged_once_at_the_start(void)
{
    set_up(0, 1);
    CHECK_HAS_TEXT(log_text(), "system state overlay=0 bgm_adopt=1");
    CHECK_INT(count_in_log("system state"), 1);
}

static void test_a_start_logs_even_when_the_state_matches_the_last_one(void)
{
    set_up(0, 1);
    system_watch_start();
    CHECK_INT(count_in_log("system state"), 2);
}

static void test_polling_an_unchanged_state_logs_nothing(void)
{
    set_up(0, 1);
    system_watch_poll();
    system_watch_poll();
    CHECK_INT(count_in_log("system state"), 1);
}

static void test_a_change_of_overlay_logs_one_line(void)
{
    set_up(0, 1);
    fake_system_state_set(1, 1);
    system_watch_poll();
    system_watch_poll();
    CHECK_HAS_TEXT(log_text(), "system state overlay=1 bgm_adopt=1");
    CHECK_INT(count_in_log("system state"), 2);
}

static void test_a_change_of_bgm_adopt_logs_one_line(void)
{
    set_up(0, 1);
    fake_system_state_set(0, 0);
    system_watch_poll();
    system_watch_poll();
    CHECK_HAS_TEXT(log_text(), "system state overlay=0 bgm_adopt=0");
    CHECK_INT(count_in_log("system state"), 2);
}

static void test_going_back_logs_again(void)
{
    set_up(0, 1);
    fake_system_state_set(1, 0);
    system_watch_poll();
    fake_system_state_set(0, 1);
    system_watch_poll();
    CHECK_INT(count_in_log("system state overlay=0 bgm_adopt=1"), 2);
    CHECK_INT(count_in_log("system state overlay=1 bgm_adopt=0"), 1);
}

int main(void)
{
    test_the_state_is_logged_once_at_the_start();
    test_a_start_logs_even_when_the_state_matches_the_last_one();
    test_polling_an_unchanged_state_logs_nothing();
    test_a_change_of_overlay_logs_one_line();
    test_a_change_of_bgm_adopt_logs_one_line();
    test_going_back_logs_again();
    fake_files_reset();
    return test_finish("unit/system_watch");
}
