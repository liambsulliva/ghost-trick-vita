#include <math.h>
#include <stdio.h>
#include <string.h>
#include "script_machine.h"

#define SOUL_VISION_MESSAGE_AFTER_TICKS 90
#define DEMO_MODE 2
#define NORMAL_PLAY_MODE 3
#define AUTO_TRICK_TICKS 30u
#define AUTO_TUTORIAL_TICKS 3u
#define PACED_TUTORIAL_TICKS 30u
#define PACED_TICKS_TO_READ_A_PAGE 120u
#define NO_CORE (-1)
#define SOUL_BEAM_TICKS 6
#define SOUL_STREAM_DS_PIXELS_PER_TICK 6
#define SOUL_STREAM_TICKS_LEAST 8
#define SOUL_STREAM_TICKS_MOST 20
#define CAMERA_VIEW_WIDTH 256
#define CAMERA_VIEW_HEIGHT 192
#define CAMERA_VIEW_MARGIN 24
#define PACED_SOUL_VIEW_TICKS 120u
#define PACED_LIVING_WORLD_TICKS 300u
#define TRICK_ACTION_AFTER_TICKS 8
#define TIME_JUMP_AFTER_TICKS 30
#define COUNTDOWN_STEP_TICKS 90
#define COUNTDOWN_LAST_STEP 3
#define PHONE_EFFECT_ID 49
#define PHONE_EFFECT_EVENT_AFTER_TICKS 30
#define CAMERA_FREE_SPEED_PIXELS 4

enum tutorial_mode {
    TUTORIAL_NONE,
    TUTORIAL_TOUCH_CHECK,
    TUTORIAL_MOVE
};

static const char *const core_verbs[SCRIPT_CORE_SLOT_COUNT] = { "core", "core1" };

static int handler_exists(const struct script_object *object, const char *verb, const char *arg)
{
    int index;

    for (index = 0; index < object->handler_count; index++) {
        const struct script_handler *handler;

        handler = &object->handlers[index];
        if (strcmp(handler->verb, verb) == 0 && (arg == NULL || handler->arg[0] == '\0' || strcmp(handler->arg, arg) == 0)) {
            return 1;
        }
    }
    return 0;
}

static int object_has_a_trick(const struct script_object *object, int slot)
{
    return handler_exists(object, core_verbs[slot], NULL) || (slot == 0 && object->is_dead);
}

static int object_can_be_a_core(const struct script_object *object, int slot)
{
    if (!object->actor.exists || object->actor.is_hard_hidden || object->actor.is_frozen_and_hidden) {
        return 0;
    }
    return object_has_a_trick(object, slot) || (slot == 0 && object->actor.has_core);
}

static void player_moved_in_tutorial(struct script_machine *machine);

static void core_position(const struct script_machine *machine, int object_index, float *x, float *y)
{
    const struct script_actor *actor;

    actor = &machine->objects[object_index].actor;
    *x = (float)(actor->x + actor->core_x);
    *y = (float)(actor->y + actor->core_y);
}

static void arrive_at_core(struct script_machine *machine, int object_index, int slot)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->active_object = object_index;
    soul->active_slot = slot;
    machine->active_core_name = machine->objects[object_index].name;
    soul->tooltip_is_on = soul->is_on;
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("core", machine->objects[object_index].name);
    }
    if (soul->is_on) {
        script_machine_send(machine, "*", "SYSTEM", "SV_CHGCORE");
    }
    player_moved_in_tutorial(machine);
}

static int stream_ticks_for_distance(float distance)
{
    int ticks;

    ticks = (int)(distance / (float)SOUL_STREAM_DS_PIXELS_PER_TICK);
    if (ticks < SOUL_STREAM_TICKS_LEAST) {
        return SOUL_STREAM_TICKS_LEAST;
    }
    return ticks > SOUL_STREAM_TICKS_MOST ? SOUL_STREAM_TICKS_MOST : ticks;
}

