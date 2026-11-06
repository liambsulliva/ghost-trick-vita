#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "file.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "test_support.h"
#include "texture_cache.h"

#define LONGEST_UPDATE_US 500000ULL

static int nearly(float actual, float expected)
{
    float difference;

    difference = actual - expected;
    if (difference < 0.0f) {
        difference = -difference;
    }
    return difference < 0.01f;
}

static const struct scene_effects *effects(void)
{
    return &scene_stage_view()->effects;
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static void update_ticks(int ticks)
{
    scene_player_update((unsigned long long)ticks * MICROSECONDS_PER_TICK);
}

static void start_scene(const char *scene_text)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    fake_files_add("scenes/opening.scene", (const unsigned char *)scene_text, (unsigned int)strlen(scene_text));
    texture_cache_init("textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    animation_library_init("animations");
    scene_player_init("scenes");
    CHECK(scene_player_play("opening"));
    scene_player_update(0);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    scene_player_update(0);
}

static void stop_scene(void)
{
    scene_player_shutdown();
    animation_library_clear();
    texture_cache_clear();
    fake_files_reset();
}

static void test_a_beam_animates_while_the_script_waits(void)
{
    start_scene("scene opening\nmask_on\nbeam_create left 56 128 56 367 0\nbeam_animate left 56 367 30 10\nwait 600\nwait 600\n");
    CHECK(effects()->mask_is_on);
    CHECK_INT(scene_player_next_step(), 4);
    update_ticks(5);
    CHECK_INT(scene_player_next_step(), 4);
    CHECK(nearly(effects()->beams[0].half_width.value, 15.0f));
    update_ticks(5);
    CHECK(effects()->beams[0].half_width.value == 30.0f);
    CHECK_INT(scene_player_next_step(), 4);
    stop_scene();
}

static void test_effect_steps_do_not_hold_the_script(void)
{
    start_scene("scene opening\nbeam_create left 0 0 0 100 0\nbeam_animate left 0 100 30 600\ndim_to 0.5 600\nflash 1 600\niris 10 20 100 600\nwait 600\n");
    CHECK_INT(scene_player_next_step(), 6);
    CHECK(effects()->beams[0].in_use);
    CHECK(effects()->iris_is_on);
    CHECK(effects()->flash_level.value == 1.0f);
    stop_scene();
}

static void test_two_effects_started_at_different_times_keep_their_own_timers(void)
{
    start_scene("scene opening\ndim_to 0 10\nwait 5\nflash 1 10\nwait 600\n");
    update_ticks(5);
    update_ticks(5);
    CHECK(effects()->brightness.value == 0.0f);
    CHECK(nearly(effects()->flash_level.value, 0.5f));
    stop_scene();
}

static void test_a_long_frame_gives_an_effect_only_the_time_after_its_step(void)
{
    start_scene("scene opening\nwait 10\nflash 1 20\nwait 600\n");
    update_ticks(15);
    CHECK_INT(scene_player_next_step(), 3);
    CHECK(nearly(effects()->flash_level.value, 0.75f));
    stop_scene();
}

static void test_a_long_frame_gives_an_effect_the_whole_hold_that_passed(void)
{
    start_scene("scene opening\nflash 1 20\nwait 5\nwait 600\n");
    update_ticks(10);
    CHECK_INT(scene_player_next_step(), 3);
    CHECK(nearly(effects()->flash_level.value, 0.5f));
    stop_scene();
}

static void test_a_long_stall_only_counts_as_half_a_second_for_effects_too(void)
{
    start_scene("scene opening\ndim_to 0 60\nwait 600\n");
    scene_player_update(LONGEST_UPDATE_US * 40ULL);
    CHECK(nearly(effects()->brightness.value, 0.5f));
    stop_scene();
}

static void test_effects_keep_going_after_the_script_ends(void)
{
    start_scene("scene opening\nflash 1 20\nwait 5\n");
    update_ticks(10);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK(nearly(effects()->flash_level.value, 0.5f));
    update_ticks(5);
    CHECK(nearly(effects()->flash_level.value, 0.25f));
    stop_scene();
}

static void test_pausing_stops_effect_timers(void)
{
    start_scene("scene opening\ndim_to 0 60\nwait 600\n");
    update_ticks(30);
    scene_player_set_paused(1);
    update_ticks(30);
    CHECK(nearly(effects()->brightness.value, 0.5f));
    scene_player_set_paused(0);
    update_ticks(30);
    CHECK(effects()->brightness.value == 0.0f);
    stop_scene();
}

static void test_skipping_a_hold_leaves_a_running_effect_on_its_own_timer(void)
{
    start_scene("scene opening\ndim_to 0 60\nwait 600\nwait 600\n");
    update_ticks(30);
    scene_player_skip_hold();
    update_ticks(0);
    CHECK_INT(scene_player_next_step(), 3);
    CHECK(nearly(effects()->brightness.value, 0.5f));
    update_ticks(30);
    CHECK(effects()->brightness.value == 0.0f);
    stop_scene();
}

static void test_a_new_room_keeps_the_mask_and_beams(void)
{
    start_scene("scene opening\nmask_on\nbeam_create left 0 0 0 100 10\nwait 5\nroom 7\nwait 600\n");
    update_ticks(6);
    CHECK_INT(scene_player_next_step(), 5);
    CHECK(effects()->mask_is_on);
    CHECK(effects()->beams[0].in_use);
    stop_scene();
}

static void test_mask_off_and_beam_delete_steps_take_effect(void)
{
    start_scene("scene opening\nmask_on\nbeam_create left 0 0 0 100 10\nbeam_delete left\nmask_off\nwait 600\n");
    CHECK(!effects()->mask_is_on);
    CHECK(!effects()->beams[0].in_use);
    CHECK_HAS_TEXT(log_text(), "scene beam_create name=left");
    CHECK_HAS_TEXT(log_text(), "scene mask on");
    CHECK_HAS_TEXT(log_text(), "scene mask off");
    stop_scene();
}

static void test_the_steps_that_matter_to_a_device_run_are_logged(void)
{
    start_scene("scene opening\nbeam_create left 0 0 0 100 0\nbeam_animate left 0 100 30 7\niris 1 2 3 8\ndim_to 0.5 9\nflash 1 6\nwait 600\n");
    CHECK_HAS_TEXT(log_text(), "scene beam_animate name=left ticks=7");
    CHECK_HAS_TEXT(log_text(), "scene iris ticks=8");
    CHECK_HAS_TEXT(log_text(), "scene dim ticks=9");
    CHECK_HAS_TEXT(log_text(), "scene flash ticks=6");
    stop_scene();
}

static void test_stopping_or_replaying_a_scene_clears_every_effect(void)
{
    start_scene("scene opening\nmask_on\nbeam_create left 0 0 0 100 10\niris 0 0 10 10\ndim_to 0.5 0\nflash 1 100\nwait 600\n");
    CHECK(effects()->mask_is_on);
    scene_player_stop();
    CHECK(!effects()->mask_is_on);
    CHECK(!effects()->iris_is_on);
    CHECK(!effects()->beams[0].in_use);
    CHECK(effects()->brightness.value == 1.0f);
    CHECK(effects()->flash_level.value == 0.0f);
    CHECK(scene_player_play("opening"));
    scene_player_update(0);
    scene_player_update(0);
    CHECK(effects()->mask_is_on);
    CHECK(scene_player_play("opening"));
    CHECK(!effects()->mask_is_on);
    CHECK(effects()->brightness.value == 1.0f);
    stop_scene();
}

int main(void)
{
    test_a_beam_animates_while_the_script_waits();
    test_effect_steps_do_not_hold_the_script();
    test_two_effects_started_at_different_times_keep_their_own_timers();
    test_a_long_frame_gives_an_effect_only_the_time_after_its_step();
    test_a_long_frame_gives_an_effect_the_whole_hold_that_passed();
    test_a_long_stall_only_counts_as_half_a_second_for_effects_too();
    test_effects_keep_going_after_the_script_ends();
    test_pausing_stops_effect_timers();
    test_skipping_a_hold_leaves_a_running_effect_on_its_own_timer();
    test_a_new_room_keeps_the_mask_and_beams();
    test_mask_off_and_beam_delete_steps_take_effect();
    test_the_steps_that_matter_to_a_device_run_are_logged();
    test_stopping_or_replaying_a_scene_clears_every_effect();
    return test_finish("unit/scene_player");
}
