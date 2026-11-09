#include <math.h>
#include <string.h>
#include "script_machine.h"
#include "script_narration.h"
#include "script_route.h"
#include "script_route_pad.h"
#include "stick_aim.h"

#define TICKS_TO_READ_A_PAGE 120u
#define TICKS_BEFORE_A_RETRY 60u
#define TICKS_TO_READ_A_CHOICE 60u
#define TICKS_BETWEEN_PRESSES 30u
#define TICKS_BETWEEN_CURSOR_MOVES 6u
#define TIP_IS_ON_THE_CORE_DS_PIXELS 0.5f

void script_route_pad_reset(struct script_route_pad *pad)
{
    memset(pad, 0, sizeof(*pad));
}

static int pad_may_press(const struct script_route_pad *pad, const struct script_machine *machine, unsigned int gap)
{
    return !pad->has_pressed || machine->tick - pad->last_press_at >= gap;
}

static void press(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame, unsigned int button)
{
    frame->pressed = frame->pressed | button;
    pad->has_pressed = 1;
    pad->last_press_at = machine->tick;
}

static int a_page_holds_the_player(const struct script_machine *machine)
{
    return (machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine)) || script_narration_wants_a_press(&machine->narration);
}

static int read_the_page(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    if (!a_page_holds_the_player(machine)) {
        pad->page_was_waiting = 0;
        return 0;
    }
    if (!script_soul_page_wants_a_press(machine)) {
        pad->page_was_waiting = 0;
        return 1;
    }
    if (!pad->page_was_waiting) {
        pad->page_was_waiting = 1;
        pad->page_ready_at = machine->tick;
    }
    if (machine->tick - pad->page_ready_at >= TICKS_TO_READ_A_PAGE) {
        press(pad, machine, frame, INPUT_CROSS);
        pad->page_was_waiting = 0;
    }
    return 1;
}

static int retry_the_game_over(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    if (!script_soul_game_over_waits(machine)) {
        pad->game_over_was_waiting = 0;
        return 0;
    }
    if (!pad->game_over_was_waiting) {
        pad->game_over_was_waiting = 1;
        pad->game_over_since = machine->tick;
    }
    if (machine->tick - pad->game_over_since >= TICKS_BEFORE_A_RETRY && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
        press(pad, machine, frame, INPUT_CROSS);
    }
    return 1;
}

static int press_the_rewind_button(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    if (!script_soul_rewind_button_waits(machine)) {
        return 0;
    }
    if (pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
        press(pad, machine, frame, INPUT_CROSS);
    }
    return 1;
}

static int route_step_chooses(const struct script_machine *machine, int *panel)
{
    const struct script_route_step *step;

    step = script_soul_route_step(machine);
    if (step == NULL || step->action != SCRIPT_ROUTE_CHOOSE) {
        return 0;
    }
    *panel = script_choice_find(machine, step->object);
    return *panel != SCRIPT_CHOICE_NONE;
}

static int choose_a_panel(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    int wanted;

    if (!machine->choice.is_open) {
        pad->choice_was_open = 0;
        return 0;
    }
    if (!pad->choice_was_open) {
        pad->choice_was_open = 1;
        pad->choice_seen_at = machine->tick;
    }
    if (machine->tick - pad->choice_seen_at < TICKS_TO_READ_A_CHOICE) {
        return 1;
    }
    if (route_step_chooses(machine, &wanted)) {
        if (!script_soul_route_step_has_begun(machine)) {
            return 1;
        }
    } else {
        wanted = script_choice_first_unseen(machine);
    }
    if (wanted == SCRIPT_CHOICE_CANCEL) {
        if (pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_CIRCLE);
        }
        return 1;
    }
    if (wanted == SCRIPT_CHOICE_NONE) {
        return 1;
    }
    if (machine->choice.cursor != wanted) {
        if (pad_may_press(pad, machine, TICKS_BETWEEN_CURSOR_MOVES)) {
            press(pad, machine, frame, INPUT_DOWN);
        }
        return 1;
    }
    if (pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
        press(pad, machine, frame, INPUT_CROSS);
    }
    return 1;
}

