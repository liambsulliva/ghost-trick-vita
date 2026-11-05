#include <stdlib.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "file.h"
#include "input.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_view.h"
#include "test_support.h"
#include "texture_cache.h"
#include "texture_file.h"
#include "viewer_app.h"

#define FRAME_US 16667
#define WHITE_ROOM_WIDTH 120
#define OTHER_ROOM_WIDTH 100
#define CHARACTER_WIDTH 32
#define CARD_WIDTH 72
#define BAR_WIDTH 16

static void write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
    bytes[2] = (unsigned char)((value >> 16) & 0xff);
    bytes[3] = (unsigned char)((value >> 24) & 0xff);
}

static void add_texture(const char *name, unsigned int width, unsigned int height)
{
    char path[FILE_PATH_MAX];
    unsigned int pixel_bytes;
    unsigned char *file;

    snprintf(path, sizeof(path), "assets/textures/%s.gtex", name);
    pixel_bytes = width * height * 4;
    file = calloc(1, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    memcpy(file, "GTEX", 4);
    write_u32(&file[4], width);
    write_u32(&file[8], height);
    write_u32(&file[12], TEXTURE_FILE_FORMAT_RGBA8888);
    write_u32(&file[16], pixel_bytes);
    fake_files_add(path, file, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    free(file);
}

static void add_text_file(const char *path, const char *text)
{
    fake_files_add(path, (const unsigned char *)text, (unsigned int)strlen(text));
}

static void set_up_platform(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
}

static void set_up_textures_and_animations(void)
{
    add_texture("room_a", WHITE_ROOM_WIDTH, 54);
    add_texture("room_b", OTHER_ROOM_WIDTH, 54);
    add_texture("walk_page", CHARACTER_WIDTH, 32);
    add_texture("idle_page", CHARACTER_WIDTH, 32);
    add_texture("card", CARD_WIDTH, 48);
    add_texture("left_bar", BAR_WIDTH, 32);
    add_texture("right_bar", BAR_WIDTH, 32);
    add_text_file("assets/animations/walk.anim", "animation walk\nticks_per_second 10\nloops 1\nframe walk_page 1 -10 -20 30 40 0 0 0.5 1\nframe walk_page 1 -6 -20 30 40 0.5 0 0.5 1\n");
    add_text_file("assets/animations/idle.anim", "animation idle\nticks_per_second 10\nloops 1\nframe idle_page 1 0 0 10 10 0 0 1 1\nframe - 1 0 0 0 0 0 0 0 0\n");
}

static void set_up_browsing_assets(void)
{
    set_up_textures_and_animations();
    add_text_file("assets/manifest.txt", "background room_a\nbackground room_b\nanimation walk\nanimation idle\n");
}

static void set_up_scene_assets(const char *scene_text)
{
    set_up_textures_and_animations();
    add_text_file("assets/manifest.txt", "scene opening\nanimation walk\n");
    add_text_file("assets/scenes/opening.scene", scene_text);
}

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static void run_frames(int frame_count)
{
    struct input_state no_input;
    int frame;

    no_input.held = 0;
    no_input.pressed = 0;
    for (frame = 0; frame < frame_count; frame++) {
        fake_clock_advance_us(FRAME_US);
        viewer_app_update(&no_input, FRAME_US);
    }
}

static void press(unsigned int button)
{
    struct input_state input;

    input.held = button;
    input.pressed = button;
    viewer_app_update(&input, 0);
}

static void draw_frame(void)
{
    fake_renderer_clear_draws();
    viewer_app_draw();
}

static int count_draws_with_texture_width(int width)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_texture_width(fake_renderer_draw(index)->texture) == width) {
            count = count + 1;
        }
    }
    return count;
}

static const struct fake_draw *find_draw_with_texture_width(int width)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_texture_width(fake_renderer_draw(index)->texture) == width) {
            return fake_renderer_draw(index);
        }
    }
    return NULL;
}

static int index_of_draw_with_texture_width(int width)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_texture_width(fake_renderer_draw(index)->texture) == width) {
            return index;
        }
    }
    return -1;
}

static int nearly_equal(float first, float second)
{
    float difference;

    difference = first - second;
    if (difference < 0.0f) {
        difference = -difference;
    }
    return difference < 0.01f;
}

