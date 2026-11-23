#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "game_settings.h"
#include "input.h"
#include "pause_chapters.h"
#include "pause_menu.h"
#include "pause_options.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"
#include "test_support.h"
#include "texture_cache.h"
#include "title_menu.h"

#define SETTLE_STEPS 40
#define LEAVE_STEPS 200
#define FLASH_STEPS 4
#define HOLD_STEPS 120
#define FADE_STEPS 60
#define CURSOR_SOUND 1
#define PICK_SOUND 26
#define SOUND_LOG_MAX 16

static const char widget_text[] =
    "slice title_button 18 0 0 64 56\nslice title_button_selected 18 64 0 64 56\n"
    "slice title_icon 14 128 0 48 48\nslice title_icon_selected 14 176 0 48 48\n"
    "slice title_button_pressed 18 0 192 58 58\nslice title_icon_pressed 15 64 192 48 48\n"
    "slice button 28 0 64 64 56\nslice button_selected 28 64 64 64 56\nslice back 17 128 64 42 34\nslice box 22 0 128 56 56\nslice choice 17 64 128 42 34\n"
    "text title.continue 0 200 120 40\ntext title.chapter_select 0 240 200 40\ntext title.new_game 0 280 130 40\ntext title.options 0 320 110 40\ntext title.language 0 360 120 40\n"
    "text row.yes 0 400 40 30\ntext row.no 0 430 40 30\ntext row.reset_ask 0 460 200 30\ntext row.new_game_ask 0 490 200 30\n"
    "text bar.back 200 200 40 20\ntext bar.title_continue 200 220 200 20\ntext bar.title_new_game 200 240 200 20\n"
    "cell trash 300 0 40 44\nglyph circle 0 0 40\n";

static int save_is_there;
static int clear_is_marked;
static int erase_calls;
static int sounds[SOUND_LOG_MAX];
static int sound_count;

static void on_sound(int kind)
{
    if (sound_count < SOUND_LOG_MAX) {
        sounds[sound_count] = kind;
    }
    sound_count = sound_count + 1;
}

static int has_save(void)
{
    return save_is_there;
}

static int is_cleared(void)
{
    return clear_is_marked;
}

static int erase(void)
{
    erase_calls = erase_calls + 1;
    save_is_there = 0;
    return 1;
}

static int chapter_reached(int chapter)
{
    return chapter <= 3;
}

static void start_with_art(int with_save)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add("set/pause/widgets.txt", (const unsigned char *)widget_text, (unsigned int)strlen(widget_text));
    fake_files_add_rgba_texture("set/textures/pause_widgets.gtex", 512, 512);
    fake_files_add_rgba_texture("set/textures/hud_sheet.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/pause_backdrop.gtex", 960, 544);
    fake_files_add_rgba_texture("set/textures/title_logo.gtex", 720, 232);
    fake_files_add_rgba_texture("set/textures/hud_button_art.gtex", 512, 512);
    texture_cache_init("set/textures", 8u << 20);
    game_settings_defaults(game_settings_now());
    CHECK(pause_widgets_load("set") > 0);
    save_is_there = with_save;
    clear_is_marked = 0;
    erase_calls = 0;
    title_menu_use(has_save, is_cleared, erase);
    title_menu_use_sound(on_sound);
    sound_count = 0;
    pause_chapters_use(chapter_reached);
}

static void frame(const struct input_state *input)
{
    texture_cache_upload_finished_reads(8);
    texture_cache_begin_frame();
    title_menu_update(input, PAUSE_TICK_US);
}

static void press(unsigned int buttons)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.pressed = buttons;
    frame(&input);
}

static void touch(int is_down, float x, float y)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.touch_is_down = is_down;
    input.touch_x = x;
    input.touch_y = y;
    frame(&input);
}

static void wait_steps(int count)
{
    int step;

    for (step = 0; step < count; step++) {
        press(0);
    }
}

static void open_until_shown(void)
{
    title_menu_open();
    wait_steps(SETTLE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
}

static void test_without_a_save_only_new_game_options_and_language_show(void)
{
    int chapter;

    start_with_art(0);
    CHECK(title_menu_is_ready());
    open_until_shown();
    CHECK(title_menu_shows(TITLE_ITEM_NEW_GAME));
    CHECK(title_menu_shows(TITLE_ITEM_OPTIONS));
    CHECK(title_menu_shows(TITLE_ITEM_LANGUAGE));
    CHECK(!title_menu_shows(TITLE_ITEM_CONTINUE));
    CHECK(!title_menu_shows(TITLE_ITEM_CHAPTERS));
    CHECK(!title_menu_shows(TITLE_ITEM_ERASE));
    CHECK(!title_menu_shows(TITLE_ITEM_RECORD));
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_NEW_GAME);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HOLDING);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NONE);
    wait_steps(LEAVE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_LEAVING);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NEW_GAME);
    CHECK(!title_menu_is_open());
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NONE);
}

