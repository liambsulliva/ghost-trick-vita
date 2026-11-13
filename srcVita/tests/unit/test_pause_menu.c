#include <math.h>
#include <stdio.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "input.h"
#include "pause_menu.h"
#include "pause_people.h"
#include "pause_save.h"
#include "script_machine.h"
#include "renderer.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRAME_ROWS 12
#define IDLE_TICKS 10
#define SETTLE_STEPS 40

static const char label_text[] =
    "label 0 0 0 100 50\nlabel 1 0 64 90 50\nlabel 2 0 128 110 50\nlabel 3 0 192 80 50\nlabel 4 0 256 120 50\nlabel 5 0 320 115 50\n"
    "arrows 0 384 40\nselected_column 320\n";
static const char track_text[] =
    "every 5\nlink 3\ndissolve 2\n"
    "idle 0\nidle 1\nidle 2\nidle 3\nidle 4\nidle 5\nidle 6\nidle 7\nidle 8\nidle 9\n"
    "fidget 10\nfidget 11\nfidget 10\nfidget 11\n";

static void add_text(const char *path, const char *text)
{
    fake_files_add(path, (const unsigned char *)text, (unsigned int)strlen(text));
}

static void add_cat_table(void)
{
    static char text[2048];
    unsigned int used;
    int row;

    used = (unsigned int)snprintf(text, sizeof(text), "pages 1\n");
    for (row = 0; row < FRAME_ROWS; row++) {
        used += (unsigned int)snprintf(text + used, sizeof(text) - used, "frame t_cat %d 1 0 %d 0 30 40 -15 -38\n", row, row * 32);
    }
    add_text("set/pause/cat.txt", text);
}

static void start_with_art(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_cat_table();
    add_text("set/pause/cat_track.txt", track_text);
    add_text("set/pause/labels.txt", label_text);
    add_text("set/pause/widgets.txt", "slice row 31 0 0 70 62\ntext bar.back 0 70 40 20\nglyph circle 40 0 40\n");
    fake_files_add_rgba_texture("set/textures/pause_widgets.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/hud_sheet.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/pause_backdrop.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/pause_spotlight.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/pause_labels.gtex", 8, 8);
    fake_files_add_rgba_texture("set/textures/pause_cat_00.gtex", 8, 8);
    texture_cache_init("set/textures", 1u << 20);
    CHECK(pause_menu_load("set"));
}

static void push_stick(float x, float y)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.stick_x = x;
    input.stick_y = y;
    texture_cache_upload_finished_reads(8);
    texture_cache_begin_frame();
    pause_menu_update(&input, PAUSE_TICK_US);
}

static void touch(int is_down, float x, float y)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.touch_is_down = is_down;
    input.touch_x = x;
    input.touch_y = y;
    texture_cache_upload_finished_reads(8);
    texture_cache_begin_frame();
    pause_menu_update(&input, PAUSE_TICK_US);
}

static void press(unsigned int buttons)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.pressed = buttons;
    texture_cache_upload_finished_reads(8);
    texture_cache_begin_frame();
    pause_menu_update(&input, PAUSE_TICK_US);
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
    pause_menu_open();
    wait_steps(SETTLE_STEPS);
}

static void check_step(struct pause_cat_step step, int first, int second, float weight)
{
    CHECK_INT(step.first, first);
    CHECK_INT(step.second, second);
    CHECK(fabsf(step.weight - weight) < 0.001f);
}

static void test_the_fidget_dissolves_in_at_the_link_tick_and_back_out_to_it(void)
{
    int tick;

    start_with_art();
    pause_cat_track_restart();
    for (tick = 0; tick < 13; tick++) {
        check_step(pause_cat_track_next(), tick % IDLE_TICKS, -1, 0.0f);
    }
    check_step(pause_cat_track_next(), 3, 10, 1.0f / 3.0f);
    check_step(pause_cat_track_next(), 4, 11, 2.0f / 3.0f);
    check_step(pause_cat_track_next(), 10, -1, 0.0f);
    check_step(pause_cat_track_next(), 11, -1, 0.0f);
    check_step(pause_cat_track_next(), 11, 3, 1.0f / 3.0f);
    check_step(pause_cat_track_next(), 11, 4, 2.0f / 3.0f);
    check_step(pause_cat_track_next(), 5, -1, 0.0f);
}

static void test_start_darkens_the_game_then_shows_the_menu(void)
{
    start_with_art();
    pause_menu_open();
    CHECK(pause_menu_is_open());
    press(0);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_DARKENING);
    CHECK(pause_menu_shows_game());
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK(!pause_menu_shows_game());
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
    CHECK(pause_menu_black_alpha() == 0.0f);
}

static void test_resume_hands_the_screen_back_to_the_game(void)
{
    start_with_art();
    open_until_shown();
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_HIDING);
    wait_steps(SETTLE_STEPS);
    CHECK(!pause_menu_is_open());
    CHECK(pause_menu_shows_game());
}

