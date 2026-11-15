#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_machine.h"
#include "ds_units.h"

#define NO_STEP -1
#define NO_GROUP 0
#define OBJECT_WITH_NO_NAME "-"
#define FLASH_TICKS 20
#define FIRST_FLASHES 3
#define FLASH_REPEATS_EVERY 300
#define PULSE_TICKS 40
#define HOLD_TICKS 60
#define PAN_PIXELS_PER_TICK 8
#define PAN_MOST_TICKS 45
#define SCREEN_MARGIN 16
#define ACT_TOKEN_WORD "act:"
#define CHARACTER_PILLS ((1u << SCRIPT_PILL_JOWD) | (1u << SCRIPT_PILL_MISSILE) | (1u << SCRIPT_PILL_CICEL))

static const struct script_route_step *step_at(const struct script_machine *machine, int index)
{
    const struct script_route *route;

    route = machine->route;
    if (route == NULL || machine->hints.script == NULL || index < 0 || index >= route->step_count) {
        return NULL;
    }
    if (strcmp(route->steps[index].script, machine->hints.script) != 0) {
        return NULL;
    }
    return &route->steps[index];
}

static int action_acts_on_an_object(enum script_route_action action)
{
    switch (action) {
    case SCRIPT_ROUTE_TRICK:
    case SCRIPT_ROUTE_TAP_BALLOON:
    case SCRIPT_ROUTE_SWAP:
    case SCRIPT_ROUTE_CHARACTER:
    case SCRIPT_ROUTE_DEAD_TALK:
    case SCRIPT_ROUTE_PHONE_CALL:
    case SCRIPT_ROUTE_PHONE_STAY:
        return 1;
    default:
        return 0;
    }
}

static int action_answers_the_phone(enum script_route_action action)
{
    return action == SCRIPT_ROUTE_PHONE_CALL || action == SCRIPT_ROUTE_PHONE_STAY;
}

static int step_names_an_object(const struct script_route_step *step)
{
    return step->object[0] != '\0' && strcmp(step->object, OBJECT_WITH_NO_NAME) != 0;
}

static int next_acting_step(const struct script_machine *machine, int from)
{
    const struct script_route_step *step;
    int index;

    if (from < 0) {
        return NO_STEP;
    }
    for (index = from; (step = step_at(machine, index)) != NULL; index++) {
        if (action_acts_on_an_object(step->action)) {
            return index;
        }
    }
    return NO_STEP;
}

static int object_on_the_stage(const struct script_machine *machine, const char *name)
{
    const struct script_actor *actor;
    int index;

    index = script_machine_index_of_object(machine, name);
    if (index == SCRIPT_NO_OBJECT) {
        return SCRIPT_NO_OBJECT;
    }
    actor = &machine->objects[index].actor;
    if (!actor->exists || !actor->is_visible || actor->is_hard_hidden || actor->is_frozen_and_hidden) {
        return SCRIPT_NO_OBJECT;
    }
    return index;
}

static int object_of_step(const struct script_machine *machine, int step_index, int *object, int *slot)
{
    const struct script_route_step *step;
    int index;

    for (index = step_index; (step = step_at(machine, index)) != NULL; index--) {
        if (index != step_index && action_acts_on_an_object(step->action)) {
            return 0;
        }
        *object = step_names_an_object(step) ? object_on_the_stage(machine, step->object) : SCRIPT_NO_OBJECT;
        if (*object != SCRIPT_NO_OBJECT) {
            *slot = step->slot;
            return 1;
        }
    }
    return 0;
}

static void target_point(const struct script_machine *machine, int object, int slot, int *x, int *y)
{
    const struct script_actor *actor;

    actor = &machine->objects[object].actor;
    if (slot == 1 && actor->has_second_core) {
        *x = actor->x + actor->second_core_x;
        *y = actor->y + actor->second_core_y;
    } else if (actor->has_core) {
        *x = actor->x + actor->core_x;
        *y = actor->y + actor->core_y;
    } else {
        *x = actor->x + actor->points[0][0];
        *y = actor->y + actor->points[0][1];
    }
}

static int point_is_in_view(const struct script_machine *machine, int camera_x, int camera_y, int x, int y, int margin)
{
    int across;
    int down;

    across = x - (camera_x - machine->world_origin_x);
    down = y - (camera_y - machine->world_origin_y);
    return across >= margin && across <= DS_VIEW_WIDTH - margin && down >= margin && down <= DS_VIEW_HEIGHT - margin;
}

