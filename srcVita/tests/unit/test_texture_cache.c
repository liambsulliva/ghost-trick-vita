#include <stdio.h>
#include <string.h>
#include "background_reader.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "file.h"
#include "run_log.h"
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

static void test_grayscale_name_reads_the_plain_file(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    entry = texture_cache_use_or_start_loading("a" TEXTURE_CACHE_GRAYSCALE_SUFFIX);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
    CHECK_INT(entry->width, 2);
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

static void test_read_the_reader_refuses_is_marked_missing(void)
{
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    fake_reader_refuse_submits(1);
    entry = texture_cache_use_or_start_loading("a");
    CHECK_INT(entry->state, CACHED_TEXTURE_MISSING);
    CHECK_INT(texture_cache_loading_count(), 0);
}

static void test_name_too_long_for_a_path_is_marked_missing(void)
{
    const struct cached_texture *entry;
    char folder[FILE_PATH_MAX];

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    memset(folder, 'f', sizeof(folder) - 4);
    folder[sizeof(folder) - 4] = '\0';
    texture_cache_init(folder, 1024);
    entry = texture_cache_use_or_start_loading("a_name_that_does_not_fit");
    CHECK_INT(entry->state, CACHED_TEXTURE_MISSING);
    CHECK_INT(fake_reader_pending_count(), 0);
}

static void add_textures_named_a_to(char last_name)
{
    char path[16];
    char name;

    for (name = 'a'; name <= last_name; name++) {
        snprintf(path, sizeof(path), "tex/%c.gtex", name);
        fake_files_add_rgba_texture(path, 2, 2);
    }
}

static void test_only_a_few_reads_run_at_once_and_the_rest_wait(void)
{
    const struct cached_texture *waiting;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_textures_named_a_to('d');
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
    waiting = texture_cache_use_or_start_loading("c");
    texture_cache_use_or_start_loading("d");
    CHECK_INT(fake_reader_pending_count(), TEXTURE_CACHE_READS_AT_ONCE);
    CHECK_INT(texture_cache_loading_count(), 4);
    CHECK_INT(waiting->state, CACHED_TEXTURE_LOADING);
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_INT(fake_reader_pending_count(), TEXTURE_CACHE_READS_AT_ONCE);
    CHECK_TEXT(fake_reader_pending_path(1), "tex/c.gtex");
    CHECK_INT(texture_cache_loading_count(), 3);
}

static void test_page_used_this_frame_is_read_before_older_requests(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_textures_named_a_to('e');
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
    texture_cache_use_or_start_loading("c");
    texture_cache_use_or_start_loading("d");
    texture_cache_use_or_start_loading("e");
    texture_cache_begin_frame();
    texture_cache_use_or_start_loading("d");
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_TEXT(fake_reader_pending_path(1), "tex/d.gtex");
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    CHECK_TEXT(fake_reader_pending_path(1), "tex/c.gtex");
}

static void test_more_pages_than_the_read_queue_all_load_without_refusals(void)
{
    struct texture_cache_totals totals;
    char path[32];
    char name[16];
    int index;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    for (index = 0; index < BACKGROUND_READER_QUEUE_MAX * 2; index++) {
        snprintf(path, sizeof(path), "tex/t%d.gtex", index);
        fake_files_add_rgba_texture(path, 2, 2);
    }
    texture_cache_init("tex", 1024 * 1024);
    for (index = 0; index < BACKGROUND_READER_QUEUE_MAX * 2; index++) {
        snprintf(name, sizeof(name), "t%d", index);
        CHECK(texture_cache_use_or_start_loading(name) != NULL);
    }
    while (texture_cache_loading_count() > 0) {
        texture_cache_upload_finished_reads(1);
    }
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.loads, BACKGROUND_READER_QUEUE_MAX * 2);
    CHECK_INT(totals.missing, 0);
}

static void test_dropping_waiting_reads_leaves_reads_in_flight_alone(void)
{
    const struct cached_texture *in_flight;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_textures_named_a_to('d');
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    in_flight = texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
    texture_cache_use_or_start_loading("c");
    texture_cache_use_or_start_loading("d");
    texture_cache_drop_waiting_reads();
    CHECK_INT(texture_cache_loading_count(), 2);
    fake_reader_release_reads(2);
    texture_cache_upload_finished_reads(2);
    CHECK_INT(in_flight->state, CACHED_TEXTURE_READY);
    CHECK_INT(fake_reader_pending_count(), 0);
    CHECK_INT(fake_renderer_textures_created(), 2);
    CHECK_INT(texture_cache_use_or_start_loading("c")->state, CACHED_TEXTURE_LOADING);
    CHECK_INT(fake_reader_pending_count(), 1);
}

