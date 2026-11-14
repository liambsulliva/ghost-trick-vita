#include <math.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "game_save.h"
#include "game_settings.h"
#include "input.h"
#include "pause_options.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"

#define KEY_MAX 64
#define ROW_TOP 150.0f
#define ROW_STEP 76.0f
#define ROW_HEIGHT 62.0f
#define ROW_LEFT 26.0f
#define ROW_RIGHT 934.0f
#define ROW_TEXT_X 56.0f
#define HEADER_GAP 50.0f
#define HEADER_ABOVE 32.0f
#define HEADER_TEXT_X 38.0f
#define LIST_BOTTOM_PAD 60.0f
#define VALUE_LEFT 560.0f
#define VALUE_RIGHT 922.0f
#define VALUE_INSET 7.0f
#define VALUE_TEXT_PAD 14.0f
#define ARROW_FROM_RIGHT 36.0f
#define FLAME_FIRST_X 545.0f
#define FLAME_STEP 38.0f
#define FLAME_RISE 2.0f
#define FLAME_REACH_Y 24.0f
#define SCROLL_KEEP_BELOW 40.0f
#define SCROLL_EASE 0.18f
#define SCROLL_SNAP 0.5f
#define DRAG_START 12.0f
#define SLIDER_KNOB_INSET 4.0f
#define PREVIEW_INSET 8.0f
#define PREVIEW_PICTURE_HEIGHT 194.0f
#define PREVIEW_LABEL_ROOM 40.0f
#define PREVIEW_CARD_HEIGHT (PREVIEW_PICTURE_HEIGHT + PREVIEW_LABEL_ROOM + PREVIEW_INSET)
#define PREVIEW_GAP 8.0f
#define PREVIEW_DIVIDER 2.0f
#define SLIDER_TEXT_PAD 8.0f
#define GREYED_ALPHA 0.42f
#define DROP_ITEM_STEP 40.0f
#define DROP_PAD 12.0f
#define DROP_FIRST 26.0f
#define DROP_OVERLAP 4.0f
#define DROP_CHOICE_INSET 8.0f
#define DROP_CHOICE_HALF 17.0f
#define CONTROLS_ACTION_X 196.0f
#define CONTROLS_GLYPH_HEIGHT 40.0f
#define CONTROLS_GLYPH_GAP 8.0f
#define CONTROLS_GLYPH_MOST 2
#define CONTROLS_TEXT_END 910.0f
#define LANGUAGE_COUNT 9
#define CONTROLS_COUNT 10

enum row_kind {
    ROW_OPENS,
    ROW_CYCLES,
    ROW_METER,
    ROW_DROPDOWN,
    ROW_SLIDER,
    ROW_FIXED
};

enum row_job {
    JOB_NONE,
    JOB_CONTROLS,
    JOB_BRIGHTNESS,
    JOB_MASTER,
    JOB_MUSIC,
    JOB_EFFECTS,
    JOB_MUSIC_TYPE,
    JOB_LANGUAGE,
    JOB_RESTORE,
    JOB_RESET_SAVE,
    JOB_HIGH_CONTRAST,
    JOB_FPS_CAP,
    JOB_VISUAL_FILTER,
    JOB_TEXT_SPEED,
    JOB_UNLOCK_ALL
};

struct option_row {
    const char *header;
    const char *name;
    enum row_kind kind;
    enum row_job job;
    int is_greyed;
    const char *fixed_value;
};

static const struct option_row rows[] = {
    { "controls", "controls", ROW_OPENS, JOB_CONTROLS, 0, NULL },
    { "display", "text_speed", ROW_SLIDER, JOB_TEXT_SPEED, 0, NULL },
    { NULL, "brightness", ROW_METER, JOB_BRIGHTNESS, 0, NULL },
    { NULL, "high_contrast", ROW_CYCLES, JOB_HIGH_CONTRAST, 0, NULL },
    { NULL, "visual_filter", ROW_CYCLES, JOB_VISUAL_FILTER, 0, NULL },
    { NULL, "fps_cap", ROW_CYCLES, JOB_FPS_CAP, 0, NULL },
    { NULL, "vsync", ROW_FIXED, JOB_NONE, 1, "on" },
    { "sound", "master", ROW_METER, JOB_MASTER, 0, NULL },
    { NULL, "music", ROW_METER, JOB_MUSIC, 0, NULL },
    { NULL, "effects", ROW_METER, JOB_EFFECTS, 0, NULL },
    { NULL, "music_type", ROW_CYCLES, JOB_MUSIC_TYPE, 0, NULL },
    { "language", "language", ROW_DROPDOWN, JOB_LANGUAGE, 0, NULL },
    { "other", "credits", ROW_OPENS, JOB_NONE, 1, NULL },
    { NULL, "reset_save", ROW_OPENS, JOB_RESET_SAVE, 0, NULL },
    { NULL, "restore", ROW_OPENS, JOB_RESTORE, 0, NULL },
    { NULL, "unlock_all", ROW_CYCLES, JOB_UNLOCK_ALL, 0, NULL }
};

