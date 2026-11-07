#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_machine.h"

#define SOUL_VISION_MESSAGE_AFTER_TICKS 90
#define DEMO_MODE 2
#define NORMAL_PLAY_MODE 3
#define PACED_TICKS_TO_READ_A_PAGE 120u
#define ROUTE_WAIT_MOST_TICKS 30000u
#define FIRST_CORE_DISABLED_BIT 1
#define SECOND_CORE_DISABLED_BIT 2
#define ROUTE_ACTION_MOST_TICKS 1800u
#define SOUL_REACH_DS_PIXELS 48.0f
#define SOUL_TIP_DS_PIXELS_PER_TICK 6.0f
#define SOUL_TIP_TOUCH_DS_PIXELS_PER_TICK 12.0f
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
#define SOUL_STREAM_DS_PIXELS_PER_TICK 6
#define SOUL_STREAM_TICKS_LEAST 8
#define SOUL_STREAM_TICKS_MOST 20
#define CAMERA_VIEW_WIDTH 256
#define CAMERA_VIEW_HEIGHT 192
#define CAMERA_VIEW_MARGIN 24
#define TRICK_ACTION_AFTER_TICKS 8
#define TIME_JUMP_AFTER_TICKS 30
#define TIME_JUMP_MOVIE "time_jump"
#define COUNTDOWN_DEFAULT_INTERVAL 300
#define COUNTDOWN_ZERO_STEP 3
#define COUNTDOWN_ENDING_STEP 4
#define FATE_LEVEL_MOST 7
#define FATE_LEVEL_LOWEST_WIPE 2
#define FATE_LEVEL_HIGHEST_WIPE 4
#define FATE_SECTION_FLAG 0x1020u
#define EYECATCH_FRAMES 114
#define EYECATCH_FRAMES_WITH_HOURGLASS 124
#define EYECATCH_RELEASE_TICKS 3
#define EYECATCH_SOUND_TRICK 11
#define EYECATCH_SOUND_UPDATE 12
#define EYECATCH_SOUND_NUMBER 13
#define PHONE_EFFECT_ID 49
#define PHONE_EFFECT_EVENT_AFTER_TICKS 30
#define CAMERA_FREE_SPEED_PIXELS 4

enum tutorial_mode {
    TUTORIAL_NONE,
    TUTORIAL_TOUCH_CHECK,
    TUTORIAL_MOVE
};

#define GHOST_FOLLOW_LEFT 124
#define GHOST_FOLLOW_RIGHT 132
#define GHOST_FOLLOW_TOP 92
#define GHOST_FOLLOW_BOTTOM 100
#define GHOST_FOLLOW_STEP 4
#define NORMAL_FOLLOW_LEFT 16
#define NORMAL_FOLLOW_RIGHT 240
#define NORMAL_FOLLOW_TOP 8
#define NORMAL_FOLLOW_BOTTOM 176
#define NORMAL_FOLLOW_STEP_X 5.5f
#define NORMAL_FOLLOW_STEP_Y 4.0f
#define NORMAL_FOLLOW_FADE 0.953125f
#define NORMAL_FOLLOW_STOPS_BELOW 0.25f
#define DS_SCREEN_WIDTH 256
#define DS_SCREEN_HEIGHT 192
#define LEAP_SPOKES_AFTER_TICKS 87
#define LEAP_SWIRL_AFTER_TICKS 102
#define LEAP_MESSAGE_AFTER_TICKS 282
#define REWIND_SLIDE_TICKS 20
#define REWIND_PRESS_TO_MOVIE_TICKS 63
#define MARK_IS_OFF -1
#define MARK_LAST_TICK 165
#define MARK_TICKS_BEFORE_THE_CORE_SHOWS 30
#define MARK_AFTER_LEAVING_TICKS 5
#define MARK_SOUND_KIND 18
#define PILL_HOLD_AFTER_VIEW_CHANGE 2
#define PILL_HOLD_AFTER_TRICK 1
#define SOUL_VIEW_ENTER_TICKS 33
#define SOUL_VIEW_LEAVE_TICKS 13
#define SOUL_LEAVE_SOUND_KIND 15
#define QUIET_LEAVING_FLAG 0x1003

static const char *const core_verbs[SCRIPT_CORE_SLOT_COUNT] = { "core", "core1" };
static const char *const fate_wipe_clips[] = { "TIME_1", "TIME_2", "TIME_3" };
static const char *const countdown_clips[] = { "DEATH_COUNT_3", "DEATH_COUNT_2", "DEATH_COUNT_1", "DEATH_COUNT_0" };

static int object_can_be_a_core(const struct script_object *object, int slot)
{
    const struct script_actor *actor;

    actor = &object->actor;
    if (!actor->exists || !actor->is_visible || actor->is_hard_hidden || actor->is_frozen_and_hidden) {
        return 0;
    }
    if (slot == 1) {
        return actor->has_second_core && !(actor->disabled_cores & SECOND_CORE_DISABLED_BIT);
    }
    return actor->has_core && !(actor->disabled_cores & FIRST_CORE_DISABLED_BIT);
}

static void player_moved_in_tutorial(struct script_machine *machine);

static void core_position(const struct script_machine *machine, int object_index, int slot, float *x, float *y)
{
    const struct script_actor *actor;

    actor = &machine->objects[object_index].actor;
    if (slot == 1 && actor->has_second_core) {
        *x = (float)(actor->x + actor->second_core_x);
        *y = (float)(actor->y + actor->second_core_y);
    } else {
        *x = (float)(actor->x + actor->core_x);
        *y = (float)(actor->y + actor->core_y);
    }
}

