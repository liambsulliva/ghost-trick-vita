#include <math.h>
#include <string.h>
#include <stddef.h>
#include "script_machine.h"

#define FATE_LEVEL_FULL 5
#define BEAM_MODE_COVER_ALPHA 8u
#define BEAM_CREATE_COVER_ALPHA_WORD 13
#define FIXED_POINT_BITS 12
#define COVER_ALPHA_FULL 31.0f
#define SOUL_CAMERA_SPEED_FIXED 0x6000
#define FIXED_ONE 4096.0f
#define RADIUS_UNITS_TO_PIXELS (3.0f / 128.0f)
#define BEAM_MODE_WIDTH 1
#define BEAM_MODE_BASE 4
#define IRIS_MODE_RADIUS 1
#define MASK_ALPHA_FULL 0x1f
#define KEEP_CAMERA_AXIS 0xffffu
#define FIXED_ONE_INT 4096
#define CAMERA_CUT_SPEED 0x3e8000
#define DS_VIEW_WIDTH 256
#define DS_VIEW_HEIGHT 192
#define FIRST_CORE_NODE 10
#define RADIANS_TO_DEGREES 57.29578f
#define CAMERA_BAND_UNUSED 0xffff
#define CAMERA_BAND_SNAP_MARGIN 16
#define CAMERA_BAND_SNAP_MOST 4
#define SOUL_CAMERA_RETURN_SPEED_FIXED 0x4000

static int signed_word(unsigned int word)
{
    return word >= 0x8000u ? (int)word - 0x10000 : (int)word;
}

static float fixed_to_float(unsigned int value)
{
    return (float)(int)value / FIXED_ONE;
}

static float radius_to_pixels(unsigned int value)
{
    return (float)value * RADIUS_UNITS_TO_PIXELS;
}

void script_machine_node_position(const struct script_object *object, int node, float *x, float *y)
{
    float node_x;
    float node_y;

    node_x = 0.0f;
    node_y = 0.0f;
    if (node >= FIRST_CORE_NODE) {
        node_x = (float)(node == FIRST_CORE_NODE + 1 && object->actor.has_second_core ? object->actor.second_core_x : object->actor.core_x);
        node_y = (float)(node == FIRST_CORE_NODE + 1 && object->actor.has_second_core ? object->actor.second_core_y : object->actor.core_y);
    } else if (node >= 0 && node < SCRIPT_ACTOR_POINT_MAX) {
        node_x = (float)object->actor.points[node][0];
        node_y = (float)object->actor.points[node][1];
    }
    *x = (float)object->actor.x + node_x;
    *y = (float)object->actor.y + node_y;
}

static void anchor_effect(struct script_machine *machine, const struct script_object *object, const char *name, int node, int aim_node, float offset_x, float offset_y)
{
    int index;

    for (index = 0; index < SCRIPT_ANCHORED_EFFECT_MAX; index++) {
        struct script_anchored_effect *anchored;

        anchored = &machine->anchored_effects[index];
        if (anchored->name == NULL) {
            anchored->name = name;
            anchored->object_index = (int)(object - machine->objects);
            anchored->node = node;
            anchored->aim_node = aim_node;
            anchored->offset_x = offset_x;
            anchored->offset_y = offset_y;
            return;
        }
    }
}

void script_machine_move_anchored_effects(struct script_machine *machine)
{
    int index;

    if (machine->outputs->effect_moved == NULL) {
        return;
    }
    for (index = 0; index < SCRIPT_ANCHORED_EFFECT_MAX; index++) {
        const struct script_anchored_effect *anchored;
        const struct script_object *object;
        float x;
        float y;
        float aim_degrees;

        anchored = &machine->anchored_effects[index];
        if (anchored->name == NULL) {
            continue;
        }
        object = &machine->objects[anchored->object_index];
        script_machine_node_position(object, anchored->node, &x, &y);
        aim_degrees = 0.0f;
        if (anchored->aim_node >= 0) {
            float aim_x;
            float aim_y;

            script_machine_node_position(object, anchored->aim_node, &aim_x, &aim_y);
            aim_degrees = atan2f(aim_y - y, aim_x - x) * RADIANS_TO_DEGREES;
        }
        machine->outputs->effect_moved(anchored->name, x + anchored->offset_x, y + anchored->offset_y, aim_degrees);
    }
}

