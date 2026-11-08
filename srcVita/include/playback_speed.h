#ifndef PLAYBACK_SPEED_H
#define PLAYBACK_SPEED_H

#define PLAYBACK_SPEED_NORMAL 256

void playback_speed_set(int speed_out_of_256);
int playback_speed_get(void);
unsigned int playback_speed_frames_to_skip(unsigned int frames_played, int speed_out_of_256);
unsigned int playback_speed_frames_to_play(unsigned int frame_count, unsigned int least_frames, int speed_out_of_256);

#endif
