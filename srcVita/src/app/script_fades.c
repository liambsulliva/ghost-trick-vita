#include <string.h>
#include "script_machine.h"

#define FADE_IN_KIND 0
#define FADE_OUT_KIND 1
#define WHITE_IN_KIND 2
#define WHITE_OUT_KIND 3
#define LEVEL_FULL 0x100000
#define LEVEL_FULL_WHOLE 256
#define LOW_BRIGHTNESS_FOR_HALF_STRENGTH 8
#define SHORT_FRAMES_FOR_HALF_STRENGTH 12
#define FULL_SCALE 0x100
#define HALF_SCALE 0x80
#define BRIGHTNESS_SCALE 16
#define MASK_FIXED_BITS 12
#define MASK_FIXED_ONE (1 << MASK_FIXED_BITS)

static int brightness_from_level(const struct script_fade *fade)
{
    int whole;

    whole = fade->level >> 12;
    switch (fade->kind) {
    case FADE_IN_KIND:
        return whole != 0 ? -((whole * BRIGHTNESS_SCALE + 255) >> 8) : 0;
    case FADE_OUT_KIND:
        whole = LEVEL_FULL_WHOLE - whole;
        return whole != 0 ? -((whole * BRIGHTNESS_SCALE + 255) >> 8) : 0;
    case WHITE_IN_KIND:
        return (whole * BRIGHTNESS_SCALE) >> 8;
    default:
        whole = LEVEL_FULL_WHOLE - whole;
        return (whole * BRIGHTNESS_SCALE) >> 8;
    }
}

void script_fades_add(struct script_machine *machine, const struct script_vm *vm, int kind, int frames)
{
    struct script_fade *fade;
    int scale;

    if (machine->fade_count >= SCRIPT_FADE_MAX) {
        script_machine_note(machine, vm, "too_many_fades", "");
        return;
    }
    if (kind == FADE_IN_KIND && machine->brightness > 0) {
        kind = WHITE_IN_KIND;
    } else if (kind == WHITE_IN_KIND && machine->brightness < 0) {
        kind = FADE_IN_KIND;
    }
    scale = FULL_SCALE;
    if (kind == WHITE_IN_KIND && machine->brightness <= LOW_BRIGHTNESS_FOR_HALF_STRENGTH && frames < SHORT_FRAMES_FOR_HALF_STRENGTH) {
        scale = HALF_SCALE;
    }
    fade = &machine->fades[machine->fade_count];
    fade->kind = kind;
    fade->level = frames == 0 ? 0 : (LEVEL_FULL / FULL_SCALE) * scale;
    fade->step = frames == 0 ? 0 : fade->level / frames;
    machine->fade_count = machine->fade_count + 1;
}

void script_fades_end_mask(struct script_machine *machine)
{
    machine->mask_fade_is_alive = 0;
    machine->mask_fade_alpha = 0;
    machine->mask_fade_fixed = 0;
    machine->mask_fade_frames = 0;
}

void script_fades_set_mask(struct script_machine *machine, int target_alpha, int frames)
{
    int difference;

    if (!machine->mask_fade_is_alive) {
        machine->mask_fade_is_alive = 1;
        machine->mask_fade_alpha = 0;
        machine->mask_fade_fixed = 0;
    }
    if (frames == 0) {
        frames = 1;
    }
    difference = (target_alpha - machine->mask_fade_alpha) * MASK_FIXED_ONE;
    machine->mask_fade_frames = frames;
    machine->mask_fade_target = target_alpha;
    machine->mask_fade_step = difference / frames;
}

static void update_mask_fade(struct script_machine *machine)
{
    if (!machine->mask_fade_is_alive || machine->mask_fade_frames <= 0) {
        return;
    }
    machine->mask_fade_frames = machine->mask_fade_frames - 1;
    if (machine->mask_fade_frames == 0) {
        machine->mask_fade_alpha = machine->mask_fade_target;
        machine->mask_fade_fixed = machine->mask_fade_target * MASK_FIXED_ONE;
        if (machine->mask_fade_alpha <= 0) {
            machine->mask_fade_is_alive = 0;
        }
        return;
    }
    machine->mask_fade_fixed = machine->mask_fade_fixed + machine->mask_fade_step;
    machine->mask_fade_alpha = machine->mask_fade_fixed >> MASK_FIXED_BITS;
}

static void update_brightness(struct script_machine *machine)
{
    int kept;
    int index;

    kept = 0;
    for (index = 0; index < machine->fade_count; index++) {
        struct script_fade fade;

        fade = machine->fades[index];
        machine->brightness = brightness_from_level(&fade);
        if (fade.level == 0) {
            continue;
        }
        fade.level = fade.level - fade.step;
        if (fade.level < 0) {
            fade.level = 0;
        }
        machine->fades[kept] = fade;
        kept = kept + 1;
    }
    machine->fade_count = kept;
}

void script_fades_update(struct script_machine *machine)
{
    update_mask_fade(machine);
    update_brightness(machine);
    if (machine->brightness != machine->reported_brightness) {
        machine->reported_brightness = machine->brightness;
        if (machine->outputs->brightness_changed != NULL) {
            machine->outputs->brightness_changed(machine->brightness);
        }
    }
    if (machine->mask_fade_is_alive != machine->reported_mask_fade_is_alive || machine->mask_fade_alpha != machine->reported_mask_fade_alpha) {
        machine->reported_mask_fade_is_alive = machine->mask_fade_is_alive;
        machine->reported_mask_fade_alpha = machine->mask_fade_alpha;
        if (machine->outputs->mask_fade_changed != NULL) {
            machine->outputs->mask_fade_changed(machine->mask_fade_is_alive, machine->mask_fade_alpha);
        }
    }
}