static int point_is_in_camera_view(const struct script_machine *machine, float x, float y)
{
    return x >= (float)(machine->camera_x - CAMERA_VIEW_MARGIN) && x <= (float)(machine->camera_x + CAMERA_VIEW_WIDTH + CAMERA_VIEW_MARGIN) && y >= (float)(machine->camera_y - CAMERA_VIEW_MARGIN) && y <= (float)(machine->camera_y + CAMERA_VIEW_HEIGHT + CAMERA_VIEW_MARGIN);
}

static void drag_soul_to(struct script_machine *machine, int object_index, int slot)
{
    struct script_soul *soul;
    float target_x;
    float target_y;

    soul = &machine->soul;
    if (soul->drag_ticks_left > 0) {
        return;
    }
    if (machine->auto_press == SCRIPT_AUTO_FAST || !soul->is_on || soul->active_object == NO_CORE || soul->active_object == object_index) {
        arrive_at_core(machine, object_index, slot);
        return;
    }
    core_position(machine, soul->active_object, &soul->drag_from_x, &soul->drag_from_y);
    core_position(machine, object_index, &target_x, &target_y);
    soul->drag_object = object_index;
    soul->drag_slot = slot;
    soul->drag_stream_ticks = stream_ticks_for_distance(sqrtf((target_x - soul->drag_from_x) * (target_x - soul->drag_from_x) + (target_y - soul->drag_from_y) * (target_y - soul->drag_from_y)));
    soul->drag_ticks_left = SOUL_BEAM_TICKS + soul->drag_stream_ticks;
    if (soul->player_opened_soul_view && !soul->camera_follow_is_off && !point_is_in_camera_view(machine, target_x, target_y)) {
        script_machine_pan_camera_to(machine, (int)target_x, (int)target_y);
    }
}

static void report_soul_view(struct script_machine *machine)
{
    struct script_soul *soul;
    int index;

    soul = &machine->soul;
    if (machine->outputs->soul_cursor_moved != NULL) {
        if (soul->is_on && soul->drag_ticks_left > 0) {
            float target_x;
            float target_y;

            core_position(machine, soul->drag_object, &target_x, &target_y);
            if (soul->drag_ticks_left > soul->drag_stream_ticks) {
                machine->outputs->soul_cursor_moved(1, soul->drag_from_x, soul->drag_from_y, target_x, target_y, SCRIPT_SOUL_BEAM);
            } else {
                float along;

                along = 1.0f - (float)soul->drag_ticks_left / (float)soul->drag_stream_ticks;
                machine->outputs->soul_cursor_moved(1, soul->drag_from_x + (target_x - soul->drag_from_x) * along, soul->drag_from_y + (target_y - soul->drag_from_y) * along, soul->drag_from_x, soul->drag_from_y, SCRIPT_SOUL_STREAM);
            }
        } else if (soul->is_on && soul->active_object != NO_CORE) {
            float x;
            float y;

            core_position(machine, soul->active_object, &x, &y);
            machine->outputs->soul_cursor_moved(1, x, y, x, y, SCRIPT_SOUL_RESTING);
        } else {
            machine->outputs->soul_cursor_moved(0, 0.0f, 0.0f, 0.0f, 0.0f, SCRIPT_SOUL_RESTING);
        }
    }
    if (machine->outputs->soul_tooltip_changed != NULL) {
        machine->outputs->soul_tooltip_changed(soul->is_on && soul->tooltip_is_on && soul->drag_ticks_left == 0 && soul->active_object != NO_CORE ? machine->objects[soul->active_object].name : NULL);
    }
    if (!soul->is_on || machine->outputs->soul_core_marked == NULL) {
        return;
    }
    for (index = 0; index < machine->object_count; index++) {
        if (machine->outputs->soul_object_styled != NULL && machine->objects[index].actor.exists) {
            machine->outputs->soul_object_styled(machine->objects[index].name, machine->objects[index].actor.class_id, machine->objects[index].actor.has_core, index == soul->active_object);
        }
        if (index != soul->active_object && object_can_be_a_core(&machine->objects[index], 0)) {
            float x;
            float y;

            core_position(machine, index, &x, &y);
            machine->outputs->soul_core_marked(index, x, y);
        }
    }
}

