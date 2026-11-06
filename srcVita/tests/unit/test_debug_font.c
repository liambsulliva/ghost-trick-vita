#include <stdlib.h>
#include "debug_font.h"
#include "fakes/fake_renderer.h"
#include "test_support.h"

static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };

static void test_font_glyphs_have_expected_pixels(void)
{
    CHECK(debug_font_glyph_pixel_is_set('I', 2, 0));
    CHECK(debug_font_glyph_pixel_is_set('I', 2, 3));
    CHECK(!debug_font_glyph_pixel_is_set('I', 0, 3));
    CHECK(debug_font_glyph_pixel_is_set('I', 1, 0));
    CHECK(debug_font_glyph_pixel_is_set('I', 1, 6));
    CHECK(!debug_font_glyph_pixel_is_set('I', 1, 3));
    CHECK(!debug_font_glyph_pixel_is_set(' ', 2, 3));
    CHECK(!debug_font_glyph_pixel_is_set('A', -1, 0));
    CHECK(!debug_font_glyph_pixel_is_set('A', 5, 0));
    CHECK(!debug_font_glyph_pixel_is_set('A', 0, 7));
    CHECK(!debug_font_glyph_pixel_is_set((char)10, 0, 0));
}

static void test_font_every_visible_character_has_ink(void)
{
    int character;
    int x;
    int y;
    int set_pixels;

    for (character = DEBUG_FONT_FIRST_CHARACTER + 1; character <= DEBUG_FONT_LAST_CHARACTER; character++) {
        set_pixels = 0;
        for (y = 0; y < DEBUG_FONT_GLYPH_HEIGHT; y++) {
            for (x = 0; x < DEBUG_FONT_GLYPH_WIDTH; x++) {
                set_pixels = set_pixels + debug_font_glyph_pixel_is_set((char)character, x, y);
            }
        }
        CHECK(set_pixels > 0);
    }
}

static void test_font_atlas_marks_ink_as_opaque_white(void)
{
    int width;
    int height;
    unsigned char *pixels;
    int glyph;
    int atlas_x;
    int atlas_y;
    int index;

    width = 16 * DEBUG_FONT_CELL_WIDTH;
    height = 6 * DEBUG_FONT_CELL_HEIGHT;
    pixels = malloc((size_t)(width * height * 4));
    debug_font_fill_atlas(pixels, width, height);
    glyph = 'I' - DEBUG_FONT_FIRST_CHARACTER;
    atlas_x = (glyph % 16) * DEBUG_FONT_CELL_WIDTH + 2;
    atlas_y = (glyph / 16) * DEBUG_FONT_CELL_HEIGHT + 3;
    index = (atlas_y * width + atlas_x) * 4;
    CHECK_INT(pixels[index], 255);
    CHECK_INT(pixels[index + 3], 255);
    index = (atlas_y * width + atlas_x - 2) * 4;
    CHECK_INT(pixels[index + 3], 0);
    free(pixels);
}

static void test_text_draws_one_sprite_per_visible_character(void)
{
    const struct fake_draw *second;

    fake_renderer_reset();
    CHECK(debug_font_init());
    debug_font_draw_text(10.0f, 20.0f, 2.0f, white, "A B");
    CHECK_INT(fake_renderer_draw_count(), 2);
    second = fake_renderer_draw(1);
    CHECK(second->screen_area.x == 10.0f + 2 * DEBUG_FONT_CELL_WIDTH * 2.0f);
    CHECK(second->screen_area.y == 20.0f);
    CHECK(second->screen_area.width == DEBUG_FONT_GLYPH_WIDTH * 2.0f);
    CHECK(second->texture_area.x >= 0.0f && second->texture_area.x < 1.0f);
    CHECK(second->texture_area.y >= 0.0f && second->texture_area.y < 1.0f);
    debug_font_shutdown();
    CHECK_INT(fake_renderer_textures_alive(), 0);
}

int main(void)
{
    test_font_glyphs_have_expected_pixels();
    test_font_every_visible_character_has_ink();
    test_font_atlas_marks_ink_as_opaque_white();
    test_text_draws_one_sprite_per_visible_character();
    return test_finish("unit/debug_font");
}
