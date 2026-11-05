#include <stdlib.h>
#include "animation.h"
#include "asset_manifest.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "test_support.h"
#include "text_lines.h"
#include "texture_cache.h"
#include "texture_file.h"

static void write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
    bytes[2] = (unsigned char)((value >> 16) & 0xff);
    bytes[3] = (unsigned char)((value >> 24) & 0xff);
}

static void add_texture_file(const char *path, unsigned int width, unsigned int height)
{
    unsigned int pixel_bytes;
    unsigned char *file;

    pixel_bytes = width * height * 4;
    file = calloc(1, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    file[0] = 'G';
    file[1] = 'T';
    file[2] = 'E';
    file[3] = 'X';
    write_u32(&file[4], width);
    write_u32(&file[8], height);
    write_u32(&file[12], TEXTURE_FILE_FORMAT_RGBA8888);
    write_u32(&file[16], pixel_bytes);
    fake_files_add(path, file, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    free(file);
}

static void test_text_lines_handle_endings_and_long_lines(void)
{
    const char text[] = "one\r\ntwo\n\nabcdefghij";
    char line[6];
    unsigned int offset;

    offset = text_lines_read_next(text, sizeof(text) - 1, 0, line, sizeof(line));
    CHECK_TEXT(line, "one");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "two");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "");
    offset = text_lines_read_next(text, sizeof(text) - 1, offset, line, sizeof(line));
    CHECK_TEXT(line, "abcde");
    CHECK_INT(offset, sizeof(text) - 1);
}

static void test_texture_header_accepts_a_valid_file(void)
{
    const unsigned char *file;
    unsigned int size;
    struct texture_file_header header;

    fake_files_reset();
    add_texture_file("t.gtex", 4, 2);
    file = fake_files_contents("t.gtex", &size);
    CHECK_INT(texture_file_read_header(file, size, &header), TEXTURE_FILE_OK);
    CHECK_INT(header.width, 4);
    CHECK_INT(header.height, 2);
    CHECK_INT(header.pixel_data_size, 32);
}

static void test_texture_header_rejects_bad_files(void)
{
    unsigned char file[TEXTURE_FILE_HEADER_SIZE + 16];
    struct texture_file_header header;

    memset(file, 0, sizeof(file));
    CHECK_INT(texture_file_read_header(file, 3, &header), TEXTURE_FILE_ERR_TOO_SHORT);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_MAGIC);

    memcpy(file, "GTEX", 4);
    write_u32(&file[4], 2);
    write_u32(&file[8], 2);
    write_u32(&file[12], 99);
    write_u32(&file[16], 16);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_FORMAT);

    write_u32(&file[12], TEXTURE_FILE_FORMAT_RGBA8888);
    write_u32(&file[16], 15);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);

    write_u32(&file[16], 16);
    CHECK_INT(texture_file_read_header(file, sizeof(file) - 1, &header), TEXTURE_FILE_ERR_TOO_SHORT);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    write_u32(&file[4], 0);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);

    write_u32(&file[4], 65536);
    write_u32(&file[8], 65536);
    write_u32(&file[16], 0);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
}

static void test_animation_parses_all_fields(void)
{
    const char text[] =
        "animation walk\n"
        "ticks_per_second 30\n"
        "loops 1\n"
        "frame walk_00 2 10 20 100 200\n"
        "\n"
        "frame walk_01 3 -5.5 21 100 200\n";
    static struct animation animation;

    CHECK_INT(animation_parse(text, sizeof(text) - 1, &animation), ANIMATION_OK);
    CHECK_TEXT(animation.name, "walk");
    CHECK_INT(animation.ticks_per_second, 30);
    CHECK_INT(animation.loops, 1);
    CHECK_INT(animation.frame_count, 2);
    CHECK_TEXT(animation.frames[1].texture_name, "walk_01");
    CHECK_INT(animation.frames[1].ticks, 3);
    CHECK(animation.frames[1].x == -5.5f);
}

static void test_animation_rejects_bad_input(void)
{
    static struct animation animation;
    const char empty[] = "animation empty\n";
    const char bad_line[] = "dance\n";
    const char missing_field[] = "frame a 1 2 3\n";
    const char zero_ticks[] = "frame a 0 0 0 1 1\n";
    const char zero_rate[] = "ticks_per_second 0\n";
    char *too_many;
    unsigned int index;
    unsigned int used;

    CHECK_INT(animation_parse(empty, sizeof(empty) - 1, &animation), ANIMATION_ERR_EMPTY);
    CHECK_INT(animation_parse(bad_line, sizeof(bad_line) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(missing_field, sizeof(missing_field) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(zero_ticks, sizeof(zero_ticks) - 1, &animation), ANIMATION_ERR_BAD_TICKS);
    CHECK_INT(animation_parse(zero_rate, sizeof(zero_rate) - 1, &animation), ANIMATION_ERR_BAD_LINE);

    too_many = malloc((ANIMATION_FRAME_MAX + 1) * 20);
    used = 0;
    for (index = 0; index < ANIMATION_FRAME_MAX + 1; index++) {
        used = used + (unsigned int)sprintf(&too_many[used], "frame a 1 0 0 1 1\n");
    }
    CHECK_INT(animation_parse(too_many, used, &animation), ANIMATION_ERR_TOO_MANY_FRAMES);
    free(too_many);
}

static void make_two_frame_animation(struct animation *animation, int loops)
{
    const char text[] = "ticks_per_second 10\nframe a 1 0 0 1 1\nframe b 2 0 0 1 1\n";

    animation_parse(text, sizeof(text) - 1, animation);
    animation->loops = loops;
}

static void test_player_holds_each_frame_for_its_ticks(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 99999);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 1);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 199999);
    CHECK_INT(player.frame_index, 1);
    CHECK(!player.is_finished);
    animation_player_advance(&player, 1);
    CHECK(player.is_finished);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 1000000);
    CHECK_INT(player.frame_index, 1);
}

