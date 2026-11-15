#include <stdio.h>
#include "script_flags.h"
#include "script_machine.h"
#include "script_narration.h"
#include "script_phone_book.h"

#define PLACE_LIST 0
#define FIRST_PLACE 1
#define BOOK_IS_HELD_FLAG 0x1022u

static void note_place(struct script_machine *machine, unsigned int place)
{
    if (place == 0 || place >= SCRIPT_PHONE_BOOK_MOST) {
        return;
    }
    if (machine->note_levels[PLACE_LIST][place] == 0) {
        machine->note_levels[PLACE_LIST][place] = 1;
    }
}

void script_phone_book_open(struct script_machine *machine, unsigned int mode, unsigned int place)
{
    struct script_phone_book *book;
    char detail[48];
    int id;

    book = &machine->phone_book;
    if (machine->outputs->marked != NULL) {
        snprintf(detail, sizeof(detail), "mode=%u place=%u", mode, place);
        machine->outputs->marked("phone_book_opened", detail);
    }
    note_place(machine, FIRST_PLACE);
    note_place(machine, place);
    book->count = 0;
    book->cursor = 0;
    for (id = 1; id < SCRIPT_PHONE_BOOK_MOST; id++) {
        if (machine->note_levels[PLACE_LIST][id] == 0) {
            continue;
        }
        if (id == (int)place) {
            book->cursor = book->count;
        }
        book->places[book->count] = id;
        book->count = book->count + 1;
    }
    machine->phone_book_is_open = 1;
}

int script_phone_book_takes_input(const struct script_machine *machine)
{
    return machine->phone_book_is_open && machine->phone_book.count > 0 && !script_flags_read(&machine->flags, BOOK_IS_HELD_FLAG) && machine->talk.phase == SCRIPT_TALK_CLOSED && !script_narration_wants_a_press(&machine->narration) && !machine->soul.dead_talk_is_on;
}

int script_phone_book_index_of(const struct script_machine *machine, int place)
{
    int index;

    for (index = 0; index < machine->phone_book.count; index++) {
        if (machine->phone_book.places[index] == place) {
            return index;
        }
    }
    return -1;
}

void script_phone_book_move_cursor(struct script_machine *machine, int steps)
{
    struct script_phone_book *book;

    book = &machine->phone_book;
    if (!script_phone_book_takes_input(machine) || steps == 0) {
        return;
    }
    book->cursor = ((book->cursor + steps) % book->count + book->count) % book->count;
}

static void answer(struct script_machine *machine, const char *event)
{
    char detail[48];

    machine->phone_book_is_open = 0;
    machine->phone_book.answers = machine->phone_book.answers + 1;
    script_hints_player_did(machine, SCRIPT_ROUTE_PHONE_CALL, NULL);
    if (machine->outputs->marked != NULL) {
        snprintf(detail, sizeof(detail), "%s place=%d", event, machine->phone_book_place);
        machine->outputs->marked("phone_book_answered", detail);
    }
    script_machine_send(machine, "*", "phone", event);
}

void script_phone_book_go_there(struct script_machine *machine)
{
    if (!script_phone_book_takes_input(machine)) {
        return;
    }
    machine->phone_book_place = machine->phone_book.places[machine->phone_book.cursor];
    answer(machine, "movie");
}

void script_phone_book_stay_here(struct script_machine *machine)
{
    if (!script_phone_book_takes_input(machine)) {
        return;
    }
    answer(machine, "cancel");
}

void script_phone_book_fill_view(const struct script_machine *machine, struct script_hud_view *view)
{
    int index;

    view->phone_book_is_up = script_phone_book_takes_input(machine);
    view->phone_book_count = view->phone_book_is_up ? machine->phone_book.count : 0;
    view->phone_book_cursor = machine->phone_book.cursor;
    for (index = 0; index < view->phone_book_count; index++) {
        view->phone_book_places[index] = machine->phone_book.places[index];
    }
}
