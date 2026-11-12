#include <math.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "input.h"
#include "memory.h"
#include "pause_widgets.h"
#include "text_lines.h"
#include "texture_cache.h"

#define LINE_MAX 160
#define TABLE_PATH "pause/widgets.txt"
#define SCREEN_WIDTH ((float)RENDERER_WIDTH)
#define SCREEN_HEIGHT ((float)RENDERER_HEIGHT)
#define BACK_LEFT 12.0f
#define BACK_RIGHT 172.0f
#define BACK_INSET 9.0f
#define BACK_GLYPH_X 34.0f
#define BACK_GLYPH_SIZE 26.0f
#define BACK_WORD_X 114.0f
#define BAR_GLYPH_ROOM 36.0f
#define DIALOG_SIDE 150.0f
#define DIALOG_TOP_PAD 40.0f
#define DIALOG_LINE_STEP 38.0f
#define DIALOG_NOTE_STEP 34.0f
#define DIALOG_BASE_HEIGHT 70.0f
#define DIALOG_OPTIONS_HEIGHT 90.0f
#define DIALOG_OPTIONS_GAP 14.0f
#define DIALOG_BUTTON_WIDTH 230.0f
#define DIALOG_BUTTON_HEIGHT 56.0f
#define DIALOG_BUTTON_SPACE 40.0f
#define DIALOG_SHADE (150.0f / 255.0f)

struct widget {
    char key[PAUSE_WIDGET_KEY_MAX];
    float x;
    float y;
    float width;
    float height;
    float corner;
};

static struct widget widgets[PAUSE_WIDGETS_MOST];
static int widget_count;

static void add_widget(const char *key, float x, float y, float width, float height, float corner)
{
    struct widget *widget;

    if (widget_count >= PAUSE_WIDGETS_MOST || strlen(key) >= PAUSE_WIDGET_KEY_MAX) {
        return;
    }
    widget = &widgets[widget_count];
    snprintf(widget->key, sizeof(widget->key), "%s", key);
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->corner = corner;
    widget_count = widget_count + 1;
}

int pause_widgets_read_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    char name[PAUSE_WIDGET_KEY_MAX];
    char key[PAUSE_WIDGET_KEY_MAX + 16];
    unsigned int offset;
    float x;
    float y;
    float width;
    float height;
    int number;
    int lit;
    int corner;

    widget_count = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (sscanf(line, "text %47s %f %f %f %f", name, &x, &y, &width, &height) == 5) {
            add_widget(name, x, y, width, height, 0.0f);
        } else if (sscanf(line, "slice %47s %d %f %f %f %f", name, &corner, &x, &y, &width, &height) == 6) {
            snprintf(key, sizeof(key), "slice.%s", name);
            add_widget(key, x, y, width, height, (float)corner);
        } else if (sscanf(line, "flame %d %d %f %f %f %f", &number, &lit, &x, &y, &width, &height) == 6) {
            snprintf(key, sizeof(key), "flame.%d.%d", number, lit);
            add_widget(key, x, y, width, height, 0.0f);
        } else if (sscanf(line, "glyph %47s %f %f %f", name, &x, &y, &width) == 4) {
            snprintf(key, sizeof(key), "glyph.%s", name);
            add_widget(key, x, y, width, width, 0.0f);
        } else if (sscanf(line, "cell %47s %f %f %f %f", name, &x, &y, &width, &height) == 5) {
            snprintf(key, sizeof(key), "cell.%s", name);
            add_widget(key, x, y, width, height, 0.0f);
        }
    }
    return widget_count;
}

int pause_widgets_load(const char *assets_folder)
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    int count;

    widget_count = 0;
    if (!file_path_join(path, assets_folder, TABLE_PATH, "") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    count = pause_widgets_read_text((const char *)bytes, size);
    mem_free(bytes);
    return count;
}

static const struct widget *find(const char *key)
{
    int index;

    if (key == NULL) {
        return NULL;
    }
    for (index = 0; index < widget_count; index++) {
        if (strcmp(widgets[index].key, key) == 0) {
            return &widgets[index];
        }
    }
    return NULL;
}

