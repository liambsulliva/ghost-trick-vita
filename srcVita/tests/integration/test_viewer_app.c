#include <stdlib.h>
#include "animation_library.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "file.h"
#include "fixtures/fixture_facts.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "input.h"
#include "run_log.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "test_support.h"
#include "texture_cache.h"
#include "viewer_app.h"

#define FRAME_US 16667
#define ROOM_A_WIDTH 120
#define ROOM_B_WIDTH 100
#define CHARACTER_WIDTH 32
#define CARD_WIDTH 72
#define PLACEHOLDER_TEXTURE_WIDTH 64
#define STATUS_TEXT_VISIBLE_CHARACTERS 11
#define SCENE_END_PROMPT_VISIBLE_CHARACTERS 35
#define FRAMES_LONGER_THAN_EVERY_PRELOAD_PASS 16
#define HALF_BUDGET_TEXTURE_WIDTH 4096
#define IRIS_TEXTURE_WIDTH 256
#define AUDIO_SCENE "scene opening\nroom 1\nplate room_a 0 0 1024 384\nwait 6\nmusic song 1\nsound ping\nwait 6\nsound ping\nwait 6000\n"
#define TEXT_LINE_WIDTH 200
#define TEXT_LINE_HEIGHT 52
#define OTHER_TEXT_LINE_WIDTH 240
#define OVERLAY_ART_WIDTH 84

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

static void set_up_platform(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_audio_reset();
    fake_memory_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
}

static void set_up_textures_and_animations(void)
{
    add_texture("room_a", ROOM_A_WIDTH, 54);
    add_texture("room_b", ROOM_B_WIDTH, 54);
    add_texture("walk_page", CHARACTER_WIDTH, 32);
    add_texture("idle_page", CHARACTER_WIDTH, 32);
    add_texture("card", CARD_WIDTH, 48);
    add_text_file("assets/animations/walk.anim", "animation walk\nloops 1\nframe walk_page 6 -10 -20 30 40 0 0 0.5 1\nframe walk_page 6 -6 -20 30 40 0.5 0 0.5 1\n");
    add_text_file("assets/animations/idle.anim", "animation idle\nloops 1\nframe idle_page 6 0 0 10 10 0 0 1 1\nframe - 6 0 0 0 0 0 0 0 0\n");
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

static int count_of_text(const char *text, const char *wanted)
{
    int count;

    count = 0;
    text = strstr(text, wanted);
    while (text != NULL) {
        count = count + 1;
        text = strstr(text + 1, wanted);
    }
    return count;
}

static void run_frames(int frame_count)
{
    struct input_state no_input;
    int frame;

    no_input.pressed = 0;
    for (frame = 0; frame < frame_count; frame++) {
        fake_clock_advance_us(FRAME_US);
        viewer_app_update(&no_input, FRAME_US);
    }
}

static void run_frames_until_scene_is_no_longer_preloading(void)
{
    int frame;

    for (frame = 0; frame < 4096 && scene_player_status() == SCENE_PLAYER_PRELOADING; frame++) {
        run_frames(1);
    }
}

static void press(unsigned int button)
{
    struct input_state input;

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

static float screen_x_when_camera_is_centred(float world_x, float camera_x)
{
    return ((float)RENDERER_WIDTH - SCENE_VIEW_DS_SCREEN_WIDTH * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL) / 2.0f + (world_x - camera_x) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
}

static float screen_y_when_camera_is_centred(float world_y, float camera_y)
{
    return ((float)RENDERER_HEIGHT - SCENE_VIEW_DS_SCREEN_HEIGHT * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL) / 2.0f + (world_y - camera_y) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
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
    CHECK_INT(count_draws_with_texture_width(PLACEHOLDER_TEXTURE_WIDTH), 1);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_browsing_draws_a_small_background_centred_with_the_character_over_it(void)
{
    const struct fake_draw *background;
    const struct fake_draw *character;
    float background_width;
    float background_height;
    float left;
    float top;

    set_up_platform();
    set_up_browsing_assets();
    viewer_app_start("assets");
    CHECK(viewer_app_has_assets());
    run_frames(1);
    viewer_app_set_hud_visible(0);
    draw_frame();
    background = find_draw_with_texture_width(ROOM_A_WIDTH);
    character = find_draw_with_texture_width(CHARACTER_WIDTH);
    CHECK(background != NULL);
    CHECK(character != NULL);
    CHECK(index_of_draw_with_texture_width(ROOM_A_WIDTH) < index_of_draw_with_texture_width(CHARACTER_WIDTH));
    background_width = ROOM_A_WIDTH;
    background_height = 54.0f;
    left = ((float)RENDERER_WIDTH - background_width) / 2.0f;
    top = ((float)RENDERER_HEIGHT - background_height) / 2.0f;
    CHECK(nearly_equal(background->screen_area.x, left));
    CHECK(nearly_equal(background->screen_area.y, top));
    CHECK(nearly_equal(background->screen_area.width, ROOM_A_WIDTH));
    CHECK(nearly_equal(character->screen_area.x + character->screen_area.width / 2.0f, left + 128.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL));
    CHECK(nearly_equal(character->screen_area.y + character->screen_area.height / 2.0f, top + 96.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL));
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
    add_text_file("assets/animations/walk.anim", "frame not_converted 6 0 0 10 10 0 0 1 1\n");
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
    CHECK(find_draw_with_texture_width(ROOM_B_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_stepping_walks_past_an_animation_that_will_not_load(void)
{
    set_up_platform();
    set_up_textures_and_animations();
    add_text_file("assets/manifest.txt", "background room_a\nanimation walk\nanimation broken\nanimation idle\n");
    add_text_file("assets/animations/broken.anim", "not an animation\n");
    viewer_app_start("assets");
    press(INPUT_DOWN);
    CHECK_HAS_TEXT(log_text(), "animation invalid name=broken");
    CHECK_HAS_TEXT(log_text(), "animation selected name=idle");
    press(INPUT_UP);
    CHECK_INT(count_of_text(log_text(), "animation selected name=walk"), 2);
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
    add_text_file("assets/animations/walk.anim", "animation walk\nloops 1\nframe walk_page 6 0 0 10 10 0 0 1 1\nframe second_page 6 0 0 10 10 0 0 1 1\n");
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

static void test_choosing_another_animation_drops_the_old_ones_unread_pages(void)
{
    set_up_platform();
    set_up_browsing_assets();
    add_text_file("assets/animations/walk.anim", "animation walk\nloops 1\nframe page_a 6 0 0 10 10 0 0 1 1\nframe page_b 6 0 0 10 10 0 0 1 1\nframe page_c 6 0 0 10 10 0 0 1 1\nframe page_d 6 0 0 10 10 0 0 1 1\n");
    add_texture("page_a", 8, 8);
    add_texture("page_b", 8, 8);
    add_texture("page_c", 8, 8);
    add_texture("page_d", 8, 8);
    fake_reader_hold_reads(1);
    viewer_app_start("assets");
    CHECK_INT(texture_cache_loading_count(), 4);
    viewer_app_show_animation("idle");
    CHECK_INT(texture_cache_loading_count(), TEXTURE_CACHE_READS_AT_ONCE + 1);
    viewer_app_stop();
}

static void test_frame_whose_page_a_full_cache_turned_away_waits_instead_of_drawing_magenta(void)
{
    char name[16];
    int index;

    set_up_platform();
    set_up_browsing_assets();
    add_text_file("assets/animations/idle.anim", "animation idle\nloops 0\nframe walk_page 6 0 0 10 10 0 0 1 1\nframe page_turned_away 6 0 0 10 10 0 0 1 1\n");
    viewer_app_start("assets");
    run_frames(5);
    fake_reader_hold_reads(1);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        texture_cache_use_or_start_loading(name);
    }
    scene_stage_set_object("extra", "idle", 0.0f, 0.0f, 0, 0);
    run_frames(20);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "texture_cache full name=");
    CHECK_HAS_TEXT(log_text(), "object=extra name=idle frames_shown=1 frames_total=2");
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
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(ROOM_B_WIDTH) == NULL);
    CHECK(strstr(log_text(), "background shown name=room_b") == NULL);
    fake_reader_release_reads(4);
    run_frames(4);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_B_WIDTH) != NULL);
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
    CHECK_INT(fake_renderer_draw_count(), draws_before + STATUS_TEXT_VISIBLE_CHARACTERS);
    viewer_app_stop();
}

static void test_scene_waits_for_textures_before_running(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nobject hero walk 100 150 1 0\nwait 60\n");
    fake_reader_hold_reads(1);
    viewer_app_start("assets");
    run_frames(FRAMES_LONGER_THAN_EVERY_PRELOAD_PASS);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    CHECK(strstr(log_text(), "scene ready") == NULL);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) == NULL);
    fake_reader_release_reads(10);
    run_frames(5);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene ready name=opening");
    run_frames(1);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_scene_applies_camera_flip_and_layers(void)
{
    const struct fake_draw *hero;
    const struct fake_draw *back;
    const struct fake_draw *front;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nobject hero walk 200 150 2 1\nobject front idle 150 40 5 0\nobject back idle 160 40 1 0\ncamera 100 20\nwait 600\n");
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    back = fake_renderer_draw(1);
    hero = fake_renderer_draw(2);
    front = fake_renderer_draw(3);
    CHECK(nearly_equal(back->screen_area.x, screen_x_when_camera_is_centred(160.0f, 100.0f)));
    CHECK(nearly_equal(front->screen_area.x, screen_x_when_camera_is_centred(150.0f, 100.0f)));
    CHECK(nearly_equal(hero->screen_area.x, screen_x_when_camera_is_centred(200.0f + 10.0f - 30.0f, 100.0f)));
    CHECK(nearly_equal(hero->screen_area.y, screen_y_when_camera_is_centred(150.0f - 20.0f, 20.0f)));
    CHECK(hero->texture_area.width == -0.5f);
    CHECK(hero->texture_area.x == 0.5f);
    viewer_app_stop();
}

static int count_filled_rects(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->is_filled_rect) {
            count = count + 1;
        }
    }
    return count;
}

static const struct fake_draw *draw_plate_with_camera(const char *scene_text)
{
    set_up_platform();
    set_up_scene_assets(scene_text);
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    return find_draw_with_texture_width(ROOM_A_WIDTH);
}

