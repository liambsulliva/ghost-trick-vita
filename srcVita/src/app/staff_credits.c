#include <string.h>
#include "ds_units.h"
#include "run_log.h"
#include "staff_credits.h"
#include "text_lines.h"

#define CREDITS_LINE_MAX 256

void staff_credits_clear(struct staff_credits *credits)
{
    memset(credits, 0, sizeof(*credits));
    credits->brightness = -NATIVE_BRIGHTNESS_SCALE;
}

static void keep_page(struct staff_credits *credits, const struct script_staff_page_row *row)
{
    int index;
    int place;

    if (row->page < 0 || row->page >= SCRIPT_STAFF_PAGE_MAX) {
        return;
    }
    for (index = 0; index < credits->page_count; index++) {
        if (credits->pages[index].page == row->page) {
            credits->pages[index] = *row;
            return;
        }
    }
    if (credits->page_count >= SCRIPT_STAFF_PAGE_MAX) {
        return;
    }
    place = credits->page_count;
    while (place > 0 && credits->pages[place - 1].page > row->page) {
        credits->pages[place] = credits->pages[place - 1];
        place = place - 1;
    }
    credits->pages[place] = *row;
    credits->page_count = credits->page_count + 1;
}

int staff_credits_read(struct staff_credits *credits, const char *text, unsigned int text_size)
{
    char line[CREDITS_LINE_MAX];
    struct script_staff_page_row row;
    unsigned int offset;

    staff_credits_clear(credits);
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (script_staff_page_row_read(line, &row)) {
            keep_page(credits, &row);
        } else {
            (void)script_staff_music_row_read(line, credits->music_name, sizeof(credits->music_name));
        }
    }
    return credits->page_count;
}

int staff_credits_can_play(const struct staff_credits *credits)
{
    return credits->page_count > 0;
}

static void open_the_page(struct staff_credits *credits)
{
    credits->step = STAFF_CREDITS_LOADING;
    credits->brightness = -NATIVE_BRIGHTNESS_SCALE;
    memset(&credits->fade, 0, sizeof(credits->fade));
}

void staff_credits_start(struct staff_credits *credits)
{
    int index;

    if (!staff_credits_can_play(credits)) {
        return;
    }
    memset(&credits->roll, 0, sizeof(credits->roll));
    for (index = 0; index < credits->page_count; index++) {
        script_staff_roll_set_screens(&credits->roll, credits->pages[index].page, credits->pages[index].screen_count);
    }
    music_remember(&credits->music_before);
    if (credits->music_name[0] != '\0') {
        music_request_play(credits->music_name, 1);
    }
    credits->page_index = 0;
    open_the_page(credits);
    run_log_write("credits start pages=%d music=%s", credits->page_count, credits->music_name[0] != '\0' ? credits->music_name : "-");
}

static void finish(struct staff_credits *credits, const char *reason)
{
    const struct script_staff_page_row *row;

    row = staff_credits_page(credits);
    credits->step = STAFF_CREDITS_OFF;
    credits->roll.is_on = 0;
    music_restore(&credits->music_before, row != NULL ? row->fade_out_ticks : 0);
    run_log_write("credits end reason=%s page=%d", reason, row != NULL ? row->page : -1);
}

static void fade(struct staff_credits *credits)
{
    credits->brightness = script_fade_step(&credits->fade);
}

int staff_credits_step(struct staff_credits *credits, int page_is_ready)
{
    const struct script_staff_page_row *row;

    row = staff_credits_page(credits);
    if (row == NULL) {
        return 0;
    }
    if (credits->step == STAFF_CREDITS_LOADING) {
        if (!page_is_ready) {
            return 0;
        }
        script_staff_roll_start(&credits->roll, row->page);
        script_fade_start(&credits->fade, SCRIPT_FADE_IN_KIND, row->fade_in_ticks, SCRIPT_FADE_FULL_SCALE);
        credits->step = STAFF_CREDITS_FADING_IN;
    }
    fade(credits);
    (void)script_staff_roll_step(&credits->roll);
    switch (credits->step) {
    case STAFF_CREDITS_FADING_IN:
        if (credits->fade.level == 0) {
            credits->step = STAFF_CREDITS_SHOWING;
        }
        break;
    case STAFF_CREDITS_SHOWING:
        if (!script_staff_roll_holds_the_script(&credits->roll)) {
            credits->step = STAFF_CREDITS_HOLDING;
            credits->hold_ticks_left = row->hold_ticks;
        }
        break;
    case STAFF_CREDITS_HOLDING:
        if (credits->hold_ticks_left > 0) {
            credits->hold_ticks_left = credits->hold_ticks_left - 1;
            break;
        }
        script_fade_start(&credits->fade, SCRIPT_FADE_OUT_KIND, row->fade_out_ticks, SCRIPT_FADE_FULL_SCALE);
        credits->step = STAFF_CREDITS_FADING_OUT;
        break;
    case STAFF_CREDITS_FADING_OUT:
        if (credits->fade.level == 0) {
            credits->brightness = script_fade_brightness(&credits->fade);
            script_staff_roll_end(&credits->roll);
            credits->step = STAFF_CREDITS_CLOSING;
        }
        break;
    case STAFF_CREDITS_CLOSING:
        if (!credits->roll.is_on) {
            if (credits->page_index + 1 >= credits->page_count) {
                finish(credits, "done");
                return 1;
            }
            credits->page_index = credits->page_index + 1;
            open_the_page(credits);
        }
        break;
    default:
        break;
    }
    return 0;
}

void staff_credits_stop(struct staff_credits *credits)
{
    if (staff_credits_is_playing(credits)) {
        finish(credits, "left");
    }
}

int staff_credits_is_playing(const struct staff_credits *credits)
{
    return credits->step != STAFF_CREDITS_OFF;
}

const struct script_staff_page_row *staff_credits_page(const struct staff_credits *credits)
{
    if (!staff_credits_is_playing(credits) || credits->page_index >= credits->page_count) {
        return NULL;
    }
    return &credits->pages[credits->page_index];
}

int staff_credits_page_is_drawn(const struct staff_credits *credits)
{
    return staff_credits_is_playing(credits) && script_staff_roll_is_drawn(&credits->roll);
}

float staff_credits_scroll(const struct staff_credits *credits)
{
    return script_staff_roll_scroll(&credits->roll);
}

float staff_credits_black(const struct staff_credits *credits)
{
    return credits->brightness < 0 ? (float)-credits->brightness / (float)NATIVE_BRIGHTNESS_SCALE : 0.0f;
}
