#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 64
#define OP_CREATE_BEAM 0xbb
#define OP_RAMP_BEAM 0xbc
#define OP_CREATE_LIGHT 0xbe
#define OP_RAMP_LIGHT 0xbf
#define OP_DELETE_LIGHT 0xc0
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define BEAM_MODE_APEX 2
#define LIGHT_MODE_PLACE 8
#define LIGHT_MODE_NODE 16
#define NO_NODE 0xffffu
#define FIXED_ONE 0x1000u
#define OWNER_X 40
#define OWNER_Y 70

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 256];
static float apex_x_told;
static float apex_y_told;
static int apex_ticks_told;
static int apex_times_told;
static float placed_x;
static float placed_y;
static int times_placed;
static int pinned_told;
static int notes;

static const char strings[] = "\0\0spot\0";

static void beam_apex_ramped(const char *name, float apex_x, float apex_y, int ticks)
{
    CHECK_TEXT(name, "spot");
    apex_x_told = apex_x;
    apex_y_told = apex_y;
    apex_ticks_told = ticks;
    apex_times_told = apex_times_told + 1;
}

static void iris_placed(const char *name, float x, float y)
{
    CHECK_TEXT(name, "spot");
    placed_x = x;
    placed_y = y;
    times_placed = times_placed + 1;
}

static void iris_pinned(const char *name, int is_on_the_screen)
{
    CHECK_TEXT(name, "spot");
    pinned_told = is_on_the_screen;
}

static void marked(const char *what, const char *name)
{
    (void)name;
    if (strstr(what, "not_ported") != NULL) {
        notes = notes + 1;
    }
}

static const struct script_outputs outputs = { .beam_apex_ramped = beam_apex_ramped, .iris_placed = iris_placed, .iris_pinned = iris_pinned, .marked = marked };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int name_offset(void)
{
    return 2;
}

static unsigned int put_fixed(unsigned int *words, unsigned int count, int whole)
{
    unsigned int value;

    value = (unsigned int)whole * FIXED_ONE;
    words[count] = value & 0xffffu;
    words[count + 1] = value >> 16;
    return count + 2;
}

static unsigned int put_long(unsigned int *words, unsigned int count, unsigned int value)
{
    words[count] = value & 0xffffu;
    words[count + 1] = value >> 16;
    return count + 2;
}

static unsigned int put_light(unsigned int *words, unsigned int count, int x, int y, unsigned int place, unsigned int node)
{
    words[count++] = OP_CREATE_LIGHT;
    words[count++] = name_offset();
    count = put_fixed(words, count, x);
    count = put_fixed(words, count, y);
    count = put_fixed(words, count, 1);
    count = put_fixed(words, count, 31);
    words[count++] = place;
    words[count++] = node;
    words[count++] = 0;
    words[count++] = 0;
    return count;
}

static unsigned int put_light_ramp(unsigned int *words, unsigned int count, unsigned int value, unsigned int mode)
{
    words[count++] = OP_RAMP_LIGHT;
    words[count++] = name_offset();
    count = put_long(words, count, value);
    count = put_long(words, count, 0);
    count = put_long(words, count, 0);
    count = put_long(words, count, mode);
    return count;
}

static void run(unsigned int *words, unsigned int count, int ticks)
{
    unsigned int index;
    unsigned int code_size;
    int tick;

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
    apex_times_told = 0;
    times_placed = 0;
    pinned_told = -1;
    notes = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_beam_ramp_of_mode_two_moves_the_apex(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = 0;
    words[count++] = OP_RAMP_BEAM;
    words[count++] = name_offset();
    count = put_fixed(words, count, 112);
    count = put_fixed(words, count, -80);
    count = put_long(words, count, 120);
    count = put_long(words, count, BEAM_MODE_APEX);
    run(words, count, 1);
    CHECK_INT(apex_times_told, 1);
    CHECK(apex_x_told == 112.0f);
    CHECK(apex_y_told == -80.0f);
    CHECK_INT(apex_ticks_told, 120);
    CHECK_INT(notes, 0);
}

static void test_a_light_made_for_the_screen_is_pinned_to_it(void)
{
    unsigned int words[WORD_MAX];

    run(words, put_light(words, 0, 128, 96, SCRIPT_LIGHT_ON_THE_SCREEN, NO_NODE), 2);
    CHECK_INT(pinned_told, 1);
    CHECK_INT(times_placed, 0);
}

static void test_a_plain_light_stays_in_the_world_where_it_was_made(void)
{
    unsigned int words[WORD_MAX];

    run(words, put_light(words, 0, 128, 96, SCRIPT_LIGHT_IN_THE_WORLD, NO_NODE), 3);
    CHECK_INT(pinned_told, 0);
    CHECK_INT(times_placed, 0);
}

static void test_a_ramp_of_mode_eight_changes_where_the_light_is_pinned(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = put_light(words, 0, 128, 96, SCRIPT_LIGHT_ON_THE_SCREEN, NO_NODE);
    count = put_light_ramp(words, count, SCRIPT_LIGHT_IN_THE_WORLD, LIGHT_MODE_PLACE);
    run(words, count, 1);
    CHECK_INT(pinned_told, 0);
    CHECK_INT(machine.lights[0].place, SCRIPT_LIGHT_IN_THE_WORLD);
    CHECK_INT(notes, 0);
}

static void test_a_ramp_of_mode_sixteen_names_the_node_the_light_follows(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;

    count = put_light(words, 0, 128, 96, SCRIPT_LIGHT_IN_THE_WORLD, NO_NODE);
    count = put_light_ramp(words, count, 3, LIGHT_MODE_NODE);
    run(words, count, 1);
    CHECK_INT(machine.lights[0].node, 3);
    CHECK_INT(notes, 0);
}

static void test_a_light_on_the_soul_core_with_no_core_waits_off_the_screen(void)
{
    unsigned int words[WORD_MAX];

    run(words, put_light(words, 0, 128, 96, SCRIPT_LIGHT_ON_THE_SOUL_CORE, NO_NODE), 2);
    CHECK(times_placed >= 2);
    CHECK(placed_x < 0.0f);
}

static void test_deleting_a_light_stops_it_following(void)
{
    unsigned int words[WORD_MAX];
    unsigned int count;
    int placed_before;

    count = put_light(words, 0, 128, 96, SCRIPT_LIGHT_ON_THE_SOUL_CORE, NO_NODE);
    words[count++] = OP_DELETE_LIGHT;
    words[count++] = name_offset();
    run(words, count, 1);
    placed_before = times_placed;
    script_machine_tick(&machine);
    script_machine_tick(&machine);
    CHECK_INT(times_placed, placed_before);
    CHECK(machine.lights[0].name == NULL);
}

int main(void)
{
    test_a_beam_ramp_of_mode_two_moves_the_apex();
    test_a_light_made_for_the_screen_is_pinned_to_it();
    test_a_plain_light_stays_in_the_world_where_it_was_made();
    test_a_ramp_of_mode_eight_changes_where_the_light_is_pinned();
    test_a_ramp_of_mode_sixteen_names_the_node_the_light_follows();
    test_a_light_on_the_soul_core_with_no_core_waits_off_the_screen();
    test_deleting_a_light_stops_it_following();
    return test_finish("unit/light_ops");
}
