#include "pcm_mix.h"

#define SAMPLE_MAX 32767
#define SAMPLE_MIN -32768

void pcm_mix_add(short *into, const short *from, unsigned int sample_count)
{
    unsigned int index;
    int sum;

    for (index = 0; index < sample_count; index++) {
        sum = into[index] + from[index];
        if (sum > SAMPLE_MAX) {
            sum = SAMPLE_MAX;
        }
        if (sum < SAMPLE_MIN) {
            sum = SAMPLE_MIN;
        }
        into[index] = (short)sum;
    }
}
