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
#define WORD_LOOP 0xffffu
#define WORD_FRAME_WITH_TICKS 0xfffbu
#define LOOP_BACK_WORDS 3
#define OFF_WORLD_LIMIT 10000
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

static void stop_table_for(struct script_machine *machine, struct script_object *object, const char *reason)
{
    script_machine_note(machine, &object->main_vm, reason, object->name);
    object->actor.has_table = 0;
}

static unsigned int table_word_length(unsigned int word)
{
    switch (word) {
    case 0xfff0u:
    case 0xfff5u:
    case 0xfff9u:
        return 4;
    case 0xfff3u:
    case 0xfff6u:
    case 0xfffau:
        return 2;
    case 0xfff4u:
    case 0xfff7u:
    case 0xfffbu:
    case 0xfffdu:
        return 3;
    case 0xfff8u:
        return 11;
    case 0xfffcu:
        return 5;
    default:
        return 0;
    }
}

static void run_box_word(struct script_machine *machine, struct script_object *object, int keeps_point_zero)
{
    struct script_actor *actor;
    int start_x;
    int start_y;
    int carry_x;
    int carry_y;

    actor = &object->actor;
    if (!read_signed_word(actor->table_source, actor->table_word + 1, &start_x) || !read_signed_word(actor->table_source, actor->table_word + 2, &start_y) || !read_signed_word(actor->table_source, actor->table_word + 3, &carry_x) || !read_signed_word(actor->table_source, actor->table_word + 4, &carry_y)) {
        stop_table_for(machine, object, "motion_word_past_code");
        return;
    }
    actor->loop_carry_x = carry_x;
    actor->loop_carry_y = carry_y;
    if (keeps_point_zero) {
        actor->x = actor->x + actor->points[0][0] - (actor->is_flipped ? -start_x : start_x);
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
        script_machine_note(machine, &object->main_vm, "motion_point_out_of_range", object->name);
        return;
    }
    if (!read_signed_word(actor->table_source, first_value_word, &x) || !read_signed_word(actor->table_source, first_value_word + 1, &y)) {
        stop_table_for(machine, object, "motion_word_past_code");
        return;
    }
    if (actor->is_flipped) {
        x = -x;
    }
    if (point == 0 && actor->is_after_loop) {
        actor->x = actor->x + (actor->is_flipped ? -actor->loop_carry_x : actor->loop_carry_x) - x;
        actor->y = actor->y + actor->loop_carry_y - y;
        actor->is_after_loop = 0;
        if (actor->x + x > OFF_WORLD_LIMIT || actor->x + x < -OFF_WORLD_LIMIT) {
            actor->is_visible = 0;
            actor->stepping_is_stopped = 1;
        }
    }
    actor->points[point][0] = x;
    actor->points[point][1] = y;
}

static void run_core_word(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;
    int core;
    int x;
    int y;

    actor = &object->actor;
    if (!read_signed_word(actor->table_source, actor->table_word + 1, &core) || !read_signed_word(actor->table_source, actor->table_word + 2, &x) || !read_signed_word(actor->table_source, actor->table_word + 3, &y)) {
        stop_table_for(machine, object, "motion_word_past_code");
        return;
    }
    if (core == 0) {
        actor->has_core = 1;
        actor->core_x = actor->is_flipped ? -x : x;
        actor->core_y = y;
    } else if (core == 1) {
        actor->has_second_core = 1;
        actor->second_core_x = actor->is_flipped ? -x : x;
        actor->second_core_y = y;
    }
}

