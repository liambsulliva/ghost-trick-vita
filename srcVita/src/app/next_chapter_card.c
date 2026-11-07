#include "next_chapter_card.h"

#define BLACK_BEFORE_TICKS 60.0f
#define FADE_IN_TICKS 24.0f
#define FULL_TICKS 216.0f
#define FADE_OUT_TICKS 24.0f

float next_chapter_card_fade(float age_ticks)
{
    float age;

    age = age_ticks - BLACK_BEFORE_TICKS;
    if (age < 0.0f) {
        return 1.0f;
    }
    if (age < FADE_IN_TICKS) {
        return 1.0f - age / FADE_IN_TICKS;
    }
    age = age - FADE_IN_TICKS - FULL_TICKS;
    if (age < 0.0f) {
        return 0.0f;
    }
    if (age < FADE_OUT_TICKS) {
        return age / FADE_OUT_TICKS;
    }
    return 1.0f;
}

int next_chapter_card_is_over(float age_ticks)
{
    return age_ticks >= BLACK_BEFORE_TICKS + FADE_IN_TICKS + FULL_TICKS + FADE_OUT_TICKS;
}
