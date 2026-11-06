#include <stdlib.h>
#include "animation_library.h"
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

static void run_frames_until_scene_is_no_longer_preloading(void)
{
    int frame;

    for (frame = 0; frame < 4096 && scene_player_status() == SCENE_PLAYER_PRELOADING; frame++) {
        run_frames(1);
    }
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

static void test_scene_with_more_textures_than_cache_entries_says_so_and_still_plays(void)
{
    static char scene_text[8192];
    char name[16];
    int index;

    set_up_platform();
    write_card_scene(scene_text, sizeof(scene_text), TEXTURE_CACHE_ENTRY_MAX + 8);
    set_up_scene_assets(scene_text);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX + 8; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        add_texture(name, 4, 4);
    }
    viewer_app_start("assets");
    run_frames_until_scene_is_no_longer_preloading();
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "texture_cache full name=t512");
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
    test_cross_pauses_and_triangle_hides_hud();
    test_list_widget_opens_selects_and_closes();
    test_list_widget_closes_after_a_selection();
    test_slow_reads_hold_the_frame_and_get_reported_as_late();
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
    test_scene_with_more_textures_than_cache_entries_says_so_and_still_plays();
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
    test_long_run_keeps_memory_bounded();
    fake_files_reset();
    return test_finish("integration/viewer_app");
}
