#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_EFFECT_STATUS 0xb3
#define OP_EFFECT_FADE_OUT 0xb9
#define OP_EFFECT_STEPPING 0xba
#define OP_EFFECTS_REFRESH_COLOR 0xee
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define NAME_OFFSET 2
#define NOT_TOLD (-99)

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static char name_told[32];
static int status_told;
static int fade_ticks_told;
static int stopped_told;

static const char strings[] = "\0\0fountain\0";

static void effect_status_set(const char *name, int status)
{
    snprintf(name_told, sizeof(name_told), "%s", name);
    status_told = status;
}

static void effect_fade_out_started(const char *name, int ticks)
{
    snprintf(name_told, sizeof(name_told), "%s", name);
    fade_ticks_told = ticks;
}

static void effect_stepping_changed(const char *name, int is_stopped)
{
    snprintf(name_told, sizeof(name_told), "%s", name);
    stopped_told = is_stopped;
}

static const struct script_outputs outputs = {
    .effect_status_set = effect_status_set,
    .effect_fade_out_started = effect_fade_out_started,
    .effect_stepping_changed = effect_stepping_changed,
};

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void run_a_scene_of(const unsigned int *op_words, unsigned int op_word_count)
{
    unsigned int words[8];
    unsigned int count;
    unsigned int index;
    unsigned int code_size;

    count = 0;
    for (index = 0; index < op_word_count; index++) {
        words[count++] = op_words[index];
    }
    words[count++] = OP_WAIT;
    words[count++] = A_LONG_WAIT;
    words[count++] = OP_END;
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    name_told[0] = '\0';
    status_told = NOT_TOLD;
    fade_ticks_told = NOT_TOLD;
    stopped_told = NOT_TOLD;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_machine_tick(&machine);
}

static int the_script_reached_the_wait(void)
{
    return machine.runner.wait_ticks > 0;
}

static void test_the_status_op_tells_the_effect_its_status(void)
{
    static const unsigned int words[] = { OP_EFFECT_STATUS, NAME_OFFSET, 2 };

    run_a_scene_of(words, 3);
    CHECK(strcmp(name_told, "fountain") == 0);
    CHECK_INT(status_told, 2);
    CHECK(the_script_reached_the_wait());
}

static void test_the_fade_op_starts_a_timed_fade_out(void)
{
    static const unsigned int words[] = { OP_EFFECT_FADE_OUT, NAME_OFFSET, 30 };

    run_a_scene_of(words, 3);
    CHECK(strcmp(name_told, "fountain") == 0);
    CHECK_INT(fade_ticks_told, 30);
    CHECK(the_script_reached_the_wait());
}

static void test_the_stepping_op_stops_and_restarts_the_effect(void)
{
    static const unsigned int stop_words[] = { OP_EFFECT_STEPPING, NAME_OFFSET, 1 };
    static const unsigned int go_words[] = { OP_EFFECT_STEPPING, NAME_OFFSET, 0 };

    run_a_scene_of(stop_words, 3);
    CHECK_INT(stopped_told, 1);
    run_a_scene_of(go_words, 3);
    CHECK_INT(stopped_told, 0);
    CHECK(the_script_reached_the_wait());
}

static void test_the_color_refresh_op_is_stepped_over_with_its_one_word(void)
{
    static const unsigned int words[] = { OP_EFFECTS_REFRESH_COLOR, NAME_OFFSET };

    run_a_scene_of(words, 2);
    CHECK(name_told[0] == '\0');
    CHECK(the_script_reached_the_wait());
}

int main(void)
{
    test_the_status_op_tells_the_effect_its_status();
    test_the_fade_op_starts_a_timed_fade_out();
    test_the_stepping_op_stops_and_restarts_the_effect();
    test_the_color_refresh_op_is_stepped_over_with_its_one_word();
    return test_finish("unit/effect_control_ops");
}
