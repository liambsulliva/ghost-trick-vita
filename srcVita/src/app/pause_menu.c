#include <math.h>
#include <stdio.h>
#include <string.h>
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "pause_menu.h"
#include "pause_options.h"
#include "pause_people.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"
#include "stick_aim.h"
#include "text_lines.h"
#include "texture_cache.h"

#define LINE_MAX 160
#define CAT_PAGE_NAME_FORMAT "pause_cat_%02d"
#define BACKDROP_NAME "pause_backdrop"
#define SPOTLIGHT_NAME "pause_spotlight"
#define LABEL_SHEET_NAME "pause_labels"
#define CAT_TABLE_PATH "pause/cat.txt"
#define TRACK_TABLE_PATH "pause/cat_track.txt"
#define LABEL_TABLE_PATH "pause/labels.txt"

#define SCREEN_WIDTH ((float)RENDERER_WIDTH)
#define SCREEN_HEIGHT ((float)RENDERER_HEIGHT)
#define FLOOR_HEIGHT 52.0f
#define FLOOR_TOP (SCREEN_HEIGHT - FLOOR_HEIGHT)
#define FLOOR_LINE_HEIGHT 2.0f
#define CENTRE_X (SCREEN_WIDTH / 2.0f)
#define FEET_X CENTRE_X
#define FEET_Y (FLOOR_TOP + 9.0f)
#define SPOTLIGHT_CENTRE_Y (FLOOR_TOP - 80.0f)

#define DARKEN_TICKS 8
#define BLACK_TICKS 3
#define SHOW_FADE_TICKS 10
#define LABEL_STAGGER_TICKS 3
#define LABEL_APPEAR_TICKS 12
#define LABEL_SLIDE 40.0f
#define CURSOR_TICKS 6
#define PAN_TICKS 18
#define LABEL_FADE_TICKS 8

#define ARROW_GAP 26.0f
#define ARROW_RIGHT_BACK 13.0f
#define ARROW_LEFT_BACK 27.0f
#define ARROW_SHOWN_FROM 0.5f
#define TOUCH_MARGIN 14.0f

#define TRACK_IDLE 0
#define TRACK_FIDGET 1
#define TRACK_RETURN 2

static const float row_x[PAUSE_MENU_ROWS] = { 180.0f, 290.0f, 330.0f };
static const char *const item_names[PAUSE_MENU_ITEM_COUNT] = { "resume", "save", "people", "title", "chapters", "options" };

static struct pause_cat_frame cat_frames[PAUSE_CAT_FRAMES_MOST];
static int cat_frame_count;
static int cat_pages;
static int idle_rows[PAUSE_CAT_TICKS_MOST];
static int idle_count;
static int fidget_rows[PAUSE_CAT_TICKS_MOST];
static int fidget_count;
static int fidget_every;
static int fidget_link;
static int dissolve_ticks;
static struct pause_label labels[PAUSE_MENU_ITEM_COUNT];
static int label_count;
static float arrow_x;
static float arrow_y;
static float arrow_size;
static float selected_column;

static int track_mode;
static int track_position;
static int track_since;
static int track_step;
static struct pause_cat_step shown_step;

static enum pause_menu_phase phase;
static int phase_ticks;
static int shown_ticks;
static int cursor;
static int previous_cursor;
static int cursor_ticks;
static int picked;
static int widget_total;
static int stick_was_out;
static int touch_was_down;
static int touched_item;
static int touched_option;
static struct pause_dialog title_dialog;
static int title_is_wanted;

static float clamp01(float value)
{
    return value < 0.0f ? 0.0f : (value > 1.0f ? 1.0f : value);
}

static float ease_out(float t)
{
    float rest;

    rest = 1.0f - clamp01(t);
    return 1.0f - rest * rest * rest;
}

static float ease_snap(float t)
{
    float rest;

    rest = 1.0f - clamp01(t);
    return 1.0f - rest * rest * rest * rest;
}

static float whole(float value)
{
    return floorf(value + 0.5f);
}

static int side_of(int item)
{
    return item < PAUSE_MENU_ROWS ? -1 : 1;
}

static int row_of(int item)
{
    return item % PAUSE_MENU_ROWS;
}

