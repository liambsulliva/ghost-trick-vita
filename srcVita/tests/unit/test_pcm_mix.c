#include "pcm_mix.h"
#include "test_support.h"

static void test_samples_add_up(void)
{
    short into[3] = { 100, -100, 0 };
    const short from[3] = { 23, -23, 5 };

    pcm_mix_add(into, from, 3);
    CHECK_INT(into[0], 123);
    CHECK_INT(into[1], -123);
    CHECK_INT(into[2], 5);
}

static void test_sums_saturate_instead_of_wrapping(void)
{
    short into[2] = { 32000, -32000 };
    const short from[2] = { 1000, -1000 };

    pcm_mix_add(into, from, 2);
    CHECK_INT(into[0], 32767);
    CHECK_INT(into[1], -32768);
}

static void test_only_the_given_count_changes(void)
{
    short into[2] = { 1, 1 };
    const short from[2] = { 1, 1 };

    pcm_mix_add(into, from, 1);
    CHECK_INT(into[0], 2);
    CHECK_INT(into[1], 1);
}

int main(void)
{
    test_samples_add_up();
    test_sums_saturate_instead_of_wrapping();
    test_only_the_given_count_changes();
    return test_finish("unit/pcm_mix");
}
