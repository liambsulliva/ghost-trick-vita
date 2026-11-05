#include <stddef.h>
#include <stdio.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include "command_file.h"
#include "device_stats.h"
#include "file.h"
#include "frame_stats.h"
#include "input.h"
#include "memory.h"
#include "renderer.h"
#include "run_log.h"
#include "screen_capture.h"
#include "test_pattern.h"
#include "timing.h"

#define CHECKER_SIZE 64
#define CHECKER_TILE 8
#define COMMAND_POLL_FRAMES 30
#define FIRST_CAPTURE_FRAME 120

#define LOG_PATH FILE_DATA_FOLDER "/run.log"
#define COMMANDS_PATH FILE_DATA_FOLDER "/commands.txt"
#define CAPTURE_FOLDER FILE_DATA_FOLDER "/captures"

static void log_device_stats(void)
{
    struct device_stats stats;

    device_stats_read(&stats);
    run_log_write("memory video_free=%u video_total=%u main_free=%u main_total=%u",
        stats.video_memory_free, stats.video_memory_total, stats.main_memory_free, stats.main_memory_total);
}

static void save_capture(const char *name)
{
    char path[FILE_PATH_MAX];
    unsigned long long started_us;
    int result;

    snprintf(path, sizeof(path), "%s/%s.bmp", CAPTURE_FOLDER, name);
    started_us = timing_now_us();
    result = screen_capture_save_bmp(path);
    run_log_write("capture name=%s result=%d took_us=%llu", name, result, timing_now_us() - started_us);
}

static int run_pending_commands(void)
{
    unsigned char *text;
    unsigned int text_size;
    unsigned int offset;
    struct command command;
    int quit_requested;

    if (file_read_whole(COMMANDS_PATH, &text, &text_size) != FILE_OK) {
        return 0;
    }
    file_delete(COMMANDS_PATH);

    quit_requested = 0;
    offset = 0;
    while (offset < text_size) {
        offset = command_parse_next_line((const char *)text, text_size, offset, &command);
        if (command.kind == COMMAND_CAPTURE) {
            save_capture(command.name);
        } else if (command.kind == COMMAND_QUIT) {
            run_log_write("command quit");
            quit_requested = 1;
        } else if (command.kind == COMMAND_UNKNOWN) {
            run_log_write("command unknown");
        }
    }
    mem_free(text);
    return quit_requested;
}

int main(void)
{
    struct input_state input;
    struct frame_stats stats;
    struct frame_timer frame_timer;
    unsigned char *pixels;
    GLuint texture;
    unsigned int frame_number;
    unsigned long long frame_started_us;
    unsigned long long update_ended_us;
    unsigned long long draw_ended_us;
    unsigned long long present_ended_us;
    unsigned long long frame_us;
    char stats_text[FRAME_STATS_TEXT_MAX];
    int quit_requested;

    file_make_folder(FILE_DATA_FOLDER);
    file_make_folder(CAPTURE_FOLDER);
    run_log_open(LOG_PATH);
    run_log_write("build id=%s", BUILD_ID);

    run_log_write("renderer ready full_resolution=%d", renderer_init());
    log_device_stats();

    pixels = mem_alloc(CHECKER_SIZE * CHECKER_SIZE * 4);
    if (pixels == NULL) {
        run_log_write("fatal checker allocation failed");
        renderer_shutdown();
        sceKernelExitProcess(1);
        return 1;
    }
    test_pattern_fill_checker(pixels, CHECKER_SIZE, CHECKER_TILE);
    texture = renderer_texture_create(pixels, CHECKER_SIZE, CHECKER_SIZE);
    mem_free(pixels);
    run_log_write("texture ready id=%u gl_error=%d", texture, (int)glGetError());

    input.held = 0;
    input.pressed = 0;
    frame_number = 0;
    quit_requested = 0;
    frame_stats_reset(&stats);
    frame_timer_start(&frame_timer);

    while (!quit_requested) {
        frame_started_us = timing_now_us();
        input_poll(&input);
        if (input.pressed & SCE_CTRL_START) {
            run_log_write("input start pressed");
            break;
        }
        update_ended_us = timing_now_us();

        renderer_begin_frame(0.05f, 0.05f, 0.1f);
        renderer_draw_triangle();
        renderer_draw_textured_quad(texture);
        draw_ended_us = timing_now_us();

        renderer_end_frame();
        present_ended_us = timing_now_us();

        frame_us = frame_timer_tick(&frame_timer);
        frame_number = frame_number + 1;
        if (frame_number > 1) {
            frame_stats_add_frame(&stats, frame_us, update_ended_us - frame_started_us, draw_ended_us - update_ended_us, present_ended_us - draw_ended_us);
        }
        if (frame_stats_window_is_full(&stats)) {
            frame_stats_format(&stats, stats_text, sizeof(stats_text));
            run_log_write("%s", stats_text);
            log_device_stats();
            frame_stats_reset(&stats);
        }
        if (frame_number == FIRST_CAPTURE_FRAME) {
            save_capture("first_frames");
        }
        if (frame_number % COMMAND_POLL_FRAMES == 0) {
            quit_requested = run_pending_commands();
        }
    }

    renderer_texture_destroy(texture);
    renderer_shutdown();
    run_log_write("exit clean frames=%u gl_error=%d", frame_number, (int)glGetError());
    sceKernelExitProcess(0);
    return 0;
}
