#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include "asset_manifest.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "scene.h"
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

static void test_texture_header_checks_compressed_sizes(void)
{
    unsigned char file[TEXTURE_FILE_HEADER_SIZE + 64];
    struct texture_file_header header;

    memset(file, 0, sizeof(file));
    memcpy(file, "GTEX", 4);
    write_u32(&file[4], 8);
    write_u32(&file[8], 8);
    write_u32(&file[12], TEXTURE_FILE_FORMAT_DXT5);
    write_u32(&file[16], 64);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    write_u32(&file[12], TEXTURE_FILE_FORMAT_DXT1);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
    write_u32(&file[16], 32);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_OK);

    write_u32(&file[4], 6);
    write_u32(&file[16], 24);
    CHECK_INT(texture_file_read_header(file, sizeof(file), &header), TEXTURE_FILE_ERR_BAD_SIZE);
}

static void test_compressed_memory_is_charged_at_power_of_two_size(void)
{
    struct texture_file_header header;

    header.width = 720;
    header.height = 540;
    header.format = TEXTURE_FILE_FORMAT_DXT1;
    header.pixel_data_size = 194400;
    CHECK_INT(texture_file_memory_bytes(&header), 524288);

    header.width = 2880;
    header.height = 2880;
    header.format = TEXTURE_FILE_FORMAT_DXT5;
    header.pixel_data_size = 8294400;
    CHECK_INT(texture_file_memory_bytes(&header), 16777216);

    header.width = 16;
    header.height = 8;
    header.pixel_data_size = 128;
    CHECK_INT(texture_file_memory_bytes(&header), 128);

    header.width = 3;
    header.height = 3;
    header.format = TEXTURE_FILE_FORMAT_RGBA8888;
    header.pixel_data_size = 36;
    CHECK_INT(texture_file_memory_bytes(&header), 36);
}

static void test_animation_parses_all_fields(void)
{
    const char text[] =
        "animation walk\n"
        "ticks_per_second 30\n"
        "loops 1\n"
        "frame walk_00 2 10 20 100 200 0 0 0.25 0.5\n"
        "\n"
        "frame walk_01 3 -5.5 21 100 200 0.25 0.5 0.25 0.5\n"
        "frame - 1 0 0 0 0 0 0 0 0\n";
    static struct animation animation;

    CHECK_INT(animation_parse(text, sizeof(text) - 1, &animation), ANIMATION_OK);
    CHECK_TEXT(animation.name, "walk");
    CHECK_INT(animation.ticks_per_second, 30);
    CHECK_INT(animation.loops, 1);
    CHECK_INT(animation.frame_count, 3);
    CHECK(animation.frames[1].u == 0.25f);
    CHECK(animation.frames[1].v_size == 0.5f);
    CHECK(!animation_frame_is_blank(&animation.frames[1]));
    CHECK(animation_frame_is_blank(&animation.frames[2]));
    CHECK_TEXT(animation.frames[1].texture_name, "walk_01");
    CHECK_INT(animation.frames[1].ticks, 3);
    CHECK(animation.frames[1].x == -5.5f);
}

