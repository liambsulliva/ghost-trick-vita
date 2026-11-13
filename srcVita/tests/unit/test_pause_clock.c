#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "pause_clock.h"
#include "test_support.h"
#include "texture_cache.h"

static const char table[] =
    "glyph d0 0 0 20 40\nglyph d1 20 0 20 40\nglyph d2 40 0 20 40\nglyph d7 60 0 20 40\nglyph colon 80 0 10 40\nglyph am 90 0 40 40\nglyph pm 130 0 40 40\n"
    "place stage01_00 0 40 120 24\nplace stage02_01 0 64 160 24\nplace stage03_00 0 88 150 24\n"
    "scene 1 st01-a_Expand 1142\nscene 1 st01-b_Expand 1151\nscene 2 st01-a_Expand 1171\n";

static void start_clock(void)
{
    CHECK_INT(pause_clock_read_text(table, (unsigned int)strlen(table)), 3);
}

static void check_time(int minutes, const char *expected)
{
    char text[PAUSE_CLOCK_TEXT_MAX];

    pause_clock_time_text(minutes, text, sizeof(text));
    CHECK_TEXT(text, expected);
}

static void test_times_read_as_a_twelve_hour_clock(void)
{
    check_time(19 * 60 + 2, "7:02 PM");
    check_time(24 * 60 + 10, "12:10 AM");
    check_time(29 * 60 + 10, "5:10 AM");
    check_time(12 * 60, "12:00 PM");
    check_time(PAUSE_CLOCK_NO_TIME, "");
}

static void test_each_scene_sets_its_chapter_time(void)
{
    start_clock();
    CHECK_INT(pause_clock_minutes(), PAUSE_CLOCK_NO_TIME);
    pause_clock_note_scene(0, "st01-a_Expand");
    CHECK_INT(pause_clock_minutes(), 1142);
    pause_clock_note_scene(1, "st01-b_Expand");
    CHECK_INT(pause_clock_minutes(), 1151);
    pause_clock_note_scene(1, "phone");
    CHECK_INT(pause_clock_minutes(), 1151);
    pause_clock_note_scene(2, "st01-a_Expand");
    CHECK_INT(pause_clock_minutes(), 1171);
    pause_clock_note_scene(18, "st13-last_Expand");
    CHECK_INT(pause_clock_minutes(), PAUSE_CLOCK_NO_TIME);
}

static void test_the_room_art_names_the_place(void)
{
    start_clock();
    CHECK(pause_clock_place() == NULL);
    pause_clock_note_plate("st01-00-02");
    CHECK_TEXT(pause_clock_place(), "stage01_00");
    pause_clock_note_plate("st02-01");
    CHECK_TEXT(pause_clock_place(), "stage02_01");
    pause_clock_note_plate("st03-02_0");
    CHECK_TEXT(pause_clock_place(), "stage03_00");
    pause_clock_note_plate("st08-00");
    CHECK_TEXT(pause_clock_place(), "stage03_00");
    pause_clock_note_plate("st01fb_00");
    CHECK_TEXT(pause_clock_place(), "stage01_00");
}

static void test_the_clock_draws_the_time_and_the_place(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/pause_clock.gtex", 256, 128);
    texture_cache_init("tex", 1u << 20);
    CHECK(texture_cache_load_now("pause_clock")->state == CACHED_TEXTURE_READY);
    start_clock();
    pause_clock_note_scene(1, "st01-a_Expand");
    pause_clock_note_plate("st01-00");
    fake_renderer_clear_draws();
    pause_clock_draw(1.0f);
    CHECK_INT(fake_renderer_draw_count(), 6);
    fake_renderer_clear_draws();
    pause_clock_draw(0.0f);
    CHECK_INT(fake_renderer_draw_count(), 0);
}

int main(void)
{
    test_times_read_as_a_twelve_hour_clock();
    test_each_scene_sets_its_chapter_time();
    test_the_room_art_names_the_place();
    test_the_clock_draws_the_time_and_the_place();
    return test_finish("unit/pause_clock");
}