static void run_sound_word(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;
    int kind;

    actor = &object->actor;
    if (!read_signed_word(actor->table_source, actor->table_word + 1, &kind)) {
        stop_table_for(machine, object, "motion_word_past_code");
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
            stop_table_for(machine, object, "motion_word_past_code");
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
        case WORD_LOOP:
            actor->table_word = actor->table_start_word + LOOP_BACK_WORDS;
            actor->is_after_loop = 1;
            actor->frame_word_index = -1;
            break;
        case WORD_FRAME_WITH_TICKS:
            if (read_signed_word(actor->table_source, actor->table_word + 2, &point) && point != 0) {
                actor->frame_word_index = actor->frame_word_index + 1;
                actor->frame_hold_ticks = point + 1;
                actor->ticks_left = point;
                actor->table_word = actor->table_word + 2;
                return;
            }
            actor->table_word = actor->table_word + 3;
            break;
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
                stop_table_for(machine, object, "motion_word_past_code");
                return;
            }
            run_point_word(machine, object, point, actor->table_word + 2);
            actor->table_word = actor->table_word + 4;
            break;
        case WORD_CORE_PLACE:
            run_core_word(machine, object);
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
            if (table_word_length(word) == 0) {
                stop_table_for(machine, object, "motion_word_not_ported");
                return;
            }
            actor->table_word = actor->table_word + table_word_length(word);
            break;
        }
        if (!actor->has_table) {
            return;
        }
    }
    stop_table_for(machine, object, "motion_table_runaway");
}

static void start_queued_table(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;

    actor = &object->actor;
    actor->has_queued_table = 0;
    actor->has_table = 1;
    actor->table_word = actor->queued_table_word;
    actor->table_start_word = actor->queued_table_word;
    actor->table_offset = actor->queued_table_offset;
    actor->is_after_loop = 0;
    actor->table_source = object->main_vm.source;
    actor->frame_word_index = -1;
    actor->frame_hold_ticks = 0;
    actor->ticks_left = 0;
    run_table(machine, object, actor->queued_keeps_point_zero);
    actor->queued_keeps_point_zero = 0;
    actor->is_visible = !actor->is_hard_hidden;
    actor->is_frozen_and_hidden = 0;
}