#define ROW_COUNT ((int)(sizeof(rows) / sizeof(rows[0])))

static const char *const languages[LANGUAGE_COUNT] = { "english", "japanese", "french", "italian", "german", "spanish", "chinese_traditional", "chinese_simplified", "korean" };
static const char *const control_keys[CONTROLS_COUNT] = { "stick", "right_stick", "cross", "circle", "l", "r", "faces", "select", "start", "touch" };
static const char *const control_glyphs[CONTROLS_COUNT][CONTROLS_GLYPH_MOST] = { { "lstick", "dpad" }, { "rstick", NULL }, { "cross", NULL }, { "circle", NULL }, { "l", NULL }, { "r", NULL }, { "triangle", "square" }, { "select", NULL }, { "start", NULL }, { "touch", NULL } };

static const char *const text_speed_keys[GAME_TEXT_SPEED_COUNT] = { "row.text_slow", "row.text_normal", "row.text_fast" };
static float row_y[ROW_COUNT];
static float list_height;
static int selected;
static int controls_selected;
static int is_on_controls_page;
static int dropdown_is_open;
static int dropdown_hovered;
static float scroll;
static int scroll_is_free;
static int drag_is_tracking;
static int drag_is_scrolling;
static float drag_land_y;
static float drag_land_scroll;
static int *drag_level;
static int drag_level_before;
static int touched_row;
static int touched_option;
static int settings_changed;
static struct pause_dialog restore_dialog;
static enum row_job dialog_job;
static int unlock_row_is_shown;
static int leaves_with_dropdown;

void pause_options_show_unlock(int is_shown)
{
    unlock_row_is_shown = is_shown;
}

static int row_count(void)
{
    return unlock_row_is_shown ? ROW_COUNT : ROW_COUNT - 1;
}

static float whole(float value)
{
    return floorf(value + 0.5f);
}

static void lay_out_rows(void)
{
    float y;
    int index;

    y = ROW_TOP;
    for (index = 0; index < row_count(); index++) {
        if (rows[index].header != NULL && index > 0) {
            y = y + HEADER_GAP;
        }
        row_y[index] = y;
        y = y + ROW_STEP;
    }
    list_height = y + LIST_BOTTOM_PAD;
}

static int is_dimmed(const struct option_row *row)
{
    return row->is_greyed || (row->job == JOB_VISUAL_FILTER && game_settings_filter_is_pinned());
}

static int is_selectable(int index)
{
    return index >= 0 && index < row_count() && !rows[index].is_greyed;
}

static int next_selectable(int from, int direction)
{
    int index;

    for (index = from + direction; index >= 0 && index < row_count(); index += direction) {
        if (is_selectable(index)) {
            return index;
        }
    }
    return from;
}

void pause_options_enter(void)
{
    lay_out_rows();
    selected = next_selectable(-1, 1);
    controls_selected = 0;
    is_on_controls_page = 0;
    dropdown_is_open = 0;
    dropdown_hovered = 0;
    scroll = 0.0f;
    scroll_is_free = 0;
    drag_is_tracking = 0;
    drag_is_scrolling = 0;
    drag_level = NULL;
    touched_row = -1;
    touched_option = PAUSE_DIALOG_NOTHING;
    settings_changed = 0;
    leaves_with_dropdown = 0;
    restore_dialog.is_open = 0;
    restore_dialog.line_count = 1;
    restore_dialog.lines[0] = "row.restore_ask";
    restore_dialog.note = NULL;
    restore_dialog.option_count = 2;
    restore_dialog.options[0] = "row.yes";
    restore_dialog.options[1] = "row.no";
    restore_dialog.selected = 1;
}

int pause_options_selected_row(void)
{
    return selected;
}

int pause_options_is_on_controls_page(void)
{
    return is_on_controls_page;
}

int pause_options_dropdown_is_open(void)
{
    return dropdown_is_open;
}

int pause_options_dialog_is_open(void)
{
    return restore_dialog.is_open;
}