static void test_app_without_assets_shows_placeholder_and_says_so(void)
{
    set_up_platform();
    viewer_app_start("assets");
    CHECK(!viewer_app_has_assets());
    CHECK_HAS_TEXT(log_text(), "manifest missing");
    run_frames(3);
    draw_frame();
    CHECK(fake_renderer_draw_count() > 0);
    CHECK_INT(count_draws_with_texture_width(64), 1);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_browsing_draws_background_then_character_in_playfield(void)
{
    const struct fake_draw *background;
    const struct fake_draw *character;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    CHECK(viewer_app_has_assets());
    run_frames(1);
    viewer_app_set_hud_visible(0);
    draw_frame();
    background = find_draw_with_texture_width(WHITE_ROOM_WIDTH);
    character = find_draw_with_texture_width(CHARACTER_WIDTH);
    CHECK(background != NULL);
    CHECK(character != NULL);
    CHECK(index_of_draw_with_texture_width(WHITE_ROOM_WIDTH) < index_of_draw_with_texture_width(CHARACTER_WIDTH));
    CHECK(background->screen_area.x == SCENE_VIEW_PLAYFIELD_X);
    CHECK(background->screen_area.y == SCENE_VIEW_PLAYFIELD_Y);
    CHECK(nearly_equal(background->screen_area.width, WHITE_ROOM_WIDTH));
    CHECK(character->screen_area.x == SCENE_VIEW_PLAYFIELD_X + (128.0f - 10.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(character->screen_area.y == SCENE_VIEW_PLAYFIELD_Y + (176.0f - 20.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(character->screen_area.width == 30.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(character->screen_area.height == 40.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(character->texture_area.width == 0.5f);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_browsing_plays_animation_over_time_and_reports_coverage(void)
{
    const struct fake_draw *character;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    character = find_draw_with_texture_width(CHARACTER_WIDTH);
    CHECK(character->texture_area.x == 0.5f);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "name=walk frames_shown=2 frames_total=2");
}

static void test_coverage_reports_unplayed_frames(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    run_frames(1);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "name=walk frames_shown=1 frames_total=2");
}

static void test_blank_frames_draw_nothing(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    CHECK(viewer_app_show_animation("idle"));
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    run_frames(6);
    draw_frame();
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) == NULL);
    viewer_app_stop();
    CHECK(strstr(log_text(), "texture missing name=-") == NULL);
}

static void test_missing_textures_are_marked_visibly(void)
{
    const struct fake_draw *draw;
    int index;
    int found_marker;

    set_up_platform();
    set_up_browsing_assets();
    add_text_file("assets/animations/walk.anim", "frame not_converted 1 0 0 10 10 0 0 1 1\n");
    viewer_app_start("assets");
    run_frames(2);
    CHECK_HAS_TEXT(log_text(), "texture missing name=not_converted");
    viewer_app_set_hud_visible(0);
    draw_frame();
    found_marker = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (draw->is_filled_rect && draw->color.red == 1.0f && draw->color.blue == 1.0f && draw->color.green == 0.0f) {
            found_marker = 1;
        }
    }
    CHECK(found_marker);
    viewer_app_stop();
}

static void test_app_survives_missing_and_invalid_animation_files(void)
{
    set_up_platform();
    set_up_browsing_assets();
    add_text_file("assets/animations/walk.anim", "this is not an animation\n");
    fake_files_add("assets/animations/idle.anim", (const unsigned char *)"", 0);
    viewer_app_start("assets");
    CHECK_HAS_TEXT(log_text(), "animation invalid name=walk");
    CHECK(viewer_app_show_animation("idle") == 0);
    CHECK(viewer_app_show_animation("not_listed") == 0);
    CHECK_HAS_TEXT(log_text(), "animation not_in_manifest name=not_listed");
    run_frames(3);
    draw_frame();
    CHECK(fake_renderer_draw_count() > 0);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_app_rejects_invalid_manifest(void)
{
    set_up_platform();
    add_text_file("assets/manifest.txt", "wrong line\n");
    viewer_app_start("assets");
    CHECK(!viewer_app_has_assets());
    CHECK_HAS_TEXT(log_text(), "manifest invalid");
    viewer_app_stop();
}

static void test_buttons_switch_background_and_animation(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    press(INPUT_RIGHT);
    CHECK_HAS_TEXT(log_text(), "background selected name=room_b");
    run_frames(2);
    CHECK_HAS_TEXT(log_text(), "background shown name=room_b");
    press(INPUT_DOWN);
    CHECK_HAS_TEXT(log_text(), "animation selected name=idle");
    press(INPUT_UP);
    run_frames(1);
    draw_frame();
    CHECK(find_draw_with_texture_width(OTHER_ROOM_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_cross_pauses_and_triangle_hides_hud(void)
{
    int draws_with_hud;
    float paused_u;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    run_frames(1);
    press(INPUT_CROSS);
    draw_frame();
    paused_u = find_draw_with_texture_width(CHARACTER_WIDTH)->texture_area.x;
    run_frames(30);
    draw_frame();
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH)->texture_area.x == paused_u);
    draws_with_hud = fake_renderer_draw_count();
    press(INPUT_TRIANGLE);
    draw_frame();
    CHECK(fake_renderer_draw_count() < draws_with_hud);
    viewer_app_stop();
}

static void test_list_widget_opens_selects_and_closes(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    press(INPUT_R);
    press(INPUT_DOWN);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_HAS_TEXT(log_text(), "animation selected name=idle");
    CHECK(strstr(log_text(), "background selected") == NULL);
    press(INPUT_R);
    press(INPUT_CIRCLE);
    press(INPUT_RIGHT);
    CHECK_HAS_TEXT(log_text(), "background selected name=room_b");
    viewer_app_stop();
}

static void test_list_widget_closes_after_a_selection(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    press(INPUT_R);
    press(INPUT_CROSS);
    press(INPUT_RIGHT);
    CHECK_HAS_TEXT(log_text(), "background selected name=room_b");
    viewer_app_stop();
}

static void test_slow_reads_hold_the_frame_and_get_reported_as_late(void)
{
    set_up_platform();
    set_up_browsing_assets();
    add_text_file("assets/animations/walk.anim", "animation walk\nticks_per_second 10\nloops 1\nframe walk_page 1 0 0 10 10 0 0 1 1\nframe second_page 1 0 0 10 10 0 0 1 1\n");
    add_texture("second_page", 40, 32);
    fake_reader_hold_reads(1);
    viewer_app_start("assets");
    fake_reader_release_reads(1);
    run_frames(11);
    viewer_app_set_hud_visible(0);
    draw_frame();
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(40) == NULL);
    fake_reader_release_reads(4);
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(40) != NULL);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "frames_shown=2 frames_total=2 late_frames=1");
}

static void test_old_background_stays_until_the_new_one_has_loaded(void)
{
    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    run_frames(2);
    fake_reader_hold_reads(1);
    press(INPUT_RIGHT);
    run_frames(1);
    draw_frame();
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(OTHER_ROOM_WIDTH) == NULL);
    CHECK(strstr(log_text(), "background shown name=room_b") == NULL);
    fake_reader_release_reads(4);
    run_frames(4);
    draw_frame();
    CHECK(find_draw_with_texture_width(OTHER_ROOM_WIDTH) != NULL);
    CHECK_HAS_TEXT(log_text(), "background shown name=room_b");
    viewer_app_stop();
}

static void test_status_text_reaches_the_hud(void)
{
    int draws_before;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    run_frames(1);
    draw_frame();
    draws_before = fake_renderer_draw_count();
    viewer_app_set_status_text("frame 16.6ms");
    draw_frame();
    CHECK_INT(fake_renderer_draw_count(), draws_before + 11);
    viewer_app_stop();
}

static void test_scene_waits_for_textures_before_running(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nobject hero walk 100 150 1 0\nwait 60\n");
    fake_reader_hold_reads(1);
    viewer_app_start("assets");
    run_frames(3);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    CHECK(strstr(log_text(), "scene ready") == NULL);
    draw_frame();
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) == NULL);
    fake_reader_release_reads(10);
    run_frames(5);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene ready name=opening");
    run_frames(1);
    draw_frame();
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_scene_applies_camera_flip_layers_and_bars(void)
{
    const struct fake_draw *hero;
    const struct fake_draw *back;
    const struct fake_draw *front;

    set_up_platform();
    set_up_scene_assets("scene opening\nside_bars left_bar right_bar\nroom 1\nplate room_a 0 0 256 192\nobject hero walk 100 150 2 1\nobject front idle 50 40 5 0\nobject back idle 60 40 1 0\ncamera 10 20\nwait 600\n");
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    back = fake_renderer_draw(1);
    hero = fake_renderer_draw(2);
    front = fake_renderer_draw(3);
    CHECK(back->screen_area.x == SCENE_VIEW_PLAYFIELD_X + (60.0f - 10.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(front->screen_area.x == SCENE_VIEW_PLAYFIELD_X + (50.0f - 10.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(hero->screen_area.x == SCENE_VIEW_PLAYFIELD_X + (100.0f + 10.0f - 30.0f - 10.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(hero->screen_area.y == SCENE_VIEW_PLAYFIELD_Y + (150.0f - 20.0f - 20.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL);
    CHECK(hero->texture_area.width == -0.5f);
    CHECK(hero->texture_area.x == 0.5f);
    CHECK_INT(count_draws_with_texture_width(BAR_WIDTH), 2);
    CHECK(fake_renderer_draw(fake_renderer_draw_count() - 1)->screen_area.x == SCENE_VIEW_PLAYFIELD_X + SCENE_VIEW_PLAYFIELD_WIDTH);
    viewer_app_stop();
}

static void test_scene_runs_steps_in_order_and_finishes(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6\nhide_card\nroom 1\nplate room_a 0 0 256 192\nwait 6\nroom 2\nplate room_b 0 0 256 192\nremove_object nobody\n");
    viewer_app_start("assets");
    run_frames(5);
    draw_frame();
    CHECK(find_draw_with_texture_width(CARD_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) == NULL);
    run_frames(8);
    draw_frame();
    CHECK(find_draw_with_texture_width(CARD_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(OTHER_ROOM_WIDTH) == NULL);
    CHECK_HAS_TEXT(log_text(), "scene room id=1");
    CHECK(strstr(log_text(), "scene room id=2") == NULL);
    run_frames(8);
    draw_frame();
    CHECK(find_draw_with_texture_width(WHITE_ROOM_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(OTHER_ROOM_WIDTH) != NULL);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK_HAS_TEXT(log_text(), "scene finished name=opening");
    viewer_app_stop();
}

static void test_scene_fades_and_pans_over_time(void)
{
    const struct scene_picture *picture;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nfade_out 60\ncamera_pan 100 0 60\nfade_in 60\nwait 60\n");
    viewer_app_start("assets");
    run_frames(4);
    picture = scene_player_picture();
    run_frames(30);
    CHECK(picture->fade_amount > 0.4f && picture->fade_amount < 0.6f);
    run_frames(32);
    CHECK(picture->fade_amount == 1.0f);
    CHECK(picture->camera_x < 10.0f);
    run_frames(30);
    CHECK(picture->camera_x > 40.0f && picture->camera_x < 60.0f);
    run_frames(32);
    CHECK(picture->camera_x == 100.0f);
    CHECK(picture->fade_amount < 1.0f);
    run_frames(62);
    CHECK(picture->fade_amount == 0.0f);
    viewer_app_stop();
}

static void test_scene_survives_bad_files_and_missing_assets(void)
{
    set_up_platform();
    set_up_scene_assets("not a scene\n");
    viewer_app_start("assets");
    CHECK_HAS_TEXT(log_text(), "scene invalid name=opening");
    CHECK_INT(scene_player_status(), SCENE_PLAYER_IDLE);
    viewer_app_stop();

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate not_there 0 0 256 192\nobject hero not_an_animation 0 0 0 0\nwait 1\n");
    viewer_app_start("assets");
    run_frames(8);
    CHECK_HAS_TEXT(log_text(), "texture missing name=not_there");
    CHECK_HAS_TEXT(log_text(), "animation missing name=not_an_animation");
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_browsing_stops_a_running_scene(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK(viewer_app_show_animation("walk"));
    CHECK_INT(scene_player_status(), SCENE_PLAYER_IDLE);
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(CARD_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    press(INPUT_L);
    run_frames(4);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    viewer_app_stop();
}

static void test_long_run_keeps_memory_bounded(void)
{
    int cycle;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    for (cycle = 0; cycle < 50; cycle++) {
        press(cycle % 2 == 0 ? INPUT_DOWN : INPUT_UP);
        press(cycle % 2 == 0 ? INPUT_RIGHT : INPUT_LEFT);
        run_frames(13);
        draw_frame();
    }
    CHECK_INT(fake_renderer_textures_created(), fake_renderer_textures_alive());
    CHECK(strstr(log_text(), "texture missing") == NULL);
    CHECK(strstr(log_text(), "over_budget") == NULL);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

int main(void)
{
    test_app_without_assets_shows_placeholder_and_says_so();
    test_browsing_draws_background_then_character_in_playfield();
    test_browsing_plays_animation_over_time_and_reports_coverage();
    test_coverage_reports_unplayed_frames();
    test_blank_frames_draw_nothing();
    test_missing_textures_are_marked_visibly();
    test_app_survives_missing_and_invalid_animation_files();
    test_app_rejects_invalid_manifest();
    test_buttons_switch_background_and_animation();
    test_cross_pauses_and_triangle_hides_hud();
    test_list_widget_opens_selects_and_closes();
    test_list_widget_closes_after_a_selection();
    test_slow_reads_hold_the_frame_and_get_reported_as_late();
    test_old_background_stays_until_the_new_one_has_loaded();
    test_status_text_reaches_the_hud();
    test_scene_waits_for_textures_before_running();
    test_scene_applies_camera_flip_layers_and_bars();
    test_scene_runs_steps_in_order_and_finishes();
    test_scene_fades_and_pans_over_time();
    test_scene_survives_bad_files_and_missing_assets();
    test_browsing_stops_a_running_scene();
    test_long_run_keeps_memory_bounded();
    fake_files_reset();
    return test_finish("integration/viewer_app");
}