static void camera_framing(const struct script_machine *machine, int x, int y, int *camera_x, int *camera_y)
{
    *camera_x = machine->world_origin_x + x - DS_VIEW_WIDTH / 2;
    *camera_y = machine->world_origin_y + y - DS_VIEW_HEIGHT / 2;
    script_machine_nearest_camera(machine, camera_x, camera_y);
}

static int find_target(const struct script_machine *machine, int *step, int *object, int *slot)
{
    int index;
    int x;
    int y;
    int camera_x;
    int camera_y;

    for (index = next_acting_step(machine, machine->hints.next_step); index != NO_STEP; index = next_acting_step(machine, index + 1)) {
        if (!object_of_step(machine, index, object, slot)) {
            continue;
        }
        target_point(machine, *object, *slot, &x, &y);
        camera_framing(machine, x, y, &camera_x, &camera_y);
        if (!point_is_in_view(machine, camera_x, camera_y, x, y, 0)) {
            return 0;
        }
        *step = index;
        return 1;
    }
    return 0;
}

static int free_tap_group(const struct script_machine *machine)
{
    int pill;

    if (!script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_RIGHT)) {
        return SCRIPT_TAP_GROUP_RIGHT;
    }
    if (script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_LEFT)) {
        return NO_GROUP;
    }
    for (pill = 0; pill < SCRIPT_PILL_COUNT; pill++) {
        if ((CHARACTER_PILLS & (1u << pill)) && script_pills_is_up(machine, (enum script_pill_id)pill)) {
            return NO_GROUP;
        }
    }
    return SCRIPT_TAP_GROUP_LEFT;
}

static int hints_are_on(const struct script_machine *machine)
{
    return machine->hints.delay_ticks > 0 && !machine->route_is_driving && machine->route != NULL && machine->hints.script != NULL;
}

static int the_bubble_can_show(const struct script_machine *machine)
{
    return !machine->soul.is_on && !machine->pause_bit && machine->soul.active_object >= 0 && script_balloons_are_live(machine);
}

static void mark(struct script_machine *machine, const char *what)
{
    char detail[SCRIPT_ROUTE_NAME_MAX + 16];
    const struct script_hints *hints;

    hints = &machine->hints;
    if (machine->outputs == NULL || machine->outputs->marked == NULL) {
        return;
    }
    snprintf(detail, sizeof(detail), "%d:%s", hints->target_step, hints->target_object >= 0 && hints->target_object < machine->object_count ? machine->objects[hints->target_object].name : OBJECT_WITH_NO_NAME);
    machine->outputs->marked(what, detail);
}

static int offer_holds(struct script_machine *machine)
{
    struct script_hints *hints;
    int group;

    hints = &machine->hints;
    group = free_tap_group(machine);
    if (!hints_are_on(machine) || group == NO_GROUP || !find_target(machine, &hints->target_step, &hints->target_object, &hints->target_slot)) {
        return 0;
    }
    hints->tap_group = group;
    return 1;
}

static void move_camera_between(struct script_machine *machine, int from_x, int from_y, int to_x, int to_y)
{
    const struct script_hints *hints;
    int wanted_x;
    int wanted_y;

    hints = &machine->hints;
    wanted_x = from_x + (to_x - from_x) * hints->phase_ticks / hints->pan_ticks;
    wanted_y = from_y + (to_y - from_y) * hints->phase_ticks / hints->pan_ticks;
    script_machine_scroll_camera(machine, wanted_x - machine->camera_x, wanted_y - machine->camera_y);
}