static void test_a_wide_room_at_its_left_end_fills_the_screen_from_its_left_edge(void)
{
    const struct fake_draw *plate;

    plate = draw_plate_with_camera("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\nwait 600\n");
    CHECK(nearly_equal(plate->screen_area.x, 0.0f));
    CHECK(nearly_equal(plate->screen_area.y + plate->screen_area.height, RENDERER_HEIGHT));
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
}

static void test_a_wide_room_keeps_the_camera_centred_in_the_middle(void)
{
    const struct fake_draw *plate;

    plate = draw_plate_with_camera("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nwait 600\n");
    CHECK(nearly_equal(plate->screen_area.x, screen_x_when_camera_is_centred(0.0f, 344.0f)));
    CHECK(nearly_equal(plate->screen_area.y, screen_y_when_camera_is_centred(0.0f, 96.0f)));
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
}

static void test_a_wide_room_stops_at_its_right_edge(void)
{
    const struct fake_draw *plate;

    plate = draw_plate_with_camera("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 768 0\nwait 600\n");
    CHECK(nearly_equal(plate->screen_area.x + plate->screen_area.width, RENDERER_WIDTH));
    CHECK(nearly_equal(plate->screen_area.y, 0.0f));
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
}

static void test_art_narrower_than_the_screen_is_centred_with_black_on_every_side(void)
{
    const struct fake_draw *plate;
    const struct fake_draw *last;

    plate = draw_plate_with_camera("scene opening\nroom 1\nplate room_b 0 0 1024 384\nplate room_a 0 384 256 192\nobject hero walk 300 500 1 0\ncamera 0 384\nwait 600\n");
    CHECK(nearly_equal(plate->screen_area.x, 120.0f));
    CHECK(nearly_equal(plate->screen_area.y, 2.0f));
    CHECK(nearly_equal(plate->screen_area.width, 720.0f));
    CHECK(nearly_equal(plate->screen_area.height, 540.0f));
    CHECK_INT(count_filled_rects(), 4);
    last = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(last->is_filled_rect);
    CHECK(index_of_draw_with_texture_width(CHARACTER_WIDTH) < fake_renderer_draw_count() - 4);
    viewer_app_stop();
}

static void test_a_plate_just_below_the_camera_does_not_count_as_art(void)
{
    const struct fake_draw *plate;

    plate = draw_plate_with_camera("scene opening\nroom 1\nplate room_a 0 0 1024 384\nplate room_b 0 384 256 192\ncamera 0 192\nwait 600\n");
    CHECK(nearly_equal(plate->screen_area.y + plate->screen_area.height, RENDERER_HEIGHT));
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
}

static void test_a_card_is_centred_at_its_own_shape_over_black(void)
{
    const struct fake_draw *card;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncard card\nwait 600\n");
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    card = find_draw_with_texture_width(CARD_WIDTH);
    CHECK(nearly_equal(card->screen_area.x, 120.0f));
    CHECK(nearly_equal(card->screen_area.y, 2.0f));
    CHECK(nearly_equal(card->screen_area.width, SCENE_VIEW_CARD_WIDTH));
    CHECK(nearly_equal(card->screen_area.height, SCENE_VIEW_CARD_HEIGHT));
    CHECK(index_of_draw_with_texture_width(ROOM_A_WIDTH) < index_of_draw_with_texture_width(CARD_WIDTH));
    CHECK(fake_renderer_draw(index_of_draw_with_texture_width(CARD_WIDTH) - 1)->is_filled_rect);
    CHECK(nearly_equal(fake_renderer_draw(index_of_draw_with_texture_width(CARD_WIDTH) - 1)->screen_area.width, RENDERER_WIDTH));
    viewer_app_stop();
}

static void test_a_fade_covers_the_whole_screen(void)
{
    const struct fake_draw *fade;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nfade_out 6\nwait 600\n");
    viewer_app_start("assets");
    run_frames(20);
    draw_frame();
    fade = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(fade->is_filled_rect);
    CHECK(nearly_equal(fade->screen_area.x, 0.0f));
    CHECK(nearly_equal(fade->screen_area.width, RENDERER_WIDTH));
    CHECK(nearly_equal(fade->screen_area.height, RENDERER_HEIGHT));
    CHECK(fade->color.alpha == 1.0f);
    viewer_app_stop();
}

static const struct fake_draw *find_mask_draw(void)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->batched_rect_count > 0) {
            return fake_renderer_draw(index);
        }
    }
    return NULL;
}

static int index_of_mask_draw(void)
{
    const struct fake_draw *mask;

    mask = find_mask_draw();
    return mask == NULL ? -1 : (int)(mask - fake_renderer_draw(0));
}

static int mask_blackens_pixel(const struct fake_draw *mask, float x, float y)
{
    const struct renderer_rect *rects;
    int index;

    rects = fake_renderer_batched_rects(mask);
    for (index = 0; index < mask->batched_rect_count; index++) {
        if (x >= rects[index].x && x < rects[index].x + rects[index].width && y >= rects[index].y && y < rects[index].y + rects[index].height) {
            return 1;
        }
    }
    return 0;
}

static const struct fake_draw *play_scene_and_draw_after(const char *scene_text, int frame_count)
{
    set_up_platform();
    set_up_scene_assets(scene_text);
    viewer_app_start("assets");
    run_frames(frame_count);
    draw_frame();
    return find_mask_draw();
}

static void test_a_mask_with_no_beams_blackens_the_whole_screen_over_the_room(void)
{
    const struct fake_draw *mask;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nobject hero walk 100 150 1 0\nmask_on\nwait 600\n", 8);
    CHECK(mask != NULL);
    CHECK_INT(mask->batched_rect_count, 1);
    CHECK(mask->color.alpha == 1.0f);
    CHECK(mask->color.red == 0.0f);
    CHECK(fake_renderer_batched_rects(mask)[0].width == RENDERER_WIDTH);
    CHECK(fake_renderer_batched_rects(mask)[0].height == RENDERER_HEIGHT);
    CHECK(index_of_draw_with_texture_width(ROOM_A_WIDTH) < index_of_mask_draw());
    CHECK(index_of_draw_with_texture_width(CHARACTER_WIDTH) < index_of_mask_draw());
    CHECK(strstr(log_text(), "scene mask_rects") == NULL);
    viewer_app_stop();
}

static void test_a_deleted_beam_lights_nothing(void)
{
    const struct fake_draw *mask;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nmask_on\nbeam_create b 400 0 400 200 40\nbeam_delete b\nwait 600\n", 8);
    CHECK(mask != NULL);
    CHECK_INT(mask->batched_rect_count, 1);
    viewer_app_stop();
}

static void test_a_beam_passes_through_the_same_world_to_screen_view_as_the_art(void)
{
    const struct fake_draw *mask;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nmask_on\nbeam_create b 400 0 400 200 40\nwait 600\n", 8);
    CHECK(mask != NULL);
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f, 344.0f), screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f - 38.0f, 344.0f), screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f - 42.0f, 344.0f), screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f + 38.0f, 344.0f), screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f + 42.0f, 344.0f), screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f, 344.0f), screen_y_when_camera_is_centred(201.0f, 96.0f)));
    CHECK(mask_blackens_pixel(mask, 0.0f, screen_y_when_camera_is_centred(199.0f, 96.0f)));
    CHECK(mask_blackens_pixel(mask, RENDERER_WIDTH - 1.0f, screen_y_when_camera_is_centred(199.0f, 96.0f)));
    viewer_app_stop();
}

static void test_a_masked_room_stays_centred_on_the_camera_even_at_its_left_end(void)
{
    const struct fake_draw *mask;
    const struct fake_draw *plate;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\nmask_on\nbeam_create b 56 128 56 367 30\nwait 600\n", 8);
    plate = find_draw_with_texture_width(ROOM_A_WIDTH);
    CHECK(mask != NULL);
    CHECK(nearly_equal(plate->screen_area.x, screen_x_when_camera_is_centred(0.0f, 0.0f)));
    CHECK(nearly_equal(plate->screen_area.y, screen_y_when_camera_is_centred(0.0f, 192.0f)));
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(56.0f, 0.0f), screen_y_when_camera_is_centred(366.0f, 192.0f)));
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(56.0f, 0.0f), screen_y_when_camera_is_centred(368.0f, 192.0f)));
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(90.0f, 0.0f), screen_y_when_camera_is_centred(366.0f, 192.0f)));
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(80.0f, 0.0f), screen_y_when_camera_is_centred(366.0f, 192.0f)));
    viewer_app_stop();
}

static void check_beam_sits_in_the_middle_of_the_screen(const struct fake_draw *mask, float row)
{
    CHECK(!mask_blackens_pixel(mask, RENDERER_WIDTH / 2.0f, row));
    CHECK(mask_blackens_pixel(mask, RENDERER_WIDTH / 2.0f - 32.0f * 2.8125f, row));
    CHECK(mask_blackens_pixel(mask, RENDERER_WIDTH / 2.0f + 32.0f * 2.8125f, row));
}

static void test_a_beam_keeps_its_place_on_screen_across_a_cut_to_narrower_art(void)
{
    const struct fake_draw *mask;
    float row_near_the_base;

    row_near_the_base = screen_y_when_camera_is_centred(383.0f, 192.0f);
    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nplate room_b 0 384 256 192\ncamera 0 192\nmask_on\nbeam_create b 128 192 128 384 30\nwait 30\ncamera 0 384\nbeam_create b 128 384 128 576 30\nwait 6000\n", 8);
    CHECK(mask != NULL);
    check_beam_sits_in_the_middle_of_the_screen(mask, row_near_the_base);
    run_frames(60);
    draw_frame();
    mask = find_mask_draw();
    CHECK(mask != NULL);
    check_beam_sits_in_the_middle_of_the_screen(mask, row_near_the_base);
    viewer_app_stop();
}