static void test_animation_rejects_bad_input(void)
{
    static struct animation animation;
    const char empty[] = "animation empty\n";
    const char bad_line[] = "dance\n";
    const char missing_field[] = "frame a 1 2 3 4 5\n";
    const char zero_ticks[] = "frame a 0 0 0 1 1 0 0 1 1\n";
    const char zero_rate[] = "ticks_per_second 0\n";
    char *too_many;
    unsigned int index;
    unsigned int used;

    CHECK_INT(animation_parse(empty, sizeof(empty) - 1, &animation), ANIMATION_ERR_EMPTY);
    CHECK_INT(animation_parse(bad_line, sizeof(bad_line) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(missing_field, sizeof(missing_field) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(zero_ticks, sizeof(zero_ticks) - 1, &animation), ANIMATION_ERR_BAD_TICKS);
    CHECK_INT(animation_parse(zero_rate, sizeof(zero_rate) - 1, &animation), ANIMATION_ERR_BAD_LINE);

    too_many = malloc((ANIMATION_FRAME_MAX + 1) * 40);
    used = 0;
    for (index = 0; index < ANIMATION_FRAME_MAX + 1; index++) {
        used = used + (unsigned int)sprintf(&too_many[used], "frame a 1 0 0 1 1 0 0 1 1\n");
    }
    CHECK_INT(animation_parse(too_many, used, &animation), ANIMATION_ERR_TOO_MANY_FRAMES);
    free(too_many);
}

static void make_two_frame_animation(struct animation *animation, int loops)
{
    const char text[] = "ticks_per_second 10\nframe a 1 0 0 1 1 0 0 1 1\nframe b 2 0 0 1 1 0 0 1 1\n";

    animation_parse(text, sizeof(text) - 1, animation);
    animation->loops = loops;
}

static int frame_is_never_ready(const struct animation_frame *frame)
{
    (void)frame;
    return 0;
}

static int frame_is_always_ready(const struct animation_frame *frame)
{
    (void)frame;
    return 1;
}

static void test_player_holds_each_frame_for_its_ticks(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 99999, NULL);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 1, NULL);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 199999, NULL);
    CHECK_INT(player.frame_index, 1);
    CHECK(!player.is_finished);
    animation_player_advance(&player, 1, NULL);
    CHECK(player.is_finished);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 1000000, NULL);
    CHECK_INT(player.frame_index, 1);
}

static void test_player_loops_and_catches_up_after_a_stall(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 1);
    animation_player_start(&player, &animation);
    animation_player_advance(&player, 350000, NULL);
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
    animation_player_advance(&player, 900000, NULL);
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
    animation_player_advance(&player, 100000, NULL);
    CHECK_INT(animation_player_count_shown_frames(&player), 2);
}

static void test_player_without_animation_is_harmless(void)
{
    static struct animation_player player;

    player.animation = NULL;
    animation_player_advance(&player, 1000, NULL);
    CHECK(animation_player_current_frame(&player) == NULL);
    CHECK_INT(animation_player_count_shown_frames(&player), 0);
}

static void test_player_holds_the_current_frame_until_the_next_is_ready(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    animation_player_advance(&player, 500000, frame_is_never_ready);
    CHECK_INT(player.frame_index, 0);
    CHECK_INT(player.late_frame_count, 1);
    animation_player_advance(&player, 500000, frame_is_never_ready);
    CHECK_INT(player.late_frame_count, 1);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 0, frame_is_always_ready);
    CHECK_INT(player.frame_index, 1);
    CHECK_INT(player.time_in_frame_us, 0);
    CHECK_INT(player.late_frame_count, 1);
    CHECK_INT(animation_player_count_shown_frames(&player), 2);
}

static void test_requested_texture_stays_loading_until_its_read_finishes(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    entry = texture_cache_request("a");
    CHECK_INT(entry->state, CACHED_TEXTURE_LOADING);
    CHECK_INT(fake_renderer_textures_created(), 0);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(entry->state, CACHED_TEXTURE_LOADING);
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
    CHECK_INT(fake_renderer_textures_created(), 1);
}

static void test_requesting_twice_only_reads_once(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_request("a");
    texture_cache_request("a");
    CHECK_INT(fake_reader_pending_count(), 1);
}

static void test_uploads_respect_the_limit_per_frame(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_request("a");
    texture_cache_request("b");
    texture_cache_upload_finished_reads(1);
    CHECK_INT(fake_renderer_textures_created(), 1);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(fake_renderer_textures_created(), 2);
}

static void test_refused_read_can_be_requested_again_later(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_refuse_submits(1);
    CHECK(texture_cache_request("a") == NULL);
    fake_reader_refuse_submits(0);
    entry = texture_cache_load_now("a");
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
}

