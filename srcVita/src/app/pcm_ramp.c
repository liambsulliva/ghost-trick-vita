#include "pcm_ramp.h"

#define GAIN_SHIFT 15

void pcm_ramp_begin(struct pcm_ramp *ramp, int start_gain, int target_gain, unsigned int frames)
{
    ramp->gain = frames > 0 ? start_gain : target_gain;
    ramp->start_gain = start_gain;
    ramp->target_gain = target_gain;
    ramp->frames_total = frames;
    ramp->frames_done = 0;
}

static int next_gain(struct pcm_ramp *ramp)
{
    long long travelled;

    if (ramp->frames_done >= ramp->frames_total) {
        return ramp->target_gain;
    }
    travelled = (long long)(ramp->target_gain - ramp->start_gain) * ramp->frames_done;
    ramp->frames_done = ramp->frames_done + 1;
    return ramp->start_gain + (int)(travelled / ramp->frames_total);
}

void pcm_ramp_apply(struct pcm_ramp *ramp, short *stereo_frames, unsigned int frame_count)
{
    unsigned int frame;

    for (frame = 0; frame < frame_count; frame++) {
        ramp->gain = next_gain(ramp);
        stereo_frames[frame * 2] = (short)((stereo_frames[frame * 2] * ramp->gain) >> GAIN_SHIFT);
        stereo_frames[frame * 2 + 1] = (short)((stereo_frames[frame * 2 + 1] * ramp->gain) >> GAIN_SHIFT);
    }
}

int pcm_ramp_has_faded_out(const struct pcm_ramp *ramp)
{
    return ramp->target_gain == 0 && ramp->frames_done >= ramp->frames_total;
}
