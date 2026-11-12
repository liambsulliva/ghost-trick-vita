#include <stdio.h>
#include <string.h>
#include "audio_volume.h"
#include "fakes/fake_platform.h"
#include "file.h"
#include "game_settings.h"
#include "input.h"
#include "pause_options.h"
#include "pause_widgets.h"
#include "test_support.h"

#define CONTROLS_ROW 0
#define BRIGHTNESS_ROW 2
#define MASTER_ROW 5
#define MUSIC_ROW 6
#define MUSIC_TYPE_ROW 8
#define LANGUAGE_ROW 9
#define RESTORE_ROW 12
#define UNLOCK_ROW 13

static void start_options(void)
{
    fake_files_reset();
    game_settings_set_default_music(GAME_MUSIC_ARRANGED);
    game_settings_set_music_folders("set/music_ds", "set/music");
    game_settings_defaults(game_settings_now());
    game_settings_apply();
    pause_options_show_unlock(0);
    pause_options_enter();
}

static void press_down_to(int row)
{
    int steps;

    for (steps = 0; steps < 20 && pause_options_selected_row() != row; steps++) {
        pause_options_step(INPUT_DOWN);
    }
}

static void test_settings_survive_a_round_trip_and_bad_values_are_clamped(void)
{
    static const char clamped[] = "brightness 0\nmaster 99\nlanguage 5\n";
    struct game_settings written;
    struct game_settings read;
    char text[GAME_SETTINGS_TEXT_MAX];
    unsigned int size;

    game_settings_defaults(&written);
    written.brightness = 4;
    written.master = 3;
    written.music_type = GAME_MUSIC_ORIGINAL;
    size = game_settings_write_text(&written, text, sizeof(text));
    game_settings_defaults(&read);
    CHECK_INT(game_settings_read_text(&read, text, size), 7);
    CHECK_INT(read.brightness, 4);
    CHECK_INT(read.master, 3);
    CHECK_INT(read.music_type, GAME_MUSIC_ORIGINAL);
    CHECK_INT(game_settings_read_text(&read, clamped, (unsigned int)strlen(clamped)), 3);
    CHECK_INT(read.brightness, 1);
    CHECK_INT(read.master, GAME_SETTING_STEPS);
    CHECK_INT(read.language, 0);
}

static void test_full_volume_leaves_samples_untouched_and_half_scales_them(void)
{
    short samples[4] = { 1000, -1000, 32767, -32768 };

    audio_volume_set(AUDIO_PORT_SOUND, AUDIO_VOLUME_FULL);
    audio_volume_scale(AUDIO_PORT_SOUND, samples, 4);
    CHECK_INT(samples[0], 1000);
    CHECK_INT(samples[3], -32768);
    audio_volume_set(AUDIO_PORT_SOUND, AUDIO_VOLUME_FULL / 2);
    audio_volume_scale(AUDIO_PORT_SOUND, samples, 4);
    CHECK_INT(samples[0], 500);
    CHECK_INT(samples[1], -500);
    audio_volume_set(AUDIO_PORT_SOUND, AUDIO_VOLUME_FULL);
}

static void test_the_cursor_skips_rows_the_vita_fixes(void)
{
    start_options();
    CHECK_INT(pause_options_selected_row(), CONTROLS_ROW);
    pause_options_step(INPUT_DOWN);
    CHECK_INT(pause_options_selected_row(), BRIGHTNESS_ROW);
    pause_options_step(INPUT_DOWN);
    CHECK_INT(pause_options_selected_row(), MASTER_ROW);
    press_down_to(RESTORE_ROW);
    pause_options_step(INPUT_DOWN);
    CHECK_INT(pause_options_selected_row(), RESTORE_ROW);
}

static void test_volume_meters_drive_the_audio_gain(void)
{
    start_options();
    press_down_to(MASTER_ROW);
    CHECK_INT(audio_volume_of(AUDIO_PORT_MUSIC), AUDIO_VOLUME_FULL);
    pause_options_step(INPUT_LEFT);
    CHECK_INT(game_settings_now()->master, GAME_SETTING_STEPS - 1);
    CHECK_INT(audio_volume_of(AUDIO_PORT_MUSIC), game_settings_volume_gain(GAME_SETTING_STEPS - 1));
    CHECK_INT(audio_volume_of(AUDIO_PORT_SOUND), game_settings_volume_gain(GAME_SETTING_STEPS - 1));
    pause_options_step(INPUT_DOWN);
    CHECK_INT(pause_options_selected_row(), MUSIC_ROW);
    pause_options_step(INPUT_LEFT);
    CHECK(audio_volume_of(AUDIO_PORT_MUSIC) < audio_volume_of(AUDIO_PORT_SOUND));
}

static void test_leaving_saves_only_what_changed(void)
{
    unsigned int size;
    const unsigned char *saved;

    start_options();
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_LEAVES);
    CHECK(!fake_files_exists(GAME_SETTINGS_PATH));
    start_options();
    press_down_to(BRIGHTNESS_ROW);
    pause_options_step(INPUT_LEFT);
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_LEAVES);
    saved = fake_files_contents(GAME_SETTINGS_PATH, &size);
    CHECK(saved != NULL);
    if (saved != NULL) {
        char text[GAME_SETTINGS_TEXT_MAX];

        snprintf(text, sizeof(text), "%.*s", (int)size, (const char *)saved);
        CHECK_HAS_TEXT(text, "brightness 9\n");
    }
}

