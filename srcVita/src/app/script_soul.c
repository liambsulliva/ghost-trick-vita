#include <math.h>
#include <stdio.h>
#include <string.h>
#include "script_machine.h"

#define SOUL_VISION_MESSAGE_AFTER_TICKS 90
#define DEMO_MODE 2
#define NORMAL_PLAY_MODE 3
#define PACED_TICKS_TO_READ_A_PAGE 120u
#define ROUTE_WAIT_MOST_TICKS 10800u
#define ROUTE_ACTION_MOST_TICKS 1800u
#define ROUTE_STEER_DEAD_ZONE 3.0f
#define SOUL_REACH_DS_PIXELS 48.0f
#define SOUL_TIP_DS_PIXELS_PER_TICK 6.0f
#define SOUL_TIP_PICKUP_NEAR 16.0f
#define SOUL_TIP_PICKUP_FAR 35.0f
#define SOUL_TIP_PICKUP_OFF_LINE 3.0f
#define SOUL_TIP_SHORTEST_PICKUP 1.0f
#define SOUL_TIP_LENGTH_FOR_SHORTEST_PICKUP 4.0f
#define SOUL_TIP_LENGTH_FOR_ANY_PICKUP 32.0f
#define NO_CORE (-1)
#define SOUL_BEAM_TICKS 6
#define SOUL_LIGHTNING_TICKS 72
#define SOUL_POSSESSION_FLASH_TICKS 18
#define SOUL_POSSESSION_SPARK_TICKS 148
#define SOUL_TOOLTIP_FADE_TICKS 24
#define SOUL_STREAM_DS_PIXELS_PER_TICK 6
#define SOUL_STREAM_TICKS_LEAST 8
#define SOUL_STREAM_TICKS_MOST 20
#define CAMERA_VIEW_WIDTH 256
#define CAMERA_VIEW_HEIGHT 192
#define CAMERA_VIEW_MARGIN 24
#define TRICK_ACTION_AFTER_TICKS 8
#define TIME_JUMP_AFTER_TICKS 30
#define TIME_JUMP_MOVIE "time_jump"
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
    soul->drag_stream_ticks = machine->objects[object_index].is_dead ? SOUL_LIGHTNING_TICKS : stream_ticks_for_distance(sqrtf((target_x - soul->drag_from_x) * (target_x - soul->drag_from_x) + (target_y - soul->drag_from_y) * (target_y - soul->drag_from_y)));
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
            } else if (machine->objects[soul->drag_object].is_dead) {
                machine->outputs->soul_cursor_moved(1, soul->drag_from_x, soul->drag_from_y, target_x, target_y, SCRIPT_SOUL_LIGHTNING);
            } else {
                float along;

                along = 1.0f - (float)soul->drag_ticks_left / (float)soul->drag_stream_ticks;
                machine->outputs->soul_cursor_moved(1, soul->drag_from_x + (target_x - soul->drag_from_x) * along, soul->drag_from_y + (target_y - soul->drag_from_y) * along, soul->drag_from_x, soul->drag_from_y, SCRIPT_SOUL_STREAM);
            }
        } else if (soul->is_on && soul->active_object != NO_CORE) {
            float x;
            float y;

            core_position(machine, soul->active_object, &x, &y);
            if (soul->tip_is_out) {
                machine->outputs->soul_cursor_moved(1, x, y, soul->tip_x, soul->tip_y, SCRIPT_SOUL_BEAM);
            } else {
                machine->outputs->soul_cursor_moved(1, x, y, x, y, SCRIPT_SOUL_RESTING);
            }
        } else {
            machine->outputs->soul_cursor_moved(0, 0.0f, 0.0f, 0.0f, 0.0f, SCRIPT_SOUL_RESTING);
        }
    }
    if (machine->outputs->soul_tooltip_changed != NULL) {
        float alpha;

        alpha = 1.0f;
        if (soul->drag_ticks_left > 0) {
            int elapsed;

            elapsed = SOUL_BEAM_TICKS + soul->drag_stream_ticks - soul->drag_ticks_left;
            alpha = machine->objects[soul->drag_object].is_dead && elapsed < SOUL_TOOLTIP_FADE_TICKS ? 1.0f - (float)elapsed / (float)SOUL_TOOLTIP_FADE_TICKS : 0.0f;
        }
        machine->outputs->soul_tooltip_changed(soul->is_on && soul->tooltip_is_on && alpha > 0.0f && soul->active_object != NO_CORE ? machine->objects[soul->active_object].name : NULL, alpha);
    }
    if (!soul->is_on || machine->outputs->soul_core_marked == NULL) {
        return;
    }
    for (index = 0; index < machine->object_count; index++) {
        if (machine->outputs->soul_object_styled != NULL && machine->objects[index].actor.exists) {
            machine->outputs->soul_object_styled(machine->objects[index].name, machine->objects[index].actor.class_id, machine->objects[index].actor.has_core, index == soul->active_object, machine->objects[index].is_dead);
        }
        if (index != soul->active_object && object_can_be_a_core(&machine->objects[index], 0)) {
            float x;
            float y;

            core_position(machine, index, &x, &y);
            machine->outputs->soul_core_marked(index, x, y, machine->objects[index].is_dead);
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
        if (machine->objects[soul->drag_object].is_dead) {
            soul->flash_ticks_left = SOUL_POSSESSION_FLASH_TICKS;
            soul->sparks_elapsed_ticks = 1;
        }
        arrive_at_core(machine, soul->drag_object, soul->drag_slot);
    }
}

