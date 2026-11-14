#include <math.h>
#include <stddef.h>
#include "game_save.h"
#include "menu_input.h"
#include "pause_chapters.h"
#include "pause_menu.h"
#include "pause_options.h"
#include "pause_people.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"
#include "texture_cache.h"
#include "title_menu.h"

#define BACKDROP_NAME "pause_backdrop"
#define SCREEN_WIDTH ((float)RENDERER_WIDTH)
#define SCREEN_HEIGHT ((float)RENDERER_HEIGHT)
#define CENTRE_X (SCREEN_WIDTH / 2.0f)

#define BLACK_TICKS 3
#define SHOW_FADE_TICKS 10
#define PAN_TICKS 18
#define LABEL_FADE_TICKS 8
#define HIDE_TICKS 16

#define LOGO_CENTRE_Y 118.0f
#define LOGO_WIDTH 600.0f
#define LEFT_COLUMN_X 330.0f
#define RIGHT_COLUMN_X 630.0f
#define FIRST_ROW_Y 262.0f
#define SECOND_ROW_Y 350.0f
#define BOTTOM_ROW_Y 438.0f
#define BUTTON_WIDTH 280.0f
#define BUTTON_HEIGHT 68.0f
#define LANGUAGE_HEIGHT 56.0f
#define ICON_SIZE 66.0f
#define ERASE_X 900.0f
#define RECORD_X 60.0f
#define RECORD_Y 60.0f
#define LABEL_ROOM 240.0f
#define NOTEBOOK_U 80.0f
#define NOTEBOOK_V 181.0f
#define NOTEBOOK_TEXELS 80.0f
#define NOTEBOOK_SIZE 52.0f
#define TOUCH_MARGIN 10.0f
#define ACROSS_WEIGHT 2.0f
#define CONE_SLOPE 0.5f

#define JOB_NONE 0
#define JOB_ERASE 1
#define JOB_NEW_GAME 2

struct title_place {
    float x;
    float y;
    float width;
    float height;
};

static const char *const item_names[TITLE_MENU_ITEM_COUNT] = { "chapters", "continue", "erase", "language", "record", "new_game", "options" };
static const char *const label_keys[TITLE_MENU_ITEM_COUNT] = { "title.chapter_select", "title.continue", NULL, "title.language", NULL, "title.new_game", "title.options" };
static const char *const bar_keys[TITLE_MENU_ITEM_COUNT] = { "bar.title_chapters", "bar.title_continue", "bar.title_erase", "bar.title_language", "bar.title_record", "bar.title_new_game", "bar.title_options" };

static title_save_check_function save_exists;
static title_save_check_function game_is_cleared;
static title_save_check_function erase_every_save;
static struct title_place places[TITLE_MENU_ITEM_COUNT];
static int shown[TITLE_MENU_ITEM_COUNT];
static enum title_menu_phase phase;
static int phase_ticks;
static int cursor;
static int picked;
static struct pause_dialog dialog;
static int dialog_job;
static enum title_menu_request request;
static int request_chapter;
static struct menu_input cursor_input;
static int touch_was_down;
static int touched_item;
static int touched_option;

void title_menu_use(title_save_check_function has_save, title_save_check_function is_cleared, title_save_check_function erase_saves)
{
    save_exists = has_save;
    game_is_cleared = is_cleared;
    erase_every_save = erase_saves;
}

int title_menu_is_ready(void)
{
    return pause_widgets_has("slice.title_button") && pause_widgets_has("title.new_game");
}

