#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <psp2/kernel/processmgr.h>
#include "clock.h"
#include "command_file.h"
#include "device_stats.h"
#include "build_info.h"
#include "file.h"
#include "file_path.h"
#include "fps_overlay.h"
#include "frame_stats.h"
#include "input.h"
#include "keep_awake.h"
#include "memory.h"
#include "music_engine.h"
#include "renderer.h"
#include "audio_thread.h"
#include "run_log.h"
#include "screen_burst.h"
#include "screen_capture.h"
#include "screen_watch.h"
#include "script_player.h"
#include "script_view.h"
#include "system_watch.h"
#include "texture_cache.h"
#include "timing.h"
#include "viewer_app.h"

#define LONG_FRAME_US 50000u
#define BURST_PAUSE_MOST_US 120000000ULL
#define ART_HOLD_SLEEP_US 4000

#define COMMAND_POLL_FRAMES 6
#define KEEP_AWAKE_FRAMES 30
#define CAPTURE_NAME_MAX 64
#define FIRST_TIMED_FRAME 2
#define LOG_FOR_TOOLS_CAPTURE_DONE "capture name=%s result=%d took_us=%llu"
#define LOG_FOR_TOOLS_MUSIC_STARVE_COMMAND "command music_starve ms=%d"

#define LOG_PATH FILE_DATA_FOLDER "/run.log"
#define COMMANDS_PATH FILE_DATA_FOLDER "/commands.txt"
#define CAPTURE_FOLDER FILE_DATA_FOLDER "/captures"
#define ASSETS_FOLDER FILE_DATA_FOLDER "/assets"

static const struct renderer_color clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };

static void use_the_memory_this_device_has(void)
{
    struct device_stats stats;

    device_stats_read(&stats);
    texture_cache_set_budget(texture_cache_budget_for_free_memory(stats.video_memory_free + stats.main_memory_free));
}

static void log_memory_and_textures(void)
{
    struct device_stats stats;
    struct texture_cache_totals totals;

    device_stats_read(&stats);
    texture_cache_read_totals(&totals);
    run_log_write("memory video_free=%u video_total=%u main_free=%u main_total=%u heap_used=%u heap_size=%u gpu_textures=%u", stats.video_memory_free, stats.video_memory_total, stats.main_memory_free, stats.main_memory_total, stats.heap_used, stats.heap_size, renderer_live_texture_count());
    run_log_write("textures bytes=%u loads=%u evictions=%u missing=%u retries=%u read_ahead_dropped=%u longest_wait_us=%llu longest_upload_us=%llu", totals.bytes_in_use, totals.loads, totals.evictions, totals.missing, totals.retries, totals.read_ahead_dropped, totals.longest_wait_us, totals.longest_upload_us);
}

#ifdef DEBUG_BUILD
static void save_capture(const char *name)
{
    char path[FILE_PATH_MAX];
    unsigned long long started_us;
    int result;

    if (!file_path_join(path, CAPTURE_FOLDER, name, ".bmp")) {
        run_log_write("capture path_too_long name=%s", name);
        return;
    }
    started_us = clock_now_us();
    result = screen_capture_save_bmp(path);
    run_log_write(LOG_FOR_TOOLS_CAPTURE_DONE, name, result, clock_now_us() - started_us);
}

static unsigned int injected_buttons;
static unsigned long long burst_pause_began_us;
static int injected_touch_frames;
static struct command injected_drag;
static int injected_drag_frame;
static float injected_touch_x;
static float injected_touch_y;
static int fps_overlay_is_on;

static void save_manual_capture(void)
{
    static unsigned int manual_capture_count;
    char capture_name[CAPTURE_NAME_MAX];

    manual_capture_count = manual_capture_count + 1;
    snprintf(capture_name, sizeof(capture_name), "manual_%u", manual_capture_count);
    save_capture(capture_name);
}

