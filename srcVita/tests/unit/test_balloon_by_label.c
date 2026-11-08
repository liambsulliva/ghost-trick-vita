#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "test_support.h"

#define MARK_MAX 64
#define TICKS_TO_RUN 200
#define TICKS_FOR_THE_SCENE_TO_START 60

static struct script_machine machine;
static struct script_route route;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];

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

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_with_a_route_step(const char *balloon_name)
{
    static const unsigned int words[] = { 0x2f, 6000, 0x0 };
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memset(&route, 0, sizeof(route));
    snprintf(route.steps[0].script, sizeof(route.steps[0].script), "a");
    route.steps[0].action = SCRIPT_ROUTE_TAP_BALLOON;
    snprintf(route.steps[0].object, sizeof(route.steps[0].object), "%s", balloon_name);
    snprintf(route.steps[0].wait, sizeof(route.steps[0].wait), "balloon:%s", balloon_name);
    route.step_count = 1;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.auto_press = SCRIPT_AUTO_PACED;
    machine.stall_breaker_is_off = 1;
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    mark_count = 0;
    CHECK(script_machine_start(&machine, "a"));
}

static void open_a_balloon_with_only_a_label(void)
{
    machine.balloons[0].in_use = 1;
    machine.balloons[0].label = "m_label_only";
    machine.balloons[0].tap_group = SCRIPT_TAP_GROUP_RIGHT;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_balloon_with_no_event_is_found_by_its_label(void)
{
    start_with_a_route_step("label:m_label_only");
    run_ticks(TICKS_FOR_THE_SCENE_TO_START);
    open_a_balloon_with_only_a_label();
    run_ticks(TICKS_TO_RUN);
    CHECK(was_marked("route_step_done 0:tap_balloon:label:m_label_only:0"));
}

static void test_a_label_that_is_not_open_keeps_the_step_waiting(void)
{
    start_with_a_route_step("label:m_other");
    run_ticks(TICKS_FOR_THE_SCENE_TO_START);
    open_a_balloon_with_only_a_label();
    run_ticks(TICKS_TO_RUN);
    CHECK(!was_marked("route_step_done 0:tap_balloon:label:m_other:0"));
}

int main(void)
{
    test_a_balloon_with_no_event_is_found_by_its_label();
    test_a_label_that_is_not_open_keeps_the_step_waiting();
    return test_finish("unit/balloon_by_label");
}
