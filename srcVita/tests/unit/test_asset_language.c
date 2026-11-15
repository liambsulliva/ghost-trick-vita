#include <stdio.h>
#include <string.h>
#include "asset_language.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "file.h"
#include "file_path.h"
#include "game_settings.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRENCH 2
#define JAPANESE 1
#define NAMES "textures/text_m01_0000_0.gtex\nscripts/talk.txt\npause/widgets.txt\n"

static void add_french_names(void)
{
    fake_files_reset();
    fake_files_add("assets/lang/fr/names.txt", (const unsigned char *)NAMES, (unsigned int)strlen(NAMES));
    asset_language_init("assets");
}

static void test_a_language_overrides_only_the_paths_its_names_list(void)
{
    char path[FILE_PATH_MAX];

    add_french_names();
    CHECK_INT(asset_language_use(FRENCH), 3);
    CHECK_INT(asset_language_now(), FRENCH);
    CHECK(asset_language_overrides("scripts/talk.txt"));
    CHECK(!asset_language_overrides("scripts/rooms.txt"));
    CHECK(asset_language_join(path, "assets", "pause/widgets.txt"));
    CHECK_TEXT(path, "assets/lang/fr/pause/widgets.txt");
    CHECK(asset_language_join(path, "assets", "pause/labels.txt"));
    CHECK_TEXT(path, "assets/pause/labels.txt");
    CHECK(asset_language_overrides_texture("text_m01_0000_0"));
    CHECK(!asset_language_overrides_texture("st01-00-02"));
}

static void test_english_and_a_language_without_files_use_the_base_set(void)
{
    char path[FILE_PATH_MAX];

    add_french_names();
    CHECK_INT(asset_language_use(ASSET_LANGUAGE_ENGLISH), 0);
    CHECK(asset_language_join(path, "assets", "pause/widgets.txt"));
    CHECK_TEXT(path, "assets/pause/widgets.txt");
    CHECK_INT(asset_language_use(JAPANESE), 0);
    CHECK(!asset_language_overrides("scripts/talk.txt"));
    CHECK_INT(asset_language_use(ASSET_LANGUAGE_COUNT), 0);
    CHECK_INT(asset_language_now(), ASSET_LANGUAGE_ENGLISH);
}

static void test_the_system_language_picks_the_matching_game_language(void)
{
    CHECK_INT(asset_language_from_system(0), JAPANESE);
    CHECK_INT(asset_language_from_system(1), ASSET_LANGUAGE_ENGLISH);
    CHECK_INT(asset_language_from_system(2), FRENCH);
    CHECK_INT(asset_language_from_system(3), 5);
    CHECK_INT(asset_language_from_system(4), 4);
    CHECK_INT(asset_language_from_system(5), 3);
    CHECK_INT(asset_language_from_system(9), 8);
    CHECK_INT(asset_language_from_system(10), 6);
    CHECK_INT(asset_language_from_system(11), 7);
    CHECK_INT(asset_language_from_system(17), ASSET_LANGUAGE_ENGLISH);
    CHECK_INT(asset_language_from_system(-1), ASSET_LANGUAGE_ENGLISH);
    CHECK_TEXT(asset_language_code(7), "zhcn");
    CHECK_TEXT(asset_language_code(99), "en");
}

static void test_a_first_boot_starts_in_the_system_language(void)
{
    struct game_settings settings;

    game_settings_set_default_language(asset_language_from_system(9));
    game_settings_defaults(&settings);
    CHECK_INT(settings.language, 8);
    CHECK_INT(game_settings_read_text(&settings, "language 0\n", 11u), 1);
    CHECK_INT(settings.language, ASSET_LANGUAGE_ENGLISH);
    game_settings_set_default_language(ASSET_LANGUAGE_ENGLISH);
}

