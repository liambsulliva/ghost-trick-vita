#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "scene_layers.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_bridge.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "test_support.h"

#define TICKS_FOR_EVERY_PART 20
#define NO_KIND (-1)
#define TEXT_AFTER_TICKS 6

static struct script_tables tables;
static const char balloons[] = "balloon -1 said 0\npart oval_bubble 0 0 10 5 0 6\npart bubble_said 0 -2 8 3 6 0\n";

static int overlays_on_screen(void)
{
    const struct scene_layers *layers;
    int index;
    int count;

    layers = scene_stage_layers();
    count = 0;
    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        if (layers->overlays[index].in_use) {
            count = count + 1;
        }
    }
    return count;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        texture_cache_upload_finished_reads(TICKS_FOR_EVERY_PART);
        texture_cache_begin_frame();
        script_bridge_finish_tick();
    }
}

static const struct script_outputs *start_with_held_reads(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/oval_bubble.gtex", 4, 4);
    fake_files_add_rgba_texture("tex/bubble_said.gtex", 4, 4);
    texture_cache_init("tex", 1u << 20);
    fake_reader_hold_reads(1);
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_balloons(&tables, balloons, sizeof(balloons) - 1));
    script_bridge_start(&tables);
    return script_bridge_outputs();
}

static void upload_without_ticks(int frames)
{
    int frame;

    for (frame = 0; frame < frames; frame++) {
        texture_cache_upload_finished_reads(TICKS_FOR_EVERY_PART);
        texture_cache_begin_frame();
        (void)script_bridge_balloon_art_is_loading();
    }
}

static void test_the_bubble_waits_until_its_text_has_loaded(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_held_reads();
    outputs->balloon_opened(0, "said", NO_KIND, 100.0f, 80.0f);
    run_ticks(1);
    CHECK_INT(overlays_on_screen(), 0);
    CHECK(script_bridge_balloon_art_is_loading());
    upload_without_ticks(TICKS_FOR_EVERY_PART);
    CHECK(script_bridge_balloon_art_is_loading());
    fake_reader_hold_reads(0);
    fake_reader_release_reads(TICKS_FOR_EVERY_PART);
    upload_without_ticks(TICKS_FOR_EVERY_PART);
    CHECK(!script_bridge_balloon_art_is_loading());
    run_ticks(1);
    CHECK_INT(overlays_on_screen(), 1);
    run_ticks(TEXT_AFTER_TICKS);
    CHECK_INT(overlays_on_screen(), 2);
}

static void test_a_balloon_with_loaded_art_shows_at_once(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_held_reads();
    fake_reader_hold_reads(0);
    outputs->balloon_opened(0, "said", NO_KIND, 100.0f, 80.0f);
    run_ticks(TICKS_FOR_EVERY_PART);
    CHECK_INT(overlays_on_screen(), 2);
    CHECK(!script_bridge_balloon_art_is_loading());
}

static void test_shown_text_and_overlays_hold_the_game_while_they_load(void)
{
    (void)start_with_held_reads();
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    CHECK(!scene_view_visible_art_is_loading(scene_stage_view()));
    scene_layers_show_text(scene_stage_layers(), 1, "bubble_said", 100.0f, 80.0f, 0);
    CHECK(scene_view_visible_art_is_loading(scene_stage_view()));
    scene_layers_clear(scene_stage_layers());
    scene_layers_show_overlay(scene_stage_layers(), 1, "oval_bubble", 100.0f, 80.0f, 10.0f, 5.0f, 0);
    CHECK(scene_view_visible_art_is_loading(scene_stage_view()));
    fake_reader_hold_reads(0);
}

int main(void)
{
    test_shown_text_and_overlays_hold_the_game_while_they_load();
    test_the_bubble_waits_until_its_text_has_loaded();
    test_a_balloon_with_loaded_art_shows_at_once();
    return test_finish("unit/balloon_waits_for_its_text");
}
