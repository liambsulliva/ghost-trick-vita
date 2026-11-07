#include <string.h>
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "file.h"
#include "input.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "test_support.h"
#include "viewer_app.h"

#define FRAME_US 16667
#define ROOM_WIDTH 124
#define OTHER_ROOM_WIDTH 116
#define ART_WIDTH 84
#define TEXT_WIDTH 200
#define CARD_WIDTH 72
#define DS_WINDOW_BOTTOM_ON_SCREEN 542.0f
#define SCENE_HEAD "scene opening\nroom 1\nplate room 0 0 1024 384\n"
#define THE_REST "camera 0 192\nroom 2\nplate other 0 0 1024 384\nwait 6000\n"

static void add_texture(const char *name, unsigned int width, unsigned int height)
{
    char path[FILE_PATH_MAX];

    snprintf(path, sizeof(path), "assets/textures/%s.gtex", name);
    fake_files_add_rgba_texture(path, width, height);
}

static void add_text_file(const char *path, const char *text)
{
    fake_files_add(path, (const unsigned char *)text, (unsigned int)strlen(text));
}

static void run_frames(int frame_count)
{
    struct input_state no_input;
    int frame;

    memset(&no_input, 0, sizeof(no_input));
    for (frame = 0; frame < frame_count; frame++) {
        fake_clock_advance_us(FRAME_US);
        viewer_app_update(&no_input, FRAME_US);
    }
}

static void press(unsigned int button)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.pressed = button;
    viewer_app_update(&input, 0);
}

static void draw_frame(void)
{
    fake_renderer_clear_draws();
    viewer_app_draw();
}

static void start_scene(const char *scene_text)
{
    int frame;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_audio_reset();
    fake_memory_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    add_texture("room", ROOM_WIDTH, 54);
    add_texture("other", OTHER_ROOM_WIDTH, 54);
    add_texture("art", ART_WIDTH, 20);
    add_texture("line", TEXT_WIDTH, 52);
    add_texture("card", CARD_WIDTH, 48);
    add_text_file("assets/manifest.txt", "scene opening\nbackground room\n");
    add_text_file("assets/scenes/opening.scene", scene_text);
    viewer_app_start("assets");
    viewer_app_play_scene("opening");
    for (frame = 0; frame < 4096 && scene_player_status() == SCENE_PLAYER_PRELOADING; frame++) {
        run_frames(1);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
}

static int nearly_equal(float actual, float expected)
{
    float difference;

    difference = actual - expected;
    return difference > -0.01f && difference < 0.01f;
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static int index_of_snapshot_draw(void)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->is_snapshot) {
            return index;
        }
    }
    return -1;
}

static int index_of_texture_width(int width)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_texture_width(fake_renderer_draw(index)->texture) == width) {
            return index;
        }
    }
    return -1;
}

static int snapshot_holds_texture_width(int width)
{
    int index;

    for (index = 0; index < fake_renderer_snapshot_draw_count(); index++) {
        if (fake_renderer_texture_width(fake_renderer_snapshot_draw(index)->texture) == width) {
            return 1;
        }
    }
    return 0;
}

static int count_snapshot_filled_rects_of(float red, float alpha)
{
    const struct fake_draw *draw;
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_snapshot_draw_count(); index++) {
        draw = fake_renderer_snapshot_draw(index);
        if (draw->is_filled_rect && draw->color.red == red && draw->color.alpha == alpha) {
            count = count + 1;
        }
    }
    return count;
}

static int snapshot_holds_a_whole_screen_of_solid_black(void)
{
    const struct fake_draw *draw;
    int index;

    for (index = 0; index < fake_renderer_snapshot_draw_count(); index++) {
        draw = fake_renderer_snapshot_draw(index);
        if (draw->is_filled_rect && draw->color.alpha == 1.0f && draw->screen_area.width == RENDERER_WIDTH && draw->screen_area.height == RENDERER_HEIGHT) {
            return 1;
        }
    }
    return 0;
}

static int snapshot_holds_a_rect_batch(void)
{
    int index;

    for (index = 0; index < fake_renderer_snapshot_draw_count(); index++) {
        if (fake_renderer_snapshot_draw(index)->is_rect_batch) {
            return 1;
        }
    }
    return 0;
}

