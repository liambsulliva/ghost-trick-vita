#include <stdio.h>
#include <string.h>
#include "crc32.h"
#include "script_route.h"
#include "test_support.h"

static struct script_route route;

static int read_text(const char *text)
{
    return script_route_read(&route, text, (unsigned int)strlen(text));
}

static void test_reads_every_field_of_a_step(void)
{
    CHECK(read_text("step game000 move guitar 1 90 act:game:gameAct040&tutorial:move\n"));
    CHECK_INT(route.step_count, 1);
    CHECK_TEXT(route.steps[0].script, "game000");
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_MOVE);
    CHECK_TEXT(route.steps[0].object, "guitar");
    CHECK_INT(route.steps[0].slot, 1);
    CHECK_INT(route.steps[0].dwell_ticks, 90);
    CHECK_TEXT(route.steps[0].wait, "act:game:gameAct040&tutorial:move");
}

static void test_reads_empty_words_as_nothing(void)
{
    CHECK(read_text("step a ghost_on - 0 45 -\n"));
    CHECK_TEXT(route.steps[0].object, "");
    CHECK_TEXT(route.steps[0].wait, "");
}

static void test_reads_every_action_and_wait_word(void)
{
    CHECK(read_text("step a move x 0 0 view&usable:x:1\nstep a trick x 1 0 core:x:1&playable\nstep a ghost_off - 0 0 no_talk&flag:1\nstep a touch x 0 0 tutorial:touch\nstep a tap_balloon x 0 0 balloon:x&free\nstep a camera - 0 0 camera_tutorial\nstep a phone_call x 0 0 noflag:2\nstep a wait - 0 0 timeleap_armed&ticks:5\n"));
    CHECK_INT(route.step_count, 8);
    CHECK_INT(route.steps[3].action, SCRIPT_ROUTE_TOUCH);
    CHECK_INT(route.steps[6].action, SCRIPT_ROUTE_PHONE_CALL);
}

static void test_rejects_bad_lines(void)
{
    CHECK(!read_text("step a dance lamp 0 0 -\n"));
    CHECK(!read_text("step a move lamp 0 0 sometime\n"));
    CHECK(!read_text("step a move lamp 0 0 view&sometime\n"));
    CHECK(!read_text("step a move lamp\n"));
    CHECK(!read_text("nonsense\n"));
}

static void test_finds_the_first_step_of_a_script(void)
{
    CHECK(read_text("step a move x 0 0 -\nstep b move y 0 0 -\nstep b trick y 0 0 -\n"));
    CHECK_INT(script_route_first_step_of_script(&route, "a"), 0);
    CHECK_INT(script_route_first_step_of_script(&route, "b"), 1);
    CHECK_INT(script_route_first_step_of_script(&route, "c"), -1);
}

static void test_names_each_action(void)
{
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_TRICK), "trick");
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_WAIT), "wait");
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_CHARACTER), "character");
    CHECK(read_text("step a character - 0 0 free\n"));
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_CHARACTER);
    CHECK_TEXT(script_route_action_name(SCRIPT_ROUTE_SWAP), "swap");
    CHECK(read_text("step a swap barrel 0 0 view\n"));
    CHECK_INT(route.steps[0].action, SCRIPT_ROUTE_SWAP);
    CHECK_TEXT(route.steps[0].object, "barrel");
}

static void test_finds_whole_wait_tokens_only(void)
{
    CHECK(script_route_wait_has_token("act:game:a&view", "act:game:a"));
    CHECK(script_route_wait_has_token("act:game:a&view", "view"));
    CHECK(!script_route_wait_has_token("act:game:ab&view", "act:game:a"));
    CHECK(!script_route_wait_has_token("", "view"));
}

static void test_pacing_overrides_the_dwell_of_named_steps(void)
{
    const char *pacing;

    CHECK(read_text("step a move x 0 90 -\nstep a trick x 0 90 -\nstep a ghost_off - 0 45 -\n"));
    pacing = "dwell 1 250\ndwell 2 60\n";
    CHECK(script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    CHECK_INT(route.steps[0].dwell_ticks, 90);
    CHECK_INT(route.steps[1].dwell_ticks, 250);
    CHECK_INT(route.steps[2].dwell_ticks, 60);
}

static void test_pacing_rejects_steps_outside_the_route(void)
{
    const char *pacing;

    CHECK(read_text("step a move x 0 90 -\n"));
    pacing = "dwell 1 250\n";
    CHECK(!script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    pacing = "dwell 0 -5\n";
    CHECK(!script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    pacing = "pace 0 5\n";
    CHECK(!script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    CHECK(script_route_read_pacing(&route, "", 0));
    CHECK_INT(route.steps[0].dwell_ticks, 90);
}

static void test_pacing_baked_for_another_route_is_ignored(void)
{
    const char *steps;
    char pacing[64];

    steps = "step a move x 0 90 -\nstep a trick x 0 90 -\n";
    CHECK(read_text(steps));
    snprintf(pacing, sizeof(pacing), "route %08x\ndwell 1 250\n", crc32_add(0u, (const unsigned char *)steps, (unsigned int)strlen(steps)));
    CHECK(script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    CHECK(!route.pacing_is_stale);
    CHECK_INT(route.steps[1].dwell_ticks, 250);
    CHECK(read_text(steps));
    snprintf(pacing, sizeof(pacing), "route %08x\ndwell 1 250\n", crc32_add(0u, (const unsigned char *)steps, (unsigned int)strlen(steps)) ^ 1u);
    CHECK(script_route_read_pacing(&route, pacing, (unsigned int)strlen(pacing)));
    CHECK(route.pacing_is_stale);
    CHECK_INT(route.steps[1].dwell_ticks, 90);
}

int main(void)
{
    test_reads_every_field_of_a_step();
    test_reads_empty_words_as_nothing();
    test_reads_every_action_and_wait_word();
    test_rejects_bad_lines();
    test_finds_the_first_step_of_a_script();
    test_names_each_action();
    test_finds_whole_wait_tokens_only();
    test_pacing_overrides_the_dwell_of_named_steps();
    test_pacing_rejects_steps_outside_the_route();
    test_pacing_baked_for_another_route_is_ignored();
    return test_finish("unit/script_route");
}
