#include <stdio.h>
#include <string.h>
#include "game_save.h"
#include "script_file.h"
#include "script_flags.h"
#include "script_machine.h"
#include "test_support.h"

#define STORY_FLAG 0x1046u
#define CHAPTER 9
#define OTHER_CHAPTER 4
#define MOST_TICKS 2000
#define SCRIPT_BYTES_MAX (SCRIPT_FILE_HEADER_SIZE + 128)
#define ROOT_AT 8u
#define EARLY_AT 13u
#define LATE_AT 19u
#define PLACE_LIST 0
#define NOTED_PLACE 4

static const char strings[] = "\0\0phone\0root\0early\0late\0chapter\0";
static const unsigned int to_root[] = { 0x24, ROOT_AT, 0x2f, 6000, 0x0 };
static const unsigned int root_by_story_flag[] = { 0x32, STORY_FLAG & 0xffffu, STORY_FLAG >> 16, 0x33, 0x24, EARLY_AT, 0x0, 0x24, LATE_AT, 0x0 };
static const unsigned int early[] = { 0x74, 1, 0x2f, 6000, 0x0 };
static const unsigned int idle[] = { 0x2f, 6000, 0x0 };

static struct script_machine machine;
static const struct script_outputs no_outputs;
static unsigned char scripts[5][SCRIPT_BYTES_MAX];
static struct game_save save;
static struct game_save picked;
static char text[2 * GAME_SAVE_TEXT_MAX];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void add_script(int slot, const char *name, const unsigned int *words, unsigned int word_count)
{
    unsigned char *into;
    unsigned int index;
    unsigned int code_size;

    into = scripts[slot];
    code_size = word_count * 2;
    memset(into, 0, SCRIPT_BYTES_MAX);
    memcpy(into, "1LMG", 4);
    put_u16(&into[0x08], code_size);
    put_u16(&into[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&into[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    CHECK(script_machine_add_source(&machine, name, into, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
}

static void make_machine(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    add_script(0, GAME_SAVE_CHAPTER_SELECT_SCRIPT, to_root, sizeof(to_root) / sizeof(to_root[0]));
    add_script(1, "phone", to_root, sizeof(to_root) / sizeof(to_root[0]));
    add_script(2, "root", root_by_story_flag, sizeof(root_by_story_flag) / sizeof(root_by_story_flag[0]));
    add_script(3, "early", early, sizeof(early) / sizeof(early[0]));
    add_script(4, "late", idle, sizeof(idle) / sizeof(idle[0]));
}

static int run_until_scene(const char *name)
{
    int tick;

    for (tick = 0; tick < MOST_TICKS; tick++) {
        if (machine.scene_name != NULL && strcmp(machine.scene_name, name) == 0) {
            return 1;
        }
        script_machine_tick(&machine);
    }
    return 0;
}

static int run_until_a_scene_outside(const char *first, const char *second)
{
    int tick;

    for (tick = 0; tick < MOST_TICKS; tick++) {
        if (machine.scene_name != NULL && strcmp(machine.scene_name, first) != 0 && strcmp(machine.scene_name, second) != 0) {
            return 1;
        }
        script_machine_tick(&machine);
    }
    return 0;
}

static unsigned int bake_two_chapter_starts(void)
{
    unsigned int used;

    make_machine();
    machine.chapter_mark = OTHER_CHAPTER - 1;
    game_save_capture(&save, &machine, GAME_SAVE_CHAPTER_SELECT_SCRIPT);
    used = game_save_write_chapter_start(&save, text, sizeof(text));
    CHECK(used > 0);
    machine.chapter_mark = CHAPTER - 1;
    machine.note_levels[PLACE_LIST][NOTED_PLACE] = 1;
    CHECK(script_flags_write_known(&machine.flags, STORY_FLAG, 1));
    game_save_capture(&save, &machine, GAME_SAVE_CHAPTER_SELECT_SCRIPT);
    used = used + game_save_write_chapter_start(&save, text + used, sizeof(text) - used);
    return used;
}

static void start_chapter_select_from(const struct game_save *start)
{
    make_machine();
    game_save_apply_before_start(start, &machine);
    CHECK(script_machine_start(&machine, start->script));
}

static const char *ride_the_phone(void)
{
    machine.phone_book_place = NOTED_PLACE;
    CHECK(script_machine_start(&machine, "phone"));
    CHECK(run_until_a_scene_outside("phone", "root"));
    return machine.scene_name;
}

static void test_each_chapter_start_is_read_from_its_own_section(void)
{
    unsigned int size;

    size = bake_two_chapter_starts();
    CHECK(game_save_read_chapter_start(&picked, text, size, CHAPTER));
    CHECK_INT(picked.chapter_mark, CHAPTER - 1);
    CHECK_INT(game_save_chapter_of_start(&picked), CHAPTER);
    CHECK_INT(picked.note_levels[PLACE_LIST][NOTED_PLACE], 1);
    CHECK(strcmp(picked.script, GAME_SAVE_CHAPTER_SELECT_SCRIPT) == 0);
    CHECK(game_save_read_chapter_start(&picked, text, size, OTHER_CHAPTER));
    CHECK_INT(picked.chapter_mark, OTHER_CHAPTER - 1);
    CHECK_INT(picked.note_levels[PLACE_LIST][NOTED_PLACE], 0);
    CHECK(!game_save_read_chapter_start(&picked, text, size, CHAPTER + 1));
}

static void test_a_bare_chapter_select_is_marked_for_its_baked_start(void)
{
    CHECK(game_save_for_chapter_select(&save, CHAPTER));
    CHECK(save.is_chapter_select);
    CHECK_INT(game_save_chapter_of_start(&save), CHAPTER);
    CHECK(game_save_read_chapter_start(&picked, text, bake_two_chapter_starts(), CHAPTER));
    CHECK(!picked.is_chapter_select);
}

static void test_a_baked_start_opens_the_scene_its_story_flags_pick(void)
{
    CHECK(game_save_read_chapter_start(&picked, text, bake_two_chapter_starts(), CHAPTER));
    start_chapter_select_from(&picked);
    CHECK(run_until_scene("late"));
    CHECK_INT(machine.chapter_mark, CHAPTER - 1);
}

static void test_a_phone_ride_keeps_the_chapter_and_the_story_state(void)
{
    static unsigned char flags_before[SCRIPT_FLAG_BYTES];
    static unsigned char notes_before[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];

    CHECK(game_save_read_chapter_start(&picked, text, bake_two_chapter_starts(), CHAPTER));
    start_chapter_select_from(&picked);
    CHECK(run_until_scene("late"));
    memcpy(flags_before, machine.flags.game, sizeof(flags_before));
    memcpy(notes_before, machine.note_levels, sizeof(notes_before));
    CHECK(strcmp(ride_the_phone(), "late") == 0);
    CHECK_INT(machine.chapter_mark, CHAPTER - 1);
    CHECK(memcmp(flags_before, machine.flags.game, sizeof(flags_before)) == 0);
    CHECK(memcmp(notes_before, machine.note_levels, sizeof(notes_before)) == 0);
}

static void test_a_start_without_the_story_flags_rides_back_to_the_first_scene(void)
{
    CHECK(game_save_for_chapter_select(&save, CHAPTER));
    start_chapter_select_from(&save);
    CHECK(strcmp(ride_the_phone(), "early") == 0);
}

int main(void)
{
    test_each_chapter_start_is_read_from_its_own_section();
    test_a_bare_chapter_select_is_marked_for_its_baked_start();
    test_a_baked_start_opens_the_scene_its_story_flags_pick();
    test_a_phone_ride_keeps_the_chapter_and_the_story_state();
    test_a_start_without_the_story_flags_rides_back_to_the_first_scene();
    return test_finish("unit/chapter_starts");
}
