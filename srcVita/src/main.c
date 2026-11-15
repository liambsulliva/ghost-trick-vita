#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <psp2/appmgr.h>
#include <psp2/kernel/processmgr.h>
#include "asset_sets.h"
#include "clock.h"
#include "device_stats.h"
#include "build_info.h"
#include "file.h"
#include "game_settings.h"
#include "frame_stats.h"
#include "input.h"
#include "keep_awake.h"
#include "memory.h"
#include "music_engine.h"
#include "renderer.h"
#include "audio_thread.h"
#include "run_log.h"
#include "script_player.h"
#include "system_watch.h"
#include "texture_cache.h"
#include "timing.h"
#include "viewer_app.h"
#ifdef DEBUG_BUILD
#include <stdlib.h>
#include "command_file.h"
#include "command_reader.h"
#include "file_path.h"
#include "fps_overlay.h"
#include "screen_burst.h"
#include "screen_capture.h"
#include "screen_watch.h"
#include "asset_language.h"
#include "game_save.h"
#include "pause_menu.h"
#include "pause_options.h"
#include "pause_people.h"
#include "script_view.h"
#include "smoke_run.h"
#include "title_menu.h"
#endif

#define LONG_FRAME_US 50000u
#define ART_HOLD_SLEEP_US 4000
#define KEEP_AWAKE_FRAMES 30
#define FIRST_TIMED_FRAME 2
#define LOG_PATH FILE_DATA_FOLDER "/run.log"
#define RELAUNCH_PATH "app0:eboot.bin"

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
#define BURST_PAUSE_MOST_US 120000000ULL
#define COMMAND_POLL_FRAMES 6
#define CAPTURE_NAME_MAX 64
#define LOG_FOR_TOOLS_CAPTURE_DONE "capture name=%s result=%d took_us=%llu"
#define LOG_FOR_TOOLS_MUSIC_STARVE_COMMAND "command music_starve ms=%d"
#define COMMANDS_PATH FILE_DATA_FOLDER "/commands.txt"
#define CAPTURE_FOLDER FILE_DATA_FOLDER "/captures"

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
    } else if (command->kind == COMMAND_SMOKE) {
        run_log_write("command smoke_inside_a_flow name=%s", command->name);
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

    if (!command_reader_take(&text, &text_size)) {
        return 0;
    }

    quit_requested = 0;
    offset = 0;
    while (offset < text_size) {
        offset = command_parse_next_line((const char *)text, text_size, offset, &command);
        if (command.kind == COMMAND_SMOKE) {
            smoke_run_begin(command.name, (const char *)&text[offset], text_size - offset);
            break;
        }
        if (run_one_command(&command)) {
            quit_requested = 1;
        }
    }
    mem_free(text);
    return quit_requested;
}

static int setting_is(const char *argument)
{
    static const char *const names[] = { "brightness", "master", "music", "effects", "music_type", "language", "unlock_all", "fps_cap", "vsync", "high_contrast", "visual_filter", "text_speed", "hint_delay" };
    const struct game_settings *settings = game_settings_now();
    const int values[] = { settings->brightness, settings->master, settings->music, settings->effects, settings->music_type, settings->language, settings->unlock_all, settings->fps_cap, settings->vsync, settings->high_contrast, settings->visual_filter, settings->text_speed, settings->hint_delay };
    char name[32];
    int wanted;
    unsigned int index;

    if (sscanf(argument, "%31s %d", name, &wanted) != 2) {
        return SMOKE_PROBE_UNKNOWN;
    }
    for (index = 0; index < sizeof(names) / sizeof(names[0]); index++) {
        if (strcmp(names[index], name) == 0) {
            return values[index] == wanted;
        }
    }
    return SMOKE_PROBE_UNKNOWN;
}

static int save_exists(const char *argument)
{
    static struct game_save save;

    if (strcmp(argument, "auto") == 0) {
        return game_save_load(&save, GAME_SAVE_AUTO);
    }
    if (strcmp(argument, "manual") == 0) {
        return game_save_load(&save, GAME_SAVE_MANUAL);
    }
    return SMOKE_PROBE_UNKNOWN;
}

