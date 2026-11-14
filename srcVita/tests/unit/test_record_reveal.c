#include <string.h>
#include "fakes/fake_platform.h"
#include "game_save.h"
#include "input.h"
#include "pause_people.h"
#include "record_reveal.h"
#include "script_machine.h"
#include "test_support.h"

static const char table[] =
    "order 3 1\n"
    "first_people 3 1\n"
    "place_count 1\n"
    "face 1 pause_face_01\n"
    "face 3 pause_face_00\n"
    "face 6 pause_face_00\n"
    "reveal 1 note 1 3 120\n"
    "reveal 2 talk m13_2160\n"
    "reveal_face 1 3 pause_face_34\n"
    "reveal_face 2 3 pause_face_cat\n"
    "thumb pause_face_00 0 0 104 78\n"
    "thumb frame 0 92 116 90\n"
    "note 1 3 10 pause_note_1_03_010\n"
    "note 1 1 10 pause_note_1_01_010\n";

static unsigned char notes[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];

static void start(void)
{
    fake_files_reset();
    memset(notes, 0, sizeof(notes));
    notes[1][3] = 11;
    notes[1][1] = 11;
    CHECK(pause_people_read_text(table, (unsigned int)strlen(table)) > 0);
    CHECK_INT(record_reveal_load(RECORD_REVEAL_PATH), 0);
    pause_people_use_notes(&notes[0][0]);
    pause_people_enter();
}

static const char *sissel_face(void)
{
    pause_people_enter();
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, 0), 3);
    return pause_people_art();
}

static void test_sissel_keeps_the_red_suit_until_the_record_says_he_is_not_that_man(void)
{
    start();
    notes[1][3] = 120;
    CHECK_INT(record_reveal_watch(NULL, &notes[0][0], RECORD_REVEAL_PATH), 0);
    CHECK_TEXT(sissel_face(), "pause_face_00");
    notes[1][3] = 121;
    CHECK_INT(record_reveal_watch(NULL, &notes[0][0], RECORD_REVEAL_PATH), 1);
    CHECK_INT(record_reveal_stage(), 1);
    CHECK_TEXT(sissel_face(), "pause_face_34");
}

static void test_the_cat_talk_shows_him_as_a_cat_and_others_keep_their_faces(void)
{
    start();
    CHECK_INT(record_reveal_watch("m13_2155", &notes[0][0], RECORD_REVEAL_PATH), 0);
    CHECK_INT(record_reveal_watch("m13_2160", &notes[0][0], RECORD_REVEAL_PATH), 1);
    CHECK_INT(record_reveal_stage(), 2);
    CHECK_TEXT(sissel_face(), "pause_face_cat");
    notes[1][3] = 121;
    CHECK_INT(record_reveal_watch(NULL, &notes[0][0], RECORD_REVEAL_PATH), 0);
    CHECK_INT(record_reveal_stage(), 2);
    pause_people_enter();
    CHECK_INT(pause_people_step(INPUT_RIGHT) == PAUSE_PEOPLE_STAYS, 1);
    CHECK_INT(pause_people_entry(PAUSE_PEOPLE_TAB_PEOPLE, pause_people_selected()), 1);
    CHECK_TEXT(pause_people_art(), "pause_face_01");
}

static void test_the_reveal_outlives_older_saves_and_only_erase_forgets_it(void)
{
    start();
    CHECK_INT(record_reveal_watch("m13_2160", &notes[0][0], RECORD_REVEAL_PATH), 1);
    CHECK(fake_files_exists(RECORD_REVEAL_PATH));
    memset(notes, 0, sizeof(notes));
    notes[1][3] = 11;
    CHECK_INT(record_reveal_load(RECORD_REVEAL_PATH), 2);
    CHECK_INT(record_reveal_watch(NULL, &notes[0][0], RECORD_REVEAL_PATH), 0);
    CHECK_TEXT(sissel_face(), "pause_face_cat");
    (void)game_save_erase_all();
    CHECK_INT(record_reveal_stage(), 0);
    CHECK(!fake_files_exists(RECORD_REVEAL_PATH));
    CHECK_INT(record_reveal_load(RECORD_REVEAL_PATH), 0);
    CHECK_TEXT(sissel_face(), "pause_face_00");
}

int main(void)
{
    test_sissel_keeps_the_red_suit_until_the_record_says_he_is_not_that_man();
    test_the_cat_talk_shows_him_as_a_cat_and_others_keep_their_faces();
    test_the_reveal_outlives_older_saves_and_only_erase_forgets_it();
    return test_finish("unit/record_reveal");
}
