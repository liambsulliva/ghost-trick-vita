#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 128];
static int background_level = -1;
static const char strings[] = "\0\0BG\0";

static void on_background(int level) { background_level = level; }
static void on_marked(const char *what, const char *name) { (void)what; (void)name; }
static const struct script_outputs outputs = { .marked = on_marked, .background_color_level_changed = on_background };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void test_bg_takes_the_palette_level(void)
{
    static const unsigned int words[] = { 0xf7, 2, 176, 0x2f, 600, 0x0 };
    unsigned int index;
    unsigned int code_size;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = sizeof(words) / sizeof(words[0]) * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK_INT(machine.background_color_level, SCRIPT_COLOR_LEVEL_NORMAL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    for (index = 0; index < 20; index++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.background_color_level, 176);
    CHECK_INT(background_level, 176);
}

int main(void)
{
    test_bg_takes_the_palette_level();
    return test_finish("unit/color_level");
}