int pause_widgets_has(const char *key)
{
    return find(key) != NULL;
}

float pause_text_width(const char *key)
{
    const struct widget *widget;

    widget = find(key);
    return widget != NULL ? widget->width : 0.0f;
}

struct renderer_color pause_white(float alpha)
{
    struct renderer_color color;

    color.red = 1.0f;
    color.green = 1.0f;
    color.blue = 1.0f;
    color.alpha = alpha;
    return color;
}

static float whole(float value)
{
    return floorf(value + 0.5f);
}

static void draw_from(const char *sheet, float u, float v, float u_size, float v_size, float x, float y, float width, float height, float alpha)
{
    const struct cached_texture *texture;
    struct renderer_rect screen;
    struct renderer_rect part;

    texture = texture_cache_use_or_start_loading(sheet);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY || texture->width == 0 || texture->height == 0 || alpha <= 0.0f || width <= 0.0f || height <= 0.0f) {
        return;
    }
    screen.x = x;
    screen.y = y;
    screen.width = width;
    screen.height = height;
    part.x = u / (float)texture->width;
    part.y = v / (float)texture->height;
    part.width = u_size / (float)texture->width;
    part.height = v_size / (float)texture->height;
    renderer_draw_sprite(texture->texture, screen, part, pause_white(alpha));
}

void pause_draw_text(const char *key, float x, float y, enum pause_anchor anchor, float alpha)
{
    const struct widget *widget;
    float left;

    widget = find(key);
    if (widget == NULL) {
        return;
    }
    left = anchor == PAUSE_ANCHOR_LEFT ? x : (anchor == PAUSE_ANCHOR_CENTRE ? x - widget->width / 2.0f : x - widget->width);
    draw_from(PAUSE_WIDGET_SHEET, widget->x, widget->y, widget->width, widget->height, whole(left), whole(y - widget->height / 2.0f), widget->width, widget->height, alpha);
}

void pause_draw_slice(const char *style, float x, float y, float width, float height, float alpha)
{
    char key[PAUSE_WIDGET_KEY_MAX + 8];
    const struct widget *widget;
    float source_x[4];
    float source_y[4];
    float target_x[4];
    float target_y[4];
    int column;
    int row;

    snprintf(key, sizeof(key), "slice.%s", style);
    widget = find(key);
    if (widget == NULL) {
        return;
    }
    x = whole(x);
    y = whole(y);
    width = whole(width);
    height = whole(height);
    source_x[0] = widget->x;
    source_x[1] = widget->x + widget->corner;
    source_x[2] = widget->x + widget->width - widget->corner;
    source_x[3] = widget->x + widget->width;
    source_y[0] = widget->y;
    source_y[1] = widget->y + widget->corner;
    source_y[2] = widget->y + widget->height - widget->corner;
    source_y[3] = widget->y + widget->height;
    target_x[0] = x;
    target_x[1] = x + widget->corner;
    target_x[2] = x + width - widget->corner;
    target_x[3] = x + width;
    target_y[0] = y;
    target_y[1] = y + widget->corner;
    target_y[2] = y + height - widget->corner;
    target_y[3] = y + height;
    for (row = 0; row < 3; row++) {
        for (column = 0; column < 3; column++) {
            draw_from(PAUSE_WIDGET_SHEET, source_x[column], source_y[row], source_x[column + 1] - source_x[column], source_y[row + 1] - source_y[row], target_x[column], target_y[row], target_x[column + 1] - target_x[column], target_y[row + 1] - target_y[row], alpha);
        }
    }
}

void pause_draw_flame(int number, int lit, float centre_x, float centre_y, float alpha)
{
    char key[32];
    const struct widget *widget;

    snprintf(key, sizeof(key), "flame.%d.%d", number, lit ? 1 : 0);
    widget = find(key);
    if (widget != NULL) {
        draw_from(PAUSE_WIDGET_SHEET, widget->x, widget->y, widget->width, widget->height, whole(centre_x - widget->width / 2.0f), whole(centre_y - widget->height / 2.0f), widget->width, widget->height, alpha);
    }
}