static void test_an_iris_uses_the_same_view_as_the_lit_room_it_opens_onto(void)
{
    const struct fake_draw *iris;
    const struct fake_draw *plate;

    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\nmask_on\niris 144 359 100 0\nwait 600\n", 8);
    plate = find_draw_with_texture_width(ROOM_A_WIDTH);
    iris = find_draw_with_texture_width(IRIS_TEXTURE_WIDTH);
    CHECK(nearly_equal(plate->screen_area.x, 0.0f));
    CHECK(nearly_equal(plate->screen_area.y + plate->screen_area.height, RENDERER_HEIGHT));
    CHECK(iris != NULL);
    CHECK(nearly_equal(iris->screen_area.x + iris->screen_area.width / 2.0f, 144.0f * 2.8125f));
    viewer_app_stop();
}

static void test_the_mask_covers_the_black_around_narrow_art_and_is_drawn_after_it(void)
{
    const struct fake_draw *mask;
    int plate_index;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_b 0 0 1024 384\nplate room_a 0 384 256 192\ncamera 0 384\nmask_on\nbeam_create b 128 384 128 500 30\nwait 600\n", 8);
    plate_index = index_of_draw_with_texture_width(ROOM_A_WIDTH);
    CHECK(mask != NULL);
    CHECK_INT(count_filled_rects(), 4);
    CHECK(fake_renderer_draw(index_of_mask_draw() - 1)->is_filled_rect);
    CHECK(plate_index < index_of_mask_draw() - 4);
    CHECK(mask_blackens_pixel(mask, 5.0f, 300.0f));
    CHECK(mask_blackens_pixel(mask, RENDERER_WIDTH - 5.0f, 300.0f));
    viewer_app_stop();
}

static void test_a_beam_swings_while_the_script_waits(void)
{
    const struct fake_draw *mask;
    float world_to_screen_y;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nmask_on\nbeam_create b 400 0 400 200 20\nbeam_animate b 500 200 20 60\nwait 6000\n", 4);
    world_to_screen_y = screen_y_when_camera_is_centred(199.0f, 96.0f);
    CHECK(mask != NULL);
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f, 344.0f), world_to_screen_y));
    run_frames(60);
    draw_frame();
    mask = find_mask_draw();
    CHECK(mask_blackens_pixel(mask, screen_x_when_camera_is_centred(400.0f, 344.0f), world_to_screen_y));
    CHECK(!mask_blackens_pixel(mask, screen_x_when_camera_is_centred(500.0f, 344.0f), world_to_screen_y));
    CHECK_INT(scene_player_next_step(), 7);
    viewer_app_stop();
}

static int count_rects_in_mask_draw(void)
{
    return find_mask_draw() == NULL ? 0 : find_mask_draw()->batched_rect_count;
}

static int count_times_in_log(const char *wanted)
{
    const char *found;
    int count;

    count = 0;
    found = strstr(log_text(), wanted);
    while (found != NULL) {
        count = count + 1;
        found = strstr(found + 1, wanted);
    }
    return count;
}

static void test_a_large_mask_is_logged_and_a_normal_one_is_not(void)
{
    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nmask_on\nbeam_create b 400 0 400 200 40\nwait 600\n", 8);
    CHECK(strstr(log_text(), "scene mask_rects") == NULL);
    viewer_app_stop();
    play_scene_and_draw_after("scene opening\nmask_on\nbeam_create a 0 0 800 544 20\nbeam_create b 300 0 1100 544 20\nbeam_create c 600 0 1400 544 20\nbeam_create d 900 0 1700 544 20\nwait 600\n", 8);
    CHECK(count_rects_in_mask_draw() > 600);
    draw_frame();
    draw_frame();
    CHECK_INT(count_times_in_log("scene mask_rects count="), 1);
    viewer_app_stop();
}

static const struct fake_draw *find_iris_draw(void)
{
    return find_draw_with_texture_width(IRIS_TEXTURE_WIDTH);
}

static void test_the_iris_draws_a_soft_circle_with_black_around_it(void)
{
    const struct fake_draw *iris;
    const struct fake_draw *around;
    float center_x;
    float center_y;
    float radius;
    float covered;
    int index;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\nmask_on\niris 400 150 100 120\nwait 6000\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(1);
    draw_frame();
    run_frames(60);
    draw_frame();
    iris = find_iris_draw();
    CHECK(iris != NULL);
    center_x = screen_x_when_camera_is_centred(400.0f, 344.0f);
    center_y = screen_y_when_camera_is_centred(150.0f, 96.0f);
    radius = iris->screen_area.width / 2.0f;
    CHECK(radius > 49.0f * 2.8125f && radius < 51.0f * 2.8125f);
    CHECK(nearly_equal(iris->screen_area.width, iris->screen_area.height));
    CHECK(nearly_equal(iris->screen_area.x + radius, center_x));
    CHECK(nearly_equal(iris->screen_area.y + radius, center_y));
    CHECK(iris->color.alpha == 1.0f);
    around = find_mask_draw();
    CHECK(around != NULL);
    CHECK(around->color.red == 0.0f && around->color.green == 0.0f && around->color.blue == 0.0f && around->color.alpha == 1.0f);
    CHECK(index_of_draw_with_texture_width(ROOM_A_WIDTH) < index_of_draw_with_texture_width(IRIS_TEXTURE_WIDTH));
    CHECK(index_of_draw_with_texture_width(IRIS_TEXTURE_WIDTH) < index_of_mask_draw());
    covered = 0.0f;
    for (index = 0; index < around->batched_rect_count; index++) {
        covered = covered + fake_renderer_batched_rects(around)[index].width * fake_renderer_batched_rects(around)[index].height;
    }
    CHECK(nearly_equal(covered + iris->screen_area.width * iris->screen_area.height, (float)RENDERER_WIDTH * RENDERER_HEIGHT));
    viewer_app_stop();
}

static void test_a_closed_iris_leaves_the_screen_black_and_an_open_one_draws_nothing(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\niris 400 150 1000 600\nwait 6000\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(1);
    draw_frame();
    CHECK(find_mask_draw() != NULL);
    run_frames(240);
    draw_frame();
    CHECK(find_iris_draw() != NULL);
    run_frames(300);
    draw_frame();
    CHECK(find_mask_draw() == NULL);
    CHECK(find_iris_draw() == NULL);
    viewer_app_stop();
}

static int iris_at_screen_corner_is_drawn(float corner_x, float corner_y, float radius)
{
    char scene_text[256];
    int is_drawn;

    snprintf(scene_text, sizeof(scene_text), "scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 344 96\niris %f %f %f 0\nwait 6000\n",
        344.0f + (corner_x - screen_x_when_camera_is_centred(344.0f, 344.0f)) / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL,
        96.0f + (corner_y - screen_y_when_camera_is_centred(96.0f, 96.0f)) / SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL,
        radius);
    set_up_platform();
    set_up_scene_assets(scene_text);
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(1);
    draw_frame();
    is_drawn = find_iris_draw() != NULL;
    viewer_app_stop();
    return is_drawn;
}

static void test_an_iris_only_clears_once_its_clear_part_reaches_the_farthest_corner(void)
{
    static const float corners[4][2] = { { 0.0f, 0.0f }, { RENDERER_WIDTH, 0.0f }, { 0.0f, RENDERER_HEIGHT }, { RENDERER_WIDTH, RENDERER_HEIGHT } };
    int index;

    for (index = 0; index < 4; index++) {
        CHECK(iris_at_screen_corner_is_drawn(corners[index][0], corners[index][1], 775.0f));
        CHECK(!iris_at_screen_corner_is_drawn(corners[index][0], corners[index][1], 790.0f));
    }
}

