#ifndef PAUSE_WIDGETS_H
#define PAUSE_WIDGETS_H

#include "renderer.h"

#define PAUSE_WIDGET_SHEET "pause_widgets"
#define PAUSE_GLYPH_SHEET "hud_sheet"
#define PAUSE_WIDGETS_MOST 256
#define PAUSE_WIDGET_KEY_MAX 48
#define PAUSE_DIALOG_LINES_MOST 3
#define PAUSE_DIALOG_OPTIONS_MOST 3
#define PAUSE_BAR_HEIGHT 52.0f
#define PAUSE_BAR_TOP ((float)RENDERER_HEIGHT - PAUSE_BAR_HEIGHT)
#define PAUSE_DIALOG_NOTHING (-1)
#define PAUSE_DIALOG_CANCEL (-2)

enum pause_anchor {
    PAUSE_ANCHOR_LEFT,
    PAUSE_ANCHOR_CENTRE,
    PAUSE_ANCHOR_RIGHT
};

struct pause_dialog {
    int is_open;
    int line_count;
    const char *lines[PAUSE_DIALOG_LINES_MOST];
    const char *note;
    int option_count;
    const char *options[PAUSE_DIALOG_OPTIONS_MOST];
    int selected;
};

int pause_widgets_read_text(const char *text, unsigned int text_size);
int pause_widgets_load(const char *assets_folder);
int pause_widgets_has(const char *key);
float pause_text_width(const char *key);
struct renderer_color pause_white(float alpha);
void pause_draw_text(const char *key, float x, float y, enum pause_anchor anchor, float alpha);
void pause_draw_slice(const char *style, float x, float y, float width, float height, float alpha);
void pause_draw_flame(int number, int lit, float centre_x, float centre_y, float alpha);
void pause_draw_glyph(const char *kind, float centre_x, float centre_y, float size, float alpha);
void pause_draw_cell(const char *name, float centre_x, float centre_y, float alpha);
void pause_draw_bar(float x_offset, const char *before_key, const char *glyph, const char *after_key);
int pause_is_on_back_pill(float x, float y);

void pause_dialog_open(struct pause_dialog *dialog, int selected);
int pause_dialog_steer(struct pause_dialog *dialog, unsigned int pressed);
int pause_dialog_option_at(const struct pause_dialog *dialog, float x, float y);
void pause_dialog_draw(const struct pause_dialog *dialog, float x_offset);

#endif
