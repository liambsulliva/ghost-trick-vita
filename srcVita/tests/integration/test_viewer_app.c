#include <stdlib.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "file.h"
#include "input.h"
#include "run_log.h"
#include "test_support.h"
#include "texture_cache.h"
#include "texture_file.h"
#include "viewer_app.h"

#define FRAME_US 16667
#define BACKGROUND_WIDTH 96
#define CHARACTER_WIDTH 32

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

static void set_up_complete_assets(void)
{
    add_texture("room_a", BACKGROUND_WIDTH, 54);
    add_texture("room_b", BACKGROUND_WIDTH, 54);
    add_texture("walk_00", CHARACTER_WIDTH, 32);
    add_texture("walk_01", CHARACTER_WIDTH, 32);
    add_texture("idle_00", CHARACTER_WIDTH, 32);
    add_text_file("assets/manifest.txt", "background room_a\nbackground room_b\nanimation walk\nanimation idle\n");
    add_text_file("assets/animations/walk.anim", "animation walk\nticks_per_second 10\nloops 1\nframe walk_00 1 100 200 300 400\nframe walk_01 1 110 200 300 400\n");
    add_text_file("assets/animations/idle.anim", "animation idle\nticks_per_second 10\nloops 1\nframe idle_00 1 500 200 300 400\n");
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

static void test_app_draws_background_then_character_scaled_to_screen(void)
{
    const struct fake_draw *background;
    const struct fake_draw *character;

    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    CHECK(viewer_app_has_assets());
    draw_frame();
    background = fake_renderer_draw(0);
    character = fake_renderer_draw(1);
    CHECK_INT(fake_renderer_texture_width(background->texture), BACKGROUND_WIDTH);
    CHECK(background->screen_area.width == 960.0f);
    CHECK(background->screen_area.height == 540.0f);
    CHECK(background->screen_area.y == 2.0f);
    CHECK_INT(fake_renderer_texture_width(character->texture), CHARACTER_WIDTH);
    CHECK(character->screen_area.x == 50.0f);
    CHECK(character->screen_area.y == 102.0f);
    CHECK(character->screen_area.width == 150.0f);
    CHECK(character->screen_area.height == 200.0f);
    viewer_app_stop();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_app_plays_animation_over_time_and_reports_coverage(void)
{
    const struct fake_draw *character;

    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    run_frames(7);
    draw_frame();
    character = fake_renderer_draw(1);
    CHECK(character->screen_area.x == 55.0f);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "animation coverage name=walk frames_shown=2 frames_total=2");
}

static void test_app_reports_unplayed_frames_in_coverage(void)
{
    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    run_frames(1);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "animation coverage name=walk frames_shown=1 frames_total=2");
}

static void test_app_marks_missing_character_textures_visibly(void)
{
    const struct fake_draw *character;

    set_up_platform();
    set_up_complete_assets();
    add_text_file("assets/animations/walk.anim", "frame not_converted 1 100 200 300 400\n");
    viewer_app_start("assets");
    CHECK_HAS_TEXT(log_text(), "texture missing name=not_converted");
    draw_frame();
    character = fake_renderer_draw(1);
    CHECK(character->is_filled_rect);
    CHECK(character->color.red == 1.0f);
    CHECK(character->color.blue == 1.0f);
    viewer_app_stop();
}

static void test_app_survives_missing_and_invalid_animation_files(void)
{
    set_up_platform();
    set_up_complete_assets();
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
    set_up_complete_assets();
    viewer_app_start("assets");
    press(INPUT_RIGHT);
    CHECK_HAS_TEXT(log_text(), "background selected name=room_b");
    press(INPUT_DOWN);
    CHECK_HAS_TEXT(log_text(), "animation loaded name=idle");
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 250.0f);
    press(INPUT_UP);
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 50.0f);
    viewer_app_stop();
}

static void test_cross_pauses_and_triangle_hides_hud(void)
{
    int draws_with_hud;

    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    press(INPUT_CROSS);
    run_frames(30);
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 50.0f);
    draws_with_hud = fake_renderer_draw_count();
    press(INPUT_TRIANGLE);
    draw_frame();
    CHECK(fake_renderer_draw_count() < draws_with_hud);
    CHECK_INT(fake_renderer_draw_count(), 2);
    viewer_app_stop();
}

static void test_list_widget_opens_selects_and_closes(void)
{
    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    press(INPUT_R);
    press(INPUT_DOWN);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_HAS_TEXT(log_text(), "animation loaded name=idle");
    CHECK(strstr(log_text(), "background selected name=room_b") == NULL);
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 250.0f);

    press(INPUT_R);
    press(INPUT_CIRCLE);
    press(INPUT_RIGHT);
    CHECK_HAS_TEXT(log_text(), "background selected name=room_b");
    viewer_app_stop();
}

static void test_list_widget_closes_after_a_selection(void)
{
    set_up_platform();
    set_up_complete_assets();
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
    set_up_complete_assets();
    viewer_app_start("assets");
    fake_reader_hold_reads(1);
    run_frames(7);
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 50.0f);
    fake_reader_release_reads(4);
    run_frames(3);
    draw_frame();
    CHECK(fake_renderer_draw(1)->screen_area.x == 55.0f);
    viewer_app_stop();
    CHECK_HAS_TEXT(log_text(), "frames_shown=2 frames_total=2 late_frames=1");
}

static void test_old_background_stays_until_the_new_one_has_loaded(void)
{
    renderer_texture first_background;

    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    draw_frame();
    first_background = fake_renderer_draw(0)->texture;

    fake_reader_hold_reads(1);
    press(INPUT_RIGHT);
    run_frames(1);
    draw_frame();
    CHECK_INT(fake_renderer_draw(0)->texture, first_background);
    CHECK(strstr(log_text(), "background shown name=room_b") == NULL);

    fake_reader_release_reads(4);
    run_frames(4);
    draw_frame();
    CHECK(fake_renderer_draw(0)->texture != first_background);
    CHECK_HAS_TEXT(log_text(), "background shown name=room_b");
    viewer_app_stop();
}

static void test_status_text_reaches_the_hud(void)
{
    int draws_before;

    set_up_platform();
    set_up_complete_assets();
    viewer_app_start("assets");
    draw_frame();
    draws_before = fake_renderer_draw_count();
    viewer_app_set_status_text("frame 16.6ms");
    draw_frame();
    CHECK_INT(fake_renderer_draw_count(), draws_before + 11);
    viewer_app_stop();
}

static void test_long_run_keeps_memory_bounded_and_every_frame_shown(void)
{
    int cycle;

    set_up_platform();
    set_up_complete_assets();
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
    test_app_draws_background_then_character_scaled_to_screen();
    test_app_plays_animation_over_time_and_reports_coverage();
    test_app_reports_unplayed_frames_in_coverage();
    test_app_marks_missing_character_textures_visibly();
    test_app_survives_missing_and_invalid_animation_files();
    test_app_rejects_invalid_manifest();
    test_buttons_switch_background_and_animation();
    test_cross_pauses_and_triangle_hides_hud();
    test_list_widget_opens_selects_and_closes();
    test_list_widget_closes_after_a_selection();
    test_slow_reads_hold_the_frame_and_get_reported_as_late();
    test_old_background_stays_until_the_new_one_has_loaded();
    test_status_text_reaches_the_hud();
    test_long_run_keeps_memory_bounded_and_every_frame_shown();
    fake_files_reset();
    return test_finish("integration/viewer_app");
}
