#include <string.h>
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_cue_list cues;

static void start_two_pages_of_two_paragraphs(void)
{
    static const char text[] = "message m01_0001\npage 0 0 2 62 62\npage 0 0 2 62 62\n";

    CHECK(script_cues_parse(text, (unsigned int)strlen(text), &cues));
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "game";
    script_narration_start(&machine.narration, &cues, &cues.messages[0], SCRIPT_TIMING_REMASTER);
}

static const char *last_monologue_event(void)
{
    const struct script_object *object;

    object = &machine.objects[0];
    if (object->message_count == 0 || strcmp(object->messages[object->message_count - 1].verb, "monologue") != 0) {
        return "";
    }
    return object->messages[object->message_count - 1].arg;
}

static void test_paragraphs_are_numbered_within_their_page(void)
{
    start_two_pages_of_two_paragraphs();
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_GROUP_IN, 1);
    CHECK_TEXT(last_monologue_event(), "paragraph1");
    machine.narration.page = 1;
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_GROUP_IN, 2);
    CHECK_TEXT(last_monologue_event(), "paragraph0");
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_GROUP_IN, 3);
    CHECK_TEXT(last_monologue_event(), "paragraph1");
}

static void test_a_page_that_ends_feeds_the_next_and_the_last_says_before_end(void)
{
    start_two_pages_of_two_paragraphs();
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_TEXT_OUT, 0);
    CHECK_TEXT(last_monologue_event(), "pagefeed");
    machine.narration.page = 1;
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_TEXT_OUT, 0);
    CHECK_TEXT(last_monologue_event(), "before end");
}

static void test_a_page_cut_at_its_end_sends_no_page_event(void)
{
    int before;

    start_two_pages_of_two_paragraphs();
    machine.narration.page = 1;
    before = machine.objects[0].message_count;
    script_narration_report_to_outputs(&machine, "m01_0001", SCRIPT_NARRATION_CUT, 0);
    CHECK_INT(machine.objects[0].message_count, before);
}

int main(void)
{
    test_a_page_cut_at_its_end_sends_no_page_event();
    test_a_page_that_ends_feeds_the_next_and_the_last_says_before_end();
    test_paragraphs_are_numbered_within_their_page();
    return test_finish("unit/monologue_events");
}
