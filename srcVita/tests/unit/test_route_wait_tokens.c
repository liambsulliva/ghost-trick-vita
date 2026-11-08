#include <string.h>
#include "script_route.h"
#include "test_support.h"

static void test_a_wait_splits_into_words_and_arguments(void)
{
    struct script_route_wait_token token;
    const char *rest;

    rest = script_route_read_wait_token("act:game:a&view&ticks:30", &token);
    CHECK_INT(token.word, SCRIPT_ROUTE_WAIT_ACT);
    CHECK(token.first != NULL && strcmp(token.first, "game") == 0);
    CHECK(token.second != NULL && strcmp(token.second, "a") == 0);
    rest = script_route_read_wait_token(rest, &token);
    CHECK_INT(token.word, SCRIPT_ROUTE_WAIT_VIEW);
    CHECK(token.first == NULL);
    rest = script_route_read_wait_token(rest, &token);
    CHECK_INT(token.word, SCRIPT_ROUTE_WAIT_TICKS);
    CHECK(rest == NULL);
}

static void test_unknown_and_partial_words_are_unknown(void)
{
    struct script_route_wait_token token;

    script_route_read_wait_token("acts:game", &token);
    CHECK_INT(token.word, SCRIPT_ROUTE_WAIT_UNKNOWN);
    script_route_read_wait_token("", &token);
    CHECK_INT(token.word, SCRIPT_ROUTE_WAIT_UNKNOWN);
}

static void test_a_route_with_an_unknown_wait_is_refused(void)
{
    static struct script_route route;
    static const char good[] = "step st01 trick lamp 0 0 view&flag:12\n";
    static const char bad[] = "step st01 trick lamp 0 0 view&viewing\n";

    CHECK(script_route_read(&route, good, (unsigned int)strlen(good)));
    CHECK(!script_route_read(&route, bad, (unsigned int)strlen(bad)));
}

int main(void)
{
    test_a_wait_splits_into_words_and_arguments();
    test_unknown_and_partial_words_are_unknown();
    test_a_route_with_an_unknown_wait_is_refused();
    return test_finish("unit/route_wait_tokens");
}