static void report_possession_effects(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (machine->outputs->soul_flash_changed != NULL) {
        machine->outputs->soul_flash_changed((float)soul->flash_ticks_left / (float)SOUL_POSSESSION_FLASH_TICKS);
    }
    if (machine->outputs->soul_sparks_changed != NULL && soul->active_object != NO_CORE) {
        float x;
        float y;

        core_position(machine, soul->active_object, &x, &y);
        machine->outputs->soul_sparks_changed(soul->sparks_elapsed_ticks, x, y);
    }
    if (soul->flash_ticks_left > 0) {
        soul->flash_ticks_left = soul->flash_ticks_left - 1;
    }
    if (soul->sparks_elapsed_ticks > 0) {
        soul->sparks_elapsed_ticks = soul->sparks_elapsed_ticks >= SOUL_POSSESSION_SPARK_TICKS ? 0 : soul->sparks_elapsed_ticks + 1;
    }
}

static int nearest_core_to_camera(const struct script_machine *machine)
{
    float camera_x;
    float camera_y;
    float best;
    int best_object;
    int object_index;

    camera_x = (float)machine->camera_x + (float)CAMERA_VIEW_WIDTH / 2.0f;
    camera_y = (float)machine->camera_y + (float)CAMERA_VIEW_HEIGHT / 2.0f;
    best = 0.0f;
    best_object = NO_CORE;
    for (object_index = 0; object_index < machine->object_count; object_index++) {
        float x;
        float y;
        float distance;

        if (!object_can_be_a_core(&machine->objects[object_index], 0)) {
            continue;
        }
        core_position(machine, object_index, &x, &y);
        distance = (x - camera_x) * (x - camera_x) + (y - camera_y) * (y - camera_y);
        if (best_object == NO_CORE || distance < best) {
            best = distance;
            best_object = object_index;
        }
    }
    return best_object;
}

static int core_is_within_tip_pickup(float root_x, float root_y, float tip_x, float tip_y, float core_x, float core_y)
{
    float along_x;
    float along_y;
    float length_squared;
    float fraction;
    float off_x;
    float off_y;
    float distance;

    distance = sqrtf((core_x - tip_x) * (core_x - tip_x) + (core_y - tip_y) * (core_y - tip_y));
    if (distance <= SOUL_TIP_PICKUP_NEAR) {
        return 1;
    }
    if (distance > SOUL_TIP_PICKUP_FAR) {
        return 0;
    }
    along_x = tip_x - root_x;
    along_y = tip_y - root_y;
    length_squared = along_x * along_x + along_y * along_y;
    if (length_squared <= 0.0f) {
        return 0;
    }
    fraction = ((core_x - root_x) * along_x + (core_y - root_y) * along_y) / length_squared;
    if (fraction < 0.0f || fraction > 1.0f) {
        return 0;
    }
    off_x = root_x + along_x * fraction - core_x;
    off_y = root_y + along_y * fraction - core_y;
    return sqrtf(off_x * off_x + off_y * off_y) <= SOUL_TIP_PICKUP_OFF_LINE;
}