static void arrive_at_core(struct script_machine *machine, int object_index, int slot)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->active_object = object_index;
    soul->active_slot = slot;
    machine->active_core_name = machine->objects[object_index].name;
    if (soul->is_on) {
        soul->mark_age_ticks = MARK_TICKS_BEFORE_THE_CORE_SHOWS;
    }
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
    core_position(machine, soul->active_object, soul->active_slot, &soul->drag_from_x, &soul->drag_from_y);
    core_position(machine, object_index, slot, &target_x, &target_y);
    soul->drag_object = object_index;
    soul->drag_slot = slot;
    soul->drag_stream_ticks = machine->objects[object_index].is_dead ? SOUL_LIGHTNING_TICKS : stream_ticks_for_distance(sqrtf((target_x - soul->drag_from_x) * (target_x - soul->drag_from_x) + (target_y - soul->drag_from_y) * (target_y - soul->drag_from_y)));
    soul->drag_ticks_left = SOUL_BEAM_TICKS + soul->drag_stream_ticks;
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

            core_position(machine, soul->drag_object, soul->drag_slot, &target_x, &target_y);
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

            core_position(machine, soul->active_object, soul->active_slot, &x, &y);
            if (soul->tip_is_out) {
                machine->outputs->soul_cursor_moved(1, x, y, soul->tip_x, soul->tip_y, SCRIPT_SOUL_BEAM);
            } else {
                machine->outputs->soul_cursor_moved(1, x, y, x, y, SCRIPT_SOUL_RESTING);
            }
        } else {
            machine->outputs->soul_cursor_moved(0, 0.0f, 0.0f, 0.0f, 0.0f, SCRIPT_SOUL_RESTING);
        }
    }
    if (machine->outputs->soul_mark_changed != NULL) {
        if (soul->mark_age_ticks >= 0 && soul->active_object != NO_CORE && soul->drag_ticks_left == 0) {
            float x;
            float y;

            core_position(machine, soul->active_object, soul->active_slot, &x, &y);
            machine->outputs->soul_mark_changed(machine->objects[soul->active_object].name, soul->mark_age_ticks, x, y);
        } else {
            machine->outputs->soul_mark_changed(NULL, 0, 0.0f, 0.0f);
        }
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

            core_position(machine, index, 0, &x, &y);
            machine->outputs->soul_core_marked(index, x, y, machine->objects[index].is_dead);
        }
    }
}

static void start_time_leap(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (machine->auto_press == SCRIPT_AUTO_FAST) {
        if (machine->outputs->swirl_changed != NULL) {
            machine->outputs->swirl_changed(1);
        }
        script_machine_send(machine, "*", "timeleap", "start");
        return;
    }
    soul->leap_spokes_at = (long long)machine->tick + LEAP_SPOKES_AFTER_TICKS;
    soul->leap_swirl_at = (long long)machine->tick + LEAP_SWIRL_AFTER_TICKS;
    soul->leap_message_at = (long long)machine->tick + LEAP_MESSAGE_AFTER_TICKS;
}

