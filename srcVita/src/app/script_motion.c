#include <string.h>
#include "script_machine.h"

#define DEFAULT_LEVEL_FOR_GIMMICK 9
#define DEFAULT_LEVEL_FOR_HUMAN 6
#define HUMAN_CLASS_ID 0x5a390072u
#define UNPLACED_X 128
#define UNPLACED_Y 96
#define WORD_FRAME_LIMIT 0xfff0u
#define FRAME_NUMBER_BITS 10
#define WORD_END 0xfffeu
#define WORD_BEGIN 0xfffdu
#define WORD_BOX 0xfffcu
#define WORD_POINT_ZERO 0xfff4u
#define WORD_POINT_N 0xfff5u
#define WORD_CORE_PLACE 0xfff0u
#define WORD_CORE_VALUE 0xfff6u
#define WORD_SOUND 0xfff7u
#define TABLE_STEP_MAX 4096

static int read_signed_word(const struct script_source *source, unsigned int word_index, int *value)
{
    unsigned int word;

    if (script_file_read_word(&source->file, word_index, &word) != SCRIPT_FILE_OK) {
        return 0;
    }
    *value = word >= 0x8000u ? (int)word - 0x10000 : (int)word;
    return 1;
}

static void halt_for_table(struct script_machine *machine, struct script_object *object, const char *reason)
{
    script_machine_halt(machine, &object->main_vm, reason, object->name);
    machine->halt.offset = object->actor.table_word * SCRIPT_FILE_BYTES_PER_WORD + SCRIPT_FILE_HEADER_SIZE;
}

static void run_box_word(struct script_machine *machine, struct script_object *object, int keeps_point_zero)
{
    struct script_actor *actor;
    int start_x;
    int start_y;

    actor = &object->actor;
    if (keeps_point_zero) {
        if (!read_signed_word(actor->table_source, actor->table_word + 1, &start_x) || !read_signed_word(actor->table_source, actor->table_word + 2, &start_y)) {
            halt_for_table(machine, object, "motion_word_past_code");
            return;
        }
        actor->x = actor->x + actor->points[0][0] - start_x;
        actor->y = actor->y + actor->points[0][1] - start_y;
    }
}

static void run_point_word(struct script_machine *machine, struct script_object *object, int point, unsigned int first_value_word)
{
    struct script_actor *actor;
    int x;
    int y;

    actor = &object->actor;
    if (point < 0 || point >= SCRIPT_ACTOR_POINT_MAX) {
        halt_for_table(machine, object, "motion_point_out_of_range");
        return;
    }
    if (!read_signed_word(actor->table_source, first_value_word, &x) || !read_signed_word(actor->table_source, first_value_word + 1, &y)) {
        halt_for_table(machine, object, "motion_word_past_code");
        return;
    }
    actor->points[point][0] = x;
    actor->points[point][1] = y;
}

static void run_sound_word(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;
    int kind;

    actor = &object->actor;
    if (!read_signed_word(actor->table_source, actor->table_word + 1, &kind)) {
        halt_for_table(machine, object, "motion_word_past_code");
        return;
    }
    if (machine->outputs->sound_played != NULL) {
        machine->outputs->sound_played(kind);
    }
}

static void run_table(struct script_machine *machine, struct script_object *object, int keeps_point_zero)
{
    struct script_actor *actor;
    unsigned int word;
    int point;
    int steps;

    actor = &object->actor;
    for (steps = 0; steps < TABLE_STEP_MAX; steps++) {
        if (script_file_read_word(&actor->table_source->file, actor->table_word, &word) != SCRIPT_FILE_OK) {
            halt_for_table(machine, object, "motion_word_past_code");
            return;
        }
        if (word < WORD_FRAME_LIMIT) {
            int hold_ticks;

            hold_ticks = (int)(word >> FRAME_NUMBER_BITS);
            if (hold_ticks != 0) {
                actor->frame_word_index = actor->frame_word_index + 1;
                actor->frame_hold_ticks = hold_ticks;
                actor->ticks_left = hold_ticks - 1;
                return;
            }
            actor->table_word = actor->table_word + 1;
            continue;
        }
        switch (word) {
        case WORD_END:
            return;
        case WORD_BEGIN:
            actor->table_word = actor->table_word + 3;
            break;
        case WORD_BOX:
            run_box_word(machine, object, keeps_point_zero);
            keeps_point_zero = 0;
            actor->table_word = actor->table_word + 5;
            break;
        case WORD_POINT_ZERO:
            run_point_word(machine, object, 0, actor->table_word + 1);
            actor->table_word = actor->table_word + 3;
            break;
        case WORD_POINT_N:
            if (!read_signed_word(actor->table_source, actor->table_word + 1, &point)) {
                halt_for_table(machine, object, "motion_word_past_code");
                return;
            }
            run_point_word(machine, object, point, actor->table_word + 2);
            actor->table_word = actor->table_word + 4;
            break;
        case WORD_CORE_PLACE:
            actor->table_word = actor->table_word + 4;
            break;
        case WORD_CORE_VALUE:
            actor->table_word = actor->table_word + 2;
            break;
        case WORD_SOUND:
            run_sound_word(machine, object);
            actor->table_word = actor->table_word + 3;
            break;
        default:
            halt_for_table(machine, object, "motion_word_not_ported");
            return;
        }
        if (machine->halt.is_halted) {
            return;
        }
    }
    halt_for_table(machine, object, "motion_table_runaway");
}

