#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_machine.h"
#include "text_lines.h"

#define TALK_LINE_TEXT_MAX 2048
#define OPEN_DELAY_TICKS 5
#define SLIDE_TICKS 9
#define FADE_IN_TICKS 30
#define LAST_PRESS_RELEASES_AFTER_TICKS 13
#define SOUND_KIND_PAGE_PRESS 5
#define SOUND_KIND_FACE_SLIDE 6
#define FACE_SOUND_AFTER_TICKS 12
#define NO_FACE "-"

static int read_talk_row(const char *line, struct script_talk_book *book)
{
    struct script_talk_message *message;

    if (book->message_count >= SCRIPT_TALK_MESSAGE_MAX) {
        return 0;
    }
    message = &book->messages[book->message_count];
    if (sscanf(line, "talk %31s", message->label) != 1) {
        return 0;
    }
    message->first_page = book->page_count;
    message->page_count = 0;
    book->message_count = book->message_count + 1;
    return 1;
}

static int read_page_row(const char *line, struct script_talk_book *book)
{
    struct script_talk_page *page;
    int ending;
    int line_count;

    if (book->message_count == 0 || book->page_count >= SCRIPT_TALK_PAGE_MAX) {
        return 0;
    }
    page = &book->pages[book->page_count];
    if (sscanf(line, "page " ASSET_NAME_SCAN " %d %d %d %d %d", page->face_texture, &page->face_is_flipped, &page->is_top_box, &page->reveal_is_fade, &ending, &line_count) != 6) {
        return 0;
    }
    page->has_face = strcmp(page->face_texture, NO_FACE) != 0;
    page->ending = (enum script_talk_ending)ending;
    page->first_line = book->line_count;
    page->line_count = 0;
    page->glyph_count = 0;
    book->page_count = book->page_count + 1;
    book->messages[book->message_count - 1].page_count = book->messages[book->message_count - 1].page_count + 1;
    return 1;
}

static int read_font_row(const char *line, struct script_talk_book *book)
{
    return sscanf(line, "font " ASSET_NAME_SCAN, book->font_texture) == 1;
}

static int read_glyph_row(const char *line, struct script_talk_book *book)
{
    struct script_talk_glyph *glyph;
    int atlas_x;
    int atlas_y;
    int width;
    int height;
    int left;
    int top;
    int advance;

    if (book->glyph_count >= SCRIPT_TALK_GLYPH_MAX) {
        return 0;
    }
    if (sscanf(line, "glyph %d %d %d %d %d %d %d", &atlas_x, &atlas_y, &width, &height, &left, &top, &advance) != 7) {
        return 0;
    }
    glyph = &book->glyphs[book->glyph_count];
    glyph->atlas_x = (short)atlas_x;
    glyph->atlas_y = (short)atlas_y;
    glyph->width = (short)width;
    glyph->height = (short)height;
    glyph->left = (short)left;
    glyph->top = (short)top;
    glyph->advance = (short)advance;
    book->glyph_count = book->glyph_count + 1;
    return 1;
}

static int read_line_row(const char *line, struct script_talk_book *book)
{
    struct script_talk_line *talk_line;
    struct script_talk_page *page;
    const char *rest;
    char *after;
    int consumed;
    int x;
    int y;
    int glyph_count;
    int index;

    if (book->page_count == 0 || book->line_count >= SCRIPT_TALK_LINE_MAX) {
        return 0;
    }
    if (sscanf(line, "line %d %d %d%n", &x, &y, &glyph_count, &consumed) != 3) {
        return 0;
    }
    if (glyph_count < 0 || book->text_count + glyph_count > SCRIPT_TALK_TEXT_MAX) {
        return 0;
    }
    talk_line = &book->lines[book->line_count];
    talk_line->x = (short)x;
    talk_line->y = (short)y;
    talk_line->glyph_count = (unsigned short)glyph_count;
    talk_line->first_text = book->text_count;
    rest = line + consumed;
    for (index = 0; index < glyph_count; index++) {
        book->text[book->text_count + index] = (unsigned short)strtol(rest, &after, 10);
        if (after == rest) {
            return 0;
        }
        rest = after;
    }
    book->text_count = book->text_count + glyph_count;
    page = &book->pages[book->page_count - 1];
    page->line_count = page->line_count + 1;
    page->glyph_count = page->glyph_count + glyph_count;
    book->line_count = book->line_count + 1;
    return 1;
}