static int *level_of(enum row_job job)
{
    struct game_settings *settings;

    settings = game_settings_now();
    switch (job) {
    case JOB_BRIGHTNESS:
        return &settings->brightness;
    case JOB_MASTER:
        return &settings->master;
    case JOB_MUSIC:
        return &settings->music;
    case JOB_EFFECTS:
        return &settings->effects;
    default:
        return NULL;
    }
}

static void settings_now_changed(const char *what)
{
    struct game_settings *settings;

    settings = game_settings_now();
    game_settings_apply();
    settings_changed = 1;
    run_log_write("pause option %s brightness=%d master=%d music=%d effects=%d music_type=%d", what, settings->brightness, settings->master, settings->music, settings->effects, settings->music_type);
}

static void set_level(enum row_job job, int level)
{
    int *value;
    int least;

    value = level_of(job);
    least = job == JOB_BRIGHTNESS ? 1 : 0;
    if (value == NULL) {
        return;
    }
    level = level < least ? least : (level > GAME_SETTING_STEPS ? GAME_SETTING_STEPS : level);
    if (*value != level) {
        *value = level;
        settings_now_changed(rows[selected].name);
    }
}

static void cycle_music_type(void)
{
    struct game_settings *settings;

    settings = game_settings_now();
    settings->music_type = settings->music_type == GAME_MUSIC_ORIGINAL ? GAME_MUSIC_ARRANGED : GAME_MUSIC_ORIGINAL;
    settings_now_changed("music_type");
}

static int *switch_of(enum row_job job)
{
    struct game_settings *settings;

    settings = game_settings_now();
    switch (job) {
    case JOB_HIGH_CONTRAST:
        return &settings->high_contrast;
    case JOB_FPS_CAP:
        return &settings->fps_cap;
    case JOB_VISUAL_FILTER:
        return &settings->visual_filter;
    case JOB_UNLOCK_ALL:
        return &settings->unlock_all;
    default:
        return NULL;
    }
}

static const char *switch_value_name(enum row_job job, int value)
{
    if (job == JOB_FPS_CAP) {
        return value == GAME_FPS_CAPPED ? "fps_30" : "fps_60";
    }
    if (job == JOB_VISUAL_FILTER) {
        return game_settings_visual_filter() == GAME_FILTER_DS ? "filter_ds" : "filter_remastered";
    }
    return value ? "on" : "off";
}

static void flip_switch(enum row_job job)
{
    int *value;

    value = switch_of(job);
    if (value == NULL) {
        return;
    }
    if (job == JOB_VISUAL_FILTER && game_settings_filter_is_pinned()) {
        run_log_write("pause option %s pinned=%d", rows[selected].name, game_settings_visual_filter());
        return;
    }
    if (job == JOB_VISUAL_FILTER) {
        *value = *value == GAME_FILTER_DS ? GAME_FILTER_REMASTERED : GAME_FILTER_DS;
    } else if (job == JOB_FPS_CAP) {
        *value = *value == GAME_FPS_CAPPED ? GAME_FPS_FULL : GAME_FPS_CAPPED;
    } else {
        *value = !*value;
    }
    settings_now_changed(rows[selected].name);
    run_log_write("pause option %s=%d", rows[selected].name, *value);
}

static void set_text_speed(int speed)
{
    struct game_settings *settings;

    settings = game_settings_now();
    speed = speed < 0 ? 0 : (speed >= GAME_TEXT_SPEED_COUNT ? GAME_TEXT_SPEED_COUNT - 1 : speed);
    if (settings->text_speed != speed) {
        settings->text_speed = speed;
        settings_now_changed("text_speed");
        run_log_write("pause option text_speed=%d", speed);
    }
}

static float slider_stop_width(void)
{
    return (VALUE_RIGHT - VALUE_LEFT) / (float)GAME_TEXT_SPEED_COUNT;
}

static int slider_stop_at(float x)
{
    int stop;

    if (x < VALUE_LEFT || x > VALUE_RIGHT) {
        return -1;
    }
    stop = (int)((x - VALUE_LEFT) / slider_stop_width());
    return stop >= GAME_TEXT_SPEED_COUNT ? GAME_TEXT_SPEED_COUNT - 1 : stop;
}

static void restore_defaults(void)
{
    game_settings_defaults(game_settings_now());
    settings_now_changed("restore");
}