static void test_mask_off_after_an_iris_shows_the_room(void)
{
    const struct fake_draw *mask;

    mask = play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nmask_on\niris 100 100 10 1\nwait 12\nmask_off\nwait 600\n", 30);
    CHECK(mask == NULL);
    CHECK(find_iris_draw() == NULL);
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_dim_flash_card_and_fade_draw_in_that_order_over_the_mask(void)
{
    const struct fake_draw *dim;
    const struct fake_draw *flash;
    const struct fake_draw *last;
    int mask_index;
    int card_index;

    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nmask_on\ndim_to 0.25 0\nflash 0.5 6000\ncard card\nfade_out 6\nwait 6000\n", 20);
    mask_index = index_of_mask_draw();
    card_index = index_of_draw_with_texture_width(CARD_WIDTH);
    CHECK(mask_index >= 0);
    dim = fake_renderer_draw(mask_index + 1);
    flash = fake_renderer_draw(mask_index + 2);
    last = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(dim->is_filled_rect);
    CHECK(dim->color.red == 0.0f);
    CHECK(nearly_equal(dim->color.alpha, 0.75f));
    CHECK(nearly_equal(dim->screen_area.width, RENDERER_WIDTH));
    CHECK(nearly_equal(dim->screen_area.height, RENDERER_HEIGHT));
    CHECK(flash->is_filled_rect);
    CHECK(flash->color.red == 1.0f);
    CHECK(flash->color.green == 1.0f);
    CHECK(flash->color.alpha > 0.49f && flash->color.alpha <= 0.5f);
    CHECK(nearly_equal(flash->screen_area.width, RENDERER_WIDTH));
    CHECK(mask_index + 2 < card_index);
    CHECK(card_index < fake_renderer_draw_count() - 1);
    CHECK(last->is_filled_rect);
    CHECK(last->color.red == 0.0f);
    CHECK(last->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_no_dim_or_flash_draws_at_full_brightness(void)
{
    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nwait 6000\n", 8);
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ndim_to 0.5 0\ndim_to 1 0\nflash 1 6\nwait 6000\n", 20);
    CHECK_INT(count_filled_rects(), 0);
    viewer_app_stop();
}

static void test_a_dim_and_a_flash_change_while_the_script_waits(void)
{
    const struct scene_effects *effects;

    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\ndim_to 0.5 60\nflash 1 60\nwait 6000\n", 4);
    effects = &scene_stage_view()->effects;
    run_frames(30);
    CHECK(effects->brightness.value < 0.8f && effects->brightness.value > 0.7f);
    CHECK(effects->flash_level.value < 0.55f && effects->flash_level.value > 0.45f);
    CHECK_INT(scene_player_next_step(), 5);
    run_frames(40);
    CHECK(effects->brightness.value == 0.5f);
    CHECK(effects->flash_level.value == 0.0f);
    viewer_app_stop();
}

static void test_browsing_after_a_masked_scene_shows_no_mask(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nmask_on\ndim_to 0.5 0\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    CHECK(find_mask_draw() != NULL);
    CHECK(viewer_app_show_animation("walk"));
    run_frames(2);
    viewer_app_set_hud_visible(0);
    draw_frame();
    CHECK(find_mask_draw() == NULL);
    CHECK_INT(count_filled_rects(), 4);
    viewer_app_stop();
}

static void test_watching_it_again_starts_with_no_mask_left_over(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nwait 6\nmask_on\ndim_to 0.5 0\n");
    viewer_app_start("assets");
    run_frames(20);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(find_mask_draw() != NULL);
    press(INPUT_CROSS);
    run_frames(4);
    draw_frame();
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK(find_mask_draw() == NULL);
    CHECK(scene_stage_view()->effects.brightness.value == 1.0f);
    viewer_app_stop();
}

static void test_the_end_prompt_stays_readable_over_a_black_mask(void)
{
    const struct fake_draw *last;

    play_scene_and_draw_after("scene opening\nmask_on\nwait 6\n", 20);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    last = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(last->texture != RENDERER_NO_TEXTURE);
    CHECK(index_of_mask_draw() < fake_renderer_draw_count() - 1);
    viewer_app_stop();
}

static void test_the_iris_pattern_is_made_once_and_freed_at_the_end(void)
{
    int created_before;

    set_up_platform();
    set_up_scene_assets("scene opening\nwait 6\n");
    viewer_app_start("assets");
    created_before = fake_renderer_textures_created();
    run_frames(30);
    CHECK_INT(fake_renderer_textures_created(), created_before);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_making_the_iris_pattern_keeps_no_memory_once_it_is_a_texture(void)
{
    int blocks_before;

    set_up_platform();
    blocks_before = fake_memory_blocks_in_use();
    scene_view_start();
    CHECK_INT(fake_memory_blocks_in_use(), blocks_before);
    scene_view_stop();
}

static int count_sprites_with_no_texture(void)
{
    const struct fake_draw *draw;
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (draw->texture == RENDERER_NO_TEXTURE && !draw->is_filled_rect && !draw->is_rect_batch) {
            count = count + 1;
        }
    }
    return count;
}

static void test_a_failed_iris_pattern_is_logged_and_the_iris_stays_black_around_its_circle(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\niris 100 100 50 0\nwait 6000\n");
    fake_renderer_fail_uploads(1);
    viewer_app_start("assets");
    CHECK_HAS_TEXT(log_text(), "scene iris_texture_failed");
    fake_renderer_fail_uploads(0);
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(1);
    draw_frame();
    CHECK(find_mask_draw() != NULL);
    CHECK(find_iris_draw() == NULL);
    CHECK_INT(count_sprites_with_no_texture(), 0);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_an_app_that_never_played_a_scene_draws_at_full_brightness(void)
{
    set_up_platform();
    set_up_textures_and_animations();
    add_text_file("assets/manifest.txt", "background room_a\n");
    viewer_app_start("assets");
    run_frames(2);
    viewer_app_set_hud_visible(0);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK_INT(count_filled_rects(), 4);
    CHECK(find_mask_draw() == NULL);
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
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) == NULL);
    run_frames(8);
    draw_frame();
    CHECK(find_draw_with_texture_width(CARD_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(ROOM_B_WIDTH) == NULL);
    CHECK_HAS_TEXT(log_text(), "scene room id=1");
    CHECK(strstr(log_text(), "scene room id=2") == NULL);
    run_frames(8);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(ROOM_B_WIDTH) != NULL);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK_HAS_TEXT(log_text(), "scene finished name=opening");
    viewer_app_stop();
}

static void test_scene_fades_and_pans_over_time(void)
{
    const struct scene_stage *stage;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nfade_out 60\ncamera_pan 100 0 60\nfade_in 60\nwait 60\n");
    viewer_app_start("assets");
    run_frames(4);
    stage = scene_stage_view();
    run_frames(30);
    CHECK(stage->fade_amount > 0.4f && stage->fade_amount < 0.6f);
    run_frames(32);
    CHECK(stage->fade_amount == 1.0f);
    CHECK(stage->camera_x < 10.0f);
    run_frames(30);
    CHECK(stage->camera_x > 40.0f && stage->camera_x < 60.0f);
    run_frames(32);
    CHECK(stage->camera_x == 100.0f);
    CHECK(stage->fade_amount < 1.0f);
    run_frames(62);
    CHECK(stage->fade_amount == 0.0f);
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
    press(INPUT_SELECT);
    press(INPUT_L);
    run_frames(4);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    viewer_app_stop();
}

static void test_browsing_centres_an_animation_on_its_first_drawn_frame_wherever_the_camera_is(void)
{
    const struct fake_draw *character;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 200 100\nwait 6000\n");
    add_text_file("assets/animations/offset.anim", "animation offset\nloops 0\nframe - 6 0 0 0 0 0 0 0 0\nframe walk_page 6 -300 -400 30 40 0 0 1 1\n");
    add_text_file("assets/manifest.txt", "scene opening\nanimation offset\n");
    viewer_app_start("assets");
    run_frames(4);
    CHECK(viewer_app_show_animation("offset"));
    run_frames(8);
    draw_frame();
    character = find_draw_with_texture_width(CHARACTER_WIDTH);
    CHECK(character != NULL);
    CHECK(nearly_equal(character->screen_area.x + character->screen_area.width / 2.0f, screen_x_when_camera_is_centred(328.0f, 200.0f)));
    CHECK(nearly_equal(character->screen_area.y + character->screen_area.height / 2.0f, screen_y_when_camera_is_centred(196.0f, 100.0f)));
    viewer_app_stop();
}

static void test_cross_skips_the_current_hold_instead_of_pausing(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nwait 6000\nwait 6000\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    CHECK_INT(scene_player_next_step(), 1);
    press(INPUT_CROSS);
    CHECK(!scene_player_is_paused());
    CHECK_HAS_TEXT(log_text(), "scene hold_skipped next_step=1");
    run_frames(1);
    CHECK_INT(scene_player_next_step(), 2);
    press(INPUT_CROSS);
    run_frames(1);
    CHECK_INT(scene_player_next_step(), 3);
    viewer_app_stop();
}

static void test_skipping_a_fade_or_pan_finishes_it_at_once(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nfade_out 6000\ncamera_pan 200 0 6000\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_CROSS);
    run_frames(1);
    CHECK(scene_stage_view()->fade_amount == 1.0f);
    press(INPUT_CROSS);
    run_frames(1);
    CHECK(scene_stage_view()->camera_x == 200.0f);
    viewer_app_stop();
}

static void test_buttons_a_player_might_press_never_stall_the_scene(void)
{
    static const unsigned int buttons[] = { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_CIRCLE, INPUT_TRIANGLE, INPUT_L, INPUT_R };
    unsigned int index;

    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    for (index = 0; index < sizeof(buttons) / sizeof(buttons[0]); index++) {
        press(buttons[index]);
        CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
        CHECK(!scene_player_is_paused());
    }
    run_frames(20);
    CHECK_INT(scene_player_next_step(), 3);
    draw_frame();
    CHECK(find_draw_with_texture_width(CARD_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_the_end_of_a_scene_says_so_and_cross_plays_it_again(void)
{
    int draws_before;
    int draws_at_the_end;

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nwait 30\nfade_out 6\n");
    viewer_app_start("assets");
    run_frames(8);
    draw_frame();
    draws_before = fake_renderer_draw_count();
    run_frames(60);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    draws_at_the_end = fake_renderer_draw_count();
    CHECK_INT(draws_at_the_end, draws_before + 1 + SCENE_END_PROMPT_VISIBLE_CHARACTERS);
    press(INPUT_CROSS);
    CHECK(scene_player_status() != SCENE_PLAYER_FINISHED);
    CHECK_INT(scene_player_next_step(), 0);
    run_frames(4);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK(scene_stage_view()->fade_amount == 0.0f);
    viewer_app_stop();
}

static void test_select_turns_the_debug_controls_on_and_off(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_SELECT);
    CHECK_HAS_TEXT(log_text(), "debug controls on");
    press(INPUT_CROSS);
    CHECK(scene_player_is_paused());
    CHECK_HAS_TEXT(log_text(), "scene paused next_step=");
    press(INPUT_SELECT);
    CHECK_HAS_TEXT(log_text(), "debug controls off");
    CHECK(!scene_player_is_paused());
    CHECK_HAS_TEXT(log_text(), "scene resumed next_step=");
    viewer_app_stop();
}

static void test_leaving_debug_controls_after_browsing_plays_the_scene_again(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_SELECT);
    press(INPUT_DOWN);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_IDLE);
    press(INPUT_SELECT);
    CHECK(scene_player_status() != SCENE_PLAYER_IDLE);
    CHECK_INT(scene_player_next_step(), 0);
    viewer_app_stop();
}

static void test_a_list_left_open_closes_when_debug_controls_go_off(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_SELECT);
    press(INPUT_R);
    press(INPUT_SELECT);
    press(INPUT_CROSS);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene hold_skipped");
    viewer_app_stop();
}

static void test_every_button_press_is_logged(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_CROSS);
    CHECK_HAS_TEXT(log_text(), "input pressed buttons=0x0010");
    viewer_app_stop();
}

static void update_with_elapsed_time(unsigned long long elapsed_us)
{
    struct input_state no_input;

    no_input.pressed = 0;
    fake_clock_advance_us(elapsed_us);
    viewer_app_update(&no_input, elapsed_us);
}

static void set_up_text_scene(const char *scene_text)
{
    set_up_platform();
    set_up_scene_assets(scene_text);
    add_texture("line_a", TEXT_LINE_WIDTH, TEXT_LINE_HEIGHT);
    add_texture("line_b", OTHER_TEXT_LINE_WIDTH, TEXT_LINE_HEIGHT);
    add_texture("art", OVERLAY_ART_WIDTH, 60);
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(1);
}