static void step_the_time_leap(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->leap_spokes_at == (long long)machine->tick && machine->outputs->soul_vision_changed != NULL) {
        machine->outputs->soul_vision_changed(1, 1);
    }
    if (soul->leap_swirl_at == (long long)machine->tick && machine->outputs->swirl_changed != NULL) {
        machine->outputs->swirl_changed(1);
    }
    if (soul->leap_message_at == (long long)machine->tick) {
        script_machine_send(machine, "*", "timeleap", "start");
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
        if (machine->objects[soul->drag_object].is_dead) {
            start_time_leap(machine);
        }
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

        core_position(machine, soul->active_object, soul->active_slot, &x, &y);
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
        core_position(machine, object_index, 0, &x, &y);
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
    float tip_length;
    float best;
    int best_object;
    int object_index;

    soul = &machine->soul;
    tip_length = sqrtf((soul->tip_x - root_x) * (soul->tip_x - root_x) + (soul->tip_y - root_y) * (soul->tip_y - root_y));
    best = 0.0f;
    best_object = NO_CORE;
    for (object_index = 0; object_index < machine->object_count; object_index++) {
        int slot;

        for (slot = 0; slot < SCRIPT_CORE_SLOT_COUNT; slot++) {
            float x;
            float y;
            float distance;

            if ((object_index == soul->active_object && slot == soul->active_slot) || !object_can_be_a_core(&machine->objects[object_index], slot)) {
                continue;
            }
            core_position(machine, object_index, slot, &x, &y);
            if (!core_is_within_tip_pickup(root_x, root_y, soul->tip_x, soul->tip_y, x, y)) {
                continue;
            }
            distance = sqrtf((x - soul->tip_x) * (x - soul->tip_x) + (y - soul->tip_y) * (y - soul->tip_y));
            if (short_tip_cannot_pick_up(tip_length, distance)) {
                continue;
            }
            if (best_object == NO_CORE || distance < best) {
                best = distance;
                best_object = object_index;
                *found_slot = slot;
            }
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
    if (soul->held_x == 0 && soul->held_y == 0 && !soul->tip_is_aimed && !soul->touch_is_down) {
        soul->tip_is_out = 0;
        return;
    }
    core_position(machine, soul->active_object, soul->active_slot, &root_x, &root_y);
    if (!soul->tip_is_out) {
        soul->tip_x = root_x;
        soul->tip_y = root_y;
        soul->tip_is_out = 1;
    }
    if (soul->touch_is_down) {
        float to_x;
        float to_y;
        float distance;

        to_x = soul->touch_x - soul->tip_x;
        to_y = soul->touch_y - soul->tip_y;
        distance = sqrtf(to_x * to_x + to_y * to_y);
        if (distance > SOUL_TIP_TOUCH_DS_PIXELS_PER_TICK) {
            to_x = to_x * SOUL_TIP_TOUCH_DS_PIXELS_PER_TICK / distance;
            to_y = to_y * SOUL_TIP_TOUCH_DS_PIXELS_PER_TICK / distance;
        }
        soul->tip_x = soul->tip_x + to_x;
        soul->tip_y = soul->tip_y + to_y;
    } else if (soul->tip_is_aimed) {
        soul->tip_x = soul->tip_x + soul->tip_aim_x * SOUL_TIP_DS_PIXELS_PER_TICK;
        soul->tip_y = soul->tip_y + soul->tip_aim_y * SOUL_TIP_DS_PIXELS_PER_TICK;
    } else {
        soul->tip_x = soul->tip_x + (float)soul->held_x * SOUL_TIP_DS_PIXELS_PER_TICK;
        soul->tip_y = soul->tip_y + (float)soul->held_y * SOUL_TIP_DS_PIXELS_PER_TICK;
    }
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
    if (soul->countdown_is_running && soul->countdown_step >= COUNTDOWN_ENDING_STEP && soul->countdown_ticks < TRICK_ACTION_AFTER_TICKS) {
        soul->countdown_ticks = TRICK_ACTION_AFTER_TICKS;
    }
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("trick", object->name);
    }
    if (object->is_dead) {
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

static void start_ui_clip(struct script_machine *machine, const char *name, int level_from, int level_to)
{
    if (machine->outputs->ui_clip_started != NULL) {
        machine->outputs->ui_clip_started(name, level_from, level_to);
    }
}

void script_soul_set_fate_level(struct script_machine *machine, int level)
{
    int previous;

    if (level < 0) {
        level = 0;
    }
    if (level > FATE_LEVEL_MOST) {
        level = FATE_LEVEL_MOST;
    }
    previous = machine->fate_level;
    if (previous == level) {
        return;
    }
    machine->fate_previous_level = previous;
    machine->fate_level = level;
    if (level >= FATE_LEVEL_LOWEST_WIPE && level <= FATE_LEVEL_HIGHEST_WIPE && previous > level && script_flags_read(&machine->flags, FATE_SECTION_FLAG)) {
        start_ui_clip(machine, fate_wipe_clips[level - FATE_LEVEL_LOWEST_WIPE], level, level);
    }
}

static void run_countdown(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (!soul->countdown_is_running || machine->pause_bit || machine->mode_byte != NORMAL_PLAY_MODE) {
        return;
    }
    script_soul_set_fate_level(machine, soul->countdown_step >= COUNTDOWN_ENDING_STEP ? 0 : 1);
    soul->countdown_ticks = soul->countdown_ticks - 1;
    if (soul->countdown_ticks > 0) {
        return;
    }
    if (soul->countdown_step < COUNTDOWN_ZERO_STEP) {
        start_ui_clip(machine, countdown_clips[soul->countdown_step], machine->fate_level, machine->fate_level);
        soul->countdown_ticks = machine->countdown_interval;
        soul->countdown_step = soul->countdown_step + 1;
    } else if (soul->countdown_step == COUNTDOWN_ZERO_STEP) {
        start_ui_clip(machine, countdown_clips[COUNTDOWN_ZERO_STEP], machine->fate_level, machine->fate_level);
        script_machine_send(machine, "*", "countdown", "zero");
        soul->countdown_ticks = machine->countdown_interval * 3 / 2;
        soul->countdown_step = COUNTDOWN_ENDING_STEP;
    } else {
        script_machine_send(machine, "*", "countdown", "end");
        soul->countdown_is_running = 0;
    }
}

void script_soul_set_countdown(struct script_machine *machine, int first_tick_count)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->countdown_is_running = first_tick_count != 0;
    soul->countdown_step = 0;
    soul->countdown_ticks = first_tick_count;
}

void script_soul_set_countdown_interval(struct script_machine *machine, int interval)
{
    machine->countdown_interval = interval < 0 ? COUNTDOWN_DEFAULT_INTERVAL : interval;
}

static void report_hud(struct script_machine *machine)
{
    struct script_hud_view view;

    if (machine->outputs->hud_changed == NULL) {
        return;
    }
    view.side_is_on = script_flags_read(&machine->flags, FATE_SECTION_FLAG);
    view.fate_level = machine->fate_level;
    view.world_is_running = !machine->pause_bit && machine->mode_byte == NORMAL_PLAY_MODE && machine->eyecatch_ticks_left == 0;
    view.is_hidden_for_movie = machine->movie_ticks_left > 0;
    view.narration_arrow_is_on = script_narration_wants_a_press(&machine->narration);
    view.soul_view_is_on = machine->soul.is_on;
    view.rewind_select_ticks = machine->soul.rewind_select_ticks;
    view.rewind_press_ticks = machine->soul.rewind_press_ticks;
    script_pills_fill_view(machine, &view);
    machine->outputs->hud_changed(&view);
}

static void start_eye_catch(struct script_machine *machine, const char *kind)
{
    const char *clip;
    int frames;

    if (strcmp(kind, "Trick") == 0) {
        clip = "TRICK";
        frames = EYECATCH_FRAMES;
    } else if (strcmp(kind, "DestinyUpdate") == 0) {
        clip = "KOSHIN";
        frames = EYECATCH_FRAMES_WITH_HOURGLASS;
    } else if (strcmp(kind, "DestinyLock") == 0) {
        clip = "KANRYO";
        frames = EYECATCH_FRAMES;
    } else if (strcmp(kind, "NumberGet") == 0 || strcmp(kind, "NumberGetForce") == 0) {
        clip = "BANGO";
        frames = EYECATCH_FRAMES;
    } else {
        script_machine_note(machine, NULL, "eyecatch_kind_not_ported", kind);
        return;
    }
    start_ui_clip(machine, clip, machine->fate_previous_level, machine->fate_level);
    if (machine->outputs->sound_played != NULL) {
        machine->outputs->sound_played(strcmp(kind, "DestinyUpdate") == 0 ? EYECATCH_SOUND_UPDATE : (strcmp(clip, "BANGO") == 0 ? EYECATCH_SOUND_NUMBER : EYECATCH_SOUND_TRICK));
    }
    machine->mode_byte = DEMO_MODE;
    machine->eyecatch_marks_the_core = strcmp(kind, "Trick") == 0;
    machine->eyecatch_ticks_left = machine->auto_press == SCRIPT_AUTO_FAST ? 0 : frames + EYECATCH_RELEASE_TICKS;
}

int script_soul_step_eyecatch(struct script_machine *machine)
{
    if (machine->eyecatch_ticks_left <= 0) {
        return 0;
    }
    machine->eyecatch_ticks_left = machine->eyecatch_ticks_left - 1;
    if (machine->eyecatch_ticks_left == 0 && machine->eyecatch_marks_the_core) {
        machine->eyecatch_marks_the_core = 0;
        script_soul_start_player_mark(machine);
    }
    report_hud(machine);
    return 1;
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
    script_pills_effect_spawned(machine, effect_id, name);
    if (effect_id == PHONE_EFFECT_ID) {
        schedule_native_event(machine, &machine->soul.phone_event, name, "ready", PHONE_EFFECT_EVENT_AFTER_TICKS);
    }
}

void script_soul_effect_signalled(struct script_machine *machine, const char *name)
{
    schedule_native_event(machine, &machine->soul.phone_event, name, "end", PHONE_EFFECT_EVENT_AFTER_TICKS);
}

static void set_soul_view_with_effect(struct script_machine *machine, int is_on, int with_effect)
{
    struct script_soul *soul;
    int holds_the_world;

    soul = &machine->soul;
    soul->is_on = is_on;
    holds_the_world = with_effect && machine->auto_press != SCRIPT_AUTO_FAST;
    soul->is_leaving_view = 0;
    if (!is_on) {
        soul->player_opened_soul_view = 0;
        soul->mark_age_ticks = MARK_IS_OFF;
        soul->is_leaving_view = holds_the_world;
    }
    soul->view_change_ticks_left = holds_the_world ? (is_on ? SOUL_VIEW_ENTER_TICKS : SOUL_VIEW_LEAVE_TICKS) : 0;
    if (holds_the_world && is_on) {
        soul->camera_was_saved = 1;
        soul->saved_camera_x = machine->camera_x;
        soul->saved_camera_y = machine->camera_y;
        soul->saved_camera_core = soul->active_object;
    } else if (holds_the_world && soul->camera_was_saved) {
        soul->camera_was_saved = 0;
        if (soul->saved_camera_core == soul->active_object) {
            script_machine_pan_camera_back_to(machine, soul->saved_camera_x, soul->saved_camera_y);
        }
    }
    machine->soul_state_keeps_pause = is_on;
    machine->pause_bit = is_on || soul->is_leaving_view;
    if (machine->outputs->soul_vision_changed != NULL) {
        machine->outputs->soul_vision_changed(is_on, with_effect);
    }
    if (is_on) {
        soul->start_message_at = (long long)machine->tick + 1;
        if (with_effect) {
            machine->soul_sound_at = (long long)machine->tick + 1;
        }
    } else {
        soul->final_message_at = (long long)machine->tick + 1;
        if (with_effect && machine->outputs->sound_played != NULL) {
            machine->outputs->sound_played(SOUL_LEAVE_SOUND_KIND);
        }
    }
}

static void set_soul_view(struct script_machine *machine, int is_on)
{
    set_soul_view_with_effect(machine, is_on, 1);
}

static int leaving_skips_the_effect(struct script_machine *machine)
{
    int is_quiet;

    is_quiet = machine->soul.next_leaving_is_quiet || script_flags_read(&machine->flags, QUIET_LEAVING_FLAG);
    machine->soul.next_leaving_is_quiet = 0;
    return is_quiet;
}

void script_soul_start_player_mark(struct script_machine *machine)
{
    if (machine->soul.active_object == NO_CORE) {
        return;
    }
    machine->soul.mark_age_ticks = 0;
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("player_mark", machine->objects[machine->soul.active_object].name);
    }
    if (machine->outputs->sound_played != NULL) {
        machine->outputs->sound_played(MARK_SOUND_KIND);
    }
}

static void age_the_player_mark(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->mark_age_ticks < 0) {
        return;
    }
    soul->mark_age_ticks = soul->mark_age_ticks + 1;
    if (soul->mark_age_ticks > MARK_LAST_TICK) {
        soul->mark_age_ticks = MARK_IS_OFF;
    }
}

