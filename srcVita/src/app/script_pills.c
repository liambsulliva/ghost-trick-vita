#include <stdio.h>
#include <string.h>
#include "script_machine.h"

#define PILL_HIDDEN 0
#define PILL_SHOWN 1
#define PILL_SLIDING_IN 2
#define PILL_SLIDING_OUT 3
#define PILL_PRESSED 4
#define PILL_SLIDE_STEPS 8
#define PILL_PRESS_TICKS 12
#define PILL_WAIT_BEFORE_SLIDE_IN 3
#define NORMAL_PLAY_MODE 3
#define FIRST_PILL_EFFECT 99
#define PILL_EFFECT_COUNT 4
#define BIT(id) (1u << (unsigned int)(id))
#define NON_NATIVE_ALL (BIT(SCRIPT_PILL_GHOST) | BIT(SCRIPT_PILL_BACK) | BIT(SCRIPT_PILL_NOTEBOOK) | BIT(SCRIPT_PILL_TRICK))

enum pill_group {
    GROUP_NONE,
    GROUP_NORMAL,
    GROUP_GHOST,
    GROUP_DEMO
};

static const enum script_pill_id pills_of_effects[PILL_EFFECT_COUNT] = { SCRIPT_PILL_GHOST, SCRIPT_PILL_BACK, SCRIPT_PILL_HOURGLASS, SCRIPT_PILL_NOTEBOOK };

void script_pills_reset(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    memset(soul->pills, 0, sizeof(soul->pills));
    memset(soul->pill_effect_names, 0, sizeof(soul->pill_effect_names));
    soul->pills_registered = 0;
    soul->pills_shown_by_effect = 0;
    soul->pill_slide_in_wait = 0;
    soul->pill_pulse_ticks = 0;
    soul->ghost_pill_is_held_out = 0;
    soul->rollback_is_on = 0;
}

void script_pills_register(struct script_machine *machine, enum script_pill_id id, int is_on)
{
    if (is_on) {
        machine->soul.pills_registered = machine->soul.pills_registered | BIT(id);
    } else {
        machine->soul.pills_registered = machine->soul.pills_registered & ~BIT(id);
    }
}

void script_pills_register_all(struct script_machine *machine)
{
    machine->soul.pills_registered = NON_NATIVE_ALL;
}

void script_pills_unregister_all(struct script_machine *machine)
{
    int index;

    machine->soul.pills_registered = 0;
    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        if (index != SCRIPT_PILL_HOURGLASS) {
            machine->soul.pills[index].state = PILL_HIDDEN;
            machine->soul.pills[index].step = 0;
        }
    }
}

void script_pills_effect_spawned(struct script_machine *machine, int effect_id, const char *name)
{
    int slot;

    slot = effect_id - FIRST_PILL_EFFECT;
    if (slot < 0 || slot >= PILL_EFFECT_COUNT) {
        return;
    }
    machine->soul.pills_shown_by_effect = machine->soul.pills_shown_by_effect | BIT(pills_of_effects[slot]);
    machine->soul.pill_effect_names[pills_of_effects[slot]] = name;
}

void script_pills_effect_deleted(struct script_machine *machine, const char *name)
{
    int index;

    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        const char *owner;

        owner = machine->soul.pill_effect_names[index];
        if (owner != NULL && strcmp(owner, name) == 0) {
            machine->soul.pill_effect_names[index] = NULL;
            machine->soul.pills_shown_by_effect = machine->soul.pills_shown_by_effect & ~BIT(index);
        }
    }
}

static enum pill_group current_group(const struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    if (machine->eyecatch_ticks_left > 0 || soul->is_leaving_view || (soul->is_on && soul->view_change_ticks_left > 0)) {
        return GROUP_NONE;
    }
    if (machine->pause_bit && machine->talk.phase != SCRIPT_TALK_CLOSED) {
        return GROUP_NONE;
    }
    if (soul->is_on) {
        return machine->demo_soul_is_on ? GROUP_DEMO : GROUP_GHOST;
    }
    if (!machine->pause_bit && machine->mode_byte == NORMAL_PLAY_MODE) {
        return GROUP_NORMAL;
    }
    return GROUP_DEMO;
}

static int current_core_is_busy(const struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    return soul->active_object >= 0 && soul->active_object < machine->object_count && (machine->objects[soul->active_object].actor.busy_cores & (1 << soul->active_slot)) != 0;
}

static int pill_is_wanted(const struct script_machine *machine, enum script_pill_id id, enum pill_group group)
{
    const struct script_soul *soul;
    int is_registered;

    soul = &machine->soul;
    if (soul->pills_shown_by_effect & BIT(id)) {
        return 1;
    }
    is_registered = (soul->pills_registered & BIT(id)) != 0;
    switch (id) {
    case SCRIPT_PILL_GHOST:
        return is_registered && group == GROUP_NORMAL && !soul->ghost_pill_is_held_out;
    case SCRIPT_PILL_TRICK:
        return is_registered && group == GROUP_NORMAL && !current_core_is_busy(machine);
    case SCRIPT_PILL_NOTEBOOK:
        return is_registered && group == GROUP_NORMAL;
    case SCRIPT_PILL_BACK:
        return is_registered && group == GROUP_GHOST;
    case SCRIPT_PILL_HOURGLASS:
        return soul->rollback_is_on && group != GROUP_NONE;
    default:
        return 0;
    }
}