static void test_clear_waits_for_reads_in_flight(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_request("a");
    texture_cache_clear();
    CHECK_INT(fake_reader_pending_count(), 0);
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_loading_textures_are_never_evicted(void)
{
    const struct cached_texture *loading;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 16);
    loading = texture_cache_load_now("a");
    CHECK_INT(loading->state, CACHED_TEXTURE_READY);
    texture_cache_begin_frame();
    fake_reader_hold_reads(1);
    loading = texture_cache_request("b");
    texture_cache_begin_frame();
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(loading->state, CACHED_TEXTURE_READY);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.bytes_in_use, 16);
}

static int parse_scene_text(const char *text)
{
    static struct scene scene;

    return scene_parse(text, (unsigned int)strlen(text), &scene);
}

static void test_scene_parse_reads_every_kind_of_step(void)
{
    const char text[] =
        "scene opening\n"
        "side_bars left right\n"
        "room 11\n"
        "plate st01 0 384 256 192\n"
        "object linne idle 281.5 353 900 1\n"
        "remove_object linne\n"
        "camera 0 192\n"
        "camera_pan 768 224 600\n"
        "card chapter\n"
        "hide_card\n"
        "fade_out 20\n"
        "fade_in 40\n"
        "wait 90\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.step_count, 12);
    CHECK_INT(scene.steps[3].kind, SCENE_STEP_OBJECT);
    CHECK_INT(scene.steps[6].kind, SCENE_STEP_CAMERA_PAN);
    CHECK_INT(scene.steps[6].number, 600);
    CHECK_INT(scene.steps[11].kind, SCENE_STEP_WAIT);
    CHECK_INT(scene.steps[11].number, 90);
}

static void test_scene_parse_rejects_durations_that_are_not_positive(void)
{
    CHECK_INT(parse_scene_text("scene s\ncamera_pan 0 0 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nfade_out 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nfade_in -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nwait 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nnot_a_step 1\n"), SCENE_ERR_BAD_LINE);
}

static void test_scene_parse_stops_at_the_step_limit(void)
{
    static char text[32 + (SCENE_STEP_MAX + 1) * 10];
    size_t used;
    int index;

    used = (size_t)sprintf(text, "scene s\n");
    for (index = 0; index < SCENE_STEP_MAX; index++) {
        used = used + (size_t)sprintf(&text[used], "hide_card\n");
    }
    CHECK_INT(parse_scene_text(text), SCENE_OK);
    sprintf(&text[used], "hide_card\n");
    CHECK_INT(parse_scene_text(text), SCENE_ERR_TOO_MANY_STEPS);
}

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

static void test_cache_loads_once_and_reuses(void)
{
    const struct cached_texture *first;
    const struct cached_texture *second;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    first = texture_cache_load_now("a");
    second = texture_cache_load_now("a");
    CHECK(first == second);
    CHECK(first->state != CACHED_TEXTURE_MISSING);
    CHECK_INT(fake_renderer_textures_created(), 1);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.loads, 1);
    CHECK_INT(totals.bytes_in_use, 16);
}

static void add_dxt5_texture_file(const char *path, unsigned int width, unsigned int height)
{
    unsigned int pixel_bytes;
    unsigned char *file;

    pixel_bytes = (width / 4) * (height / 4) * 16;
    file = calloc(1, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    memcpy(file, "GTEX", 4);
    write_u32(&file[4], width);
    write_u32(&file[8], height);
    write_u32(&file[12], TEXTURE_FILE_FORMAT_DXT5);
    write_u32(&file[16], pixel_bytes);
    fake_files_add(path, file, TEXTURE_FILE_HEADER_SIZE + pixel_bytes);
    free(file);
}

static void test_cache_budget_counts_rounded_up_compressed_memory(void)
{
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_dxt5_texture_file("tex/a.gtex", 12, 8);
    add_dxt5_texture_file("tex/b.gtex", 12, 8);
    texture_cache_init("tex", 230);
    texture_cache_load_now("a");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.bytes_in_use, 128);
    texture_cache_begin_frame();
    texture_cache_load_now("b");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.bytes_in_use, 128);
}

static void test_cache_eviction_skips_entries_that_are_not_ready(void)
{
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 16);
    texture_cache_load_now("not_there");
    texture_cache_begin_frame();
    texture_cache_load_now("a");
    texture_cache_begin_frame();
    texture_cache_load_now("b");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.missing, 1);
}