static void push_the_stick_toward_the_next_core(const struct script_machine *machine, const struct script_route_step *step, struct input_state *frame)
{
    int target;
    int hop_object;
    int hop_slot;
    float core_x;
    float core_y;
    float along_x;
    float along_y;
    float length;

    if (!machine->soul.is_on) {
        return;
    }
    target = script_machine_index_of_object(machine, step->object);
    if (target == SCRIPT_NO_OBJECT || !script_soul_route_hop(machine, target, step->slot, &hop_object, &hop_slot)) {
        return;
    }
    script_soul_core_position(machine, hop_object, hop_slot, &core_x, &core_y);
    along_x = core_x - machine->soul.tip_x;
    along_y = core_y - machine->soul.tip_y;
    length = sqrtf(along_x * along_x + along_y * along_y);
    if (length < TIP_IS_ON_THE_CORE_DS_PIXELS) {
        return;
    }
    frame->stick_x = along_x / length * STICK_AXIS_FOR_FULL_SPEED;
    frame->stick_y = along_y / length * STICK_AXIS_FOR_FULL_SPEED;
}

static int a_character_pill_is_up(const struct script_machine *machine)
{
    return script_pills_is_up(machine, SCRIPT_PILL_JOWD) || script_pills_is_up(machine, SCRIPT_PILL_MISSILE) || script_pills_is_up(machine, SCRIPT_PILL_CICEL);
}

static void play_the_route_step(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    const struct script_route_step *step;
    int group;

    step = script_soul_route_step(machine);
    if (step == NULL || !script_soul_route_step_has_begun(machine)) {
        return;
    }
    switch (step->action) {
    case SCRIPT_ROUTE_MOVE:
        push_the_stick_toward_the_next_core(machine, step, frame);
        break;
    case SCRIPT_ROUTE_TRICK:
        if (script_pills_is_up(machine, SCRIPT_PILL_TRICK) && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_CROSS);
        }
        break;
    case SCRIPT_ROUTE_TOUCH:
        if (pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_CROSS);
        }
        break;
    case SCRIPT_ROUTE_GHOST_ON:
        if (script_pills_is_up(machine, SCRIPT_PILL_GHOST) && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_L);
        }
        break;
    case SCRIPT_ROUTE_GHOST_OFF:
        if (script_pills_is_up(machine, SCRIPT_PILL_BACK) && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_CIRCLE);
        }
        break;
    case SCRIPT_ROUTE_TAP_BALLOON:
        group = script_soul_route_balloon_group(machine, step->object);
        if (group != 0 && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, group == SCRIPT_TAP_GROUP_RIGHT ? INPUT_SQUARE : INPUT_TRIANGLE);
        }
        break;
    case SCRIPT_ROUTE_CHARACTER:
        if (a_character_pill_is_up(machine) && pad_may_press(pad, machine, TICKS_BETWEEN_PRESSES)) {
            press(pad, machine, frame, INPUT_TRIANGLE);
        }
        break;
    default:
        break;
    }
}

static int scroll_for_the_camera_tutorial(const struct script_machine *machine, struct input_state *frame)
{
    const struct script_route_step *step;

    step = script_soul_route_step(machine);
    if (step == NULL || step->action != SCRIPT_ROUTE_CAMERA || !script_soul_route_step_has_begun(machine) || !machine->soul.camera_tutorial_is_on) {
        return 0;
    }
    frame->held = frame->held | INPUT_RIGHT;
    return 1;
}

void script_route_pad_frame(struct script_route_pad *pad, const struct script_machine *machine, struct input_state *frame)
{
    memset(frame, 0, sizeof(*frame));
    if (scroll_for_the_camera_tutorial(machine, frame)) {
        return;
    }
    if (press_the_rewind_button(pad, machine, frame)) {
        return;
    }
    if (read_the_page(pad, machine, frame)) {
        return;
    }
    if (retry_the_game_over(pad, machine, frame)) {
        return;
    }
    if (choose_a_panel(pad, machine, frame)) {
        return;
    }
    play_the_route_step(pad, machine, frame);
}