static int short_tip_cannot_pick_up(float tip_length, float distance)
{
    float pickup;
    float length_needed;

    pickup = SOUL_TIP_SHORTEST_PICKUP;
    length_needed = SOUL_TIP_LENGTH_FOR_SHORTEST_PICKUP;
    while (length_needed <= SOUL_TIP_LENGTH_FOR_ANY_PICKUP) {
        if (tip_length < length_needed && distance > pickup) {
            return 1;
        }
        pickup = pickup * 2.0f;
        length_needed = length_needed * 2.0f;
    }
    return 0;
}

static int core_under_the_tip(const struct script_machine *machine, float root_x, float root_y, int *found_slot)
{
    const struct script_soul *soul;
    float best;
    int best_object;
    int object_index;

    soul = &machine->soul;
    best = 0.0f;
    best_object = NO_CORE;
    for (object_index = 0; object_index < machine->object_count; object_index++) {
        float x;
        float y;
        float distance;
        int slot;

        if (object_index == soul->active_object) {
            continue;
        }
        for (slot = 0; slot < SCRIPT_CORE_SLOT_COUNT; slot++) {
            if (object_can_be_a_core(&machine->objects[object_index], slot)) {
                break;
            }
        }
        if (slot == SCRIPT_CORE_SLOT_COUNT) {
            continue;
        }
        core_position(machine, object_index, &x, &y);
        if (!core_is_within_tip_pickup(root_x, root_y, soul->tip_x, soul->tip_y, x, y)) {
            continue;
        }
        distance = sqrtf((x - soul->tip_x) * (x - soul->tip_x) + (y - soul->tip_y) * (y - soul->tip_y));
        if (short_tip_cannot_pick_up(sqrtf((soul->tip_x - root_x) * (soul->tip_x - root_x) + (soul->tip_y - root_y) * (soul->tip_y - root_y)), distance)) {
            continue;
        }
        if (best_object == NO_CORE || distance < best) {
            best = distance;
            best_object = object_index;
            *found_slot = slot;
        }
    }
    return best_object;
}

static void clamp_tip_to_reach(struct script_soul *soul, float root_x, float root_y)
{
    float along_x;
    float along_y;
    float length;

    along_x = soul->tip_x - root_x;
    along_y = soul->tip_y - root_y;
    length = sqrtf(along_x * along_x + along_y * along_y);
    if (length > SOUL_REACH_DS_PIXELS) {
        soul->tip_x = root_x + along_x * SOUL_REACH_DS_PIXELS / length;
        soul->tip_y = root_y + along_y * SOUL_REACH_DS_PIXELS / length;
    }
}