static void step_table(struct script_machine *machine, struct script_object *object)
{
    struct script_actor *actor;
    unsigned int word;

    actor = &object->actor;
    if (script_file_read_word(&actor->table_source->file, actor->table_word, &word) != SCRIPT_FILE_OK) {
        stop_table_for(machine, object, "motion_word_past_code");
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
    } else if (actor->has_table && !machine->pause_bit && !actor->stepping_is_stopped && !actor->is_frozen_and_hidden && !object->is_sleeping) {
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
    if (!actor->keeps_following) {
        actor->dock_is_pending = 0;
    }
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
    view.is_flipped = actor->is_flipped;
    view.class_id = actor->class_id;
    view.has_core = actor->has_core;
    view.core_x = actor->x + actor->core_x;
    view.core_y = actor->y + actor->core_y;
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

    for (index = 0; index < machine->object_count; index++) {
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
        script_machine_note(machine, vm, "runner_has_no_actor", "");
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
    actor->class_id = script_machine_u32(words, 0);
    actor->level = actor->class_id == HUMAN_CLASS_ID ? DEFAULT_LEVEL_FOR_HUMAN : DEFAULT_LEVEL_FOR_GIMMICK;
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
    if (script_file_word_of_offset(&vm->source->file, script_machine_u32(words, 0), &word) != SCRIPT_FILE_OK) {
        script_machine_note(machine, vm, "bad_motion_table", object->name);
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
        script_machine_note(machine, vm, "motion_point_out_of_range", object->name);
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

static void dock_actor(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int keeps_following)
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
        script_machine_note(machine, vm, "dock_point_not_ported", object->name);
        return;
    }
    object->actor.dock_is_pending = 1;
    object->actor.keeps_following = keeps_following;
    object->actor.dock_target = (int)(target - machine->objects);
    object->actor.dock_target_point = (int)words[1];
    object->actor.dock_own_point = (int)words[2];
}

static void compare_active_core(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *name;

    name = script_machine_string(machine, vm, words[0]);
    if (name != NULL) {
        vm->condition = machine->active_core_name != NULL && strcmp(machine->active_core_name, name) == 0;
    }
}

static int name_matches_target(const char *name, const char *target)
{
    if (target[0] == '*') {
        return 1;
    }
    if (target[0] == '@') {
        return strstr(name, target + 1) != NULL;
    }
    return strcmp(name, target) == 0;
}

static void set_hold_on_targets(struct script_machine *machine, struct script_vm *vm, unsigned int name_word, int frozen_value, int hard_value, int shows)
{
    const char *target;
    int index;

    target = script_machine_string(machine, vm, name_word);
    if (target == NULL) {
        return;
    }
    for (index = 0; index < machine->object_count; index++) {
        struct script_actor *actor;

        actor = &machine->objects[index].actor;
        if (!actor->exists || !name_matches_target(machine->objects[index].name, target)) {
            continue;
        }
        if (frozen_value >= 0) {
            actor->is_frozen_and_hidden = frozen_value;
        }
        if (hard_value >= 0) {
            actor->is_hard_hidden = hard_value;
        }
        actor->is_visible = shows && actor->has_table;
    }
}

static void face_actor(struct script_object *object, int should_flip)
{
    struct script_actor *actor;
    int index;

    actor = &object->actor;
    if (should_flip == actor->is_flipped) {
        return;
    }
    actor->x = actor->x + 2 * actor->points[0][0];
    for (index = 0; index < SCRIPT_ACTOR_POINT_MAX; index++) {
        actor->points[index][0] = -actor->points[index][0];
    }
    actor->core_x = -actor->core_x;
    actor->second_core_x = -actor->second_core_x;
    actor->is_flipped = should_flip;
}

static int motion_has_ended(const struct script_object *object)
{
    const struct script_actor *actor;
    unsigned int word;

    actor = &object->actor;
    if (!actor->has_table) {
        return 1;
    }
    if (script_file_read_word(&actor->table_source->file, actor->table_word, &word) != SCRIPT_FILE_OK) {
        return 1;
    }
    return word == WORD_END;
}

static void set_core_enabled(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int is_disabled)
{
    const char *name;
    struct script_object *object;
    int core;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    object = script_machine_find_object(machine, name);
    if (object == NULL) {
        return;
    }
    core = signed_of(words[1]);
    if (core < 0) {
        object->actor.disabled_cores = is_disabled ? -1 : 0;
    } else if (is_disabled) {
        object->actor.disabled_cores = object->actor.disabled_cores | (1 << core);
    } else {
        object->actor.disabled_cores = object->actor.disabled_cores & ~(1 << core);
    }
}

enum script_step_result script_ops_actor_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    struct script_object *object;

    switch (op) {
    case 0x82:
        script_machine_note(machine, vm, "free_unit_not_ported", "");
        break;
    case 0x85:
        dock_actor(machine, vm, words, 1);
        break;
    case 0x87:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists) {
            object->actor.x = (int)script_machine_u32(words, 0) / 4096;
            object->actor.y = (int)script_machine_u32(words, 2) / 4096;
        }
        break;
    case 0x8f:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists) {
            face_actor(object, words[0] != 0);
        }
        break;
    case 0x94:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists) {
            object->actor.is_visible = 1;
            object->actor.is_hard_hidden = 0;
            object->actor.is_frozen_and_hidden = 0;
        }
        break;
    case 0x96:
        set_hold_on_targets(machine, vm, words[0], 1, -1, 0);
        break;
    case 0x97:
        set_hold_on_targets(machine, vm, words[0], 0, -1, 1);
        break;
    case 0x98:
        set_hold_on_targets(machine, vm, words[0], 0, 1, 0);
        break;
    case 0x99:
        set_hold_on_targets(machine, vm, words[0], -1, 0, 1);
        break;
    case 0x9d:
        set_core_enabled(machine, vm, words, 0);
        break;
    case 0x9e:
        set_core_enabled(machine, vm, words, 1);
        break;
    case 0xa9:
        object = script_machine_current_object(machine, vm);
        if (object != NULL && object->actor.exists && !motion_has_ended(object)) {
            machine->last_timer_tick = machine->tick;
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        break;
    case 0xac:
        object = script_machine_current_object(machine, vm);
        if (object != NULL) {
            object->actor.stepping_is_stopped = 1;
        }
        break;
    case 0xad:
        object = script_machine_current_object(machine, vm);
        if (object != NULL) {
            object->actor.stepping_is_stopped = 0;
        }
        break;
    case 0xa0:
        compare_active_core(machine, vm, words);
        break;
    case 0x81:
        create_actor(machine, vm, words);
        break;
    case 0x83:
        select_actor(machine, vm, words);
        break;
    case 0x86:
        dock_actor(machine, vm, words, 0);
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
    case 0x9c:
        object = script_machine_current_object(machine, vm);
        if (object != NULL) {
            script_soul_set_first_core(machine, (int)(object - machine->objects), (int)words[0]);
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
