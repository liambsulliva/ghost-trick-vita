#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_flags.h"
#include "script_machine.h"
#include "test_support.h"

#define LONG_WAIT_TICKS 60000
#define TICKS_FOR_THE_PILL 60
#define MARK_MAX 64
#define FATE_SECTION_FLAG 0x1020u

static struct script_machine machine;
static char marks[MARK_MAX][96];
static int mark_count;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];

static void on_marked(const char *what, const char *name)
{
    if (mark_count < MARK_MAX) {
        snprintf(marks[mark_count], sizeof(marks[mark_count]), "%s %s", what, name);
        mark_count = mark_count + 1;
    }
}

static const struct script_outputs outputs = { .marked = on_marked };

static int count_marks(const char *text)
{
    int index;
    int count;

    count = 0;
    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], text) == 0) {
            count = count + 1;
        }
    }
    return count;
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void put_u32(unsigned char *at, unsigned int value)
{
    put_u16(at, value & 0xffffu);
    put_u16(at + 2, value >> 16);
}

static unsigned int make_script(unsigned char *bytes, unsigned int capacity, const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(bytes, 0, capacity);
    memcpy(bytes, "1LMG", 4);
    code_size = word_count * 2;
    put_u32(&bytes[0x08], code_size);
    put_u32(&bytes[0x0c], 2);
    put_u32(&bytes[0x10], 0);
    for (index = 0; index < word_count; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    return SCRIPT_FILE_HEADER_SIZE + code_size + 2;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_machine_tick(&machine);
    }
}

static void start_a_scene_with_the_rollback_on(int in_the_four_minutes)
{
    static const unsigned int words[] = { 0xfe, 0x2f, LONG_WAIT_TICKS, 0x0 };
    unsigned int size;

    mark_count = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    size = make_script(scene, sizeof(scene), words, sizeof(words) / sizeof(words[0]));
    CHECK(script_machine_add_source(&machine, "attempt", scene, size, NULL));
    CHECK(script_machine_start(&machine, "attempt"));
    script_flags_write(&machine.flags, FATE_SECTION_FLAG, in_the_four_minutes);
    run_ticks(TICKS_FOR_THE_PILL);
}

static void test_the_hourglass_stays_away_outside_the_four_minutes(void)
{
    start_a_scene_with_the_rollback_on(0);
    CHECK_INT(machine.soul.rollback_is_on, 1);
    CHECK(!script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(10);
    CHECK_INT(count_marks("rollback_retry reload"), 0);
    CHECK_INT(count_marks("script_begun attempt"), 1);
    CHECK_INT(machine.retries_of_the_scene, 0);
}

static void test_the_hourglass_restarts_the_attempt_inside_the_four_minutes(void)
{
    start_a_scene_with_the_rollback_on(1);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(10);
    CHECK_INT(count_marks("rollback_retry reload"), 1);
    CHECK_INT(count_marks("script_begun attempt"), 2);
    CHECK_INT(machine.retries_of_the_scene, 1);
}

static void test_the_hourglass_waits_out_a_talk(void)
{
    start_a_scene_with_the_rollback_on(1);
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_HOURGLASS));
    machine.talk_is_open = 1;
    script_soul_note_pill(&machine, SCRIPT_PILL_HOURGLASS);
    run_ticks(1);
    machine.talk_is_open = 0;
    run_ticks(10);
    CHECK_INT(count_marks("rollback_retry reload"), 0);
    CHECK_INT(machine.soul.rollback_is_pending, 0);
}

int main(void)
{
    test_the_hourglass_stays_away_outside_the_four_minutes();
    test_the_hourglass_restarts_the_attempt_inside_the_four_minutes();
    test_the_hourglass_waits_out_a_talk();
    return test_finish("unit/hourglass_rollback");
}
