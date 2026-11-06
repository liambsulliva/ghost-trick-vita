#include <string.h>
#include "script_machine.h"

#define MODE_BYTE_NARRATION 0x12
#define FIXED_ONE 4096.0f
#define SOUND_KIND_NONE 0
#define SOUND_KIND_REFUSED 33

void script_narration_report_to_outputs(void *listener, const char *label, enum script_narration_event event, int group)
{
    struct script_machine *machine;

    machine = listener;
    if (machine->outputs->narration_changed != NULL) {
        machine->outputs->narration_changed(label, event, group);
    }
}

static void start_narration(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const struct script_cue_message *message;
    const char *label;

    label = script_machine_string(machine, vm, words[0]);
    if (label == NULL) {
        return;
    }
    message = script_cues_find(vm->source->cues, label);
    if (message == NULL) {
        script_machine_note(machine, vm, "missing_cue", label);
        return;
    }
    machine->pause_bit = 1;
    machine->mode_byte = MODE_BYTE_NARRATION;
    script_narration_start(&machine->narration, vm->source->cues, message, machine->timing);
    machine->narration_started_this_tick = 1;
}

static struct script_balloon *find_slot(struct script_machine *machine)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (!machine->balloons[index].in_use) {
            return &machine->balloons[index];
        }
    }
    return &machine->balloons[0];
}

static void open_balloon(struct script_machine *machine, struct script_vm *vm, const char *label, int kind, int ticks)
{
    struct script_object *object;
    struct script_balloon *balloon;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || !object->actor.exists) {
        return;
    }
    balloon = find_slot(machine);
    memset(balloon, 0, sizeof(*balloon));
    balloon->in_use = 1;
    balloon->owner_object = (int)(object - machine->objects);
    balloon->label = label;
    balloon->kind = kind;
    balloon->timer = ticks;
    balloon->open_is_pending = 1;
}

static void place_balloon(struct script_machine *machine, const struct script_vm *vm, const unsigned int *words)
{
    const struct script_object *object;
    int index;

    object = script_machine_current_object(machine, vm);
    if (object == NULL) {
        return;
    }
    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].owner_object == (int)(object - machine->objects)) {
            machine->balloons[index].offset_x = (float)(int)script_machine_u32(words, 0) / FIXED_ONE;
            machine->balloons[index].offset_y = (float)(int)script_machine_u32(words, 2) / FIXED_ONE;
        }
    }
}

void script_balloons_count_down(struct script_machine *machine)
{
    int index;

    if (machine->pause_bit) {
        return;
    }
    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        struct script_balloon *balloon;

        balloon = &machine->balloons[index];
        if (!balloon->in_use || balloon->timer <= 0) {
            continue;
        }
        balloon->timer = balloon->timer - 1;
        if (balloon->timer == 0) {
            balloon->in_use = 0;
            if (machine->outputs->balloon_closed != NULL) {
                machine->outputs->balloon_closed(index);
            }
        }
    }
}

void script_balloons_report_opened(struct script_machine *machine)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        struct script_balloon *balloon;
        const struct script_actor *actor;

        balloon = &machine->balloons[index];
        if (!balloon->in_use || !balloon->open_is_pending) {
            continue;
        }
        balloon->open_is_pending = 0;
        actor = &machine->objects[balloon->owner_object].actor;
        if (machine->outputs->balloon_opened != NULL) {
            machine->outputs->balloon_opened(index, balloon->label, balloon->kind, (float)(actor->x + actor->points[0][0]) + balloon->offset_x, (float)(actor->y + actor->points[0][1]) + balloon->offset_y);
        }
    }
}

static void close_all_balloons(struct script_machine *machine)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use) {
            machine->balloons[index].in_use = 0;
            if (machine->outputs->balloon_closed != NULL) {
                machine->outputs->balloon_closed(index);
            }
        }
    }
}

static int actor_has_a_balloon(struct script_machine *machine, const struct script_vm *vm)
{
    const struct script_object *object;
    int index;

    object = script_machine_current_object(machine, vm);
    if (object == NULL) {
        return 0;
    }
    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].owner_object == (int)(object - machine->objects)) {
            return 1;
        }
    }
    return 0;
}

static void start_music(struct script_machine *machine, int id)
{
    if (machine->music_id == id) {
        return;
    }
    machine->music_id = id;
    if (machine->outputs->music_started != NULL) {
        machine->outputs->music_started(id);
    }
}

static void play_sound(struct script_machine *machine, int kind)
{
    if (kind == SOUND_KIND_NONE || kind == SOUND_KIND_REFUSED) {
        return;
    }
    if (machine->outputs->sound_played != NULL) {
        machine->outputs->sound_played(kind);
    }
}

enum script_step_result script_ops_media_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    const char *label;

    switch (op) {
    case 0x37:
    case 0x5c:
    case 0xdb:
        label = script_machine_string(machine, vm, words[0]);
        if (label != NULL) {
            script_talk_open(machine, label);
        }
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x5e:
        if (machine->narration.is_running) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0x45:
    case 0x46:
    case 0x4c:
    case 0x4e:
        label = script_machine_string(machine, vm, words[0]);
        if (label != NULL) {
            open_balloon(machine, vm, label, (int)words[2], (int)words[3]);
        }
        break;
    case 0x4d:
    case 0x57:
        close_all_balloons(machine);
        break;
    case 0x54:
    case 0x55:
        place_balloon(machine, vm, words);
        break;
    case 0x5a:
        if (actor_has_a_balloon(machine, vm)) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0x109:
        if (machine->outputs->music_volume_changed != NULL) {
            machine->outputs->music_volume_changed((int)words[0], (int)words[1]);
        }
        break;
    case 0x10c:
    case 0x10d:
        play_sound(machine, (int)words[0]);
        break;
    case 0x10e:
        if (machine->outputs->sounds_stopped != NULL) {
            machine->outputs->sounds_stopped();
        }
        break;
    case 0x39:
        if (machine->talk_is_open) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0x51:
        label = script_machine_string(machine, vm, words[0]);
        if (label != NULL) {
            open_balloon(machine, vm, label, (int)words[2], (int)words[3]);
        }
        break;
    case 0x52:
        open_balloon(machine, vm, NULL, (int)words[0], (int)words[1]);
        break;
    case 0x53:
        place_balloon(machine, vm, words);
        break;
    case 0x5d:
        start_narration(machine, vm, words);
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x106:
        start_music(machine, (int)words[0]);
        break;
    case 0x108:
        machine->music_id = 0;
        if (machine->outputs->music_stopped != NULL) {
            machine->outputs->music_stopped((int)words[0]);
        }
        break;
    case 0x10a:
        if (machine->outputs->music_prepared != NULL) {
            machine->outputs->music_prepared((int)words[0]);
        }
        break;
    case 0x10b:
        play_sound(machine, (int)words[0]);
        break;
    default:
        break;
    }
    return SCRIPT_STEP_NEXT;
}