static void move_soul_tip(struct script_machine *machine)
{
    struct script_soul *soul;
    float root_x;
    float root_y;
    int found_object;
    int found_slot;

    soul = &machine->soul;
    if (!soul->is_on || soul->active_object == NO_CORE || soul->drag_ticks_left > 0 || (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent)) {
        soul->tip_is_out = 0;
        return;
    }
    if (soul->held_x == 0 && soul->held_y == 0) {
        soul->tip_is_out = 0;
        return;
    }
    core_position(machine, soul->active_object, &root_x, &root_y);
    if (!soul->tip_is_out) {
        soul->tip_x = root_x;
        soul->tip_y = root_y;
        soul->tip_is_out = 1;
    }
    soul->tip_x = soul->tip_x + (float)soul->held_x * SOUL_TIP_DS_PIXELS_PER_TICK;
    soul->tip_y = soul->tip_y + (float)soul->held_y * SOUL_TIP_DS_PIXELS_PER_TICK;
    clamp_tip_to_reach(soul, root_x, root_y);
    found_slot = 0;
    found_object = core_under_the_tip(machine, root_x, root_y, &found_slot);
    if (found_object != NO_CORE) {
        soul->tip_is_out = 0;
        drag_soul_to(machine, found_object, found_slot);
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
    soul->tricks_fired = soul->tricks_fired + 1;
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("trick", object->name);
    }
    if (object->is_dead) {
        if (machine->outputs->swirl_changed != NULL) {
            machine->outputs->swirl_changed(1);
        }
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
    } else if (strcmp(verb, "Timeleap") == 0 && strcmp(arg, "BREAK") == 0) {
        if (machine->outputs->swirl_changed != NULL) {
            machine->outputs->swirl_changed(0);
        }
    } else if (strcmp(verb, "Timeleap") == 0 && strcmp(arg, "SELECT") == 0) {
        if (machine->outputs->movie_started != NULL && machine->time_jump_movie_ticks > 0) {
            machine->outputs->movie_started(TIME_JUMP_MOVIE);
        }
        schedule_native_event(machine, &machine->soul.time_jump_event, "timeleap", "jump", machine->time_jump_movie_ticks > 0 ? machine->time_jump_movie_ticks : TIME_JUMP_AFTER_TICKS);
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

static void take_direction(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent) {
        player_moved_in_tutorial(machine);
        return;
    }
    if (soul->tutorial_mode == TUTORIAL_NONE && !soul->is_on && script_soul_world_is_playable(machine)) {
        soul->player_opened_soul_view = 1;
        set_soul_view(machine, 1);
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
            int first_core;

            first_core = nearest_core_to_camera(machine);
            if (first_core != NO_CORE) {
                arrive_at_core(machine, first_core, 0);
            }
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

static int object_named(const struct script_machine *machine, const char *name)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        if (strcmp(machine->objects[index].name, name) == 0) {
            return index;
        }
    }
    return NO_CORE;
}

static const struct script_route_step *current_route_step(const struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    if (machine->route == NULL || soul->route_script == NULL || soul->route_step < 0 || soul->route_step >= machine->route->step_count) {
        return NULL;
    }
    if (strcmp(machine->route->steps[soul->route_step].script, soul->route_script) != 0) {
        return NULL;
    }
    return &machine->route->steps[soul->route_step];
}

static void mark_route_step(struct script_machine *machine, const char *what, const struct script_route_step *step)
{
    char detail[SCRIPT_ROUTE_NAME_MAX + 32];

    if (machine->outputs->marked == NULL) {
        return;
    }
    snprintf(detail, sizeof(detail), "%d:%s:%s", machine->soul.route_step, script_route_action_name(step->action), step->object[0] != '\0' ? step->object : "-");
    machine->outputs->marked(what, detail);
}

static void go_to_route_step(struct script_machine *machine, int step_index, int dwell_ticks)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->route_step = step_index;
    soul->route_step_began_at = machine->tick;
    soul->route_dwell_until = machine->tick + (machine->auto_press == SCRIPT_AUTO_FAST ? 0u : (unsigned int)dwell_ticks);
    soul->route_wait_is_over = 0;
    soul->route_tricks_when_step_began = soul->tricks_fired;
}

static void finish_route_step(struct script_machine *machine, const struct script_route_step *step)
{
    const struct script_route_step *next;
    int dwell_ticks;

    mark_route_step(machine, "route_step_done", step);
    machine->last_new_site_tick = machine->tick;
    next = machine->soul.route_step + 1 < machine->route->step_count ? &machine->route->steps[machine->soul.route_step + 1] : NULL;
    dwell_ticks = next != NULL ? next->dwell_ticks : 0;
    go_to_route_step(machine, machine->soul.route_step + 1, dwell_ticks);
}

static int route_act_was_seen(const struct script_soul *soul, const char *act_name)
{
    int index;

    for (index = 0; index < soul->acts_seen_count; index++) {
        if (strcmp(soul->acts_seen[index], act_name) == 0) {
            return 1;
        }
    }
    return 0;
}

