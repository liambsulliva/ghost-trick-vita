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

int main(void)
{
    test_pages_due_soon_after_the_act_are_read_ahead();
    test_the_window_moves_on_and_drops_pages_already_due();
    test_nothing_is_read_for_another_scene_or_before_a_scene();
    return test_finish("unit/read_ahead");
}
