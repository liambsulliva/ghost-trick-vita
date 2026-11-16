#include <stdio.h>
#include <string.h>
#include "asset_sets.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_task.h"
#include "fakes/fake_texture_files.h"
#include "fakes/fake_work_queue.h"
#include "game_settings.h"
#include "input.h"
#include "run_log.h"
#include "test_support.h"
#include "texture_cache.h"
#include "viewer_app.h"

#define FRAME_US 16667ULL
#define FRAMES_TO_PLAY 120

static void add_text_file(const char *path, const char *text)
{
    fake_files_add(path, (const unsigned char *)text, (unsigned int)strlen(text));
}

static void add_set(const char *folder, const char *art_line)
{
    char path[FILE_PATH_MAX];
    char text[256];

    snprintf(path, sizeof(path), "%s/textures/room.gtex", folder);
    fake_files_add_rgba_texture(path, 64, 48);
    snprintf(path, sizeof(path), "%s/textures/walk_page.gtex", folder);
    fake_files_add_rgba_texture(path, 16, 16);
    snprintf(path, sizeof(path), "%s/animations/walk.anim", folder);
    add_text_file(path, "animation walk\nloops 1\nframe walk_page 6 -8 -16 16 16 0 0 1 1\n");
    snprintf(path, sizeof(path), "%s/scenes/opening.scene", folder);
    add_text_file(path, "scene opening\nroom 1\nplate room 0 0 64 48\nobject hero walk 20 30 1 0\nwait 60\n");
    snprintf(path, sizeof(path), "%s/manifest.txt", folder);
    snprintf(text, sizeof(text), "%sscene opening\nanimation walk\nbackground room\n", art_line);
    add_text_file(path, text);
}

static void set_up_platform(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_task_reset();
    fake_work_queue_reset();
    fake_audio_reset();
    fake_memory_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    game_settings_defaults(game_settings_now());
}

static void test_only_a_ds_set_runs_the_ds_set_pinned(void)
{
    set_up_platform();
    add_set("ds", "art ds\n");
    CHECK_TEXT(asset_sets_choose("remastered", "ds"), "ds");
    CHECK_INT(asset_sets_running_filter(), GAME_FILTER_DS);
    CHECK_INT(game_settings_filter_is_pinned(), 1);
    CHECK_INT(asset_sets_switch_is_due(), 0);
}

static void test_only_a_remastered_set_runs_the_remastered_set_pinned(void)
{
    set_up_platform();
    add_set("remastered", "art remastered\n");
    game_settings_now()->visual_filter = GAME_FILTER_DS;
    CHECK_TEXT(asset_sets_choose("remastered", "ds"), "remastered");
    CHECK_INT(asset_sets_running_filter(), GAME_FILTER_REMASTERED);
    CHECK_INT(game_settings_filter_is_pinned(), 1);
}

static void test_with_both_sets_the_saved_filter_picks_and_a_change_waits_for_the_switch(void)
{
    set_up_platform();
    add_set("remastered", "art remastered\n");
    add_set("ds", "art ds\n");
    game_settings_now()->visual_filter = GAME_FILTER_DS;
    CHECK_TEXT(asset_sets_choose("remastered", "ds"), "ds");
    CHECK_INT(game_settings_filter_is_pinned(), 0);
    CHECK_INT(asset_sets_switch_is_due(), 0);
    game_settings_now()->visual_filter = GAME_FILTER_REMASTERED;
    CHECK_INT(asset_sets_switch_is_due(), 1);
}

static void test_a_filter_picked_on_the_title_survives_the_next_launch(void)
{
    set_up_platform();
    add_set(ASSET_SETS_REMASTERED_FOLDER, "art remastered\n");
    add_set(ASSET_SETS_DS_FOLDER, "art ds\n");
    CHECK_TEXT(asset_sets_choose(ASSET_SETS_REMASTERED_FOLDER, ASSET_SETS_DS_FOLDER), ASSET_SETS_REMASTERED_FOLDER);
    game_settings_now()->visual_filter = GAME_FILTER_DS;
    CHECK_INT(asset_sets_switch_is_due(), 1);
    viewer_app_keep_settings_for_switch();
    game_settings_defaults(game_settings_now());
    game_settings_load(GAME_SETTINGS_PATH);
    CHECK_TEXT(asset_sets_choose(ASSET_SETS_REMASTERED_FOLDER, ASSET_SETS_DS_FOLDER), ASSET_SETS_DS_FOLDER);
}