void script_machine_spawn_table_effect(struct script_machine *machine, struct script_object *object, int effect_id, float offset_x, float offset_y, const char *name)
{
    float x;
    float y;

    if (machine->outputs->effect_spawned == NULL || name == NULL) {
        return;
    }
    script_machine_node_position(object, 0, &x, &y);
    machine->outputs->effect_spawned(effect_id, name, x + offset_x, y + offset_y);
}

static void spawn_effect(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const struct script_object *object;
    const char *name;
    float x;
    float y;
    float offset_x;
    float offset_y;

    name = script_machine_string(machine, vm, words[9]);
    if (name == NULL) {
        return;
    }
    script_soul_effect_spawned(machine, (int)words[0], name);
    if (machine->outputs->effect_spawned == NULL) {
        return;
    }
    offset_x = fixed_to_float(script_machine_u32(words, 1));
    offset_y = fixed_to_float(script_machine_u32(words, 3));
    x = offset_x;
    y = offset_y;
    object = script_machine_current_object(machine, vm);
    if (object != NULL && object->actor.exists && signed_word(words[10]) >= 0) {
        script_machine_node_position(object, signed_word(words[10]), &x, &y);
        x = x + offset_x;
        y = y + offset_y;
    }
    machine->outputs->effect_spawned((int)words[0], name, x, y);
    if (object != NULL && object->actor.exists && signed_word(words[10]) >= 0 && words[15] == 0) {
        anchor_effect(machine, object, name, signed_word(words[10]), signed_word(words[11]), offset_x, offset_y);
    }
}

static void delete_effect(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL) {
        int index;

        script_pills_effect_deleted(machine, name);
        script_soul_effect_deleted(machine, name);
        for (index = 0; index < SCRIPT_ANCHORED_EFFECT_MAX; index++) {
            if (machine->anchored_effects[index].name != NULL && (strcmp(name, "*") == 0 || strcmp(machine->anchored_effects[index].name, name) == 0)) {
                machine->anchored_effects[index].name = NULL;
            }
        }
    }
    if (name != NULL && machine->outputs->effect_deleted != NULL) {
        machine->outputs->effect_deleted(name);
    }
}

static float cover_alpha_of(unsigned int fixed_alpha)
{
    return (float)(fixed_alpha >> FIXED_POINT_BITS) / COVER_ALPHA_FULL;
}

static void create_beam(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL && machine->outputs->beam_created != NULL) {
        machine->outputs->beam_created(name, fixed_to_float(script_machine_u32(words, 1)), fixed_to_float(script_machine_u32(words, 3)), fixed_to_float(script_machine_u32(words, 5)), fixed_to_float(script_machine_u32(words, 7)), radius_to_pixels(script_machine_u32(words, 9)), cover_alpha_of(script_machine_u32(words, BEAM_CREATE_COVER_ALPHA_WORD)));
    }
}

static void ramp_beam(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;
    unsigned int mode;
    int ticks;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    ticks = (int)script_machine_u32(words, 5);
    mode = script_machine_u32(words, 7);
    if (mode == BEAM_MODE_WIDTH) {
        if (machine->outputs->beam_width_ramped != NULL) {
            machine->outputs->beam_width_ramped(name, radius_to_pixels(script_machine_u32(words, 1)), ticks);
        }
    } else if (mode == BEAM_MODE_BASE) {
        if (machine->outputs->beam_base_ramped != NULL) {
            machine->outputs->beam_base_ramped(name, fixed_to_float(script_machine_u32(words, 1)), fixed_to_float(script_machine_u32(words, 3)), ticks);
        }
    } else if (mode == BEAM_MODE_COVER_ALPHA) {
        if (machine->outputs->beam_cover_ramped != NULL) {
            machine->outputs->beam_cover_ramped(name, cover_alpha_of(script_machine_u32(words, 1)), ticks);
        }
    } else {
        script_machine_note(machine, vm, "beam_mode_not_ported", name);
    }
}

