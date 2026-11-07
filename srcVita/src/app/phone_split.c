#include "phone_split.h"

#define SCREEN_MIDDLE_Y 272.0f
#define BELT_FIRST_AGE 16
#define BELT_LAST_AGE 31
#define BELT_STEP_PX 5.625f
#define SLIDE_FIRST_AGE_AFTER_BELTS_OPEN 32
#define SLIDE_FIRST_AGE_WITH_BELTS_ON 15
#define SLIDE_STEPS 16
#define DOOR_STEPS 16
#define DOOR_STEP_PX 16.875f
#define CENTER_FIRST_X (-22.5f)
#define CENTER_STEP_PX 31.40625f
#define DOOR_GAP_FROM_CENTER 11.25f
#define STRIP_HALF_WIDTH 22.5f
#define CAP_HALF_WIDTH 45.0f
#define SIDE_COLUMN_WIDTH 120.0f

static int clamped(int value, int lowest, int highest)
{
    if (value < lowest) {
        return lowest;
    }
    return value > highest ? highest : value;
}

void phone_split_layout_at(int age, int signal_age, int belts_were_on, float slide_px, struct phone_split_layout *layout)
{
    int slide_first_age;
    int belt_steps_left;
    int slide_step;
    int door_step;

    belt_steps_left = belts_were_on ? 0 : clamped(BELT_LAST_AGE - age, 0, BELT_LAST_AGE - BELT_FIRST_AGE + 1);
    layout->belt_offset = (float)belt_steps_left * BELT_STEP_PX;
    slide_first_age = belts_were_on ? SLIDE_FIRST_AGE_WITH_BELTS_ON : SLIDE_FIRST_AGE_AFTER_BELTS_OPEN;
    layout->panels_are_up = age >= slide_first_age;
    slide_step = clamped(age - slide_first_age, 0, SLIDE_STEPS);
    door_step = signal_age == PHONE_SPLIT_NOT_SIGNALLED ? 0 : clamped(age - signal_age, 0, DOOR_STEPS);
    layout->capture_is_shown = layout->panels_are_up && signal_age == PHONE_SPLIT_NOT_SIGNALLED;
    layout->capture_x = slide_px * (float)slide_step / (float)SLIDE_STEPS;
    layout->center_x = CENTER_FIRST_X + CENTER_STEP_PX * (float)slide_step;
    layout->side_column_width = SIDE_COLUMN_WIDTH * (float)slide_step / (float)SLIDE_STEPS;
    layout->top_door_bottom = SCREEN_MIDDLE_Y - DOOR_STEP_PX * (float)door_step;
    layout->bottom_door_top = SCREEN_MIDDLE_Y + DOOR_STEP_PX * (float)door_step;
    layout->strip_left = layout->center_x - STRIP_HALF_WIDTH;
    layout->strip_right = layout->center_x + STRIP_HALF_WIDTH;
    layout->cap_left = layout->center_x - CAP_HALF_WIDTH;
    layout->cap_right = layout->center_x + CAP_HALF_WIDTH;
    layout->door_right = layout->center_x - DOOR_GAP_FROM_CENTER;
}
