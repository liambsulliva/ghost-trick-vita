#ifndef PHONE_SPLIT_H
#define PHONE_SPLIT_H

#define PHONE_SPLIT_CAPTURE_AGE 13
#define PHONE_SPLIT_NOT_SIGNALLED (-1)

struct phone_split_layout {
    int panels_are_up;
    int capture_is_shown;
    float belt_offset;
    float capture_x;
    float center_x;
    float side_column_width;
    float top_door_bottom;
    float bottom_door_top;
    float strip_left;
    float strip_right;
    float cap_left;
    float cap_right;
    float door_right;
};

void phone_split_layout_at(int age, int signal_age, int belts_were_on, float slide_px, struct phone_split_layout *layout);

#endif