int script_talk_book_parse(const char *text, unsigned int text_size, struct script_talk_book *book)
{
    char line[TALK_LINE_TEXT_MAX];
    unsigned int offset;

    memset(book, 0, sizeof(*book));
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (text_line_starts_with(line, "glyph ")) {
            if (!read_glyph_row(line, book)) {
                return 0;
            }
        } else if (text_line_starts_with(line, "font ")) {
            if (!read_font_row(line, book)) {
                return 0;
            }
        } else if (text_line_starts_with(line, "talk ")) {
            if (!read_talk_row(line, book)) {
                return 0;
            }
        } else if (text_line_starts_with(line, "page ")) {
            if (!read_page_row(line, book)) {
                return 0;
            }
        } else if (text_line_starts_with(line, "line ")) {
            if (!read_line_row(line, book)) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

const struct script_talk_message *script_talk_book_find(const struct script_talk_book *book, const char *label)
{
    int index;

    if (book == NULL) {
        return NULL;
    }
    for (index = 0; index < book->message_count; index++) {
        if (strcmp(book->messages[index].label, label) == 0) {
            return &book->messages[index];
        }
    }
    return NULL;
}

static const struct script_talk_page *current_page(const struct script_machine *machine)
{
    const struct script_talk *talk;

    talk = &machine->talk;
    if (machine->talk_book == NULL || talk->message == NULL) {
        return NULL;
    }
    return &machine->talk_book->pages[talk->message->first_page + talk->page_index];
}

static float slide_of(const struct script_talk *talk)
{
    int ticks;

    switch (talk->phase) {
    case SCRIPT_TALK_OPENING:
        ticks = talk->timer - OPEN_DELAY_TICKS;
        if (ticks <= 0) {
            return 0.0f;
        }
        return (float)ticks / (float)SLIDE_TICKS;
    case SCRIPT_TALK_SWITCHING_OUT:
    case SCRIPT_TALK_CLOSING:
        ticks = SLIDE_TICKS - talk->timer;
        if (ticks <= 0) {
            return 0.0f;
        }
        return (float)ticks / (float)SLIDE_TICKS;
    default:
        return 1.0f;
    }
}

static void report(struct script_machine *machine)
{
    const struct script_talk *talk;
    struct script_talk_view view;
    const struct script_talk_page *page;

    if (machine->outputs->talk_changed == NULL) {
        return;
    }
    talk = &machine->talk;
    page = current_page(machine);
    memset(&view, 0, sizeof(view));
    view.book = machine->talk_book;
    view.page = talk->phase == SCRIPT_TALK_CLOSED ? NULL : page;
    view.slide = slide_of(talk);
    view.glyphs_shown = talk->glyphs_shown;
    view.cursor_is_on = talk->phase == SCRIPT_TALK_WAITING && page != NULL && page->ending == SCRIPT_TALK_ENDS_ON_PRESS;
    view.text_alpha = 1.0f;
    if (page != NULL && page->reveal_is_fade) {
        view.text_alpha = (float)talk->fade_ticks / (float)FADE_IN_TICKS;
    }
    if (talk->phase == SCRIPT_TALK_OPENING || talk->phase == SCRIPT_TALK_CLOSING || talk->phase == SCRIPT_TALK_SWITCHING_OUT) {
        view.glyphs_shown = talk->phase == SCRIPT_TALK_OPENING ? 0 : (page != NULL ? page->glyph_count : 0);
        if (talk->phase == SCRIPT_TALK_OPENING) {
            view.text_alpha = 0.0f;
        }
    }
    machine->outputs->talk_changed(&view);
}

static void begin_reveal(struct script_talk *talk)
{
    talk->phase = SCRIPT_TALK_REVEALING;
    talk->timer = 0;
    talk->glyphs_shown = 0;
    talk->fade_ticks = 0;
}

void script_talk_open(struct script_machine *machine, const char *label)
{
    struct script_talk *talk;

    talk = &machine->talk;
    memset(talk, 0, sizeof(*talk));
    talk->message = script_talk_book_find(machine->talk_book, label);
    if (talk->message == NULL || talk->message->page_count == 0) {
        script_machine_note(machine, NULL, "no_talk_row", label);
        talk->message = NULL;
        return;
    }
    machine->pause_bit = 1;
    machine->talk_is_open = 1;
    talk->phase = SCRIPT_TALK_OPENING;
}

static void schedule_close(struct script_machine *machine)
{
    struct script_talk *talk;

    talk = &machine->talk;
    talk->phase = SCRIPT_TALK_CLOSING;
    talk->timer = 0;
    talk->close_is_scheduled = 1;
    machine->talk_close_at = (long long)machine->tick + LAST_PRESS_RELEASES_AFTER_TICKS;
}

static int next_page_needs_a_slide(const struct script_machine *machine)
{
    const struct script_talk_page *page;
    const struct script_talk_page *next;

    page = current_page(machine);
    next = page + 1;
    return page->is_top_box != next->is_top_box || page->has_face != next->has_face || (page->has_face && strcmp(page->face_texture, next->face_texture) != 0);
}

static void go_to_next_page_or_close(struct script_machine *machine)
{
    struct script_talk *talk;

    talk = &machine->talk;
    if (talk->message == NULL || talk->page_index + 1 >= talk->message->page_count) {
        schedule_close(machine);
        return;
    }
    if (next_page_needs_a_slide(machine)) {
        talk->phase = SCRIPT_TALK_SWITCHING_OUT;
        talk->timer = 0;
        return;
    }
    talk->page_index = talk->page_index + 1;
    begin_reveal(talk);
}

static void finish_reveal(struct script_machine *machine)
{
    const struct script_talk_page *page;

    machine->talk.phase = SCRIPT_TALK_WAITING;
    page = current_page(machine);
    if (page != NULL && page->ending != SCRIPT_TALK_ENDS_ON_PRESS) {
        script_machine_send(machine, "*", "message", "pause in");
    }
}

static void play_sound(struct script_machine *machine, int kind)
{
    if (machine->outputs->sound_played != NULL) {
        machine->outputs->sound_played(kind);
    }
}

static void step_revealing(struct script_machine *machine)
{
    struct script_talk *talk;
    const struct script_talk_page *page;

    talk = &machine->talk;
    page = current_page(machine);
    if (page == NULL) {
        finish_reveal(machine);
        return;
    }
    if (talk->press_is_pending) {
        talk->press_is_pending = 0;
        talk->glyphs_shown = page->glyph_count;
        talk->fade_ticks = FADE_IN_TICKS;
    }
    if (page->reveal_is_fade) {
        talk->glyphs_shown = page->glyph_count;
        talk->fade_ticks = talk->fade_ticks + 1;
        if (talk->fade_ticks >= FADE_IN_TICKS) {
            finish_reveal(machine);
        }
        return;
    }
    talk->glyphs_shown = talk->glyphs_shown + 1;
    if (talk->glyphs_shown >= page->glyph_count) {
        talk->glyphs_shown = page->glyph_count;
        finish_reveal(machine);
    }
}

static void step_waiting(struct script_machine *machine)
{
    struct script_talk *talk;
    const struct script_talk_page *page;

    talk = &machine->talk;
    page = current_page(machine);
    if (page == NULL || page->ending == SCRIPT_TALK_ENDS_ON_PRESS) {
        if (talk->press_is_pending) {
            talk->press_is_pending = 0;
            play_sound(machine, SOUND_KIND_PAGE_PRESS);
            go_to_next_page_or_close(machine);
        }
        return;
    }
    if (talk->forward_is_pending) {
        talk->forward_is_pending = 0;
        if (page->ending == SCRIPT_TALK_ENDS_ON_FORWARD_THEN_CLOSE) {
            schedule_close(machine);
        } else {
            go_to_next_page_or_close(machine);
        }
    }
}

void script_talk_step(struct script_machine *machine)
{
    struct script_talk *talk;
    const struct script_talk_page *page;

    talk = &machine->talk;
    if (talk->phase == SCRIPT_TALK_CLOSED) {
        return;
    }
    talk->timer = talk->timer + 1;
    switch (talk->phase) {
    case SCRIPT_TALK_OPENING:
        page = current_page(machine);
        if (page != NULL && page->has_face && talk->timer == FACE_SOUND_AFTER_TICKS) {
            play_sound(machine, SOUND_KIND_FACE_SLIDE);
        }
        if (talk->timer >= OPEN_DELAY_TICKS + SLIDE_TICKS) {
            begin_reveal(talk);
        }
        talk->press_is_pending = 0;
        break;
    case SCRIPT_TALK_REVEALING:
        step_revealing(machine);
        break;
    case SCRIPT_TALK_WAITING:
        step_waiting(machine);
        break;
    case SCRIPT_TALK_SWITCHING_OUT:
        talk->press_is_pending = 0;
        if (talk->timer >= SLIDE_TICKS) {
            talk->page_index = talk->page_index + 1;
            talk->phase = SCRIPT_TALK_OPENING;
            talk->timer = OPEN_DELAY_TICKS;
            talk->glyphs_shown = 0;
            talk->fade_ticks = 0;
        }
        break;
    default:
        talk->press_is_pending = 0;
        break;
    }
    report(machine);
}

void script_talk_note_press(struct script_machine *machine)
{
    if (machine->talk.phase != SCRIPT_TALK_CLOSED) {
        machine->talk.press_is_pending = 1;
    }
}

void script_talk_note_forward(struct script_machine *machine)
{
    machine->talk.forward_is_pending = 1;
}

void script_talk_close_when_due(struct script_machine *machine)
{
    struct script_talk *talk;

    talk = &machine->talk;
    if (machine->talk_close_at != (long long)machine->tick) {
        return;
    }
    machine->talk_close_at = -1;
    talk->phase = SCRIPT_TALK_CLOSED;
    talk->message = NULL;
    machine->talk_is_open = 0;
    if (!machine->soul_state_keeps_pause) {
        machine->pause_bit = 0;
    }
    report(machine);
}

int script_talk_wants_a_press(const struct script_machine *machine)
{
    const struct script_talk_page *page;

    page = current_page(machine);
    return page == NULL || page->ending == SCRIPT_TALK_ENDS_ON_PRESS || machine->talk.phase != SCRIPT_TALK_WAITING;
}

void script_talk_close_now(struct script_machine *machine)
{
    machine->talk_close_at = (long long)machine->tick + 1;
}
