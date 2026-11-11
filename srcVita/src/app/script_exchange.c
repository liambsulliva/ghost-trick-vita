#include <string.h>
#include "script_machine.h"

#define OUTLINE_TICKS 0x18
#define SWAP_CHAIN_TICKS 0x19
#define SWAP_SETTLE_TICKS 0x20
#define NO_OBJECT -1

static struct script_exchange *exchange_of(struct script_machine *machine)
{
    return &machine->soul.exchange;
}

static int object_is_known(const struct script_machine *machine, int object_index)
{
    return object_index >= 0 && object_index < machine->object_count;
}

static void end_the_link(struct script_machine *machine)
{
    struct script_exchange *exchange;

    exchange = exchange_of(machine);
    exchange->phase = SCRIPT_EXCHANGE_OFF;
    exchange->ticks = 0;
    exchange->root_object = NO_OBJECT;
    exchange->target_object = NO_OBJECT;
    exchange->scripts_answered = 0;
}

void script_exchange_reset(struct script_machine *machine)
{
    memset(exchange_of(machine), 0, sizeof(struct script_exchange));
    end_the_link(machine);
}

int script_exchange_is_on(const struct script_machine *machine)
{
    return machine->soul.exchange.phase != SCRIPT_EXCHANGE_OFF;
}

int script_exchange_holds_the_chain_out(const struct script_machine *machine)
{
    enum script_exchange_phase phase;

    phase = machine->soul.exchange.phase;
    return phase == SCRIPT_EXCHANGE_LINKED || phase == SCRIPT_EXCHANGE_PRESSED || phase == SCRIPT_EXCHANGE_ASKED;
}

int script_exchange_offers_the_buttons(const struct script_machine *machine)
{
    return machine->soul.exchange.phase == SCRIPT_EXCHANGE_LINKED && machine->soul.exchange.ticks >= OUTLINE_TICKS;
}

static void tell_both_ends(struct script_machine *machine, const char *root_word, const char *target_word)
{
    const struct script_exchange *exchange;

    exchange = exchange_of(machine);
    if (object_is_known(machine, exchange->root_object)) {
        script_machine_send(machine, machine->objects[exchange->root_object].name, "EXCHANGE", root_word);
    }
    if (object_is_known(machine, exchange->target_object)) {
        script_machine_send(machine, machine->objects[exchange->target_object].name, "EXCHANGE", target_word);
    }
}

int script_exchange_link(struct script_machine *machine, int object_index, int slot)
{
    struct script_soul *soul;
    struct script_exchange *exchange;
    int shape;

    soul = &machine->soul;
    exchange = &soul->exchange;
    if (exchange->phase != SCRIPT_EXCHANGE_OFF || soul->active_ghost != SCRIPT_GHOST_MISSILE || !object_is_known(machine, soul->active_object) || !object_is_known(machine, object_index)) {
        return 0;
    }
    shape = machine->objects[soul->active_object].actor.shape;
    if (shape == 0 || machine->objects[object_index].actor.shape != shape) {
        return 0;
    }
    exchange->phase = SCRIPT_EXCHANGE_LINKED;
    exchange->ticks = 0;
    exchange->root_object = soul->active_object;
    exchange->root_slot = soul->active_slot;
    exchange->target_object = object_index;
    exchange->target_slot = slot;
    exchange->scripts_answered = 0;
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("exchange_linked", machine->objects[object_index].name);
    }
    tell_both_ends(machine, "CONNECTED", "CONNECTED");
    return 1;
}

void script_exchange_confirm(struct script_machine *machine)
{
    struct script_exchange *exchange;

    exchange = exchange_of(machine);
    if (exchange->phase != SCRIPT_EXCHANGE_LINKED) {
        return;
    }
    exchange->phase = SCRIPT_EXCHANGE_PRESSED;
    exchange->ticks = 0;
}