static int smoke_condition_holds(const char *word, const char *argument)
{
    struct texture_cache_totals totals;

    if (strcmp(word, "title") == 0) {
        return title_menu_is_open() && title_menu_phase() == TITLE_PHASE_SHOWING;
    }
    if (strcmp(word, "title_submenu") == 0) {
        return title_menu_is_open() && title_menu_phase() == TITLE_PHASE_SUBMENU;
    }
    if (strcmp(word, "title_cursor") == 0) {
        return title_menu_cursor() == atoi(argument);
    }
    if (strcmp(word, "title_dialog") == 0) {
        return title_menu_dialog_is_open();
    }
    if (strcmp(word, "pause") == 0) {
        return pause_menu_is_open() && pause_menu_phase() == PAUSE_PHASE_SHOWING;
    }
    if (strcmp(word, "pause_submenu") == 0) {
        return pause_menu_is_open() && pause_menu_phase() == PAUSE_PHASE_SUBMENU;
    }
    if (strcmp(word, "pause_cursor") == 0) {
        return pause_menu_cursor() == atoi(argument);
    }
    if (strcmp(word, "pause_open") == 0) {
        return pause_menu_is_open();
    }
    if (strcmp(word, "game") == 0) {
        return script_player_is_active() && script_player_status() == SCENE_PLAYER_RUNNING && !pause_menu_is_open() && !title_menu_is_open();
    }
    if (strcmp(word, "loading") == 0) {
        return script_player_is_active() && script_player_status() == SCENE_PLAYER_PRELOADING;
    }
    if (strcmp(word, "paused") == 0) {
        return script_player_is_paused();
    }
    if (strcmp(word, "script") == 0) {
        return strcmp(script_player_script_name(), argument) == 0;
    }
    if (strcmp(word, "tick") == 0) {
        return script_player_tick() >= (unsigned int)strtoul(argument, NULL, 10);
    }
    if (strcmp(word, "shows") == 0) {
        return script_player_shows(argument);
    }
    if (strcmp(word, "options_row") == 0) {
        return pause_options_selected_row() == atoi(argument);
    }
    if (strcmp(word, "record_tab") == 0) {
        return pause_people_tab() == atoi(argument);
    }
    if (strcmp(word, "route") == 0) {
        return script_player_route_wait_holds(argument);
    }
    if (strcmp(word, "set") == 0) {
        return strcmp(argument, asset_sets_running_filter() == GAME_FILTER_DS ? "ds" : "remastered") == 0;
    }
    if (strcmp(word, "language") == 0) {
        return strcmp(argument, asset_language_code(asset_language_now())) == 0;
    }
    if (strcmp(word, "setting") == 0) {
        return setting_is(argument);
    }
    if (strcmp(word, "save") == 0) {
        return save_exists(argument);
    }
    if (strcmp(word, "assets") == 0) {
        return viewer_app_has_assets();
    }
    if (strcmp(word, "textures_missing_most") == 0) {
        texture_cache_read_totals(&totals);
        return totals.missing <= (unsigned int)strtoul(argument, NULL, 10);
    }
    return SMOKE_PROBE_UNKNOWN;
}

static int run_smoke_lines(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    struct command command;
    int quit_requested;

    quit_requested = 0;
    smoke_run_note_frame();
    while (smoke_run_next_command(line, sizeof(line))) {
        command_parse_next_line(line, (unsigned int)strlen(line), 0, &command);
        if (run_one_command(&command)) {
            quit_requested = 1;
        }
        if (command.kind == COMMAND_PRESS || command.kind == COMMAND_TOUCH || command.kind == COMMAND_DRAG || command.kind == COMMAND_DRAG_SOUL) {
            break;
        }
    }
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
#ifdef DEBUG_BUILD
    screen_watch_publish();
#endif
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
#ifdef DEBUG_BUILD
    screen_watch_start();
    run_log_watch(smoke_run_note_log_line);
    smoke_run_use_probe(smoke_condition_holds);
    if (!command_reader_start(COMMANDS_PATH)) {
        run_log_write("command reader_not_started");
    }
#endif

    while (!quit_requested) {
        if (viewer_app_wants_relaunch()) {
            run_log_write("relaunch for the other art set");
            viewer_app_keep_settings_for_relaunch();
            break;
        }
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
#ifdef DEBUG_BUILD
        screen_watch_note_frame();
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
        if (frame_number % COMMAND_POLL_FRAMES == 0 && !smoke_run_is_active()) {
            quit_requested = run_pending_commands();
        }
        if (run_smoke_lines()) {
            quit_requested = 1;
        }
#endif
    }
    run_log_write("loop ended frames=%u", frame_number);
}

int main(void)
{
    int full_resolution;
    int relaunch_is_wanted;

    keep_awake_tick();
    file_make_folder(FILE_DATA_FOLDER);
#ifdef DEBUG_BUILD
    file_make_folder(CAPTURE_FOLDER);
#endif
    run_log_open(LOG_PATH);
    run_log_write("build id=%s", build_info_id());
    system_watch_start();

    full_resolution = renderer_init();
    run_log_write("renderer ready full_resolution=%d", full_resolution);
#ifdef DEBUG_BUILD
    screen_burst_reserve_memory();
#endif
    game_settings_load(GAME_SETTINGS_PATH);
    viewer_app_start(asset_sets_choose(ASSET_SETS_REMASTERED_FOLDER, ASSET_SETS_DS_FOLDER));
    use_the_memory_this_device_has();
    log_memory_and_textures();
    run_log_write("viewer ready has_assets=%d", viewer_app_has_assets());

    run_main_loop();

    relaunch_is_wanted = viewer_app_wants_relaunch();
    viewer_app_stop();
    renderer_shutdown();
    run_log_write("exit clean relaunch=%d", relaunch_is_wanted);
    run_log_flush();
    if (relaunch_is_wanted) {
        sceAppMgrLoadExec(RELAUNCH_PATH, NULL, NULL);
    }
    sceKernelExitProcess(0);
    return 0;
}