static void use_the_hint(struct script_machine *machine)
{
    struct script_hints *hints;
    int x;
    int y;
    int far_x;
    int far_y;

    hints = &machine->hints;
    hints->was_used = 1;
    hints->flash_clock = 0;
    hints->pulse_clock = 0;
    hints->phase_ticks = 0;
    mark(machine, "hint_used");
    target_point(machine, hints->target_object, hints->target_slot, &x, &y);
    if (point_is_in_view(machine, machine->camera_x, machine->camera_y, x, y, SCREEN_MARGIN)) {
        hints->phase = SCRIPT_HINT_SHOWN;
        return;
    }
    hints->home_camera_x = machine->camera_x;
    hints->home_camera_y = machine->camera_y;
    camera_framing(machine, x, y, &hints->goal_camera_x, &hints->goal_camera_y);
    far_x = abs(hints->goal_camera_x - hints->home_camera_x);
    far_y = abs(hints->goal_camera_y - hints->home_camera_y);
    hints->pan_ticks = (far_x > far_y ? far_x : far_y) / PAN_PIXELS_PER_TICK;
    hints->pan_ticks = hints->pan_ticks < 1 ? 1 : (hints->pan_ticks > PAN_MOST_TICKS ? PAN_MOST_TICKS : hints->pan_ticks);
    hints->phase = SCRIPT_HINT_PANNING_OUT;
    machine->pause_bit = 1;
}

static void finish_the_hint_when_its_step_is_done(struct script_machine *machine)
{
    struct script_hints *hints;

    hints = &machine->hints;
    if (hints->phase == SCRIPT_HINT_SHOWN && (hints->next_step > hints->target_step || hints->next_step == NO_STEP)) {
        hints->phase = SCRIPT_HINT_SPENT;
        mark(machine, "hint_done");
    }
}

static void go_to_step(struct script_machine *machine, int step_index)
{
    struct script_hints *hints;

    hints = &machine->hints;
    hints->next_step = step_at(machine, step_index) != NULL ? step_index : NO_STEP;
    hints->stuck_ticks = 0;
    if (hints->phase == SCRIPT_HINT_OFFERED) {
        hints->phase = SCRIPT_HINT_WAITING;
    }
    finish_the_hint_when_its_step_is_done(machine);
}

static int first_step_after_retries(const struct script_machine *machine, int retries)
{
    const struct script_route_step *step;
    struct script_route_wait_token token;
    const char *rest;
    int first;
    int found;
    int index;

    first = script_route_first_step_of_script(machine->route, machine->hints.script);
    found = NO_STEP;
    for (index = first; (step = step_at(machine, index)) != NULL; index++) {
        rest = step->wait;
        while (rest != NULL && *rest != '\0') {
            rest = script_route_read_wait_token(rest, &token);
            if (token.word == SCRIPT_ROUTE_WAIT_RETRIED && retries >= (token.first != NULL ? atoi(token.first) : 1)) {
                found = index;
            }
        }
    }
    return found;
}

void script_hints_scene_retried(struct script_machine *machine)
{
    struct script_hints *hints;
    int stuck_ticks;
    int restart_step;

    hints = &machine->hints;
    if (machine->route == NULL || hints->script == NULL) {
        return;
    }
    stuck_ticks = hints->stuck_ticks;
    if (hints->phase == SCRIPT_HINT_PANNING_OUT || hints->phase == SCRIPT_HINT_HOLDING || hints->phase == SCRIPT_HINT_PANNING_BACK) {
        hints->phase = SCRIPT_HINT_SHOWN;
    }
    restart_step = first_step_after_retries(machine, machine->retries_of_the_scene);
    if (restart_step != NO_STEP) {
        go_to_step(machine, restart_step);
    }
    hints->stuck_ticks = stuck_ticks;
    if (hints->phase == SCRIPT_HINT_SHOWN && hints->target_step < hints->next_step) {
        hints->phase = SCRIPT_HINT_SPENT;
    }
}

void script_hints_script_began(struct script_machine *machine, const char *script, int is_a_retry)
{
    struct script_hints *hints;
    int delay_ticks;

    hints = &machine->hints;
    if (is_a_retry && hints->script != NULL && strcmp(hints->script, script) == 0) {
        hints->script = script;
        script_hints_scene_retried(machine);
        return;
    }
    delay_ticks = hints->delay_ticks;
    memset(hints, 0, sizeof(*hints));
    hints->delay_ticks = delay_ticks;
    hints->script = script;
    hints->target_step = NO_STEP;
    hints->target_object = SCRIPT_NO_OBJECT;
    hints->next_step = machine->route != NULL ? script_route_first_step_of_script(machine->route, script) : NO_STEP;
}