static void finish_drag_when_due(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->drag_ticks_left <= 0) {
        return;
    }
    soul->drag_ticks_left = soul->drag_ticks_left - 1;
    if (soul->drag_ticks_left == 0) {
        arrive_at_core(machine, soul->drag_object, soul->drag_slot);
    }
}

static int find_nearest_untried_core(struct script_machine *machine, int *found_object, int *found_slot)
{
    const struct script_soul *soul;
    float from_x;
    float from_y;
    float best;
    int object_index;
    int slot;
    int is_found;

    soul = &machine->soul;
    from_x = 0.0f;
    from_y = 0.0f;
    if (soul->active_object != NO_CORE) {
        core_position(machine, soul->active_object, &from_x, &from_y);
    }
    best = 0.0f;
    is_found = 0;
    for (object_index = 0; object_index < machine->object_count; object_index++) {
        for (slot = 0; slot < SCRIPT_CORE_SLOT_COUNT; slot++) {
            float x;
            float y;
            float distance;

            if (!object_can_be_a_core(&machine->objects[object_index], slot) || soul->tried_cores[object_index][slot] || (object_index == soul->active_object && slot == soul->active_slot)) {
                continue;
            }
            core_position(machine, object_index, &x, &y);
            distance = (x - from_x) * (x - from_x) + (y - from_y) * (y - from_y);
            if (!is_found || distance < best) {
                best = distance;
                *found_object = object_index;
                *found_slot = slot;
                is_found = 1;
            }
        }
    }
    return is_found;
}

static void step_to_next_core(struct script_machine *machine)
{
    struct script_soul *soul;
    int object_index;
    int slot;

    soul = &machine->soul;
    if (!find_nearest_untried_core(machine, &object_index, &slot)) {
        memset(soul->tried_cores, 0, sizeof(soul->tried_cores));
        if (!find_nearest_untried_core(machine, &object_index, &slot)) {
            return;
        }
    }
    soul->tried_cores[object_index][slot] = 1;
    drag_soul_to(machine, object_index, slot);
}

static long long off_axis_cost(int along, int across)
{
    return (long long)along * along + 4LL * (long long)across * across;
}

static void step_to_core_in_direction(struct script_machine *machine, int direction_x, int direction_y)
{
    const struct script_soul *soul;
    int from_x;
    int from_y;
    int object_index;
    int best_object;
    int best_slot;
    long long best_cost;

    soul = &machine->soul;
    if (soul->active_object == NO_CORE) {
        step_to_next_core(machine);
        return;
    }
    from_x = machine->objects[soul->active_object].actor.x + machine->objects[soul->active_object].actor.core_x;
    from_y = machine->objects[soul->active_object].actor.y + machine->objects[soul->active_object].actor.core_y;
    best_object = NO_CORE;
    best_slot = 0;
    best_cost = 0;
    for (object_index = 0; object_index < machine->object_count; object_index++) {
        const struct script_object *object;
        int slot;

        object = &machine->objects[object_index];
        for (slot = 0; slot < SCRIPT_CORE_SLOT_COUNT; slot++) {
            int along;
            int across;
            long long cost;

            if (!object_can_be_a_core(object, slot) || object_index == soul->active_object) {
                continue;
            }
            along = (object->actor.x + object->actor.core_x - from_x) * direction_x + (object->actor.y + object->actor.core_y - from_y) * direction_y;
            across = (object->actor.x + object->actor.core_x - from_x) * direction_y - (object->actor.y + object->actor.core_y - from_y) * direction_x;
            if (along <= 0) {
                continue;
            }
            cost = off_axis_cost(along, across);
            if (best_object == NO_CORE || cost < best_cost) {
                best_object = object_index;
                best_slot = slot;
                best_cost = cost;
            }
            break;
        }
    }
    if (best_object != NO_CORE) {
        drag_soul_to(machine, best_object, best_slot);
    }
}

