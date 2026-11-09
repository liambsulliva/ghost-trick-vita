#include <string.h>
#include "ds_units.h"
#include "run_log.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_input.h"
#include "script_machine.h"
#include "script_ui_view.h"
#include "stick_aim.h"

void script_input_reset(struct script_input *input)
{
    memset(input, 0, sizeof(*input));
}

static int axis_of(unsigned int buttons, unsigned int negative, unsigned int positive)
{
    if (buttons & positive) {
        return 1;
    }
    return (buttons & negative) ? -1 : 0;
}

static void send_directions(struct script_input *input, struct script_machine *machine, const struct input_state *frame)
{
    int pressed_x;
    int pressed_y;
    int held_x;
    int held_y;
    int stick_is_out;

    pressed_x = axis_of(frame->pressed, INPUT_LEFT, INPUT_RIGHT);
    pressed_y = axis_of(frame->pressed, INPUT_UP, INPUT_DOWN);
    stick_is_out = frame->stick_x != 0.0f || frame->stick_y != 0.0f;
    if (pressed_x == 0 && pressed_y == 0 && stick_is_out && !input->stick_was_out) {
        stick_aim_first_push(frame->stick_x, frame->stick_y, &pressed_x, &pressed_y);
    }
    input->stick_was_out = stick_is_out;
    if (pressed_x != 0 || pressed_y != 0) {
        script_soul_note_direction(machine, pressed_x, pressed_y);
    }
    held_x = axis_of(frame->held, INPUT_LEFT, INPUT_RIGHT);
    held_y = axis_of(frame->held, INPUT_UP, INPUT_DOWN);
    if (held_x != 0 || held_y != 0) {
        script_soul_set_held(machine, (float)held_x, (float)held_y);
        return;
    }
    script_soul_set_held(machine, stick_aim_axis_speed(frame->stick_x), stick_aim_axis_speed(frame->stick_y));
}

static void press_the_pill(struct script_machine *machine, int pill)
{
    switch (pill) {
    case SCRIPT_PILL_GHOST:
    case SCRIPT_PILL_BACK:
        script_soul_note_ghost(machine);
        break;
    case SCRIPT_PILL_TRICK:
        script_machine_note_press(machine);
        break;
    default:
        script_soul_note_pill(machine, (enum script_pill_id)pill);
        break;
    }
}

static void touch_began(struct script_input *input, struct script_machine *machine, float x, float y)
{
    int pill;
    int balloon_kind;

    input->touch_is_dragging = 0;
    pill = script_ui_view_pill_at(x, y);
    balloon_kind = script_ui_view_tap_balloon_kind_at(x, y);
    run_log_write("touch began x=%d y=%d tick=%u pill=%d balloon=%d wants_tap=%d", (int)x, (int)y, machine->tick, pill, balloon_kind, script_soul_wants_a_tap(machine));
    if (pill >= 0) {
        press_the_pill(machine, pill);
        return;
    }
    if (balloon_kind != 0) {
        script_soul_note_tap(machine, balloon_kind == SCRIPT_BALLOON_KIND_TAP_DOTS ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT);
        return;
    }
    if (machine->soul.rewind_select_ticks > 0) {
        if (script_ui_view_rewind_button_is_at(x, y)) {
            script_machine_note_press(machine);
        }
        return;
    }
    if (script_soul_wants_a_tap(machine)) {
        script_machine_note_press(machine);
        return;
    }
    input->touch_is_dragging = 1;
    input->touch_last_x = x;
    input->touch_last_y = y;
}

static void touch_dragged(struct script_input *input, struct script_machine *machine, float x, float y)
{
    float world_x;
    float world_y;

    if (machine->soul.is_on) {
        scene_view_screen_to_world(scene_stage_view(), x, y, &world_x, &world_y);
        script_soul_set_touch(machine, 1, world_x, world_y);
    } else {
        script_soul_drag_camera(machine, (input->touch_last_x - x) / VITA_PIXELS_PER_DS_PIXEL, (input->touch_last_y - y) / VITA_PIXELS_PER_DS_PIXEL);
    }
    input->touch_last_x = x;
    input->touch_last_y = y;
}

static void send_touch(struct script_input *input, struct script_machine *machine, const struct input_state *frame)
{
    if (machine->auto_press != SCRIPT_AUTO_OFF) {
        input->touch_was_down = 0;
        return;
    }
    if (frame->touch_is_down && !input->touch_was_down) {
        touch_began(input, machine, frame->touch_x, frame->touch_y);
    }
    if (frame->touch_is_down && input->touch_is_dragging) {
        touch_dragged(input, machine, frame->touch_x, frame->touch_y);
    }
    if (!frame->touch_is_down && input->touch_was_down) {
        input->touch_is_dragging = 0;
        script_soul_set_touch(machine, 0, 0.0f, 0.0f);
    }
    input->touch_was_down = frame->touch_is_down;
}

static void send_buttons(struct script_machine *machine, unsigned int pressed)
{
    if (pressed & INPUT_L) {
        script_soul_note_ghost(machine);
    }
    if (pressed & INPUT_CIRCLE) {
        if (machine->choice.is_open) {
            script_choice_note_cancel(machine);
        } else if (machine->soul.is_on) {
            script_soul_note_ghost(machine);
        }
    }
    if (pressed & INPUT_SQUARE) {
        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_RIGHT);
    }
    if (pressed & INPUT_TRIANGLE) {
        script_soul_note_tap(machine, SCRIPT_TAP_GROUP_LEFT);
        script_soul_note_character_button(machine);
    }
    if (pressed & INPUT_R) {
        script_soul_note_pill(machine, SCRIPT_PILL_HOURGLASS);
    }
    if (pressed & INPUT_SELECT) {
        script_soul_note_pill(machine, SCRIPT_PILL_NOTEBOOK);
    }
    if (pressed & INPUT_CROSS) {
        script_machine_note_press(machine);
    }
}

void script_input_frame(struct script_input *input, struct script_machine *machine, const struct input_state *frame)
{
    send_directions(input, machine, frame);
    send_touch(input, machine, frame);
    send_buttons(machine, frame->pressed);
}