static float clamp01(float value)
{
    return value < 0.0f ? 0.0f : (value > 1.0f ? 1.0f : value);
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

static void place(int item, float x, float y, float width, float height)
{
    shown[item] = 1;
    places[item].x = x;
    places[item].y = y;
    places[item].width = width;
    places[item].height = height;
}

static void lay_out(void)
{
    int item;
    int has_save;

    for (item = 0; item < TITLE_MENU_ITEM_COUNT; item++) {
        shown[item] = 0;
    }
    has_save = save_exists != NULL && save_exists();
    if (has_save) {
        place(TITLE_ITEM_CHAPTERS, LEFT_COLUMN_X, FIRST_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
        place(TITLE_ITEM_CONTINUE, RIGHT_COLUMN_X, FIRST_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
        place(TITLE_ITEM_NEW_GAME, LEFT_COLUMN_X, SECOND_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
        place(TITLE_ITEM_OPTIONS, RIGHT_COLUMN_X, SECOND_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
        place(TITLE_ITEM_ERASE, ERASE_X, BOTTOM_ROW_Y, ICON_SIZE, ICON_SIZE);
        if (game_is_cleared != NULL && game_is_cleared() && pause_people_is_loaded()) {
            place(TITLE_ITEM_RECORD, RECORD_X, RECORD_Y, ICON_SIZE, ICON_SIZE);
        }
    } else {
        place(TITLE_ITEM_NEW_GAME, CENTRE_X, FIRST_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
        place(TITLE_ITEM_OPTIONS, CENTRE_X, SECOND_ROW_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
    }
    place(TITLE_ITEM_LANGUAGE, CENTRE_X, BOTTOM_ROW_Y, BUTTON_WIDTH, LANGUAGE_HEIGHT);
    cursor = has_save ? TITLE_ITEM_CONTINUE : TITLE_ITEM_NEW_GAME;
}

static void enter(enum title_menu_phase next)
{
    phase = next;
    phase_ticks = 0;
}

void title_menu_open(void)
{
    lay_out();
    picked = cursor;
    dialog.is_open = 0;
    dialog_job = JOB_NONE;
    request = TITLE_REQUEST_NONE;
    request_chapter = 0;
    menu_input_reset(&cursor_input);
    touch_was_down = 1;
    touched_item = -1;
    touched_option = PAUSE_DIALOG_NOTHING;
    enter(TITLE_PHASE_WAITING_FOR_ART);
    run_log_write("title open save=%d cleared=%d", shown[TITLE_ITEM_CONTINUE], shown[TITLE_ITEM_RECORD]);
}

void title_menu_close(void)
{
    if (phase != TITLE_PHASE_CLOSED) {
        run_log_write("title closed");
    }
    phase = TITLE_PHASE_CLOSED;
    request = TITLE_REQUEST_NONE;
}

int title_menu_is_open(void)
{
    return phase != TITLE_PHASE_CLOSED;
}

int title_menu_shows(int item)
{
    return item >= 0 && item < TITLE_MENU_ITEM_COUNT && shown[item];
}

int title_menu_cursor(void)
{
    return cursor;
}

enum title_menu_phase title_menu_phase(void)
{
    return phase;
}

int title_menu_dialog_is_open(void)
{
    return dialog.is_open;
}

static int texture_is_settled(const char *name)
{
    const struct cached_texture *texture;

    texture = texture_cache_use_or_start_loading(name);
    return texture == NULL || texture->state != CACHED_TEXTURE_LOADING;
}

static int art_is_settled(void)
{
    int settled;

    settled = texture_is_settled(BACKDROP_NAME);
    settled = texture_is_settled(TITLE_LOGO_NAME) && settled;
    settled = texture_is_settled(TITLE_HUD_ART_NAME) && settled;
    settled = texture_is_settled(PAUSE_WIDGET_SHEET) && settled;
    settled = texture_is_settled(PAUSE_GLYPH_SHEET) && settled;
    return settled;
}

static void keep_art(void)
{
    texture_cache_keep_or_start_reading_ahead(BACKDROP_NAME);
    texture_cache_keep_or_start_reading_ahead(TITLE_LOGO_NAME);
    texture_cache_keep_or_start_reading_ahead(TITLE_HUD_ART_NAME);
    texture_cache_keep_or_start_reading_ahead(PAUSE_WIDGET_SHEET);
    texture_cache_keep_or_start_reading_ahead(PAUSE_GLYPH_SHEET);
}

static int nearest_in_direction(int from, float dx, float dy, int within_cone)
{
    int item;
    int best;
    float best_score;
    float along;
    float across;
    float score;

    best = -1;
    best_score = 0.0f;
    for (item = 0; item < TITLE_MENU_ITEM_COUNT; item++) {
        if (!shown[item] || item == from) {
            continue;
        }
        along = (places[item].x - places[from].x) * dx + (places[item].y - places[from].y) * dy;
        across = fabsf((places[item].x - places[from].x) * dy) + fabsf((places[item].y - places[from].y) * dx);
        if (along <= 0.0f || (within_cone && across > along * CONE_SLOPE)) {
            continue;
        }
        score = along + ACROSS_WEIGHT * across;
        if (best < 0 || score < best_score) {
            best = item;
            best_score = score;
        }
    }
    return best;
}

static int next_in_direction(int from, float dx, float dy)
{
    int next;

    next = nearest_in_direction(from, dx, dy, 1);
    if (next < 0) {
        next = nearest_in_direction(from, dx, dy, 0);
    }
    return next < 0 ? from : next;
}

static void steer(unsigned int pressed)
{
    if (pressed & INPUT_UP) {
        cursor = next_in_direction(cursor, 0.0f, -1.0f);
    } else if (pressed & INPUT_DOWN) {
        cursor = next_in_direction(cursor, 0.0f, 1.0f);
    } else if (pressed & INPUT_LEFT) {
        cursor = next_in_direction(cursor, -1.0f, 0.0f);
    } else if (pressed & INPUT_RIGHT) {
        cursor = next_in_direction(cursor, 1.0f, 0.0f);
    }
}

static void ask(int job, const char *line, const char *note)
{
    dialog_job = job;
    dialog.line_count = 1;
    dialog.lines[0] = line;
    dialog.note = note;
    dialog.option_count = 2;
    dialog.options[0] = "row.yes";
    dialog.options[1] = "row.no";
    pause_dialog_open(&dialog, 1);
}

static void leave_with(enum title_menu_request wanted, int chapter)
{
    request = wanted;
    request_chapter = chapter;
    run_log_write("title leave request=%d chapter=%d", (int)wanted, chapter);
    enter(TITLE_PHASE_HIDING);
}

static void erase_saves(void)
{
    if (erase_every_save != NULL) {
        (void)erase_every_save();
    }
    lay_out();
    run_log_write("title erased");
}

static void step_dialog(unsigned int pressed)
{
    int chosen;

    chosen = pause_dialog_steer(&dialog, pressed);
    if (chosen != 0) {
        return;
    }
    if (dialog_job == JOB_ERASE) {
        erase_saves();
    } else if (dialog_job == JOB_NEW_GAME) {
        leave_with(TITLE_REQUEST_NEW_GAME, 0);
    }
}

static void open_submenu(int item)
{
    picked = item;
    if (item == TITLE_ITEM_OPTIONS) {
        pause_options_enter();
    } else if (item == TITLE_ITEM_LANGUAGE) {
        pause_options_enter_on_language();
    } else if (item == TITLE_ITEM_CHAPTERS) {
        pause_chapters_enter(NULL);
    } else {
        pause_people_use_notes(pause_people_every_note());
        pause_people_use_reached_chapter(GAME_SAVE_CHAPTER_COUNT);
        pause_people_enter();
    }
    enter(TITLE_PHASE_PANNING_OUT);
}

static void pick(void)
{
    run_log_write("title pick %s", item_names[cursor]);
    switch (cursor) {
    case TITLE_ITEM_CONTINUE:
        leave_with(TITLE_REQUEST_CONTINUE, 0);
        break;
    case TITLE_ITEM_NEW_GAME:
        if (shown[TITLE_ITEM_CONTINUE]) {
            ask(JOB_NEW_GAME, "row.new_game_ask", NULL);
        } else {
            leave_with(TITLE_REQUEST_NEW_GAME, 0);
        }
        break;
    case TITLE_ITEM_ERASE:
        ask(JOB_ERASE, "row.reset_ask", "bar.reset_warning");
        break;
    default:
        open_submenu(cursor);
        break;
    }
}

static void step_showing(unsigned int pressed)
{
    if (dialog.is_open) {
        step_dialog(pressed);
        return;
    }
    if (pressed & INPUT_CROSS) {
        pick();
        return;
    }
    steer(pressed);
}

static int submenu_step(unsigned int pressed)
{
    int result;

    if (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE) {
        return pause_options_step(pressed) == PAUSE_OPTIONS_LEAVES;
    }
    if (picked == TITLE_ITEM_CHAPTERS) {
        result = pause_chapters_step(pressed);
        if (result == PAUSE_CHAPTERS_PLAYS) {
            leave_with(TITLE_REQUEST_CHAPTER, pause_chapters_selected());
        }
        return result == PAUSE_CHAPTERS_LEAVES;
    }
    return pause_people_step(pressed) == PAUSE_PEOPLE_LEAVES;
}

static void advance_phase(unsigned int pressed)
{
    switch (phase) {
    case TITLE_PHASE_WAITING_FOR_ART:
        if (phase_ticks >= BLACK_TICKS && art_is_settled()) {
            enter(TITLE_PHASE_SHOWING);
        }
        break;
    case TITLE_PHASE_SHOWING:
        step_showing(pressed);
        break;
    case TITLE_PHASE_PANNING_OUT:
        if (phase_ticks >= PAN_TICKS) {
            enter(TITLE_PHASE_SUBMENU);
        }
        break;
    case TITLE_PHASE_SUBMENU:
        if (submenu_step(pressed)) {
            enter(TITLE_PHASE_PANNING_BACK);
        }
        break;
    case TITLE_PHASE_PANNING_BACK:
        if (phase_ticks >= PAN_TICKS) {
            enter(TITLE_PHASE_SHOWING);
            phase_ticks = SHOW_FADE_TICKS;
        }
        break;
    case TITLE_PHASE_HIDING:
        if (phase_ticks >= HIDE_TICKS) {
            enter(TITLE_PHASE_LEAVING);
        }
        break;
    case TITLE_PHASE_LEAVING:
    case TITLE_PHASE_CLOSED:
        break;
    }
}

void title_menu_step(unsigned int pressed)
{
    if (phase == TITLE_PHASE_CLOSED) {
        return;
    }
    if ((phase == TITLE_PHASE_PANNING_OUT || phase == TITLE_PHASE_SUBMENU || phase == TITLE_PHASE_PANNING_BACK) && (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE)) {
        pause_options_tick();
    }
    phase_ticks = phase_ticks + 1;
    advance_phase(pressed);
}

static int item_under(float x, float y)
{
    int item;

    for (item = 0; item < TITLE_MENU_ITEM_COUNT; item++) {
        if (shown[item] && fabsf(x - places[item].x) <= places[item].width / 2.0f + TOUCH_MARGIN && fabsf(y - places[item].y) <= places[item].height / 2.0f + TOUCH_MARGIN) {
            return item;
        }
    }
    return -1;
}

static unsigned int touch_dialog(int lands, int lifts, float x, float y)
{
    unsigned int pressed;

    pressed = 0;
    if (lands) {
        touched_option = pause_dialog_option_at(&dialog, x, y);
        if (touched_option >= 0) {
            dialog.selected = touched_option;
        }
    } else if (lifts && touched_option >= 0 && pause_dialog_option_at(&dialog, x, y) == touched_option) {
        pressed = INPUT_CROSS;
    }
    if (lifts) {
        touched_option = PAUSE_DIALOG_NOTHING;
    }
    return pressed;
}

static unsigned int touch_menu(int lands, int lifts, int is_down, float x, float y)
{
    if (lands) {
        touched_item = item_under(x, y);
        if (touched_item >= 0) {
            cursor = touched_item;
        }
    } else if (is_down && touched_item >= 0 && item_under(x, y) != touched_item) {
        touched_item = -1;
    } else if (lifts && touched_item >= 0) {
        touched_item = -1;
        return INPUT_CROSS;
    }
    return 0;
}

static unsigned int touch_submenu(int lands, int lifts, int is_down, float x, float y)
{
    if (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE) {
        return pause_options_touch(lands, lifts, is_down, x, y);
    }
    if (picked == TITLE_ITEM_CHAPTERS) {
        return pause_chapters_touch(lands, lifts, x, y);
    }
    return pause_people_touch(lands, x, y);
}

static unsigned int touch_presses(const struct input_state *input)
{
    unsigned int pressed;
    int lands;
    int lifts;

    lands = input->touch_is_down && !touch_was_down;
    lifts = !input->touch_is_down && touch_was_down;
    pressed = 0;
    if (phase == TITLE_PHASE_SHOWING && dialog.is_open) {
        pressed = touch_dialog(lands, lifts, input->touch_x, input->touch_y);
    } else if (phase == TITLE_PHASE_SHOWING) {
        pressed = touch_menu(lands, lifts, input->touch_is_down, input->touch_x, input->touch_y);
    } else if (phase == TITLE_PHASE_SUBMENU) {
        pressed = touch_submenu(lands, lifts, input->touch_is_down, input->touch_x, input->touch_y);
    }
    if (lifts) {
        touched_item = -1;
    }
    touch_was_down = input->touch_is_down;
    return pressed;
}

void title_menu_update(const struct input_state *input, unsigned long long elapsed_us)
{
    unsigned long long steps;
    unsigned int pressed;

    if (phase == TITLE_PHASE_CLOSED) {
        return;
    }
    keep_art();
    steps = elapsed_us > PAUSE_TICK_US / 2ull ? (elapsed_us + PAUSE_TICK_US / 2ull) / PAUSE_TICK_US : 1ull;
    if (steps > PAUSE_TICKS_PER_UPDATE_MOST) {
        steps = PAUSE_TICKS_PER_UPDATE_MOST;
    }
    pressed = input->pressed | menu_input_stick_presses(&cursor_input, input) | touch_presses(input);
    pressed = pressed | menu_input_repeat_presses(&cursor_input, input, pressed, elapsed_us);
    title_menu_step(pressed);
    while (steps > 1ull) {
        title_menu_step(0);
        steps = steps - 1ull;
    }
}

enum title_menu_request title_menu_takes_request(int *chapter)
{
    enum title_menu_request taken;

    if (phase != TITLE_PHASE_LEAVING || request == TITLE_REQUEST_NONE) {
        return TITLE_REQUEST_NONE;
    }
    taken = request;
    if (chapter != NULL) {
        *chapter = request_chapter;
    }
    request = TITLE_REQUEST_NONE;
    phase = TITLE_PHASE_CLOSED;
    return taken;
}

float title_menu_camera_x(void)
{
    if (phase == TITLE_PHASE_PANNING_OUT) {
        return SCREEN_WIDTH * ease_snap((float)phase_ticks / (float)PAN_TICKS);
    }
    if (phase == TITLE_PHASE_SUBMENU) {
        return SCREEN_WIDTH;
    }
    if (phase == TITLE_PHASE_PANNING_BACK) {
        return SCREEN_WIDTH * (1.0f - ease_snap((float)phase_ticks / (float)PAN_TICKS));
    }
    return 0.0f;
}

static float menu_alpha(void)
{
    if (phase == TITLE_PHASE_PANNING_OUT) {
        return 1.0f - clamp01((float)phase_ticks / (float)LABEL_FADE_TICKS);
    }
    if (phase == TITLE_PHASE_SUBMENU) {
        return 0.0f;
    }
    if (phase == TITLE_PHASE_PANNING_BACK) {
        return clamp01((float)(phase_ticks - 1 - (PAN_TICKS - LABEL_FADE_TICKS)) / (float)LABEL_FADE_TICKS);
    }
    return 1.0f;
}

float title_menu_cover_alpha(void)
{
    switch (phase) {
    case TITLE_PHASE_WAITING_FOR_ART:
    case TITLE_PHASE_LEAVING:
        return 1.0f;
    case TITLE_PHASE_SHOWING:
        return 1.0f - clamp01((float)phase_ticks / (float)SHOW_FADE_TICKS);
    case TITLE_PHASE_HIDING:
        return clamp01((float)phase_ticks / (float)HIDE_TICKS);
    default:
        return 0.0f;
    }
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

static void draw_screen_texture(const char *name, float x, float y, float width, float alpha)
{
    const struct cached_texture *texture;
    float height;

    texture = texture_cache_use_or_start_loading(name);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY || texture->width == 0) {
        return;
    }
    height = width * (float)texture->height / (float)texture->width;
    pause_draw_from(name, 0.0f, 0.0f, (float)texture->width, (float)texture->height, whole(x), whole(y), whole(width), whole(height), alpha);
}

static void draw_logo(float camera_x, float alpha)
{
    const struct cached_texture *texture;
    float height;

    texture = texture_cache_use_or_start_loading(TITLE_LOGO_NAME);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY || texture->width == 0) {
        return;
    }
    height = LOGO_WIDTH * (float)texture->height / (float)texture->width;
    draw_screen_texture(TITLE_LOGO_NAME, CENTRE_X - LOGO_WIDTH / 2.0f - camera_x, LOGO_CENTRE_Y - height / 2.0f, LOGO_WIDTH, alpha);
}

static void draw_item(int item, float camera_x, float alpha)
{
    const struct title_place *spot;
    const char *style;
    float x;
    int is_icon;

    spot = &places[item];
    is_icon = item == TITLE_ITEM_ERASE || item == TITLE_ITEM_RECORD;
    if (is_icon) {
        style = item == cursor ? "title_icon_selected" : "title_icon";
    } else {
        style = item == cursor ? "title_button_selected" : "title_button";
    }
    x = spot->x - camera_x;
    pause_draw_slice(style, x - spot->width / 2.0f, spot->y - spot->height / 2.0f, spot->width, spot->height, alpha);
    if (item == TITLE_ITEM_ERASE) {
        pause_draw_cell("trash", x, spot->y, alpha);
    } else if (item == TITLE_ITEM_RECORD) {
        pause_draw_from(TITLE_HUD_ART_NAME, NOTEBOOK_U, NOTEBOOK_V, NOTEBOOK_TEXELS, NOTEBOOK_TEXELS, whole(x - NOTEBOOK_SIZE / 2.0f), whole(spot->y - NOTEBOOK_SIZE / 2.0f), NOTEBOOK_SIZE, NOTEBOOK_SIZE, alpha);
    } else {
        pause_draw_text_within(label_keys[item], x - pause_text_width(label_keys[item]) * pause_fit_scale(label_keys[item], LABEL_ROOM) / 2.0f, spot->y, LABEL_ROOM, alpha);
    }
}

static void draw_submenu(float side_x)
{
    if (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE) {
        pause_options_draw(side_x);
    } else if (picked == TITLE_ITEM_CHAPTERS) {
        pause_chapters_draw(side_x);
    } else {
        pause_people_draw(side_x);
    }
}

static void draw_submenu_bar(float side_x)
{
    if (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE) {
        pause_options_draw_bar(side_x);
    } else if (picked == TITLE_ITEM_CHAPTERS) {
        pause_chapters_draw_bar(side_x);
    } else {
        pause_people_draw_bar(side_x);
    }
}

static void draw_submenu_dialog(float side_x)
{
    if (picked == TITLE_ITEM_OPTIONS || picked == TITLE_ITEM_LANGUAGE) {
        pause_options_draw_dialog(side_x);
    } else if (picked == TITLE_ITEM_CHAPTERS) {
        pause_chapters_draw_dialog(side_x);
    } else {
        pause_people_draw_dialog(side_x);
    }
}

static void draw_cover(float alpha)
{
    struct renderer_color color;
    float shade;

    if (alpha <= 0.0f) {
        return;
    }
    shade = phase == TITLE_PHASE_HIDING || phase == TITLE_PHASE_LEAVING ? 1.0f : 0.0f;
    color.red = shade;
    color.green = shade;
    color.blue = shade;
    color.alpha = alpha;
    renderer_draw_filled_rect(rect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT), color);
}

void title_menu_draw(void)
{
    float camera_x;
    float side_x;
    float alpha;
    int item;

    if (phase == TITLE_PHASE_CLOSED) {
        return;
    }
    if (phase != TITLE_PHASE_WAITING_FOR_ART && phase != TITLE_PHASE_LEAVING) {
        camera_x = whole(title_menu_camera_x());
        side_x = SCREEN_WIDTH - camera_x;
        alpha = menu_alpha();
        draw_screen_texture(BACKDROP_NAME, -camera_x, 0.0f, SCREEN_WIDTH, 1.0f);
        if (camera_x != 0.0f) {
            draw_screen_texture(BACKDROP_NAME, side_x, 0.0f, SCREEN_WIDTH, 1.0f);
            draw_submenu(side_x);
        }
        draw_logo(camera_x, alpha);
        for (item = 0; item < TITLE_MENU_ITEM_COUNT; item++) {
            if (shown[item]) {
                draw_item(item, camera_x, alpha);
            }
        }
        pause_draw_floor(1.0f);
        if (camera_x != 0.0f) {
            draw_submenu_bar(side_x);
        } else {
            pause_draw_bar_note(0.0f, bar_keys[cursor]);
        }
        if (phase == TITLE_PHASE_SHOWING) {
            pause_dialog_draw(&dialog, 0.0f);
        }
        if (camera_x != 0.0f) {
            draw_submenu_dialog(side_x);
        }
    }
    draw_cover(title_menu_cover_alpha());
}
