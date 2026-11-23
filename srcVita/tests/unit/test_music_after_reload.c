#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_START_MUSIC 0x106
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define MUSIC 12
#define TICKS_FOR_A_RELOAD 40

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 32];
static int music_starts;
static int forced_changes;

static void music_started(int id)
{
    CHECK_INT(id, MUSIC);
    music_starts = music_starts + 1;
}

static void scene_change_was_forced(void)
{
    forced_changes = forced_changes + 1;
}

static const struct script_outputs outputs = {
    .music_started = music_started,
    .scene_change_was_forced = scene_change_was_forced,
};

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void start_a_scene_that_plays_music(void)
{
    static const unsigned int words[] = { OP_START_MUSIC, MUSIC, 0, OP_WAIT, A_LONG_WAIT, OP_END };
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    music_starts = 0;
    forced_changes = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_machine_tick(&machine);
}

static void test_a_reloaded_scene_starts_its_music_again(void)
{
    int tick;

    start_a_scene_that_plays_music();
    CHECK_INT(music_starts, 1);
    script_machine_reload_scene(&machine);
    CHECK_INT(forced_changes, 1);
    for (tick = 0; tick < TICKS_FOR_A_RELOAD; tick++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(music_starts, 2);
}

int main(void)
{
    test_a_reloaded_scene_starts_its_music_again();
    return test_finish("unit/music_after_reload");
}