static void finish_leaving_view_when_due(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->view_change_ticks_left <= 0) {
        return;
    }
    soul->view_change_ticks_left = soul->view_change_ticks_left - 1;
    if (soul->is_leaving_view && soul->view_change_ticks_left == SOUL_VIEW_LEAVE_TICKS - MARK_AFTER_LEAVING_TICKS && (machine->mode_byte == NORMAL_PLAY_MODE || soul->leaving_shows_the_mark)) {
        soul->leaving_shows_the_mark = 0;
        script_soul_start_player_mark(machine);
    }
    if (soul->is_on && soul->view_change_ticks_left == 0 && soul->player_opened_soul_view) {
        soul->mark_age_ticks = MARK_TICKS_BEFORE_THE_CORE_SHOWS;
    }
    if (soul->view_change_ticks_left == 0 && soul->is_leaving_view) {
        soul->is_leaving_view = 0;
        machine->pause_bit = 0;
        script_machine_send(machine, "*", "SYSTEM", "SV_END");
    }
}

static void enter_scripted_soul_view(struct script_machine *machine, const char *arg)
{
    set_soul_view_with_effect(machine, 1, strcmp(arg, "NoEffectStart") != 0);
    machine->soul_vision_message_at = (long long)machine->tick + SOUL_VISION_MESSAGE_AFTER_TICKS;
}

static void leave_soul_view(struct script_machine *machine)
{
    machine->soul.tutorial_mode = TUTORIAL_NONE;
    machine->mode_byte = DEMO_MODE;
    set_soul_view_with_effect(machine, 0, !leaving_skips_the_effect(machine));
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
        soul->leaving_shows_the_mark = 1;
        leave_soul_view(machine);
    }
}

