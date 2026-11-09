#include "script_classes.h"
#include "script_machine.h"

#define GHOST_COPY_FADE_IN_TICKS 60
#define GHOST_COPY_FALL_TICKS 100
#define GHOST_COPY_HOLD_BEFORE_VANISHING 60
#define GHOST_COPY_VANISH_TICKS 100

void script_ghost_copy_fade_out(struct script_ghost_copy *copy)
{
    copy->hold_ticks = 0;
    copy->fall_ticks = GHOST_COPY_HOLD_BEFORE_VANISHING;
    copy->vanishes = 1;
    copy->is_fading_in = 0;
    copy->vanish_ticks = 0;
}

int script_ghost_copy_shown_level(const struct script_ghost_copy *copy)
{
    if (!copy->exists) {
        return -1;
    }
    return copy->is_gone ? 0 : copy->level;
}

static void make_the_copy(struct script_ghost_copy *copy)
{
    copy->exists = 1;
    copy->is_gone = 0;
    copy->level = SCRIPT_GHOST_COPY_LEVEL_FULL;
    copy->was_visible = 1;
}

static void step_the_copy(struct script_ghost_copy *copy, int source_is_visible)
{
    if (!copy->exists || copy->is_gone) {
        return;
    }
    if (!source_is_visible) {
        copy->was_visible = 0;
        copy->level = 0;
        return;
    }
    if (!copy->was_visible) {
        copy->was_visible = 1;
        copy->is_fading_in = 1;
        copy->fade_in_ticks = 0;
    }
    if (copy->is_fading_in) {
        copy->level = copy->fade_in_ticks * SCRIPT_GHOST_COPY_LEVEL_FULL / GHOST_COPY_FADE_IN_TICKS;
        copy->fade_in_ticks = copy->fade_in_ticks + 1;
        if (copy->level > SCRIPT_GHOST_COPY_LEVEL_FULL) {
            copy->level = SCRIPT_GHOST_COPY_LEVEL_FULL;
            copy->is_fading_in = 0;
        }
        return;
    }
    if (copy->hold_ticks > 0) {
        copy->hold_ticks = copy->hold_ticks - 1;
        if (copy->hold_ticks <= 0) {
            copy->fall_ticks = GHOST_COPY_FALL_TICKS;
        }
    }
    if (copy->fall_ticks > 0) {
        copy->fall_ticks = copy->fall_ticks - 1;
        if (copy->fall_ticks <= 0) {
            if (copy->vanishes) {
                copy->vanish_ticks = GHOST_COPY_VANISH_TICKS;
            } else {
                copy->level = 0;
            }
            return;
        }
        if (!copy->vanishes) {
            copy->level = SCRIPT_GHOST_COPY_LEVEL_FULL * copy->fall_ticks / GHOST_COPY_FALL_TICKS;
        }
    }
    if (copy->vanish_ticks != 0) {
        copy->level = SCRIPT_GHOST_COPY_LEVEL_FULL * copy->vanish_ticks / GHOST_COPY_VANISH_TICKS;
        copy->vanish_ticks = copy->vanish_ticks - 1;
        if (copy->vanish_ticks <= 0) {
            copy->is_gone = 1;
            copy->level = 0;
        }
    }
}

static void fade_out_every_past_dead_copy(struct script_machine *machine)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        struct script_actor *actor;

        actor = &machine->objects[index].actor;
        if (script_class_is_past_dead(actor->class_id) && actor->ghost_copy.exists && !actor->ghost_copy.is_gone) {
            script_ghost_copy_fade_out(&actor->ghost_copy);
        }
    }
}

static void watch_the_soul_leave_its_start(struct script_machine *machine, int index)
{
    struct script_actor *actor;
    int soul_is_here;

    actor = &machine->objects[index].actor;
    if (!actor->ghost_copy.exists) {
        make_the_copy(&actor->ghost_copy);
    }
    soul_is_here = machine->soul.active_object == index;
    if (soul_is_here) {
        actor->ghost_copy.held_the_soul = 1;
        return;
    }
    if (actor->ghost_copy.held_the_soul && !actor->ghost_copy.is_gone) {
        actor->ghost_copy.held_the_soul = 0;
        script_ghost_copy_fade_out(&actor->ghost_copy);
        fade_out_every_past_dead_copy(machine);
    }
}

void script_ghost_copies_update(struct script_machine *machine)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        struct script_actor *actor;

        actor = &machine->objects[index].actor;
        if (!actor->exists) {
            continue;
        }
        if (script_class_is_soul_start(actor->class_id)) {
            watch_the_soul_leave_its_start(machine, index);
        } else if (script_class_is_past_dead(actor->class_id) && !actor->ghost_copy.exists && actor->is_visible) {
            make_the_copy(&actor->ghost_copy);
        }
    }
    for (index = 0; index < machine->object_count; index++) {
        struct script_actor *actor;

        actor = &machine->objects[index].actor;
        if (actor->exists) {
            step_the_copy(&actor->ghost_copy, actor->is_visible);
        }
    }
}