static void fire_trick(struct script_machine *machine)
{
    struct script_soul *soul;
    const struct script_object *object;

    soul = &machine->soul;
    if (soul->active_object == NO_CORE || soul->action_is_due_in > 0) {
        return;
    }
    object = &machine->objects[soul->active_object];
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("trick", object->name);
    }
    if (object->is_dead) {
        script_machine_send(machine, "*", "timeleap", "start");
        return;
    }
    script_machine_send(machine, object->name, core_verbs[soul->active_slot], "ready");
    soul->action_is_due_in = TRICK_ACTION_AFTER_TICKS;
    soul->action_object = soul->active_object;
    soul->action_slot = soul->active_slot;
}

static void send_trick_action_when_due(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->action_is_due_in <= 0 || machine->pause_bit) {
        return;
    }
    soul->action_is_due_in = soul->action_is_due_in - 1;
    if (soul->action_is_due_in == 0) {
        script_machine_send(machine, machine->objects[soul->action_object].name, core_verbs[soul->action_slot], "action");
    }
}

static void run_countdown(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (!soul->countdown_is_running || machine->pause_bit || machine->mode_byte != NORMAL_PLAY_MODE) {
        return;
    }
    soul->countdown_ticks = soul->countdown_ticks - 1;
    if (soul->countdown_ticks > 0) {
        return;
    }
    soul->countdown_step = soul->countdown_step + 1;
    if (soul->countdown_step == COUNTDOWN_LAST_STEP) {
        script_machine_send(machine, "*", "countdown", "zero");
        soul->countdown_ticks = COUNTDOWN_STEP_TICKS * 3 / 2;
    } else if (soul->countdown_step > COUNTDOWN_LAST_STEP) {
        script_machine_send(machine, "*", "countdown", "end");
        soul->countdown_is_running = 0;
    } else {
        soul->countdown_ticks = COUNTDOWN_STEP_TICKS;
    }
}

void script_soul_set_countdown(struct script_machine *machine, int is_on)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->countdown_is_running = is_on;
    soul->countdown_step = 0;
    soul->countdown_ticks = COUNTDOWN_STEP_TICKS;
}

static void send_timed_native_event(struct script_machine *machine, struct script_timed_event *event)
{
    if (event->target == NULL || event->at != (long long)machine->tick) {
        return;
    }
    script_machine_send(machine, event->target, event->verb, event->arg);
    event->target = NULL;
}

static void schedule_native_event(struct script_machine *machine, struct script_timed_event *event, const char *verb, const char *arg, int ticks)
{
    event->target = "*";
    event->verb = verb;
    event->arg = arg;
    event->at = (long long)machine->tick + ticks;
}

void script_soul_effect_spawned(struct script_machine *machine, int effect_id, const char *name)
{
    if (effect_id == PHONE_EFFECT_ID) {
        schedule_native_event(machine, &machine->soul.phone_event, name, "ready", PHONE_EFFECT_EVENT_AFTER_TICKS);
    }
}

void script_soul_effect_signalled(struct script_machine *machine, const char *name)
{
    schedule_native_event(machine, &machine->soul.phone_event, name, "end", PHONE_EFFECT_EVENT_AFTER_TICKS);
}

static void set_soul_view(struct script_machine *machine, int is_on)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->is_on = is_on;
    if (!is_on) {
        soul->player_opened_soul_view = 0;
        soul->tooltip_is_on = 0;
    }
    machine->soul_state_keeps_pause = is_on;
    machine->pause_bit = is_on;
    if (machine->outputs->soul_vision_changed != NULL) {
        machine->outputs->soul_vision_changed(is_on);
    }
    if (is_on) {
        soul->start_message_at = (long long)machine->tick + 1;
        machine->soul_sound_at = (long long)machine->tick + 1;
    }
}

static void enter_scripted_soul_view(struct script_machine *machine)
{
    set_soul_view(machine, 1);
    machine->soul_vision_message_at = (long long)machine->tick + SOUL_VISION_MESSAGE_AFTER_TICKS;
}

static void leave_soul_view(struct script_machine *machine)
{
    machine->soul.tutorial_mode = TUTORIAL_NONE;
    machine->mode_byte = DEMO_MODE;
    set_soul_view(machine, 0);
}

