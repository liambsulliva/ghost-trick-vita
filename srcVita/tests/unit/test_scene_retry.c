#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "script_input.h"
#include "script_route_pad.h"
#include "test_support.h"

#define LONG_WAIT_TICKS 60000
#define TICKS_TO_RUN 400
#define MARK_MAX 64

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
static unsigned char waiting_scene[SCRIPT_FILE_HEADER_SIZE + 64];
static unsigned char next_scene[SCRIPT_FILE_HEADER_SIZE + 64];
static unsigned char failing_scene[SCRIPT_FILE_HEADER_SIZE + 64];

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

static int was_marked_starting_with(const char *prefix)
{
    int index;

    for (index = 0; index < mark_count; index++) {
        if (strncmp(marks[index], prefix, strlen(prefix)) == 0) {
            return 1;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void put_u32(unsigned char *at, unsigned int value)
{
    put_u16(at, value & 0xffffu);
    put_u16(at + 2, value >> 16);
}

static unsigned int make_script(unsigned char *bytes, unsigned int capacity, const unsigned int *words, unsigned int word_count, const char *strings, unsigned int string_size)
{
    unsigned int index;
    unsigned int code_size;

    memset(bytes, 0, capacity);
    memcpy(bytes, "1LMG", 4);
    code_size = word_count * 2;
    put_u32(&bytes[0x08], code_size);
    put_u32(&bytes[0x0c], string_size);
    put_u32(&bytes[0x10], 0);
    for (index = 0; index < word_count; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&bytes[SCRIPT_FILE_HEADER_SIZE + code_size], strings, string_size);
    return SCRIPT_FILE_HEADER_SIZE + code_size + string_size;
}

static void start_in(const char *first_scene, int auto_press)
{
    static const unsigned int waiting_words[] = { 0x2f, LONG_WAIT_TICKS, 0x24, 0, 0x0 };
    static const unsigned int next_words[] = { 0x2f, LONG_WAIT_TICKS, 0x0 };
    unsigned int size;

    mark_count = 0;
    memset(&route, 0, sizeof(route));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = auto_press != SCRIPT_AUTO_OFF;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    size = make_script(waiting_scene, sizeof(waiting_scene), waiting_words, sizeof(waiting_words) / sizeof(waiting_words[0]), "\0\0", 2);
    CHECK(script_machine_add_source(&machine, "a", waiting_scene, size, NULL));
    size = make_script(next_scene, sizeof(next_scene), next_words, sizeof(next_words) / sizeof(next_words[0]), "\0\0", 2);
    CHECK(script_machine_add_source(&machine, "b", next_scene, size, NULL));
    CHECK(script_machine_start(&machine, first_scene));
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        drive_one_tick();
        script_machine_tick(&machine);
    }
}

static void test_auto_play_never_moves_a_waiting_scene_on(void)
{
    start_in("a", SCRIPT_AUTO_PACED);
    run_ticks(TICKS_TO_RUN * 50);
    CHECK(!was_marked("script_begun b"));
    CHECK(!was_marked_starting_with("stuck_"));
    CHECK(!was_marked_starting_with("scene_timeout_"));
    CHECK(!was_marked_starting_with("stall_"));
}

static void test_a_press_left_from_the_earlier_scene_is_cleared(void)
{
    start_in("a", SCRIPT_AUTO_PACED);
    script_machine_note_press(&machine);
    CHECK(script_machine_start(&machine, "b"));
    CHECK(was_marked("scene_leftover_cleared press"));
    CHECK_INT(machine.press_is_pending, 0);
}

static int count_marks(const char *text)
{
    int index;
    int count;

    count = 0;
    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            count = count + 1;
        }
    }
    return count;
}

static void start_a_scene_that_fails(void)
{
    static const unsigned int failing_words[] = { 0x2f, 50, 0xdb, 2, 0, 0, 0x2f, LONG_WAIT_TICKS, 0x0 };
    unsigned int size;

    mark_count = 0;
    memset(&route, 0, sizeof(route));
    snprintf(route.steps[0].script, sizeof(route.steps[0].script), "fail");
    route.steps[0].action = SCRIPT_ROUTE_WAIT;
    snprintf(route.steps[0].wait, sizeof(route.steps[0].wait), "ticks:1");
    snprintf(route.steps[1].script, sizeof(route.steps[1].script), "fail");
    route.steps[1].action = SCRIPT_ROUTE_WAIT;
    snprintf(route.steps[1].wait, sizeof(route.steps[1].wait), "ticks:100000");
    route.step_count = 2;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = SCRIPT_AUTO_PACED != SCRIPT_AUTO_OFF;
    script_route_pad_reset(&route_pad);
    script_input_reset(&route_input);
    size = make_script(failing_scene, sizeof(failing_scene), failing_words, sizeof(failing_words) / sizeof(failing_words[0]), "\0\0ab\0", 5);
    CHECK(script_machine_add_source(&machine, "fail", failing_scene, size, NULL));
    size = make_script(next_scene, sizeof(next_scene), failing_words + 6, 3, "\0\0", 2);
    CHECK(script_machine_add_source(&machine, "elsewhere", next_scene, size, NULL));
    CHECK(script_machine_start(&machine, "fail"));
}