void pause_draw_glyph(const char *kind, float centre_x, float centre_y, float size, float alpha)
{
    char key[PAUSE_WIDGET_KEY_MAX + 8];
    const struct widget *widget;

    snprintf(key, sizeof(key), "glyph.%s", kind);
    widget = find(key);
    if (widget != NULL) {
        draw_from(PAUSE_GLYPH_SHEET, widget->x, widget->y, widget->width, widget->height, whole(centre_x - size / 2.0f), whole(centre_y - size / 2.0f), size, size, alpha);
    }
}

void pause_draw_cell(const char *name, float centre_x, float centre_y, float alpha)
{
    char key[PAUSE_WIDGET_KEY_MAX + 8];
    const struct widget *widget;

    snprintf(key, sizeof(key), "cell.%s", name);
    widget = find(key);
    if (widget != NULL) {
        draw_from(PAUSE_WIDGET_SHEET, widget->x, widget->y, widget->width, widget->height, whole(centre_x - widget->width / 2.0f), whole(centre_y - widget->height / 2.0f), widget->width, widget->height, alpha);
    }
}

void pause_draw_bar(float x_offset, const char *before_key, const char *glyph, const char *after_key)
{
    float middle;
    float total;
    float x;

    middle = PAUSE_BAR_TOP + PAUSE_BAR_HEIGHT / 2.0f;
    pause_draw_slice("back", BACK_LEFT + x_offset, PAUSE_BAR_TOP + BACK_INSET, BACK_RIGHT - BACK_LEFT, PAUSE_BAR_HEIGHT - 2.0f * BACK_INSET, 1.0f);
    pause_draw_glyph("circle", BACK_GLYPH_X + x_offset, middle, BACK_GLYPH_SIZE, 1.0f);
    pause_draw_text("bar.back", BACK_WORD_X + x_offset, middle, PAUSE_ANCHOR_CENTRE, 1.0f);
    total = pause_text_width(before_key) + (glyph != NULL ? BAR_GLYPH_ROOM + pause_text_width(after_key) : 0.0f);
    x = (BACK_RIGHT + SCREEN_WIDTH) / 2.0f - total / 2.0f + x_offset;
    pause_draw_text(before_key, x, middle, PAUSE_ANCHOR_LEFT, 1.0f);
    if (glyph != NULL) {
        x = x + pause_text_width(before_key);
        pause_draw_glyph(glyph, x + BAR_GLYPH_ROOM / 2.0f, middle, BACK_GLYPH_SIZE - 4.0f, 1.0f);
        pause_draw_text(after_key, x + BAR_GLYPH_ROOM, middle, PAUSE_ANCHOR_LEFT, 1.0f);
    }
}

int pause_is_on_back_pill(float x, float y)
{
    return x >= BACK_LEFT && x <= BACK_RIGHT && y >= PAUSE_BAR_TOP + BACK_INSET && y <= SCREEN_HEIGHT - BACK_INSET;
}

void pause_dialog_open(struct pause_dialog *dialog, int selected)
{
    dialog->is_open = 1;
    dialog->selected = selected;
}

int pause_dialog_steer(struct pause_dialog *dialog, unsigned int pressed)
{
    if (!dialog->is_open) {
        return PAUSE_DIALOG_NOTHING;
    }
    if (pressed & INPUT_CIRCLE) {
        dialog->is_open = 0;
        return PAUSE_DIALOG_CANCEL;
    }
    if (pressed & INPUT_CROSS) {
        dialog->is_open = 0;
        return dialog->option_count > 0 ? dialog->selected : PAUSE_DIALOG_CANCEL;
    }
    if ((pressed & INPUT_LEFT) && dialog->selected > 0) {
        dialog->selected = dialog->selected - 1;
    } else if ((pressed & INPUT_RIGHT) && dialog->selected < dialog->option_count - 1) {
        dialog->selected = dialog->selected + 1;
    }
    return PAUSE_DIALOG_NOTHING;
}