static void run_tutorial_command(struct script_machine *machine, const char *arg)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (strcmp(arg, "TouchCheck") == 0) {
        soul->tutorial_mode = TUTORIAL_TOUCH_CHECK;
        soul->tutorial_click_was_sent = 0;
    } else if (strcmp(arg, "MoveStart") == 0) {
        soul->tutorial_mode = TUTORIAL_MOVE;
    } else if (strcmp(arg, "TouchCheckEnd") == 0 || strcmp(arg, "MoveEnd") == 0) {
        soul->tutorial_mode = TUTORIAL_NONE;
    } else if (strcmp(arg, "ReturnGame") == 0) {
        leave_soul_view(machine);
    }
}

static void run_button_command(struct script_machine *machine, const char *arg)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (strcmp(arg, "All:Off") == 0) {
        soul->trick_is_on = 0;
    } else if (strcmp(arg, "All:On") == 0 || strcmp(arg, "Action:On") == 0 || strcmp(arg, "Trick:On") == 0) {
        soul->trick_is_on = 1;
    } else if (strcmp(arg, "Input:Off") == 0) {
        soul->input_is_off = 1;
    } else if (strcmp(arg, "Input:On") == 0) {
        soul->input_is_off = 0;
    } else if (strcmp(arg, "Camera:Off") == 0) {
        soul->camera_is_locked = 1;
    } else if (strcmp(arg, "Camera:On") == 0) {
        soul->camera_is_locked = 0;
    }
}

static void run_exception_command(struct script_machine *machine, const char *arg)
{
    if (strcmp(arg, "CoreHoseiOff") == 0) {
        machine->soul.camera_follow_is_off = 1;
    } else if (strcmp(arg, "CoreHoseiOn") == 0) {
        machine->soul.camera_follow_is_off = 0;
    } else if (strcmp(arg, "BreakTalk") == 0) {
        machine->pause_bit = 0;
        machine->soul_state_keeps_pause = 0;
    } else if (strcmp(arg, "ForceDemo") == 0) {
        machine->mode_byte = DEMO_MODE;
    }
}

static void run_custom_soul_command(struct script_machine *machine, const char *arg)
{
    if (strcmp(arg, "Start") == 0 || strcmp(arg, "NoEffectStart") == 0) {
        set_soul_view(machine, 1);
    } else if (strcmp(arg, "NoEffectFinish") == 0 || strcmp(arg, "ReturnDemo") == 0) {
        if (machine->soul.is_on) {
            set_soul_view(machine, 0);
        }
    }
}

void script_soul_run_command(struct script_machine *machine, const char *verb, const char *arg)
{
    if (strcmp(verb, "SoulVision") == 0) {
        enter_scripted_soul_view(machine);
    } else if (strcmp(verb, "Message") == 0) {
        script_talk_note_forward(machine);
    } else if (strcmp(verb, "SVTUTO") == 0) {
        run_tutorial_command(machine, arg);
    } else if (strcmp(verb, "Button") == 0) {
        run_button_command(machine, arg);
    } else if (strcmp(verb, "Exception") == 0) {
        run_exception_command(machine, arg);
    } else if (strcmp(verb, "CustomSoulVision") == 0) {
        run_custom_soul_command(machine, arg);
    } else if (strcmp(verb, "FreeCamera") == 0) {
        machine->soul.camera_tutorial_is_on = strcmp(arg, "TutorialStart") == 0;
    } else if (strcmp(verb, "Timeleap") == 0 && strcmp(arg, "SELECT") == 0) {
        schedule_native_event(machine, &machine->soul.time_jump_event, "timeleap", "jump", TIME_JUMP_AFTER_TICKS);
    }
}

static void player_moved_in_tutorial(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK) {
        if (!soul->tutorial_click_was_sent) {
            soul->tutorial_click_was_sent = 1;
            script_machine_send(machine, "*", "SVTUTO", "onClick");
            return;
        }
        script_machine_send(machine, "*", "SVTUTO", "onTouch");
    } else if (soul->tutorial_mode == TUTORIAL_MOVE) {
        script_machine_send(machine, "*", "SVTUTO", "move");
    }
}

