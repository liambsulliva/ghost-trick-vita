#include <string.h>
#include "pause_widgets.h"
#include "test_support.h"

static const char table[] =
    "text bar.short 0 0 100 30\n"
    "text bar.long 0 40 800 30\n"
    "glyph cross 0 0 40\n"
    "glyph start 240 0 72 40\n"
    "glyph rstick 420 112 40 40\n";

static void test_glyphs_keep_their_own_width_and_old_square_lines_still_read(void)
{
    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 5);
    CHECK(pause_glyph_width("cross", 40.0f) == 40.0f);
    CHECK(pause_glyph_width("start", 40.0f) == 72.0f);
    CHECK(pause_glyph_width("start", 20.0f) == 36.0f);
    CHECK(pause_glyph_width("rstick", 30.0f) == 30.0f);
    CHECK(pause_glyph_width("missing", 40.0f) == 0.0f);
}

static void test_text_wider_than_its_room_shrinks_to_fit_and_shorter_text_keeps_its_size(void)
{
    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 5);
    CHECK(pause_fit_scale("bar.short", 400.0f) == 1.0f);
    CHECK(pause_fit_scale("bar.long", 400.0f) == 0.5f);
    CHECK(pause_fit_scale("bar.long", 800.0f) == 1.0f);
}

int main(void)
{
    test_glyphs_keep_their_own_width_and_old_square_lines_still_read();
    test_text_wider_than_its_room_shrinks_to_fit_and_shorter_text_keeps_its_size();
    return test_finish("unit/pause_widgets");
}
