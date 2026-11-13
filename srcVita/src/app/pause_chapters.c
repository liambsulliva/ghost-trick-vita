#include <math.h>
#include <stdio.h>
#include "input.h"
#include "pause_chapters.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"

#define COLUMNS 3
#define CELL_LEFT 60.0f
#define CELL_STEP_X 290.0f
#define CELL_WIDTH 260.0f
#define CELL_TOP 110.0f
#define CELL_STEP_Y 62.0f
#define CELL_HEIGHT 56.0f
#define TITLE_Y 40.0f
#define LOCKED_ALPHA 0.42f

static pause_chapter_reached_function checker;
static int reached[PAUSE_CHAPTERS_MOST + 1];
static int selected;
static struct pause_dialog dialog;
static int touched_option;

void pause_chapters_use(pause_chapter_reached_function chapter_is_reached)
{
    checker = chapter_is_reached;
}

void pause_chapters_enter(void)
{
    int chapter;

    selected = 1;
    for (chapter = 1; chapter <= PAUSE_CHAPTERS_MOST; chapter++) {
        reached[chapter] = checker != NULL && checker(chapter);
        if (reached[chapter]) {
            selected = chapter;
        }
    }
    dialog.is_open = 0;
    touched_option = PAUSE_DIALOG_NOTHING;
}

static void move_to(int chapter)
{
    if (chapter >= 1 && chapter <= PAUSE_CHAPTERS_MOST) {
        selected = chapter;
    }
}

static void ask(void)
{
    dialog.line_count = 1;
    dialog.lines[0] = "row.chapters_ask";
    dialog.note = "bar.title_warning";
    dialog.option_count = 2;
    dialog.options[0] = "row.yes";
    dialog.options[1] = "row.no";
    pause_dialog_open(&dialog, 1);
}

int pause_chapters_step(unsigned int pressed)
{
    int chosen;

    if (dialog.is_open) {
        chosen = pause_dialog_steer(&dialog, pressed);
        if (chosen == 0) {
            run_log_write("pause chapter_replay chapter=%d", selected);
            return PAUSE_CHAPTERS_PLAYS;
        }
        return PAUSE_CHAPTERS_STAYS;
    }
    if (pressed & INPUT_CIRCLE) {
        return PAUSE_CHAPTERS_LEAVES;
    }
    if (pressed & INPUT_LEFT) {
        if ((selected - 1) % COLUMNS != 0) {
            move_to(selected - 1);
        }
    } else if (pressed & INPUT_RIGHT) {
        if (selected % COLUMNS != 0) {
            move_to(selected + 1);
        }
    } else if (pressed & INPUT_UP) {
        move_to(selected - COLUMNS);
    } else if (pressed & INPUT_DOWN) {
        move_to(selected + COLUMNS);
    } else if ((pressed & INPUT_CROSS) && reached[selected]) {
        ask();
    }
    return PAUSE_CHAPTERS_STAYS;
}

static float cell_left(int chapter)
{
    return CELL_LEFT + (float)((chapter - 1) % COLUMNS) * CELL_STEP_X;
}

static float cell_top(int chapter)
{
    return CELL_TOP + (float)((chapter - 1) / COLUMNS) * CELL_STEP_Y;
}

static int chapter_at(float x, float y)
{
    int chapter;

    for (chapter = 1; chapter <= PAUSE_CHAPTERS_MOST; chapter++) {
        if (x >= cell_left(chapter) && x < cell_left(chapter) + CELL_WIDTH && y >= cell_top(chapter) && y < cell_top(chapter) + CELL_HEIGHT) {
            return chapter;
        }
    }
    return 0;
}

unsigned int pause_chapters_touch(int lands, int lifts, float x, float y)
{
    int chapter;

    if (dialog.is_open) {
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
    if (!lands) {
        return 0;
    }
    if (pause_is_on_back_pill(x, y)) {
        return INPUT_CIRCLE;
    }
    chapter = chapter_at(x, y);
    if (chapter == 0) {
        return 0;
    }
    if (chapter == selected) {
        return INPUT_CROSS;
    }
    selected = chapter;
    return 0;
}

int pause_chapters_selected(void)
{
    return selected;
}

int pause_chapters_dialog_is_open(void)
{
    return dialog.is_open;
}

void pause_chapters_draw(float x_offset)
{
    char key[PAUSE_WIDGET_KEY_MAX];
    int chapter;
    float alpha;

    x_offset = floorf(x_offset + 0.5f);
    pause_draw_text("row.chapters_title", (float)RENDERER_WIDTH / 2.0f + x_offset, TITLE_Y, PAUSE_ANCHOR_CENTRE, 1.0f);
    for (chapter = 1; chapter <= PAUSE_CHAPTERS_MOST; chapter++) {
        alpha = reached[chapter] ? 1.0f : LOCKED_ALPHA;
        snprintf(key, sizeof(key), "row.chapter_%d", chapter);
        pause_draw_slice(chapter == selected ? "button_selected" : "button", cell_left(chapter) + x_offset, cell_top(chapter), CELL_WIDTH, CELL_HEIGHT, chapter == selected ? 1.0f : alpha);
        pause_draw_text(key, cell_left(chapter) + CELL_WIDTH / 2.0f + x_offset, cell_top(chapter) + CELL_HEIGHT / 2.0f, PAUSE_ANCHOR_CENTRE, alpha);
    }
}

void pause_chapters_draw_bar(float x_offset)
{
    pause_draw_bar(x_offset, reached[selected] ? "bar.chapters" : "bar.chapter_locked", NULL, NULL);
}

void pause_chapters_draw_dialog(float x_offset)
{
    pause_dialog_draw(&dialog, floorf(x_offset + 0.5f));
}
