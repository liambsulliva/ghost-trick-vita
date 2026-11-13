#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define FADE_TICKS 6
#define WAIT_TICKS 40
#define TICKS_TO_RUN 20

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 32];
static int still_is_on;
static int times_turned_off;
static int fades_seen;
static float last_alpha;
static int alpha_rose;

static void on_still(int is_on)
{
    still_is_on = is_on;
    if (!is_on) {
        times_turned_off = times_turned_off + 1;
    }
}

static void on_fade(float alpha)
{
    if (fades_seen > 0 && alpha > last_alpha) {
        alpha_rose = 1;
    }
    last_alpha = alpha;
    fades_seen = fades_seen + 1;
}

static const struct script_outputs outputs = { .screen_still_changed = on_still, .screen_still_faded = on_fade };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void run_script(const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    int tick;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], word_count * 2);
    for (index = 0; index < word_count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    still_is_on = 0;
    times_turned_off = 0;
    fades_seen = 0;
    last_alpha = 1.0f;
    alpha_rose = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + word_count * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_the_still_fades_out_and_goes_away(void)
{
    static const unsigned int words[] = { 0xf3, 0, 0xf6, FADE_TICKS, 0x2f, WAIT_TICKS, 0x0 };

    CHECK_INT(script_opcode_family(0xf6), SCRIPT_OP_CONTROL);
    run_script(words, sizeof(words) / sizeof(words[0]));
    CHECK_INT(still_is_on, 0);
    CHECK_INT(times_turned_off, 1);
    CHECK_INT(fades_seen, FADE_TICKS - 1);
    CHECK(!alpha_rose);
    CHECK(last_alpha > 0.0f && last_alpha < 1.0f);
}

static void test_a_fade_with_no_still_does_nothing(void)
{
    static const unsigned int words[] = { 0xf6, FADE_TICKS, 0x2f, WAIT_TICKS, 0x0 };

    run_script(words, sizeof(words) / sizeof(words[0]));
    CHECK_INT(times_turned_off, 0);
    CHECK_INT(fades_seen, 0);
}

static void test_a_still_with_no_fade_stays(void)
{
    static const unsigned int words[] = { 0xf3, 0, 0x2f, WAIT_TICKS, 0x0 };

    run_script(words, sizeof(words) / sizeof(words[0]));
    CHECK_INT(still_is_on, 1);
    CHECK_INT(fades_seen, 0);
}

int main(void)
{
    test_the_still_fades_out_and_goes_away();
    test_a_fade_with_no_still_does_nothing();
    test_a_still_with_no_fade_stays();
    return test_finish("unit/screen_still_fade");
}