static int run_one_command(const struct command *command)
{
    int succeeded;

    if (command->kind == COMMAND_CAPTURE) {
        save_capture(command->name);
    } else if (command->kind == COMMAND_SHOW_BACKGROUND) {
        succeeded = viewer_app_show_background(command->name);
        run_log_write("command show_background name=%s ok=%d", command->name, succeeded);
    } else if (command->kind == COMMAND_SHOW_ANIMATION) {
        succeeded = viewer_app_show_animation(command->name);
        run_log_write("command show_animation name=%s ok=%d", command->name, succeeded);
    } else if (command->kind == COMMAND_PLAY_SCRIPT) {
        succeeded = viewer_app_play_script(command->name);
        run_log_write("command play_script name=%s ok=%d", command->name, succeeded);
    } else if (command->kind == COMMAND_HUD_ON) {
        viewer_app_set_hud_visible(1);
    } else if (command->kind == COMMAND_HUD_OFF) {
        viewer_app_set_hud_visible(0);
    } else if (command->kind == COMMAND_MUSIC_STARVE) {
        music_request_starve(command->number);
        run_log_write(LOG_FOR_TOOLS_MUSIC_STARVE_COMMAND, command->number);
    } else if (command->kind == COMMAND_FPS) {
        fps_overlay_is_on = command->number;
        fps_overlay_reset();
    } else if (command->kind == COMMAND_AUTO_PLAY) {
        script_player_set_auto_play(command->number);
    } else if (command->kind == COMMAND_FAST_FORWARD) {
        script_player_set_fast_forward(command->number);
    } else if (command->kind == COMMAND_FAST_FORWARD_UNTIL) {
        script_player_run_until(command->detail, command->name);
    } else if (command->kind == COMMAND_RESUME) {
        burst_pause_began_us = 0;
        script_player_set_paused(0);
    } else if (command->kind == COMMAND_PRESS) {
        injected_buttons = (unsigned int)command->number;
    } else if (command->kind == COMMAND_TOUCH) {
        injected_touch_frames = command->number;
        injected_touch_x = (float)command->touch_x;
        injected_touch_y = (float)command->touch_y;
    } else if (command->kind == COMMAND_DRAG_SOUL) {
        float soul_x;
        float soul_y;

        injected_drag = *command;
        injected_drag_frame = 0;
        if (script_view_soul_screen_position(&soul_x, &soul_y)) {
            injected_drag.touch_x = (int)soul_x;
            injected_drag.touch_y = (int)soul_y;
            injected_drag.touch_end_x = injected_drag.touch_x + command->touch_end_x;
            injected_drag.touch_end_y = injected_drag.touch_y + command->touch_end_y;
        } else {
            injected_drag.number = 0;
        }
        run_log_write("command drag_soul from=%d,%d by=%d,%d frames=%d", injected_drag.touch_x, injected_drag.touch_y, command->touch_end_x, command->touch_end_y, injected_drag.number);
    } else if (command->kind == COMMAND_DRAG) {
        injected_drag = *command;
        injected_drag_frame = 0;
        run_log_write("command drag from=%d,%d to=%d,%d frames=%d delay=%d", command->touch_x, command->touch_y, command->touch_end_x, command->touch_end_y, command->number, command->delay_frames);
    } else if (command->kind == COMMAND_BURST) {
        screen_burst_run_for_seconds(command->number, command->ms_between_pictures);
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
#endif

static void publish_frame_stats(struct frame_stats *stats)
{
    char log_text[FRAME_STATS_TEXT_MAX];
    char status_text[VIEWER_STATUS_TEXT_MAX];
    unsigned long long average_frame_us;

    frame_stats_format(stats, log_text, sizeof(log_text));
    run_log_write("%s", log_text);
    log_memory_and_textures();
    screen_watch_publish();
    system_watch_poll();

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
    unsigned long long frame_started_us;
    unsigned long long update_ended_us;
    unsigned long long draw_ended_us;
    unsigned long long present_ended_us;
    unsigned long long last_frame_us;
    int quit_requested;

    input.pressed = 0;
    input.held = 0;
    frame_number = 0;
    last_frame_us = 0;
    quit_requested = 0;
    frame_stats_reset(&stats);
    frame_timer_start(&frame_timer);
    screen_watch_start();

    while (!quit_requested) {
        if (frame_number % KEEP_AWAKE_FRAMES == 0) {
            keep_awake_tick();
        }
        frame_started_us = clock_now_us();
        input_poll(&input);
#ifdef DEBUG_BUILD
        input.pressed = input.pressed | injected_buttons;
        input.held = input.held | injected_buttons;
        injected_buttons = 0;
        if (injected_touch_frames > 0) {
            injected_touch_frames = injected_touch_frames - 1;
            input.touch_is_down = 1;
            input.touch_x = injected_touch_x;
            input.touch_y = injected_touch_y;
        }
        if (injected_drag.delay_frames > 0) {
            injected_drag.delay_frames = injected_drag.delay_frames - 1;
        } else if (injected_drag_frame < injected_drag.number) {
            float share;

            share = injected_drag.number > 1 ? (float)injected_drag_frame / (float)(injected_drag.number - 1) : 1.0f;
            input.touch_is_down = 1;
            input.touch_x = (float)injected_drag.touch_x + (float)(injected_drag.touch_end_x - injected_drag.touch_x) * share;
            input.touch_y = (float)injected_drag.touch_y + (float)(injected_drag.touch_end_y - injected_drag.touch_y) * share;
            injected_drag_frame = injected_drag_frame + 1;
        }
#endif
        if ((input.pressed & INPUT_START) && !script_player_is_active()) {
            run_log_write("input start pressed");
            break;
        }
#ifdef DEBUG_BUILD
        if ((input.pressed & INPUT_SQUARE) && !script_player_is_active()) {
            save_manual_capture();
        }
#endif
        viewer_app_update(&input, last_frame_us);
        update_ended_us = clock_now_us();

        if (script_player_is_holding_for_art()) {
            audio_thread_sleep_us(ART_HOLD_SLEEP_US);
            draw_ended_us = clock_now_us();
            present_ended_us = draw_ended_us;
        } else {
            renderer_begin_frame(clear_color);
            viewer_app_draw();
#ifdef DEBUG_BUILD
            if (fps_overlay_is_on) {
                fps_overlay_draw();
            }
#endif
            draw_ended_us = clock_now_us();

            renderer_end_frame();
            present_ended_us = clock_now_us();
        }

        last_frame_us = frame_timer_tick(&frame_timer);
        screen_watch_note_frame();
#ifdef DEBUG_BUILD
        fps_overlay_note_frame(last_frame_us);
        if (burst_pause_began_us != 0 && clock_now_us() - burst_pause_began_us > BURST_PAUSE_MOST_US) {
            burst_pause_began_us = 0;
            script_player_set_paused(0);
            run_log_write("burst pause_timed_out");
        }
        screen_burst_note_game_time(script_player_tick(), script_player_acted_route_step(), script_player_route_action_began_at());
        if (screen_burst_note_frame_and_say_if_it_ended()) {
            burst_pause_began_us = clock_now_us();
            script_player_set_paused(1);
        }
#endif
        frame_number = frame_number + 1;
        if (frame_number >= FIRST_TIMED_FRAME) {
            frame_stats_add_frame(&stats, last_frame_us, update_ended_us - frame_started_us, draw_ended_us - update_ended_us, present_ended_us - draw_ended_us);
            if (last_frame_us >= LONG_FRAME_US) {
                run_log_write("frame long us=%llu update_us=%llu draw_us=%llu present_us=%llu", (unsigned long long)last_frame_us, update_ended_us - frame_started_us, draw_ended_us - update_ended_us, present_ended_us - draw_ended_us);
            }
        }
        if (frame_stats_window_is_full(&stats)) {
            publish_frame_stats(&stats);
        }
#ifdef DEBUG_BUILD
        if (frame_number % COMMAND_POLL_FRAMES == 0) {
            quit_requested = run_pending_commands();
        }
#endif
    }
    run_log_write("loop ended frames=%u", frame_number);
}

int main(void)
{
    int full_resolution;

    keep_awake_tick();
    file_make_folder(FILE_DATA_FOLDER);
    file_make_folder(CAPTURE_FOLDER);
    run_log_open(LOG_PATH);
    run_log_write("build id=%s", build_info_id());
    system_watch_start();

    full_resolution = renderer_init();
    run_log_write("renderer ready full_resolution=%d", full_resolution);
#ifdef DEBUG_BUILD
    screen_burst_reserve_memory();
#endif
    viewer_app_start(ASSETS_FOLDER);
    use_the_memory_this_device_has();
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