static void test_textures_are_read_from_their_name_bucket_when_buckets_are_on(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/03/a.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/30/movie_time_jump_012.gtex", 2, 2);
    texture_cache_init("tex", 1024);
    texture_cache_set_bucket_count(64u);
    CHECK_INT(texture_cache_load_now("a")->state, CACHED_TEXTURE_READY);
    CHECK_INT(texture_cache_load_now("movie_time_jump_012")->state, CACHED_TEXTURE_READY);
    texture_cache_set_bucket_count(0u);
}

static void test_waiting_reads_left_behind_are_dropped_only_under_their_prefix(void)
{
    const struct cached_texture *in_flight;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_textures_named_a_to('d');
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    in_flight = texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
    texture_cache_use_or_start_loading("c");
    texture_cache_begin_frame();
    texture_cache_use_or_start_loading("d");
    texture_cache_drop_waiting_reads_left_behind("d");
    CHECK_INT(texture_cache_loading_count(), 4);
    texture_cache_drop_waiting_reads_left_behind("a");
    CHECK_INT(texture_cache_loading_count(), 4);
    CHECK_INT(in_flight->state, CACHED_TEXTURE_LOADING);
    texture_cache_drop_waiting_reads_left_behind("c");
    CHECK_INT(texture_cache_loading_count(), 3);
}

static void test_read_out_of_memory_is_tried_again_when_asked_for_again(void)
{
    const struct cached_texture *entry;
    struct texture_cache_totals totals;
    const char *log_text;
    const char *first_line;
    unsigned int size;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/a.gtex", 2, 2);
    run_log_open("run.log");
    texture_cache_init("tex", 1024);
    fake_reader_run_out_of_memory_on_reads(2);
    texture_cache_use_or_start_loading("a");
    texture_cache_upload_finished_reads(1);
    CHECK_INT(texture_cache_loading_count(), 0);
    texture_cache_use_or_start_loading("a");
    texture_cache_upload_finished_reads(1);
    entry = texture_cache_use_or_start_loading("a");
    texture_cache_upload_finished_reads(1);
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
    texture_cache_read_totals(&totals);
    CHECK_INT(totals.missing, 0);
    run_log_flush();
    log_text = (const char *)fake_files_contents("run.log", &size);
    first_line = strstr(log_text, "read_out_of_memory");
    CHECK(first_line != NULL);
    CHECK(strstr(first_line + 1, "read_out_of_memory") == NULL);
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

static void test_clear_drops_waiting_reads_instead_of_reading_them(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    add_textures_named_a_to('d');
    texture_cache_init("tex", 1024);
    fake_reader_hold_reads(1);
    texture_cache_use_or_start_loading("a");
    texture_cache_use_or_start_loading("b");
    texture_cache_use_or_start_loading("c");
    texture_cache_use_or_start_loading("d");
    texture_cache_clear();
    CHECK_INT(fake_renderer_textures_created(), TEXTURE_CACHE_READS_AT_ONCE);
    CHECK_INT(texture_cache_loading_count(), 0);
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

static void test_full_cache_turns_a_page_away_without_reading_it(void)
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
    CHECK_INT(fake_reader_pending_count(), 0);
    texture_cache_begin_frame();
    entry = texture_cache_load_now("later");
    CHECK_INT(entry->state, CACHED_TEXTURE_READY);
    CHECK_INT(entry->width, 2);
    CHECK_INT(fake_reader_pending_count(), 0);
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
    test_grayscale_name_reads_the_plain_file();
    test_requesting_twice_only_reads_once();
    test_uploads_respect_the_limit_per_frame();
    test_read_the_reader_refuses_is_marked_missing();
    test_name_too_long_for_a_path_is_marked_missing();
    test_only_a_few_reads_run_at_once_and_the_rest_wait();
    test_page_used_this_frame_is_read_before_older_requests();
    test_more_pages_than_the_read_queue_all_load_without_refusals();
    test_dropping_waiting_reads_leaves_reads_in_flight_alone();
    test_textures_are_read_from_their_name_bucket_when_buckets_are_on();
    test_waiting_reads_left_behind_are_dropped_only_under_their_prefix();
    test_read_out_of_memory_is_tried_again_when_asked_for_again();
    test_clear_waits_for_reads_in_flight();
    test_clear_drops_waiting_reads_instead_of_reading_them();
    test_loading_textures_are_never_evicted();
    test_full_cache_turns_a_page_away_without_reading_it();
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