static void run_button_command(struct script_machine *machine, const char *arg)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (strcmp(arg, "All:Off") == 0) {
        soul->trick_is_on = 0;
        script_pills_unregister_all(machine);
    } else if (strcmp(arg, "All:On") == 0) {
        soul->trick_is_on = 1;
        script_pills_register_all(machine);
    } else if (strcmp(arg, "Action:On") == 0) {
        soul->trick_is_on = 1;
        script_pills_register(machine, SCRIPT_PILL_TRICK, 1);
    } else if (strcmp(arg, "Trick:On") == 0) {
        soul->trick_is_on = 1;
        script_pills_register(machine, SCRIPT_PILL_GHOST, 1);
    } else if (strcmp(arg, "Trick:Off") == 0) {
        script_pills_register(machine, SCRIPT_PILL_GHOST, 0);
    } else if (strcmp(arg, "BackToMain:On") == 0 || strcmp(arg, "BackToMain:Off") == 0) {
        script_pills_register(machine, SCRIPT_PILL_BACK, strcmp(arg, "BackToMain:On") == 0);
    } else if (strcmp(arg, "Database:On") == 0 || strcmp(arg, "Database:Off") == 0) {
        script_pills_register(machine, SCRIPT_PILL_NOTEBOOK, strcmp(arg, "Database:On") == 0);
    } else if (strcmp(arg, "Rollback:On") == 0) {
        soul->rollback_is_on = 1;
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
    if (strcmp(arg, "Start") == 0) {
        set_soul_view(machine, 1);
    } else if (strcmp(arg, "NoEffectStart") == 0) {
        set_soul_view_with_effect(machine, 1, 0);
    } else if (strcmp(arg, "NoEffectFinish") == 0) {
        machine->soul.next_leaving_is_quiet = 1;
    } else if (strcmp(arg, "ReturnDemo") == 0 && machine->soul.is_on) {
        machine->mode_byte = DEMO_MODE;
        set_soul_view_with_effect(machine, 0, !leaving_skips_the_effect(machine));
    }
}

static void start_the_time_jump(struct script_machine *machine)
{
    if (machine->outputs->movie_started != NULL && machine->time_jump_movie_ticks > 0) {
        machine->movie_ticks_left = machine->auto_press == SCRIPT_AUTO_FAST ? 0 : machine->time_jump_movie_ticks;
        machine->outputs->movie_started(TIME_JUMP_MOVIE);
    }
    schedule_native_event(machine, &machine->soul.time_jump_event, "timeleap", "jump", machine->time_jump_movie_ticks > 0 ? machine->time_jump_movie_ticks : TIME_JUMP_AFTER_TICKS);
}

static void step_the_rewind_select(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    if (soul->rewind_select_ticks == 0) {
        return;
    }
    soul->rewind_select_ticks = soul->rewind_select_ticks + 1;
    if (soul->rewind_press_ticks == 0) {
        return;
    }
    soul->rewind_press_ticks = soul->rewind_press_ticks + 1;
    if (soul->rewind_press_ticks > REWIND_PRESS_TO_MOVIE_TICKS) {
        soul->rewind_select_ticks = 0;
        soul->rewind_press_ticks = 0;
        start_the_time_jump(machine);
    }
}

