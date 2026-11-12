#include <string.h>
#include "input.h"
#include "pause_people.h"
#include "script_machine.h"
#include "test_support.h"

static const char table[] =
    "order 3 1 8 2 10 11 12 13 14\n"
    "skip_people 2 4\n"
    "skip_places 2 3\n"
    "first_people 3 1\n"
    "first_places 1\n"
    "place_count 5\n"
    "face 1 pause_face_01\n"
    "face 3 pause_face_00\n"
    "place 1 pause_place_00\n"
    "place 4 pause_place_01\n"
    "thumb pause_face_00 0 0 104 78\n"
    "thumb unknown 120 0 104 78\n"
    "thumb frame 0 92 116 90\n"
    "note 1 3 10 pause_note_1_03_010\n"
    "note 1 3 40 pause_note_1_03_040\n"
    "note 1 1 20 pause_note_1_01_020\n"
    "note 2 1 10 pause_note_2_01_010\n"
    "note 2 4 30 pause_note_2_04_030\n";

static unsigned char notes[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];

static void know(int list, int id, int level)
{
    notes[list][id] = (unsigned char)(level + 1);
}

static void start_people(void)
{
    memset(notes, 0, sizeof(notes));
    CHECK_INT(pause_people_read_text(table, (unsigned int)strlen(table)), 5);
    pause_people_use_notes(&notes[0][0]);
}

static void test_the_table_needs_order_cards_and_thumbs(void)
{
    static const char empty[] = "order 3 1\n";

    CHECK_INT(pause_people_read_text(empty, (unsigned int)strlen(empty)), 0);
    CHECK_INT(pause_people_is_loaded(), 0);
    start_people();
    CHECK_INT(pause_people_is_loaded(), 1);
}

static void test_people_follow_the_order_table_and_skip_unknown_and_variant_ids(void)
{
    start_people();
    know(1, 1, 20);
    know(1, 2, 10);
    know(1, 3, 40);
    know(1, 13, 10);
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 3);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 0), 3);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 1), 1);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 2), 13);
    CHECK_INT(pause_people_level(PAUSE_PEOPLE_TAB_PEOPLE, 0), 40);
    CHECK_INT(pause_people_level(PAUSE_PEOPLE_TAB_PEOPLE, 1), 20);
}

static void test_places_run_in_id_order_and_skip_listed_ones(void)
{
    start_people();
    know(2, 1, 10);
    know(2, 2, 10);
    know(2, 4, 30);
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PLACES), 2);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PLACES, 0), 1);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PLACES, 1), 4);
}

static void test_an_empty_tab_shows_the_first_entries_at_level_ten(void)
{
    start_people();
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 2);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 0), 3);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 1), 1);
    CHECK_INT(pause_people_level(PAUSE_PEOPLE_TAB_PEOPLE, 1), 10);
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PLACES), 1);
    pause_people_use_notes(NULL);
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 2);
}

static void test_every_note_lists_all_unskipped_entries_at_their_highest_card(void)
{
    start_people();
    pause_people_use_notes(pause_people_every_note());
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 8);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 0), 3);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 2), 8);
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PLACES), 3);
    CHECK_TEXT(pause_people_card(), "pause_note_1_03_040");
    pause_people_use_notes(&notes[0][0]);
}

static void test_the_card_is_the_highest_level_baked_at_or_below_the_known_one(void)
{
    start_people();
    know(1, 3, 30);
    know(1, 1, 10);
    pause_people_enter();
    CHECK_TEXT(pause_people_card(), "pause_note_1_03_010");
    CHECK_TEXT(pause_people_art(), "pause_face_00");
    pause_people_step(INPUT_RIGHT);
    CHECK_TEXT(pause_people_card(), "pause_note_1_01_020");
    CHECK_TEXT(pause_people_art(), "pause_face_01");
}

static void test_left_right_move_within_the_tab_and_l_r_switch_tabs(void)
{
    int index;

    start_people();
    for (index = 1; index <= 14; index++) {
        know(1, index, 10);
    }
    know(2, 4, 30);
    pause_people_enter();
    CHECK_INT(pause_people_count(PAUSE_PEOPLE_TAB_PEOPLE), 8);
    pause_people_step(INPUT_LEFT);
    CHECK_INT(pause_people_selected(), 0);
    for (index = 0; index < 10; index++) {
        pause_people_step(INPUT_RIGHT);
    }
    CHECK_INT(pause_people_selected(), 7);
    CHECK_INT(pause_people_first_slot(), 1);
    CHECK_INT(pause_people_step(INPUT_R), PAUSE_PEOPLE_STAYS);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PLACES);
    CHECK_INT(pause_people_selected(), 0);
    CHECK_TEXT(pause_people_card(), "pause_note_2_04_030");
    CHECK_TEXT(pause_people_art(), "pause_place_01");
    pause_people_step(INPUT_L);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PEOPLE);
    CHECK_INT(pause_people_selected(), 7);
    CHECK_INT(pause_people_step(INPUT_CIRCLE), PAUSE_PEOPLE_LEAVES);
}

static void test_touch_picks_tabs_thumbs_and_the_back_pill(void)
{
    start_people();
    know(1, 3, 10);
    know(1, 1, 20);
    pause_people_enter();
    CHECK_INT(pause_people_touch(1, 200.0f, 100.0f), 0u);
    CHECK_INT(pause_people_selected(), 1);
    CHECK_INT(pause_people_touch(1, 830.0f, 100.0f), 0u);
    CHECK_INT(pause_people_selected(), 1);
    CHECK_INT(pause_people_touch(0, 600.0f, 30.0f), 0u);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PEOPLE);
    CHECK_INT(pause_people_touch(1, 600.0f, 30.0f), 0u);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PLACES);
    CHECK_INT(pause_people_touch(1, 262.0f, 32.0f), 0u);
    CHECK_INT(pause_people_tab(), PAUSE_PEOPLE_TAB_PEOPLE);
    CHECK_INT(pause_people_touch(1, 90.0f, 518.0f), INPUT_CIRCLE);
}

int main(void)
{
    test_the_table_needs_order_cards_and_thumbs();
    test_people_follow_the_order_table_and_skip_unknown_and_variant_ids();
    test_places_run_in_id_order_and_skip_listed_ones();
    test_every_note_lists_all_unskipped_entries_at_their_highest_card();
    test_an_empty_tab_shows_the_first_entries_at_level_ten();
    test_the_card_is_the_highest_level_baked_at_or_below_the_known_one();
    test_left_right_move_within_the_tab_and_l_r_switch_tabs();
    test_touch_picks_tabs_thumbs_and_the_back_pill();
    return test_finish("unit/pause_people");
}
