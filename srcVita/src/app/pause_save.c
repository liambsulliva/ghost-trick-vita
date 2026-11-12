#include <math.h>
#include <stddef.h>
#include "input.h"
#include "pause_save.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"

#define PANEL_LEFT 40.0f
#define PANEL_TOP 84.0f
#define PANEL_STEP 200.0f
#define PANEL_HEIGHT 180.0f
#define PANEL_TEXT_X 64.0f
#define LABEL_DROP 30.0f
#define STATE_DROP 100.0f
#define TITLE_Y 40.0f

enum save_dialog {
    DIALOG_NONE,
    DIALOG_ASK,
    DIALOG_DONE,
    DIALOG_FAILED
};

static pause_save_now_function saver;
static pause_slot_has_data_function slot_checker;
static int selected;
static enum save_dialog showing;
static struct pause_dialog dialog;
static int touched_option;
static int has_data[2];

static void check_slots(void)
{
    int slot;

    for (slot = 0; slot < 2; slot++) {
        has_data[slot] = slot_checker != NULL && slot_checker(slot);
    }
}

void pause_save_use(pause_save_now_function save_now, pause_slot_has_data_function slot_has_data)
{
    saver = save_now;
    slot_checker = slot_has_data;
}

int pause_save_now(void)
{
    return saver != NULL && saver();
}

static void show(enum save_dialog which)
{
    showing = which;
    dialog.note = NULL;
    dialog.option_count = 2;
    dialog.options[0] = "row.yes";
    dialog.options[1] = "row.no";
    dialog.line_count = 1;
    if (which == DIALOG_ASK) {
        dialog.lines[0] = "row.save_ask";
        dialog.note = "bar.save_ask_note";
    } else if (which == DIALOG_DONE) {
        dialog.lines[0] = "row.save_done";
    } else {
        dialog.lines[0] = "row.save_failed";
        dialog.option_count = 1;
        dialog.options[0] = "row.ok";
    }
    pause_dialog_open(&dialog, 0);
}

void pause_save_enter(void)
{
    selected = PAUSE_SAVE_MANUAL_PANEL;
    showing = DIALOG_NONE;
    dialog.is_open = 0;
    touched_option = PAUSE_DIALOG_NOTHING;
    check_slots();
}

static int step_dialog(unsigned int pressed)
{
    enum save_dialog was;
    int chosen;

    was = showing;
    chosen = pause_dialog_steer(&dialog, pressed);
    if (chosen == PAUSE_DIALOG_NOTHING) {
        return PAUSE_SAVE_STAYS;
    }
    showing = DIALOG_NONE;
    if (was == DIALOG_ASK && chosen == 0) {
        show(pause_save_now() ? DIALOG_DONE : DIALOG_FAILED);
        check_slots();
    } else if (was == DIALOG_DONE && chosen == 0) {
        return PAUSE_SAVE_RESUMES;
    }
    return PAUSE_SAVE_STAYS;
}

int pause_save_step(unsigned int pressed)
{
    if (showing != DIALOG_NONE) {
        return step_dialog(pressed);
    }
    if (pressed & INPUT_CIRCLE) {
        return PAUSE_SAVE_LEAVES;
    }
    if (pressed & INPUT_UP) {
        selected = PAUSE_SAVE_AUTO_PANEL;
    } else if (pressed & INPUT_DOWN) {
        selected = PAUSE_SAVE_MANUAL_PANEL;
    } else if ((pressed & INPUT_CROSS) && selected == PAUSE_SAVE_MANUAL_PANEL) {
        show(DIALOG_ASK);
    }
    return PAUSE_SAVE_STAYS;
}

static float panel_top(int panel)
{
    return PANEL_TOP + (float)panel * PANEL_STEP;
}

static int panel_at(float x, float y)
{
    int panel;

    for (panel = 0; panel < 2; panel++) {
        if (x >= PANEL_LEFT && x < (float)RENDERER_WIDTH - PANEL_LEFT && y >= panel_top(panel) && y < panel_top(panel) + PANEL_HEIGHT) {
            return panel;
        }
    }
    return -1;
}

unsigned int pause_save_touch(int lands, int lifts, float x, float y)
{
    int panel;

    if (lands && pause_is_on_back_pill(x, y) && showing == DIALOG_NONE) {
        return INPUT_CIRCLE;
    }
    if (showing != DIALOG_NONE) {
        if (lands) {
            touched_option = pause_dialog_option_at(&dialog, x, y);
            if (touched_option >= 0) {
                dialog.selected = touched_option;
            }
        } else if (lifts && touched_option >= 0 && pause_dialog_option_at(&dialog, x, y) == touched_option) {
            touched_option = PAUSE_DIALOG_NOTHING;
            return INPUT_CROSS;
        }
        return 0;
    }
    if (lands) {
        panel = panel_at(x, y);
        if (panel >= 0 && panel == selected) {
            return INPUT_CROSS;
        }
        if (panel >= 0) {
            selected = panel;
        }
    }
    return 0;
}

int pause_save_selected(void)
{
    return selected;
}

int pause_save_dialog_is_open(void)
{
    return showing != DIALOG_NONE;
}

void pause_save_draw(float x_offset)
{
    int panel;
    float top;
    float width;

    x_offset = floorf(x_offset + 0.5f);
    width = (float)RENDERER_WIDTH - 2.0f * PANEL_LEFT;
    pause_draw_text("row.save_data", (float)RENDERER_WIDTH / 2.0f + x_offset, TITLE_Y, PAUSE_ANCHOR_CENTRE, 1.0f);
    for (panel = 0; panel < 2; panel++) {
        top = panel_top(panel);
        pause_draw_slice(panel == selected ? "save_panel_selected" : "save_panel", PANEL_LEFT + x_offset, top, width, PANEL_HEIGHT, 1.0f);
        pause_draw_text(panel == PAUSE_SAVE_AUTO_PANEL ? "header.autosave" : "header.manual_save", PANEL_TEXT_X + x_offset, top + LABEL_DROP, PAUSE_ANCHOR_LEFT, 1.0f);
        pause_draw_text(has_data[panel] ? "row.saved_game" : "row.no_data", PANEL_TEXT_X + x_offset, top + STATE_DROP, PAUSE_ANCHOR_LEFT, 1.0f);
    }
}

void pause_save_draw_bar(float x_offset)
{
    pause_draw_bar(x_offset, selected == PAUSE_SAVE_AUTO_PANEL ? "bar.save_auto" : "bar.save_manual", NULL, NULL);
}

void pause_save_draw_dialog(float x_offset)
{
    pause_dialog_draw(&dialog, floorf(x_offset + 0.5f));
}