static int route_wait_is_satisfied(const struct script_machine *machine, const struct script_route_step *step)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    switch (step->wait) {
    case SCRIPT_ROUTE_WAIT_PLAYABLE:
        return script_soul_world_is_playable(machine);
    case SCRIPT_ROUTE_WAIT_TUTORIAL_TOUCH:
        return soul->tutorial_mode == TUTORIAL_TOUCH_CHECK;
    case SCRIPT_ROUTE_WAIT_TUTORIAL_MOVE:
        return soul->tutorial_mode == TUTORIAL_MOVE;
    case SCRIPT_ROUTE_WAIT_ACT:
        return route_act_was_seen(soul, step->wait_name);
    case SCRIPT_ROUTE_WAIT_CORE:
        return machine->active_core_name != NULL && strcmp(machine->active_core_name, step->wait_name) == 0;
    default:
        return 1;
    }
}

static void steer_tip_toward_object(struct script_machine *machine, int object_index)
{
    struct script_soul *soul;
    float from_x;
    float from_y;
    float target_x;
    float target_y;

    soul = &machine->soul;
    if (soul->tip_is_out) {
        from_x = soul->tip_x;
        from_y = soul->tip_y;
    } else {
        core_position(machine, soul->active_object, &from_x, &from_y);
    }
    core_position(machine, object_index, &target_x, &target_y);
    soul->held_x = target_x > from_x + ROUTE_STEER_DEAD_ZONE ? 1 : (target_x < from_x - ROUTE_STEER_DEAD_ZONE ? -1 : 0);
    soul->held_y = target_y > from_y + ROUTE_STEER_DEAD_ZONE ? 1 : (target_y < from_y - ROUTE_STEER_DEAD_ZONE ? -1 : 0);
}

static int run_route_move(struct script_machine *machine, const struct script_route_step *step)
{
    struct script_soul *soul;
    int object_index;

    soul = &machine->soul;
    object_index = object_named(machine, step->object);
    if (object_index == NO_CORE || !object_can_be_a_core(&machine->objects[object_index], step->slot)) {
        return 0;
    }
    if (soul->active_object == object_index && soul->drag_ticks_left == 0) {
        soul->active_slot = step->slot;
        return 1;
    }
    if (!soul->is_on) {
        if (script_soul_world_is_playable(machine)) {
            soul->ghost_is_pending = 1;
        }
        return 0;
    }
    if (soul->drag_ticks_left == 0 && soul->active_object != NO_CORE) {
        steer_tip_toward_object(machine, object_index);
    }
    return 0;
}

static int run_route_action(struct script_machine *machine, const struct script_route_step *step)
{
    struct script_soul *soul;

    soul = &machine->soul;
    switch (step->action) {
    case SCRIPT_ROUTE_MOVE:
        return run_route_move(machine, step);
    case SCRIPT_ROUTE_TRICK:
        if (soul->tricks_fired != soul->route_tricks_when_step_began) {
            return 1;
        }
        if (machine->active_core_name != NULL && strcmp(machine->active_core_name, step->object) == 0 && soul->drag_ticks_left == 0) {
            machine->press_is_pending = 1;
        }
        return 0;
    case SCRIPT_ROUTE_GHOST_ON:
        if (soul->is_on) {
            return 1;
        }
        if (script_soul_world_is_playable(machine)) {
            soul->ghost_is_pending = 1;
        }
        return 0;
    case SCRIPT_ROUTE_GHOST_OFF:
        if (!soul->is_on) {
            return 1;
        }
        soul->ghost_is_pending = 1;
        return 0;
    case SCRIPT_ROUTE_TAP_LEFT:
    case SCRIPT_ROUTE_TAP_RIGHT:
        if (!script_balloons_can_be_tapped(machine, step->action == SCRIPT_ROUTE_TAP_LEFT ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT)) {
            return 0;
        }
        soul->tap_group_is_pending = step->action == SCRIPT_ROUTE_TAP_LEFT ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT;
        return 1;
    default:
        return 1;
    }
}

static void give_up_on_route_step(struct script_machine *machine, const struct script_route_step *step)
{
    struct script_soul *soul;
    int object_index;

    soul = &machine->soul;
    mark_route_step(machine, "route_step_failed", step);
    if (step->action == SCRIPT_ROUTE_MOVE && soul->is_on) {
        object_index = object_named(machine, step->object);
        if (object_index != NO_CORE && object_can_be_a_core(&machine->objects[object_index], step->slot)) {
            mark_route_step(machine, "route_hop_beyond_reach", step);
            arrive_at_core(machine, object_index, step->slot);
        }
    }
    finish_route_step(machine, step);
}

