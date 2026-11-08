#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "test_support.h"

#define BUNDLE_TEXT_MAX 200000
#define NEW_ANIMATIONS 50

static int checks_made;
static const struct animation *kept_in_use;

static int count_and_answer(const struct animation *animation)
{
    checks_made = checks_made + 1;
    return animation == kept_in_use;
}

static unsigned int write_bundle(char *text, int first, int count)
{
    unsigned int used;
    int index;

    used = 0;
    for (index = first; index < first + count; index++) {
        used = used + (unsigned int)snprintf(&text[used], BUNDLE_TEXT_MAX - used, "animation t_%04d\nloops 0\nframe page_%04d 4 0 0 8 8 0 0 1 1\n", index, index);
    }
    return used;
}

static void test_a_full_library_makes_room_for_a_bundle_in_one_pass(void)
{
    static char text[BUNDLE_TEXT_MAX];
    unsigned int size;

    fake_files_reset();
    animation_library_init("assets/animations");
    animation_library_set_in_use_check(count_and_answer);
    size = write_bundle(text, 0, ANIMATION_LIBRARY_MAX);
    CHECK_INT(animation_library_add_bundle(text, size), ANIMATION_LIBRARY_MAX);
    kept_in_use = animation_library_load("t_0000");
    checks_made = 0;
    size = write_bundle(text, ANIMATION_LIBRARY_MAX, NEW_ANIMATIONS);
    CHECK_INT(animation_library_add_bundle(text, size), NEW_ANIMATIONS);
    CHECK(checks_made <= ANIMATION_LIBRARY_MAX);
    CHECK(animation_library_load("t_0000") == kept_in_use);
    CHECK(animation_library_load("t_0549") != NULL);
    CHECK_INT(animation_library_evicted_count(), NEW_ANIMATIONS);
    animation_library_set_in_use_check(NULL);
}

int main(void)
{
    test_a_full_library_makes_room_for_a_bundle_in_one_pass();
    return test_finish("unit/bundle_eviction_pass");
}