void script_exchange_cancel(struct script_machine *machine)
{
    if (exchange_of(machine)->phase != SCRIPT_EXCHANGE_LINKED) {
        return;
    }
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("exchange_cancelled", machine->objects[exchange_of(machine)->target_object].name);
    }
    tell_both_ends(machine, "CANCEL", "CANCEL");
    end_the_link(machine);
}

static void send_the_tutorial_clicks(struct script_machine *machine)
{
    struct script_exchange *exchange;
    int is_dragging;

    exchange = exchange_of(machine);
    if (!exchange->tutorial_is_on) {
        return;
    }
    is_dragging = machine->soul.tip_input_is_held;
    if (is_dragging && !exchange->tutorial_click_is_sent) {
        script_machine_send(machine, "*", "Exchange", "onClick");
        exchange->tutorial_click_is_sent = 1;
    } else if (!is_dragging && exchange->tutorial_click_is_sent) {
        script_machine_send(machine, "*", "Exchange", "onRelease");
        exchange->tutorial_click_is_sent = 0;
    }
}

void script_exchange_step(struct script_machine *machine)
{
    struct script_exchange *exchange;

    exchange = exchange_of(machine);
    send_the_tutorial_clicks(machine);
    if (exchange->phase == SCRIPT_EXCHANGE_OFF) {
        return;
    }
    if (!machine->soul.is_on || machine->soul.active_ghost != SCRIPT_GHOST_MISSILE || !object_is_known(machine, exchange->root_object) || !object_is_known(machine, exchange->target_object)) {
        end_the_link(machine);
        return;
    }
    switch (exchange->phase) {
    case SCRIPT_EXCHANGE_LINKED:
        if (exchange->ticks < OUTLINE_TICKS) {
            exchange->ticks = exchange->ticks + 1;
        }
        break;
    case SCRIPT_EXCHANGE_PRESSED:
        exchange->phase = SCRIPT_EXCHANGE_ASKED;
        exchange->scripts_answered = 0;
        if (machine->outputs->marked != NULL) {
            machine->outputs->marked("exchange", machine->objects[exchange->target_object].name);
        }
        tell_both_ends(machine, "ROOT", "TARGET");
        script_motion_hard_hide(&machine->objects[exchange->root_object].actor);
        script_motion_hard_hide(&machine->objects[exchange->target_object].actor);
        break;
    case SCRIPT_EXCHANGE_ASKED:
        if (exchange->scripts_answered) {
            exchange->phase = SCRIPT_EXCHANGE_SETTLING;
            exchange->ticks = 0;
        }
        break;
    case SCRIPT_EXCHANGE_SETTLING:
        exchange->ticks = exchange->ticks + 1;
        if (exchange->ticks >= SWAP_CHAIN_TICKS + SWAP_SETTLE_TICKS) {
            exchange->swaps_done = exchange->swaps_done + 1;
            end_the_link(machine);
        }
        break;
    default:
        break;
    }
}

void script_exchange_note_core(struct script_machine *machine, int is_target, int object_index, int slot)
{
    struct script_exchange *exchange;

    exchange = exchange_of(machine);
    if (exchange->phase != SCRIPT_EXCHANGE_ASKED && exchange->phase != SCRIPT_EXCHANGE_LINKED) {
        return;
    }
    if (exchange->phase == SCRIPT_EXCHANGE_ASKED) {
        exchange->scripts_answered = 1;
    }
    if (is_target) {
        exchange->target_object = object_index;
        exchange->target_slot = slot;
    } else {
        exchange->root_object = object_index;
        exchange->root_slot = slot;
    }
}

void script_exchange_run_command(struct script_machine *machine, const char *arg)
{
    struct script_exchange *exchange;

    exchange = exchange_of(machine);
    if (strcmp(arg, "TutoStart") == 0) {
        exchange->tutorial_is_on = 1;
        exchange->tutorial_click_is_sent = 0;
    } else if (strcmp(arg, "TutoEnd") == 0) {
        exchange->tutorial_is_on = 0;
    }
}