static void act_on_row(void)
{
    switch (rows[selected].job) {
    case JOB_CONTROLS:
        is_on_controls_page = 1;
        controls_selected = 0;
        scroll = 0.0f;
        break;
    case JOB_MUSIC_TYPE:
        cycle_music_type();
        break;
    case JOB_TEXT_SPEED:
        set_text_speed((game_settings_now()->text_speed + 1) % GAME_TEXT_SPEED_COUNT);
        break;
    case JOB_HIGH_CONTRAST:
    case JOB_FPS_CAP:
    case JOB_VISUAL_FILTER:
    case JOB_UNLOCK_ALL:
        flip_switch(rows[selected].job);
        break;
    case JOB_LANGUAGE:
        dropdown_is_open = 1;
        dropdown_hovered = game_settings_now()->language;
        break;
    case JOB_RESTORE:
    case JOB_RESET_SAVE:
        dialog_job = rows[selected].job;
        restore_dialog.lines[0] = rows[selected].job == JOB_RESTORE ? "row.restore_ask" : "row.reset_ask";
        restore_dialog.note = rows[selected].job == JOB_RESTORE ? NULL : "bar.reset_warning";
        pause_dialog_open(&restore_dialog, 1);
        break;
    default:
        break;
    }
}

static void choose_language(int index)
{
    if (index == 0) {
        game_settings_now()->language = 0;
        dropdown_is_open = 0;
        return;
    }
    run_log_write("pause option language=%s not_ported", languages[index]);
}

static int step_dropdown(unsigned int pressed)
{
    if (pressed & INPUT_CIRCLE) {
        dropdown_is_open = 0;
    } else if (pressed & INPUT_CROSS) {
        choose_language(dropdown_hovered);
    } else if ((pressed & INPUT_UP) && dropdown_hovered > 0) {
        dropdown_hovered = dropdown_hovered - 1;
    } else if ((pressed & INPUT_DOWN) && dropdown_hovered < LANGUAGE_COUNT - 1) {
        dropdown_hovered = dropdown_hovered + 1;
    }
    return PAUSE_OPTIONS_STAYS;
}

static int step_controls_page(unsigned int pressed)
{
    if (pressed & (INPUT_UP | INPUT_DOWN)) {
        scroll_is_free = 0;
    }
    if (pressed & INPUT_CIRCLE) {
        is_on_controls_page = 0;
    } else if ((pressed & INPUT_UP) && controls_selected > 0) {
        controls_selected = controls_selected - 1;
    } else if ((pressed & INPUT_DOWN) && controls_selected < CONTROLS_COUNT - 1) {
        controls_selected = controls_selected + 1;
    }
    return PAUSE_OPTIONS_STAYS;
}

void pause_options_leave(void)
{
    if (settings_changed) {
        (void)game_settings_save(GAME_SETTINGS_PATH);
        settings_changed = 0;
    }
}

int pause_options_step(unsigned int pressed)
{
    int chosen;
    int *level;

    if (restore_dialog.is_open) {
        chosen = pause_dialog_steer(&restore_dialog, pressed);
        if (chosen == 0 && dialog_job == JOB_RESET_SAVE) {
            (void)game_save_erase_all();
        } else if (chosen == 0) {
            restore_defaults();
        }
        return PAUSE_OPTIONS_STAYS;
    }
    if (leaves_with_dropdown && !dropdown_is_open) {
        pause_options_leave();
        return PAUSE_OPTIONS_LEAVES;
    }
    if (dropdown_is_open) {
        return step_dropdown(pressed);
    }
    if (is_on_controls_page) {
        return step_controls_page(pressed);
    }
    if (pressed & INPUT_CIRCLE) {
        pause_options_leave();
        return PAUSE_OPTIONS_LEAVES;
    }
    if (pressed & (INPUT_UP | INPUT_DOWN)) {
        scroll_is_free = 0;
    }
    if (pressed & INPUT_UP) {
        selected = next_selectable(selected, -1);
    } else if (pressed & INPUT_DOWN) {
        selected = next_selectable(selected, 1);
    } else if (pressed & (INPUT_LEFT | INPUT_RIGHT)) {
        level = level_of(rows[selected].job);
        if (level != NULL) {
            set_level(rows[selected].job, *level + ((pressed & INPUT_RIGHT) ? 1 : -1));
        } else if (rows[selected].kind == ROW_SLIDER) {
            set_text_speed(game_settings_now()->text_speed + ((pressed & INPUT_RIGHT) ? 1 : -1));
        } else if (rows[selected].kind == ROW_CYCLES) {
            act_on_row();
        }
    } else if (pressed & INPUT_CROSS) {
        act_on_row();
    }
    return PAUSE_OPTIONS_STAYS;
}