static void test_the_step_takes_one_snapshot_of_the_stage_before_the_steps_after_it(void)
{
    start_scene(SCENE_HEAD "wait 2\ndissolve 32 2\n" THE_REST);
    CHECK_INT(fake_renderer_snapshots_taken(), 0);
    run_frames(4);
    CHECK_INT(fake_renderer_snapshots_taken(), 1);
    CHECK(!fake_renderer_snapshot_is_open());
    CHECK(snapshot_holds_texture_width(ROOM_WIDTH));
    CHECK(!snapshot_holds_texture_width(OTHER_ROOM_WIDTH));
    draw_frame();
    CHECK(index_of_texture_width(OTHER_ROOM_WIDTH) >= 0);
    CHECK(index_of_texture_width(ROOM_WIDTH) < 0);
    viewer_app_stop();
}

static void test_the_snapshot_is_made_once_and_never_again_while_it_plays(void)
{
    int textures_before;

    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(2);
    textures_before = fake_renderer_textures_created();
    CHECK_INT(fake_renderer_snapshots_taken(), 1);
    run_frames(20);
    draw_frame();
    CHECK(index_of_snapshot_draw() >= 0);
    CHECK_INT(fake_renderer_snapshots_taken(), 1);
    CHECK_INT(fake_renderer_textures_created(), textures_before);
    viewer_app_stop();
}

static void test_the_snapshot_fades_and_grows_about_the_bottom_centre_of_the_ds_window(void)
{
    const struct fake_draw *snapshot;

    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(1);
    draw_frame();
    snapshot = fake_renderer_draw(index_of_snapshot_draw());
    CHECK(snapshot->color.alpha > 0.9f && snapshot->color.alpha <= 1.0f);
    CHECK(snapshot->screen_area.width >= RENDERER_WIDTH && snapshot->screen_area.width < RENDERER_WIDTH * 1.1f);
    run_frames(15);
    draw_frame();
    snapshot = fake_renderer_draw(index_of_snapshot_draw());
    CHECK(snapshot->color.alpha > 0.45f && snapshot->color.alpha < 0.55f);
    CHECK(snapshot->screen_area.width > RENDERER_WIDTH * 1.45f && snapshot->screen_area.width < RENDERER_WIDTH * 1.55f);
    CHECK(snapshot->screen_area.height > RENDERER_HEIGHT * 1.45f && snapshot->screen_area.height < RENDERER_HEIGHT * 1.55f);
    CHECK(nearly_equal(snapshot->screen_area.x + snapshot->screen_area.width / 2.0f, RENDERER_WIDTH / 2.0f));
    CHECK(nearly_equal(snapshot->screen_area.y + snapshot->screen_area.height * (DS_WINDOW_BOTTOM_ON_SCREEN / RENDERER_HEIGHT), DS_WINDOW_BOTTOM_ON_SCREEN));
    viewer_app_stop();
}

static void test_the_anchor_is_where_the_ds_window_ends_on_screen(void)
{
    CHECK(nearly_equal(SCENE_VIEW_DISSOLVE_ANCHOR_X, 480.0f));
    CHECK(nearly_equal(SCENE_VIEW_DISSOLVE_ANCHOR_Y, DS_WINDOW_BOTTOM_ON_SCREEN));
}

static void test_a_named_end_zoom_changes_the_rate(void)
{
    const struct fake_draw *snapshot;

    start_scene(SCENE_HEAD "dissolve 32 1\n" THE_REST);
    run_frames(17);
    draw_frame();
    snapshot = fake_renderer_draw(index_of_snapshot_draw());
    CHECK(nearly_equal(snapshot->screen_area.width, RENDERER_WIDTH));
    CHECK(nearly_equal(snapshot->screen_area.x, 0.0f));
    CHECK(snapshot->color.alpha > 0.45f && snapshot->color.alpha < 0.55f);
    viewer_app_stop();
}

