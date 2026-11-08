#include <string.h>
#include "script_machine.h"
#include "ds_units.h"

#define MODE_BYTE_NARRATION 0x12
#define SOUND_KIND_NONE 0
#define SOUND_KIND_REFUSED 33
#define PARAGRAPH_ARG_COUNT 8
#define TAP_BALLOON_ICON_KIND 5
#define ANY_TAP_GROUP -1
#define LABEL_THAT_CLOSES 0u

static const char *const paragraph_args[PARAGRAPH_ARG_COUNT] = { "paragraph0", "paragraph1", "paragraph2", "paragraph3", "paragraph4", "paragraph5", "paragraph6", "paragraph7" };

void script_narration_report_to_outputs(void *listener, const char *label, enum script_narration_event event, int group)
{
    struct script_machine *machine;

    machine = listener;
    if (event == SCRIPT_NARRATION_GROUP_IN && group >= 0 && group < PARAGRAPH_ARG_COUNT) {
        script_machine_send(machine, "*", "monologue", paragraph_args[group]);
    }
    if (event == SCRIPT_NARRATION_DIM_OUT) {
        script_machine_send(machine, "*", "message", "end");
    }
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

static void close_slot(struct script_machine *machine, int slot)
{
    if (!machine->balloons[slot].in_use) {
        return;
    }
    machine->balloons[slot].in_use = 0;
    if (machine->outputs->balloon_closed != NULL) {
        machine->outputs->balloon_closed(slot);
    }
}

static struct script_balloon *claim_slot(struct script_machine *machine, int tap_group)
{
    int index;
    int shortest;

    if (tap_group == SCRIPT_TAP_GROUP_LEFT || tap_group == SCRIPT_TAP_GROUP_RIGHT) {
        index = tap_group == SCRIPT_TAP_GROUP_LEFT ? 0 : 1;
        close_slot(machine, index);
        return &machine->balloons[index];
    }
    shortest = SCRIPT_TAP_BALLOON_SLOTS;
    for (index = SCRIPT_TAP_BALLOON_SLOTS; index < SCRIPT_BALLOON_MAX; index++) {
        if (!machine->balloons[index].in_use) {
            return &machine->balloons[index];
        }
        if (machine->balloons[index].timer < machine->balloons[shortest].timer) {
            shortest = index;
        }
    }
    close_slot(machine, shortest);
    return &machine->balloons[shortest];
}

static struct script_balloon *open_balloon(struct script_machine *machine, struct script_vm *vm, const char *label, int kind, int ticks, int tap_group)
{
    struct script_object *object;
    struct script_balloon *balloon;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || !object->actor.exists) {
        return NULL;
    }
    balloon = claim_slot(machine, tap_group);
    memset(balloon, 0, sizeof(*balloon));
    balloon->in_use = 1;
    balloon->tap_group = tap_group;
    balloon->owner_object = (int)(object - machine->objects);
    balloon->label = label;
    balloon->kind = kind;
    balloon->timer = ticks;
    balloon->open_is_pending = 1;
    return balloon;
}

static void open_tap_balloon(struct script_machine *machine, struct script_vm *vm, const char *label, int ticks, int tap_group)
{
    open_balloon(machine, vm, label, label == NULL ? TAP_BALLOON_ICON_KIND : 0, ticks, tap_group);
}

static void open_tap_balloon_that_sends(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int tap_group)
{
    struct script_balloon *balloon;

    balloon = open_balloon(machine, vm, NULL, tap_group == SCRIPT_TAP_GROUP_LEFT ? SCRIPT_BALLOON_KIND_TAP_DOTS : SCRIPT_BALLOON_KIND_TAP_MARKS, (int)script_machine_u32(words, 0), tap_group);
    if (balloon == NULL) {
        return;
    }
    balloon->tap_target = script_machine_string(machine, vm, words[2]);
    balloon->tap_verb = script_machine_string(machine, vm, words[3]);
    balloon->tap_arg = script_machine_string(machine, vm, words[4]);
}