static void test_player_loops_and_catches_up_after_a_stall(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 1);
    animation_player_start(&player, &animation);
    animation_player_advance(&player, 350000);
    CHECK_INT(player.frame_index, 0);
    CHECK_INT(player.time_in_frame_us, 50000);
    CHECK(!player.is_finished);
}

static void test_player_pause_stops_time(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 1);
    animation_player_start(&player, &animation);
    player.is_paused = 1;
    animation_player_advance(&player, 900000);
    CHECK_INT(player.frame_index, 0);
    CHECK_INT(player.time_in_frame_us, 0);
}

static void test_player_tracks_which_frames_were_shown(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    CHECK_INT(animation_player_count_shown_frames(&player), 1);
    animation_player_advance(&player, 100000);
    CHECK_INT(animation_player_count_shown_frames(&player), 2);
}

static void test_player_without_animation_is_harmless(void)
{
    static struct animation_player player;

    player.animation = NULL;
    animation_player_advance(&player, 1000);
    CHECK(animation_player_current_frame(&player) == NULL);
    CHECK_INT(animation_player_count_shown_frames(&player), 0);
}

static void test_manifest_lists_assets(void)
{
    const char text[] = "background room\n\nanimation walk\nanimation idle\n";
    static struct asset_manifest manifest;

    CHECK_INT(asset_manifest_parse(text, sizeof(text) - 1, &manifest), ASSET_MANIFEST_OK);
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

static void test_cache_loads_once_and_reuses(void)
{
    const struct cached_texture *first;
    const struct cached_texture *second;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    first = texture_cache_get("a");
    second = texture_cache_get("a");
    CHECK(first == second);
    CHECK(!first->is_missing);
    CHECK_INT(fake_renderer_textures_created(), 1);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.loads, 1);
    CHECK_INT(totals.bytes_in_use, 16);
}

static void test_cache_evicts_least_recently_used_when_over_budget(void)
{
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    add_texture_file("tex/c.gtex", 2, 2);
    texture_cache_init("tex", 32);
    texture_cache_get("a");
    texture_cache_begin_frame();
    texture_cache_get("b");
    texture_cache_begin_frame();
    texture_cache_get("a");
    texture_cache_begin_frame();
    texture_cache_get("c");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.bytes_in_use, 32);
    CHECK_INT(fake_renderer_textures_alive(), 2);
    texture_cache_get("a");
    CHECK_INT(fake_renderer_textures_created(), 3);
    texture_cache_get("b");
    CHECK_INT(fake_renderer_textures_created(), 4);
}

static void test_cache_never_evicts_what_this_frame_uses(void)
{
    const struct cached_texture *first;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 16);
    first = texture_cache_get("a");
    texture_cache_get("b");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 0);
    CHECK_INT(totals.bytes_in_use, 32);
    CHECK(first->texture != RENDERER_NO_TEXTURE);
}

static void test_cache_remembers_missing_files_and_only_tries_once(void)
{
    const struct cached_texture *entry;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    texture_cache_init("tex", 1024);
    entry = texture_cache_get("gone");
    CHECK(entry != NULL);
    CHECK(entry->is_missing);
    texture_cache_get("gone");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.missing, 1);
    CHECK_INT(fake_renderer_textures_created(), 0);
}

static void test_cache_reports_corrupt_and_failed_uploads_as_missing(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_files_add("tex/bad.gtex", (const unsigned char *)"not a texture file at all", 25);
    add_texture_file("tex/ok.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    entry = texture_cache_get("bad");
    CHECK(entry->is_missing);

    fake_renderer_fail_uploads(1);
    entry = texture_cache_get("ok");
    CHECK(entry->is_missing);
    fake_renderer_fail_uploads(0);
}

static void test_cache_clear_destroys_every_texture(void)
{
    fake_files_reset();
    fake_renderer_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_get("a");
    texture_cache_get("b");
    CHECK_INT(fake_renderer_textures_alive(), 2);
    texture_cache_clear();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

int main(void)
{
    test_text_lines_handle_endings_and_long_lines();
    test_texture_header_accepts_a_valid_file();
    test_texture_header_rejects_bad_files();
    test_animation_parses_all_fields();
    test_animation_rejects_bad_input();
    test_player_holds_each_frame_for_its_ticks();
    test_player_loops_and_catches_up_after_a_stall();
    test_player_pause_stops_time();
    test_player_tracks_which_frames_were_shown();
    test_player_without_animation_is_harmless();
    test_manifest_lists_assets();
    test_manifest_rejects_unknown_lines();
    test_cache_loads_once_and_reuses();
    test_cache_evicts_least_recently_used_when_over_budget();
    test_cache_never_evicts_what_this_frame_uses();
    test_cache_remembers_missing_files_and_only_tries_once();
    test_cache_reports_corrupt_and_failed_uploads_as_missing();
    test_cache_clear_destroys_every_texture();
    fake_files_reset();
    return test_finish("unit/assets");
}
