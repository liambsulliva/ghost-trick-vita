#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_route.h"
#include "test_support.h"

#define MARK_MAX 64
#define TICKS_PAST_A_STUCK_PLAYABLE_SCENE 20000

static struct script_machine machine;
static struct script_route route;
static unsigned char scene_a[SCRIPT_FILE_HEADER_SIZE + 16];
static unsigned char scene_b[SCRIPT_FILE_HEADER_SIZE + 16];
static unsigned char scene_c[SCRIPT_FILE_HEADER_SIZE + 16];
static char marks[MARK_MAX][96];
static int mark_count;

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

static unsigned int idle_scene(unsigned char *into)
{
    static const unsigned int words[] = { 0x2f, 60000, 0x0 };
    unsigned int index;

    memset(into, 0, SCRIPT_FILE_HEADER_SIZE + 16);
    memcpy(into, "1LMG", 4);
    put_u16(&into[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    return SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2;
}

static void add_step(const char *script, enum script_route_action action)
{
    snprintf(route.steps[route.step_count].script, sizeof(route.steps[route.step_count].script), "%s", script);
    route.steps[route.step_count].action = action;
    snprintf(route.steps[route.step_count].object, sizeof(route.steps[route.step_count].object), "-");
    snprintf(route.steps[route.step_count].wait, sizeof(route.steps[route.step_count].wait), "ticks:100000");
    route.step_count = route.step_count + 1;
}

static void run_a_stuck_scene(int route_is_driving)
{
    int tick;

    memset(&route, 0, sizeof(route));
    add_step("a", SCRIPT_ROUTE_WAIT);
    add_step("b", SCRIPT_ROUTE_WAIT);
    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = route_is_driving;
    CHECK(script_machine_add_source(&machine, "a", scene_a, idle_scene(scene_a), NULL));
    CHECK(script_machine_add_source(&machine, "b", scene_b, idle_scene(scene_b), NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < TICKS_PAST_A_STUCK_PLAYABLE_SCENE; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_stuck_routed_scene_is_skipped_to_the_next_routed_scene(void)
{
    run_a_stuck_scene(1);
    CHECK(was_marked("stall_left a"));
    CHECK(was_marked("stall_moved_to b"));
    CHECK(was_marked("script_begun b"));
}

static void test_a_player_without_auto_play_is_never_skipped(void)
{
    run_a_stuck_scene(0);
    CHECK(!was_marked("stall_left a"));
    CHECK(!was_marked("script_begun b"));
}

static void test_a_stuck_scene_with_no_steps_never_jumps_back_to_the_start_of_the_route(void)
{
    int tick;

    memset(&route, 0, sizeof(route));
    add_step("a", SCRIPT_ROUTE_WAIT);
    add_step("b", SCRIPT_ROUTE_WAIT);
    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = 1;
    CHECK(script_machine_add_source(&machine, "a", scene_a, idle_scene(scene_a), NULL));
    CHECK(script_machine_add_source(&machine, "b", scene_b, idle_scene(scene_b), NULL));
    CHECK(script_machine_add_source(&machine, "c", scene_c, idle_scene(scene_c), NULL));
    CHECK(script_machine_start(&machine, "c"));
    for (tick = 0; tick < TICKS_PAST_A_STUCK_PLAYABLE_SCENE; tick++) {
        script_machine_tick(&machine);
    }
    CHECK(!was_marked("stall_moved_to a"));
    CHECK(!was_marked("script_begun a"));
}

static void test_a_stuck_scene_after_a_finished_block_moves_on_rather_than_back(void)
{
    int tick;

    memset(&route, 0, sizeof(route));
    add_step("a", SCRIPT_ROUTE_WAIT);
    add_step("b", SCRIPT_ROUTE_WAIT);
    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.route = &route;
    machine.route_is_driving = 1;
    CHECK(script_machine_add_source(&machine, "a", scene_a, idle_scene(scene_a), NULL));
    CHECK(script_machine_add_source(&machine, "b", scene_b, idle_scene(scene_b), NULL));
    CHECK(script_machine_add_source(&machine, "c", scene_c, idle_scene(scene_c), NULL));
    CHECK(script_machine_start(&machine, "c"));
    machine.route_furthest_acted_step = 0;
    machine.soul.route_step = 0;
    for (tick = 0; tick < TICKS_PAST_A_STUCK_PLAYABLE_SCENE; tick++) {
        script_machine_tick(&machine);
    }
    CHECK(was_marked("stall_left c"));
    CHECK(was_marked("stall_moved_to b"));
    CHECK(!was_marked("stall_moved_to a"));
}

int main(void)
{
    test_a_stuck_routed_scene_is_skipped_to_the_next_routed_scene();
    test_a_player_without_auto_play_is_never_skipped();
    test_a_stuck_scene_with_no_steps_never_jumps_back_to_the_start_of_the_route();
    test_a_stuck_scene_after_a_finished_block_moves_on_rather_than_back();
    return test_finish("unit/stall_skip");
}
