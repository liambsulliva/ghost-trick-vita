#include <string.h>
#include "scene_stage.h"
#include "script_bridge.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "test_support.h"

#define CARD_ID 78
#define NATIVE_ALPHA_FULL 31
#define CARD_TICKS 273

static struct script_tables tables;
static const char effects[] = "effect 78 card chapter_card_12 0 0 0 0\n";

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        texture_cache_begin_frame();
        script_bridge_finish_tick();
    }
}

static const struct script_outputs *start_with_a_card(void)
{
    texture_cache_init("tex", 1u << 20);
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_effects(&tables, effects, sizeof(effects) - 1));
    script_bridge_start(&tables);
    return script_bridge_outputs();
}

static void test_a_card_over_a_long_black_mask_ends_after_its_own_fade(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_a_card();
    outputs->mask_fade_changed(1, NATIVE_ALPHA_FULL);
    outputs->effect_spawned(CARD_ID, "iEffect::CHAPTER_START_13", 0.0f, 0.0f);
    run_ticks(CARD_TICKS);
    CHECK(scene_stage_view()->card_texture[0] != '\0');
    run_ticks(1);
    CHECK(scene_stage_view()->card_texture[0] == '\0');
    CHECK(scene_stage_view()->fade_amount == 0.0f);
    run_ticks(600);
    CHECK(scene_stage_view()->card_texture[0] == '\0');
    CHECK(scene_stage_view()->fade_amount == 0.0f);
}

static void test_a_card_still_ends_when_the_mask_fade_dies_first(void)
{
    const struct script_outputs *outputs;

    outputs = start_with_a_card();
    outputs->mask_fade_changed(1, NATIVE_ALPHA_FULL);
    outputs->effect_spawned(CARD_ID, "iEffect::CHAPTER_START_01", 0.0f, 0.0f);
    run_ticks(100);
    outputs->mask_fade_changed(0, 0);
    run_ticks(1);
    CHECK(scene_stage_view()->card_texture[0] == '\0');
    CHECK(scene_stage_view()->fade_amount == 0.0f);
}

int main(void)
{
    test_a_card_over_a_long_black_mask_ends_after_its_own_fade();
    test_a_card_still_ends_when_the_mask_fade_dies_first();
    return test_finish("unit/chapter_card_ends_on_its_own");
}
