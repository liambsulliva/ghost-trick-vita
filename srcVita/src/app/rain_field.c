#include "rain_field.h"
#include "ds_units.h"

#define STREAKS_PER_SPAN_DS_PIXEL 0.4f
#define LENGTH_CHOICES 48u
#define HALF_LENGTH_PER_CHOICE 0.375f
#define SLOWEST 16
#define SPEED_CHOICES 16u
#define FIRST_ROW_RANGE 256u
#define FIRST_ROW_START (-32)
#define LATER_ROW_RANGE 128u
#define LATER_ROW_START 96
#define LAST_ROW 160.0f
#define ALPHA_MASK 15
#define RANDOM_SEED 12345u
#define RANDOM_MULTIPLIER 1103515245u
#define RANDOM_INCREMENT 12345u
#define RANDOM_SHIFT 16

static unsigned int next_random(struct rain_field *field, unsigned int range)
{
    field->random_state = field->random_state * RANDOM_MULTIPLIER + RANDOM_INCREMENT;
    return (field->random_state >> RANDOM_SHIFT) % range;
}

static void spawn(struct rain_field *field, struct rain_streak *streak, int is_first, float center_x, float top_y)
{
    streak->half_length = (float)next_random(field, LENGTH_CHOICES) * HALF_LENGTH_PER_CHOICE;
    streak->speed = (float)(SLOWEST + (int)next_random(field, SPEED_CHOICES));
    streak->x = center_x + (float)field->first_x + (float)next_random(field, (unsigned int)field->span);
    if (is_first) {
        streak->y = top_y + (float)(FIRST_ROW_START + (int)next_random(field, FIRST_ROW_RANGE));
    } else {
        streak->y = top_y + (float)(LATER_ROW_START - (int)next_random(field, LATER_ROW_RANGE));
    }
    streak->is_falling = 1;
}

void rain_field_seed(struct rain_field *field)
{
    field->count = 0;
    field->random_state = RANDOM_SEED;
}

void rain_field_start(struct rain_field *field, int span, int first_x, float center_x, float top_y)
{
    int index;

    field->span = span;
    field->first_x = first_x;
    field->count = (int)((float)span * STREAKS_PER_SPAN_DS_PIXEL);
    if (field->count > RAIN_FIELD_STREAK_MAX) {
        field->count = RAIN_FIELD_STREAK_MAX;
    }
    for (index = 0; index < field->count; index++) {
        spawn(field, &field->streaks[index], 1, center_x, top_y);
    }
}

void rain_field_advance(struct rain_field *field, float center_x, float top_y)
{
    int index;

    for (index = 0; index < field->count; index++) {
        struct rain_streak *streak;

        streak = &field->streaks[index];
        if (!streak->is_falling) {
            spawn(field, streak, 0, center_x, top_y);
        } else {
            streak->y = streak->y + streak->speed;
            if (streak->y > top_y + LAST_ROW) {
                streak->is_falling = 0;
            }
        }
    }
}

float rain_field_alpha_of(int index)
{
    return (float)((index & ALPHA_MASK) + 1) / (float)NATIVE_ALPHA_FULL;
}
