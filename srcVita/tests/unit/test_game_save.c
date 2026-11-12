#include <string.h>
#include "fakes/fake_platform.h"
#include "game_save.h"
#include "input.h"
#include "pause_chapters.h"
#include "pause_save.h"
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static struct game_save save;
static struct game_save read_back;
static char text[GAME_SAVE_TEXT_MAX];
static int save_calls;
static int save_result;

static int fake_save_now(void)
{
    save_calls = save_calls + 1;
    return save_result;
}

static int fake_slot_has_data(int slot)
{
    return slot == PAUSE_SAVE_AUTO_PANEL ? 2 : PAUSE_SAVE_NO_DATA;
}

static void fill_machine(void)
{
    memset(&machine, 0, sizeof(machine));
    machine.chapter_mark = 3;
    machine.phone_book_place = 2;
    machine.flags.game[5] = 0x81;
    machine.flags.chapter[7] = 0x40;
    machine.flags.game_bank_saved[1] = 0x22;
    machine.note_levels[1][3] = 41;
    machine.note_levels[2][1] = 11;
    machine.soul.pills_registered = 16;
    machine.soul.trick_is_on = 1;
}

static void test_a_save_survives_text_and_restores_the_machine(void)
{
    unsigned int size;

    fill_machine();
    game_save_capture(&save, &machine, "st03-st03_root");
    save.serial = 7;
    size = game_save_write_text(&save, text, sizeof(text));
    CHECK(size > 0);
    CHECK_INT(game_save_read_text(&read_back, text, size), 1);
    CHECK_TEXT(read_back.script, "st03-st03_root");
    CHECK_INT((int)read_back.serial, 7);
    CHECK_INT(read_back.chapter_mark, 3);
    CHECK_INT(read_back.note_levels[1][3], 41);
    CHECK_INT(read_back.has_soul_state, 1);
    memset(&machine, 0, sizeof(machine));
    game_save_apply_before_start(&read_back, &machine);
    CHECK_INT(machine.chapter_mark, 3);
    CHECK_INT(machine.phone_book_place, 2);
    CHECK_INT(machine.flags.game[5], 0x81);
    CHECK_INT(machine.flags.chapter[7], 0x40);
    CHECK_INT(machine.flags.game_bank_saved[1], 0x22);
    CHECK_INT(machine.note_levels[2][1], 11);
    CHECK_INT((int)machine.soul.pills_registered, 0);
    game_save_apply_after_start(&read_back, &machine);
    CHECK_INT((int)machine.soul.pills_registered, 16);
    CHECK_INT(machine.soul.trick_is_on, 1);
}

static void test_a_save_without_flags_or_cut_short_is_refused(void)
{
    static const char no_flags[] = "script st01\nchapter_mark 1\n";
    static const char short_flags[] = "script st01\ngame_flags 00ff\n";

    CHECK_INT(game_save_read_text(&read_back, no_flags, (unsigned int)strlen(no_flags)), 0);
    CHECK_INT(game_save_read_text(&read_back, short_flags, (unsigned int)strlen(short_flags)), 0);
    CHECK_INT((int)game_save_write_text(&save, text, 16), 0);
}

static void test_the_newest_slot_wins_and_each_store_counts_up(void)
{
    fake_files_reset();
    CHECK_INT(game_save_load_newest(&read_back), 0);
    fill_machine();
    game_save_capture(&save, &machine, "first");
    CHECK_INT(game_save_store_as_newest(&save, GAME_SAVE_MANUAL), 1);
    game_save_capture(&save, &machine, "second");
    CHECK_INT(game_save_store_as_newest(&save, GAME_SAVE_AUTO), 1);
    CHECK_INT(game_save_load_newest(&read_back), 1);
    CHECK_TEXT(read_back.script, "second");
    CHECK_INT((int)read_back.serial, 2);
    game_save_capture(&save, &machine, "third");
    CHECK_INT(game_save_store_as_newest(&save, GAME_SAVE_MANUAL), 1);
    CHECK_INT(game_save_load_newest(&read_back), 1);
    CHECK_TEXT(read_back.script, "third");
    CHECK_INT(game_save_erase_all(), 1);
    CHECK_INT(game_save_load_newest(&read_back), 0);
}