static float scroll_most(void)
{
    float most;

    most = is_on_controls_page ? ROW_TOP + (float)CONTROLS_COUNT * ROW_STEP + LIST_BOTTOM_PAD - PAUSE_BAR_TOP : list_height - PAUSE_BAR_TOP;
    return most < 0.0f ? 0.0f : most;
}

static float clamp_scroll(float value)
{
    float most;

    most = scroll_most();
    return value < 0.0f ? 0.0f : (value > most ? most : value);
}

static float scroll_target(void)
{
    float bottom;

    if (is_on_controls_page) {
        bottom = ROW_TOP + (float)controls_selected * ROW_STEP + ROW_HEIGHT + SCROLL_KEEP_BELOW;
    } else {
        bottom = row_y[selected] + ROW_HEIGHT + SCROLL_KEEP_BELOW;
    }
    return clamp_scroll(bottom - PAUSE_BAR_TOP);
}

float pause_options_scroll(void)
{
    return scroll;
}

void pause_options_tick(void)
{
    float target;

    if (scroll_is_free) {
        return;
    }
    target = scroll_target();
    scroll = scroll + (target - scroll) * SCROLL_EASE;
    if (fabsf(target - scroll) < SCROLL_SNAP) {
        scroll = target;
    }
}

static int dropdown_row(void);

void pause_options_enter_on_language(void)
{
    pause_options_enter();
    selected = dropdown_row();
    scroll = scroll_target();
    dropdown_is_open = 1;
    dropdown_hovered = game_settings_now()->language;
    leaves_with_dropdown = 1;
}

static int row_at(float y)
{
    int index;

    if (y >= PAUSE_BAR_TOP) {
        return -1;
    }
    for (index = 0; index < row_count(); index++) {
        if (y >= row_y[index] - scroll && y <= row_y[index] - scroll + ROW_HEIGHT) {
            return index;
        }
    }
    return -1;
}

static int flame_at(float x)
{
    int step;

    for (step = 0; step < GAME_SETTING_STEPS; step++) {
        if (fabsf(x - (FLAME_FIRST_X + (float)step * FLAME_STEP)) <= FLAME_STEP / 2.0f) {
            return step + 1;
        }
    }
    return 0;
}

static int dropdown_row(void)
{
    int index;

    for (index = 0; index < row_count(); index++) {
        if (rows[index].kind == ROW_DROPDOWN) {
            return index;
        }
    }
    return 0;
}

static float dropdown_top(void)
{
    return row_y[dropdown_row()] + DROP_OVERLAP - (DROP_PAD + (float)LANGUAGE_COUNT * DROP_ITEM_STEP) - scroll;
}

static int language_at(float x, float y)
{
    int index;
    float centre;

    if (x < VALUE_LEFT || x > VALUE_RIGHT) {
        return -1;
    }
    for (index = 0; index < LANGUAGE_COUNT; index++) {
        centre = dropdown_top() + DROP_FIRST + (float)index * DROP_ITEM_STEP;
        if (fabsf(y - centre) <= DROP_ITEM_STEP / 2.0f) {
            return index;
        }
    }
    return -1;
}

static int drag_scrolls(int lands, int lifts, int is_down, float y)
{
    if (lands) {
        drag_is_tracking = y < PAUSE_BAR_TOP;
        drag_is_scrolling = 0;
        drag_land_y = y;
        drag_land_scroll = scroll;
        drag_level = NULL;
        return 0;
    }
    if (lifts) {
        drag_is_tracking = 0;
        drag_level = NULL;
        if (drag_is_scrolling) {
            drag_is_scrolling = 0;
            touched_row = -1;
            return 1;
        }
        return 0;
    }
    if (!is_down || !drag_is_tracking) {
        return 0;
    }
    if (!drag_is_scrolling && fabsf(y - drag_land_y) >= DRAG_START) {
        drag_is_scrolling = 1;
        scroll_is_free = 1;
        touched_row = -1;
        if (drag_level != NULL && *drag_level != drag_level_before) {
            *drag_level = drag_level_before;
            settings_now_changed("drag");
        }
    }
    if (drag_is_scrolling) {
        scroll = clamp_scroll(drag_land_scroll - (y - drag_land_y));
        return 1;
    }
    return 0;
}