int script_soul_world_is_playable(const struct script_machine *machine)
{
    return !machine->pause_bit && machine->mode_byte == NORMAL_PLAY_MODE && !machine->soul.input_is_off;
}

static void take_press(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) {
        script_talk_note_press(machine);
        return;
    }
    if (script_narration_wants_a_press(&machine->narration)) {
        script_narration_note_press(&machine->narration);
        return;
    }
    if (soul->tutorial_mode != TUTORIAL_NONE) {
        if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent) {
            player_moved_in_tutorial(machine);
        } else {
            step_to_next_core(machine);
        }
        return;
    }
    if (soul->is_on && soul->player_opened_soul_view) {
        set_soul_view(machine, 0);
        if (soul->trick_is_on) {
            fire_trick(machine);
        }
        return;
    }
    if (script_soul_world_is_playable(machine) && soul->trick_is_on) {
        fire_trick(machine);
    }
}

static void take_direction(struct script_machine *machine, int direction_x, int direction_y)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->tutorial_mode != TUTORIAL_NONE) {
        if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent) {
            player_moved_in_tutorial(machine);
        } else {
            step_to_core_in_direction(machine, direction_x, direction_y);
        }
        return;
    }
    if (!soul->is_on && script_soul_world_is_playable(machine)) {
        soul->player_opened_soul_view = 1;
        set_soul_view(machine, 1);
    }
    if (soul->is_on) {
        step_to_core_in_direction(machine, direction_x, direction_y);
    }
}

static void take_ghost_button(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->is_on && soul->player_opened_soul_view) {
        set_soul_view(machine, 0);
        return;
    }
    if (!soul->is_on && script_soul_world_is_playable(machine)) {
        soul->player_opened_soul_view = 1;
        set_soul_view(machine, 1);
        if (soul->active_object == NO_CORE) {
            step_to_next_core(machine);
        }
    }
}

static void move_free_camera(struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    if (soul->held_x == 0 && soul->held_y == 0) {
        return;
    }
    if (soul->is_on || soul->camera_is_locked || (machine->pause_bit && !soul->camera_tutorial_is_on)) {
        return;
    }
    script_machine_scroll_camera(machine, soul->held_x * CAMERA_FREE_SPEED_PIXELS, soul->held_y * CAMERA_FREE_SPEED_PIXELS);
}

static void press_when_the_page_has_been_read(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (machine->auto_press == SCRIPT_AUTO_FAST) {
        machine->press_is_pending = 1;
        return;
    }
    if (machine->talk.phase != SCRIPT_TALK_WAITING && !script_narration_wants_a_press(&machine->narration)) {
        soul->auto_reading_ticks = 0;
        return;
    }
    soul->auto_reading_ticks = soul->auto_reading_ticks + 1;
    if (soul->auto_reading_ticks >= PACED_TICKS_TO_READ_A_PAGE) {
        soul->auto_reading_ticks = 0;
        machine->press_is_pending = 1;
        machine->last_new_site_tick = machine->tick;
    }
}