void script_hints_player_did(struct script_machine *machine, enum script_route_action action, const char *object_name)
{
    const struct script_route_step *step;
    int index;

    index = next_acting_step(machine, machine->hints.next_step);
    step = step_at(machine, index);
    if (step == NULL) {
        return;
    }
    if (step->action != action && !(action_answers_the_phone(step->action) && action_answers_the_phone(action))) {
        return;
    }
    if (object_name != NULL && step_names_an_object(step) && strcmp(step->object, object_name) != 0) {
        return;
    }
    go_to_step(machine, index + 1);
}

void script_hints_act_began(struct script_machine *machine, const char *object_name, const char *act_name)
{
    const struct script_route_step *step;
    char act_prefix[SCRIPT_ROUTE_WAIT_MAX];
    char act_token[SCRIPT_ROUTE_WAIT_MAX];
    int acting_step;
    int waits_on_another_act;
    int index;

    acting_step = next_acting_step(machine, machine->hints.next_step);
    if (acting_step == NO_STEP) {
        return;
    }
    snprintf(act_prefix, sizeof(act_prefix), ACT_TOKEN_WORD "%s:", object_name);
    snprintf(act_token, sizeof(act_token), "%s%s", act_prefix, act_name);
    waits_on_another_act = 0;
    for (index = machine->hints.next_step; index <= acting_step; index++) {
        step = step_at(machine, index);
        if (script_route_wait_has_token(step->wait, act_token)) {
            return;
        }
        if (strstr(step->wait, act_prefix) != NULL) {
            waits_on_another_act = 1;
        }
    }
    if (!waits_on_another_act) {
        return;
    }
    for (index = acting_step + 1; (step = step_at(machine, index)) != NULL; index++) {
        if (script_route_wait_has_token(step->wait, act_token)) {
            go_to_step(machine, index);
            return;
        }
    }
}

int script_hints_have_passed_step(const struct script_machine *machine, int step_index)
{
    const struct script_route_step *step;

    step = step_at(machine, step_index);
    return step == NULL || !action_acts_on_an_object(step->action) || machine->hints.next_step > step_index || machine->hints.next_step == NO_STEP;
}

int script_hints_are_ahead_of_step(const struct script_machine *machine, int step_index)
{
    int acting_step;

    acting_step = next_acting_step(machine, step_index + 1);
    if (step_at(machine, step_index) == NULL || acting_step == NO_STEP) {
        return 0;
    }
    return machine->hints.next_step == NO_STEP || machine->hints.next_step > acting_step;
}

int script_hints_can_point_at_step(const struct script_machine *machine, int step_index)
{
    const struct script_route_step *step;
    int found_step;
    int object;
    int slot;

    step = step_at(machine, step_index);
    if (step == NULL || !action_acts_on_an_object(step->action)) {
        return 1;
    }
    return find_target(machine, &found_step, &object, &slot) && found_step == step_index;
}

void script_hints_note_tap(struct script_machine *machine)
{
    machine->hints.tap_is_pending = 1;
}

static int script_hints_bubble_is_up(const struct script_machine *machine)
{
    return machine->hints.phase == SCRIPT_HINT_OFFERED && the_bubble_can_show(machine);
}

int script_hints_take_tap(struct script_machine *machine, int tap_group)
{
    if (!script_hints_bubble_is_up(machine) || machine->hints.tap_group != tap_group) {
        return 0;
    }
    machine->hints.tap_is_pending = 1;
    return 1;
}

int script_hints_hold_the_soul(const struct script_machine *machine)
{
    return machine->hints.phase == SCRIPT_HINT_PANNING_OUT || machine->hints.phase == SCRIPT_HINT_HOLDING || machine->hints.phase == SCRIPT_HINT_PANNING_BACK;
}

