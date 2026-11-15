#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define WORD_MAX 16
#define TICKS_TO_RUN 3
#define OP_FRAME_ON 0x6f
#define OP_SEND 0x1d
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];
static int times_the_view_was_told;
static int frame_the_view_was_told;
static int notes_about_gameapp;

static const char strings[] = "\0\0GameApp\0EventFrame\0On\0Off\0";

static void film_frame_changed(int is_on)
{
    times_the_view_was_told = times_the_view_was_told + 1;
    frame_the_view_was_told = is_on;
}

static void marked(const char *what, const char *name)
{
    (void)name;
    if (strcmp(what, "gameapp_not_ported") == 0) {
        notes_about_gameapp = notes_about_gameapp + 1;
    }
}

static const struct script_outputs outputs = { .film_frame_changed = film_frame_changed, .marked = marked };

static unsigned int offset_of(const char *text)
{
    unsigned int offset;

    for (offset = 0; offset < sizeof(strings) - 1; offset++) {
        if (strcmp(&strings[offset], text) == 0) {
            return offset;
        }
    }
    return 0;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static void open_the_frame_then_send(const char *arg)
{
    unsigned int words[WORD_MAX];
    unsigned int count;
    int tick;

    count = 0;
    words[count++] = OP_FRAME_ON;
    words[count++] = OP_SEND;
    words[count++] = offset_of("GameApp");
    words[count++] = offset_of("EventFrame");
    words[count++] = offset_of(arg);
    words[count++] = OP_WAIT;
    words[count++] = A_LONG_WAIT;
    words[count++] = OP_END;
    times_the_view_was_told = 0;
    notes_about_gameapp = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(words, count), NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        script_machine_tick(&machine);
    }
}

static void test_event_frame_off_closes_the_film_frame(void)
{
    open_the_frame_then_send("Off");
    CHECK_INT(machine.film_frame_is_on, 0);
    CHECK_INT(frame_the_view_was_told, 0);
    CHECK_INT(times_the_view_was_told, 2);
    CHECK_INT(notes_about_gameapp, 0);
}

static void test_event_frame_on_leaves_an_open_frame_open(void)
{
    open_the_frame_then_send("On");
    CHECK_INT(machine.film_frame_is_on, 1);
    CHECK_INT(times_the_view_was_told, 1);
    CHECK_INT(notes_about_gameapp, 0);
}

int main(void)
{
    test_event_frame_off_closes_the_film_frame();
    test_event_frame_on_leaves_an_open_frame_open();
    return test_finish("unit/event_frame_command");
}
