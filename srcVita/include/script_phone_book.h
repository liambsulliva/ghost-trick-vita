#ifndef SCRIPT_PHONE_BOOK_H
#define SCRIPT_PHONE_BOOK_H

#include "script_outputs.h"

struct script_phone_book {
    int places[SCRIPT_PHONE_BOOK_MOST];
    int count;
    int cursor;
    int answers;
};

struct script_machine;

void script_phone_book_open(struct script_machine *machine, unsigned int mode, unsigned int place);
int script_phone_book_takes_input(const struct script_machine *machine);
int script_phone_book_index_of(const struct script_machine *machine, int place);
void script_phone_book_move_cursor(struct script_machine *machine, int steps);
void script_phone_book_go_there(struct script_machine *machine);
void script_phone_book_stay_here(struct script_machine *machine);
void script_phone_book_fill_view(const struct script_machine *machine, struct script_hud_view *view);

#endif
