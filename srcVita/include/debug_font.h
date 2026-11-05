#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H

#include "renderer.h"

#define DEBUG_FONT_GLYPH_WIDTH 5
#define DEBUG_FONT_GLYPH_HEIGHT 7
#define DEBUG_FONT_FIRST_CHARACTER 32
#define DEBUG_FONT_LAST_CHARACTER 126
#define DEBUG_FONT_CELL_WIDTH 6
#define DEBUG_FONT_CELL_HEIGHT 8

int debug_font_glyph_pixel_is_set(char character, int x, int y);
void debug_font_fill_atlas(unsigned char *rgba_pixels, int atlas_width, int atlas_height);
int debug_font_init(void);
void debug_font_shutdown(void);
void debug_font_draw_text(float x, float y, float scale, struct renderer_color color, const char *text);
float debug_font_text_width(const char *text, float scale);

#endif