static void test_the_save_screen_asks_then_saves_and_yes_resumes_play(void)
{
    pause_save_use(fake_save_now, fake_slot_has_data);
    save_calls = 0;
    save_result = 1;
    pause_save_enter();
    CHECK_INT(pause_save_selected(), PAUSE_SAVE_MANUAL_PANEL);
    pause_save_step(INPUT_UP);
    CHECK_INT(pause_save_step(INPUT_CROSS), PAUSE_SAVE_STAYS);
    CHECK_INT(pause_save_dialog_is_open(), 0);
    pause_save_step(INPUT_DOWN);
    pause_save_step(INPUT_CROSS);
    CHECK_INT(pause_save_dialog_is_open(), 1);
    CHECK_INT(save_calls, 0);
    pause_save_step(INPUT_CROSS);
    CHECK_INT(save_calls, 1);
    CHECK_INT(pause_save_dialog_is_open(), 1);
    CHECK_INT(pause_save_step(INPUT_CROSS), PAUSE_SAVE_RESUMES);
    CHECK_INT(pause_save_step(INPUT_CIRCLE), PAUSE_SAVE_LEAVES);
}

static void test_no_or_a_failed_save_keeps_the_save_screen(void)
{
    pause_save_use(fake_save_now, fake_slot_has_data);
    save_calls = 0;
    save_result = 0;
    pause_save_enter();
    pause_save_step(INPUT_CROSS);
    pause_save_step(INPUT_RIGHT);
    pause_save_step(INPUT_CROSS);
    CHECK_INT(save_calls, 0);
    CHECK_INT(pause_save_dialog_is_open(), 0);
    pause_save_step(INPUT_CROSS);
    pause_save_step(INPUT_CROSS);
    CHECK_INT(save_calls, 1);
    CHECK_INT(pause_save_dialog_is_open(), 1);
    CHECK_INT(pause_save_step(INPUT_CROSS), PAUSE_SAVE_STAYS);
    CHECK_INT(pause_save_dialog_is_open(), 0);
}

static int fake_chapter_reached(int chapter)
{
    return chapter == 1 || chapter == 2;
}

static void test_chapter_slots_store_by_number_and_erase_with_the_rest(void)
{
    fake_files_reset();
    fill_machine();
    game_save_capture(&save, &machine, "st02-st02_root");
    CHECK_INT(game_save_store_chapter(&save, 4), 1);
    CHECK_INT(game_save_store_chapter(&save, 19), 0);
    CHECK_INT(game_save_load_chapter(&read_back, 4), 1);
    CHECK_TEXT(read_back.script, "st02-st02_root");
    CHECK_INT(game_save_load_chapter(&read_back, 3), 0);
    CHECK_INT(game_save_erase_all(), 1);
    CHECK_INT(game_save_load_chapter(&read_back, 4), 0);
}

static void test_the_chapters_grid_plays_only_reached_chapters_after_asking(void)
{
    pause_chapters_use(fake_chapter_reached);
    pause_chapters_enter();
    CHECK_INT(pause_chapters_selected(), 2);
    pause_chapters_step(INPUT_RIGHT);
    CHECK_INT(pause_chapters_selected(), 3);
    pause_chapters_step(INPUT_RIGHT);
    CHECK_INT(pause_chapters_selected(), 3);
    pause_chapters_step(INPUT_CROSS);
    CHECK_INT(pause_chapters_dialog_is_open(), 0);
    pause_chapters_step(INPUT_DOWN);
    CHECK_INT(pause_chapters_selected(), 6);
    pause_chapters_step(INPUT_UP);
    pause_chapters_step(INPUT_LEFT);
    pause_chapters_step(INPUT_LEFT);
    CHECK_INT(pause_chapters_selected(), 1);
    pause_chapters_step(INPUT_LEFT);
    CHECK_INT(pause_chapters_selected(), 1);
    pause_chapters_step(INPUT_CROSS);
    CHECK_INT(pause_chapters_dialog_is_open(), 1);
    CHECK_INT(pause_chapters_step(INPUT_CROSS), PAUSE_CHAPTERS_STAYS);
    pause_chapters_step(INPUT_CROSS);
    pause_chapters_step(INPUT_LEFT);
    CHECK_INT(pause_chapters_step(INPUT_CROSS), PAUSE_CHAPTERS_PLAYS);
    CHECK_INT(pause_chapters_step(INPUT_CIRCLE), PAUSE_CHAPTERS_LEAVES);
    CHECK_INT(pause_chapters_touch(1, 0, 60.0f + 290.0f + 10.0f, 115.0f), 0u);
    CHECK_INT(pause_chapters_selected(), 2);
    CHECK_INT(pause_chapters_touch(1, 0, 60.0f + 290.0f + 10.0f, 115.0f), INPUT_CROSS);
}

int main(void)
{
    test_a_save_survives_text_and_restores_the_machine();
    test_a_save_without_flags_or_cut_short_is_refused();
    test_the_newest_slot_wins_and_each_store_counts_up();
    test_the_save_screen_asks_then_saves_and_yes_resumes_play();
    test_no_or_a_failed_save_keeps_the_save_screen();
    test_chapter_slots_store_by_number_and_erase_with_the_rest();
    test_the_chapters_grid_plays_only_reached_chapters_after_asking();
    return test_finish("unit/game_save");
}