int pause_menu_read_cat_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    char name[64];
    struct pause_cat_frame frame;
    unsigned int offset;
    int index;
    int ticks;

    cat_frame_count = 0;
    cat_pages = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (sscanf(line, "pages %d", &cat_pages) == 1) {
            continue;
        }
        if (cat_frame_count >= PAUSE_CAT_FRAMES_MOST) {
            continue;
        }
        if (sscanf(line, "frame %63s %d %d %d %f %f %f %f %d %d", name, &index, &ticks, &frame.page, &frame.x, &frame.y, &frame.width, &frame.height, &frame.left, &frame.top) == 10) {
            cat_frames[cat_frame_count] = frame;
            cat_frame_count = cat_frame_count + 1;
        }
    }
    if (cat_pages > PAUSE_CAT_PAGES_MOST) {
        cat_pages = PAUSE_CAT_PAGES_MOST;
    }
    return cat_frame_count;
}

static void add_row(int *rows, int *count, int row)
{
    if (*count < PAUSE_CAT_TICKS_MOST && row >= 0 && row < cat_frame_count) {
        rows[*count] = row;
        *count = *count + 1;
    }
}

int pause_menu_read_track_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    unsigned int offset;
    int row;

    idle_count = 0;
    fidget_count = 0;
    fidget_every = 0;
    fidget_link = 0;
    dissolve_ticks = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (sscanf(line, "idle %d", &row) == 1) {
            add_row(idle_rows, &idle_count, row);
        } else if (sscanf(line, "fidget %d", &row) == 1) {
            add_row(fidget_rows, &fidget_count, row);
        } else if (sscanf(line, "every %d", &row) == 1) {
            fidget_every = row;
        } else if (sscanf(line, "link %d", &row) == 1) {
            fidget_link = row;
        } else if (sscanf(line, "dissolve %d", &row) == 1) {
            dissolve_ticks = row;
        }
    }
    if (fidget_link >= idle_count || dissolve_ticks >= fidget_count) {
        fidget_count = 0;
    }
    return idle_count;
}

int pause_menu_read_label_text(const char *text, unsigned int text_size)
{
    char line[LINE_MAX];
    struct pause_label label;
    unsigned int offset;
    int index;

    label_count = 0;
    arrow_size = 0.0f;
    selected_column = 0.0f;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (sscanf(line, "label %d %f %f %f %f", &index, &label.x, &label.y, &label.width, &label.height) == 5) {
            if (index >= 0 && index < PAUSE_MENU_ITEM_COUNT) {
                labels[index] = label;
                if (index + 1 > label_count) {
                    label_count = index + 1;
                }
            }
        } else if (sscanf(line, "arrows %f %f %f", &arrow_x, &arrow_y, &arrow_size) == 3) {
            continue;
        } else {
            (void)sscanf(line, "selected_column %f", &selected_column);
        }
    }
    return label_count;
}

