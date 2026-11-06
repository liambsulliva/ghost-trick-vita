#include <stddef.h>
#include "script_machine.h"

#define FIXED_ONE 4096.0f
#define RADIUS_UNITS_TO_PIXELS (3.0f / 128.0f)
#define CARD_EFFECT_ID 66
#define ZOOM_EFFECT_ID 85
#define STREETLIGHT_EFFECT_ID 90
#define SOUL_SPARK_EFFECT_ID 31
#define CORE_EFFECT_ID 4
#define BEAM_MODE_WIDTH 1
#define BEAM_MODE_BASE 4
#define IRIS_MODE_RADIUS 1
#define MASK_ALPHA_FULL 0x1f
#define KEEP_CAMERA_AXIS 0xffffu

static float fixed_to_float(unsigned int value)
{
    return (float)(int)value / FIXED_ONE;
}

static float radius_to_pixels(unsigned int value)
{
    return (float)value * RADIUS_UNITS_TO_PIXELS;
}

static void note_effect_once(struct script_machine *machine, int id, const char *name)
{
    if (machine->noted_effects[id / 8] & (1u << (id % 8))) {
        return;
    }
    machine->noted_effects[id / 8] = (unsigned char)(machine->noted_effects[id / 8] | (1u << (id % 8)));
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("effect_not_drawn", name);
    }
}

static void spawn_effect(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;
    int id;

    id = (int)words[0];
    name = script_machine_string(machine, vm, words[9]);
    if (name == NULL) {
        return;
    }
    if (id == CARD_EFFECT_ID || id == ZOOM_EFFECT_ID) {
        if (machine->outputs->effect_spawned != NULL) {
            machine->outputs->effect_spawned(id, name);
        }
        return;
    }
    if (id == STREETLIGHT_EFFECT_ID || id == SOUL_SPARK_EFFECT_ID || id == CORE_EFFECT_ID) {
        note_effect_once(machine, id, name);
        return;
    }
    script_machine_halt(machine, vm, "effect_not_ported", name);
}

static void create_beam(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL && machine->outputs->beam_created != NULL) {
        machine->outputs->beam_created(name, fixed_to_float(script_machine_u32(words, 1)), fixed_to_float(script_machine_u32(words, 3)), fixed_to_float(script_machine_u32(words, 5)), fixed_to_float(script_machine_u32(words, 7)), radius_to_pixels(script_machine_u32(words, 9)));
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
    } else {
        script_machine_halt(machine, vm, "beam_mode_not_ported", name);
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
        script_machine_halt(machine, vm, "iris_mode_not_ported", name);
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

static int signed_word(unsigned int word)
{
    return word >= 0x8000u ? (int)word - 0x10000 : (int)word;
}

static void load_room(struct script_machine *machine, const unsigned int *words)
{
    if (words[1] != KEEP_CAMERA_AXIS) {
        machine->camera_x = signed_word(words[1]);
    }
    if (words[2] != KEEP_CAMERA_AXIS) {
        machine->camera_y = signed_word(words[2]);
    }
    if (machine->outputs->room_loaded != NULL) {
        machine->outputs->room_loaded((int)words[0], machine->camera_x, machine->camera_y);
    }
}

static void cut_camera(struct script_machine *machine, const unsigned int *words)
{
    machine->camera_x = signed_word(words[0]);
    machine->camera_y = signed_word(words[1]);
    if (machine->outputs->camera_cut != NULL) {
        machine->outputs->camera_cut(machine->camera_x, machine->camera_y);
    }
}

enum script_step_result script_ops_view_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    unsigned int next_word;
    unsigned int length_with_force;

    switch (op) {
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
