#include "script_classes.h"
#include "test_support.h"

static void test_both_human_classes_are_human_and_share_the_orange_style(void)
{
    CHECK(script_class_is_human(SCRIPT_CLASS_HUM_COMMON));
    CHECK(script_class_is_human(SCRIPT_CLASS_HUMAN));
    CHECK(!script_class_is_human(SCRIPT_CLASS_SCR_OBJ));
    CHECK_INT((int)script_class_of_soul_style(SCRIPT_CLASS_HUMAN), (int)SCRIPT_CLASS_HUM_COMMON);
    CHECK_INT(script_class_default_level(SCRIPT_CLASS_HUMAN), 6);
    CHECK_INT(script_class_default_level(SCRIPT_CLASS_GMK_COMMON_DEAD), 4);
    CHECK_INT(script_class_default_level(SCRIPT_CLASS_SCR_OBJ), 9);
}

static void test_past_dead_bodies_never_draw_as_sprites(void)
{
    CHECK(!script_class_draws_in_living_view(SCRIPT_CLASS_HUM_PAST_DEAD));
    CHECK(!script_class_draws_in_soul_view(SCRIPT_CLASS_HUM_PAST_DEAD));
}

static void test_view_only_classes(void)
{
    CHECK(!script_class_draws_in_living_view(SCRIPT_CLASS_GMK_SOUL_VISION));
    CHECK(script_class_draws_in_soul_view(SCRIPT_CLASS_GMK_SOUL_VISION));
    CHECK(script_class_draws_in_living_view(SCRIPT_CLASS_SCR_OBJ));
    CHECK(!script_class_draws_in_soul_view(SCRIPT_CLASS_SCR_OBJ));
    CHECK(!script_class_draws_in_soul_view(SCRIPT_CLASS_GMK_STEALTH));
    CHECK(!script_class_draws_in_living_view(SCRIPT_CLASS_GMK_COMMON_DEAD_SOUL_VIEW));
    CHECK(script_class_draws_in_living_view(SCRIPT_CLASS_HUMAN));
    CHECK(script_class_draws_in_soul_view(SCRIPT_CLASS_HUMAN));
}

int main(void)
{
    test_both_human_classes_are_human_and_share_the_orange_style();
    test_past_dead_bodies_never_draw_as_sprites();
    test_view_only_classes();
    return test_finish("unit/script_classes");
}