static void test_a_step_waiting_for_the_retry_holds_through_the_failed_attempt(void)
{
    start_a_scene_that_fails();
    snprintf(route.steps[1].wait, sizeof(route.steps[1].wait), "retried");
    while (!was_marked("game_over_retry reload") && machine.tick < 1000) {
        run_ticks(1);
        CHECK(!was_marked("route_step_done 1:wait:-:0"));
    }
    CHECK_INT(machine.retries_of_the_scene, 1);
    run_ticks(30);
    CHECK(was_marked("route_step_done 1:wait:-:0"));
}

static void test_a_fresh_start_or_another_scene_forgets_the_retries(void)
{
    start_a_scene_that_fails();
    while (count_marks("script_begun fail") < 2 && machine.tick < 1000) {
        run_ticks(1);
    }
    CHECK_INT(machine.retries_of_the_scene, 1);
    CHECK(script_machine_start(&machine, "fail"));
    CHECK_INT(machine.retries_of_the_scene, 0);
    machine.retries_of_the_scene = 1;
    CHECK(script_machine_start(&machine, "elsewhere"));
    CHECK_INT(machine.retries_of_the_scene, 0);
}

static void test_a_retry_after_a_game_over_resumes_the_route_at_its_step(void)
{
    start_a_scene_that_fails();
    run_ticks(1000);
    CHECK(count_marks("route_step_done 0:wait:-:0") == 1);
    CHECK(was_marked("game_over_retry reload"));
    CHECK(count_marks("script_begun fail") >= 2);
}

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = 100;
    machine.objects[index].main_vm.source = machine.runner.source;
    machine.objects[index].main_vm.has_ended = 1;
    machine.objects[index].idle_handler = SCRIPT_NO_SELECTION;
}

static void place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder(void)
{
    machine.object_count = 2;
    place_core(0, "body", 100);
    place_core(1, "ladder", 140);
    script_soul_set_first_core(&machine, 0, 0);
}

static void fail_while_the_soul_is_on_the_ladder(void)
{
    start_a_scene_that_fails();
    place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder();
    script_soul_set_first_core(&machine, 1, 0);
    while (count_marks("script_begun fail") < 2 && machine.tick < 1000) {
        run_ticks(1);
    }
    CHECK(was_marked("game_over_retry reload"));
}

static void test_a_retry_starts_the_soul_on_the_core_it_failed_on(void)
{
    fail_while_the_soul_is_on_the_ladder();
    place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder();
    CHECK_INT(machine.soul.active_object, 1);
    script_soul_set_first_core(&machine, 0, 0);
    CHECK_INT(machine.soul.active_object, 0);
}

static void test_a_fresh_start_of_the_scene_starts_the_soul_on_its_own_first_core(void)
{
    fail_while_the_soul_is_on_the_ladder();
    place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder();
    CHECK(script_machine_start(&machine, "fail"));
    place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder();
    CHECK_INT(machine.soul.active_object, 0);
}

static void test_another_scene_forgets_the_core_kept_for_the_retry(void)
{
    fail_while_the_soul_is_on_the_ladder();
    CHECK(script_machine_start(&machine, "elsewhere"));
    place_a_body_whose_scene_starts_the_soul_on_it_and_a_ladder();
    CHECK_INT(machine.soul.active_object, 0);
}

int main(void)
{
    test_a_retry_after_a_game_over_resumes_the_route_at_its_step();
    test_a_step_waiting_for_the_retry_holds_through_the_failed_attempt();
    test_a_fresh_start_or_another_scene_forgets_the_retries();
    test_a_retry_starts_the_soul_on_the_core_it_failed_on();
    test_a_fresh_start_of_the_scene_starts_the_soul_on_its_own_first_core();
    test_another_scene_forgets_the_core_kept_for_the_retry();
    test_auto_play_never_moves_a_waiting_scene_on();
    test_a_press_left_from_the_earlier_scene_is_cleared();
    return test_finish("unit/scene_retry");
}