static void test_text_is_drawn_centred_at_its_top_with_its_fade_as_alpha(void)
{
    const struct fake_draw *line;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 60\nwait 6000\n");
    run_frames(29);
    draw_frame();
    line = find_draw_with_texture_width(TEXT_LINE_WIDTH);
    CHECK(line != NULL);
    CHECK(nearly_equal(line->screen_area.x, 480.0f - TEXT_LINE_WIDTH / 2.0f));
    CHECK(nearly_equal(line->screen_area.y, 72.0f));
    CHECK(nearly_equal(line->screen_area.width, TEXT_LINE_WIDTH));
    CHECK(nearly_equal(line->screen_area.height, TEXT_LINE_HEIGHT));
    CHECK(line->texture_area.x == 0.0f);
    CHECK(line->texture_area.width == 1.0f);
    CHECK(line->texture_area.height == 1.0f);
    CHECK(line->color.red == 1.0f);
    CHECK(line->color.alpha > 0.45f && line->color.alpha < 0.55f);
    run_frames(40);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH)->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_text_does_not_hold_the_script(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 600\ntext_hide 1 600\noverlay 1 art 10 20 30 40 600\noverlay_hide 1\ntext_hide_all\nwait 6000\n");
    CHECK_INT(scene_player_next_step(), 6);
    viewer_app_stop();
}

static void test_a_line_starts_invisible_and_is_not_drawn_until_it_has_alpha(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ntext_show 1 line_a 480 72 60\nwait 6000\n");
    add_texture("line_a", TEXT_LINE_WIDTH, TEXT_LINE_HEIGHT);
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    update_with_elapsed_time(0);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_a_hidden_line_fades_and_is_then_gone(void)
{
    const struct fake_draw *line;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6\ntext_hide 1 60\nwait 6000\n");
    run_frames(4);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH)->color.alpha == 1.0f);
    run_frames(32);
    draw_frame();
    line = find_draw_with_texture_width(TEXT_LINE_WIDTH);
    CHECK(line != NULL);
    CHECK(line->color.alpha > 0.4f && line->color.alpha < 0.6f);
    run_frames(32);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_hiding_all_text_cuts_every_line_in_one_frame(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\ntext_show 2 line_b 480 122 0\nwait 6\ntext_hide_all\nwait 6000\n");
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(OTHER_TEXT_LINE_WIDTH) != NULL);
    run_frames(7);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(OTHER_TEXT_LINE_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_replaying_a_scene_clears_its_text_and_overlays(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\noverlay 1 art 0 0 30 20 0\nwait 6000\n");
    press(INPUT_SELECT);
    press(INPUT_L);
    CHECK_INT(scene_player_next_step(), 0);
    CHECK(!scene_stage_view()->layers.text_lines[0].in_use);
    CHECK(!scene_stage_view()->layers.overlays[0].in_use);
    viewer_app_stop();
}

static void test_leaving_a_scene_for_browsing_clears_its_text_and_overlays(void)
{
    set_up_text_scene("scene opening\nroom 1\nplate room_a 0 0 1024 384\ntext_show 1 line_a 480 72 0\noverlay 1 art 0 0 30 20 0\nwait 6000\n");
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) != NULL);
    CHECK(viewer_app_show_animation("walk"));
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_skipping_a_hold_with_cross_strands_no_text(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 60\nwait 6000\ntext_hide 1 60\nwait 6000\n");
    run_frames(10);
    press(INPUT_CROSS);
    run_frames(1);
    CHECK_INT(scene_player_next_step(), 4);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    run_frames(62);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_skipping_to_the_end_of_a_scene_still_finishes_the_last_fade(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6000\ntext_hide 1 60\n");
    press(INPUT_CROSS);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    run_frames(62);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_pausing_stops_text_fades(void)
{
    float alpha_when_paused;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 60\nwait 6000\n");
    run_frames(20);
    press(INPUT_SELECT);
    press(INPUT_CROSS);
    viewer_app_set_hud_visible(0);
    draw_frame();
    alpha_when_paused = find_draw_with_texture_width(TEXT_LINE_WIDTH)->color.alpha;
    run_frames(20);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH)->color.alpha == alpha_when_paused);
    viewer_app_stop();
}

static void test_text_textures_are_loaded_before_the_scene_starts(void)
{
    set_up_text_scene("scene opening\nwait 6000\ntext_show 1 line_a 480 72 60\noverlay 1 art 0 0 30 20 6\n");
    CHECK_INT(scene_player_next_step(), 1);
    CHECK_HAS_TEXT(log_text(), "texture loaded name=line_a");
    CHECK_HAS_TEXT(log_text(), "texture loaded name=art");
    viewer_app_stop();
}

static void test_a_missing_text_texture_is_marked_visibly(void)
{
    const struct fake_draw *draw;

    set_up_text_scene("scene opening\ntext_show 1 not_converted 480 72 0\nwait 6000\n");
    run_frames(1);
    CHECK_HAS_TEXT(log_text(), "texture missing name=not_converted");
    draw_frame();
    draw = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 1.0f && draw->color.green == 0.0f && draw->color.blue == 1.0f);
    CHECK(nearly_equal(draw->screen_area.y, 72.0f));
    CHECK(nearly_equal(draw->screen_area.x + draw->screen_area.width / 2.0f, 480.0f));
    viewer_app_stop();
}

static int count_sprite_draws(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (!fake_renderer_draw(index)->is_filled_rect) {
            count = count + 1;
        }
    }
    return count;
}

static void test_a_line_hidden_with_no_fade_is_gone_from_that_frame(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6\ntext_hide 1 0\nwait 6000\n");
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    run_frames(7);
    draw_frame();
    CHECK_INT(count_sprite_draws(), 0);
    viewer_app_stop();
}

static void test_a_text_texture_still_loading_at_draw_time_draws_nothing_until_it_arrives(void)
{
    const struct fake_draw *line;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6000\n");
    texture_cache_clear();
    fake_reader_hold_reads(1);
    draw_frame();
    CHECK_INT(count_sprite_draws(), 0);
    CHECK_INT(fake_reader_pending_count(), 1);
    fake_reader_release_reads(1);
    run_frames(1);
    draw_frame();
    line = find_draw_with_texture_width(TEXT_LINE_WIDTH);
    CHECK(line != NULL);
    CHECK(line->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_a_text_texture_the_read_queue_refuses_at_draw_time_is_marked_visibly(void)
{
    const struct fake_draw *draw;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6000\n");
    texture_cache_clear();
    fake_reader_refuse_submits(1);
    draw_frame();
    CHECK_HAS_TEXT(log_text(), "texture missing name=line_a reason=read_refused");
    draw = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 1.0f && draw->color.green == 0.0f && draw->color.blue == 1.0f);
    CHECK(nearly_equal(draw->screen_area.y, 72.0f));
    CHECK(nearly_equal(draw->screen_area.x + draw->screen_area.width / 2.0f, 480.0f));
    fake_reader_refuse_submits(0);
    viewer_app_stop();
}

static void test_a_text_texture_the_full_cache_cannot_hold_is_marked_visibly(void)
{
    const struct fake_draw *draw;
    char name[ASSET_NAME_MAX];
    int index;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6000\n");
    texture_cache_clear();
    fake_reader_hold_reads(1);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(name, sizeof(name), "filler_%d", index);
        texture_cache_use_or_start_loading(name);
    }
    draw_frame();
    CHECK_HAS_TEXT(log_text(), "texture_cache full name=line_a");
    draw = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 1.0f && draw->color.green == 0.0f && draw->color.blue == 1.0f);
    CHECK(nearly_equal(draw->screen_area.y, 72.0f));
    fake_reader_hold_reads(0);
    fake_reader_release_reads(fake_reader_pending_count());
    viewer_app_stop();
}

static void test_the_opening_bubble_and_its_text_land_on_whole_screen_pixels(void)
{
    const struct fake_draw *bubble;
    const struct fake_draw *text;

    set_up_text_scene("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\noverlay 3 art 98.8444 242.8444 30 20 0\noverlay 4 line_a 113.7778 251.0222 71.1111 18.4889 0\nwait 6000\n");
    run_frames(1);
    draw_frame();
    bubble = find_draw_with_texture_width(OVERLAY_ART_WIDTH);
    text = find_draw_with_texture_width(TEXT_LINE_WIDTH);
    CHECK(nearly_equal(bubble->screen_area.x, 278.0f));
    CHECK(nearly_equal(bubble->screen_area.y, 147.0f));
    CHECK(nearly_equal(text->screen_area.x, 320.0f));
    CHECK(nearly_equal(text->screen_area.y, 170.0f));
    CHECK(nearly_equal(text->screen_area.width, TEXT_LINE_WIDTH));
    viewer_app_stop();
}

static void test_an_overlay_goes_through_the_view_and_grows_about_its_centre(void)
{
    const struct fake_draw *overlay;
    float center_x;
    float center_y;
    float full_width;
    float full_height;

    set_up_text_scene("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\noverlay 1 art 100 240 40 20 60\nwait 6000\n");
    full_width = 40.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    full_height = 20.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    center_x = 120.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    center_y = RENDERER_HEIGHT - (384.0f - 250.0f) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    run_frames(29);
    draw_frame();
    overlay = find_draw_with_texture_width(OVERLAY_ART_WIDTH);
    CHECK(overlay != NULL);
    CHECK(overlay->screen_area.width > full_width * 0.45f && overlay->screen_area.width < full_width * 0.55f);
    CHECK(overlay->screen_area.height > full_height * 0.45f && overlay->screen_area.height < full_height * 0.55f);
    CHECK(nearly_equal(overlay->screen_area.x + overlay->screen_area.width / 2.0f, center_x));
    CHECK(nearly_equal(overlay->screen_area.y + overlay->screen_area.height / 2.0f, center_y));
    run_frames(40);
    draw_frame();
    overlay = find_draw_with_texture_width(OVERLAY_ART_WIDTH);
    CHECK(nearly_equal(overlay->screen_area.x, 100.0f * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL));
    CHECK(nearly_equal(overlay->screen_area.width, full_width));
    CHECK(nearly_equal(overlay->screen_area.height, full_height));
    CHECK(overlay->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_a_new_room_removes_overlays_but_keeps_text(void)
{
    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\noverlay 1 art 0 0 30 20 0\nwait 6\nroom 2\nwait 6000\n");
    draw_frame();
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) != NULL);
    run_frames(7);
    draw_frame();
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) == NULL);
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_hiding_an_overlay_step_removes_it_at_once(void)
{
    set_up_text_scene("scene opening\noverlay 1 art 0 0 30 20 0\nwait 6\noverlay_hide 1\nwait 6000\n");
    draw_frame();
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) != NULL);
    run_frames(7);
    draw_frame();
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) == NULL);
    viewer_app_stop();
}