static void test_with_a_save_the_cursor_starts_on_continue_and_picks_it(void)
{
    int chapter;

    start_with_art(1);
    open_until_shown();
    CHECK(title_menu_shows(TITLE_ITEM_CONTINUE));
    CHECK(title_menu_shows(TITLE_ITEM_CHAPTERS));
    CHECK(title_menu_shows(TITLE_ITEM_ERASE));
    CHECK(title_menu_shows(TITLE_ITEM_NEW_GAME));
    CHECK(!title_menu_shows(TITLE_ITEM_RECORD));
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_CONTINUE);
    press(INPUT_CROSS);
    wait_steps(LEAVE_STEPS);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_CONTINUE);
}

static void test_the_cursor_moves_to_the_nearest_entry_each_way(void)
{
    start_with_art(1);
    open_until_shown();
    press(INPUT_LEFT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_CHAPTERS);
    press(INPUT_LEFT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_CHAPTERS);
    press(INPUT_DOWN);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_NEW_GAME);
    press(INPUT_DOWN);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_LANGUAGE);
    press(INPUT_RIGHT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_ERASE);
    press(INPUT_UP);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_OPTIONS);
    press(INPUT_UP);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_CONTINUE);
}

static void test_erasing_asks_first_and_yes_leaves_only_a_new_game(void)
{
    start_with_art(1);
    open_until_shown();
    press(INPUT_DOWN);
    press(INPUT_DOWN);
    press(INPUT_RIGHT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_ERASE);
    press(INPUT_CROSS);
    CHECK(title_menu_dialog_is_open());
    press(INPUT_CROSS);
    CHECK(!title_menu_dialog_is_open());
    CHECK_INT(erase_calls, 0);
    CHECK(title_menu_shows(TITLE_ITEM_CONTINUE));
    press(INPUT_CROSS);
    press(INPUT_CIRCLE);
    CHECK_INT(erase_calls, 0);
    press(INPUT_CROSS);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_INT(erase_calls, 1);
    CHECK(!title_menu_shows(TITLE_ITEM_CONTINUE));
    CHECK(!title_menu_shows(TITLE_ITEM_ERASE));
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_NEW_GAME);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
}

static void test_a_new_game_over_a_save_asks_first(void)
{
    int chapter;

    start_with_art(1);
    open_until_shown();
    press(INPUT_DOWN);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_OPTIONS);
    press(INPUT_LEFT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_NEW_GAME);
    sound_count = 0;
    press(INPUT_CROSS);
    CHECK(title_menu_dialog_is_open());
    CHECK_INT(sound_count, 1);
    CHECK_INT(sounds[0], CURSOR_SOUND);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
    press(INPUT_CROSS);
    press(INPUT_LEFT);
    sound_count = 0;
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HOLDING);
    CHECK_INT(sounds[sound_count - 1], PICK_SOUND);
    wait_steps(LEAVE_STEPS);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NEW_GAME);
}

static void test_chapter_select_pans_to_the_grid_and_plays_a_reached_chapter(void)
{
    int chapter;

    start_with_art(1);
    open_until_shown();
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_OUT);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SUBMENU);
    CHECK_INT(pause_chapters_selected(), 3);
    press(INPUT_CIRCLE);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_BACK);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_CHAPTERS);
    press(INPUT_CROSS);
    wait_steps(SETTLE_STEPS);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK(pause_chapters_dialog_is_open());
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HOLDING);
    wait_steps(LEAVE_STEPS);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_CHAPTER);
    CHECK_INT(chapter, 2);
}

static void test_language_opens_the_options_language_list_and_closing_it_returns(void)
{
    start_with_art(0);
    open_until_shown();
    press(INPUT_DOWN);
    press(INPUT_DOWN);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_LANGUAGE);
    press(INPUT_CROSS);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SUBMENU);
    CHECK(pause_options_dropdown_is_open());
    press(INPUT_CIRCLE);
    press(0);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_BACK);
}

static void test_options_opens_the_pause_options_screen(void)
{
    start_with_art(0);
    open_until_shown();
    press(INPUT_DOWN);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_OPTIONS);
    press(INPUT_CROSS);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SUBMENU);
    CHECK(!pause_options_dropdown_is_open());
    press(INPUT_CIRCLE);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_BACK);
}

static void test_tapping_an_entry_selects_it_and_lifting_on_it_picks_it(void)
{
    start_with_art(1);
    open_until_shown();
    touch(1, 630.0f, 350.0f);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_OPTIONS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
    touch(0, 630.0f, 350.0f);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_OUT);
}

static void test_a_cleared_game_shows_the_notebook_only_once_the_people_screen_is_loaded(void)
{
    start_with_art(1);
    clear_is_marked = 1;
    open_until_shown();
    CHECK(!title_menu_shows(TITLE_ITEM_RECORD));
}