static void run_route(struct script_machine *machine)
{
    struct script_soul *soul;
    const struct script_route_step *step;

    soul = &machine->soul;
    step = current_route_step(machine);
    if (step == NULL || machine->tick < soul->route_dwell_until) {
        return;
    }
    if (!soul->route_wait_is_over) {
        if (!route_wait_is_satisfied(machine, step)) {
            if (step->wait != SCRIPT_ROUTE_WAIT_PLAYABLE && machine->tick - soul->route_step_began_at > ROUTE_WAIT_MOST_TICKS) {
                give_up_on_route_step(machine, step);
            }
            return;
        }
        soul->route_wait_is_over = 1;
        soul->route_step_began_at = machine->tick;
    }
    if (run_route_action(machine, step)) {
        finish_route_step(machine, step);
    } else if (machine->tick - soul->route_step_began_at > ROUTE_ACTION_MOST_TICKS) {
        give_up_on_route_step(machine, step);
    }
}

void script_soul_script_began(struct script_machine *machine, const char *script)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->route_script = script;
    soul->acts_seen_count = 0;
    soul->route_step = machine->route != NULL ? script_route_first_step_of_script(machine->route, script) : -1;
    soul->route_step_began_at = machine->tick;
    soul->route_dwell_until = machine->tick;
    soul->route_wait_is_over = 0;
    soul->route_tricks_when_step_began = soul->tricks_fired;
    if (machine->auto_press != SCRIPT_AUTO_OFF && machine->route != NULL && soul->route_step < 0 && machine->outputs->marked != NULL) {
        machine->outputs->marked("route_has_no_steps", script);
    }
}

static void skip_route_ahead_to_act(struct script_machine *machine, const char *act_name)
{
    const struct script_soul *soul;
    int index;

    soul = &machine->soul;
    if (current_route_step(machine) == NULL) {
        return;
    }
    for (index = soul->route_step + 1; index < machine->route->step_count; index++) {
        const struct script_route_step *step;

        step = &machine->route->steps[index];
        if (strcmp(step->script, soul->route_script) != 0) {
            return;
        }
        if (step->wait == SCRIPT_ROUTE_WAIT_ACT && strcmp(step->wait_name, act_name) == 0) {
            mark_route_step(machine, "route_resynced_past", &machine->route->steps[soul->route_step]);
            go_to_route_step(machine, index, step->dwell_ticks);
            return;
        }
    }
}

void script_soul_act_began(struct script_machine *machine, const char *act_name)
{
    struct script_soul *soul;
    const struct script_route_step *step;

    soul = &machine->soul;
    if (soul->acts_seen_count < SCRIPT_ROUTE_ACTS_SEEN_MAX) {
        soul->acts_seen[soul->acts_seen_count] = act_name;
        soul->acts_seen_count = soul->acts_seen_count + 1;
    }
    step = current_route_step(machine);
    if (step != NULL && !(step->wait == SCRIPT_ROUTE_WAIT_ACT && strcmp(step->wait_name, act_name) == 0)) {
        skip_route_ahead_to_act(machine, act_name);
    }
}

static void play_automatically(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->held_x = 0;
    soul->held_y = 0;
    if ((machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) || script_narration_wants_a_press(&machine->narration)) {
        press_when_the_page_has_been_read(machine);
        return;
    }
    soul->auto_reading_ticks = 0;
    if (soul->camera_tutorial_is_on) {
        script_machine_scroll_camera(machine, CAMERA_FREE_SPEED_PIXELS, CAMERA_FREE_SPEED_PIXELS);
        return;
    }
    if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent) {
        machine->press_is_pending = 1;
        return;
    }
    run_route(machine);
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
        take_direction(machine);
        soul->direction_x = 0;
        soul->direction_y = 0;
    }
    move_soul_tip(machine);
    move_free_camera(machine);
    finish_drag_when_due(machine);
    report_soul_view(machine);
    report_possession_effects(machine);
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
    machine->soul.route_step = -1;
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