static int read_table(const char *assets_folder, const char *relative_path, int (*reader)(const char *, unsigned int))
{
    char path[FILE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    int count;

    if (!file_path_join(path, assets_folder, relative_path, "") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    count = reader((const char *)bytes, size);
    mem_free(bytes);
    return count;
}

int pause_menu_load(const char *assets_folder)
{
    int frames;
    int ticks;
    int words;

    widget_total = 0;
    cat_frame_count = 0;
    cat_pages = 0;
    idle_count = 0;
    fidget_count = 0;
    label_count = 0;
    arrow_size = 0.0f;
    frames = read_table(assets_folder, CAT_TABLE_PATH, pause_menu_read_cat_text);
    ticks = frames > 0 ? read_table(assets_folder, TRACK_TABLE_PATH, pause_menu_read_track_text) : 0;
    words = read_table(assets_folder, LABEL_TABLE_PATH, pause_menu_read_label_text);
    widget_total = pause_widgets_load(assets_folder);
    phase = PAUSE_PHASE_CLOSED;
    run_log_write("pause art frames=%d idle_ticks=%d fidget_ticks=%d labels=%d widgets=%d people=%d", frames, ticks, fidget_count, words, widget_total, pause_people_load(assets_folder));
    return pause_menu_is_ready();
}

int pause_menu_is_ready(void)
{
    return idle_count > 0 && label_count == PAUSE_MENU_ITEM_COUNT && arrow_size > 0.0f && widget_total > 0;
}

const struct pause_cat_frame *pause_cat_frame_at(int row)
{
    return row >= 0 && row < cat_frame_count ? &cat_frames[row] : NULL;
}

int pause_cat_page_count(void)
{
    return cat_pages;
}

void pause_cat_track_restart(void)
{
    track_mode = TRACK_IDLE;
    track_position = 0;
    track_since = 0;
    track_step = 0;
}

static float dissolve_weight(int step)
{
    return (float)(step + 1) / (float)(dissolve_ticks + 1);
}

struct pause_cat_step pause_cat_track_next(void)
{
    struct pause_cat_step step;

    step.second = -1;
    step.weight = 0.0f;
    if (idle_count == 0) {
        step.first = -1;
        return step;
    }
    if (track_mode == TRACK_IDLE) {
        if (fidget_count > 0 && track_since >= fidget_every && track_position % idle_count == fidget_link) {
            track_mode = TRACK_FIDGET;
            track_step = 0;
        } else {
            step.first = idle_rows[track_position % idle_count];
            track_position = track_position + 1;
            track_since = track_since + 1;
            return step;
        }
    }
    if (track_mode == TRACK_FIDGET) {
        if (track_step < dissolve_ticks) {
            step.first = idle_rows[(track_position + track_step) % idle_count];
            step.second = fidget_rows[track_step];
            step.weight = dissolve_weight(track_step);
        } else {
            step.first = fidget_rows[track_step];
        }
        track_step = track_step + 1;
        if (track_step == fidget_count) {
            track_mode = TRACK_RETURN;
            track_step = 0;
        }
        return step;
    }
    step.first = fidget_rows[fidget_count - 1];
    step.second = idle_rows[(fidget_link + track_step) % idle_count];
    step.weight = dissolve_weight(track_step);
    track_step = track_step + 1;
    if (track_step == dissolve_ticks) {
        track_mode = TRACK_IDLE;
        track_position = fidget_link + dissolve_ticks;
        track_since = 0;
    }
    return step;
}

static void cat_page_name(char *name, unsigned int size, int page)
{
    snprintf(name, size, CAT_PAGE_NAME_FORMAT, page);
}

static int texture_is_settled(const char *name)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(name);
    return texture == NULL || texture->state != CACHED_TEXTURE_LOADING;
}

static int art_is_settled(void)
{
    char name[ASSET_NAME_MAX];
    int settled;
    int page;

    settled = texture_is_settled(BACKDROP_NAME);
    settled = texture_is_settled(SPOTLIGHT_NAME) && settled;
    settled = texture_is_settled(LABEL_SHEET_NAME) && settled;
    settled = texture_is_settled(PAUSE_WIDGET_SHEET) && settled;
    settled = texture_is_settled(PAUSE_GLYPH_SHEET) && settled;
    for (page = 0; page < cat_pages; page++) {
        cat_page_name(name, sizeof(name), page);
        settled = texture_is_settled(name) && settled;
    }
    return settled;
}

static void keep_art(void)
{
    char name[ASSET_NAME_MAX];
    int page;

    texture_cache_keep_or_start_reading_ahead(BACKDROP_NAME);
    texture_cache_keep_or_start_reading_ahead(SPOTLIGHT_NAME);
    texture_cache_keep_or_start_reading_ahead(LABEL_SHEET_NAME);
    texture_cache_keep_or_start_reading_ahead(PAUSE_WIDGET_SHEET);
    texture_cache_keep_or_start_reading_ahead(PAUSE_GLYPH_SHEET);
    for (page = 0; page < cat_pages; page++) {
        cat_page_name(name, sizeof(name), page);
        texture_cache_keep_or_start_reading_ahead(name);
    }
}

static void enter(enum pause_menu_phase next)
{
    phase = next;
    phase_ticks = 0;
}

void pause_menu_open(void)
{
    if (phase != PAUSE_PHASE_CLOSED || !pause_menu_is_ready()) {
        return;
    }
    cursor = PAUSE_ITEM_RESUME;
    previous_cursor = -1;
    cursor_ticks = CURSOR_TICKS;
    picked = PAUSE_ITEM_RESUME;
    stick_was_out = 1;
    touch_was_down = 1;
    touched_item = -1;
    touched_option = PAUSE_DIALOG_NOTHING;
    title_dialog.is_open = 0;
    title_is_wanted = 0;
    shown_ticks = 0;
    pause_cat_track_restart();
    shown_step.first = -1;
    shown_step.second = -1;
    shown_step.weight = 0.0f;
    enter(PAUSE_PHASE_DARKENING);
    run_log_write("pause open");
}

int pause_menu_is_open(void)
{
    return phase != PAUSE_PHASE_CLOSED;
}

int pause_menu_shows_game(void)
{
    return phase == PAUSE_PHASE_CLOSED || phase == PAUSE_PHASE_DARKENING || phase == PAUSE_PHASE_BRIGHTENING;
}

enum pause_menu_phase pause_menu_phase(void)
{
    return phase;
}

int pause_menu_cursor(void)
{
    return cursor;
}

static void move_cursor(int next)
{
    if (next == cursor) {
        return;
    }
    previous_cursor = cursor;
    cursor = next;
    cursor_ticks = 0;
}

static void steer(unsigned int pressed)
{
    int row;

    row = row_of(cursor);
    if ((pressed & INPUT_UP) && row > 0) {
        move_cursor(cursor - 1);
    } else if ((pressed & INPUT_DOWN) && row < PAUSE_MENU_ROWS - 1) {
        move_cursor(cursor + 1);
    } else if ((pressed & INPUT_LEFT) && side_of(cursor) > 0) {
        move_cursor(cursor - PAUSE_MENU_ROWS);
    } else if ((pressed & INPUT_RIGHT) && side_of(cursor) < 0) {
        move_cursor(cursor + PAUSE_MENU_ROWS);
    }
}

static void begin_hiding(void)
{
    run_log_write("pause close");
    enter(PAUSE_PHASE_HIDING);
}

static void leave_submenu(void)
{
    if (picked == PAUSE_ITEM_OPTIONS) {
        pause_options_leave();
    }
}

static void open_title_dialog(void)
{
    title_dialog.line_count = 1;
    title_dialog.lines[0] = "row.title_ask";
    title_dialog.note = "bar.title_warning";
    title_dialog.option_count = 2;
    title_dialog.options[0] = "row.yes";
    title_dialog.options[1] = "row.no";
    pause_dialog_open(&title_dialog, 1);
}

static void step_title(unsigned int pressed)
{
    int chosen;

    chosen = pause_dialog_steer(&title_dialog, pressed);
    if (chosen == 0) {
        title_is_wanted = 1;
        run_log_write("pause return_to_title");
        begin_hiding();
    } else if (chosen != PAUSE_DIALOG_NOTHING) {
        enter(PAUSE_PHASE_PANNING_BACK);
    }
}

int pause_menu_takes_title_request(void)
{
    if (!title_is_wanted || phase != PAUSE_PHASE_BLACK_BEFORE_GAME) {
        return 0;
    }
    title_is_wanted = 0;
    return 1;
}

static int people_is_picked(void)
{
    return picked == PAUSE_ITEM_PEOPLE && pause_people_is_loaded();
}

static int submenu_is_on_screen(void)
{
    return phase == PAUSE_PHASE_PANNING_OUT || phase == PAUSE_PHASE_SUBMENU || phase == PAUSE_PHASE_PANNING_BACK;
}

static void step_showing(unsigned int pressed)
{
    if (pressed & (INPUT_CIRCLE | INPUT_START)) {
        begin_hiding();
        return;
    }
    if (pressed & INPUT_CROSS) {
        if (cursor == PAUSE_ITEM_RESUME) {
            begin_hiding();
            return;
        }
        picked = cursor;
        if (picked == PAUSE_ITEM_OPTIONS) {
            pause_options_enter();
            run_log_write("pause pick %s", item_names[picked]);
        } else if (picked == PAUSE_ITEM_PEOPLE && pause_people_is_loaded()) {
            pause_people_enter();
            run_log_write("pause pick %s", item_names[picked]);
        } else if (picked == PAUSE_ITEM_TITLE) {
            run_log_write("pause pick %s", item_names[picked]);
        } else {
            run_log_write("pause pick %s submenu=not_ported", item_names[picked]);
        }
        enter(PAUSE_PHASE_PANNING_OUT);
        return;
    }
    steer(pressed);
}

static void advance_phase(unsigned int pressed)
{
    switch (phase) {
    case PAUSE_PHASE_DARKENING:
        if (phase_ticks >= DARKEN_TICKS) {
            enter(PAUSE_PHASE_WAITING_FOR_ART);
        }
        break;
    case PAUSE_PHASE_WAITING_FOR_ART:
        if (phase_ticks >= BLACK_TICKS && art_is_settled()) {
            enter(PAUSE_PHASE_SHOWING);
        }
        break;
    case PAUSE_PHASE_SHOWING:
        step_showing(pressed);
        break;
    case PAUSE_PHASE_PANNING_OUT:
        if (phase_ticks >= PAN_TICKS) {
            enter(PAUSE_PHASE_SUBMENU);
            if (picked == PAUSE_ITEM_TITLE) {
                open_title_dialog();
            }
        }
        break;
    case PAUSE_PHASE_SUBMENU:
        if (pressed & INPUT_START) {
            leave_submenu();
            begin_hiding();
        } else if (picked == PAUSE_ITEM_OPTIONS) {
            if (pause_options_step(pressed) == PAUSE_OPTIONS_LEAVES) {
                enter(PAUSE_PHASE_PANNING_BACK);
            }
        } else if (people_is_picked()) {
            if (pause_people_step(pressed) == PAUSE_PEOPLE_LEAVES) {
                enter(PAUSE_PHASE_PANNING_BACK);
            }
        } else if (picked == PAUSE_ITEM_TITLE) {
            step_title(pressed);
        } else if (pressed & INPUT_CIRCLE) {
            enter(PAUSE_PHASE_PANNING_BACK);
        }
        break;
    case PAUSE_PHASE_PANNING_BACK:
        if (phase_ticks >= PAN_TICKS) {
            enter(PAUSE_PHASE_SHOWING);
            phase_ticks = SHOW_FADE_TICKS;
        }
        break;
    case PAUSE_PHASE_HIDING:
        if (phase_ticks >= DARKEN_TICKS) {
            enter(PAUSE_PHASE_BLACK_BEFORE_GAME);
        }
        break;
    case PAUSE_PHASE_BLACK_BEFORE_GAME:
        if (phase_ticks >= BLACK_TICKS) {
            enter(PAUSE_PHASE_BRIGHTENING);
        }
        break;
    case PAUSE_PHASE_BRIGHTENING:
        if (phase_ticks >= SHOW_FADE_TICKS) {
            enter(PAUSE_PHASE_CLOSED);
        }
        break;
    case PAUSE_PHASE_CLOSED:
        break;
    }
}

static int cat_is_on_screen(void)
{
    return phase >= PAUSE_PHASE_SHOWING && phase <= PAUSE_PHASE_HIDING;
}

void pause_menu_step(unsigned int pressed)
{
    enum pause_menu_phase before;

    if (phase == PAUSE_PHASE_CLOSED) {
        return;
    }
    before = phase;
    if (cat_is_on_screen()) {
        shown_step = pause_cat_track_next();
        shown_ticks = shown_ticks + 1;
        if (cursor_ticks < CURSOR_TICKS) {
            cursor_ticks = cursor_ticks + 1;
        }
    }
    if (submenu_is_on_screen() && picked == PAUSE_ITEM_OPTIONS) {
        pause_options_tick();
    }
    phase_ticks = phase_ticks + 1;
    advance_phase(pressed);
    if (phase == PAUSE_PHASE_SHOWING && before == PAUSE_PHASE_WAITING_FOR_ART) {
        pause_cat_track_restart();
        shown_step = pause_cat_track_next();
    }
}

static unsigned int stick_presses(const struct input_state *input)
{
    int pressed_x;
    int pressed_y;
    int stick_is_out;
    unsigned int pressed;

    pressed = 0;
    stick_is_out = input->stick_x != 0.0f || input->stick_y != 0.0f;
    if (stick_is_out && !stick_was_out) {
        stick_aim_first_push(input->stick_x, input->stick_y, &pressed_x, &pressed_y);
        pressed = (pressed_x < 0 ? INPUT_LEFT : 0) | (pressed_x > 0 ? INPUT_RIGHT : 0) | (pressed_y < 0 ? INPUT_UP : 0) | (pressed_y > 0 ? INPUT_DOWN : 0);
    }
    stick_was_out = stick_is_out;
    return pressed;
}

static float label_centre_x(int item)
{
    return CENTRE_X + (float)side_of(item) * row_x[row_of(item)];
}

static float label_centre_y(int item)
{
    return whole(FLOOR_TOP * (float)(row_of(item) + 1) / 4.0f);
}

static int item_under(float x, float y)
{
    int item;

    for (item = 0; item < PAUSE_MENU_ITEM_COUNT; item++) {
        float half_width;
        float half_height;

        half_width = labels[item].width / 2.0f + TOUCH_MARGIN;
        half_height = labels[item].height / 2.0f + TOUCH_MARGIN;
        if (fabsf(x - label_centre_x(item)) <= half_width && fabsf(y - label_centre_y(item)) <= half_height) {
            return item;
        }
    }
    return -1;
}

static unsigned int touch_presses(const struct input_state *input)
{
    unsigned int pressed;
    int lands;
    int lifts;

    pressed = 0;
    lands = input->touch_is_down && !touch_was_down;
    lifts = !input->touch_is_down && touch_was_down;
    if (phase == PAUSE_PHASE_SHOWING) {
        if (lands) {
            touched_item = item_under(input->touch_x, input->touch_y);
            if (touched_item >= 0) {
                move_cursor(touched_item);
            }
        } else if (input->touch_is_down && touched_item >= 0 && item_under(input->touch_x, input->touch_y) != touched_item) {
            touched_item = -1;
        } else if (lifts && touched_item >= 0) {
            pressed = INPUT_CROSS;
        }
    } else if (phase == PAUSE_PHASE_SUBMENU && picked == PAUSE_ITEM_OPTIONS) {
        pressed = pause_options_touch(lands, lifts, input->touch_is_down, input->touch_x, input->touch_y);
    } else if (phase == PAUSE_PHASE_SUBMENU && picked == PAUSE_ITEM_TITLE && title_dialog.is_open) {
        if (lands) {
            touched_option = pause_dialog_option_at(&title_dialog, input->touch_x, input->touch_y);
            if (touched_option >= 0) {
                title_dialog.selected = touched_option;
            }
        } else if (lifts && touched_option >= 0 && pause_dialog_option_at(&title_dialog, input->touch_x, input->touch_y) == touched_option) {
            pressed = INPUT_CROSS;
        }
        if (lifts) {
            touched_option = PAUSE_DIALOG_NOTHING;
        }
    } else if (phase == PAUSE_PHASE_SUBMENU && people_is_picked()) {
        pressed = pause_people_touch(lands, input->touch_x, input->touch_y);
    } else if (phase == PAUSE_PHASE_SUBMENU && lands && pause_is_on_back_pill(input->touch_x, input->touch_y)) {
        pressed = INPUT_CIRCLE;
    }
    if (lifts) {
        touched_item = -1;
    }
    touch_was_down = input->touch_is_down;
    return pressed;
}

void pause_menu_update(const struct input_state *input, unsigned long long elapsed_us)
{
    unsigned long long steps;
    unsigned int pressed;

    if (phase == PAUSE_PHASE_CLOSED) {
        return;
    }
    keep_art();
    steps = elapsed_us > PAUSE_TICK_US / 2ull ? (elapsed_us + PAUSE_TICK_US / 2ull) / PAUSE_TICK_US : 1ull;
    if (steps > PAUSE_TICKS_PER_UPDATE_MOST) {
        steps = PAUSE_TICKS_PER_UPDATE_MOST;
    }
    pressed = input->pressed | stick_presses(input) | touch_presses(input);
    pause_menu_step(pressed);
    while (steps > 1ull) {
        pause_menu_step(0);
        steps = steps - 1ull;
    }
}

float pause_menu_camera_x(void)
{
    float side;

    side = (float)side_of(picked) * SCREEN_WIDTH;
    if (phase == PAUSE_PHASE_PANNING_OUT) {
        return side * ease_snap((float)phase_ticks / (float)PAN_TICKS);
    }
    if (phase == PAUSE_PHASE_SUBMENU) {
        return side;
    }
    if (phase == PAUSE_PHASE_PANNING_BACK) {
        return side * (1.0f - ease_snap((float)phase_ticks / (float)PAN_TICKS));
    }
    return 0.0f;
}

float pause_menu_label_alpha(void)
{
    if (phase == PAUSE_PHASE_PANNING_OUT) {
        return 1.0f - clamp01((float)phase_ticks / (float)LABEL_FADE_TICKS);
    }
    if (phase == PAUSE_PHASE_SUBMENU) {
        return 0.0f;
    }
    if (phase == PAUSE_PHASE_PANNING_BACK) {
        return clamp01((float)(phase_ticks - 1 - (PAN_TICKS - LABEL_FADE_TICKS)) / (float)LABEL_FADE_TICKS);
    }
    return 1.0f;
}

float pause_menu_black_alpha(void)
{
    switch (phase) {
    case PAUSE_PHASE_DARKENING:
        return clamp01((float)phase_ticks / (float)DARKEN_TICKS);
    case PAUSE_PHASE_WAITING_FOR_ART:
    case PAUSE_PHASE_BLACK_BEFORE_GAME:
        return 1.0f;
    case PAUSE_PHASE_SHOWING:
        return 1.0f - clamp01((float)phase_ticks / (float)SHOW_FADE_TICKS);
    case PAUSE_PHASE_HIDING:
        return clamp01((float)phase_ticks / (float)DARKEN_TICKS);
    case PAUSE_PHASE_BRIGHTENING:
        return 1.0f - clamp01((float)phase_ticks / (float)SHOW_FADE_TICKS);
    default:
        return 0.0f;
    }
}

static struct renderer_color white(float alpha)
{
    struct renderer_color color;

    color.red = 1.0f;
    color.green = 1.0f;
    color.blue = 1.0f;
    color.alpha = alpha;
    return color;
}

static struct renderer_rect rect(float x, float y, float width, float height)
{
    struct renderer_rect area;

    area.x = x;
    area.y = y;
    area.width = width;
    area.height = height;
    return area;
}

static void draw_part(const struct cached_texture *texture, float x, float y, float u, float v, float width, float height, float alpha)
{
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY || texture->width == 0 || texture->height == 0 || alpha <= 0.0f) {
        return;
    }
    renderer_draw_sprite(texture->texture, rect(x, y, width, height), rect(u / (float)texture->width, v / (float)texture->height, width / (float)texture->width, height / (float)texture->height), white(alpha));
}

