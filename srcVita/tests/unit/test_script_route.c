#include <string.h>
#include "script_route.h"
#include "test_support.h"

static struct script_route route;

static int read_text(const char *text)
{
    return script_route_read(&route, text, (unsigned int)strlen(text));
}

static void test_reads_every_field_of_a_step(void)
{
    CHECK(read_text("step game000 move guitar 1 tutorial:move 90\n"));
    CHECK_INT(route.step_count, 1);
    CHECK_TEXT(route.steps[0].script, "game000");
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_MOVE);
    CHECK_TEXT(route.steps[0].object, "guitar");
    CHECK_INT(route.steps[0].slot, 1);
    CHECK_INT(route.steps[0].wait, SCRIPT_ROUTE_WAIT_TUTORIAL_MOVE);
    CHECK_INT(route.steps[0].dwell_ticks, 90);
}

static void test_reads_named_waits_and_empty_words(void)
{
    CHECK(read_text("step a wait - 0 act:gameAct060 0\nstep a trick lamp 0 core:lamp 5\nstep a ghost_on - 0 - 0\nstep a tap_left - 0 playable 0\n"));
    CHECK_INT(route.step_count, 4);
    CHECK_INT(route.steps[0].wait, SCRIPT_ROUTE_WAIT_ACT);
    CHECK_TEXT(route.steps[0].wait_name, "gameAct060");
    CHECK_TEXT(route.steps[0].object, "");
    CHECK_INT(route.steps[1].wait, SCRIPT_ROUTE_WAIT_CORE);
    CHECK_TEXT(route.steps[1].wait_name, "lamp");
    CHECK_INT(route.steps[2].wait, SCRIPT_ROUTE_WAIT_NOTHING);
    CHECK_INT(route.steps[3].action, SCRIPT_ROUTE_TAP_LEFT);
    CHECK_INT(route.steps[3].wait, SCRIPT_ROUTE_WAIT_PLAYABLE);
}

static void test_rejects_bad_lines(void)
{
    CHECK(!read_text("step a dance lamp 0 - 0\n"));
    CHECK(!read_text("step a move lamp 0 sometime 0\n"));
    CHECK(!read_text("step a move lamp\n"));
    CHECK(!read_text("nonsense\n"));
}

static void test_finds_the_first_step_of_a_script(void)
{
    CHECK(read_text("step a move x 0 - 0\nstep b move y 0 - 0\nstep b trick y 0 - 0\n"));
    CHECK_INT(script_route_first_step_of_script(&route, "a"), 0);
    CHECK_INT(script_route_first_step_of_script(&route, "b"), 1);
    CHECK_INT(script_route_first_step_of_script(&route, "c"), -1);
}

static void test_names_each_action(void)
{
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_TRICK), "trick");
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_WAIT), "wait");
}

int main(void)
{
    test_reads_every_field_of_a_step();
    test_reads_named_waits_and_empty_words();
    test_rejects_bad_lines();
    test_finds_the_first_step_of_a_script();
    test_names_each_action();
    return test_finish("unit/script_route");
}