static void test_a_switch_in_the_same_process_runs_the_other_set_alone(void)
{
    struct input_state no_input;
    const char *folder;
    int frame;

    set_up_platform();
    add_set("remastered", "art remastered\n");
    add_set("ds", "art ds\n");
    viewer_app_start(asset_sets_choose("remastered", "ds"));
    CHECK(!texture_cache_sharp_art_is_on());
    memset(&no_input, 0, sizeof(no_input));
    for (frame = 0; frame < FRAMES_TO_PLAY; frame++) {
        viewer_app_update(&no_input, FRAME_US);
        viewer_app_draw();
    }
    game_settings_now()->visual_filter = GAME_FILTER_DS;
    CHECK_INT(asset_sets_switch_is_due(), 1);
    viewer_app_keep_settings_for_switch();
    viewer_app_stop();
    folder = asset_sets_choose("remastered", "ds");
    CHECK_TEXT(folder, "ds");
    fake_files_watch_reads_under("remastered/");
    viewer_app_start(folder);
    CHECK_INT(asset_sets_switch_is_due(), 0);
    CHECK(texture_cache_sharp_art_is_on());
    CHECK(viewer_app_play_scene("opening"));
    for (frame = 0; frame < FRAMES_TO_PLAY; frame++) {
        viewer_app_update(&no_input, FRAME_US);
        viewer_app_draw();
    }
    CHECK_INT(fake_files_watched_reads(), 0);
    viewer_app_stop();
}

static void test_the_ds_set_reads_nothing_from_the_remastered_set(void)
{
    struct input_state no_input;
    const char *folder;
    int frame;

    set_up_platform();
    add_set("remastered", "art remastered\n");
    add_set("ds", "art ds\n");
    game_settings_now()->visual_filter = GAME_FILTER_DS;
    folder = asset_sets_choose("remastered", "ds");
    fake_files_watch_reads_under("remastered/");
    viewer_app_start(folder);
    CHECK(texture_cache_sharp_art_is_on());
    CHECK(viewer_app_play_scene("opening"));
    memset(&no_input, 0, sizeof(no_input));
    for (frame = 0; frame < FRAMES_TO_PLAY; frame++) {
        viewer_app_update(&no_input, FRAME_US);
        viewer_app_draw();
    }
    CHECK_INT(fake_files_watched_reads(), 0);
    viewer_app_stop();
}

static void test_the_remastered_set_reads_nothing_from_the_ds_set(void)
{
    struct input_state no_input;
    const char *folder;
    int frame;

    set_up_platform();
    add_set("remastered", "art remastered\n");
    add_set("ds", "art ds\n");
    folder = asset_sets_choose("remastered", "ds");
    fake_files_watch_reads_under("ds/");
    viewer_app_start(folder);
    CHECK(!texture_cache_sharp_art_is_on());
    CHECK(viewer_app_play_scene("opening"));
    memset(&no_input, 0, sizeof(no_input));
    for (frame = 0; frame < FRAMES_TO_PLAY; frame++) {
        viewer_app_update(&no_input, FRAME_US);
        viewer_app_draw();
    }
    CHECK_INT(fake_files_watched_reads(), 0);
    viewer_app_stop();
}

int main(void)
{
    test_only_a_ds_set_runs_the_ds_set_pinned();
    test_only_a_remastered_set_runs_the_remastered_set_pinned();
    test_with_both_sets_the_saved_filter_picks_and_a_change_waits_for_the_switch();
    test_a_filter_picked_on_the_title_survives_the_next_launch();
    test_a_switch_in_the_same_process_runs_the_other_set_alone();
    test_the_ds_set_reads_nothing_from_the_remastered_set();
    test_the_remastered_set_reads_nothing_from_the_ds_set();
    return test_finish("integration/asset_sets");
}