unsigned int pause_options_touch(int lands, int lifts, int is_down, float x, float y)
{
    int row;
    int flame;

    if (lands && pause_is_on_back_pill(x, y)) {
        return INPUT_CIRCLE;
    }
    if (restore_dialog.is_open) {
        if (lands) {
            touched_option = pause_dialog_option_at(&restore_dialog, x, y);
            if (touched_option >= 0) {
                restore_dialog.selected = touched_option;
            }
        } else if (lifts && touched_option >= 0 && pause_dialog_option_at(&restore_dialog, x, y) == touched_option) {
            touched_option = PAUSE_DIALOG_NOTHING;
            return INPUT_CROSS;
        }
        return 0;
    }
    if (dropdown_is_open) {
        if (lands) {
            touched_option = language_at(x, y);
            if (touched_option < 0) {
                dropdown_is_open = 0;
            } else {
                dropdown_hovered = touched_option;
            }
        } else if (lifts && touched_option >= 0 && language_at(x, y) == touched_option) {
            touched_option = PAUSE_DIALOG_NOTHING;
            return INPUT_CROSS;
        }
        return 0;
    }
    if (drag_scrolls(lands, lifts, is_down, y)) {
        return 0;
    }
    if (is_on_controls_page) {
        return 0;
    }
    if (lands) {
        row = row_at(y);
        touched_row = is_selectable(row) ? row : -1;
        if (touched_row >= 0) {
            selected = touched_row;
            flame = rows[selected].kind == ROW_METER ? flame_at(x) : 0;
            if (rows[selected].kind == ROW_SLIDER && slider_stop_at(x) >= 0) {
                drag_level = &game_settings_now()->text_speed;
                drag_level_before = *drag_level;
                set_text_speed(slider_stop_at(x));
                touched_row = -1;
            } else if (flame > 0) {
                drag_level = level_of(rows[selected].job);
                drag_level_before = drag_level != NULL ? *drag_level : 0;
                set_level(rows[selected].job, flame);
                touched_row = -1;
            }
        }
    } else if (lifts && touched_row >= 0) {
        row = row_at(y);
        touched_row = -1;
        if (row == selected && rows[row].kind != ROW_METER && rows[row].kind != ROW_SLIDER) {
            return INPUT_CROSS;
        }
    }
    return 0;
}

static const char *value_key(const struct option_row *row, char *key, unsigned int key_size)
{
    if (row->fixed_value != NULL) {
        snprintf(key, key_size, "row.%s", row->fixed_value);
    } else if (row->job == JOB_MUSIC_TYPE) {
        snprintf(key, key_size, "row.%s", game_settings_now()->music_type == GAME_MUSIC_ORIGINAL ? "original" : "arranged");
    } else if (row->job == JOB_LANGUAGE) {
        snprintf(key, key_size, "language.%s", languages[game_settings_now()->language]);
    } else if (switch_of(row->job) != NULL) {
        snprintf(key, key_size, "row.%s", switch_value_name(row->job, *switch_of(row->job)));
    } else {
        return NULL;
    }
    return key;
}

static void draw_slider(float top, float middle, float x_offset, float alpha)
{
    float width;
    float left;
    float room;
    int stop;

    width = slider_stop_width();
    pause_draw_slice("value", VALUE_LEFT + x_offset, top + VALUE_INSET, VALUE_RIGHT - VALUE_LEFT, ROW_HEIGHT - 2.0f * VALUE_INSET, alpha);
    left = VALUE_LEFT + (float)game_settings_now()->text_speed * width;
    pause_draw_slice("tab_selected", left + SLIDER_KNOB_INSET + x_offset, top + VALUE_INSET + SLIDER_KNOB_INSET, width - 2.0f * SLIDER_KNOB_INSET, ROW_HEIGHT - 2.0f * (VALUE_INSET + SLIDER_KNOB_INSET), alpha);
    room = width - 2.0f * SLIDER_TEXT_PAD;
    for (stop = 0; stop < GAME_TEXT_SPEED_COUNT; stop++) {
        left = VALUE_LEFT + ((float)stop + 0.5f) * width;
        pause_draw_text_within(text_speed_keys[stop], left - pause_text_width(text_speed_keys[stop]) * pause_fit_scale(text_speed_keys[stop], room) / 2.0f + x_offset, middle, room, alpha);
    }
}