void script_hints_step(struct script_machine *machine)
{
    struct script_hints *hints;
    int tap_is_pending;

    hints = &machine->hints;
    tap_is_pending = hints->tap_is_pending;
    hints->tap_is_pending = 0;
    switch (hints->phase) {
    case SCRIPT_HINT_WAITING:
        if (!hints_are_on(machine) || hints->was_used) {
            break;
        }
        if (hints->stuck_ticks < hints->delay_ticks && script_soul_player_has_control(machine)) {
            hints->stuck_ticks = hints->stuck_ticks + 1;
        }
        if (hints->stuck_ticks >= hints->delay_ticks && the_bubble_can_show(machine) && offer_holds(machine)) {
            hints->phase = SCRIPT_HINT_OFFERED;
            mark(machine, "hint_offered");
        }
        break;
    case SCRIPT_HINT_OFFERED:
        if (!offer_holds(machine)) {
            hints->phase = SCRIPT_HINT_WAITING;
        } else if (tap_is_pending && the_bubble_can_show(machine)) {
            use_the_hint(machine);
        }
        break;
    case SCRIPT_HINT_PANNING_OUT:
        hints->phase_ticks = hints->phase_ticks + 1;
        move_camera_between(machine, hints->home_camera_x, hints->home_camera_y, hints->goal_camera_x, hints->goal_camera_y);
        if (hints->phase_ticks >= hints->pan_ticks) {
            hints->phase = SCRIPT_HINT_HOLDING;
            hints->phase_ticks = 0;
        }
        break;
    case SCRIPT_HINT_HOLDING:
        hints->phase_ticks = hints->phase_ticks + 1;
        hints->flash_clock = hints->flash_clock + 1;
        if (hints->phase_ticks >= HOLD_TICKS) {
            hints->phase = SCRIPT_HINT_PANNING_BACK;
            hints->phase_ticks = 0;
        }
        break;
    case SCRIPT_HINT_PANNING_BACK:
        hints->phase_ticks = hints->phase_ticks + 1;
        move_camera_between(machine, hints->goal_camera_x, hints->goal_camera_y, hints->home_camera_x, hints->home_camera_y);
        if (hints->phase_ticks >= hints->pan_ticks) {
            hints->phase = SCRIPT_HINT_SHOWN;
            machine->pause_bit = 0;
            script_soul_start_player_mark(machine);
            finish_the_hint_when_its_step_is_done(machine);
        }
        break;
    case SCRIPT_HINT_SHOWN:
        hints->flash_clock = hints->flash_clock + 1;
        hints->pulse_clock = hints->pulse_clock + 1;
        if (hints->target_object >= machine->object_count || machine->objects[hints->target_object].is_deleted || !machine->objects[hints->target_object].actor.exists) {
            hints->phase = SCRIPT_HINT_SPENT;
            mark(machine, "hint_done");
        }
        break;
    default:
        break;
    }
}

static float rise_and_fall(int tick, int length)
{
    float along;

    along = (float)tick / (float)length;
    return along < 0.5f ? along * 2.0f : (1.0f - along) * 2.0f;
}

static float flash_alpha(const struct script_hints *hints)
{
    int tick;

    if (hints->phase != SCRIPT_HINT_HOLDING && hints->phase != SCRIPT_HINT_SHOWN) {
        return 0.0f;
    }
    tick = hints->flash_clock < FIRST_FLASHES * FLASH_TICKS ? hints->flash_clock % FLASH_TICKS : hints->flash_clock % FLASH_REPEATS_EVERY;
    return tick < FLASH_TICKS ? rise_and_fall(tick, FLASH_TICKS) : 0.0f;
}

void script_hints_report(struct script_machine *machine)
{
    const struct script_hints *hints;
    struct script_hint_view view;
    int x;
    int y;

    hints = &machine->hints;
    if (machine->outputs == NULL || machine->outputs->hint_changed == NULL) {
        return;
    }
    memset(&view, 0, sizeof(view));
    if (script_hints_bubble_is_up(machine)) {
        target_point(machine, machine->soul.active_object, machine->soul.active_slot, &x, &y);
        view.bubble_is_up = 1;
        view.bubble_tap_group = hints->tap_group;
        view.bubble_x = (float)x;
        view.bubble_y = (float)y;
    }
    if ((hints->phase == SCRIPT_HINT_HOLDING || hints->phase == SCRIPT_HINT_SHOWN) && hints->target_object >= 0 && hints->target_object < machine->object_count) {
        target_point(machine, hints->target_object, hints->target_slot, &x, &y);
        view.flash_object = machine->objects[hints->target_object].name;
        view.flash_alpha = machine->soul.is_on ? 0.0f : flash_alpha(hints);
        view.pulse_is_on = machine->soul.is_on && hints->phase == SCRIPT_HINT_SHOWN;
        view.pulse_x = (float)x;
        view.pulse_y = (float)y;
        view.pulse_level = rise_and_fall(hints->pulse_clock % PULSE_TICKS, PULSE_TICKS);
    }
    machine->outputs->hint_changed(&view);
}
