#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include "command_file.h"
#include "device_stats.h"
#include "build_info.h"
#include "file.h"
#include "file_path.h"
#include "frame_stats.h"
#include "input.h"
#include "memory.h"
#include "renderer.h"
#include "run_log.h"
#include "screen_capture.h"
#include "texture_cache.h"
#include "timing.h"
#include "viewer_app.h"

#define COMMAND_POLL_FRAMES 30
#define FIRST_CAPTURE_FRAME 120
#define CAPTURE_NAME_MAX 64

#define LOG_PATH FILE_DATA_FOLDER "/run.log"
#define COMMANDS_PATH FILE_DATA_FOLDER "/commands.txt"
#define CAPTURE_FOLDER FILE_DATA_FOLDER "/captures"
#define ASSETS_FOLDER FILE_DATA_FOLDER "/assets"

static const struct renderer_color clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

static void log_memory_and_textures(void)
{
    struct device_stats stats;
    struct texture_cache_totals totals;

    device_stats_read(&stats);
    texture_cache_read_totals(&totals);
    run_log_write("memory video_free=%u video_total=%u main_free=%u main_total=%u", stats.video_memory_free, stats.video_memory_total, stats.main_memory_free, stats.main_memory_total);
    run_log_write("textures bytes=%u loads=%u evictions=%u missing=%u longest_wait_us=%llu longest_upload_us=%llu", totals.bytes_in_use, totals.loads, totals.evictions, totals.missing, totals.longest_wait_us, totals.longest_upload_us);
}

static void save_capture(const char *name)
{
    char path[FILE_PATH_MAX];
    unsigned long long started_us;
    int result;

    if (!file_path_join(path, CAPTURE_FOLDER, name, ".bmp")) {
        run_log_write("capture path_too_long name=%s", name);
        return;
    }
    started_us = timing_now_us();
    result = screen_capture_save_bmp(path);
    run_log_write("capture name=%s result=%d took_us=%llu", name, result, timing_now_us() - started_us);
}

static int run_one_command(const struct command *command)
{
    if (command->kind == COMMAND_CAPTURE) {
        save_capture(command->name);
    } else if (command->kind == COMMAND_SHOW_BACKGROUND) {
        run_log_write("command show_background name=%s ok=%d", command->name, viewer_app_show_background(command->name));
    } else if (command->kind == COMMAND_SHOW_ANIMATION) {
        run_log_write("command show_animation name=%s ok=%d", command->name, viewer_app_show_animation(command->name));
    } else if (command->kind == COMMAND_PLAY_SCENE) {
        run_log_write("command play_scene name=%s ok=%d", command->name, viewer_app_play_scene(command->name));
    } else if (command->kind == COMMAND_HUD_ON) {
        viewer_app_set_hud_visible(1);
    } else if (command->kind == COMMAND_HUD_OFF) {
        viewer_app_set_hud_visible(0);
    } else if (command->kind == COMMAND_QUIT) {
        run_log_write("command quit");
        return 1;
    } else if (command->kind == COMMAND_UNKNOWN) {
        run_log_write("command unknown");
    }
    return 0;
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
        if (run_one_command(&command)) {
            quit_requested = 1;
        }
    }
    mem_free(text);
    return quit_requested;
}

static void publish_frame_stats(struct frame_stats *stats)
{
    char log_text[FRAME_STATS_TEXT_MAX];
    char status_text[VIEWER_STATUS_TEXT_MAX];
    unsigned long long average_frame_us;

    frame_stats_format(stats, log_text, sizeof(log_text));
    run_log_write("%s", log_text);
    log_memory_and_textures();

    average_frame_us = stats->total_frame_us / stats->frame_count;
    snprintf(status_text, sizeof(status_text), "frame %llu.%llums slow %u", average_frame_us / 1000, average_frame_us / 100 % 10, stats->slow_frame_count);
    viewer_app_set_status_text(status_text);
    frame_stats_reset(stats);
}

static void run_main_loop(void)
{
    struct input_state input;
    struct frame_stats stats;
    struct frame_timer frame_timer;
    unsigned int frame_number;
    unsigned int manual_capture_count;
    unsigned long long frame_started_us;
    unsigned long long update_ended_us;
    unsigned long long draw_ended_us;
    unsigned long long present_ended_us;
    unsigned long long frame_us;
    char capture_name[CAPTURE_NAME_MAX];
    int quit_requested;

    input.held = 0;
    input.pressed = 0;
    frame_number = 0;
    manual_capture_count = 0;
    frame_us = 0;
    quit_requested = 0;
    frame_stats_reset(&stats);
    frame_timer_start(&frame_timer);

    while (!quit_requested) {
        frame_started_us = timing_now_us();
        input_poll(&input);
        if (input.pressed & INPUT_START) {
            run_log_write("input start pressed");
            break;
        }
        if (input.pressed & INPUT_SQUARE) {
            manual_capture_count = manual_capture_count + 1;
            snprintf(capture_name, sizeof(capture_name), "manual_%u", manual_capture_count);
            save_capture(capture_name);
        }
        viewer_app_update(&input, frame_us);
        update_ended_us = timing_now_us();

        renderer_begin_frame(clear_color);
        viewer_app_draw();
        draw_ended_us = timing_now_us();

        renderer_end_frame();
        present_ended_us = timing_now_us();

        frame_us = frame_timer_tick(&frame_timer);
        frame_number = frame_number + 1;
        if (frame_number > 1) {
            frame_stats_add_frame(&stats, frame_us, update_ended_us - frame_started_us, draw_ended_us - update_ended_us, present_ended_us - draw_ended_us);
        }
        if (frame_stats_window_is_full(&stats)) {
            publish_frame_stats(&stats);
        }
        if (frame_number == FIRST_CAPTURE_FRAME) {
            save_capture("first_frames");
        }
        if (frame_number % COMMAND_POLL_FRAMES == 0) {
            quit_requested = run_pending_commands();
        }
    }
    run_log_write("loop ended frames=%u", frame_number);
}

int main(void)
{
    file_make_folder(FILE_DATA_FOLDER);
    file_make_folder(CAPTURE_FOLDER);
    run_log_open(LOG_PATH);
    run_log_write("build id=%s", build_info_id());

    run_log_write("renderer ready full_resolution=%d", renderer_init());
    viewer_app_start(ASSETS_FOLDER);
    log_memory_and_textures();
    run_log_write("viewer ready has_assets=%d", viewer_app_has_assets());

    run_main_loop();

    viewer_app_stop();
    renderer_shutdown();
    run_log_write("exit clean");
    run_log_flush();
    sceKernelExitProcess(0);
    return 0;
}
