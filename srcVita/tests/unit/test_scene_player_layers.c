#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "test_support.h"
#include "texture_cache.h"

#define ONE_TICK_US 16667ULL
#define UPLOADS_PER_UPDATE 16
#define TEXTURE_BUDGET_BYTES (1024u * 1024u)
#define PRELOAD_UPDATE_LIMIT 64

static void update(unsigned long long elapsed_us)
{
    fake_clock_advance_us(elapsed_us);
    texture_cache_upload_finished_reads(UPLOADS_PER_UPDATE);
    texture_cache_begin_frame();
    scene_player_update(elapsed_us);
}

static void start_scene(const char *scene_text)
{
    int count;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    fake_files_add_rgba_texture("assets/textures/line_a.gtex", 8, 8);
    fake_files_add_rgba_texture("assets/textures/art.gtex", 8, 8);
    fake_files_add("assets/scenes/opening.scene", (const unsigned char *)scene_text, (unsigned int)strlen(scene_text));
    texture_cache_init("assets/textures", TEXTURE_BUDGET_BYTES);
    animation_library_init("assets/animations");
    scene_player_init("assets/scenes");
    CHECK(scene_player_play("opening"));
    for (count = 0; count < PRELOAD_UPDATE_LIMIT && scene_player_status() == SCENE_PLAYER_PRELOADING; count++) {
        update(0);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    update(0);
}

static void finish_scene(void)
{
    scene_player_shutdown();
    animation_library_clear();
    texture_cache_clear();
}

static const struct scene_text_line *first_line(void)
{
    return &scene_stage_view()->layers.text_lines[0];
}

static const struct scene_overlay *first_overlay(void)
{
    return &scene_stage_view()->layers.overlays[0];
}

static void test_text_steps_reach_the_layer_with_every_field(void)
{
    start_scene("scene s\ntext_show 3 line_a 470 72 60\nwait 6000\n");
    CHECK_INT(scene_player_next_step(), 2);
    CHECK(first_line()->in_use);
    CHECK_INT(first_line()->id, 3);
    CHECK_TEXT(first_line()->texture_name, "line_a");
    CHECK(first_line()->center_x == 470.0f);
    CHECK(first_line()->top_y == 72.0f);
    CHECK(first_line()->alpha == 0.0f);
    update(ONE_TICK_US * 30);
    CHECK(first_line()->alpha > 0.45f && first_line()->alpha < 0.55f);
    finish_scene();
}

static void test_overlay_steps_reach_the_layer_with_every_field(void)
{
    start_scene("scene s\noverlay 2 art 10 20 30 40 60\nwait 6000\n");
    CHECK(first_overlay()->in_use);
    CHECK_INT(first_overlay()->id, 2);
    CHECK_TEXT(first_overlay()->texture_name, "art");
    CHECK(first_overlay()->x == 10.0f);
    CHECK(first_overlay()->y == 20.0f);
    CHECK(first_overlay()->width == 30.0f);
    CHECK(first_overlay()->height == 40.0f);
    update(ONE_TICK_US * 30);
    CHECK(first_overlay()->size_fraction > 0.45f && first_overlay()->size_fraction < 0.55f);
    finish_scene();
}

static void test_a_second_overlay_step_for_an_id_restarts_its_growth(void)
{
    start_scene("scene s\noverlay 2 art 10 20 30 40 60\nwait 30\noverlay 2 art 10 20 30 40 60\nwait 6000\n");
    update(ONE_TICK_US * 30);
    CHECK(first_overlay()->size_fraction < 0.05f);
    update(ONE_TICK_US * 15);
    CHECK(first_overlay()->size_fraction > 0.2f && first_overlay()->size_fraction < 0.3f);
    finish_scene();
}

static void test_hide_steps_name_the_line_and_overlay_they_remove(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\ntext_show 2 line_a 480 122 0\noverlay 1 art 0 0 8 8 0\noverlay 2 art 0 0 8 8 0\nwait 6\ntext_hide 1 0\noverlay_hide 2\nwait 6000\n");
    update(ONE_TICK_US * 7);
    CHECK(!scene_stage_view()->layers.text_lines[0].in_use);
    CHECK(scene_stage_view()->layers.text_lines[1].in_use);
    CHECK(scene_stage_view()->layers.overlays[0].in_use);
    CHECK(!scene_stage_view()->layers.overlays[1].in_use);
    finish_scene();
}

static void test_a_hide_step_passes_its_fade_length_on(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\nwait 6\ntext_hide 1 60\nwait 6000\n");
    update(ONE_TICK_US * 7);
    CHECK(first_line()->in_use);
    update(ONE_TICK_US * 30);
    CHECK(first_line()->alpha > 0.4f && first_line()->alpha < 0.6f);
    finish_scene();
}

static void test_hide_all_step_removes_every_line(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\ntext_show 2 line_a 480 122 0\nwait 6\ntext_hide_all\nwait 6000\n");
    update(ONE_TICK_US * 7);
    CHECK(!scene_stage_view()->layers.text_lines[0].in_use);
    CHECK(!scene_stage_view()->layers.text_lines[1].in_use);
    finish_scene();
}

static void test_stopping_clears_text_and_overlays(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\noverlay 1 art 0 0 8 8 0\nwait 6000\n");
    scene_player_stop();
    CHECK(!first_line()->in_use);
    CHECK(!first_overlay()->in_use);
    finish_scene();
}

static void test_playing_again_clears_text_and_overlays(void)
{
    start_scene("scene s\nwait 6\ntext_show 1 line_a 480 72 0\noverlay 1 art 0 0 8 8 0\nwait 6000\n");
    update(ONE_TICK_US * 7);
    CHECK(first_line()->in_use);
    CHECK(first_overlay()->in_use);
    CHECK(scene_player_play("opening"));
    CHECK(!first_line()->in_use);
    CHECK(!first_overlay()->in_use);
    finish_scene();
}

static void test_a_scene_that_fails_to_play_keeps_its_text(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\nwait 6000\n");
    CHECK(!scene_player_play("missing"));
    CHECK(first_line()->in_use);
    finish_scene();
}

static void test_shutting_down_clears_text(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\nwait 6000\n");
    finish_scene();
    CHECK(!first_line()->in_use);
}

static void test_skipping_a_hold_runs_the_hide_that_follows_it(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\nwait 6000\ntext_hide 1 0\nwait 6000\n");
    CHECK(first_line()->in_use);
    scene_player_skip_hold();
    update(0);
    CHECK(!first_line()->in_use);
    finish_scene();
}

static void test_fades_keep_running_after_the_scene_finishes(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 0\nwait 1\ntext_hide 1 60\n");
    update(ONE_TICK_US * 2);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK(first_line()->in_use);
    update(ONE_TICK_US * 30);
    CHECK(first_line()->in_use);
    update(ONE_TICK_US * 31);
    CHECK(!first_line()->in_use);
    finish_scene();
}

static void test_pausing_stops_the_timers(void)
{
    start_scene("scene s\ntext_show 1 line_a 480 72 60\noverlay 1 art 0 0 8 8 60\nwait 6000\n");
    scene_player_set_paused(1);
    update(ONE_TICK_US * 30);
    CHECK(first_line()->alpha == 0.0f);
    CHECK(first_overlay()->size_fraction == 0.0f);
    finish_scene();
}

int main(void)
{
    test_text_steps_reach_the_layer_with_every_field();
    test_overlay_steps_reach_the_layer_with_every_field();
    test_a_second_overlay_step_for_an_id_restarts_its_growth();
    test_hide_steps_name_the_line_and_overlay_they_remove();
    test_a_hide_step_passes_its_fade_length_on();
    test_hide_all_step_removes_every_line();
    test_stopping_clears_text_and_overlays();
    test_playing_again_clears_text_and_overlays();
    test_a_scene_that_fails_to_play_keeps_its_text();
    test_shutting_down_clears_text();
    test_skipping_a_hold_runs_the_hide_that_follows_it();
    test_fades_keep_running_after_the_scene_finishes();
    test_pausing_stops_the_timers();
    fake_files_reset();
    return test_finish("unit/scene_player_layers");
}