static void draw_whole(const char *name, float x, float y, float alpha)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(name);
    if (texture != NULL) {
        draw_part(texture, x, y, 0.0f, 0.0f, (float)texture->width, (float)texture->height, alpha);
    }
}

static void draw_cat_frame(int row, float feet_x, float alpha)
{
    char name[ASSET_NAME_MAX];
    const struct pause_cat_frame *frame;

    frame = pause_cat_frame_at(row);
    if (frame == NULL) {
        return;
    }
    cat_page_name(name, sizeof(name), frame->page);
    draw_part(texture_cache_use_or_start_loading(name), whole(feet_x) + (float)frame->left, FEET_Y + (float)frame->top, frame->x, frame->y, frame->width, frame->height, alpha);
}

static float cursor_weight(int item)
{
    float moved;

    moved = ease_out((float)cursor_ticks / (float)CURSOR_TICKS);
    if (item == cursor) {
        return moved;
    }
    return item == previous_cursor ? 1.0f - moved : 0.0f;
}

static void draw_labels(float camera_x, float alpha)
{
    const struct cached_texture *sheet;
    int item;

    sheet = texture_cache_use_or_start_loading(LABEL_SHEET_NAME);
    for (item = 0; item < PAUSE_MENU_ITEM_COUNT; item++) {
        const struct pause_label *label;
        float appear;
        float weight;
        float x;
        float y;
        float half;

        appear = ease_out((float)(shown_ticks - item * LABEL_STAGGER_TICKS) / (float)LABEL_APPEAR_TICKS);
        if (appear <= 0.0f) {
            continue;
        }
        label = &labels[item];
        weight = cursor_weight(item);
        x = label_centre_x(item) + (float)side_of(item) * LABEL_SLIDE * (1.0f - appear) - camera_x;
        y = label_centre_y(item);
        draw_part(sheet, whole(x - label->width / 2.0f), whole(y - label->height / 2.0f), label->x, label->y, label->width, label->height, alpha);
        draw_part(sheet, whole(x - label->width / 2.0f), whole(y - label->height / 2.0f), label->x + selected_column, label->y, label->width, label->height, alpha * weight);
        if (weight > ARROW_SHOWN_FROM) {
            half = label->width / 2.0f;
            if (side_of(item) < 0) {
                draw_part(sheet, whole(x - half - ARROW_GAP - ARROW_RIGHT_BACK), whole(y - arrow_size / 2.0f), arrow_x, arrow_y, arrow_size, arrow_size, alpha);
            } else {
                draw_part(sheet, whole(x + half + ARROW_GAP - ARROW_LEFT_BACK), whole(y - arrow_size / 2.0f), arrow_x + arrow_size, arrow_y, arrow_size, arrow_size, alpha);
            }
        }
    }
}

