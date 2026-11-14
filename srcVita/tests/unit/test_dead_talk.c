#include <string.h>
#include "script_machine.h"
#include "script_route.h"
#include "test_support.h"

static struct script_machine machine;
static struct script_route route;
static const struct script_outputs no_outputs;

static void start_with_a_dead_talk_step(const char *object)
{
    memset(&route, 0, sizeof(route));
    CHECK(script_route_read(&route, "step s dead_talk lynne 0 0 -\n", 29));
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_DEAD_TALK);
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = object;
}

static void test_the_route_reads_a_dead_talk_step(void)
{
    start_with_a_dead_talk_step("lynne");
    CHECK_TEXT(route.steps[0].object, "lynne");
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_DEAD_TALK), "dead_talk");
}

static int film_frame_reports;
static int film_frame_reported;

static void film_frame_changed(int is_on)
{
    film_frame_reports++;
    film_frame_reported = is_on;
}

static const struct script_outputs film_outputs = { .film_frame_changed = film_frame_changed };

static void test_a_talk_that_starts_again_closes_the_film_frame_a_flashback_left(void)
{
    script_machine_init(&machine, &film_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.film_frame_is_on = 1;
    film_frame_reports = 0;
    script_soul_run_command(&machine, "DeadTalk", "Restart");
    CHECK_INT(machine.film_frame_is_on, 0);
    CHECK_INT(film_frame_reports, 1);
    CHECK_INT(film_frame_reported, 0);
    CHECK_INT(machine.soul.dead_talk_is_on, 1);
}

static int swirl_reports;
static int swirl_reported;

static void swirl_changed(int is_on)
{
    swirl_reports++;
    swirl_reported = is_on;
}

static const struct script_outputs swirl_outputs = { .swirl_changed = swirl_changed };

static void start_a_talk_with_the_swirl_watched(void)
{
    script_machine_init(&machine, &swirl_outputs, SCRIPT_TIMING_REMASTER, NULL);
    swirl_reports = 0;
    swirl_reported = 0;
    script_soul_run_command(&machine, "DeadTalk", "Restart");
}

static void test_every_dead_talk_puts_up_the_swirl_and_finish_takes_it_down(void)
{
    start_a_talk_with_the_swirl_watched();
    CHECK_INT(machine.soul.dead_talk_is_on, 1);
    CHECK_INT(machine.swirl_is_on, 1);
    CHECK_INT(swirl_reported, 1);
    script_soul_run_command(&machine, "DeadTalk", "Finish");
    CHECK_INT(machine.soul.dead_talk_is_on, 0);
    CHECK_INT(machine.swirl_is_on, 0);
    CHECK_INT(swirl_reported, 0);
}

static void test_an_aborted_dead_talk_takes_the_swirl_down_and_a_restart_puts_it_back(void)
{
    start_a_talk_with_the_swirl_watched();
    script_soul_run_command(&machine, "DeadTalk", "Abort");
    CHECK_INT(machine.swirl_is_on, 0);
    CHECK_INT(swirl_reported, 0);
    script_soul_run_command(&machine, "DeadTalk", "Restart");
    CHECK_INT(machine.swirl_is_on, 1);
    CHECK_INT(swirl_reported, 1);
}

static void test_a_script_swirl_inside_the_talk_is_not_reported_twice(void)
{
    start_a_talk_with_the_swirl_watched();
    CHECK_INT(swirl_reports, 1);
    script_soul_run_command(&machine, "DeadTalk", "Finish");
    CHECK_INT(swirl_reports, 2);
    script_soul_run_command(&machine, "DeadTalk", "Finish");
    CHECK_INT(swirl_reports, 2);
}

int main(void)
{
    test_every_dead_talk_puts_up_the_swirl_and_finish_takes_it_down();
    test_an_aborted_dead_talk_takes_the_swirl_down_and_a_restart_puts_it_back();
    test_a_script_swirl_inside_the_talk_is_not_reported_twice();
    test_a_talk_that_starts_again_closes_the_film_frame_a_flashback_left();
    test_the_route_reads_a_dead_talk_step();
    return test_finish("unit/dead_talk");
}