static void play_automatically(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if ((machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) || script_narration_wants_a_press(&machine->narration)) {
        press_when_the_page_has_been_read(machine);
        return;
    }
    soul->auto_reading_ticks = 0;
    if (soul->camera_tutorial_is_on) {
        script_machine_scroll_camera(machine, CAMERA_FREE_SPEED_PIXELS, CAMERA_FREE_SPEED_PIXELS);
        return;
    }
    if (machine->tick < soul->next_auto_try_tick) {
        return;
    }
    if (soul->tutorial_mode != TUTORIAL_NONE) {
        soul->next_auto_try_tick = machine->tick + (machine->auto_press == SCRIPT_AUTO_FAST ? AUTO_TUTORIAL_TICKS : PACED_TUTORIAL_TICKS);
        machine->press_is_pending = 1;
        return;
    }
    if (machine->auto_press == SCRIPT_AUTO_FAST) {
        soul->next_auto_try_tick = machine->tick + AUTO_TRICK_TICKS;
    }
    if (soul->is_on && soul->player_opened_soul_view) {
        if (machine->auto_press == SCRIPT_AUTO_FAST) {
            step_to_next_core(machine);
            machine->press_is_pending = 1;
        } else if (!soul->auto_core_was_chosen) {
            soul->auto_core_was_chosen = 1;
            step_to_next_core(machine);
            soul->next_auto_try_tick = machine->tick + PACED_SOUL_VIEW_TICKS;
        } else if (soul->drag_ticks_left == 0) {
            soul->auto_core_was_chosen = 0;
            machine->press_is_pending = 1;
            soul->next_auto_try_tick = machine->tick + PACED_LIVING_WORLD_TICKS;
        }
        return;
    }
    if (machine->pause_bit) {
        return;
    }
    if (script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_RIGHT)) {
        soul->tap_group_is_pending = SCRIPT_TAP_GROUP_RIGHT;
        return;
    }
    if (script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_LEFT)) {
        soul->tap_group_is_pending = SCRIPT_TAP_GROUP_LEFT;
        return;
    }
    if (script_soul_world_is_playable(machine)) {
        soul->ghost_is_pending = 1;
        if (machine->auto_press != SCRIPT_AUTO_FAST) {
            soul->next_auto_try_tick = machine->tick + PACED_SOUL_VIEW_TICKS;
        }
    }
}

void script_soul_step_before_tasks(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->start_message_at == (long long)machine->tick) {
        soul->start_message_at = -1;
        script_machine_send(machine, "*", "SYSTEM", "SV_START");
    }
    if (machine->auto_press != SCRIPT_AUTO_OFF) {
        play_automatically(machine);
    }
    if (machine->press_is_pending) {
        machine->press_is_pending = 0;
        take_press(machine);
    }
    if (soul->ghost_is_pending) {
        soul->ghost_is_pending = 0;
        take_ghost_button(machine);
    }
    if (soul->tap_group_is_pending != 0) {
        if (!machine->pause_bit) {
            script_balloons_tap(machine, soul->tap_group_is_pending);
        }
        soul->tap_group_is_pending = 0;
    }
    if (soul->direction_x != 0 || soul->direction_y != 0) {
        take_direction(machine, soul->direction_x, soul->direction_y);
        soul->direction_x = 0;
        soul->direction_y = 0;
    }
    move_free_camera(machine);
    finish_drag_when_due(machine);
    report_soul_view(machine);
    send_trick_action_when_due(machine);
    run_countdown(machine);
    send_timed_native_event(machine, &soul->phone_event);
    send_timed_native_event(machine, &soul->time_jump_event);
}

void script_soul_note_ghost(struct script_machine *machine)
{
    machine->soul.ghost_is_pending = 1;
}

void script_soul_note_tap(struct script_machine *machine, int tap_group)
{
    machine->soul.tap_group_is_pending = tap_group;
}

void script_soul_note_direction(struct script_machine *machine, int direction_x, int direction_y)
{
    machine->soul.direction_x = direction_x;
    machine->soul.direction_y = direction_y;
}

void script_soul_set_held(struct script_machine *machine, int held_x, int held_y)
{
    machine->soul.held_x = held_x;
    machine->soul.held_y = held_y;
}

void script_soul_reset(struct script_machine *machine)
{
    memset(&machine->soul, 0, sizeof(machine->soul));
    machine->soul.active_object = NO_CORE;
    machine->soul.start_message_at = -1;
    machine->soul.trick_is_on = 1;
    machine->active_core_name = NULL;
}

void script_soul_set_first_core(struct script_machine *machine, int object_index)
{
    machine->soul.active_object = object_index;
    machine->soul.active_slot = 0;
    machine->active_core_name = machine->objects[object_index].name;
}

int script_soul_waits_on_the_player(const struct script_machine *machine)
{
    return machine->soul.tutorial_mode != TUTORIAL_NONE || machine->soul.camera_tutorial_is_on || machine->soul.player_opened_soul_view || script_narration_wants_a_press(&machine->narration);
}