static void test_cache_evicts_least_recently_used_when_over_budget(void)
{
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    add_texture_file("tex/c.gtex", 2, 2);
    texture_cache_init("tex", 32);
    texture_cache_load_now("a");
    texture_cache_begin_frame();
    texture_cache_load_now("b");
    texture_cache_begin_frame();
    texture_cache_load_now("a");
    texture_cache_begin_frame();
    texture_cache_load_now("c");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.bytes_in_use, 32);
    CHECK_INT(fake_renderer_textures_alive(), 2);
    texture_cache_load_now("a");
    CHECK_INT(fake_renderer_textures_created(), 3);
    texture_cache_load_now("b");
    CHECK_INT(fake_renderer_textures_created(), 4);
}

static void test_cache_never_evicts_what_this_frame_uses(void)
{
    const struct cached_texture *first;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 16);
    first = texture_cache_load_now("a");
    texture_cache_load_now("b");
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
    fake_reader_reset();
    texture_cache_init("tex", 1024);
    entry = texture_cache_load_now("gone");
    CHECK(entry != NULL);
    CHECK(entry->state == CACHED_TEXTURE_MISSING);
    texture_cache_load_now("gone");
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.missing, 1);
    CHECK_INT(fake_renderer_textures_created(), 0);
}

static void test_cache_reports_corrupt_and_failed_uploads_as_missing(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add("tex/bad.gtex", (const unsigned char *)"not a texture file at all", 25);
    add_texture_file("tex/ok.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    entry = texture_cache_load_now("bad");
    CHECK(entry->state == CACHED_TEXTURE_MISSING);

    fake_renderer_fail_uploads(1);
    entry = texture_cache_load_now("ok");
    CHECK(entry->state == CACHED_TEXTURE_MISSING);
    fake_renderer_fail_uploads(0);
}

static void test_cache_clear_destroys_every_texture(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_texture_file("tex/a.gtex", 2, 2);
    add_texture_file("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_load_now("a");
    texture_cache_load_now("b");
    CHECK_INT(fake_renderer_textures_alive(), 2);
    texture_cache_clear();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

int main(void)
{
    test_text_lines_handle_endings_and_long_lines();
    test_texture_header_accepts_a_valid_file();
    test_texture_header_rejects_bad_files();
    test_texture_header_checks_compressed_sizes();
    test_compressed_memory_is_charged_at_power_of_two_size();
    test_animation_parses_all_fields();
    test_animation_rejects_bad_input();
    test_player_holds_each_frame_for_its_ticks();
    test_player_loops_and_catches_up_after_a_stall();
    test_player_pause_stops_time();
    test_player_tracks_which_frames_were_shown();
    test_player_without_animation_is_harmless();
    test_player_holds_the_current_frame_until_the_next_is_ready();
    test_requested_texture_stays_loading_until_its_read_finishes();
    test_requesting_twice_only_reads_once();
    test_uploads_respect_the_limit_per_frame();
    test_refused_read_can_be_requested_again_later();
    test_clear_waits_for_reads_in_flight();
    test_loading_textures_are_never_evicted();
    test_scene_parse_reads_every_kind_of_step();
    test_scene_parse_rejects_durations_that_are_not_positive();
    test_scene_parse_stops_at_the_step_limit();
    test_manifest_lists_assets();
    test_manifest_rejects_unknown_lines();
    test_cache_loads_once_and_reuses();
    test_cache_budget_counts_rounded_up_compressed_memory();
    test_cache_eviction_skips_entries_that_are_not_ready();
    test_cache_evicts_least_recently_used_when_over_budget();
    test_cache_never_evicts_what_this_frame_uses();
    test_cache_remembers_missing_files_and_only_tries_once();
    test_cache_reports_corrupt_and_failed_uploads_as_missing();
    test_cache_clear_destroys_every_texture();
    fake_files_reset();
    return test_finish("unit/assets");
}