static void draw_row(int index, float x_offset)
{
    const struct option_row *row;
    char key[KEY_MAX];
    float top;
    float middle;
    float alpha;
    int *level;
    int step;
    float room;

    row = &rows[index];
    top = row_y[index] - scroll;
    middle = top + ROW_HEIGHT / 2.0f;
    alpha = is_dimmed(row) ? GREYED_ALPHA : 1.0f;
    if (row->header != NULL) {
        snprintf(key, sizeof(key), "header.%s", row->header);
        pause_draw_text(key, HEADER_TEXT_X + x_offset, top - HEADER_ABOVE, PAUSE_ANCHOR_LEFT, 1.0f);
    }
    pause_draw_slice(index == selected ? "row_selected" : "row", ROW_LEFT + x_offset, top, ROW_RIGHT - ROW_LEFT, ROW_HEIGHT, 1.0f);
    snprintf(key, sizeof(key), "row.%s", row->name);
    pause_draw_text(key, ROW_TEXT_X + x_offset, middle, PAUSE_ANCHOR_LEFT, alpha);
    if (row->kind == ROW_METER) {
        level = level_of(row->job);
        for (step = 0; step < GAME_SETTING_STEPS; step++) {
            pause_draw_flame(step + 1, level != NULL && step < *level, FLAME_FIRST_X + (float)step * FLAME_STEP + x_offset, middle - FLAME_RISE, alpha);
        }
        return;
    }
    if (row->kind == ROW_SLIDER) {
        draw_slider(top, middle, x_offset, alpha);
        return;
    }
    if (value_key(row, key, sizeof(key)) == NULL) {
        return;
    }
    pause_draw_slice("value", VALUE_LEFT + x_offset, top + VALUE_INSET, VALUE_RIGHT - VALUE_LEFT, ROW_HEIGHT - 2.0f * VALUE_INSET, alpha);
    room = VALUE_RIGHT - VALUE_LEFT - 2.0f * VALUE_TEXT_PAD;
    pause_draw_text_within(key, (VALUE_LEFT + VALUE_RIGHT) / 2.0f - pause_text_width(key) * pause_fit_scale(key, room) / 2.0f + x_offset, middle, room, alpha);
    if (row->kind == ROW_DROPDOWN) {
        pause_draw_cell("arrow_down", VALUE_RIGHT - ARROW_FROM_RIGHT + x_offset, middle, alpha);
    }
}

static void draw_dropdown(float x_offset)
{
    char key[KEY_MAX];
    float top;
    float centre;
    int index;

    top = dropdown_top();
    pause_draw_slice("box", VALUE_LEFT + x_offset, top, VALUE_RIGHT - VALUE_LEFT, DROP_PAD + (float)LANGUAGE_COUNT * DROP_ITEM_STEP, 1.0f);
    for (index = 0; index < LANGUAGE_COUNT; index++) {
        centre = top + DROP_FIRST + (float)index * DROP_ITEM_STEP;
        if (index == dropdown_hovered) {
            pause_draw_slice("choice", VALUE_LEFT + DROP_CHOICE_INSET + x_offset, centre - DROP_CHOICE_HALF, VALUE_RIGHT - VALUE_LEFT - 2.0f * DROP_CHOICE_INSET, 2.0f * DROP_CHOICE_HALF, 1.0f);
        }
        snprintf(key, sizeof(key), "language.%s", languages[index]);
        pause_draw_text(key, (VALUE_LEFT + VALUE_RIGHT) / 2.0f + x_offset, centre, PAUSE_ANCHOR_CENTRE, index == 0 ? 1.0f : GREYED_ALPHA);
    }
}

static void draw_control_glyphs(int control, float left, float centre_y)
{
    const char *glyph;
    int which;

    for (which = 0; which < CONTROLS_GLYPH_MOST; which++) {
        glyph = control_glyphs[control][which];
        if (glyph != NULL) {
            left = left + pause_draw_glyph_from_left(glyph, left, centre_y, CONTROLS_GLYPH_HEIGHT, 1.0f) + CONTROLS_GLYPH_GAP;
        }
    }
}

static void draw_controls_page(float x_offset)
{
    char key[KEY_MAX];
    float top;
    int index;

    for (index = 0; index < CONTROLS_COUNT; index++) {
        top = ROW_TOP + (float)index * ROW_STEP - scroll;
        if (index == 0) {
            pause_draw_text("header.controls", HEADER_TEXT_X + x_offset, top - HEADER_ABOVE, PAUSE_ANCHOR_LEFT, 1.0f);
        }
        pause_draw_slice(index == controls_selected ? "row_selected" : "row", ROW_LEFT + x_offset, top, ROW_RIGHT - ROW_LEFT, ROW_HEIGHT, 1.0f);
        draw_control_glyphs(index, ROW_TEXT_X + x_offset, top + ROW_HEIGHT / 2.0f);
        snprintf(key, sizeof(key), "bar.do_%s", control_keys[index]);
        pause_draw_text_within(key, CONTROLS_ACTION_X + x_offset, top + ROW_HEIGHT / 2.0f, CONTROLS_TEXT_END - CONTROLS_ACTION_X, 1.0f);
    }
}