static const struct fake_draw *cover(void)
{
    fake_renderer_clear_draws();
    title_menu_draw();
    return fake_renderer_draw(fake_renderer_draw_count() - 1);
}

static void test_the_screen_fades_in_from_black_and_a_pick_flashes_white_holds_then_fades_to_black(void)
{
    const struct fake_draw *draw;
    int chapter;

    start_with_art(0);
    title_menu_open();
    fake_renderer_clear_draws();
    title_menu_draw();
    CHECK_INT(fake_renderer_draw_count(), 1);
    draw = fake_renderer_draw(0);
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 0.0f);
    wait_steps(SETTLE_STEPS);
    fake_renderer_clear_draws();
    title_menu_draw();
    CHECK(fake_renderer_draw_count() > 4);
    CHECK(!fake_renderer_draw(fake_renderer_draw_count() - 1)->is_filled_rect || fake_renderer_draw(fake_renderer_draw_count() - 1)->color.alpha < 1.0f);
    sound_count = 0;
    press(INPUT_CROSS);
    CHECK_INT(sound_count, 1);
    CHECK_INT(sounds[0], PICK_SOUND);
    draw = cover();
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 1.0f);
    CHECK(draw->color.alpha > 0.5f);
    wait_steps(FLASH_STEPS);
    draw = cover();
    CHECK(!draw->is_filled_rect || draw->color.alpha == 0.0f);
    wait_steps(HOLD_STEPS - FLASH_STEPS - 2);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HOLDING);
    wait_steps(2);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HIDING);
    wait_steps(FADE_STEPS / 2);
    draw = cover();
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 0.0f);
    CHECK(draw->color.alpha > 0.4f && draw->color.alpha < 0.6f);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NONE);
    wait_steps(FADE_STEPS / 2 + 1);
    draw = cover();
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 0.0f);
    CHECK(draw->color.alpha == 1.0f);
    CHECK_INT(title_menu_takes_request(&chapter), TITLE_REQUEST_NEW_GAME);
}

static int slice_draws(float x, float y, float width)
{
    const struct fake_draw *draw;
    int count;
    int index;

    fake_renderer_clear_draws();
    title_menu_draw();
    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && draw->texture_area.y == y / 512.0f && draw->texture_area.x >= x / 512.0f && draw->texture_area.x < (x + width) / 512.0f) {
            count = count + 1;
        }
    }
    return count;
}

static void test_a_picked_button_looks_pressed_through_the_hold_and_the_fade(void)
{
    start_with_art(1);
    open_until_shown();
    CHECK_INT(slice_draws(0.0f, 192.0f, 58.0f), 0);
    CHECK_INT(slice_draws(64.0f, 0.0f, 64.0f), 3);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HOLDING);
    CHECK_INT(slice_draws(0.0f, 192.0f, 58.0f), 3);
    CHECK_INT(slice_draws(64.0f, 0.0f, 64.0f), 0);
    wait_steps(HOLD_STEPS + FADE_STEPS / 2);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HIDING);
    CHECK_INT(slice_draws(0.0f, 192.0f, 58.0f), 3);
    CHECK_INT(slice_draws(64.0f, 0.0f, 64.0f), 0);
}

static void test_the_trash_looks_pressed_while_its_ask_is_open_and_not_after_no(void)
{
    start_with_art(1);
    open_until_shown();
    press(INPUT_LEFT);
    press(INPUT_DOWN);
    press(INPUT_DOWN);
    press(INPUT_RIGHT);
    CHECK_INT(title_menu_cursor(), TITLE_ITEM_ERASE);
    CHECK_INT(slice_draws(64.0f, 192.0f, 48.0f), 0);
    press(INPUT_CROSS);
    CHECK_INT(slice_draws(64.0f, 192.0f, 48.0f), 3);
    press(INPUT_CIRCLE);
    CHECK_INT(slice_draws(64.0f, 192.0f, 48.0f), 0);
    CHECK_INT(slice_draws(176.0f, 0.0f, 48.0f), 3);
}

static void test_moving_the_cursor_plays_the_cursor_sound_and_a_submenu_pick_the_pick_sound(void)
{
    start_with_art(1);
    open_until_shown();
    sound_count = 0;
    press(INPUT_LEFT);
    CHECK_INT(sound_count, 1);
    CHECK_INT(sounds[0], CURSOR_SOUND);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_PANNING_OUT);
    CHECK_INT(sound_count, 2);
    CHECK_INT(sounds[1], PICK_SOUND);
}

static int steer_to(int item)
{
    static const unsigned int ways[4] = { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT };
    int first;
    int second;

    for (first = 0; first < 4; first++) {
        for (second = 0; second < 5; second++) {
            open_until_shown();
            press(ways[first]);
            if (second < 4 && title_menu_cursor() != item) {
                press(ways[second]);
            }
            if (title_menu_cursor() == item) {
                return 1;
            }
        }
    }
    return 0;
}

