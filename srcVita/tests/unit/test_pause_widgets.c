#include <math.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "pause_widgets.h"
#include "texture_cache.h"
#include "test_support.h"

static const char table[] =
    "text bar.short 0 0 100 30\n"
    "text bar.long 0 40 800 30\n"
    "text bar.two_lines 0 80 300 64\n"
    "glyph cross 0 0 40\n"
    "glyph start 240 0 72 40\n"
    "glyph rstick 420 112 40 40\n";

static void test_glyphs_keep_their_own_width_and_old_square_lines_still_read(void)
{
    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 6);
    CHECK(pause_glyph_width("cross", 40.0f) == 40.0f);
    CHECK(pause_glyph_width("start", 40.0f) == 72.0f);
    CHECK(pause_glyph_width("start", 20.0f) == 36.0f);
    CHECK(pause_glyph_width("rstick", 30.0f) == 30.0f);
    CHECK(pause_glyph_width("missing", 40.0f) == 0.0f);
}

static void test_text_wider_than_its_room_shrinks_to_fit_and_shorter_text_keeps_its_size(void)
{
    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 6);
    CHECK(pause_fit_scale("bar.short", 400.0f) == 1.0f);
    CHECK(pause_fit_scale("bar.long", 400.0f) == 0.5f);
    CHECK(pause_fit_scale("bar.long", 800.0f) == 1.0f);
}

static void test_two_line_text_shrinks_to_the_bar_height_and_the_tighter_room_wins(void)
{
    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 6);
    CHECK(pause_fit_box_scale("bar.two_lines", 756.0f, 48.0f) == 0.75f);
    CHECK(pause_fit_box_scale("bar.long", 400.0f, 48.0f) == 0.5f);
    CHECK(pause_fit_box_scale("bar.short", 400.0f, 48.0f) == 1.0f);
}

static const char preview_table[] = "preview room 0.25 0.125 0.5 0.75\n";

static int is_near(float value, float wanted)
{
    return fabsf(value - wanted) < 0.0001f;
}

static void check_the_preview_shows_the_same_part(const char *folder, const char *path, unsigned int width, unsigned int height)
{
    const struct fake_draw *draw;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture(path, width, height);
    texture_cache_init(folder, 1u << 24);
    CHECK(texture_cache_load_now("room")->state == CACHED_TEXTURE_READY);
    CHECK_INT(pause_widgets_read_text(preview_table, (unsigned int)strlen(preview_table)), 0);
    fake_renderer_clear_draws();
    CHECK_INT(pause_draw_preview(100.0f, 50.0f, 200.0f, 80.0f, 0.5f, 1.0f), 1);
    CHECK_INT(fake_renderer_draw_count(), 1);
    draw = fake_renderer_draw(0);
    CHECK(is_near(draw->texture_area.x, 0.5f));
    CHECK(is_near(draw->texture_area.y, 0.125f));
    CHECK(is_near(draw->texture_area.width, 0.25f));
    CHECK(is_near(draw->texture_area.height, 0.75f));
    CHECK(is_near(draw->screen_area.x, 200.0f));
    CHECK(is_near(draw->screen_area.width, 100.0f));
}

static void test_the_preview_shows_the_same_part_of_its_plate_at_either_art_size(void)
{
    check_the_preview_shows_the_same_part("big", "big/room.gtex", 1440, 540);
    check_the_preview_shows_the_same_part("small", "small/room.gtex", 512, 192);
}

static void test_a_preview_part_given_in_pixels_is_dropped(void)
{
    static const char pixels[] = "preview room 60 70 832 468\n";

    pause_widgets_read_text(pixels, (unsigned int)strlen(pixels));
    CHECK(pause_preview_plate() == NULL);
    CHECK_INT(pause_draw_preview(0.0f, 0.0f, 200.0f, 80.0f, 0.0f, 1.0f), 0);
}

static void test_a_part_that_leaves_its_texture_draws_nothing(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/sheet.gtex", 512, 192);
    texture_cache_init("tex", 1u << 24);
    CHECK(texture_cache_load_now("sheet")->state == CACHED_TEXTURE_READY);
    fake_renderer_clear_draws();
    pause_draw_from("sheet", 60.0f, 70.0f, 832.0f, 468.0f, 0.0f, 0.0f, 100.0f, 100.0f, 1.0f);
    pause_draw_from("sheet", 0.0f, 100.0f, 512.0f, 93.0f, 0.0f, 0.0f, 100.0f, 100.0f, 1.0f);
    CHECK_INT(fake_renderer_draw_count(), 0);
    pause_draw_from("sheet", 0.0f, 100.0f, 512.0f, 92.0f, 0.0f, 0.0f, 100.0f, 100.0f, 1.0f);
    CHECK_INT(fake_renderer_draw_count(), 1);
}

int main(void)
{
    test_the_preview_shows_the_same_part_of_its_plate_at_either_art_size();
    test_a_preview_part_given_in_pixels_is_dropped();
    test_a_part_that_leaves_its_texture_draws_nothing();
    test_glyphs_keep_their_own_width_and_old_square_lines_still_read();
    test_text_wider_than_its_room_shrinks_to_fit_and_shorter_text_keeps_its_size();
    test_two_line_text_shrinks_to_the_bar_height_and_the_tighter_room_wins();
    return test_finish("unit/pause_widgets");
}