static void start_queued_table(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;

    actor = &object->actor;
    actor->has_queued_table = 0;
    actor->has_table = 1;
    actor->table_word = actor->queued_table_word;
    actor->table_offset = actor->queued_table_offset;
    actor->table_source = object->main_vm.source;
    actor->frame_word_index = -1;
    actor->frame_hold_ticks = 0;
    actor->ticks_left = 0;
    run_table(machine, object, actor->queued_keeps_point_zero);
    actor->queued_keeps_point_zero = 0;
    actor->is_visible = 1;
}

static void step_table(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;
    unsigned int word;

    actor = &object->actor;
    if (script_file_read_word(&actor->table_source->file, actor->table_word, &word) != SCRIPT_FILE_OK) {
        halt_for_table(machine, object, "motion_word_past_code");
        return;
    }
    if (word == WORD_END) {
        return;
    }
    if (actor->ticks_left != 0) {
        actor->ticks_left = actor->ticks_left - 1;
        return;
    }
    actor->table_word = actor->table_word + 1;
    run_table(machine, object, 0);
}

static void apply_place_and_shift(struct script_actor *actor)
{
    if (actor->place_is_pending) {
        actor->x = actor->place_x - actor->points[actor->place_point][0];
        actor->y = actor->place_y - actor->points[actor->place_point][1];
        actor->place_is_pending = 0;
    }
    if (actor->shift_is_pending) {
        actor->x = actor->x + actor->shift_x;
        actor->y = actor->y + actor->shift_y;
        actor->shift_x = 0;
        actor->shift_y = 0;
        actor->shift_is_pending = 0;
    }
}

static void update_actor(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;

    actor = &object->actor;
    if (actor->has_queued_table) {
        start_queued_table(machine, object);
    } else if (actor->has_table && !machine->pause_bit) {
        step_table(machine, object);
    }
    apply_place_and_shift(actor);
}

static void apply_dock(struct script_machine *machine, struct script_actor *actor)
{
    const struct script_actor *target;

    if (!actor->is_visible || !actor->dock_is_pending) {
        return;
    }
    target = &machine->objects[actor->dock_target].actor;
    actor->x = target->x + target->points[actor->dock_target_point][0] - actor->points[actor->dock_own_point][0];
    actor->y = target->y + target->points[actor->dock_target_point][1] - actor->points[actor->dock_own_point][1];
    actor->dock_is_pending = 0;
}

static void report_actor_if_changed(struct script_machine *machine, const struct script_object *object, struct script_actor *actor)
{
    struct script_actor_view view;

    if (!actor->has_table || machine->outputs->actor_changed == NULL) {
        return;
    }
    memset(&view, 0, sizeof(view));
    view.name = object->name;
    view.file_name = actor->table_source->name;
    view.table_offset = actor->table_offset;
    view.frame_word_index = actor->frame_word_index;
    view.ticks_into_frame = actor->frame_hold_ticks - 1 - actor->ticks_left;
    view.x = actor->x;
    view.y = actor->y;
    view.level = actor->level;
    view.creation_index = actor->creation_index;
    view.is_visible = actor->is_visible;
    if (actor->has_last_view && memcmp(&view, &actor->last_view, sizeof(view)) == 0) {
        return;
    }
    actor->last_view = view;
    actor->has_last_view = 1;
    machine->outputs->actor_changed(&view);
}

void script_motion_update_all(struct script_machine *machine)
{
    int index;

    for (index = 0; index < machine->object_count && !machine->halt.is_halted; index++) {
        if (machine->objects[index].actor.exists) {
            update_actor(machine, &machine->objects[index]);
        }
    }
    for (index = 0; index < machine->object_count; index++) {
        if (machine->objects[index].actor.exists) {
            apply_dock(machine, &machine->objects[index].actor);
        }
    }
    for (index = 0; index < machine->object_count; index++) {
        if (machine->objects[index].actor.exists) {
            report_actor_if_changed(machine, &machine->objects[index], &machine->objects[index].actor);
        }
    }
}