static void test_the_cursor_walks_the_two_columns(void)
{
    start_with_art();
    open_until_shown();
    press(INPUT_UP);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
    press(INPUT_DOWN);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_SAVE);
    press(INPUT_RIGHT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_CHAPTERS);
    press(INPUT_RIGHT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_CHAPTERS);
    press(INPUT_DOWN);
    press(INPUT_DOWN);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_OPTIONS);
    press(INPUT_LEFT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_PEOPLE);
}

static void test_the_left_stick_steps_the_cursor_once_per_push(void)
{
    start_with_art();
    open_until_shown();
    push_stick(0.0f, 0.9f);
    push_stick(0.0f, 1.0f);
    push_stick(0.1f, 0.8f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_SAVE);
    push_stick(0.0f, 0.0f);
    push_stick(0.9f, 0.2f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_CHAPTERS);
    push_stick(0.0f, 0.0f);
    push_stick(-0.2f, -0.9f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_TITLE);
}

static void hold(unsigned int buttons, unsigned int fresh, float stick_y)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.held = buttons;
    input.pressed = fresh;
    input.stick_y = stick_y;
    texture_cache_upload_finished_reads(8);
    texture_cache_begin_frame();
    pause_menu_update(&input, PAUSE_TICK_US);
}

static void hold_for(unsigned int buttons, float stick_y, int steps)
{
    int step;

    for (step = 0; step < steps; step++) {
        hold(buttons, 0, stick_y);
    }
}

static void test_a_held_direction_repeats_after_a_pause_but_only_from_a_fresh_press(void)
{
    start_with_art();
    open_until_shown();
    hold(INPUT_DOWN, INPUT_DOWN, 0.0f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_SAVE);
    hold_for(INPUT_DOWN, 0.0f, 20);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_SAVE);
    hold_for(INPUT_DOWN, 0.0f, 12);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_PEOPLE);
    hold(0, 0, 0.0f);
    hold(0, 0, -1.0f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_SAVE);
    hold_for(0, -1.0f, 40);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
    hold(0, 0, 0.0f);
}

static void test_a_direction_held_into_the_menu_never_repeats(void)
{
    start_with_art();
    pause_menu_open();
    hold_for(INPUT_DOWN, 0.0f, SETTLE_STEPS + 60);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
}

static void test_a_stick_held_while_start_opens_the_menu_waits_for_a_fresh_push(void)
{
    int step;

    start_with_art();
    pause_menu_open();
    for (step = 0; step < SETTLE_STEPS; step++) {
        push_stick(0.0f, 1.0f);
    }
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
}

static void test_tapping_a_label_selects_it_and_lifting_on_it_picks_it(void)
{
    start_with_art();
    open_until_shown();
    touch(1, 810.0f, 369.0f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_OPTIONS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    touch(0, 0.0f, 0.0f);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_PANNING_OUT);
    wait_steps(SETTLE_STEPS);
    touch(1, 90.0f, 518.0f);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_PANNING_BACK);
}

static void test_sliding_off_a_label_before_lifting_picks_nothing(void)
{
    start_with_art();
    open_until_shown();
    touch(1, 300.0f, 123.0f);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
    touch(1, 480.0f, 300.0f);
    touch(0, 0.0f, 0.0f);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    touch(1, 480.0f, 300.0f);
    touch(0, 0.0f, 0.0f);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
}

static void test_picking_pans_toward_the_item_side_and_back_fades_the_labels_in(void)
{
    start_with_art();
    open_until_shown();
    press(INPUT_DOWN);
    press(INPUT_RIGHT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_CHAPTERS);
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_PANNING_OUT);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SUBMENU);
    CHECK(pause_menu_camera_x() == (float)RENDERER_WIDTH);
    CHECK(pause_menu_label_alpha() == 0.0f);
    press(INPUT_CIRCLE);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_PANNING_BACK);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK(pause_menu_camera_x() == 0.0f);
    CHECK(pause_menu_label_alpha() == 1.0f);
    CHECK(pause_menu_black_alpha() == 0.0f);
}

static void test_people_opens_the_notebook_screen_where_l_switches_tabs_and_circle_returns(void)
{
    static unsigned char notes[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];

    start_with_art();
    add_text("set/pause/people.txt", "order 3 1\nplace_count 2\nface 3 pause_face_00\nthumb pause_face_00 0 0 104 78\nnote 1 3 10 pause_note_1_03_010\n");
    CHECK(pause_menu_load("set"));
    memset(notes, 0, sizeof(notes));
    notes[1][3] = 11;
    pause_people_use_notes(&notes[0][0]);
    open_until_shown();
    press(INPUT_DOWN);
    press(INPUT_DOWN);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_PEOPLE);
    press(INPUT_CROSS);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SUBMENU);
    CHECK(pause_menu_camera_x() == -(float)RENDERER_WIDTH);
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 1);
    CHECK_TEXT(pause_people_card(), "pause_note_1_03_010");
    press(INPUT_L);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PLACES);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SUBMENU);
    press(INPUT_CIRCLE);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_PANNING_BACK);
}

