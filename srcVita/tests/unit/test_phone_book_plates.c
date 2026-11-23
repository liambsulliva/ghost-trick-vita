#include <stdio.h>
#include <string.h>
#include "phone_book_plates.h"
#include "script_machine.h"
#include "script_flags.h"
#include "script_phone_book.h"
#include "test_support.h"

#define NUMBER_PLATE 32788
#define PARLOR_PLATE 32791
#define ROOM_PLATE 32794
#define JUNKYARD_PLATE 32785
#define LEVEL_LIST 2
#define GO_THERE_PLATE 32866
#define STAY_HERE_PLATE 32869
#define PROMPT_BAR_PLATE 32777
#define PROMPT_TRACE_PLATE 32783
#define FATE_SECTION_FLAG 0x1020u

static const char table[] =
    "sheet ds_phone_book\n"
    "plate 32785 0 0 128 32 -64 -16\n"
    "plate 32786 130 0 128 32 -64 -16\n"
    "plate 32788 260 0 128 32 -64 -16\n"
    "plate 32789 390 0 128 32 -64 -16\n"
    "plate 32791 520 0 128 32 -64 -16\n"
    "plate 32792 650 0 128 32 -64 -16\n"
    "plate 32794 780 0 128 32 -64 -16\n"
    "place 1 32785 32785 32785 20 255\n"
    "place 2 32812 32812 32812 255 255\n"
    "place 22 32788 32791 32794 20 30\n";

static const char traced_table[] =
    "sheet ds_phone_book\n"
    "plate 32777 0 40 256 24 -128 72\n"
    "plate 32783 0 70 192 24 -102 72\n"
    "plate 32785 0 0 128 32 -64 -16\n"
    "plate 32866 0 100 128 32 -64 -16\n"
    "plate 32867 130 100 128 32 -64 -16\n"
    "plate 32869 260 100 128 32 -64 -16\n"
    "plate 32870 390 100 128 32 -64 -16\n"
    "place 1 32785 32785 32785 20 255\n"
    "choice 0 32866\n"
    "choice 1 32869\n"
    "prompt bar 32777\n"
    "prompt where_to 32782\n"
    "prompt trace 32783\n";

static struct script_machine machine;
static const struct script_outputs no_outputs;

static int stored(int level)
{
    return level + 1;
}

static void test_a_table_without_its_sheet_or_plates_is_not_loaded(void)
{
    static const char no_sheet[] = "plate 32785 0 0 128 32 -64 -16\nplace 1 32785 32785 32785 20 255\n";
    static const char no_plates[] = "sheet ds_phone_book\nplace 1 32785 32785 32785 20 255\n";

    CHECK_INT(phone_book_plates_read_text(no_sheet, sizeof(no_sheet) - 1), 0);
    CHECK(!phone_book_plates_is_loaded());
    CHECK_INT(phone_book_plates_read_text(no_plates, sizeof(no_plates) - 1), 0);
    CHECK(!phone_book_plates_is_loaded());
    CHECK_INT(phone_book_plates_read_text(table, sizeof(table) - 1), 3);
    CHECK(phone_book_plates_is_loaded());
}

static void test_a_place_shows_its_number_until_the_note_reaches_each_reveal(void)
{
    phone_book_plates_read_text(table, sizeof(table) - 1);
    CHECK_INT(phone_book_plates_level(22, 0), 0);
    CHECK_INT(phone_book_plates_level(22, stored(19)), 0);
    CHECK_INT(phone_book_plates_level(22, stored(20)), 1);
    CHECK_INT(phone_book_plates_level(22, stored(29)), 1);
    CHECK_INT(phone_book_plates_level(22, stored(30)), 2);
    CHECK_INT(phone_book_plates_level(22, stored(90)), 2);
    CHECK_INT(phone_book_plates_level(2, stored(90)), 0);
    CHECK_INT(phone_book_plates_level(3, stored(90)), -1);
    CHECK_INT(phone_book_plates_level(-1, 0), -1);
    CHECK_INT(phone_book_plates_level(PHONE_BOOK_PLATES_PLACE_MOST, 0), -1);
}

static void test_each_level_and_the_selection_pick_their_own_plate(void)
{
    const struct phone_book_plate *plate;

    phone_book_plates_read_text(table, sizeof(table) - 1);
    plate = phone_book_plates_find(22, 0, 0);
    CHECK(plate != NULL && plate->resource == NUMBER_PLATE);
    plate = phone_book_plates_find(22, 0, 1);
    CHECK(plate != NULL && plate->resource == NUMBER_PLATE + 1);
    plate = phone_book_plates_find(22, stored(20), 0);
    CHECK(plate != NULL && plate->resource == PARLOR_PLATE);
    plate = phone_book_plates_find(22, stored(30), 0);
    CHECK(plate != NULL && plate->resource == ROOM_PLATE);
    CHECK(phone_book_plates_find(22, stored(30), 1) == NULL);
    plate = phone_book_plates_find(1, 0, 1);
    CHECK(plate != NULL && plate->resource == JUNKYARD_PLATE + 1 && plate->x == 130.0f && plate->left == -64.0f && plate->top == -16.0f);
    CHECK(phone_book_plates_find(2, 0, 0) == NULL);
    CHECK(phone_book_plates_find(5, 0, 0) == NULL);
}

