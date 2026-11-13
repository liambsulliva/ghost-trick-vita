#include <string.h>
#include "ds_units.h"
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 8
#define FADE_OUT_KIND 1
#define FADE_FRAMES 30
#define WHITE_WIPE_EFFECT 50

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static const char strings[] = "\0\0";
static int last_brightness;
static int brightness_reports;

static void on_brightness(int level)
{
    last_brightness = level;
    brightness_reports = brightness_reports + 1;
}

static const struct script_outputs outputs = { .brightness_changed = on_brightness };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_a_waiting_scene(int from_black)
{
    static const unsigned int words[] = { 0x2f, 6000, 0x0 };
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = (unsigned int)(sizeof(words) / sizeof(words[0])) * 2u;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    last_brightness = 0;
    brightness_reports = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    if (from_black) {
        script_machine_start_from_black(&machine);
    }
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_fresh_start_shows_black_until_the_script_fades_in(void)
{
    start_a_waiting_scene(1);
    run_ticks(1);
    CHECK_INT(last_brightness, -NATIVE_BRIGHTNESS_SCALE);
    CHECK(script_machine_picture_is_hidden(&machine));
}

static void test_a_fade_to_black_reaches_the_screen_when_no_white_wipe_runs(void)
{
    start_a_waiting_scene(0);
    run_ticks(1);
    CHECK_INT(brightness_reports, 0);
    script_fades_add(&machine, NULL, FADE_OUT_KIND, FADE_FRAMES);
    run_ticks(FADE_FRAMES + 2);
    CHECK_INT(last_brightness, -NATIVE_BRIGHTNESS_SCALE);
}

static void test_a_running_white_wipe_still_wins_over_black(void)
{
    start_a_waiting_scene(1);
    script_soul_effect_spawned(&machine, WHITE_WIPE_EFFECT, "wipe");
    run_ticks(80);
    CHECK_INT(last_brightness, NATIVE_BRIGHTNESS_SCALE);
}

int main(void)
{
    test_a_fresh_start_shows_black_until_the_script_fades_in();
    test_a_fade_to_black_reaches_the_screen_when_no_white_wipe_runs();
    test_a_running_white_wipe_still_wins_over_black();
    return test_finish("unit/screen_brightness");
}