static void test_the_room_draws_first_then_bubbles_then_text_then_the_fade(void)
{
    const struct fake_draw *last;

    set_up_text_scene("scene opening\nroom 1\nplate room_a 0 0 1024 384\noverlay 1 art 0 0 30 20 0\ntext_show 1 line_a 480 72 0\nfade_out 6\nwait 6000\n");
    run_frames(8);
    draw_frame();
    CHECK(index_of_draw_with_texture_width(ROOM_A_WIDTH) < index_of_draw_with_texture_width(OVERLAY_ART_WIDTH));
    CHECK(index_of_draw_with_texture_width(OVERLAY_ART_WIDTH) < index_of_draw_with_texture_width(TEXT_LINE_WIDTH));
    CHECK_INT(index_of_draw_with_texture_width(TEXT_LINE_WIDTH), fake_renderer_draw_count() - 2);
    last = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(last->is_filled_rect);
    CHECK(last->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_bubbles_sit_under_the_dim_and_text_over_it_and_under_the_flash(void)
{
    const struct fake_draw *dim;
    const struct fake_draw *flash;
    int mask_index;
    int overlay_index;
    int text_index;
    int card_index;

    play_scene_and_draw_after("scene opening\nroom 1\nplate room_a 0 0 1024 384\nmask_on\noverlay 1 room_b 0 0 30 20 0\ntext_show 1 walk_page 480 72 0\ndim_to 0.25 0\nflash 0.5 6000\ncard card\nfade_out 6\nwait 6000\n", 20);
    mask_index = index_of_mask_draw();
    overlay_index = index_of_draw_with_texture_width(ROOM_B_WIDTH);
    text_index = index_of_draw_with_texture_width(CHARACTER_WIDTH);
    card_index = index_of_draw_with_texture_width(CARD_WIDTH);
    CHECK(mask_index >= 0);
    CHECK_INT(overlay_index, mask_index + 1);
    dim = fake_renderer_draw(overlay_index + 1);
    CHECK(dim->is_filled_rect);
    CHECK(nearly_equal(dim->color.alpha, 0.75f));
    CHECK_INT(text_index, overlay_index + 2);
    flash = fake_renderer_draw(text_index + 1);
    CHECK(flash->is_filled_rect);
    CHECK(flash->color.red == 1.0f);
    CHECK(card_index > text_index + 1);
    CHECK(fake_renderer_draw(fake_renderer_draw_count() - 1)->color.alpha == 1.0f);
    viewer_app_stop();
}

static void test_a_burst_under_the_mask_keeps_its_place_on_the_beam_view(void)
{
    const struct fake_draw *overlay;

    set_up_text_scene("scene opening\nroom 1\nplate room_a 0 0 1024 384\ncamera 0 192\nmask_on\noverlay 1 art 41.6 226.14 28.4444 28.4444 0\nwait 6000\n");
    run_frames(1);
    draw_frame();
    overlay = find_draw_with_texture_width(OVERLAY_ART_WIDTH);
    CHECK(overlay != NULL);
    CHECK(nearly_equal(overlay->screen_area.x, screen_x_when_camera_is_centred(41.6f, 0.0f)));
    CHECK(nearly_equal(overlay->screen_area.y, screen_y_when_camera_is_centred(226.14f, 192.0f)));
    viewer_app_stop();
}

static void test_a_scene_that_ends_leaves_its_last_text_fading_and_clears_on_replay(void)
{
    set_up_text_scene("scene opening\nmask_on\noverlay 1 art 0 0 30 20 0\ntext_show 1 line_a 480 72 0\ndim_to 0.5 0\nwait 6\ntext_hide 1 60\n");
    run_frames(8);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(OVERLAY_ART_WIDTH) != NULL);
    CHECK(scene_stage_view()->effects.mask_is_on);
    run_frames(62);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    press(INPUT_CROSS);
    CHECK_INT(scene_player_next_step(), 0);
    CHECK(!scene_stage_view()->layers.overlays[0].in_use);
    CHECK(!scene_stage_view()->layers.text_lines[0].in_use);
    CHECK(!scene_stage_view()->effects.mask_is_on);
    CHECK(scene_stage_view()->effects.brightness.value == 1.0f);
    viewer_app_stop();
}

static void test_the_end_prompt_waits_for_text_that_is_still_fading_out(void)
{
    int draws_during_the_fade;
    int draws_after_the_fade;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6\ntext_hide 1 60\n");
    run_frames(8);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    draws_during_the_fade = fake_renderer_draw_count();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    run_frames(62);
    draw_frame();
    draws_after_the_fade = fake_renderer_draw_count();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) == NULL);
    CHECK_INT(draws_after_the_fade, draws_during_the_fade - 1 + SCENE_END_PROMPT_VISIBLE_CHARACTERS);
    viewer_app_stop();
}

static void test_the_end_prompt_is_not_held_back_by_text_left_on_screen(void)
{
    int draws_while_running;

    set_up_text_scene("scene opening\ntext_show 1 line_a 480 72 0\nwait 6\n");
    draw_frame();
    draws_while_running = fake_renderer_draw_count();
    run_frames(8);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    CHECK_INT(fake_renderer_draw_count(), draws_while_running + SCENE_END_PROMPT_VISIBLE_CHARACTERS);
    viewer_app_stop();
}

static void test_stopping_a_scene_clears_text_overlays_mask_dim_and_flash_together(void)
{
    set_up_text_scene("scene opening\nmask_on\noverlay 1 art 0 0 30 20 0\ntext_show 1 line_a 480 72 0\ndim_to 0.5 0\nflash 1 6000\nwait 6000\n");
    run_frames(1);
    draw_frame();
    CHECK(find_draw_with_texture_width(TEXT_LINE_WIDTH) != NULL);
    scene_player_stop();
    CHECK(!scene_stage_view()->layers.overlays[0].in_use);
    CHECK(!scene_stage_view()->layers.text_lines[0].in_use);
    CHECK(!scene_stage_view()->effects.mask_is_on);
    CHECK(scene_stage_view()->effects.brightness.value == 1.0f);
    CHECK(scene_stage_view()->effects.flash_level.value == 0.0f);
    viewer_app_stop();
}

static void write_card_scene(char *text, unsigned int text_size, int texture_count)
{
    unsigned int used;
    int index;

    used = (unsigned int)snprintf(text, text_size, "scene opening\n");
    for (index = 0; index < texture_count; index++) {
        used = used + (unsigned int)snprintf(&text[used], text_size - used, "card t%d\n", index);
    }
    snprintf(&text[used], text_size - used, "wait 60\n");
}

static void test_scene_with_more_textures_than_the_read_queue_loads_them_all(void)
{
    static char scene_text[4096];
    const struct cached_texture *texture;
    char name[16];
    int index;

    set_up_platform();
    write_card_scene(scene_text, sizeof(scene_text), 130);
    set_up_scene_assets(scene_text);
    for (index = 0; index < 130; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        add_texture(name, 4, 4);
    }
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene ready name=opening");
    CHECK(strstr(log_text(), "texture missing") == NULL);
    for (index = 0; index < 130; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        texture = texture_cache_use_or_start_loading(name);
        CHECK(texture != NULL);
        CHECK_INT(texture->state, CACHED_TEXTURE_READY);
    }
    CHECK(strstr(log_text(), "does_not_fit_in_cache") == NULL);
    viewer_app_stop();
}

static void test_more_textures_than_cache_entries_says_so_and_still_plays(void)
{
    static char scene_text[256 * 16];
    char name[16];
    char expected[64];
    int index;

    set_up_platform();
    write_card_scene(scene_text, sizeof(scene_text), 4);
    set_up_scene_assets(scene_text);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX + 8; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        add_texture(name, 4, 4);
    }
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX + 8; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        texture_cache_use_or_start_loading(name);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    snprintf(expected, sizeof(expected), "texture_cache full name=t%d", TEXTURE_CACHE_ENTRY_MAX);
    CHECK_HAS_TEXT(log_text(), expected);
    viewer_app_stop();
}