static void test_select_opens_straight_onto_the_record_and_circle_returns_to_the_game(void)
{
    static unsigned char notes[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];

    start_with_art();
    add_text("set/pause/people.txt", "order 3 1\nplace_count 2\nface 3 pause_face_00\nthumb pause_face_00 0 0 104 78\nnote 1 3 10 pause_note_1_03_010\n");
    CHECK(pause_menu_load("set"));
    memset(notes, 0, sizeof(notes));
    notes[1][3] = 11;
    pause_people_use_notes(&notes[0][0]);
    pause_menu_open_on_record();
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_DARKENING);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SUBMENU);
    CHECK(pause_menu_camera_x() == -(float)RENDERER_WIDTH);
    CHECK_TEXT(pause_people_card(), "pause_note_1_03_010");
    CHECK(pause_menu_black_alpha() == 0.0f);
    press(INPUT_CIRCLE);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_HIDING);
    pause_menu_open();
    wait_steps(SETTLE_STEPS * 4);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_CLOSED);
    pause_menu_open();
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
}

static int saves_made;

static int fake_save(void)
{
    saves_made = saves_made + 1;
    return 1;
}

static int no_save(void)
{
    return 0;
}

static void pick_title(void)
{
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    wait_steps(SETTLE_STEPS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK(pause_menu_camera_x() == 0.0f);
}

static void test_title_asks_first_and_yes_hands_over_once_the_screen_is_black(void)
{
    int steps;

    start_with_art();
    pause_save_use(no_save, NULL);
    open_until_shown();
    press(INPUT_RIGHT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_TITLE);
    pick_title();
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    CHECK_INT(pause_menu_takes_title_request(), 0);
    press(INPUT_LEFT);
    CHECK_INT(pause_menu_cursor(), PAUSE_ITEM_RESUME);
    press(INPUT_RIGHT);
    pick_title();
    press(INPUT_CIRCLE);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_SHOWING);
    pick_title();
    press(INPUT_RIGHT);
    press(INPUT_CROSS);
    press(INPUT_LEFT);
    press(INPUT_CROSS);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_HIDING);
    CHECK_INT(pause_menu_takes_title_request(), 0);
    for (steps = 0; steps < 200 && pause_menu_phase() != PAUSE_PHASE_BLACK_BEFORE_GAME; steps++) {
        press(0);
    }
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_BLACK_BEFORE_GAME);
    CHECK_INT(pause_menu_takes_title_request(), 1);
    CHECK_INT(pause_menu_takes_title_request(), 0);
}

static void test_title_saves_first_when_yes_is_kept(void)
{
    start_with_art();
    saves_made = 0;
    pause_save_use(fake_save, NULL);
    open_until_shown();
    press(INPUT_RIGHT);
    pick_title();
    press(INPUT_CROSS);
    CHECK_INT(saves_made, 1);
    CHECK_INT(pause_menu_phase(), PAUSE_PHASE_HIDING);
    pause_save_use(NULL, NULL);
}

static void test_every_menu_sprite_lands_on_whole_pixels_through_a_pan(void)
{
    int step;
    int draw;
    int off_grid;

    start_with_art();
    open_until_shown();
    press(INPUT_DOWN);
    press(INPUT_CROSS);
    off_grid = 0;
    for (step = 0; step < SETTLE_STEPS; step++) {
        fake_renderer_clear_draws();
        pause_menu_draw();
        for (draw = 0; draw < fake_renderer_draw_count(); draw++) {
            const struct fake_draw *made;

            made = fake_renderer_draw(draw);
            if (made->screen_area.x != floorf(made->screen_area.x) || made->screen_area.y != floorf(made->screen_area.y)) {
                off_grid = off_grid + 1;
            }
        }
        press(step == SETTLE_STEPS / 2 ? INPUT_CIRCLE : 0);
    }
    CHECK_INT(off_grid, 0);
}

static void test_the_menu_stays_shut_without_its_art(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    texture_cache_init("set/textures", 1u << 20);
    CHECK(!pause_menu_load("set"));
    pause_menu_open();
    CHECK(!pause_menu_is_open());
}

int main(void)
{
    test_select_opens_straight_onto_the_record_and_circle_returns_to_the_game();
    test_a_held_direction_repeats_after_a_pause_but_only_from_a_fresh_press();
    test_a_direction_held_into_the_menu_never_repeats();
    test_the_fidget_dissolves_in_at_the_link_tick_and_back_out_to_it();
    test_start_darkens_the_game_then_shows_the_menu();
    test_resume_hands_the_screen_back_to_the_game();
    test_the_cursor_walks_the_two_columns();
    test_the_left_stick_steps_the_cursor_once_per_push();
    test_a_stick_held_while_start_opens_the_menu_waits_for_a_fresh_push();
    test_tapping_a_label_selects_it_and_lifting_on_it_picks_it();
    test_sliding_off_a_label_before_lifting_picks_nothing();
    test_picking_pans_toward_the_item_side_and_back_fades_the_labels_in();
    test_people_opens_the_notebook_screen_where_l_switches_tabs_and_circle_returns();
    test_title_asks_first_and_yes_hands_over_once_the_screen_is_black();
    test_title_saves_first_when_yes_is_kept();
    test_every_menu_sprite_lands_on_whole_pixels_through_a_pan();
    test_the_menu_stays_shut_without_its_art();
    return test_finish("unit/pause_menu");
}
