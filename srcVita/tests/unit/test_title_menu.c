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
#include "test_support.h"
#include "texture_cache.h"
#include "title_menu.h"

#define SETTLE_STEPS 40
#define LEAVE_STEPS 60

static const char widget_text[] =
    "slice title_button 18 0 0 64 56\nslice title_button_selected 18 64 0 64 56\n"
    "slice title_icon 14 128 0 48 48\nslice title_icon_selected 14 176 0 48 48\n"
    "slice button 28 0 64 64 56\nslice button_selected 28 64 64 64 56\nslice back 17 128 64 42 34\nslice box 22 0 128 56 56\nslice choice 17 64 128 42 34\n"
    "text title.continue 0 200 120 40\ntext title.chapter_select 0 240 200 40\ntext title.new_game 0 280 130 40\ntext title.options 0 320 110 40\ntext title.language 0 360 120 40\n"
    "text row.yes 0 400 40 30\ntext row.no 0 430 40 30\ntext row.reset_ask 0 460 200 30\ntext row.new_game_ask 0 490 200 30\n"
    "text bar.back 200 200 40 20\ntext bar.title_continue 200 220 200 20\ntext bar.title_new_game 200 240 200 20\n"
    "cell trash 300 0 40 44\nglyph circle 0 0 40\n";

static int save_is_there;
static int clear_is_marked;
static int erase_calls;

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
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HIDING);
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
    press(INPUT_CROSS);
    CHECK(title_menu_dialog_is_open());
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_SHOWING);
    press(INPUT_CROSS);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HIDING);
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
    CHECK_INT(title_menu_phase(), TITLE_PHASE_HIDING);
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

static void test_the_screen_fades_in_from_black_and_leaves_through_white(void)
{
    const struct fake_draw *draw;

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
    press(INPUT_CROSS);
    wait_steps(LEAVE_STEPS);
    fake_renderer_clear_draws();
    title_menu_draw();
    draw = fake_renderer_draw(fake_renderer_draw_count() - 1);
    CHECK(draw->is_filled_rect);
    CHECK(draw->color.red == 1.0f);
    CHECK(draw->color.alpha == 1.0f);
}

static void test_the_menu_is_not_ready_without_its_art(void)
{
    fake_files_reset();
    texture_cache_init("set/textures", 1u << 20);
    CHECK_INT(pause_widgets_load("set"), 0);
    CHECK(!title_menu_is_ready());
}

int main(void)
{
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
    test_the_screen_fades_in_from_black_and_leaves_through_white();
    test_the_menu_is_not_ready_without_its_art();
    return test_finish("unit/title_menu");
}