static int some_pill_is_leaving(const struct script_soul *soul)
{
    int index;

    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        if (soul->pills[index].state == PILL_SLIDING_OUT || soul->pills[index].state == PILL_PRESSED || soul->pills[index].hold_ticks > 0) {
            return 1;
        }
    }
    return 0;
}

static void step_pill(struct script_pill *pill, int is_wanted, int may_slide_in)
{
    switch (pill->state) {
    case PILL_PRESSED:
        pill->press_ticks = pill->press_ticks + 1;
        if (pill->press_ticks >= PILL_PRESS_TICKS) {
            pill->state = PILL_SHOWN;
        }
        break;
    case PILL_SHOWN:
        if (pill->hold_ticks > 0) {
            pill->hold_ticks = pill->hold_ticks - 1;
        } else if (!is_wanted) {
            pill->state = PILL_SLIDING_OUT;
            pill->step = PILL_SLIDE_STEPS;
        }
        break;
    case PILL_SLIDING_IN:
        pill->step = pill->step + 1;
        if (!is_wanted) {
            pill->state = PILL_SLIDING_OUT;
        } else if (pill->step >= PILL_SLIDE_STEPS) {
            pill->state = PILL_SHOWN;
        }
        break;
    case PILL_SLIDING_OUT:
        pill->step = pill->step - 1;
        if (pill->step <= 0) {
            pill->step = 0;
            pill->state = PILL_HIDDEN;
        }
        break;
    default:
        if (is_wanted && may_slide_in) {
            pill->state = PILL_SLIDING_IN;
            pill->step = 0;
        }
        break;
    }
}

static int pill_press_changes_the_view(const struct script_pill *pill)
{
    return pill->state == PILL_PRESSED;
}

void script_pills_step(struct script_machine *machine)
{
    struct script_soul *soul;
    enum pill_group group;
    int is_leaving;
    int index;
    int some_slid_in;
    int view_press_holds_the_others;

    soul = &machine->soul;
    group = current_group(machine);
    is_leaving = some_pill_is_leaving(soul);
    view_press_holds_the_others = pill_press_changes_the_view(&soul->pills[SCRIPT_PILL_GHOST]) || pill_press_changes_the_view(&soul->pills[SCRIPT_PILL_BACK]);
    if (is_leaving) {
        soul->pill_slide_in_wait = PILL_WAIT_BEFORE_SLIDE_IN;
    } else if (soul->pill_slide_in_wait > 0) {
        soul->pill_slide_in_wait = soul->pill_slide_in_wait - 1;
    }
    some_slid_in = 0;
    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        struct script_pill *pill;
        int was_hidden;

        pill = &soul->pills[index];
        was_hidden = pill->state == PILL_HIDDEN;
        step_pill(pill, pill_is_wanted(machine, (enum script_pill_id)index, group) || (view_press_holds_the_others && pill->state == PILL_SHOWN), !is_leaving && soul->pill_slide_in_wait == 0);
        if (was_hidden && pill->state == PILL_SLIDING_IN) {
            some_slid_in = 1;
        }
    }
    soul->pill_pulse_ticks = some_slid_in ? 0u : soul->pill_pulse_ticks + 1u;
}

void script_pills_press(struct script_machine *machine, enum script_pill_id id, int ticks_after_press)
{
    struct script_pill *pill;

    pill = &machine->soul.pills[id];
    if (pill->state != PILL_SHOWN) {
        return;
    }
    pill->state = PILL_PRESSED;
    pill->step = PILL_SLIDE_STEPS;
    pill->press_ticks = 0;
    pill->hold_ticks = ticks_after_press;
}

int script_pills_is_up(const struct script_machine *machine, enum script_pill_id id)
{
    return machine->soul.pills[id].state == PILL_SHOWN;
}

void script_pills_fill_view(const struct script_machine *machine, struct script_hud_view *view)
{
    int index;

    view->pulse_ticks = (int)machine->soul.pill_pulse_ticks;
    for (index = 0; index < SCRIPT_PILL_COUNT; index++) {
        const struct script_pill *pill;

        pill = &machine->soul.pills[index];
        view->pills[index].is_on_screen = pill->state != PILL_HIDDEN;
        view->pills[index].is_pressed = pill->state == PILL_PRESSED;
        view->pills[index].slide_step = pill->state == PILL_SHOWN || pill->state == PILL_PRESSED ? PILL_SLIDE_STEPS : pill->step;
    }
}