static void test_scene_larger_than_the_memory_budget_says_so_and_still_plays(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard big_a\ncard big_b\ncard big_c\ncard big_d\nwait 60\n");
    add_texture("big_a", HALF_BUDGET_TEXTURE_WIDTH, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("big_b", HALF_BUDGET_TEXTURE_WIDTH, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("big_c", HALF_BUDGET_TEXTURE_WIDTH, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("big_d", HALF_BUDGET_TEXTURE_WIDTH, HALF_BUDGET_TEXTURE_WIDTH);
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene does_not_fit_in_cache name=opening");
    viewer_app_stop();
}

static void test_time_past_the_end_of_a_hold_counts_towards_the_next_hold(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nwait 6\nfade_out 60\nwait 600\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    update_with_elapsed_time(0);
    CHECK_INT(scene_player_next_step(), 1);
    update_with_elapsed_time(6 * 16666 + 100000);
    CHECK_INT(scene_player_next_step(), 2);
    CHECK(scene_stage_view()->fade_amount > 0.09f);
    CHECK(scene_stage_view()->fade_amount < 0.11f);
    viewer_app_stop();
}

static void test_each_frame_change_of_a_moving_object_is_logged(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nobject hero walk 100 150 1 0\nwait 6000\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    run_frames(10);
    CHECK_HAS_TEXT(log_text(), "frame_change object=hero from=0 to=1 passed_over=0");
    viewer_app_stop();
}

static void test_a_long_frame_logs_the_frames_it_passed_over_and_does_not_count_them_as_shown(void)
{
    set_up_platform();
    add_text_file("assets/animations/three.anim", "animation three\nloops 0\nframe walk_page 6 0 0 10 10 0 0 1 1\nframe walk_page 6 0 0 10 10 0 0 1 1\nframe walk_page 6 0 0 10 10 0 0 1 1\n");
    set_up_scene_assets("scene opening\nobject hero three 100 150 1 0\nwait 6000\nremove_object hero\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    update_with_elapsed_time(0);
    update_with_elapsed_time(13 * 16667);
    CHECK_HAS_TEXT(log_text(), "frame_change object=hero from=0 to=2 passed_over=1");
    press(INPUT_CROSS);
    run_frames(1);
    CHECK_HAS_TEXT(log_text(), "animation coverage object=hero name=three frames_shown=2 frames_total=3");
    viewer_app_stop();
}

static void test_the_first_update_does_not_shorten_the_first_hold(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nwait 6\nwait 600\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    update_with_elapsed_time(6 * 16666 + 100000);
    CHECK_INT(scene_player_next_step(), 1);
    viewer_app_stop();
}

static void test_a_scene_that_fails_to_play_leaves_the_room_alone(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 256 192\nobject hero walk 100 150 1 0\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(8);
    CHECK_INT(viewer_app_play_scene("missing"), 0);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_a_long_stall_only_counts_as_half_a_second(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nfade_out 60\nwait 600\n");
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    update_with_elapsed_time(0);
    update_with_elapsed_time(20000000);
    CHECK_INT(scene_player_next_step(), 1);
    CHECK(scene_stage_view()->fade_amount > 0.49f);
    CHECK(scene_stage_view()->fade_amount < 0.51f);
    viewer_app_stop();
}

static void write_manifest_of_animations(int animation_count)
{
    static char manifest_text[8192];
    unsigned int used;
    char path[64];
    char text[128];
    int index;

    used = (unsigned int)snprintf(manifest_text, sizeof(manifest_text), "background room_a\n");
    for (index = 0; index < animation_count; index++) {
        snprintf(path, sizeof(path), "assets/animations/a%d.anim", index);
        snprintf(text, sizeof(text), "animation a%d\nloops 1\nframe walk_page 6 0 0 10 10 0 0 1 1\n", index);
        add_text_file(path, text);
        used = used + (unsigned int)snprintf(&manifest_text[used], sizeof(manifest_text) - used, "animation a%d\n", index);
    }
    add_text_file("assets/manifest.txt", manifest_text);
}

static void test_browsing_past_the_animation_library_limit_keeps_working(void)
{
    char name[16];
    int index;

    set_up_platform();
    set_up_textures_and_animations();
    write_manifest_of_animations(ANIMATION_LIBRARY_MAX + 2);
    viewer_app_start("assets");
    for (index = 0; index < ANIMATION_LIBRARY_MAX + 2; index++) {
        snprintf(name, sizeof(name), "a%d", index);
        CHECK(viewer_app_show_animation(name));
    }
    CHECK(strstr(log_text(), "library_full") == NULL);
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    CHECK(find_draw_with_texture_width(CHARACTER_WIDTH) != NULL);
    viewer_app_stop();
}

static void write_scene_of_animations(const char *path, const char *scene_name, int first_animation, int animation_count)
{
    static char scene_text[8192];
    unsigned int used;
    int index;

    used = (unsigned int)snprintf(scene_text, sizeof(scene_text), "scene %s\n", scene_name);
    for (index = 0; index < animation_count; index++) {
        used = used + (unsigned int)snprintf(&scene_text[used], sizeof(scene_text) - used, "object o%d a%d 0 0 0 0\n", index % 8, first_animation + index);
    }
    snprintf(&scene_text[used], sizeof(scene_text) - used, "wait 60\n");
    add_text_file(path, scene_text);
}

static void test_each_scene_starts_with_an_empty_animation_library(void)
{
    set_up_platform();
    set_up_textures_and_animations();
    write_manifest_of_animations(200);
    write_scene_of_animations("assets/scenes/first.scene", "first", 0, 100);
    write_scene_of_animations("assets/scenes/second.scene", "second", 100, 100);
    viewer_app_start("assets");
    CHECK(viewer_app_play_scene("first"));
    run_frames_until_scene_is_no_longer_preloading();
    CHECK(viewer_app_play_scene("second"));
    run_frames_until_scene_is_no_longer_preloading();
    CHECK(strstr(log_text(), "library_full") == NULL);
    viewer_app_stop();
}

static void test_replaying_a_paused_scene_starts_it_unpaused(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\ncard card\nwait 6000\n");
    viewer_app_start("assets");
    run_frames(4);
    press(INPUT_SELECT);
    press(INPUT_CROSS);
    CHECK(scene_player_is_paused());
    press(INPUT_L);
    CHECK_INT(scene_player_next_step(), 0);
    run_frames(8);
    CHECK(!scene_player_is_paused());
    CHECK(scene_player_next_step() > 0);
    viewer_app_stop();
}

static void test_uploading_never_evicts_what_the_last_frame_drew(void)
{
    set_up_platform();
    add_texture("room_a", HALF_BUDGET_TEXTURE_WIDTH - 16, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("room_b", HALF_BUDGET_TEXTURE_WIDTH - 8, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("room_c", HALF_BUDGET_TEXTURE_WIDTH - 4, HALF_BUDGET_TEXTURE_WIDTH);
    add_texture("big_page", HALF_BUDGET_TEXTURE_WIDTH, HALF_BUDGET_TEXTURE_WIDTH);
    add_text_file("assets/animations/big.anim", "animation big\nloops 1\nframe big_page 6 0 0 10 10 0 0 1 1\n");
    add_text_file("assets/manifest.txt", "background room_a\nbackground room_b\nbackground room_c\nanimation big\n");
    viewer_app_start("assets");
    run_frames(2);
    press(INPUT_RIGHT);
    run_frames(2);
    draw_frame();
    CHECK(find_draw_with_texture_width(HALF_BUDGET_TEXTURE_WIDTH) != NULL);
    press(INPUT_RIGHT);
    draw_frame();
    run_frames(1);
    draw_frame();
    CHECK_HAS_TEXT(log_text(), "background shown name=room_c");
    CHECK(find_draw_with_texture_width(HALF_BUDGET_TEXTURE_WIDTH) != NULL);
    viewer_app_stop();
}

static void test_a_reader_thread_that_fails_to_start_is_logged(void)
{
    set_up_platform();
    fake_reader_fail_start(1);
    set_up_browsing_assets();
    viewer_app_start("assets");
    CHECK_HAS_TEXT(log_text(), "reader thread_failed");
    viewer_app_stop();
}

static void add_audio_files(void)
{
    fake_files_add("assets/music/song.ogg", sine_stereo_ogg, sizeof(sine_stereo_ogg));
    fake_files_add("assets/sounds/ping.ogg", sine_mono_ogg, sizeof(sine_mono_ogg));
}

static void pump_audio_workers(int steps)
{
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, steps);
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, steps);
}

static void run_frames_and_workers_until_scene_runs(void)
{
    int frame;

    for (frame = 0; frame < 64 && scene_player_status() == SCENE_PLAYER_PRELOADING; frame++) {
        pump_audio_workers(1);
        run_frames(1);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
}

static void test_a_scene_with_audio_plays_through_both_ports(void)
{
    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    viewer_app_start("assets");
    run_frames(10);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    run_frames_and_workers_until_scene_runs();
    CHECK_HAS_TEXT(log_text(), "scene ready name=opening");
    run_frames(20);
    pump_audio_workers(12);
    CHECK_INT(fake_audio_port_rate(AUDIO_PORT_MUSIC), 44100);
    CHECK_INT(fake_audio_port_grain_frames(AUDIO_PORT_MUSIC), 4096);
    CHECK_INT(fake_audio_port_rate(AUDIO_PORT_SOUND), 44100);
    CHECK_INT(fake_audio_port_grain_frames(AUDIO_PORT_SOUND), 512);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), 12);
    CHECK_HAS_TEXT(log_text(), "music started name=song looping=1");
    CHECK_HAS_TEXT(log_text(), "music window name=song index=0 at=0 source_frame=0 grains=11 ");
    CHECK_HAS_TEXT(log_text(), "sound started name=ping voice=0");
    CHECK_HAS_TEXT(log_text(), "sound started name=ping voice=1");
    viewer_app_stop();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_a_stalled_main_loop_costs_the_workers_nothing(void)
{
    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    run_frames(10);
    pump_audio_workers(1);
    pump_audio_workers(30);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), 31);
    CHECK(fake_audio_port_grain_count(AUDIO_PORT_SOUND) > 30);
    CHECK_HAS_TEXT(log_text(), " late=0 ");
    CHECK(strstr(log_text(), "late=1") == NULL);
    viewer_app_stop();
}

static void test_stopping_the_app_closes_both_ports_before_returning(void)
{
    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    run_frames(10);
    pump_audio_workers(3);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(fake_audio_port_is_open(AUDIO_PORT_SOUND));
    viewer_app_stop();
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK(!fake_audio_thread_is_alive(AUDIO_THREAD_MUSIC));
    CHECK(!fake_audio_thread_is_alive(AUDIO_THREAD_SOUND));
    CHECK_HAS_TEXT(fake_audio_port_events(AUDIO_PORT_MUSIC), "drain close ");
    CHECK_HAS_TEXT(fake_audio_port_events(AUDIO_PORT_SOUND), "drain close ");
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK_HAS_TEXT(log_text(), "sound stopped reason=stop ");
}

static void test_a_scene_with_missing_audio_files_still_plays_to_its_end(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nmusic song 1\nsound ping\nwait 30\n");
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    CHECK_HAS_TEXT(log_text(), "music file_missing name=song");
    CHECK_HAS_TEXT(log_text(), "sound file_missing name=ping");
    run_frames(40);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    pump_audio_workers(5);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), 0);
    viewer_app_stop();
}

static void test_a_scene_with_damaged_audio_files_still_plays_to_its_end(void)
{
    static unsigned char damaged_song[sizeof(sine_stereo_ogg)];
    static unsigned char damaged_ping[sizeof(sine_mono_ogg)];

    set_up_platform();
    set_up_scene_assets("scene opening\nroom 1\nplate room_a 0 0 1024 384\nmusic song 1\nsound ping\nwait 30\n");
    memcpy(damaged_song, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    damaged_song[SINE_STEREO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("assets/music/song.ogg", damaged_song, sizeof(damaged_song));
    memcpy(damaged_ping, sine_mono_ogg, sizeof(sine_mono_ogg));
    damaged_ping[SINE_MONO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("assets/sounds/ping.ogg", damaged_ping, sizeof(damaged_ping));
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    CHECK_HAS_TEXT(log_text(), "music decoder_failed name=song error=-1");
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=-1");
    run_frames(40);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    draw_frame();
    CHECK(find_draw_with_texture_width(ROOM_A_WIDTH) != NULL);
    pump_audio_workers(5);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), 0);
    viewer_app_stop();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_a_port_that_will_not_open_leaves_the_scene_running(void)
{
    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    fake_audio_port_fail_open(AUDIO_PORT_MUSIC, -7);
    fake_audio_port_fail_open(AUDIO_PORT_SOUND, -8);
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    run_frames(20);
    pump_audio_workers(3);
    CHECK_HAS_TEXT(log_text(), "music port_open_failed code=0xfffffff9");
    CHECK_HAS_TEXT(log_text(), "sound port_open_failed code=0xfffffff8");
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    viewer_app_stop();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_running_out_of_memory_for_audio_leaves_the_scene_running(void)
{
    set_up_platform();
    set_up_scene_assets("scene opening\nmusic song 1\nsound ping\nwait 6000\n");
    add_audio_files();
    viewer_app_start("assets");
    fake_memory_set_largest_block(64);
    run_frames_and_workers_until_scene_runs();
    CHECK_HAS_TEXT(log_text(), "music out_of_memory name=song");
    CHECK_HAS_TEXT(log_text(), "sound out_of_memory name=ping");
    fake_memory_set_largest_block(0);
    run_frames(10);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    viewer_app_stop();
}

static void test_browsing_silences_the_scene_and_debug_off_plays_it_again(void)
{
    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    run_frames(20);
    pump_audio_workers(3);
    CHECK(viewer_app_show_animation("walk"));
    pump_audio_workers(2);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    press(INPUT_SELECT);
    press(INPUT_SELECT);
    run_frames_and_workers_until_scene_runs();
    run_frames(20);
    pump_audio_workers(2);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "scene started name=opening");
    viewer_app_stop();
}

static void test_cross_can_skip_every_hold_of_a_scene_with_audio_and_replay_it(void)
{
    int press_count;

    set_up_platform();
    set_up_scene_assets(AUDIO_SCENE);
    add_audio_files();
    viewer_app_start("assets");
    run_frames_and_workers_until_scene_runs();
    run_frames(1);
    for (press_count = 0; press_count < 3; press_count++) {
        press(INPUT_CROSS);
        run_frames(1);
        pump_audio_workers(1);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    pump_audio_workers(2);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    press(INPUT_CROSS);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    run_frames_and_workers_until_scene_runs();
    run_frames(20);
    pump_audio_workers(2);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 2);
    viewer_app_stop();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
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
    test_browsing_draws_a_small_background_centred_with_the_character_over_it();
    test_browsing_plays_animation_over_time_and_reports_coverage();
    test_coverage_reports_unplayed_frames();
    test_blank_frames_draw_nothing();
    test_missing_textures_are_marked_visibly();
    test_app_survives_missing_and_invalid_animation_files();
    test_app_rejects_invalid_manifest();
    test_buttons_switch_background_and_animation();
    test_stepping_walks_past_an_animation_that_will_not_load();
    test_cross_pauses_and_triangle_hides_hud();
    test_list_widget_opens_selects_and_closes();
    test_list_widget_closes_after_a_selection();
    test_slow_reads_hold_the_frame_and_get_reported_as_late();
    test_choosing_another_animation_drops_the_old_ones_unread_pages();
    test_frame_whose_page_a_full_cache_turned_away_waits_instead_of_drawing_magenta();
    test_old_background_stays_until_the_new_one_has_loaded();
    test_status_text_reaches_the_hud();
    test_scene_waits_for_textures_before_running();
    test_scene_applies_camera_flip_and_layers();
    test_a_wide_room_at_its_left_end_fills_the_screen_from_its_left_edge();
    test_a_wide_room_keeps_the_camera_centred_in_the_middle();
    test_a_wide_room_stops_at_its_right_edge();
    test_art_narrower_than_the_screen_is_centred_with_black_on_every_side();
    test_a_plate_just_below_the_camera_does_not_count_as_art();
    test_a_card_is_centred_at_its_own_shape_over_black();
    test_a_fade_covers_the_whole_screen();
    test_a_mask_with_no_beams_blackens_the_whole_screen_over_the_room();
    test_a_deleted_beam_lights_nothing();
    test_a_beam_passes_through_the_same_world_to_screen_view_as_the_art();
    test_a_masked_room_stays_centred_on_the_camera_even_at_its_left_end();
    test_a_beam_keeps_its_place_on_screen_across_a_cut_to_narrower_art();
    test_an_iris_uses_the_same_view_as_the_lit_room_it_opens_onto();
    test_the_mask_covers_the_black_around_narrow_art_and_is_drawn_after_it();
    test_a_beam_swings_while_the_script_waits();
    test_a_large_mask_is_logged_and_a_normal_one_is_not();
    test_the_iris_draws_a_soft_circle_with_black_around_it();
    test_a_closed_iris_leaves_the_screen_black_and_an_open_one_draws_nothing();
    test_an_iris_only_clears_once_its_clear_part_reaches_the_farthest_corner();
    test_mask_off_after_an_iris_shows_the_room();
    test_dim_flash_card_and_fade_draw_in_that_order_over_the_mask();
    test_no_dim_or_flash_draws_at_full_brightness();
    test_a_dim_and_a_flash_change_while_the_script_waits();
    test_browsing_after_a_masked_scene_shows_no_mask();
    test_watching_it_again_starts_with_no_mask_left_over();
    test_the_end_prompt_stays_readable_over_a_black_mask();
    test_the_iris_pattern_is_made_once_and_freed_at_the_end();
    test_making_the_iris_pattern_keeps_no_memory_once_it_is_a_texture();
    test_a_failed_iris_pattern_is_logged_and_the_iris_stays_black_around_its_circle();
    test_an_app_that_never_played_a_scene_draws_at_full_brightness();
    test_text_is_drawn_centred_at_its_top_with_its_fade_as_alpha();
    test_text_does_not_hold_the_script();
    test_a_line_starts_invisible_and_is_not_drawn_until_it_has_alpha();
    test_a_hidden_line_fades_and_is_then_gone();
    test_hiding_all_text_cuts_every_line_in_one_frame();
    test_replaying_a_scene_clears_its_text_and_overlays();
    test_leaving_a_scene_for_browsing_clears_its_text_and_overlays();
    test_skipping_a_hold_with_cross_strands_no_text();
    test_skipping_to_the_end_of_a_scene_still_finishes_the_last_fade();
    test_pausing_stops_text_fades();
    test_text_textures_are_loaded_before_the_scene_starts();
    test_a_missing_text_texture_is_marked_visibly();
    test_a_line_hidden_with_no_fade_is_gone_from_that_frame();
    test_a_text_texture_still_loading_at_draw_time_draws_nothing_until_it_arrives();
    test_a_text_texture_the_read_queue_refuses_at_draw_time_is_marked_visibly();
    test_a_text_texture_the_full_cache_cannot_hold_is_marked_visibly();
    test_the_opening_bubble_and_its_text_land_on_whole_screen_pixels();
    test_an_overlay_goes_through_the_view_and_grows_about_its_centre();
    test_a_new_room_removes_overlays_but_keeps_text();
    test_hiding_an_overlay_step_removes_it_at_once();
    test_the_room_draws_first_then_bubbles_then_text_then_the_fade();
    test_bubbles_sit_under_the_dim_and_text_over_it_and_under_the_flash();
    test_a_burst_under_the_mask_keeps_its_place_on_the_beam_view();
    test_a_scene_that_ends_leaves_its_last_text_fading_and_clears_on_replay();
    test_the_end_prompt_waits_for_text_that_is_still_fading_out();
    test_the_end_prompt_is_not_held_back_by_text_left_on_screen();
    test_stopping_a_scene_clears_text_overlays_mask_dim_and_flash_together();
    test_scene_runs_steps_in_order_and_finishes();
    test_scene_fades_and_pans_over_time();
    test_scene_survives_bad_files_and_missing_assets();
    test_browsing_stops_a_running_scene();
    test_browsing_centres_an_animation_on_its_first_drawn_frame_wherever_the_camera_is();
    test_cross_skips_the_current_hold_instead_of_pausing();
    test_skipping_a_fade_or_pan_finishes_it_at_once();
    test_buttons_a_player_might_press_never_stall_the_scene();
    test_the_end_of_a_scene_says_so_and_cross_plays_it_again();
    test_select_turns_the_debug_controls_on_and_off();
    test_leaving_debug_controls_after_browsing_plays_the_scene_again();
    test_a_list_left_open_closes_when_debug_controls_go_off();
    test_every_button_press_is_logged();
    test_scene_with_more_textures_than_the_read_queue_loads_them_all();
    test_more_textures_than_cache_entries_says_so_and_still_plays();
    test_scene_larger_than_the_memory_budget_says_so_and_still_plays();
    test_time_past_the_end_of_a_hold_counts_towards_the_next_hold();
    test_each_frame_change_of_a_moving_object_is_logged();
    test_a_long_frame_logs_the_frames_it_passed_over_and_does_not_count_them_as_shown();
    test_the_first_update_does_not_shorten_the_first_hold();
    test_a_scene_that_fails_to_play_leaves_the_room_alone();
    test_a_long_stall_only_counts_as_half_a_second();
    test_browsing_past_the_animation_library_limit_keeps_working();
    test_each_scene_starts_with_an_empty_animation_library();
    test_replaying_a_paused_scene_starts_it_unpaused();
    test_uploading_never_evicts_what_the_last_frame_drew();
    test_a_reader_thread_that_fails_to_start_is_logged();
    test_a_scene_with_audio_plays_through_both_ports();
    test_a_stalled_main_loop_costs_the_workers_nothing();
    test_stopping_the_app_closes_both_ports_before_returning();
    test_a_scene_with_missing_audio_files_still_plays_to_its_end();
    test_a_scene_with_damaged_audio_files_still_plays_to_its_end();
    test_a_port_that_will_not_open_leaves_the_scene_running();
    test_running_out_of_memory_for_audio_leaves_the_scene_running();
    test_browsing_silences_the_scene_and_debug_off_plays_it_again();
    test_cross_can_skip_every_hold_of_a_scene_with_audio_and_replay_it();
    test_long_run_keeps_memory_bounded();
    fake_audio_reset();
    fake_files_reset();
    return test_finish("integration/viewer_app");
}
