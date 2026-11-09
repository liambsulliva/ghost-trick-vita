#include <string.h>
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3

static struct script_machine machine;
static struct script_outputs outputs;
static char last_mark[64];
static char last_detail[64];

static void remember_mark(const char *what, const char *name)
{
    strncpy(last_mark, what, sizeof(last_mark) - 1);
    strncpy(last_detail, name, sizeof(last_detail) - 1);
}

static void start_in_normal_play_before_any_pill_is_up(int auto_press)
{
    memset(&outputs, 0, sizeof(outputs));
    outputs.marked = remember_mark;
    memset(last_mark, 0, sizeof(last_mark));
    memset(last_detail, 0, sizeof(last_detail));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "body";
    machine.objects[0].actor.exists = 1;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    machine.auto_press = auto_press;
    script_soul_set_first_core(&machine, 0, 0);
}

static void test_auto_play_marks_a_ghost_press_made_before_its_pill_is_up(void)
{
    start_in_normal_play_before_any_pill_is_up(SCRIPT_AUTO_PACED);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_GHOST));
    script_soul_note_ghost(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK_TEXT(last_mark, "acted_before_pill");
    CHECK(strncmp(last_detail, "pill=ghost", 10) == 0);
}

static void test_a_player_press_before_the_pill_is_up_does_nothing(void)
{
    start_in_normal_play_before_any_pill_is_up(SCRIPT_AUTO_OFF);
    script_soul_note_ghost(&machine);
    script_soul_step_before_tasks(&machine);
    CHECK(!machine.soul.is_on);
    CHECK(strcmp(last_mark, "acted_before_pill") != 0);
}

int main(void)
{
    test_auto_play_marks_a_ghost_press_made_before_its_pill_is_up();
    test_a_player_press_before_the_pill_is_up_does_nothing();
    return test_finish("unit/acted_before_pill");
}