void script_soul_run_command(struct script_machine *machine, const char *verb, const char *arg)
{
    if (strcmp(verb, "SoulVision") == 0) {
        enter_scripted_soul_view(machine, arg);
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
    } else if (strcmp(verb, "EyeCatch") == 0) {
        start_eye_catch(machine, arg);
    } else if (strcmp(verb, "FreeCamera") == 0) {
        machine->soul.camera_tutorial_is_on = strcmp(arg, "TutorialStart") == 0;
    } else if (strcmp(verb, "Timeleap") == 0 && strcmp(arg, "BREAK") == 0) {
        if (machine->outputs->swirl_changed != NULL) {
            machine->outputs->swirl_changed(0);
        }
    } else if (strcmp(verb, "Timeleap") == 0 && strcmp(arg, "SELECT") == 0) {
        if (machine->auto_press == SCRIPT_AUTO_FAST) {
            start_the_time_jump(machine);
        } else {
            machine->soul.rewind_select_ticks = 1;
            machine->soul.rewind_press_ticks = 0;
            if (machine->outputs->marked != NULL) {
                machine->outputs->marked("rewind_select", arg);
            }
        }
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

static int pill_answers_a_press(const struct script_machine *machine, enum script_pill_id id)
{
    return machine->auto_press != SCRIPT_AUTO_OFF || script_pills_is_up(machine, id);
}

static int core_is_busy(const struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    return soul->active_object != NO_CORE && (machine->objects[soul->active_object].actor.busy_cores & (1 << soul->active_slot)) != 0;
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
    if (soul->rewind_select_ticks >= REWIND_SLIDE_TICKS) {
        if (soul->rewind_press_ticks == 0) {
            soul->rewind_press_ticks = 1;
        }
        return;
    }
    if (soul->tutorial_mode != TUTORIAL_NONE) {
        if (soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent) {
            player_moved_in_tutorial(machine);
        }
        return;
    }
    if (soul->is_on) {
        return;
    }
    if (script_soul_world_is_playable(machine) && soul->trick_is_on && !core_is_busy(machine) && pill_answers_a_press(machine, SCRIPT_PILL_TRICK)) {
        script_pills_press(machine, SCRIPT_PILL_TRICK, PILL_HOLD_AFTER_TRICK);
        soul->mark_age_ticks = MARK_IS_OFF;
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
    if (soul->view_change_ticks_left > 0) {
        return;
    }
    if (soul->is_on && soul->player_opened_soul_view) {
        if (pill_answers_a_press(machine, SCRIPT_PILL_BACK)) {
            script_pills_press(machine, SCRIPT_PILL_BACK, PILL_HOLD_AFTER_VIEW_CHANGE);
            set_soul_view(machine, 0);
        }
        return;
    }
    if (!soul->is_on && script_soul_world_is_playable(machine) && pill_answers_a_press(machine, SCRIPT_PILL_GHOST)) {
        script_pills_press(machine, SCRIPT_PILL_GHOST, PILL_HOLD_AFTER_VIEW_CHANGE);
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

static int free_camera_is_allowed(const struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    if (soul->is_on || soul->camera_is_locked || (machine->pause_bit && !soul->camera_tutorial_is_on)) {
        return 0;
    }
    return !soul->free_camera_is_forbidden || soul->camera_tutorial_is_on;
}

void script_soul_drag_camera(struct script_machine *machine, float delta_x, float delta_y)
{
    struct script_soul *soul;
    int whole_x;
    int whole_y;

    soul = &machine->soul;
    if (!free_camera_is_allowed(machine)) {
        return;
    }
    soul->camera_drag_x = soul->camera_drag_x + delta_x;
    soul->camera_drag_y = soul->camera_drag_y + delta_y;
    whole_x = (int)soul->camera_drag_x;
    whole_y = (int)soul->camera_drag_y;
    soul->camera_drag_x = soul->camera_drag_x - (float)whole_x;
    soul->camera_drag_y = soul->camera_drag_y - (float)whole_y;
    if (whole_x != 0 || whole_y != 0) {
        script_machine_scroll_camera(machine, whole_x, whole_y);
    }
}

static void move_free_camera(struct script_machine *machine)
{
    const struct script_soul *soul;

    soul = &machine->soul;
    if (soul->held_x == 0 && soul->held_y == 0) {
        return;
    }
    if (!free_camera_is_allowed(machine)) {
        return;
    }
    script_machine_scroll_camera(machine, soul->held_x * CAMERA_FREE_SPEED_PIXELS, soul->held_y * CAMERA_FREE_SPEED_PIXELS);
}

static void follow_core_in_ghost_view(struct script_machine *machine, float core_x, float core_y)
{
    int across;
    int down;
    int step_x;
    int step_y;

    if (machine->soul.view_change_ticks_left > 0) {
        return;
    }
    across = (int)core_x - (machine->camera_x - machine->world_origin_x);
    down = (int)core_y - (machine->camera_y - machine->world_origin_y);
    step_x = across < GHOST_FOLLOW_LEFT ? -GHOST_FOLLOW_STEP : (across > GHOST_FOLLOW_RIGHT ? GHOST_FOLLOW_STEP : 0);
    step_y = down < GHOST_FOLLOW_TOP ? -GHOST_FOLLOW_STEP : (down > GHOST_FOLLOW_BOTTOM ? GHOST_FOLLOW_STEP : 0);
    if (step_x != 0 || step_y != 0) {
        script_machine_scroll_camera(machine, step_x, step_y);
    }
}

static void follow_core_in_normal_play(struct script_machine *machine, float core_x, float core_y)
{
    struct script_soul *soul;
    const struct script_room_bounds *bounds;
    int across;
    int down;
    float step_y;
    int whole_x;
    int whole_y;

    soul = &machine->soul;
    if (machine->pause_bit || machine->mode_byte != NORMAL_PLAY_MODE || machine->room < 0 || machine->room >= SCRIPT_ROOM_ID_MAX) {
        return;
    }
    bounds = &machine->room_bounds[machine->room];
    across = (int)core_x - (machine->camera_x - machine->world_origin_x);
    down = (int)core_y - (machine->camera_y - machine->world_origin_y);
    step_y = 0.0f;
    if (bounds->height > DS_SCREEN_HEIGHT) {
        step_y = (float)script_machine_camera_band_snap(machine, (int)core_y);
        if (step_y == 0.0f) {
            step_y = down < NORMAL_FOLLOW_TOP ? -NORMAL_FOLLOW_STEP_Y : (down > NORMAL_FOLLOW_BOTTOM ? NORMAL_FOLLOW_STEP_Y : 0.0f);
        }
    }
    if (bounds->width > DS_SCREEN_WIDTH) {
        if (across < NORMAL_FOLLOW_LEFT) {
            soul->camera_follow_x = -NORMAL_FOLLOW_STEP_X;
        } else if (across > NORMAL_FOLLOW_RIGHT) {
            soul->camera_follow_x = NORMAL_FOLLOW_STEP_X;
        }
    } else {
        soul->camera_follow_x = 0.0f;
    }
    soul->camera_carry_x = soul->camera_carry_x + soul->camera_follow_x;
    soul->camera_carry_y = soul->camera_carry_y + step_y;
    whole_x = (int)soul->camera_carry_x;
    whole_y = (int)soul->camera_carry_y;
    soul->camera_carry_x = soul->camera_carry_x - (float)whole_x;
    soul->camera_carry_y = soul->camera_carry_y - (float)whole_y;
    if (whole_x != 0 || whole_y != 0) {
        script_machine_scroll_camera(machine, whole_x, whole_y);
    }
    soul->camera_follow_x = soul->camera_follow_x * NORMAL_FOLLOW_FADE;
    if (fabsf(soul->camera_follow_x) < NORMAL_FOLLOW_STOPS_BELOW) {
        soul->camera_follow_x = 0.0f;
    }
}

static void follow_core_with_camera(struct script_machine *machine)
{
    struct script_soul *soul;
    float core_x;
    float core_y;

    soul = &machine->soul;
    if (machine->auto_press == SCRIPT_AUTO_FAST || soul->camera_follow_is_off || soul->camera_is_locked || machine->pan_is_active || soul->active_object == NO_CORE) {
        return;
    }
    core_position(machine, soul->active_object, soul->active_slot, &core_x, &core_y);
    if (soul->is_on) {
        follow_core_in_ghost_view(machine, core_x, core_y);
    } else {
        follow_core_in_normal_play(machine, core_x, core_y);
    }
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
    snprintf(detail, sizeof(detail), "%d:%s:%s:%d", machine->soul.route_step, script_route_action_name(step->action), step->object[0] != '\0' ? step->object : "-", step->slot);
    machine->outputs->marked(what, detail);
}

static void go_to_route_step(struct script_machine *machine, int step_index)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->route_step = step_index;
    soul->route_step_began_at = machine->tick;
    soul->route_ready_at = 0;
    soul->route_action_has_begun = 0;
    soul->route_tricks_when_step_began = soul->tricks_fired;
}

static void finish_route_step(struct script_machine *machine, const struct script_route_step *step)
{
    mark_route_step(machine, "route_step_done", step);
    machine->last_new_site_tick = machine->tick;
    machine->soul.route_last_action_tick = machine->tick;
    go_to_route_step(machine, machine->soul.route_step + 1);
}

static int balloon_sending(const struct script_machine *machine, const char *event)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].tap_arg != NULL && strcmp(machine->balloons[index].tap_arg, event) == 0) {
            return index;
        }
    }
    return -1;
}

