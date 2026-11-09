#ifndef STICK_AIM_H
#define STICK_AIM_H

#define STICK_AXIS_FOR_FULL_SPEED 0.75f

void stick_aim_from_raw(int raw_x, int raw_y, float *stick_x, float *stick_y);
float stick_aim_axis_speed(float axis);
void stick_aim_first_push(float stick_x, float stick_y, int *pressed_x, int *pressed_y);

#endif
