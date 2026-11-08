#include "playback_speed.h"

static volatile int current_speed_out_of_256 = PLAYBACK_SPEED_NORMAL;

void playback_speed_set(int speed_out_of_256)
{
    current_speed_out_of_256 = speed_out_of_256 < PLAYBACK_SPEED_NORMAL ? PLAYBACK_SPEED_NORMAL : speed_out_of_256;
}

int playback_speed_get(void)
{
    return current_speed_out_of_256;
}

unsigned int playback_speed_frames_to_skip(unsigned int frames_played, int speed_out_of_256)
{
    if (speed_out_of_256 <= PLAYBACK_SPEED_NORMAL) {
        return 0;
    }
    return (unsigned int)((unsigned long long)frames_played * (unsigned int)(speed_out_of_256 - PLAYBACK_SPEED_NORMAL) / PLAYBACK_SPEED_NORMAL);
}

unsigned int playback_speed_frames_to_play(unsigned int frame_count, unsigned int least_frames, int speed_out_of_256)
{
    unsigned int frames;

    if (speed_out_of_256 <= PLAYBACK_SPEED_NORMAL) {
        return frame_count;
    }
    frames = (unsigned int)((unsigned long long)frame_count * PLAYBACK_SPEED_NORMAL / (unsigned int)speed_out_of_256);
    if (frames < least_frames) {
        frames = least_frames;
    }
    return frames < frame_count ? frames : frame_count;
}
