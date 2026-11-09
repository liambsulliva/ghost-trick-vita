#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define OPEN_CHOICE_OP 0x3b
#define CHOICE_PANELS 3
#define FIRST_FLAG 0x1500
#define DOWN 1
#define UP (-1)

static struct script_machine machine;
static const struct script_outputs no_outputs;
static const char *const events[CHOICE_PANELS] = { "first", "second", "third" };

static void open_a_choice_of_three(int offers_cancel)
{
    static const unsigned int no_words[8];
    int index;

    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "linne";
    machine.objects[0].actor.exists = 1;
    for (index = 0; index < CHOICE_PANELS; index++) {
        machine.choice.panels[index].action = SCRIPT_CHOICE_SENDS_EVENT;
        machine.choice.panels[index].flag = FIRST_FLAG + (unsigned int)index;
        machine.choice.panels[index].target = "linne";
        machine.choice.panels[index].kind = "select";
        machine.choice.panels[index].event = events[index];
    }
    machine.choice.panel_count = CHOICE_PANELS;
    machine.choice.panel_limit = CHOICE_PANELS;
    machine.choice.control_is_up = 1;
    machine.choice.cancel_is_offered = offers_cancel;
    script_choice_run_op(&machine, NULL, OPEN_CHOICE_OP, no_words);
}

static void press_direction(int direction_y)
{
    script_soul_note_direction(&machine, 0, direction_y);
    script_soul_step_before_tasks(&machine);
}

static void press_cross(void)
{
    script_machine_note_press(&machine);
    script_soul_step_before_tasks(&machine);
}

static int linne_got(const char *event)
{
    int index;

    for (index = 0; index < machine.objects[0].message_count; index++) {
        if (strcmp(machine.objects[0].messages[index].verb, "select") == 0 && strcmp(machine.objects[0].messages[index].arg, event) == 0) {
            return 1;
        }
    }
    return 0;
}

static void test_cross_picks_the_marked_panel_which_starts_on_the_first(void)
{
    open_a_choice_of_three(0);
    CHECK(machine.choice.is_open);
    CHECK_INT(machine.choice.cursor, 0);
    press_cross();
    CHECK(!machine.choice.is_open);
    CHECK(linne_got("first"));
}

static void test_down_moves_the_mark_and_wraps_to_the_first(void)
{
    open_a_choice_of_three(0);
    press_direction(DOWN);
    CHECK_INT(machine.choice.cursor, 1);
    press_direction(DOWN);
    press_direction(DOWN);
    CHECK_INT(machine.choice.cursor, 0);
    press_direction(DOWN);
    press_cross();
    CHECK(linne_got("second"));
}

static void test_up_from_the_first_wraps_to_the_last(void)
{
    open_a_choice_of_three(0);
    press_direction(UP);
    CHECK_INT(machine.choice.cursor, 2);
    press_cross();
    CHECK(linne_got("third"));
}

static void test_back_cancels_only_when_the_choice_offers_it(void)
{
    open_a_choice_of_three(0);
    script_choice_note_cancel(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK(machine.choice.is_open);
    open_a_choice_of_three(1);
    script_choice_note_cancel(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK(!machine.choice.is_open);
    CHECK(linne_got("cancel"));
    CHECK_INT(machine.objects[0].message_count, 1);
}

int main(void)
{
    test_cross_picks_the_marked_panel_which_starts_on_the_first();
    test_down_moves_the_mark_and_wraps_to_the_first();
    test_up_from_the_first_wraps_to_the_last();
    test_back_cancels_only_when_the_choice_offers_it();
    return test_finish("unit/choice_input");
}
