#include <stdio.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "read_ahead.h"
#include "test_support.h"
#include "texture_cache.h"

#define SCRIPT_COUNT 4
#define LINES_PER_SCRIPT (READ_AHEAD_ENTRY_MAX / 2 + 1)
#define PLAN_TEXT_MAX (LINES_PER_SCRIPT * 48)

static char plan_texts[SCRIPT_COUNT][PLAN_TEXT_MAX];

static int pending_path_mentions(const char *name)
{
    int index;

    for (index = 0; index < fake_reader_pending_count(); index++) {
        if (strstr(fake_reader_pending_path(index), name) != NULL) {
            return 1;
        }
    }
    return 0;
}

static void write_plan(int script, char *text)
{
    unsigned int length;
    int line;

    length = 0;
    for (line = 0; line < LINES_PER_SCRIPT; line++) {
        length = length + (unsigned int)snprintf(&text[length], PLAN_TEXT_MAX - length, "ahead * %d page_%d_%d\n", 1000 + line * 10, script, line);
    }
}

static void set_up(void)
{
    char name[16];
    int script;

    fake_files_reset();
    fake_reader_reset();
    fake_reader_hold_reads(1);
    texture_cache_init("assets/textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    read_ahead_clear();
    for (script = 0; script < SCRIPT_COUNT; script++) {
        write_plan(script, plan_texts[script]);
        snprintf(name, sizeof(name), "scene%d", script);
        CHECK_INT(read_ahead_add_text(name, plan_texts[script], (unsigned int)strlen(plan_texts[script])), LINES_PER_SCRIPT);
    }
}

static void test_the_last_script_of_a_long_chain_keeps_its_plan(void)
{
    set_up();
    read_ahead_note_mark("script_begun", "scene3", 0);
    CHECK_INT(read_ahead_entry_count(), LINES_PER_SCRIPT);
    CHECK(read_ahead_step(1000) > 0);
    CHECK(pending_path_mentions("page_3_0"));
}

static void test_a_new_script_reads_only_its_own_plan(void)
{
    set_up();
    read_ahead_note_mark("script_begun", "scene1", 0);
    read_ahead_note_mark("script_begun", "scene2", 5000);
    CHECK_INT(read_ahead_entry_count(), LINES_PER_SCRIPT);
    CHECK(read_ahead_step(6000) > 0);
    CHECK(pending_path_mentions("page_2_0"));
    CHECK(!pending_path_mentions("page_1_"));
}

static void test_a_script_with_no_plan_reads_nothing(void)
{
    set_up();
    read_ahead_note_mark("script_begun", "scene0", 0);
    read_ahead_note_mark("script_begun", "unplanned", 100);
    CHECK_INT(read_ahead_entry_count(), 0);
    CHECK_INT(read_ahead_step(1100), 0);
}

int main(void)
{
    test_the_last_script_of_a_long_chain_keeps_its_plan();
    test_a_new_script_reads_only_its_own_plan();
    test_a_script_with_no_plan_reads_nothing();
    return test_finish("unit/read_ahead_every_script");
}
