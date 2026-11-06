#ifndef PCM_RAMP_H
#define PCM_RAMP_H

#define PCM_RAMP_FULL_GAIN 32768

struct pcm_ramp {
    int gain;
    int start_gain;
    int target_gain;
    unsigned int frames_total;
    unsigned int frames_done;
};

void pcm_ramp_begin(struct pcm_ramp *ramp, int start_gain, int target_gain, unsigned int frames);
void pcm_ramp_apply(struct pcm_ramp *ramp, short *stereo_frames, unsigned int frame_count);
int pcm_ramp_has_faded_out(const struct pcm_ramp *ramp);

#endif