static int route_token_holds(const struct script_machine *machine, char *token)
{
    const struct script_soul *soul;
    char *first;
    char *second;
    int object_index;

    soul = &machine->soul;
    first = strchr(token, ':');
    second = NULL;
    if (first != NULL) {
        *first = '\0';
        first = first + 1;
        second = strchr(first, ':');
        if (second != NULL) {
            *second = '\0';
            second = second + 1;
        }
    }
    if (strcmp(token, "playable") == 0) {
        return script_soul_world_is_playable(machine) && soul->trick_is_on;
    }
    if (strcmp(token, "free") == 0) {
        return script_soul_world_is_playable(machine);
    }
    if (strcmp(token, "view") == 0) {
        return soul->is_on;
    }
    if (strcmp(token, "no_talk") == 0) {
        return machine->talk.phase == SCRIPT_TALK_CLOSED && !machine->talk_is_open;
    }
    if (strcmp(token, "camera_tutorial") == 0) {
        return soul->camera_tutorial_is_on;
    }
    if (strcmp(token, "timeleap_armed") == 0) {
        return soul->rewind_select_ticks >= REWIND_SLIDE_TICKS || soul->time_jump_event.target != NULL;
    }
    if (first == NULL) {
        return 0;
    }
    if (strcmp(token, "act") == 0) {
        object_index = object_named(machine, first);
        return object_index != NO_CORE && second != NULL && machine->objects[object_index].current_act != NULL && strcmp(machine->objects[object_index].current_act, second) == 0;
    }
    if (strcmp(token, "tutorial") == 0) {
        if (strcmp(first, "touch") == 0) {
            return soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && !soul->tutorial_click_was_sent;
        }
        if (strcmp(first, "touched") == 0) {
            return soul->tutorial_mode == TUTORIAL_TOUCH_CHECK && soul->tutorial_click_was_sent;
        }
        return strcmp(first, "move") == 0 && soul->tutorial_mode == TUTORIAL_MOVE;
    }
    if (strcmp(token, "flag") == 0) {
        return script_flags_read(&machine->flags, (unsigned int)strtoul(first, NULL, 16));
    }
    if (strcmp(token, "noflag") == 0) {
        return !script_flags_read(&machine->flags, (unsigned int)strtoul(first, NULL, 16));
    }
    if (strcmp(token, "core") == 0) {
        return machine->active_core_name != NULL && strcmp(machine->active_core_name, first) == 0 && (second == NULL || soul->active_slot == atoi(second));
    }
    if (strcmp(token, "usable") == 0) {
        object_index = object_named(machine, first);
        return object_index != NO_CORE && object_can_be_a_core(&machine->objects[object_index], second != NULL ? atoi(second) : 0);
    }
    if (strcmp(token, "balloon") == 0) {
        return balloon_sending(machine, first) >= 0;
    }
    if (strcmp(token, "ticks") == 0) {
        return machine->tick - soul->route_last_action_tick >= (unsigned int)atoi(first);
    }
    return 0;
}

static int route_wait_holds(const struct script_machine *machine, const char *wait)
{
    char copy[SCRIPT_ROUTE_WAIT_MAX];
    char *token;

    snprintf(copy, sizeof(copy), "%s", wait);
    token = copy;
    while (*token != '\0') {
        char *end;
        int is_last;

        end = strchr(token, '&');
        is_last = end == NULL;
        if (!is_last) {
            *end = '\0';
        }
        if (!route_token_holds(machine, token)) {
            return 0;
        }
        if (is_last) {
            break;
        }
        token = end + 1;
    }
    return 1;
}

static void aim_tip_at_core(struct script_machine *machine, int object_index, int slot)
{
    struct script_soul *soul;
    float from_x;
    float from_y;
    float target_x;
    float target_y;
    float length;

    soul = &machine->soul;
    core_position(machine, soul->active_object, soul->active_slot, &from_x, &from_y);
    core_position(machine, object_index, slot, &target_x, &target_y);
    length = sqrtf((target_x - from_x) * (target_x - from_x) + (target_y - from_y) * (target_y - from_y));
    if (length <= 0.0f) {
        return;
    }
    soul->tip_is_aimed = 1;
    soul->tip_aim_x = (target_x - from_x) / length;
    soul->tip_aim_y = (target_y - from_y) / length;
}

static int run_route_move(struct script_machine *machine, const struct script_route_step *step)
{
    struct script_soul *soul;
    int object_index;

    soul = &machine->soul;
    object_index = object_named(machine, step->object);
    if (object_index == NO_CORE) {
        return 0;
    }
    if (soul->active_object == object_index && soul->active_slot == step->slot && soul->drag_ticks_left == 0) {
        return 1;
    }
    if (!soul->is_on || soul->drag_ticks_left > 0 || soul->active_object == NO_CORE) {
        return 0;
    }
    if (!object_can_be_a_core(&machine->objects[object_index], step->slot)) {
        return 0;
    }
    aim_tip_at_core(machine, object_index, step->slot);
    return 0;
}