float pause_options_preview_top(void)
{
    float row_top;
    float top;

    row_top = row_y[selected] - scroll;
    top = row_top - PREVIEW_GAP - PREVIEW_CARD_HEIGHT;
    return top >= PREVIEW_GAP ? top : row_top + ROW_HEIGHT + PREVIEW_GAP;
}

static void draw_contrast_preview(float x_offset)
{
    struct renderer_rect half;
    struct renderer_rect divider;
    float left;
    float top;
    float picture_width;
    float label_y;

    left = VALUE_LEFT + x_offset;
    top = pause_options_preview_top();
    picture_width = VALUE_RIGHT - VALUE_LEFT - 2.0f * PREVIEW_INSET;
    pause_draw_slice("box", left, top, VALUE_RIGHT - VALUE_LEFT, PREVIEW_CARD_HEIGHT, 1.0f);
    if (!pause_draw_preview(left + PREVIEW_INSET, top + PREVIEW_INSET, picture_width, PREVIEW_PICTURE_HEIGHT, 0.0f, 0.5f) || !pause_draw_preview(left + PREVIEW_INSET, top + PREVIEW_INSET, picture_width, PREVIEW_PICTURE_HEIGHT, 0.5f, 1.0f)) {
        return;
    }
    half.x = left + PREVIEW_INSET + picture_width / 2.0f;
    half.y = top + PREVIEW_INSET;
    half.width = picture_width / 2.0f;
    half.height = PREVIEW_PICTURE_HEIGHT;
    renderer_raise_contrast_in(half);
    divider.x = half.x - PREVIEW_DIVIDER / 2.0f;
    divider.y = half.y;
    divider.width = PREVIEW_DIVIDER;
    divider.height = PREVIEW_PICTURE_HEIGHT;
    renderer_draw_filled_rect(divider, pause_white(1.0f));
    label_y = top + PREVIEW_INSET + PREVIEW_PICTURE_HEIGHT + PREVIEW_LABEL_ROOM / 2.0f;
    pause_draw_text("row.off", left + PREVIEW_INSET + picture_width / 4.0f, label_y, PAUSE_ANCHOR_CENTRE, 1.0f);
    pause_draw_text("row.on", left + PREVIEW_INSET + 3.0f * picture_width / 4.0f, label_y, PAUSE_ANCHOR_CENTRE, 1.0f);
}

void pause_options_draw(float x_offset)
{
    int index;

    x_offset = whole(x_offset);
    if (is_on_controls_page) {
        draw_controls_page(x_offset);
        return;
    }
    for (index = 0; index < row_count(); index++) {
        if (row_y[index] - scroll < PAUSE_BAR_TOP && row_y[index] - scroll + ROW_HEIGHT > -HEADER_GAP) {
            draw_row(index, x_offset);
        }
    }
    if (dropdown_is_open) {
        draw_dropdown(x_offset);
    } else if (rows[selected].job == JOB_HIGH_CONTRAST) {
        draw_contrast_preview(x_offset);
    }
}

void pause_options_draw_bar(float x_offset)
{
    char key[KEY_MAX];

    x_offset = whole(x_offset);
    if (dropdown_is_open) {
        pause_draw_bar(x_offset, "bar.press", "cross", "bar.to_choose_language");
        return;
    }
    if (is_on_controls_page) {
        pause_draw_bar(x_offset, "bar.controls_page", NULL, NULL);
        return;
    }
    if (rows[selected].job == JOB_VISUAL_FILTER && game_settings_filter_is_pinned()) {
        snprintf(key, sizeof(key), "bar.%s_only", switch_value_name(rows[selected].job, 0));
    } else if (rows[selected].job == JOB_FPS_CAP || rows[selected].job == JOB_VISUAL_FILTER) {
        snprintf(key, sizeof(key), "bar.%s", switch_value_name(rows[selected].job, *switch_of(rows[selected].job)));
    } else {
        snprintf(key, sizeof(key), "bar.%s", rows[selected].name);
    }
    pause_draw_bar(x_offset, key, NULL, NULL);
}

void pause_options_draw_dialog(float x_offset)
{
    pause_dialog_draw(&restore_dialog, whole(x_offset));
}