static void test_the_texture_cache_reads_a_language_texture_from_its_folder(void)
{
    add_french_names();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("assets/textures/text_m01_0000_0.gtex", 2, 2);
    fake_files_add_rgba_texture("assets/lang/fr/textures/text_m01_0000_0.gtex", 2, 2);
    fake_files_add_rgba_texture("assets/textures/plate.gtex", 2, 2);
    texture_cache_init("assets/textures", 1u << 20);
    texture_cache_use_language(FRENCH);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("text_m01_0000_0");
    texture_cache_use_or_start_loading("plate");
    CHECK_TEXT(fake_reader_pending_path(0), "assets/lang/fr/textures/text_m01_0000_0.gtex");
    CHECK_TEXT(fake_reader_pending_path(1), "assets/textures/plate.gtex");
    fake_reader_release_reads(2);
    texture_cache_upload_finished_reads(2);
    texture_cache_use_language(ASSET_LANGUAGE_ENGLISH);
    CHECK_INT(texture_cache_loading_count(), 0);
    texture_cache_use_or_start_loading("text_m01_0000_0");
    CHECK_INT(texture_cache_loading_count(), 1);
    CHECK_TEXT(fake_reader_pending_path(0), "assets/textures/text_m01_0000_0.gtex");
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
}

static void test_a_language_texture_is_read_from_its_bucket_like_the_base_set(void)
{
    add_french_names();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("assets/lang/fr/textures/2e/text_m01_0000_0.gtex", 2, 2);
    fake_files_add_rgba_texture("assets/textures/1b/plate.gtex", 2, 2);
    texture_cache_init("assets/textures", 1u << 20);
    texture_cache_set_bucket_count(64u);
    texture_cache_use_language(FRENCH);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("text_m01_0000_0");
    texture_cache_use_or_start_loading("plate");
    CHECK_TEXT(fake_reader_pending_path(0), "assets/lang/fr/textures/2e/text_m01_0000_0.gtex");
    CHECK_TEXT(fake_reader_pending_path(1), "assets/textures/1b/plate.gtex");
    fake_reader_release_reads(2);
    texture_cache_upload_finished_reads(2);
    texture_cache_use_language(ASSET_LANGUAGE_ENGLISH);
    texture_cache_set_bucket_count(0u);
}

static void test_a_set_carries_only_the_languages_its_list_names(void)
{
    static const char listed[] = "fr\nde\nxx\n";

    fake_files_reset();
    fake_files_add("assets/lang/languages.txt", (const unsigned char *)listed, (unsigned int)strlen(listed));
    asset_language_init("assets");
    CHECK_INT(asset_language_carried_count(), 3);
    CHECK(asset_language_carries(ASSET_LANGUAGE_ENGLISH));
    CHECK(asset_language_carries(FRENCH));
    CHECK(!asset_language_carries(JAPANESE));
    CHECK_INT(asset_language_carried_at(1), FRENCH);
    CHECK_INT(asset_language_carried_at(2), 4);
    CHECK_INT(asset_language_position_of(4), 2);
    CHECK_INT(asset_language_carried_or_english(8), ASSET_LANGUAGE_ENGLISH);
    CHECK_INT(asset_language_from_system(0), ASSET_LANGUAGE_ENGLISH);
    CHECK_INT(asset_language_from_system(2), FRENCH);
    CHECK_INT(asset_language_use(JAPANESE), 0);
    CHECK_INT(asset_language_now(), ASSET_LANGUAGE_ENGLISH);
    fake_files_reset();
    asset_language_init("assets");
    CHECK_INT(asset_language_carried_count(), ASSET_LANGUAGE_COUNT);
}

int main(void)
{
    test_a_language_overrides_only_the_paths_its_names_list();
    test_english_and_a_language_without_files_use_the_base_set();
    test_the_system_language_picks_the_matching_game_language();
    test_a_first_boot_starts_in_the_system_language();
    test_a_set_carries_only_the_languages_its_list_names();
    test_the_texture_cache_reads_a_language_texture_from_its_folder();
    test_a_language_texture_is_read_from_its_bucket_like_the_base_set();
    return test_finish("unit/asset_language");
}