static void test_restore_asks_first_and_no_keeps_the_settings(void)
{
    start_options();
    press_down_to(MASTER_ROW);
    pause_options_step(INPUT_LEFT);
    press_down_to(RESTORE_ROW);
    pause_options_step(INPUT_CROSS);
    CHECK(pause_options_dialog_is_open());
    pause_options_step(INPUT_CROSS);
    CHECK(!pause_options_dialog_is_open());
    CHECK_INT(game_settings_now()->master, GAME_SETTING_STEPS - 1);
    pause_options_step(INPUT_CROSS);
    pause_options_step(INPUT_LEFT);
    pause_options_step(INPUT_CROSS);
    CHECK_INT(game_settings_now()->master, GAME_SETTING_STEPS);
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_LEAVES);
}

static void test_music_type_switches_the_music_folder_setting(void)
{
    start_options();
    press_down_to(MUSIC_TYPE_ROW);
    pause_options_step(INPUT_RIGHT);
    CHECK_INT(game_settings_now()->music_type, GAME_MUSIC_ORIGINAL);
    pause_options_step(INPUT_CROSS);
    CHECK_INT(game_settings_now()->music_type, GAME_MUSIC_ARRANGED);
}

static void test_the_language_list_opens_on_cross_and_circle_closes_it_first(void)
{
    start_options();
    press_down_to(LANGUAGE_ROW);
    pause_options_step(INPUT_RIGHT);
    CHECK(!pause_options_dropdown_is_open());
    pause_options_step(INPUT_CROSS);
    CHECK(pause_options_dropdown_is_open());
    pause_options_step(INPUT_DOWN);
    pause_options_step(INPUT_CROSS);
    CHECK(pause_options_dropdown_is_open());
    CHECK_INT(game_settings_now()->language, 0);
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_STAYS);
    CHECK(!pause_options_dropdown_is_open());
}

static void test_controls_opens_a_page_that_circle_closes(void)
{
    start_options();
    pause_options_step(INPUT_CROSS);
    CHECK(pause_options_is_on_controls_page());
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_STAYS);
    CHECK(!pause_options_is_on_controls_page());
}

static void test_a_tap_on_a_flame_sets_that_level(void)
{
    int tick;

    start_options();
    press_down_to(MASTER_ROW);
    for (tick = 0; tick < 120; tick++) {
        pause_options_tick();
    }
    pause_options_step(INPUT_UP);
    pause_options_step(INPUT_DOWN);
    CHECK_INT(pause_options_selected_row(), MASTER_ROW);
    CHECK_INT(pause_options_touch(1, 0, 1, 545.0f + 3.0f * 38.0f, 0.0f), 0u);
    CHECK_INT(pause_options_touch(0, 1, 0, 0.0f, 0.0f), 0u);
    CHECK_INT(pause_options_touch(1, 0, 1, 90.0f, 518.0f), INPUT_CIRCLE);
}

static void test_the_unlock_row_shows_only_when_built_in_and_flips_a_saved_setting(void)
{
    unsigned int size;
    const unsigned char *saved;
    struct game_settings read;

    start_options();
    press_down_to(UNLOCK_ROW);
    CHECK_INT(pause_options_selected_row(), RESTORE_ROW);
    pause_options_show_unlock(1);
    pause_options_enter();
    press_down_to(UNLOCK_ROW);
    CHECK_INT(pause_options_selected_row(), UNLOCK_ROW);
    CHECK_INT(game_settings_now()->unlock_all, 0);
    pause_options_step(INPUT_CROSS);
    CHECK_INT(game_settings_now()->unlock_all, 1);
    pause_options_step(INPUT_RIGHT);
    CHECK_INT(game_settings_now()->unlock_all, 0);
    pause_options_step(INPUT_LEFT);
    CHECK_INT(pause_options_step(INPUT_CIRCLE), PAUSE_OPTIONS_LEAVES);
    saved = fake_files_contents(GAME_SETTINGS_PATH, &size);
    CHECK(saved != NULL);
    if (saved != NULL) {
        game_settings_defaults(&read);
        game_settings_read_text(&read, (const char *)saved, size);
        CHECK_INT(read.unlock_all, 1);
    }
    pause_options_show_unlock(0);
}

int main(void)
{
    test_settings_survive_a_round_trip_and_bad_values_are_clamped();
    test_full_volume_leaves_samples_untouched_and_half_scales_them();
    test_the_cursor_skips_rows_the_vita_fixes();
    test_volume_meters_drive_the_audio_gain();
    test_leaving_saves_only_what_changed();
    test_restore_asks_first_and_no_keeps_the_settings();
    test_music_type_switches_the_music_folder_setting();
    test_the_language_list_opens_on_cross_and_circle_closes_it_first();
    test_controls_opens_a_page_that_circle_closes();
    test_a_tap_on_a_flame_sets_that_level();
    test_the_unlock_row_shows_only_when_built_in_and_flips_a_saved_setting();
    return test_finish("unit/pause_options");
}
