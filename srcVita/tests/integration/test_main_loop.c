#define main app_main
#include "../../src/main.c"
#undef main
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_system_state.h"
#include "test_support.h"

#define FRAME_US 16667ULL
#define TICKS_REMEMBERED 64
#define NEVER -1

static int frames_polled;
static int frame_that_quits;
static int frame_that_changes_the_state;
static int tick_count;
static int ticks_at_frame[TICKS_REMEMBERED];
static int log_existed_at_first_tick;

void keep_awake_tick(void)
{
    if (tick_count == 0) {
        log_existed_at_first_tick = fake_files_exists(LOG_PATH);
    }
    if (tick_count < TICKS_REMEMBERED) {
        ticks_at_frame[tick_count] = frames_polled;
    }
    tick_count = tick_count + 1;
}

void input_poll(struct input_state *state)
{
    fake_clock_advance_us(FRAME_US);
    state->pressed = frames_polled == frame_that_quits ? INPUT_START : 0;
    state->touch_is_down = 0;
    if (frames_polled == frame_that_changes_the_state) {
        fake_system_state_set(1, 0);
    }
    frames_polled = frames_polled + 1;
}

void device_stats_read(struct device_stats *stats)
{
    memset(stats, 0, sizeof(*stats));
}

const char *build_info_id(void)
{
    return "test";
}

int screen_capture_save_bmp(const char *path)
{
    (void)path;
    return 0;
}

void screen_watch_start(void)
{
}

void screen_watch_note_frame(void)
{
}

void screen_watch_publish(void)
{
}

int sceKernelExitProcess(int result)
{
    return result;
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents(LOG_PATH, &size);
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

static void run_the_app_for(int frames, int change_state_at_frame)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    fake_reader_reset();
    fake_renderer_reset();
    fake_clock_set_us(1000);
    fake_clock_set_tick_per_read_us(0);
    fake_system_state_set(0, 1);
    frames_polled = 0;
    frame_that_quits = frames;
    frame_that_changes_the_state = change_state_at_frame;
    tick_count = 0;
    log_existed_at_first_tick = 0;
    CHECK_INT(app_main(), 0);
}

static void test_the_first_tick_comes_before_anything_else_starts(void)
{
    run_the_app_for(1, NEVER);
    CHECK_INT(tick_count, 2);
    CHECK(!log_existed_at_first_tick);
    CHECK_INT(ticks_at_frame[0], 0);
}

static void test_the_loop_ticks_from_its_first_frame_and_then_every_30_frames(void)
{
    run_the_app_for(61, NEVER);
    CHECK_INT(tick_count, 4);
    CHECK_INT(ticks_at_frame[1], 0);
    CHECK_INT(ticks_at_frame[2], 30);
    CHECK_INT(ticks_at_frame[3], 60);
}

static void test_the_system_state_is_logged_at_startup_with_the_build_line_first(void)
{
    const char *build_line;
    const char *state_line;
    const char *renderer_line;

    run_the_app_for(1, NEVER);
    build_line = strstr(log_text(), "build id=test");
    state_line = strstr(log_text(), "system state overlay=0 bgm_adopt=1");
    renderer_line = strstr(log_text(), "renderer ready");
    CHECK(build_line != NULL && state_line != NULL && renderer_line != NULL);
    CHECK(build_line < state_line);
    CHECK(state_line < renderer_line);
    CHECK_INT(count_in_log("system state"), 1);
}

static void test_a_changed_system_state_shows_up_when_the_frame_stats_are_published(void)
{
    run_the_app_for(110, 100);
    CHECK_INT(count_in_log("system state"), 1);
    run_the_app_for(200, 100);
    CHECK_INT(count_in_log("system state"), 2);
    CHECK_HAS_TEXT(log_text(), "system state overlay=1 bgm_adopt=0");
}

int main(void)
{
    test_the_first_tick_comes_before_anything_else_starts();
    test_the_loop_ticks_from_its_first_frame_and_then_every_30_frames();
    test_the_system_state_is_logged_at_startup_with_the_build_line_first();
    test_a_changed_system_state_shows_up_when_the_frame_stats_are_published();
    fake_audio_reset();
    fake_files_reset();
    return test_finish("integration/main_loop");
}