static void delete_beam(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL && machine->outputs->beam_deleted != NULL) {
        machine->outputs->beam_deleted(name);
    }
}

static void create_iris(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL && machine->outputs->iris_created != NULL) {
        machine->outputs->iris_created(name, fixed_to_float(script_machine_u32(words, 1)), fixed_to_float(script_machine_u32(words, 3)), radius_to_pixels(script_machine_u32(words, 5)));
    }
}

static void ramp_iris(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    if (script_machine_u32(words, 7) != IRIS_MODE_RADIUS) {
        script_machine_note(machine, vm, "iris_mode_not_ported", name);
        return;
    }
    if (machine->outputs->iris_radius_ramped != NULL) {
        machine->outputs->iris_radius_ramped(name, radius_to_pixels(script_machine_u32(words, 1)), (int)script_machine_u32(words, 5));
    }
}

static void delete_iris(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL && machine->outputs->iris_deleted != NULL) {
        machine->outputs->iris_deleted(name);
    }
}

static int clamped_axis(int value, int room_size, int view_size)
{
    int largest;

    if (room_size <= 0) {
        return value;
    }
    largest = room_size > view_size ? room_size - view_size : 0;
    if (value > largest) {
        return largest;
    }
    return value < 0 ? 0 : value;
}

static int clamped_x(const struct script_machine *machine, int x)
{
    return clamped_axis(x, machine->room >= 0 && machine->room < SCRIPT_ROOM_ID_MAX ? machine->room_bounds[machine->room].width : 0, DS_VIEW_WIDTH);
}

static int clamped_y(const struct script_machine *machine, int y)
{
    return clamped_axis(y, machine->room >= 0 && machine->room < SCRIPT_ROOM_ID_MAX ? machine->room_bounds[machine->room].height : 0, DS_VIEW_HEIGHT);
}

static void reset_camera_bands(struct script_machine *machine)
{
    int index;

    for (index = 0; index < SCRIPT_CAMERA_BAND_MAX; index++) {
        machine->camera_band_keys[index] = CAMERA_BAND_UNUSED;
        machine->camera_band_values[index] = CAMERA_BAND_UNUSED;
    }
}

static void set_camera_band(struct script_machine *machine, int key, int value)
{
    int index;
    int later;

    for (index = 0; index < SCRIPT_CAMERA_BAND_MAX; index++) {
        if (machine->camera_band_keys[index] == key) {
            machine->camera_band_values[index] = value;
            return;
        }
        if (machine->camera_band_keys[index] > key) {
            for (later = SCRIPT_CAMERA_BAND_MAX - 1; later > index; later--) {
                machine->camera_band_keys[later] = machine->camera_band_keys[later - 1];
                machine->camera_band_values[later] = machine->camera_band_values[later - 1];
            }
            machine->camera_band_keys[index] = key;
            machine->camera_band_values[index] = value;
            return;
        }
    }
}

static int band_holding(const struct script_machine *machine, int object_y)
{
    int index;

    for (index = 0; index < SCRIPT_CAMERA_BAND_MAX; index++) {
        if (machine->camera_band_keys[index] > object_y && machine->camera_band_values[index] < object_y) {
            return index;
        }
    }
    return -1;
}

int script_machine_camera_top_for(const struct script_machine *machine, int object_y)
{
    int band;
    int top;

    band = band_holding(machine, object_y);
    top = band >= 0 ? machine->camera_band_keys[band] - DS_VIEW_HEIGHT : object_y - DS_VIEW_HEIGHT / 2;
    return machine->world_origin_y + (top < 0 ? 0 : top);
}

