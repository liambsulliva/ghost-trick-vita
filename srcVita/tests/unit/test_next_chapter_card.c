#include "next_chapter_card.h"
#include "test_support.h"

#define FADE_SCALE 100.0f

static int fade_percent(float age_ticks)
{
    return (int)(next_chapter_card_fade(age_ticks) * FADE_SCALE + 0.5f);
}

static void test_the_card_comes_up_from_black_and_goes_back_to_black(void)
{
    CHECK_INT(fade_percent(0.0f), 100);
    CHECK_INT(fade_percent(59.0f), 100);
    CHECK_INT(fade_percent(72.0f), 50);
    CHECK_INT(fade_percent(84.0f), 0);
    CHECK_INT(fade_percent(299.0f), 0);
    CHECK_INT(fade_percent(312.0f), 50);
    CHECK_INT(fade_percent(324.0f), 100);
}

static void test_the_card_is_over_after_its_last_fade(void)
{
    CHECK_INT(next_chapter_card_is_over(323.0f), 0);
    CHECK_INT(next_chapter_card_is_over(324.0f), 1);
}

int main(void)
{
    test_the_card_comes_up_from_black_and_goes_back_to_black();
    test_the_card_is_over_after_its_last_fade();
    return test_finish("unit/next_chapter_card");
}
