#include "pcm_ramp.h"
#include "test_support.h"

#define RAMP_FRAMES 512
#define BUFFER_FRAMES 600

static short samples[BUFFER_FRAMES * 2];

static void fill_samples(short value)
{
    int index;

    for (index = 0; index < BUFFER_FRAMES * 2; index++) {
        samples[index] = value;
    }
}

static void test_a_ramp_starts_at_its_first_gain_and_rises_to_exactly_the_target(void)
{
    struct pcm_ramp ramp;
    int frame;

    fill_samples(16384);
    pcm_ramp_begin(&ramp, 0, PCM_RAMP_FULL_GAIN, RAMP_FRAMES);
    pcm_ramp_apply(&ramp, samples, BUFFER_FRAMES);
    CHECK_INT(samples[0], 0);
    CHECK_INT(samples[1], 0);
    for (frame = 1; frame < RAMP_FRAMES; frame++) {
        CHECK(samples[frame * 2] >= samples[(frame - 1) * 2]);
        CHECK_INT(samples[frame * 2 + 1], samples[frame * 2]);
    }
    CHECK_INT(samples[(RAMP_FRAMES - 1) * 2], 16384 * (RAMP_FRAMES - 1) / RAMP_FRAMES);
    CHECK_INT(samples[RAMP_FRAMES * 2], 16384);
    CHECK_INT(samples[(BUFFER_FRAMES - 1) * 2], 16384);
    CHECK_INT(ramp.gain, PCM_RAMP_FULL_GAIN);
}

static void test_a_ramp_can_span_several_calls(void)
{
    struct pcm_ramp ramp;

    fill_samples(32767);
    pcm_ramp_begin(&ramp, PCM_RAMP_FULL_GAIN, 0, 4);
    pcm_ramp_apply(&ramp, samples, 2);
    CHECK_INT(ramp.gain, 24576);
    CHECK(!pcm_ramp_has_faded_out(&ramp));
    pcm_ramp_apply(&ramp, &samples[4], 2);
    CHECK(pcm_ramp_has_faded_out(&ramp));
    pcm_ramp_apply(&ramp, &samples[8], 1);
    CHECK_INT(ramp.gain, 0);
    CHECK_INT(samples[8], 0);
    CHECK_INT(samples[9], 0);
}

static void test_full_gain_changes_nothing_at_the_extremes(void)
{
    struct pcm_ramp ramp;

    samples[0] = -32768;
    samples[1] = 32767;
    pcm_ramp_begin(&ramp, PCM_RAMP_FULL_GAIN, PCM_RAMP_FULL_GAIN, 0);
    pcm_ramp_apply(&ramp, samples, 1);
    CHECK_INT(samples[0], -32768);
    CHECK_INT(samples[1], 32767);
}

static void test_a_ramp_without_frames_lands_on_its_target_at_once(void)
{
    struct pcm_ramp ramp;

    samples[0] = 1000;
    samples[1] = 1000;
    pcm_ramp_begin(&ramp, PCM_RAMP_FULL_GAIN, 0, 0);
    CHECK_INT(ramp.gain, 0);
    CHECK(pcm_ramp_has_faded_out(&ramp));
    pcm_ramp_apply(&ramp, samples, 1);
    CHECK_INT(samples[0], 0);
}

static void test_a_ramp_up_is_not_a_fade_out(void)
{
    struct pcm_ramp ramp;

    pcm_ramp_begin(&ramp, 0, PCM_RAMP_FULL_GAIN, 0);
    CHECK(!pcm_ramp_has_faded_out(&ramp));
}

static void test_a_long_ramp_does_not_overflow(void)
{
    struct pcm_ramp ramp;
    short frame[2];
    unsigned int index;

    pcm_ramp_begin(&ramp, 0, PCM_RAMP_FULL_GAIN, 441000);
    for (index = 0; index < 220500; index++) {
        frame[0] = 32767;
        frame[1] = 32767;
        pcm_ramp_apply(&ramp, frame, 1);
    }
    CHECK_INT(ramp.gain, 16383);
}

int main(void)
{
    test_a_ramp_starts_at_its_first_gain_and_rises_to_exactly_the_target();
    test_a_ramp_can_span_several_calls();
    test_full_gain_changes_nothing_at_the_extremes();
    test_a_ramp_without_frames_lands_on_its_target_at_once();
    test_a_ramp_up_is_not_a_fade_out();
    test_a_long_ramp_does_not_overflow();
    return test_finish("unit/pcm_ramp");
}