int script_machine_camera_band_snap(const struct script_machine *machine, int object_y)
{
    int band;
    int wanted_bottom;
    int bottom;
    int step;

    if (machine->camera_snap_is_off) {
        return 0;
    }
    band = band_holding(machine, object_y);
    if (band < 0 || machine->camera_band_keys[band] - CAMERA_BAND_SNAP_MARGIN < object_y) {
        return 0;
    }
    wanted_bottom = machine->camera_band_keys[band] + machine->world_origin_y;
    bottom = machine->camera_y + DS_VIEW_HEIGHT;
    step = wanted_bottom - bottom;
    if (step > CAMERA_BAND_SNAP_MOST) {
        return CAMERA_BAND_SNAP_MOST;
    }
    return step < -CAMERA_BAND_SNAP_MOST ? -CAMERA_BAND_SNAP_MOST : step;
}

static void load_room(struct script_machine *machine, const unsigned int *words)
{
    machine->room = (int)words[0];
    machine->pan_is_active = 0;
    reset_camera_bands(machine);
    if (machine->room >= 0 && machine->room < SCRIPT_ROOM_ID_MAX) {
        const struct script_room_bounds *bounds;
        int index;

        bounds = &machine->room_bounds[machine->room];
        for (index = 0; index < bounds->band_count; index++) {
            set_camera_band(machine, bounds->band_keys[index], bounds->band_values[index]);
        }
    }
    if (words[1] != KEEP_CAMERA_AXIS) {
        machine->camera_x = machine->world_origin_x + signed_word(words[1]);
    }
    if (words[2] != KEEP_CAMERA_AXIS) {
        machine->camera_y = machine->world_origin_y + signed_word(words[2]);
    }
    if (machine->outputs->room_loaded != NULL) {
        machine->outputs->room_loaded((int)words[0], machine->camera_x, machine->camera_y);
    }
}

static void cut_camera(struct script_machine *machine, const unsigned int *words)
{
    machine->pan_is_active = 0;
    machine->camera_x = clamped_x(machine, machine->world_origin_x + signed_word(words[0]));
    machine->camera_y = clamped_y(machine, machine->world_origin_y + signed_word(words[1]));
    if (machine->outputs->camera_cut != NULL) {
        machine->outputs->camera_cut(machine->camera_x, machine->camera_y);
    }
}

static void begin_pan(struct script_machine *machine, int target_x, int target_y, int speed_fixed, int sends_hosei_end)
{
    int delta_x;
    int delta_y;
    int longest;
    int ticks;

    target_x = clamped_x(machine, target_x);
    target_y = clamped_y(machine, target_y);
    delta_x = target_x - machine->camera_x;
    delta_y = target_y - machine->camera_y;
    if (speed_fixed >= CAMERA_CUT_SPEED || (delta_x == 0 && delta_y == 0)) {
        machine->camera_x = target_x;
        machine->camera_y = target_y;
        machine->pan_is_active = 0;
        if (machine->outputs->camera_moved != NULL) {
            machine->outputs->camera_moved(target_x, target_y);
        }
        script_machine_send(machine, "*", "camera", "end");
        if (sends_hosei_end) {
            script_machine_send(machine, "*", "SYSTEM", "hosei end");
        }
        return;
    }
    if (speed_fixed <= 0) {
        speed_fixed = FIXED_ONE_INT;
    }
    longest = delta_x < 0 ? -delta_x : delta_x;
    if ((delta_y < 0 ? -delta_y : delta_y) > longest) {
        longest = delta_y < 0 ? -delta_y : delta_y;
    }
    ticks = (longest * FIXED_ONE_INT) / speed_fixed;
    if (ticks < 1) {
        ticks = 1;
    }
    machine->pan_is_active = 1;
    machine->pan_ticks_left = ticks;
    machine->pan_x_fixed = machine->camera_x * FIXED_ONE_INT;
    machine->pan_y_fixed = machine->camera_y * FIXED_ONE_INT;
    machine->pan_step_x_fixed = delta_x * FIXED_ONE_INT / ticks;
    machine->pan_step_y_fixed = delta_y * FIXED_ONE_INT / ticks;
    machine->pan_target_x = target_x;
    machine->pan_target_y = target_y;
    machine->pan_sends_hosei_end = sends_hosei_end;
}

