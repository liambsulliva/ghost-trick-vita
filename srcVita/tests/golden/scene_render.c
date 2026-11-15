#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "host/host_platform.h"
#include "host/host_renderer.h"
#include "game_settings.h"
#include "input.h"
#include "renderer.h"
#include "run_log.h"
#include "script_player.h"
#include "texture_cache.h"
#include "viewer_app.h"

#define FRAME_US 16667ULL
#define FAST_TICKS_PER_FRAME 240
#define FRAMES_MOST_TO_REACH_A_POINT 4000000
#define LOAD_ROUNDS 6
#define UPLOADS_PER_ROUND 64
#define PATH_MOST 1024
#define POINT_MOST 64

static const struct renderer_color black = { 0.0f, 0.0f, 0.0f, 1.0f };
static struct input_state no_input;

static void run_one_frame(void)
{
    host_clock_advance_us(FRAME_US);
    viewer_app_update(&no_input, FRAME_US);
}

static void draw_until_the_art_is_in(void)
{
    int round;

    for (round = 0; round < LOAD_ROUNDS; round++) {
        renderer_begin_frame(black);
        viewer_app_draw();
        renderer_end_frame();
        texture_cache_upload_finished_reads(UPLOADS_PER_ROUND);
    }
}

static int run_fast_to(const char *kind, const char *name)
{
    int frame;

    script_player_set_fast_forward(FAST_TICKS_PER_FRAME);
    script_player_run_until(kind, name);
    for (frame = 0; frame < FRAMES_MOST_TO_REACH_A_POINT; frame++) {
        run_one_frame();
        if (script_player_is_paused()) {
            script_player_set_paused(0);
            return 1;
        }
        if (!script_player_is_active()) {
            return 0;
        }
    }
    return 0;
}

static void save_frame(const char *folder, const char *label, int number)
{
    char path[PATH_MOST];

    draw_until_the_art_is_in();
    snprintf(path, sizeof(path), "%s/%s_%03d_tick%u.ppm", folder, label, number, script_player_tick());
    if (!host_renderer_save_screen(path)) {
        fprintf(stderr, "cannot write %s\n", path);
        exit(1);
    }
    printf("%s\n", path);
}

int main(int argument_count, char **arguments)
{
    char point[POINT_MOST];
    char *name;
    const char *assets;
    const char *output;
    int frames;
    int every;
    int index;
    int frame;

    if (argument_count < 4) {
        fprintf(stderr, "usage: scene_render <set folder> <output folder> <tick:N | step:N | script:NAME | act:NAME> [frames] [frames between]\n");
        return 2;
    }
    assets = arguments[1];
    output = arguments[2];
    snprintf(point, sizeof(point), "%s", arguments[3]);
    name = strchr(point, ':');
    if (name == NULL) {
        fprintf(stderr, "a point is kind:name\n");
        return 2;
    }
    *name = '\0';
    name = name + 1;
    frames = argument_count > 4 ? atoi(arguments[4]) : 1;
    every = argument_count > 5 ? atoi(arguments[5]) : 1;
    host_files_keep_device_files_in(output);
    run_log_open("ux0:data/ghosttrick/run.log");
    renderer_init();
    game_settings_defaults(game_settings_now());
    viewer_app_start(assets);
    if (!viewer_app_has_assets()) {
        fprintf(stderr, "no assets in %s\n", assets);
        return 1;
    }
    viewer_app_play_script("route");
    script_player_set_auto_play(1);
    if (!run_fast_to(point, name)) {
        fprintf(stderr, "the game ended before %s:%s\n", point, name);
        return 1;
    }
    for (index = 0; index < frames; index++) {
        save_frame(output, arguments[3][0] == 't' ? "tick" : point, index);
        for (frame = 0; frame < every; frame++) {
            run_one_frame();
        }
    }
    run_log_flush();
    return 0;
}
