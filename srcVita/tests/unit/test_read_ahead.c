#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "read_ahead.h"
#include "test_support.h"
#include "texture_cache.h"

static const char schedule[] =
    "ahead gameAct010 100 page_a\n"
    "ahead gameAct010 600 page_b\n"
    "ahead gameAct020 0 page_c\n";

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

static void set_up(void)
{
    fake_files_reset();
    fake_reader_reset();
    fake_reader_hold_reads(1);
    texture_cache_init("assets/textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    read_ahead_clear();
    CHECK_INT(read_ahead_add_text("scene", schedule, (unsigned int)strlen(schedule)), 3);
}

static void test_pages_due_soon_after_the_act_are_read_ahead(void)
{
    set_up();
    read_ahead_note_mark("script_begun", "scene", 0);
    read_ahead_note_mark("act_begun", "gameAct010", 1000);
    read_ahead_note_mark("act_begun", "cicelAct000", 1001);
    CHECK_INT(read_ahead_step(1000), 1);
    CHECK(pending_path_mentions("page_a"));
    CHECK(!pending_path_mentions("page_b"));
    CHECK(!pending_path_mentions("page_c"));
}

static void test_the_window_moves_on_and_drops_pages_already_due(void)
{
    set_up();
    read_ahead_note_mark("script_begun", "scene", 0);
    read_ahead_note_mark("act_begun", "gameAct010", 1000);
    CHECK_INT(read_ahead_step(1400), 1);
    CHECK(pending_path_mentions("page_b"));
    CHECK(!pending_path_mentions("page_a"));
}

static void test_nothing_is_read_for_another_scene_or_before_a_scene(void)
{
    set_up();
    CHECK_INT(read_ahead_step(10), 0);
    read_ahead_note_mark("script_begun", "other_scene", 0);
    read_ahead_note_mark("act_begun", "gameAct010", 0);
    CHECK_INT(read_ahead_step(100), 0);
    CHECK_INT(fake_reader_pending_count(), 0);
}

static void test_a_plan_names_the_script_that_comes_next(void)
{
    static const char plan[] = "next second\nahead * 4 page_a\n";

    set_up();
    CHECK_INT(read_ahead_add_text("first", plan, (unsigned int)strlen(plan)), 1);
    read_ahead_note_mark("script_begun", "first", 0);
    CHECK_TEXT(read_ahead_next_script(), "second");
    CHECK_INT(read_ahead_entry_count(), 1);
    read_ahead_note_mark("script_begun", "scene", 10);
    CHECK_TEXT(read_ahead_next_script(), "");
    read_ahead_note_mark("script_begun", "st01-st01_root", 20);
    CHECK_TEXT(read_ahead_next_script(), "");
}

static void test_a_roots_plan_names_the_first_scene_for_each_chapter_mark(void)
{
    static const char root_plan[] = "next game000\nnext_for 12 game000\nnext_for 13 game001\n";
    static const char scene_plan[] = "next_for 1 wrong\nnext game003\nahead * 4 page_a\n";

    set_up();
    read_ahead_add_text("root", root_plan, (unsigned int)strlen(root_plan));
    read_ahead_add_text("game002", scene_plan, (unsigned int)strlen(scene_plan));
    CHECK_TEXT(read_ahead_next_script_of("root", 13), "game001");
    CHECK_TEXT(read_ahead_next_script_of("root", 12), "game000");
    CHECK_TEXT(read_ahead_next_script_of("root", 7), "game000");
    CHECK_TEXT(read_ahead_next_script_of("unknown", 13), "");
    read_ahead_note_mark("script_begun", "game002", 0);
    CHECK_TEXT(read_ahead_next_script(), "game003");
}

int main(void)
{
    test_a_roots_plan_names_the_first_scene_for_each_chapter_mark();
    test_a_plan_names_the_script_that_comes_next();
    test_pages_due_soon_after_the_act_are_read_ahead();
    test_the_window_moves_on_and_drops_pages_already_due();
    test_nothing_is_read_for_another_scene_or_before_a_scene();
    return test_finish("unit/read_ahead");
}