static void pan_to_position(struct script_machine *machine, const unsigned int *words)
{
    int target_x;
    int target_y;

    target_x = words[0] == KEEP_CAMERA_AXIS ? machine->camera_x : machine->world_origin_x + signed_word(words[0]);
    target_y = words[1] == KEEP_CAMERA_AXIS ? machine->camera_y : machine->world_origin_y + signed_word(words[1]);
    begin_pan(machine, target_x, target_y, (int)script_machine_u32(words, 2), 0);
}

static void pan_to_object(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;
    const struct script_object *object;
    float node_x;
    float node_y;
    int target_x;
    int target_y;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    object = script_machine_find_object(machine, name);
    if (object == NULL || !object->actor.exists) {
        script_machine_send(machine, "*", "SYSTEM", "hosei end");
        return;
    }
    if (signed_word(words[1]) < 0 && machine->soul.active_object >= 0) {
        script_machine_node_position(&machine->objects[machine->soul.active_object], FIRST_CORE_NODE + machine->soul.active_slot, &node_x, &node_y);
    } else {
        script_machine_node_position(object, signed_word(words[1]), &node_x, &node_y);
    }
    target_x = signed_word(words[2]) < 0 ? machine->camera_x : machine->world_origin_x + (int)node_x - signed_word(words[2]);
    target_y = signed_word(words[3]) < 0 ? machine->camera_y : machine->world_origin_y + (int)node_y - signed_word(words[3]);
    begin_pan(machine, target_x, target_y, (int)script_machine_u32(words, 4), 1);
}

void script_machine_set_world_origin(struct script_machine *machine, int x, int y)
{
    machine->world_origin_x = x;
    machine->world_origin_y = y;
    if (machine->outputs->world_origin_changed != NULL) {
        machine->outputs->world_origin_changed(x, y);
    }
}

void script_machine_pan_camera_to(struct script_machine *machine, int center_x, int center_y)
{
    begin_pan(machine, machine->world_origin_x + center_x - DS_VIEW_WIDTH / 2, script_machine_camera_top_for(machine, center_y), SOUL_CAMERA_SPEED_FIXED, 0);
}

void script_machine_pan_camera_back_to(struct script_machine *machine, int x, int y)
{
    begin_pan(machine, x, y, SOUL_CAMERA_RETURN_SPEED_FIXED, 0);
}

void script_machine_scroll_camera(struct script_machine *machine, int delta_x, int delta_y)
{
    machine->camera_x = clamped_x(machine, machine->camera_x + delta_x);
    machine->camera_y = clamped_y(machine, machine->camera_y + delta_y);
    if (machine->outputs->camera_moved != NULL) {
        machine->outputs->camera_moved(machine->camera_x, machine->camera_y);
    }
}

void script_machine_update_camera(struct script_machine *machine)
{
    if (machine->shake_ticks_left > 0) {
        machine->shake_ticks_left = machine->shake_ticks_left - 1;
    }
    if (!machine->pan_is_active || (machine->pause_bit && !machine->soul.is_on)) {
        return;
    }
    machine->pan_ticks_left = machine->pan_ticks_left - 1;
    machine->pan_x_fixed = machine->pan_x_fixed + machine->pan_step_x_fixed;
    machine->pan_y_fixed = machine->pan_y_fixed + machine->pan_step_y_fixed;
    machine->camera_x = machine->pan_x_fixed / FIXED_ONE_INT;
    machine->camera_y = machine->pan_y_fixed / FIXED_ONE_INT;
    if (machine->pan_ticks_left <= 0) {
        machine->camera_x = machine->pan_target_x;
        machine->camera_y = machine->pan_target_y;
        machine->pan_is_active = 0;
        script_machine_send(machine, "*", "camera", "end");
        if (machine->pan_sends_hosei_end) {
            script_machine_send(machine, "*", "SYSTEM", "hosei end");
        }
    }
    if (machine->outputs->camera_moved != NULL) {
        machine->outputs->camera_moved(machine->camera_x, machine->camera_y);
    }
}

