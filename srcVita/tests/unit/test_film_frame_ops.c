#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_FRAME_ON 0x6f
#define OP_FRAME_OFF 0x70
#define OP_FRAME_SLIDES_ON 0x71
#define OP_FRAME_SLIDES_OFF 0x72
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define NOT_TOLD_YET (-1)

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static int frame_the_view_was_told;

static const char strings[] = "\0\0";

static void film_frame_changed(int is_on)
{
    frame_the_view_was_told = is_on;
}

static const struct script_outputs outputs = { .film_frame_changed = film_frame_changed };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void run_a_scene_that_starts_with(unsigned int first_op, unsigned int second_op)
{
    unsigned int words[5];
    unsigned int index;
    unsigned int code_size;

    words[0] = first_op;
    words[1] = second_op;
    words[2] = OP_WAIT;
    words[3] = A_LONG_WAIT;
    words[4] = OP_END;
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = sizeof(words) / sizeof(words[0]) * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    frame_the_view_was_told = NOT_TOLD_YET;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_machine_tick(&machine);
}

static void test_the_sliding_frame_turns_the_film_frame_on(void)
{
    run_a_scene_that_starts_with(OP_FRAME_OFF, OP_FRAME_SLIDES_ON);
    CHECK_INT(machine.film_frame_is_on, 1);
    CHECK_INT(frame_the_view_was_told, 1);
}

static void test_the_sliding_frame_turns_the_film_frame_off(void)
{
    run_a_scene_that_starts_with(OP_FRAME_ON, OP_FRAME_SLIDES_OFF);
    CHECK_INT(machine.film_frame_is_on, 0);
    CHECK_INT(frame_the_view_was_told, 0);
}

static void test_the_plain_frame_ops_still_work(void)
{
    run_a_scene_that_starts_with(OP_FRAME_SLIDES_OFF, OP_FRAME_ON);
    CHECK_INT(machine.film_frame_is_on, 1);
    run_a_scene_that_starts_with(OP_FRAME_SLIDES_ON, OP_FRAME_OFF);
    CHECK_INT(machine.film_frame_is_on, 0);
}

int main(void)
{
    test_the_sliding_frame_turns_the_film_frame_on();
    test_the_sliding_frame_turns_the_film_frame_off();
    test_the_plain_frame_ops_still_work();
    return test_finish("unit/film_frame_ops");
}
