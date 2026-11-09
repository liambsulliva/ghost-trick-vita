#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define TICKS_FOR_THE_SCENE_TO_START 60
#define MARK_MAX 32

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];
static char marks[MARK_MAX][64];
static int mark_count;

static void on_marked(const char *what, const char *name)
{
    if (mark_count < MARK_MAX) {
        snprintf(marks[mark_count], sizeof(marks[mark_count]), "%s %s", what, name);
        mark_count = mark_count + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static int was_marked(const char *text)
{
    int index;

    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            return 1;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_a_scene_with_the_countdown(int is_running)
{
    static const unsigned int words[] = { 0x2f, 6000, 0x0 };
    unsigned int index;
    int tick;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.soul.countdown_is_running = is_running;
    for (tick = 0; tick < TICKS_FOR_THE_SCENE_TO_START; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_a_running_countdown_is_carried_over_at_game_start(void)
{
    start_a_scene_with_the_countdown(1);
    CHECK(was_marked("countdown_carried_over a"));
}

static void test_no_countdown_means_no_carry_over(void)
{
    start_a_scene_with_the_countdown(0);
    CHECK(!was_marked("countdown_carried_over a"));
}

int main(void)
{
    test_a_running_countdown_is_carried_over_at_game_start();
    test_no_countdown_means_no_carry_over();
    return test_finish("unit/countdown_carry_over");
}
