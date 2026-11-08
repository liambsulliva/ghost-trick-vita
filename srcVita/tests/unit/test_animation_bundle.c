#include <string.h>
#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "test_support.h"

static const char bundle[] =
    "animation t_ball\n"
    "loops 1\n"
    "frame ball_00 4 0 0 10 10 0 0 1 1\n"
    "frame ball_01 4 0 0 10 10 0 0 1 1\n"
    "animation t_hand\n"
    "loops 0\n"
    "frame hand_00 6 0 0 8 8 0 0 1 1\n";

static void test_a_bundle_loads_each_animation_without_reading_files(void)
{
    const struct animation *ball;
    const struct animation *hand;

    fake_files_reset();
    animation_library_init("assets/animations");
    CHECK_INT(animation_library_add_bundle(bundle, (unsigned int)strlen(bundle)), 2);
    ball = animation_library_load("t_ball");
    hand = animation_library_load("t_hand");
    CHECK(ball != NULL);
    CHECK(hand != NULL);
    CHECK_INT((int)ball->frame_count, 2);
    CHECK_INT((int)hand->frame_count, 1);
    CHECK(strcmp(hand->frames[0].texture_name, "hand_00") == 0);
}

static void test_a_second_bundle_does_not_add_an_animation_twice(void)
{
    fake_files_reset();
    animation_library_init("assets/animations");
    CHECK_INT(animation_library_add_bundle(bundle, (unsigned int)strlen(bundle)), 2);
    CHECK_INT(animation_library_add_bundle(bundle, (unsigned int)strlen(bundle)), 0);
}

int main(void)
{
    test_a_bundle_loads_each_animation_without_reading_files();
    test_a_second_bundle_does_not_add_an_animation_twice();
    return test_finish("unit/animation_bundle");
}