static void test_only_buttons_that_enter_the_game_play_the_pick_sound(void)
{
    int with_save;
    int item;
    int step;
    int left_menu;

    CHECK(title_menu_item_enters_game(TITLE_ITEM_CONTINUE));
    CHECK(title_menu_item_enters_game(TITLE_ITEM_NEW_GAME));
    CHECK(title_menu_item_enters_game(TITLE_ITEM_CHAPTERS));
    CHECK(!title_menu_item_enters_game(TITLE_ITEM_OPTIONS));
    CHECK(!title_menu_item_enters_game(TITLE_ITEM_LANGUAGE));
    CHECK(!title_menu_item_enters_game(TITLE_ITEM_ERASE));
    CHECK(!title_menu_item_enters_game(TITLE_ITEM_RECORD));
    for (with_save = 0; with_save < 2; with_save++) {
        for (item = 0; item < TITLE_MENU_ITEM_COUNT; item++) {
            start_with_art(with_save);
            open_until_shown();
            if (!title_menu_shows(item)) {
                continue;
            }
            CHECK(steer_to(item));
            sound_count = 0;
            press(INPUT_CROSS);
            CHECK_INT(sound_count, 1);
            CHECK_INT(sounds[0], title_menu_item_enters_game(item) && !(item == TITLE_ITEM_NEW_GAME && with_save) ? PICK_SOUND : CURSOR_SOUND);
            left_menu = 0;
            for (step = 0; step < LEAVE_STEPS; step++) {
                press(0);
                left_menu = left_menu || title_menu_phase() == TITLE_PHASE_HOLDING;
            }
            if (!title_menu_item_enters_game(item)) {
                CHECK(!left_menu);
            }
        }
    }
}

static void test_the_menu_is_not_ready_without_its_art(void)
{
    fake_files_reset();
    texture_cache_init("set/textures", 1u << 20);
    CHECK_INT(pause_widgets_load("set"), 0);
    CHECK(!title_menu_is_ready());
}

static int notebook_art_lines;

static void note_notebook_art_line(const char *line)
{
    if (strstr(line, "hud_button_art") != NULL) {
        notebook_art_lines = notebook_art_lines + 1;
    }
}

static int the_title_asks_for_the_notebook_art(int sharp_art)
{
    start_with_art(1);
    fake_files_reset();
    fake_files_add("set/pause/widgets.txt", (const unsigned char *)widget_text, (unsigned int)strlen(widget_text));
    fake_files_add_rgba_texture("set/textures/pause_widgets.gtex", 512, 512);
    texture_cache_init("set/textures", 8u << 20);
    texture_cache_use_sharp_art(sharp_art);
    notebook_art_lines = 0;
    run_log_watch(note_notebook_art_line);
    title_menu_open();
    wait_steps(SETTLE_STEPS);
    fake_reader_release_reads(64);
    wait_steps(4);
    title_menu_draw();
    run_log_watch(NULL);
    texture_cache_use_sharp_art(0);
    return notebook_art_lines > 0;
}

static void test_the_ds_title_never_asks_for_the_remastered_notebook_art(void)
{
    CHECK_INT(the_title_asks_for_the_notebook_art(0), 1);
    CHECK_INT(the_title_asks_for_the_notebook_art(1), 0);
}

int main(void)
{
    test_the_ds_title_never_asks_for_the_remastered_notebook_art();
    test_without_a_save_only_new_game_options_and_language_show();
    test_with_a_save_the_cursor_starts_on_continue_and_picks_it();
    test_the_cursor_moves_to_the_nearest_entry_each_way();
    test_erasing_asks_first_and_yes_leaves_only_a_new_game();
    test_a_new_game_over_a_save_asks_first();
    test_chapter_select_pans_to_the_grid_and_plays_a_reached_chapter();
    test_language_opens_the_options_language_list_and_closing_it_returns();
    test_options_opens_the_pause_options_screen();
    test_tapping_an_entry_selects_it_and_lifting_on_it_picks_it();
    test_a_cleared_game_shows_the_notebook_only_once_the_people_screen_is_loaded();
    test_the_screen_fades_in_from_black_and_a_pick_flashes_white_holds_then_fades_to_black();
    test_moving_the_cursor_plays_the_cursor_sound_and_a_submenu_pick_the_pick_sound();
    test_a_picked_button_looks_pressed_through_the_hold_and_the_fade();
    test_the_trash_looks_pressed_while_its_ask_is_open_and_not_after_no();
    test_only_buttons_that_enter_the_game_play_the_pick_sound();
    test_the_menu_is_not_ready_without_its_art();
    return test_finish("unit/title_menu");
}
