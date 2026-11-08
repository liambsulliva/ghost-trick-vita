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

int main(void)
{
    test_the_route_reads_a_dead_talk_step();
    return test_finish("unit/dead_talk");
}
