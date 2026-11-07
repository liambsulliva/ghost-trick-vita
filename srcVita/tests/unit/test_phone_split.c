#include "phone_split.h"
#include "test_support.h"

#define SLIDE_PX 236.25f
#define TENTHS 10.0f

static int tenths(float value)
{
    return (int)(value * TENTHS + (value < 0.0f ? -0.5f : 0.5f));
}

static void test_belts_slide_in_before_the_panels_come_up(void)
{
    struct phone_split_layout layout;

    phone_split_layout_at(15, PHONE_SPLIT_NOT_SIGNALLED, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.belt_offset), 900);
    CHECK_INT(layout.panels_are_up, 0);
    phone_split_layout_at(31, PHONE_SPLIT_NOT_SIGNALLED, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.belt_offset), 0);
    CHECK_INT(layout.panels_are_up, 0);
}

static void test_the_capture_slides_right_while_the_divider_reaches_the_middle(void)
{
    struct phone_split_layout layout;

    phone_split_layout_at(32, PHONE_SPLIT_NOT_SIGNALLED, 0, SLIDE_PX, &layout);
    CHECK_INT(layout.capture_is_shown, 1);
    CHECK_INT(tenths(layout.capture_x), 0);
    CHECK_INT(tenths(layout.center_x), -225);
    phone_split_layout_at(48, PHONE_SPLIT_NOT_SIGNALLED, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.capture_x), 2363);
    CHECK_INT(tenths(layout.center_x), 4800);
    CHECK_INT(tenths(layout.side_column_width), 1200);
    phone_split_layout_at(90, PHONE_SPLIT_NOT_SIGNALLED, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.center_x), 4800);
}

static void test_the_door_opens_from_the_signal_and_the_capture_goes(void)
{
    struct phone_split_layout layout;

    phone_split_layout_at(51, 51, 0, SLIDE_PX, &layout);
    CHECK_INT(layout.capture_is_shown, 0);
    CHECK_INT(tenths(layout.top_door_bottom), 2720);
    CHECK_INT(tenths(layout.bottom_door_top), 2720);
    phone_split_layout_at(59, 51, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.top_door_bottom), 1370);
    CHECK_INT(tenths(layout.bottom_door_top), 4070);
    phone_split_layout_at(200, 51, 0, SLIDE_PX, &layout);
    CHECK_INT(tenths(layout.top_door_bottom), 20);
    CHECK_INT(tenths(layout.bottom_door_top), 5420);
}

static void test_belts_already_on_start_the_slide_sooner(void)
{
    struct phone_split_layout layout;

    phone_split_layout_at(15, PHONE_SPLIT_NOT_SIGNALLED, 1, SLIDE_PX, &layout);
    CHECK_INT(layout.panels_are_up, 1);
    CHECK_INT(tenths(layout.belt_offset), 0);
}

int main(void)
{
    test_belts_slide_in_before_the_panels_come_up();
    test_the_capture_slides_right_while_the_divider_reaches_the_middle();
    test_the_door_opens_from_the_signal_and_the_capture_goes();
    test_belts_already_on_start_the_slide_sooner();
    return test_finish("unit/phone_split");
}