static void test_the_view_carries_each_listed_place_s_note_level(void)
{
    struct script_hud_view view;

    memset(&machine, 0, sizeof(machine));
    memset(&view, 0, sizeof(view));
    machine.outputs = &no_outputs;
    machine.note_levels[0][22] = (unsigned char)stored(10);
    machine.note_levels[LEVEL_LIST][22] = (unsigned char)stored(30);
    script_phone_book_open(&machine, 0, 1);
    script_phone_book_fill_view(&machine, &view);
    CHECK_INT(view.phone_book_count, 2);
    CHECK_INT(view.phone_book_places[0], 1);
    CHECK_INT(view.phone_book_note_levels[0], 0);
    CHECK_INT(view.phone_book_places[1], 22);
    CHECK_INT(view.phone_book_note_levels[1], stored(30));
}

static void test_a_traced_call_finds_its_resting_and_selected_plates_and_prompts(void)
{
    const struct phone_book_plate *plate;

    phone_book_plates_read_text(traced_table, sizeof(traced_table) - 1);
    plate = phone_book_plates_find_choice(SCRIPT_PHONE_TRACE_GO_THERE, 0);
    CHECK(plate != NULL && plate->resource == GO_THERE_PLATE);
    plate = phone_book_plates_find_choice(SCRIPT_PHONE_TRACE_GO_THERE, 1);
    CHECK(plate != NULL && plate->resource == GO_THERE_PLATE + 1 && plate->x == 130.0f);
    plate = phone_book_plates_find_choice(SCRIPT_PHONE_TRACE_STAY_HERE, 0);
    CHECK(plate != NULL && plate->resource == STAY_HERE_PLATE);
    plate = phone_book_plates_find_choice(SCRIPT_PHONE_TRACE_STAY_HERE, 1);
    CHECK(plate != NULL && plate->resource == STAY_HERE_PLATE + 1);
    CHECK(phone_book_plates_find_choice(PHONE_BOOK_PLATES_CHOICES, 0) == NULL);
    CHECK(phone_book_plates_find_choice(-1, 0) == NULL);
    plate = phone_book_plates_find_prompt("bar");
    CHECK(plate != NULL && plate->resource == PROMPT_BAR_PLATE && plate->top == 72.0f && plate->width == 256.0f);
    plate = phone_book_plates_find_prompt("trace");
    CHECK(plate != NULL && plate->resource == PROMPT_TRACE_PLATE && plate->left == -102.0f);
    CHECK(phone_book_plates_find_prompt("where_to") == NULL);
    CHECK(phone_book_plates_find_prompt("nothing") == NULL);
}

static void test_an_older_table_has_no_traced_call_plates(void)
{
    phone_book_plates_read_text(table, sizeof(table) - 1);
    CHECK(phone_book_plates_is_loaded());
    CHECK(phone_book_plates_find_choice(SCRIPT_PHONE_TRACE_GO_THERE, 0) == NULL);
    CHECK(phone_book_plates_find_prompt("trace") == NULL);
}

static void test_the_view_of_a_traced_call_lists_no_places(void)
{
    struct script_hud_view view;

    memset(&machine, 0, sizeof(machine));
    memset(&view, 0, sizeof(view));
    machine.outputs = &no_outputs;
    machine.note_levels[0][22] = (unsigned char)stored(10);
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, 1);
    script_phone_book_open(&machine, 0, 22);
    script_phone_book_fill_view(&machine, &view);
    CHECK(view.phone_book_is_up);
    CHECK(view.phone_book_is_traced);
    CHECK_INT(view.phone_book_count, SCRIPT_PHONE_TRACE_CHOICES);
    CHECK_INT(view.phone_book_cursor, SCRIPT_PHONE_TRACE_GO_THERE);
    CHECK_INT(view.phone_book_places[0], 0);
    CHECK_INT(machine.note_levels[0][1], 1);
}

int main(void)
{
    test_a_table_without_its_sheet_or_plates_is_not_loaded();
    test_a_place_shows_its_number_until_the_note_reaches_each_reveal();
    test_each_level_and_the_selection_pick_their_own_plate();
    test_the_view_carries_each_listed_place_s_note_level();
    test_a_traced_call_finds_its_resting_and_selected_plates_and_prompts();
    test_an_older_table_has_no_traced_call_plates();
    test_the_view_of_a_traced_call_lists_no_places();
    return test_finish("unit/phone_book_plates");
}
