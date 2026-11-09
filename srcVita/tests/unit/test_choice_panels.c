#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "test_support.h"

#define MARK_MAX 64
#define TICKS_TO_RUN 200
#define WORD_MAX 64

static struct script_machine machine;
static struct script_route_pad route_pad;
static struct script_input route_input;

static void drive_one_tick(void)
{
    struct input_state frame;

    if (!machine.route_is_driving) {
        return;
    }
    script_route_pad_frame(&route_pad, &machine, &frame);
    script_input_frame(&route_input, &machine, &frame);
}

static struct script_route route;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 512];

static const char strings[] = "\0\0game\0EVENT\0panel1_1\0panel2_1\0cap_one\0cap_two\0cap_three\0talk_three\0";

static void on_marked(const char *what, const char *name)
{
    if (mark_count < MARK_MAX) {
        snprintf(marks[mark_count], sizeof(marks[mark_count]), "%s %s", what, name);
        mark_count = mark_count + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static int was_marked(const char *text)
{
    int index;

    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            return 1;
        }
    }
    return 0;
}

static unsigned int offset_of(const char *text)
{
    unsigned int offset;

    for (offset = 0; offset < sizeof(strings) - 1; offset++) {
        if (strcmp(&strings[offset], text) == 0) {
            return offset;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static unsigned int add_event_panel(unsigned int *words, unsigned int at, const char *caption, unsigned int flag, const char *event)
{
    words[at] = 0x40;
    words[at + 1] = offset_of(caption);
    words[at + 2] = 0;
    words[at + 3] = flag;
    words[at + 4] = 0;
    words[at + 5] = offset_of("game");
    words[at + 6] = offset_of("EVENT");
    words[at + 7] = offset_of(event);
    return at + 8;
}

static unsigned int add_turned_talk_panel(unsigned int *words, unsigned int at, unsigned int index, unsigned int flag)
{
    words[at] = 0x3d;
    words[at + 1] = index;
    words[at + 2] = offset_of("cap_three");
    words[at + 3] = 0;
    words[at + 4] = offset_of("talk_three");
    words[at + 5] = 0;
    words[at + 6] = flag;
    words[at + 7] = 0;
    return at + 8;
}

#define NO_TURNED_PANEL -1

static void start_scene(int auto_press, int turned_panel_index, int with_cancel, const char *route_choice)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = 0x3a;
    words[count++] = 0x3e;
    count = add_event_panel(words, count, "cap_one", 2, "panel1_1");
    count = add_event_panel(words, count, "cap_two", 3, "panel2_1");
    if (turned_panel_index != NO_TURNED_PANEL) {
        count = add_turned_talk_panel(words, count, (unsigned int)turned_panel_index, 5);
    }
    if (with_cancel) {
        words[count++] = 0x42;
    }
    words[count++] = 0x3b;
    words[count++] = 0x39;
    words[count++] = 0x3e;
    words[count++] = 0x2f;
    words[count++] = 6000;
    words[count++] = 0x0;
    mark_count = 0;
    memset(&route, 0, sizeof(route));
    if (route_choice != NULL) {
        snprintf(route.steps[0].script, sizeof(route.steps[0].script), "a");
        route.steps[0].action = SCRIPT_ROUTE_CHOOSE;
        snprintf(route.steps[0].object, sizeof(route.steps[0].object), "%s", route_choice);
        snprintf(route.steps[0].wait, sizeof(route.steps[0].wait), "choice");
        route.step_count = 1;
    }
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = auto_press != SCRIPT_AUTO_OFF;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        drive_one_tick();
        script_machine_tick(&machine);
    }
}

static int flag_is_set(unsigned int flag)
{
    int value;

    return script_flags_read_known(&machine.flags, flag, &value) && value;
}

static void test_auto_play_picks_the_first_unseen_panel_and_sets_its_flag(void)
{
    start_scene(SCRIPT_AUTO_PACED, NO_TURNED_PANEL, 0, NULL);
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_opened panel1_1,panel2_1"));
    CHECK(was_marked("choice_picked panel1_1"));
    CHECK(flag_is_set(2));
    CHECK(!flag_is_set(3));
    CHECK_INT(machine.choice.control_is_up, 0);
}

static void test_a_panel_already_seen_is_passed_over(void)
{
    start_scene(SCRIPT_AUTO_PACED, NO_TURNED_PANEL, 0, NULL);
    script_flags_write_known(&machine.flags, 2, 1);
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_picked panel2_1"));
    CHECK(flag_is_set(3));
}

static void test_a_route_step_chooses_by_event_name(void)
{
    start_scene(SCRIPT_AUTO_PACED, NO_TURNED_PANEL, 0, "panel2_1");
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_picked panel2_1"));
    CHECK(!was_marked("choice_picked_without_route a"));
    CHECK(was_marked("route_step_done 0:choose:panel2_1:0"));
}

static void test_the_choice_holds_the_script_until_a_panel_is_picked(void)
{
    start_scene(SCRIPT_AUTO_OFF, NO_TURNED_PANEL, 0, NULL);
    run_ticks(TICKS_TO_RUN);
    CHECK_INT(machine.choice.is_open, 1);
    CHECK(!script_soul_world_is_playable(&machine));
    script_machine_note_press(&machine);
    run_ticks(2);
    CHECK(was_marked("choice_picked panel1_1"));
    CHECK_INT(machine.choice.is_open, 0);
}

static void test_a_turned_panel_shows_at_its_index(void)
{
    start_scene(SCRIPT_AUTO_PACED, 2, 0, "cap_three");
    run_ticks(3);
    CHECK_INT(script_choice_panels_shown(&machine), 3);
    CHECK_INT(script_choice_find(&machine, "talk_three"), 2);
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_picked talk_three"));
    CHECK(flag_is_set(5));
}

static void test_a_turned_panel_shows_over_the_panel_at_its_index(void)
{
    start_scene(SCRIPT_AUTO_PACED, 1, 0, "cap_three");
    run_ticks(3);
    CHECK_INT(script_choice_panels_shown(&machine), 2);
    CHECK_INT(script_choice_find(&machine, "talk_three"), 1);
    CHECK_INT(script_choice_find(&machine, "panel2_1"), SCRIPT_CHOICE_NONE);
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_picked talk_three"));
    CHECK(flag_is_set(5));
}

static void test_cancel_needs_the_cancel_panel(void)
{
    start_scene(SCRIPT_AUTO_PACED, NO_TURNED_PANEL, 0, NULL);
    run_ticks(3);
    CHECK_INT(script_choice_find(&machine, "cancel"), SCRIPT_CHOICE_NONE);
    start_scene(SCRIPT_AUTO_PACED, NO_TURNED_PANEL, 1, "cancel");
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("choice_picked cancel"));
    CHECK(!flag_is_set(2));
}

static void test_a_scene_change_clears_the_panels(void)
{
    start_scene(SCRIPT_AUTO_OFF, NO_TURNED_PANEL, 0, NULL);
    run_ticks(3);
    CHECK_INT(machine.choice.is_open, 1);
    CHECK(script_machine_start(&machine, "a"));
    CHECK(was_marked("scene_leftover_cleared choice"));
    CHECK_INT(script_choice_panels_shown(&machine), 0);
}

int main(void)
{
    test_auto_play_picks_the_first_unseen_panel_and_sets_its_flag();
    test_a_panel_already_seen_is_passed_over();
    test_a_route_step_chooses_by_event_name();
    test_the_choice_holds_the_script_until_a_panel_is_picked();
    test_a_turned_panel_shows_at_its_index();
    test_a_turned_panel_shows_over_the_panel_at_its_index();
    test_cancel_needs_the_cancel_panel();
    test_a_scene_change_clears_the_panels();
    return test_finish("unit/choice_panels");
}