static void create_actor(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_actor *actor;

    if (vm->owner == NULL) {
        script_machine_halt(machine, vm, "op_not_ported", "runner has no actor");
        return;
    }
    actor = &vm->owner->actor;
    if (actor->exists) {
        return;
    }
    actor->exists = 1;
    actor->creation_index = machine->actor_count;
    machine->actor_count = machine->actor_count + 1;
    actor->x = UNPLACED_X;
    actor->y = UNPLACED_Y;
    actor->level = script_machine_u32(words, 0) == HUMAN_CLASS_ID ? DEFAULT_LEVEL_FOR_HUMAN : DEFAULT_LEVEL_FOR_GIMMICK;
}

static void select_actor(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const struct script_object *object;
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    object = script_machine_find_object(machine, name);
    if (object != NULL && object->actor.exists) {
        vm->selected_object = (int)(object - machine->objects);
    }
}

static void queue_table(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_object *object;
    unsigned int word;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || !object->actor.exists) {
        return;
    }
    if (words[3] != 0) {
        script_machine_halt(machine, vm, "motion_flag_b_not_ported", object->name);
        return;
    }
    if (script_file_word_of_offset(&vm->source->file, script_machine_u32(words, 0), &word) != SCRIPT_FILE_OK) {
        script_machine_halt(machine, vm, "bad_motion_table", object->name);
        return;
    }
    object->actor.has_queued_table = 1;
    object->actor.queued_table_word = word;
    object->actor.queued_table_offset = script_machine_u32(words, 0);
    object->actor.queued_keeps_point_zero = words[2] != 0;
}

static int signed_of(unsigned int word)
{
    return word >= 0x8000u ? (int)word - 0x10000 : (int)word;
}

static void place_actor(struct script_machine *machine, struct script_vm *vm, int x, int y, int point)
{
    struct script_object *object;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || !object->actor.exists) {
        return;
    }
    if (point >= SCRIPT_ACTOR_POINT_MAX) {
        script_machine_halt(machine, vm, "motion_point_out_of_range", object->name);
        return;
    }
    object->actor.place_is_pending = 1;
    object->actor.place_x = x;
    object->actor.place_y = y;
    object->actor.place_point = point;
}

static void nudge_actor(struct script_machine *machine, struct script_vm *vm, int x, int y)
{
    struct script_object *object;

    object = script_machine_current_object(machine, vm);
    if (object == NULL || !object->actor.exists) {
        return;
    }
    if (object->actor.place_is_pending) {
        object->actor.place_x = object->actor.place_x + x;
        object->actor.place_y = object->actor.place_y + y;
        return;
    }
    object->actor.shift_is_pending = 1;
    object->actor.shift_x = object->actor.shift_x + x;
    object->actor.shift_y = object->actor.shift_y + y;
}

static void dock_actor(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_object *object;
    const struct script_object *target;
    const char *name;

    object = script_machine_current_object(machine, vm);
    name = script_machine_string(machine, vm, words[0]);
    if (object == NULL || !object->actor.exists || name == NULL) {
        return;
    }
    target = script_machine_find_object(machine, name);
    if (target == NULL || !target->actor.exists) {
        return;
    }
    if (words[1] >= SCRIPT_ACTOR_POINT_MAX || words[2] >= SCRIPT_ACTOR_POINT_MAX) {
        script_machine_halt(machine, vm, "dock_point_not_ported", object->name);
        return;
    }
    object->actor.dock_is_pending = 1;
    object->actor.dock_target = (int)(target - machine->objects);
    object->actor.dock_target_point = (int)words[1];
    object->actor.dock_own_point = (int)words[2];
}

enum script_step_result script_ops_actor_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    struct script_object *object;

    switch (op) {
    case 0x81:
        create_actor(machine, vm, words);
        break;
    case 0x83:
        select_actor(machine, vm, words);
        break;
    case 0x86:
        dock_actor(machine, vm, words);
        break;
    case 0x8e:
        place_actor(machine, vm, signed_of(words[0]), signed_of(words[1]), 0);
        break;
    case 0x90:
        place_actor(machine, vm, signed_of(words[0]), signed_of(words[1]), (int)(words[2] & 0xf));
        break;
    case 0x91:
        nudge_actor(machine, vm, signed_of(words[0]), signed_of(words[1]));
        break;
    case 0x95:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists) {
            object->actor.is_visible = 0;
        }
        break;
    case 0x9b:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists) {
            object->actor.level = (int)(words[0] & 0xf);
        }
        break;
    case 0x116:
        vm->selected_object = SCRIPT_NO_SELECTION;
        break;
    case 0x117:
        queue_table(machine, vm, words);
        break;
    default:
        break;
    }
    return SCRIPT_STEP_NEXT;
}
