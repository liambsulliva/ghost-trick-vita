#include <stdio.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "test_support.h"
#include "texture_cache.h"

static void test_requested_texture_stays_loading_until_its_read_finishes(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    entry = texture_cache_use_or_start_loading("a");
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("a");
    CHECK_INT(fake_reader_pending_count(), 1);
}

static void test_uploads_respect_the_limit_per_frame(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_refuse_submits(1);
    CHECK(texture_cache_use_or_start_loading("a") == NULL);
    fake_reader_refuse_submits(0);
    entry = texture_cache_load_now("a");
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
}

static void test_clear_waits_for_reads_in_flight(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("a");
    texture_cache_clear();
    CHECK_INT(fake_reader_pending_count(), 0);
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

static void test_loading_textures_are_never_evicted(void)
{
    const struct cached_texture *ready_texture;
    const struct cached_texture *loading_texture;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 16);
    ready_texture = texture_cache_load_now("a");
    CHECK_INT(ready_texture->state, CACHED_TEXTURE_READY);
    texture_cache_begin_frame();
    fake_reader_hold_reads(1);
    loading_texture = texture_cache_use_or_start_loading("b");
    texture_cache_begin_frame();
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(loading_texture->state, CACHED_TEXTURE_READY);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 1);
    CHECK_INT(totals.bytes_in_use, 16);
}

static void test_read_refused_by_a_full_cache_keeps_its_own_request_id(void)
{
    const struct cached_texture *entry;
    char path[64];
    char name[32];
    int index;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(path, sizeof(path), "tex/t%d.gtex", index);
        fake_files_add_rgba_texture(path, 2, 2);
    }
    fake_files_add_rgba_texture("tex/refused.gtex", 4, 4);
    fake_files_add_rgba_texture("tex/later.gtex", 2, 2);
    texture_cache_init("tex", 1024 * 1024);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        texture_cache_load_now(name);
    }
    CHECK(texture_cache_use_or_start_loading("refused") == NULL);
    texture_cache_begin_frame();
    entry = texture_cache_load_now("later");
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
    CHECK_INT(entry->width, 2);
    CHECK_INT(fake_reader_pending_count(), 0);
}

static void test_a_refused_read_evicts_nothing_from_a_full_cache(void)
{
    struct texture_cache_totals totals;
    char path[64];
    char name[32];
    int index;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(path, sizeof(path), "tex/t%d.gtex", index);
        fake_files_add_rgba_texture(path, 2, 2);
    }
    fake_files_add_rgba_texture("tex/refused.gtex", 2, 2);
    texture_cache_init("tex", 1024 * 1024);
    for (index = 0; index < TEXTURE_CACHE_ENTRY_MAX; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        texture_cache_load_now(name);
    }
    texture_cache_begin_frame();
    fake_reader_refuse_submits(1);
    CHECK(texture_cache_use_or_start_loading("refused") == NULL);
    fake_reader_refuse_submits(0);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.evictions, 0);
}

static void test_cache_loads_once_and_reuses(void)
{
    const struct cached_texture *first;
    const struct cached_texture *second;
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
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

static void test_cache_budget_counts_rounded_up_compressed_memory(void)
{
    struct texture_cache_totals totals;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_dxt5_texture("tex/a.gtex", 12, 8);
    fake_files_add_dxt5_texture("tex/b.gtex", 12, 8);
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/c.gtex", 2, 2);
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
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
    fake_files_add_rgba_texture("tex/ok.gtex", 2, 2);
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
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/b.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_load_now("a");
    texture_cache_load_now("b");
    CHECK_INT(fake_renderer_textures_alive(), 2);
    texture_cache_clear();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

int main(void)
{
    test_requested_texture_stays_loading_until_its_read_finishes();
    test_requesting_twice_only_reads_once();
    test_uploads_respect_the_limit_per_frame();
    test_refused_read_can_be_requested_again_later();
    test_clear_waits_for_reads_in_flight();
    test_loading_textures_are_never_evicted();
    test_read_refused_by_a_full_cache_keeps_its_own_request_id();
    test_a_refused_read_evicts_nothing_from_a_full_cache();
    test_cache_loads_once_and_reuses();
    test_cache_budget_counts_rounded_up_compressed_memory();
    test_cache_eviction_skips_entries_that_are_not_ready();
    test_cache_evicts_least_recently_used_when_over_budget();
    test_cache_never_evicts_what_this_frame_uses();
    test_cache_remembers_missing_files_and_only_tries_once();
    test_cache_reports_corrupt_and_failed_uploads_as_missing();
    test_cache_clear_destroys_every_texture();
    fake_files_reset();
    return test_finish("unit/texture_cache");
}
