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

int main(void)
{
    test_a_talk_that_starts_again_closes_the_film_frame_a_flashback_left();
    test_the_route_reads_a_dead_talk_step();
    return test_finish("unit/dead_talk");
}