void script_balloons_tap(struct script_machine *machine, int tap_group)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        struct script_balloon *balloon;

        balloon = &machine->balloons[index];
        if (!balloon->in_use || balloon->tap_group != tap_group) {
            continue;
        }
        if (balloon->tap_target != NULL && balloon->tap_verb != NULL && balloon->tap_arg != NULL) {
            script_machine_send(machine, balloon->tap_target, balloon->tap_verb, balloon->tap_arg);
        }
        balloon->in_use = 0;
        if (machine->outputs->balloon_closed != NULL) {
            machine->outputs->balloon_closed(index);
        }
    }
}

int script_balloons_can_be_tapped(const struct script_machine *machine, int tap_group)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].tap_group == tap_group) {
            return 1;
        }
    }
    return 0;
}

static void place_balloon(struct script_machine *machine, const struct script_vm *vm, const unsigned int *words, int placement)
{
    const struct script_object *object;
    int index;

    object = script_machine_current_object(machine, vm);
    if (object == NULL) {
        return;
    }
    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].owner_object == (int)(object - machine->objects)) {
            machine->balloons[index].offset_x = (float)(int)script_machine_u32(words, 0) / ((float)NATIVE_FIXED_ONE);
            machine->balloons[index].offset_y = (float)(int)script_machine_u32(words, 2) / ((float)NATIVE_FIXED_ONE);
            machine->balloons[index].placement = placement;
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
        if (machine->outputs->balloon_opened == NULL) {
            continue;
        }
        if (balloon->placement == SCRIPT_BALLOON_AT_WORLD_POINT) {
            machine->outputs->balloon_opened(index, balloon->label, balloon->kind, balloon->offset_x, balloon->offset_y);
        } else if (balloon->placement == SCRIPT_BALLOON_AT_SCREEN_POINT) {
            machine->outputs->balloon_opened(index, balloon->label, balloon->kind, (float)machine->camera_x + balloon->offset_x, (float)machine->camera_y + balloon->offset_y);
        } else {
            machine->outputs->balloon_opened(index, balloon->label, balloon->kind, (float)(actor->x + actor->points[0][0]) + balloon->offset_x, (float)(actor->y + actor->points[0][1]) + balloon->offset_y);
        }
    }
}

static void close_balloons(struct script_machine *machine, int only_tap_group)
{
    int index;

    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && (only_tap_group == ANY_TAP_GROUP || machine->balloons[index].tap_group == only_tap_group)) {
            machine->balloons[index].in_use = 0;
            if (machine->outputs->balloon_closed != NULL) {
                machine->outputs->balloon_closed(index);
            }
        }
    }
}