static void test_the_snapshot_is_gone_when_its_ticks_are_over(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(31);
    draw_frame();
    CHECK(index_of_snapshot_draw() >= 0);
    run_frames(3);
    draw_frame();
    CHECK(index_of_snapshot_draw() < 0);
    viewer_app_stop();
}

static void test_the_dissolve_does_not_hold_the_script(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\nwait 4\nwait 6000\n");
    run_frames(1);
    CHECK_INT(scene_player_next_step(), 4);
    run_frames(4);
    CHECK_INT(scene_player_next_step(), 5);
    viewer_app_stop();
}

static void test_the_snapshot_holds_stage_layers_and_nothing_above_them(void)
{
    start_scene("scene opening\nroom 1\nplate room 0 0 1024 384\ncamera 0 192\nmask_on\nbeam_create b 10 10 10 300 20\noverlay 1 art 10 200 30 20 0\ndim_to 0.5 0\ntext_show 1 line 480 72 0\nflash 1 6000\ncard card\nfade_out 1\nwait 1\ndissolve 32 2\nwait 6000\n");
    run_frames(4);
    CHECK_INT(fake_renderer_snapshots_taken(), 1);
    CHECK(snapshot_holds_texture_width(ROOM_WIDTH));
    CHECK(snapshot_holds_texture_width(ART_WIDTH));
    CHECK(snapshot_holds_a_rect_batch());
    CHECK_INT(count_snapshot_filled_rects_of(0.0f, 0.5f), 1);
    CHECK_INT(count_snapshot_filled_rects_of(1.0f, 1.0f), 0);
    CHECK(!snapshot_holds_a_whole_screen_of_solid_black());
    CHECK(!snapshot_holds_texture_width(TEXT_WIDTH));
    CHECK(!snapshot_holds_texture_width(CARD_WIDTH));
    viewer_app_stop();
}

static void test_the_black_around_narrow_art_goes_into_the_snapshot(void)
{
    start_scene("scene opening\nroom 1\nplate room 0 0 256 192\nwait 1\ndissolve 32 2\nwait 6000\n");
    run_frames(3);
    CHECK_INT(count_snapshot_filled_rects_of(0.0f, 1.0f), 4);
    viewer_app_stop();
}

static void test_the_snapshot_sits_above_the_stage_and_below_everything_else(void)
{
    const struct fake_draw *last;
    int snapshot_index;

    start_scene("scene opening\nroom 1\nplate room 0 0 1024 384\ncamera 0 192\noverlay 1 art 10 200 30 20 0\ntext_show 1 line 480 72 0\nflash 1 6000\ncard card\nfade_out 1\nwait 1\ndissolve 32 2\nwait 6000\n");
    run_frames(4);
    draw_frame();
    snapshot_index = index_of_snapshot_draw();
    CHECK(snapshot_index >= 0);
    CHECK(index_of_texture_width(ROOM_WIDTH) < snapshot_index);
    CHECK(index_of_texture_width(ART_WIDTH) < snapshot_index);
    CHECK(snapshot_index < index_of_texture_width(TEXT_WIDTH));
    CHECK(fake_renderer_draw(index_of_texture_width(TEXT_WIDTH) + 1)->is_filled_rect);
    CHECK(fake_renderer_draw(index_of_texture_width(TEXT_WIDTH) + 1)->color.red == 1.0f);
    CHECK(index_of_texture_width(TEXT_WIDTH) + 1 < index_of_texture_width(CARD_WIDTH));
    last = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(last->is_filled_rect);
    CHECK(last->color.red == 0.0f);
    viewer_app_stop();
}

static void test_a_replay_during_a_dissolve_clears_it(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(4);
    draw_frame();
    CHECK(index_of_snapshot_draw() >= 0);
    viewer_app_play_scene("opening");
    CHECK(!scene_stage_view()->effects.dissolve_is_on);
    viewer_app_stop();
}

static void test_browsing_during_a_dissolve_clears_it(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(4);
    CHECK(scene_stage_view()->effects.dissolve_is_on);
    CHECK(viewer_app_show_background("room"));
    CHECK(!scene_stage_view()->effects.dissolve_is_on);
    run_frames(2);
    viewer_app_set_hud_visible(0);
    draw_frame();
    CHECK(index_of_snapshot_draw() < 0);
    viewer_app_stop();
}

