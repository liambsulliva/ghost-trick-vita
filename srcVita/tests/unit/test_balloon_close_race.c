#include <string.h>
#include "scene_layers.h"
#include "scene_stage.h"
#include "script_bridge.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "test_support.h"

#define TICKS_FOR_EVERY_PART 20
#define NO_KIND (-1)

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
        script_bridge_finish_tick();
    }
}

static const struct script_outputs *start_with_one_balloon(void)
{
    texture_cache_init("tex", 1u << 20);
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_balloons(&tables, balloons, sizeof(balloons) - 1));
    script_bridge_start(&tables);
    return script_bridge_outputs();
}

static void test_a_balloon_closed_before_its_parts_appear_leaves_nothing(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_one_balloon();
    outputs->balloon_opened(0, "said", NO_KIND, 100.0f, 80.0f);
    run_ticks(1);
    outputs->balloon_closed(0);
    run_ticks(TICKS_FOR_EVERY_PART);
    CHECK_INT(overlays_on_screen(), 0);
}

static void test_a_balloon_shows_its_parts_and_takes_them_away(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_one_balloon();
    outputs->balloon_opened(0, "said", NO_KIND, 100.0f, 80.0f);
    run_ticks(TICKS_FOR_EVERY_PART);
    CHECK_INT(overlays_on_screen(), 2);
    outputs->balloon_closed(0);
    run_ticks(TICKS_FOR_EVERY_PART);
    CHECK_INT(overlays_on_screen(), 0);
}

static void test_a_balloon_reopened_in_its_slot_keeps_its_parts(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_one_balloon();
    outputs->balloon_opened(0, "said", NO_KIND, 100.0f, 80.0f);
    run_ticks(TICKS_FOR_EVERY_PART);
    outputs->balloon_closed(0);
    outputs->balloon_opened(0, "said", NO_KIND, 120.0f, 80.0f);
    run_ticks(TICKS_FOR_EVERY_PART);
    CHECK_INT(overlays_on_screen(), 2);
}

int main(void)
{
    test_a_balloon_closed_before_its_parts_appear_leaves_nothing();
    test_a_balloon_shows_its_parts_and_takes_them_away();
    test_a_balloon_reopened_in_its_slot_keeps_its_parts();
    return test_finish("unit/balloon_close_race");
}