static void shake_the_actors_balloon(struct script_machine *machine, const struct script_vm *vm, int strength, int ticks)
{
    const struct script_object *object;
    int index;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || machine->outputs->balloon_shaken == NULL) {
        return;
    }
    for (index = 0; index < SCRIPT_BALLOON_MAX; index++) {
        if (machine->balloons[index].in_use && machine->balloons[index].owner_object == (int)(object - machine->objects)) {
            machine->outputs->balloon_shaken(index, strength, ticks);
            return;
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

void script_machine_start_music(struct script_machine *machine, int id)
{
    if (machine->music_id == id) {
        return;
    }
    machine->music_id = id;
    if (machine->outputs->music_started != NULL) {
        machine->outputs->music_started(id);
    }
}

static void fade_in_music(struct script_machine *machine, int id, int fade_in_ticks)
{
    if (machine->music_id == id || fade_in_ticks <= 0 || machine->outputs->music_faded_in == NULL) {
        script_machine_start_music(machine, id);
        return;
    }
    machine->music_id = id;
    machine->outputs->music_faded_in(id, fade_in_ticks);
}

void script_machine_stop_music(struct script_machine *machine, int fade_ticks)
{
    machine->music_id = 0;
    if (machine->outputs->music_stopped != NULL) {
        machine->outputs->music_stopped(fade_ticks);
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

static void play_named_sound(struct script_machine *machine, struct script_vm *vm, int kind, unsigned int name_word)
{
    const char *tag;

    tag = script_machine_string(machine, vm, name_word);
    if (tag == NULL || tag[0] == '*' || tag[0] == '\0' || machine->outputs->sound_loop_started == NULL) {
        play_sound(machine, kind);
        return;
    }
    if (kind != SOUND_KIND_NONE && kind != SOUND_KIND_REFUSED) {
        machine->outputs->sound_loop_started(kind, tag);
    }
}

static void stop_named_sounds(struct script_machine *machine, struct script_vm *vm, unsigned int name_word)
{
    const char *tag;

    tag = script_machine_string(machine, vm, name_word);
    if (tag != NULL && tag[0] != '*' && machine->outputs->sound_loop_stopped != NULL) {
        machine->outputs->sound_loop_stopped(tag);
        return;
    }
    if (machine->outputs->sounds_stopped != NULL) {
        machine->outputs->sounds_stopped();
    }
}

enum script_step_result script_ops_media_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    const char *label;

    switch (op) {
    case 0x37:
    case 0x5c:
    case 0xdb:
        if (op == 0xdb) {
            machine->soul.rollback_is_on = 0;
        }
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
    case 0x4c:
        if (op == 0x45 && words[0] == LABEL_THAT_CLOSES) {
            close_balloons(machine, SCRIPT_TAP_GROUP_LEFT);
            break;
        }
        label = script_machine_string(machine, vm, words[0]);
        if (label != NULL) {
            open_tap_balloon(machine, vm, label, (int)script_machine_u32(words, 2), op == 0x45 ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT);
        }
        break;
    case 0x46:
    case 0x4e:
        open_tap_balloon_that_sends(machine, vm, words, op == 0x46 ? SCRIPT_TAP_GROUP_LEFT : SCRIPT_TAP_GROUP_RIGHT);
        break;
    case 0x4d:
        close_balloons(machine, SCRIPT_TAP_GROUP_RIGHT);
        break;
    case 0x4b:
        vm->condition = script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_LEFT);
        break;
    case 0x50:
        vm->condition = script_balloons_can_be_tapped(machine, SCRIPT_TAP_GROUP_RIGHT);
        break;
    case 0x57:
        close_balloons(machine, ANY_TAP_GROUP);
        break;
    case 0x54:
        place_balloon(machine, vm, words, SCRIPT_BALLOON_AT_WORLD_POINT);
        break;
    case 0x55:
        place_balloon(machine, vm, words, SCRIPT_BALLOON_AT_SCREEN_POINT);
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
        play_named_sound(machine, vm, (int)words[0], words[3]);
        break;
    case 0x10e:
        stop_named_sounds(machine, vm, words[1]);
        break;
    case 0x39:
        if (machine->talk_is_open || script_choice_holds_the_script(machine)) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0x56:
        shake_the_actors_balloon(machine, vm, (int)words[0], (int)words[1]);
        break;
    case 0x51:
        label = script_machine_string(machine, vm, words[0]);
        if (label != NULL) {
            open_balloon(machine, vm, label, (int)words[2], (int)words[3], SCRIPT_NO_TAP_GROUP);
        }
        break;
    case 0x52:
        open_balloon(machine, vm, NULL, (int)words[0], (int)words[1], SCRIPT_NO_TAP_GROUP);
        break;
    case 0x53:
        place_balloon(machine, vm, words, SCRIPT_BALLOON_ON_OWNER);
        break;
    case 0x5d:
        start_narration(machine, vm, words);
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x106:
        script_machine_start_music(machine, (int)words[0]);
        break;
    case 0x107:
        fade_in_music(machine, (int)words[0], (int)words[2]);
        break;
    case 0x108:
        script_machine_stop_music(machine, (int)words[0]);
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
