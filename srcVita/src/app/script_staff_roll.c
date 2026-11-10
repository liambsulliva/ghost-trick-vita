#include "script_staff_roll.h"

#define TICKS_BEFORE_THE_PAGE_STARTS 0x3c
#define TICKS_THE_FIRST_SCREEN_RESTS 0x8c
#define TICKS_EACH_SCREEN_RESTS 0x78
#define SCROLL_PER_TICK 0x1000
#define FIXED_POINT_BITS 12

void script_staff_roll_set_screens(struct script_staff_roll *roll, int page, int screen_count)
{
    if (page >= 0 && page < SCRIPT_STAFF_PAGE_MAX && screen_count > 0) {
        roll->screens_of_pages[page] = screen_count;
    }
}

void script_staff_roll_start(struct script_staff_roll *roll, int page)
{
    roll->is_on = 1;
    roll->page = page;
    roll->screen_count = page >= 0 && page < SCRIPT_STAFF_PAGE_MAX && roll->screens_of_pages[page] > 0 ? roll->screens_of_pages[page] : 1;
    roll->state = SCRIPT_STAFF_ROLL_OPENING;
    roll->wait_ticks = 0;
    roll->page_state = SCRIPT_STAFF_PAGE_NOT_STARTED;
    roll->page_wait_ticks = TICKS_THE_FIRST_SCREEN_RESTS;
    roll->scroll_fixed = 0;
    roll->screens_passed = 0;
    roll->scroll_limit = (roll->screen_count - 1) * SCRIPT_STAFF_SCREEN_HEIGHT;
}

static int scroll_in_pixels(const struct script_staff_roll *roll)
{
    return roll->scroll_fixed >> FIXED_POINT_BITS;
}

static void scroll_one_tick(struct script_staff_roll *roll)
{
    int next_screen_top;

    roll->scroll_fixed = roll->scroll_fixed + SCROLL_PER_TICK;
    next_screen_top = ((roll->screens_passed + 1) * SCRIPT_STAFF_SCREEN_HEIGHT) << FIXED_POINT_BITS;
    if (roll->scroll_fixed > next_screen_top) {
        roll->scroll_fixed = next_screen_top;
        roll->screens_passed = roll->screens_passed + 1;
    }
    if (roll->scroll_fixed > roll->scroll_limit << FIXED_POINT_BITS) {
        roll->scroll_fixed = roll->scroll_limit << FIXED_POINT_BITS;
    }
}

static void step_the_page(struct script_staff_roll *roll)
{
    if (roll->page_wait_ticks != 0) {
        roll->page_wait_ticks = roll->page_wait_ticks - 1;
        return;
    }
    switch (roll->page_state) {
    case SCRIPT_STAFF_PAGE_RESTS:
        roll->page_state = scroll_in_pixels(roll) < roll->scroll_limit ? SCRIPT_STAFF_PAGE_SCROLLS : SCRIPT_STAFF_PAGE_IS_DONE;
        break;
    case SCRIPT_STAFF_PAGE_SCROLLS:
        scroll_one_tick(roll);
        if (scroll_in_pixels(roll) == roll->screens_passed * SCRIPT_STAFF_SCREEN_HEIGHT) {
            roll->page_state = SCRIPT_STAFF_PAGE_RESTS;
            roll->page_wait_ticks = TICKS_EACH_SCREEN_RESTS;
        }
        break;
    default:
        break;
    }
}

static int step_the_roll(struct script_staff_roll *roll)
{
    if (roll->wait_ticks != 0) {
        roll->wait_ticks = roll->wait_ticks - 1;
        return 0;
    }
    switch (roll->state) {
    case SCRIPT_STAFF_ROLL_OPENING:
        roll->state = SCRIPT_STAFF_ROLL_STARTS_THE_PAGE;
        roll->wait_ticks = TICKS_BEFORE_THE_PAGE_STARTS;
        return 0;
    case SCRIPT_STAFF_ROLL_STARTS_THE_PAGE:
        roll->page_state = SCRIPT_STAFF_PAGE_SCROLLS;
        roll->state = SCRIPT_STAFF_ROLL_SHOWS_THE_PAGE;
        return 0;
    case SCRIPT_STAFF_ROLL_SHOWS_THE_PAGE:
        if (roll->page_state == SCRIPT_STAFF_PAGE_IS_DONE) {
            roll->state = SCRIPT_STAFF_ROLL_PAGE_IS_OVER;
        }
        return 0;
    case SCRIPT_STAFF_ROLL_CLOSING:
        roll->state = SCRIPT_STAFF_ROLL_LEAVING;
        return 0;
    case SCRIPT_STAFF_ROLL_LEAVING:
        roll->is_on = 0;
        return 1;
    default:
        return 0;
    }
}

int script_staff_roll_step(struct script_staff_roll *roll)
{
    int has_left;

    if (!roll->is_on) {
        return 0;
    }
    has_left = step_the_roll(roll);
    if (roll->is_on && script_staff_roll_is_drawn(roll)) {
        step_the_page(roll);
    }
    return has_left;
}

void script_staff_roll_end(struct script_staff_roll *roll)
{
    if (roll->is_on) {
        roll->state = SCRIPT_STAFF_ROLL_CLOSING;
        roll->wait_ticks = 0;
    }
}

int script_staff_roll_holds_the_script(const struct script_staff_roll *roll)
{
    return roll->is_on && roll->state != SCRIPT_STAFF_ROLL_PAGE_IS_OVER;
}

int script_staff_roll_is_drawn(const struct script_staff_roll *roll)
{
    return roll->is_on && roll->state != SCRIPT_STAFF_ROLL_LEAVING;
}

float script_staff_roll_scroll(const struct script_staff_roll *roll)
{
    return (float)roll->scroll_fixed / (float)(1 << FIXED_POINT_BITS);
}

int script_staff_roll_screen_span(float scroll, int screen, struct script_staff_screen_span *span)
{
    float top;
    float bottom;

    top = (float)(screen * SCRIPT_STAFF_SCREEN_HEIGHT) - scroll;
    bottom = top + (float)SCRIPT_STAFF_SCREEN_HEIGHT;
    if (bottom <= 0.0f || top >= (float)SCRIPT_STAFF_SCREEN_HEIGHT) {
        return 0;
    }
    span->top = top > 0.0f ? top : 0.0f;
    span->first_row = top < 0.0f ? -top : 0.0f;
    span->row_count = (bottom < (float)SCRIPT_STAFF_SCREEN_HEIGHT ? bottom : (float)SCRIPT_STAFF_SCREEN_HEIGHT) - span->top;
    return 1;
}
