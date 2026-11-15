#include <stdio.h>
#include "asset_manifest.h"
#include "test_support.h"

static void test_manifest_lists_assets(void)
{
    const char text[] = "scene opening\nbackground room\n\nanimation walk\nanimation idle\n";
    static struct asset_manifest manifest;

    CHECK_INT(asset_manifest_parse(text, sizeof(text) - 1, &manifest), ASSET_MANIFEST_OK);
    CHECK_INT(manifest.scene_count, 1);
    CHECK_TEXT(manifest.scene_names[0], "opening");
    CHECK_INT(manifest.background_count, 1);
    CHECK_INT(manifest.animation_count, 2);
    CHECK_INT(asset_manifest_find_animation(&manifest, "idle"), 1);
    CHECK_INT(asset_manifest_find_animation(&manifest, "nope"), -1);
    CHECK_INT(asset_manifest_find_background(&manifest, "room"), 0);
    CHECK_INT(asset_manifest_find_background(&manifest, "nope"), -1);
}

static void test_manifest_rejects_unknown_lines(void)
{
    const char text[] = "music theme\n";
    static struct asset_manifest manifest;

    CHECK_INT(asset_manifest_parse(text, sizeof(text) - 1, &manifest), ASSET_MANIFEST_ERR_BAD_LINE);
}

static void test_a_list_longer_than_its_table_keeps_the_set(void)
{
    static char text[(ASSET_MANIFEST_ANIMATION_MAX + 40) * 24];
    static struct asset_manifest manifest;
    unsigned int length;
    int index;

    length = (unsigned int)snprintf(text, sizeof(text), "art ds\n");
    for (index = 0; index < ASSET_MANIFEST_ANIMATION_MAX + 40; index++) {
        length = length + (unsigned int)snprintf(&text[length], sizeof(text) - length, "animation walk_%d\n", index);
    }
    CHECK_INT(asset_manifest_parse(text, length, &manifest), ASSET_MANIFEST_OK);
    CHECK_INT(manifest.animation_count, ASSET_MANIFEST_ANIMATION_MAX);
    CHECK_INT(manifest.names_left_out, 40);
    CHECK_INT(manifest.art_sets, ASSET_ART_DS);
    CHECK_INT(asset_manifest_find_animation(&manifest, "walk_0"), 0);
    CHECK_INT(asset_manifest_find_animation(&manifest, "walk_540"), -1);
}

static void test_a_line_past_the_table_must_still_name_something(void)
{
    static char text[(ASSET_MANIFEST_SCENE_MAX + 2) * 24];
    static struct asset_manifest manifest;
    unsigned int length;
    int index;

    length = 0;
    for (index = 0; index < ASSET_MANIFEST_SCENE_MAX; index++) {
        length = length + (unsigned int)snprintf(&text[length], sizeof(text) - length, "scene part_%d\n", index);
    }
    length = length + (unsigned int)snprintf(&text[length], sizeof(text) - length, "scene \n");
    CHECK_INT(asset_manifest_parse(text, length, &manifest), ASSET_MANIFEST_ERR_BAD_LINE);
}

static void test_manifest_lists_the_bundled_art_sets(void)
{
    const char both[] = "art remastered\nart ds\nbackground room\n";
    const char ds_only[] = "art ds\n";
    const char none[] = "background room\n";
    const char unknown[] = "art gba\n";
    static struct asset_manifest manifest;

    CHECK_INT(asset_manifest_parse(both, sizeof(both) - 1, &manifest), ASSET_MANIFEST_OK);
    CHECK_INT(manifest.art_sets, ASSET_ART_REMASTERED | ASSET_ART_DS);
    CHECK_INT(manifest.background_count, 1);
    CHECK_INT(asset_manifest_parse(ds_only, sizeof(ds_only) - 1, &manifest), ASSET_MANIFEST_OK);
    CHECK_INT(manifest.art_sets, ASSET_ART_DS);
    CHECK_INT(asset_manifest_parse(none, sizeof(none) - 1, &manifest), ASSET_MANIFEST_OK);
    CHECK_INT(manifest.art_sets, 0);
    CHECK_INT(asset_manifest_parse(unknown, sizeof(unknown) - 1, &manifest), ASSET_MANIFEST_ERR_BAD_LINE);
}

int main(void)
{
    test_manifest_lists_assets();
    test_manifest_rejects_unknown_lines();
    test_a_list_longer_than_its_table_keeps_the_set();
    test_a_line_past_the_table_must_still_name_something();
    test_manifest_lists_the_bundled_art_sets();
    return test_finish("unit/asset_manifest");
}
