#ifndef SCRIPT_ROUTE_PAD_H
#define SCRIPT_ROUTE_PAD_H

#include "input.h"

struct script_machine;

struct script_route_pad {
    int page_was_waiting;
    unsigned int page_ready_at;
    int game_over_was_waiting;
    unsigned int game_over_since;
    int choice_was_open;
    unsigned int choice_seen_at;
    int has_pressed;
    unsigned int last_press_at;
};

void script_route_pad_reset(struct script_route_pad *pad);
void script_route_pad_frame(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame);

#endif