static int run_route_action(struct script_machine *machine, const struct script_route_step *step)
{
    struct script_soul *soul;
    int balloon;

    soul = &machine->soul;
    switch (step->action) {
    case SCRIPT_ROUTE_MOVE:
        return run_route_move(machine, step);
    case SCRIPT_ROUTE_TRICK:
        if (soul->tricks_fired != soul->route_tricks_when_step_began) {
            return 1;
        }
        machine->press_is_pending = 1;
        return 0;
    case SCRIPT_ROUTE_TOUCH:
        machine->press_is_pending = 1;
        return 1;
    case SCRIPT_ROUTE_GHOST_ON:
        if (soul->is_on) {
            return 1;
        }
        soul->ghost_is_pending = 1;
        return 0;
    case SCRIPT_ROUTE_GHOST_OFF:
        if (!soul->is_on) {
            return 1;
        }
        soul->ghost_is_pending = 1;
        return 0;
    case SCRIPT_ROUTE_TAP_BALLOON:
        balloon = balloon_sending(machine, step->object);
        if (balloon < 0) {
            return 0;
        }
        soul->tap_group_is_pending = machine->balloons[balloon].tap_group;
        return 1;
    case SCRIPT_ROUTE_CAMERA:
        soul->route_camera_is_scrolling = 1;
        return 1;
    case SCRIPT_ROUTE_PHONE_CALL:
        script_machine_send(machine, "*", "phone", "movie");
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
    if (step == NULL || soul->view_change_ticks_left > 0) {
        return;
    }
    if (!soul->route_action_has_begun) {
        if (soul->drag_ticks_left > 0 || soul->action_is_due_in > 0) {
            return;
        }
        if (!route_wait_holds(machine, step->wait)) {
            soul->route_ready_at = 0;
            if (machine->tick - soul->route_step_began_at > ROUTE_WAIT_MOST_TICKS) {
                give_up_on_route_step(machine, step);
            }
            return;
        }
        if (soul->route_ready_at == 0) {
            soul->route_ready_at = machine->tick + (machine->auto_press == SCRIPT_AUTO_FAST ? 0u : (unsigned int)step->dwell_ticks);
        }
        if (machine->tick < soul->route_ready_at) {
            return;
        }
        soul->route_action_has_begun = 1;
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
    const struct script_route_step *unfinished;

    soul = &machine->soul;
    unfinished = current_route_step(machine);
    if (unfinished != NULL) {
        mark_route_step(machine, "route_step_failed", unfinished);
    }
    soul->route_script = script;
    soul->route_camera_is_scrolling = 0;
    go_to_route_step(machine, machine->route != NULL ? script_route_first_step_of_script(machine->route, script) : -1);
    soul->route_last_action_tick = machine->tick;
}

void script_soul_act_began(struct script_machine *machine, const char *object_name, const char *act_name)
{
    const struct script_soul *soul;
    const struct script_route_step *step;
    char act_prefix[SCRIPT_ROUTE_NAME_MAX];
    char act_token[SCRIPT_ROUTE_WAIT_MAX];
    int index;

    soul = &machine->soul;
    step = current_route_step(machine);
    if (step == NULL || soul->route_action_has_begun) {
        return;
    }
    snprintf(act_prefix, sizeof(act_prefix), "act:%s:", object_name);
    if (strstr(step->wait, act_prefix) == NULL) {
        return;
    }
    snprintf(act_token, sizeof(act_token), "%s%s", act_prefix, act_name);
    if (script_route_wait_has_token(step->wait, act_token)) {
        return;
    }
    for (index = soul->route_step + 1; index < machine->route->step_count; index++) {
        const struct script_route_step *later;

        later = &machine->route->steps[index];
        if (strcmp(later->script, soul->route_script) != 0) {
            return;
        }
        if (script_route_wait_has_token(later->wait, act_token)) {
            mark_route_step(machine, "route_step_failed", step);
            go_to_route_step(machine, index);
            return;
        }
    }
}

static void play_automatically(struct script_machine *machine)
{
    struct script_soul *soul;

    soul = &machine->soul;
    soul->held_x = 0;
    soul->held_y = 0;
    soul->tip_is_aimed = 0;
    if ((machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) || script_narration_wants_a_press(&machine->narration)) {
        press_when_the_page_has_been_read(machine);
        return;
    }
    soul->auto_reading_ticks = 0;
    if (soul->route_camera_is_scrolling) {
        if (soul->camera_tutorial_is_on) {
            script_machine_scroll_camera(machine, CAMERA_FREE_SPEED_PIXELS, 0);
            return;
        }
        soul->route_camera_is_scrolling = 0;
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
    if (soul->final_message_at == (long long)machine->tick) {
        soul->final_message_at = -1;
        script_machine_send(machine, "*", "SYSTEM", "SV_FINAL");
        if (!soul->is_leaving_view) {
            script_machine_send(machine, "*", "SYSTEM", "SV_END");
        }
    }
    finish_leaving_view_when_due(machine);
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
    follow_core_with_camera(machine);
    age_the_player_mark(machine);
    step_the_rewind_select(machine);
    step_the_time_leap(machine);
    finish_drag_when_due(machine);
    report_soul_view(machine);
    report_possession_effects(machine);
    send_trick_action_when_due(machine);
    run_countdown(machine);
    script_pills_step(machine);
    report_hud(machine);
    send_timed_native_event(machine, &soul->phone_event);
    send_timed_native_event(machine, &soul->time_jump_event);
}

int script_soul_wants_a_tap(const struct script_machine *machine)
{
    if (machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) {
        return 1;
    }
    if (script_narration_wants_a_press(&machine->narration)) {
        return 1;
    }
    return machine->soul.tutorial_mode == TUTORIAL_TOUCH_CHECK && !machine->soul.tutorial_click_was_sent;
}

void script_soul_set_touch(struct script_machine *machine, int is_down, float x, float y)
{
    machine->soul.touch_is_down = is_down;
    machine->soul.touch_x = x;
    machine->soul.touch_y = y;
}

void script_soul_note_ghost(struct script_machine *machine)
{
    machine->soul.ghost_is_pending = 1;
}

void script_soul_note_pill(struct script_machine *machine, enum script_pill_id id)
{
    if (!script_pills_is_up(machine, id)) {
        return;
    }
    script_pills_press(machine, id, 0);
    script_machine_note(machine, NULL, "pill_not_ported", id == SCRIPT_PILL_HOURGLASS ? "hourglass" : "notebook");
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
    if (machine->soul.is_on && machine->outputs != NULL && machine->outputs->soul_vision_changed != NULL) {
        machine->outputs->soul_vision_changed(0, 0);
    }
    memset(&machine->soul, 0, sizeof(machine->soul));
    script_pills_reset(machine);
    machine->soul.route_step = -1;
    machine->soul.active_object = NO_CORE;
    machine->soul.start_message_at = -1;
    machine->soul.final_message_at = -1;
    machine->soul.leap_spokes_at = -1;
    machine->soul.leap_swirl_at = -1;
    machine->soul.leap_message_at = -1;
    machine->soul.mark_age_ticks = MARK_IS_OFF;
    machine->soul.trick_is_on = 1;
    machine->countdown_interval = COUNTDOWN_DEFAULT_INTERVAL;
    machine->active_core_name = NULL;
}

void script_soul_set_first_core(struct script_machine *machine, int object_index, int slot)
{
    machine->soul.active_object = object_index;
    machine->soul.active_slot = slot;
    machine->active_core_name = machine->objects[object_index].name;
}

int script_soul_waits_on_the_player(const struct script_machine *machine)
{
    return machine->soul.tutorial_mode != TUTORIAL_NONE || machine->soul.camera_tutorial_is_on || machine->soul.player_opened_soul_view || script_narration_wants_a_press(&machine->narration);
}