static float dialog_height(const struct pause_dialog *dialog)
{
    return DIALOG_BASE_HEIGHT + DIALOG_LINE_STEP * (float)dialog->line_count + (dialog->note != NULL ? DIALOG_NOTE_STEP : 0.0f) + (dialog->option_count > 0 ? DIALOG_OPTIONS_HEIGHT : 0.0f);
}

static float dialog_top(const struct pause_dialog *dialog)
{
    return floorf((PAUSE_BAR_TOP - dialog_height(dialog)) / 2.0f);
}

static float options_top(const struct pause_dialog *dialog)
{
    return dialog_top(dialog) + DIALOG_TOP_PAD + DIALOG_LINE_STEP * (float)dialog->line_count + (dialog->note != NULL ? DIALOG_NOTE_STEP : 0.0f) + DIALOG_OPTIONS_GAP;
}

static float option_left(const struct pause_dialog *dialog, int option)
{
    float total;

    total = (float)dialog->option_count * DIALOG_BUTTON_WIDTH + (float)(dialog->option_count - 1) * DIALOG_BUTTON_SPACE;
    return (SCREEN_WIDTH - total) / 2.0f + (float)option * (DIALOG_BUTTON_WIDTH + DIALOG_BUTTON_SPACE);
}

int pause_dialog_option_at(const struct pause_dialog *dialog, float x, float y)
{
    int option;
    float top;

    if (!dialog->is_open) {
        return PAUSE_DIALOG_NOTHING;
    }
    top = options_top(dialog);
    for (option = 0; option < dialog->option_count; option++) {
        if (x >= option_left(dialog, option) && x <= option_left(dialog, option) + DIALOG_BUTTON_WIDTH && y >= top && y <= top + DIALOG_BUTTON_HEIGHT) {
            return option;
        }
    }
    return PAUSE_DIALOG_NOTHING;
}

void pause_dialog_draw(const struct pause_dialog *dialog, float x_offset)
{
    struct renderer_rect shade;
    struct renderer_color black;
    float top;
    float y;
    int index;

    if (!dialog->is_open) {
        return;
    }
    shade.x = 0.0f;
    shade.y = 0.0f;
    shade.width = SCREEN_WIDTH;
    shade.height = SCREEN_HEIGHT;
    black.red = 0.0f;
    black.green = 0.0f;
    black.blue = 0.0f;
    black.alpha = DIALOG_SHADE;
    renderer_draw_filled_rect(shade, black);
    top = dialog_top(dialog);
    pause_draw_slice("box", DIALOG_SIDE + x_offset, top, SCREEN_WIDTH - 2.0f * DIALOG_SIDE, dialog_height(dialog), 1.0f);
    y = top + DIALOG_TOP_PAD;
    for (index = 0; index < dialog->line_count; index++) {
        pause_draw_text(dialog->lines[index], SCREEN_WIDTH / 2.0f + x_offset, y, PAUSE_ANCHOR_CENTRE, 1.0f);
        y = y + DIALOG_LINE_STEP;
    }
    if (dialog->note != NULL) {
        pause_draw_text(dialog->note, SCREEN_WIDTH / 2.0f + x_offset, y, PAUSE_ANCHOR_CENTRE, 1.0f);
    }
    for (index = 0; index < dialog->option_count; index++) {
        float left;

        left = option_left(dialog, index) + x_offset;
        pause_draw_slice(index == dialog->selected ? "button_selected" : "button", left, options_top(dialog), DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, 1.0f);
        pause_draw_text(dialog->options[index], left + DIALOG_BUTTON_WIDTH / 2.0f, options_top(dialog) + DIALOG_BUTTON_HEIGHT / 2.0f, PAUSE_ANCHOR_CENTRE, 1.0f);
    }
}