static void test_a_skip_during_a_dissolve_leaves_no_snapshot_on_screen(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(4);
    press(INPUT_CROSS);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(index_of_snapshot_draw() >= 0);
    run_frames(32);
    draw_frame();
    CHECK(index_of_snapshot_draw() < 0);
    viewer_app_stop();
}

static void test_a_pause_holds_the_dissolve_and_a_resume_finishes_it(void)
{
    float progress;

    start_scene(SCENE_HEAD "dissolve 32 2\n" THE_REST);
    run_frames(4);
    scene_player_set_paused(1);
    progress = scene_stage_view()->effects.dissolve_progress.value;
    run_frames(60);
    CHECK(scene_stage_view()->effects.dissolve_progress.value == progress);
    draw_frame();
    CHECK(index_of_snapshot_draw() >= 0);
    scene_player_set_paused(0);
    run_frames(32);
    draw_frame();
    CHECK(index_of_snapshot_draw() < 0);
    viewer_app_stop();
}

static void test_a_second_dissolve_replaces_the_first_without_snapshotting_it(void)
{
    start_scene(SCENE_HEAD "dissolve 32 2\nwait 8\ndissolve 32 2\nwait 6000\n");
    run_frames(1);
    CHECK_INT(fake_renderer_snapshots_taken(), 1);
    run_frames(9);
    CHECK_INT(fake_renderer_snapshots_taken(), 2);
    CHECK(scene_stage_view()->effects.dissolve_progress.value < 0.1f);
    viewer_app_stop();
}

static void test_a_snapshot_that_cannot_be_taken_is_a_plain_cut_and_says_so(void)
{
    start_scene(SCENE_HEAD "wait 2\ndissolve 32 2\n" THE_REST);
    fake_renderer_fail_snapshots(1);
    run_frames(4);
    CHECK(!scene_stage_view()->effects.dissolve_is_on);
    draw_frame();
    CHECK(index_of_snapshot_draw() < 0);
    CHECK(index_of_texture_width(OTHER_ROOM_WIDTH) >= 0);
    CHECK_HAS_TEXT(log_text(), "scene dissolve ticks=32 end_zoom=2.000 snapshot=0 snapshot_us=");
    viewer_app_stop();
}

static void test_the_dissolve_is_logged_with_what_the_snapshot_cost(void)
{
    start_scene(SCENE_HEAD "wait 2\ndissolve 32 2\n" THE_REST);
    run_frames(4);
    CHECK_HAS_TEXT(log_text(), "scene dissolve ticks=32 end_zoom=2.000 snapshot=1 snapshot_us=");
    viewer_app_stop();
}

int main(void)
{
    test_the_step_takes_one_snapshot_of_the_stage_before_the_steps_after_it();
    test_the_snapshot_is_made_once_and_never_again_while_it_plays();
    test_the_snapshot_fades_and_grows_about_the_bottom_centre_of_the_ds_window();
    test_the_anchor_is_where_the_ds_window_ends_on_screen();
    test_a_named_end_zoom_changes_the_rate();
    test_the_snapshot_is_gone_when_its_ticks_are_over();
    test_the_dissolve_does_not_hold_the_script();
    test_the_snapshot_holds_stage_layers_and_nothing_above_them();
    test_the_black_around_narrow_art_goes_into_the_snapshot();
    test_the_snapshot_sits_above_the_stage_and_below_everything_else();
    test_a_replay_during_a_dissolve_clears_it();
    test_browsing_during_a_dissolve_clears_it();
    test_a_skip_during_a_dissolve_leaves_no_snapshot_on_screen();
    test_a_pause_holds_the_dissolve_and_a_resume_finishes_it();
    test_a_second_dissolve_replaces_the_first_without_snapshotting_it();
    test_a_snapshot_that_cannot_be_taken_is_a_plain_cut_and_says_so();
    test_the_dissolve_is_logged_with_what_the_snapshot_cost();
    fake_files_reset();
    return test_finish("integration/scene_dissolve");
}