static void draw_floor(void)
{
    struct renderer_color color;

    color.red = 0.0f;
    color.green = 0.0f;
    color.blue = 0.0f;
    color.alpha = 1.0f;
    renderer_draw_filled_rect(rect(0.0f, FLOOR_TOP, SCREEN_WIDTH, FLOOR_HEIGHT), color);
    color.red = 110.0f / 255.0f;
    color.green = 16.0f / 255.0f;
    color.blue = 16.0f / 255.0f;
    renderer_draw_filled_rect(rect(0.0f, FLOOR_TOP - FLOOR_LINE_HEIGHT / 2.0f, SCREEN_WIDTH, FLOOR_LINE_HEIGHT), color);
}

static void draw_black(float alpha)
{
    struct renderer_color color;

    if (alpha <= 0.0f) {
        return;
    }
    color.red = 0.0f;
    color.green = 0.0f;
    color.blue = 0.0f;
    color.alpha = alpha;
    renderer_draw_filled_rect(rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT), color);
}

static void draw_menu(void)
{
    const struct cached_texture *spotlight;
    float camera_x;
    float side_x;

    camera_x = whole(pause_menu_camera_x());
    side_x = (float)side_of(picked) * SCREEN_WIDTH - camera_x;
    draw_whole(BACKDROP_NAME, -camera_x, 0.0f, 1.0f);
    if (camera_x != 0.0f) {
        draw_whole(BACKDROP_NAME, side_x, 0.0f, 1.0f);
    }
    spotlight = texture_cache_use_or_start_loading(SPOTLIGHT_NAME);
    if (spotlight != NULL) {
        draw_part(spotlight, CENTRE_X - (float)(spotlight->width / 2u) - camera_x, SPOTLIGHT_CENTRE_Y - (float)(spotlight->height / 2u), 0.0f, 0.0f, (float)spotlight->width, (float)spotlight->height, 1.0f);
    }
    if (camera_x != 0.0f && picked == PAUSE_ITEM_OPTIONS) {
        pause_options_draw(side_x);
    } else if (camera_x != 0.0f && people_is_picked()) {
        pause_people_draw(side_x);
    }
    draw_floor();
    if (camera_x != 0.0f) {
        if (picked == PAUSE_ITEM_OPTIONS) {
            pause_options_draw_bar(side_x);
        } else if (people_is_picked()) {
            pause_people_draw_bar(side_x);
        } else if (picked == PAUSE_ITEM_TITLE) {
            pause_draw_bar(side_x, "bar.title", NULL, NULL);
        } else {
            pause_draw_bar(side_x, NULL, NULL, NULL);
        }
    }
    draw_labels(camera_x, pause_menu_label_alpha());
    draw_cat_frame(shown_step.first, FEET_X - camera_x, 1.0f);
    if (shown_step.second >= 0) {
        draw_cat_frame(shown_step.second, FEET_X - camera_x, shown_step.weight);
    }
    if (camera_x != 0.0f && picked == PAUSE_ITEM_OPTIONS) {
        pause_options_draw_dialog(side_x);
    } else if (camera_x != 0.0f && picked == PAUSE_ITEM_TITLE) {
        pause_dialog_draw(&title_dialog, whole(side_x));
    }
}

void pause_menu_draw(void)
{
    if (phase == PAUSE_PHASE_CLOSED) {
        return;
    }
    if (cat_is_on_screen()) {
        draw_menu();
    }
    draw_black(pause_menu_black_alpha());
}
