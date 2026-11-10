#ifndef SCRIPT_STAFF_ROLL_H
#define SCRIPT_STAFF_ROLL_H

#define SCRIPT_STAFF_PAGE_MAX 16
#define SCRIPT_STAFF_SCREEN_HEIGHT 192

enum script_staff_roll_state {
    SCRIPT_STAFF_ROLL_OPENING = 0,
    SCRIPT_STAFF_ROLL_STARTS_THE_PAGE = 1,
    SCRIPT_STAFF_ROLL_SHOWS_THE_PAGE = 2,
    SCRIPT_STAFF_ROLL_CLOSING = 3,
    SCRIPT_STAFF_ROLL_LEAVING = 4,
    SCRIPT_STAFF_ROLL_PAGE_IS_OVER = 5
};

enum script_staff_page_state {
    SCRIPT_STAFF_PAGE_NOT_STARTED = 0,
    SCRIPT_STAFF_PAGE_RESTS = 1,
    SCRIPT_STAFF_PAGE_SCROLLS = 2,
    SCRIPT_STAFF_PAGE_IS_DONE = 3
};

struct script_staff_roll {
    int screens_of_pages[SCRIPT_STAFF_PAGE_MAX];
    int is_on;
    int page;
    int screen_count;
    enum script_staff_roll_state state;
    int wait_ticks;
    enum script_staff_page_state page_state;
    int page_wait_ticks;
    int scroll_fixed;
    int screens_passed;
    int scroll_limit;
};

struct script_staff_screen_span {
    float top;
    float first_row;
    float row_count;
};

void script_staff_roll_set_screens(struct script_staff_roll *roll, int page, int screen_count);
void script_staff_roll_start(struct script_staff_roll *roll, int page);
int script_staff_roll_step(struct script_staff_roll *roll);
void script_staff_roll_end(struct script_staff_roll *roll);
int script_staff_roll_holds_the_script(const struct script_staff_roll *roll);
int script_staff_roll_is_drawn(const struct script_staff_roll *roll);
float script_staff_roll_scroll(const struct script_staff_roll *roll);
int script_staff_roll_screen_span(float scroll, int screen, struct script_staff_screen_span *span);

#endif
