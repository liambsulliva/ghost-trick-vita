#include <string.h>
#include "script_narration.h"
#include "test_support.h"

#define TICKS_INTO_THE_DIM 3
#define TICKS_FOR_A_PAGE_TO_SETTLE 2
#define TICKS_FAR_PAST_EVERY_WAIT 2000

static struct script_cue_list cues;
static struct script_narration narration;
static int groups_shown;
static int pages_ended;
static int texts_finished;

static void on_report(void *listener, const char *label, enum script_narration_event event, int group)
{
    (void)listener;
    (void)label;
    (void)group;
    if (event == SCRIPT_NARRATION_GROUP_IN) {
        groups_shown = groups_shown + 1;
    }
    if (event == SCRIPT_NARRATION_TEXT_OUT || event == SCRIPT_NARRATION_CUT) {
        pages_ended = pages_ended + 1;
    }
    if (event == SCRIPT_NARRATION_TEXT_FULL) {
        texts_finished = texts_finished + 1;
    }
}

static void start_a_page_of_three_lines(const char *page_line)
{
    char text[96];

    strcpy(text, "message m01_0001\n");
    strcat(text, page_line);
    strcat(text, "\n");
    CHECK(script_cues_parse(text, (unsigned int)strlen(text), &cues));
    groups_shown = 0;
    pages_ended = 0;
    texts_finished = 0;
    script_narration_start(&narration, &cues, &cues.messages[0], SCRIPT_TIMING_REMASTER);
}

static void step_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_narration_step(&narration, on_report, NULL);
    }
}

static void test_a_press_in_the_fade_in_shows_the_whole_page_then_the_next_press_moves_on(void)
{
    start_a_page_of_three_lines("page 0 0 3 62 62 62");
    step_ticks(TICKS_INTO_THE_DIM);
    CHECK_INT(groups_shown, 0);
    CHECK(script_narration_takes_a_skip(&narration));
    CHECK(!script_narration_wants_a_press(&narration));
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(groups_shown, 3);
    CHECK(script_narration_wants_a_press(&narration));
    CHECK_INT(pages_ended, 0);
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(pages_ended, 1);
}

static void test_a_press_moves_on_from_a_timed_page_without_its_wait(void)
{
    start_a_page_of_three_lines("page 50 0 3 62 62 62");
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(groups_shown, 3);
    CHECK_INT(pages_ended, 0);
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(pages_ended, 1);
}

static void test_a_page_left_alone_still_shows_every_line_in_its_own_time(void)
{
    start_a_page_of_three_lines("page 50 0 3 62 62 62");
    step_ticks(TICKS_FAR_PAST_EVERY_WAIT);
    CHECK_INT(groups_shown, 3);
    CHECK_INT(pages_ended, 1);
}

static void start_a_shown_at_once_page(void)
{
    static const char text[] = "message m01_0002\npage 0 1 2 0 0\n";

    CHECK(script_cues_parse(text, (unsigned int)strlen(text), &cues));
    groups_shown = 0;
    pages_ended = 0;
    texts_finished = 0;
    script_narration_start(&narration, &cues, &cues.messages[0], SCRIPT_TIMING_DS);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
}

static void test_a_press_while_lines_still_fade_in_finishes_them_and_a_second_press_moves_on(void)
{
    start_a_shown_at_once_page();
    CHECK_INT(groups_shown, 2);
    CHECK(!script_narration_wants_a_press(&narration));
    CHECK(script_narration_takes_a_skip(&narration));
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(texts_finished, 1);
    CHECK_INT(pages_ended, 0);
    CHECK(script_narration_wants_a_press(&narration));
    script_narration_note_press(&narration);
    step_ticks(TICKS_FOR_A_PAGE_TO_SETTLE);
    CHECK_INT(pages_ended, 1);
}

static void test_the_arrow_waits_for_the_lines_to_finish_fading_in(void)
{
    start_a_shown_at_once_page();
    CHECK(!script_narration_wants_a_press(&narration));
    step_ticks(SCRIPT_NARRATION_GROUP_IN_TICKS);
    CHECK(script_narration_wants_a_press(&narration));
    CHECK_INT(texts_finished, 0);
    CHECK_INT(pages_ended, 0);
}

int main(void)
{
    test_a_press_in_the_fade_in_shows_the_whole_page_then_the_next_press_moves_on();
    test_a_press_moves_on_from_a_timed_page_without_its_wait();
    test_a_page_left_alone_still_shows_every_line_in_its_own_time();
    test_a_press_while_lines_still_fade_in_finishes_them_and_a_second_press_moves_on();
    test_the_arrow_waits_for_the_lines_to_finish_fading_in();
    return test_finish("unit/narration_skip");
}