enum script_step_result script_ops_view_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    unsigned int next_word;
    unsigned int length_with_force;

    switch (op) {
    case 0xa:
        if (machine->fade_count > 0) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0x11:
    case 0x12:
        if (machine->outputs->mask_fade_color_changed != NULL) {
            machine->outputs->mask_fade_color_changed(1);
        }
        script_fades_set_mask(machine, op == 0x11 ? 0 : MASK_ALPHA_FULL, (int)words[0]);
        break;
    case 0xcb:
        machine->camera_snap_is_off = 0;
        break;
    case 0xcc:
        machine->camera_snap_is_off = 1;
        break;
    case 0xd5:
        set_camera_band(machine, signed_word(words[0]), signed_word(words[1]));
        break;
    case 0xd6:
        reset_camera_bands(machine);
        break;
    case 0x13:
        if (machine->outputs->screen_filter_changed != NULL) {
            machine->outputs->screen_filter_changed((int)(words[0] & 7));
        }
        break;
    case 0x2a:
        machine->shake_level = (int)words[0];
        machine->shake_ticks_left = (int)words[1];
        if (machine->outputs->shake_changed != NULL) {
            machine->outputs->shake_changed((int)words[0], (int)words[1]);
        }
        break;
    case 0x2b:
        script_fades_add(machine, vm, 2, 4);
        break;
    case 0xc7:
        pan_to_position(machine, words);
        break;
    case 0xce:
        pan_to_object(machine, vm, words);
        break;
    case 0xcf:
        if (machine->pan_is_active) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0xd0:
        machine->pan_is_active = 0;
        break;
    case 0xd7:
        script_machine_scroll_camera(machine, signed_word(words[0]) - machine->world_origin_x, signed_word(words[1]) - machine->world_origin_y);
        script_machine_set_world_origin(machine, signed_word(words[0]), signed_word(words[1]));
        break;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
        script_fades_add(machine, vm, (int)op - 0xb, (int)words[0]);
        break;
    case 0xf:
        script_fades_set_mask(machine, 0, (int)words[0]);
        break;
    case 0x10:
        script_fades_set_mask(machine, MASK_ALPHA_FULL, (int)words[0]);
        break;
    case 0xb0:
        spawn_effect(machine, vm, words);
        break;
    case 0xb1:
        delete_effect(machine, vm, words);
        break;
    case 0x6f:
    case 0x70:
        if (machine->outputs->film_frame_changed != NULL) {
            machine->outputs->film_frame_changed(op == 0x6f);
        }
        break;
    case 0xb2:
        if (script_machine_string(machine, vm, words[0]) != NULL) {
            script_soul_effect_signalled(machine, script_machine_string(machine, vm, words[0]));
            if (machine->outputs->effect_signalled != NULL) {
                machine->outputs->effect_signalled(script_machine_string(machine, vm, words[0]));
            }
        }
        break;
    case 0xd8:
        script_soul_set_countdown(machine, (int)words[0]);
        break;
    case 0xd9:
        script_soul_set_countdown_interval(machine, signed_word(words[0]));
        break;
    case 0x102:
        script_soul_set_fate_level(machine, FATE_LEVEL_FULL);
        break;
    case 0x103:
        script_soul_set_fate_level(machine, (int)words[0]);
        break;
    case 0x104:
        script_soul_set_fate_level(machine, machine->fate_level + 1);
        break;
    case 0x105:
        script_soul_set_fate_level(machine, machine->fate_level - 1);
        break;
    case 0xbb:
        create_beam(machine, vm, words);
        break;
    case 0xbc:
        ramp_beam(machine, vm, words);
        break;
    case 0xbd:
        delete_beam(machine, vm, words);
        break;
    case 0xbe:
        create_iris(machine, vm, words);
        break;
    case 0xbf:
        ramp_iris(machine, vm, words);
        break;
    case 0xc0:
        delete_iris(machine, vm, words);
        break;
    case 0xc1:
        load_room(machine, words);
        length_with_force = 4;
        if (script_file_read_word(&vm->source->file, vm->pc + 4, &next_word) == SCRIPT_FILE_OK && next_word == 0xc3) {
            length_with_force = 5;
        }
        vm->pc = vm->pc + length_with_force;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0xc8:
        cut_camera(machine, words);
        break;
    default:
        break;
    }
    return SCRIPT_STEP_NEXT;
}
