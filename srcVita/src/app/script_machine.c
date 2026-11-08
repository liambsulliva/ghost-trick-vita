#include <stdio.h>
#include <string.h>
#include "script_machine.h"
#include "script_opcodes.h"
#include "ds_units.h"

#define NOTE_ADDED_TALK_LABEL "sys_add_data"
#define MODE_BYTE_LOADING 1
#define MODE_BYTE_NORMAL 2
#define MODE_BYTE_ALSO_RUNS_IDLE 3
#define GAME_START_AFTER_LOAD_TICKS 3
#define LOAD_AFTER_TEARDOWN_TICKS 4
#define SOUL_SOUND_KIND 14
#define NARRATION_OVERHEAD_TICKS 3
#define MOVIE_SOUND_KIND 82
#define NEVER -1
#define SCRIPT_CHANGE_OP 0x24u
#define PAUSE_CLASS_NORMAL 0
#define PAUSE_CLASS_ONE 1
#define PAUSE_CLASS_TALK 2
#define PAUSE_CLASS_FREE 3
#define GAME_APP_TARGET "GameApp"
#define XML_SUFFIX ".xml"
#define STUCK_AFTER_QUIET_TICKS 240
#define DEBUG_ARG_PREFIX "debug"
#define STUCK_AFTER_ANY_TICKS 3000
#define STUCK_AFTER_ANY_TICKS_WHEN_A_ROUTE_DRIVES 1200
#define STUCK_AFTER_TICKS_WHEN_THE_PLAYER_CAN_ACT 18000
#define STUCK_AFTER_TICKS_WHEN_PLAYING_AT_A_HUMAN_PACE 3600
#define STUCK_PUSHES_BEFORE_LEAVING_THE_SCRIPT 60
#define FNV_OFFSET_BASIS 2166136261u
#define CHAPTER_MARK_OP 0x74u
#define CHAPTER_MARK_SEARCH_WORDS 12u
#define SCRIPT_MOVE_ON_MARK_MAX 64

static const char *vm_kind_name(enum script_vm_kind kind)
{
    switch (kind) {
    case SCRIPT_VM_RUNNER:
        return "runner_dbc";
    case SCRIPT_VM_MAIN:
        return "main_ec4";
    case SCRIPT_VM_SCANNER:
        return "scan_e14";
    default:
        return "handler_e6c";
    }
}

void script_machine_name_of_path(const char *path, char *name, unsigned int name_size)
{
    unsigned int length;
    unsigned int index;

    snprintf(name, name_size, "%s", path);
    for (index = 0; name[index] != '\0'; index++) {
        if (name[index] == '/') {
            name[index] = '-';
        }
    }
    length = (unsigned int)strlen(name);
    if (length > strlen(XML_SUFFIX) && strcmp(&name[length - strlen(XML_SUFFIX)], XML_SUFFIX) == 0) {
        name[length - strlen(XML_SUFFIX)] = '\0';
    }
}

void script_machine_init(struct script_machine *machine, const struct script_outputs *outputs, enum script_timing timing, const char *stop_act)
{
    memset(machine, 0, sizeof(*machine));
    machine->outputs = outputs;
    machine->timing = timing;
    machine->stop_act = stop_act;
    machine->teardown_at = NEVER;
    machine->load_at = NEVER;
    machine->furthest_scene = -1;
    machine->route_furthest_acted_step = -1;
    machine->route_step_after_retry = -1;
    machine->game_over_retry_step = -1;
    machine->game_start_at = NEVER;
    machine->camera_follow_object = SCRIPT_NO_SELECTION;
    machine->pause_clear_at = NEVER;
    machine->soul_vision_message_at = NEVER;
    machine->soul_sound_at = NEVER;
    machine->talk_close_at = NEVER;
    script_soul_reset(machine);
    script_flags_clear_all(&machine->flags);
    script_choice_clear_all(machine);
}

int script_machine_add_source(struct script_machine *machine, const char *name, unsigned char *data, unsigned int size, const struct script_cue_list *cues)
{
    struct script_source *source;

    if (machine->source_count >= SCRIPT_SOURCE_MAX) {
        return 0;
    }
    source = &machine->sources[machine->source_count];
    if (script_file_open(&source->file, data, size) != SCRIPT_FILE_OK) {
        return 0;
    }
    snprintf(source->name, sizeof(source->name), "%s", name);
    source->cues = cues;
    machine->source_count = machine->source_count + 1;
    return 1;
}

static const struct script_source *find_source(const struct script_machine *machine, const char *name)
{
    int index;

    for (index = 0; index < machine->source_count; index++) {
        if (strcmp(machine->sources[index].name, name) == 0) {
            return &machine->sources[index];
        }
    }
    return NULL;
}

static void begin_vm(struct script_vm *vm, enum script_vm_kind kind, const struct script_source *source, struct script_object *owner, unsigned int pc)
{
    memset(vm, 0, sizeof(*vm));
    vm->kind = kind;
    vm->source = source;
    vm->owner = owner;
    vm->pc = pc;
    vm->start_pc = pc;
    vm->selected_object = SCRIPT_NO_SELECTION;
}

static void clear_what_the_earlier_scene_left(struct script_machine *machine);
static void note_scene_began(struct script_machine *machine, const char *name);
static int scene_was_seen(const struct script_machine *machine, unsigned int hash);
static unsigned int hash_text(unsigned int hash, const char *text);

static int chapter_mark_of_source(const struct script_source *source)
{
    unsigned int index;
    unsigned int op;
    unsigned int mark;

    for (index = 0; index + 1 < CHAPTER_MARK_SEARCH_WORDS; index++) {
        if (script_file_read_word(&source->file, index, &op) != SCRIPT_FILE_OK || script_file_read_word(&source->file, index + 1, &mark) != SCRIPT_FILE_OK) {
            return 0;
        }
        if (op == CHAPTER_MARK_OP) {
            return (int)mark;
        }
    }
    return 0;
}

static int scene_is_behind_the_story(const struct script_machine *machine, const struct script_source *source, int was_seen)
{
    int mark;

    if (machine->route == NULL || machine->route->scene_count == 0 || machine->auto_press == SCRIPT_AUTO_OFF || !was_seen) {
        return 0;
    }
    mark = chapter_mark_of_source(source);
    return mark > 0 && mark < machine->chapter_mark;
}

static void begin_script(struct script_machine *machine, const struct script_source *source)
{
    clear_what_the_earlier_scene_left(machine);
    machine->scene_is_behind_the_story = scene_is_behind_the_story(machine, source, scene_was_seen(machine, hash_text(FNV_OFFSET_BASIS, source->name)));
    note_scene_began(machine, source->name);
    begin_vm(&machine->runner, SCRIPT_VM_RUNNER, source, NULL, 0);
    machine->has_runner = 1;
    memset(machine->visited_sites, 0, sizeof(machine->visited_sites));
    memset(machine->pushed_sites, 0, sizeof(machine->pushed_sites));
    machine->pushes_in_this_script = 0;
    machine->last_new_site_tick = machine->tick;
    machine->last_timer_tick = machine->tick;
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("script_begun", source->name);
    }
    script_soul_script_began(machine, source->name);
    script_machine_set_world_origin(machine, 0, 0);
    machine->object_count = 0;
    machine->actor_count = 0;
    machine->gameapp_count = 0;
    machine->mode_byte = MODE_BYTE_LOADING;
    machine->pause_bit = 0;
    script_flags_clear_local(&machine->flags);
    machine->game_start_at = (long long)machine->tick + GAME_START_AFTER_LOAD_TICKS;
}

int script_machine_start(struct script_machine *machine, const char *name)
{
    const struct script_source *source;

    source = find_source(machine, name);
    if (source == NULL) {
        return 0;
    }
    begin_script(machine, source);
    return 1;
}

static unsigned int hash_text(unsigned int hash, const char *text)
{
    while (*text != '\0') {
        hash = (hash ^ (unsigned char)*text) * 16777619u;
        text = text + 1;
    }
    return hash;
}

static int scene_was_seen(const struct script_machine *machine, unsigned int hash)
{
    int index;

    for (index = 0; index < machine->scenes_seen; index++) {
        if (machine->scene_hashes_seen[index] == hash) {
            return 1;
        }
    }
    return 0;
}

static void note_scene_began(struct script_machine *machine, const char *name)
{
    unsigned int hash;
    int scene;

    machine->scene_name = name;
    hash = hash_text(FNV_OFFSET_BASIS, name);
    if (!scene_was_seen(machine, hash)) {
        if (machine->scenes_seen < SCRIPT_SCENES_SEEN_MAX) {
            machine->scene_hashes_seen[machine->scenes_seen] = hash;
            machine->scenes_seen = machine->scenes_seen + 1;
        }
        machine->last_new_scene_tick = machine->tick;
    }
    scene = machine->route != NULL ? script_route_scene_index(machine->route, name) : -1;
    if (scene > machine->furthest_scene) {
        machine->furthest_scene = scene;
    }
}

static void note_leftover(struct script_machine *machine, const char *what)
{
    if (machine->outputs != NULL && machine->outputs->marked != NULL) {
        machine->outputs->marked("scene_leftover_cleared", what);
    }
}

static void clear_what_the_earlier_scene_left(struct script_machine *machine)
{
    if (machine->press_is_pending) {
        machine->press_is_pending = 0;
        note_leftover(machine, "press");
    }
    if (machine->talk_is_open || machine->talk.phase != SCRIPT_TALK_CLOSED || machine->talk.message != NULL || machine->talk_close_at != NEVER) {
        memset(&machine->talk, 0, sizeof(machine->talk));
        machine->talk_is_open = 0;
        machine->talk_close_at = NEVER;
        note_leftover(machine, "talk");
    }
    if (machine->narration.is_running || machine->narration.press_is_pending) {
        machine->narration.is_running = 0;
        machine->narration.press_is_pending = 0;
        note_leftover(machine, "narration");
    }
    if (machine->choice.control_is_up) {
        note_leftover(machine, "choice");
    }
    script_choice_clear_all(machine);
}

void script_machine_note(struct script_machine *machine, const struct script_vm *vm, const char *reason, const char *detail)
{
    char text[SCRIPT_SOURCE_NAME_MAX + 96];
    unsigned int op;
    unsigned int hash;

    op = 0;
    if (vm != NULL) {
        script_file_read_word(&vm->source->file, vm->pc, &op);
    }
    hash = hash_text(hash_text(FNV_OFFSET_BASIS + op, reason), detail);
    hash = hash % (SCRIPT_NOTED_PROBLEM_BYTES * 8u);
    if (machine->noted_problems[hash / 8] & (1u << (hash % 8))) {
        return;
    }
    machine->noted_problems[hash / 8] = (unsigned char)(machine->noted_problems[hash / 8] | (1u << (hash % 8)));
    if (machine->outputs->marked == NULL) {
        return;
    }
    if (vm != NULL) {
        snprintf(text, sizeof(text), "%s op=0x%x file=%s object=%s tick=%u", detail, op, vm->source->name, vm->owner != NULL ? vm->owner->name : "runner", machine->tick);
    } else {
        snprintf(text, sizeof(text), "%s tick=%u", detail, machine->tick);
    }
    machine->outputs->marked(reason, text);
}

void script_machine_end_block(struct script_machine *machine, struct script_vm *vm, const char *reason)
{
    script_machine_note(machine, vm, reason, "");
    vm->has_ended = 1;
}

const char *script_machine_string(struct script_machine *machine, const struct script_vm *vm, unsigned int string_offset)
{
    const char *text;

    text = script_file_string(&vm->source->file, string_offset);
    if (text == NULL) {
        script_machine_note(machine, vm, "bad_string", "");
    }
    return text;
}

int script_machine_index_of_object(const struct script_machine *machine, const char *name)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        if (strcmp(machine->objects[index].name, name) == 0) {
            return index;
        }
    }
    return SCRIPT_NO_OBJECT;
}

struct script_object *script_machine_find_object(struct script_machine *machine, const char *name)
{
    int index;

    index = script_machine_index_of_object(machine, name);
    return index == SCRIPT_NO_OBJECT ? NULL : &machine->objects[index];
}

struct script_object *script_machine_current_object(struct script_machine *machine, const struct script_vm *vm)
{
    if (vm->selected_object != SCRIPT_NO_SELECTION) {
        return &machine->objects[vm->selected_object];
    }
    return vm->owner;
}

unsigned int script_machine_u32(const unsigned int *words, int first)
{
    return words[first] | (words[first + 1] << 16);
}

static int text_is_all_objects(const char *target)
{
    return target[0] == '*';
}

static void queue_message(struct script_object *object, const char *verb, const char *arg)
{
    if (object->is_sleeping && !(strcmp(verb, "command") == 0 && strcmp(arg, "wake") == 0)) {
        return;
    }
    if (object->message_count >= SCRIPT_MESSAGE_MAX) {
        return;
    }
    object->messages[object->message_count].verb = verb;
    object->messages[object->message_count].arg = arg;
    object->message_count = object->message_count + 1;
}

void script_machine_send(struct script_machine *machine, const char *target, const char *verb, const char *arg)
{
    int index;

    if (arg[0] == '\0') {
        arg = "*";
    }
    for (index = 0; index < machine->object_count; index++) {
        if (text_is_all_objects(target) || strcmp(machine->objects[index].name, target) == 0) {
            queue_message(&machine->objects[index], verb, arg);
        }
    }
}

static void queue_gameapp(struct script_machine *machine, const struct script_vm *vm, const char *verb, const char *arg)
{
    if (!script_soul_knows_command(verb)) {
        char pair[SCRIPT_SOURCE_NAME_MAX];

        snprintf(pair, sizeof(pair), "%s|%s", verb, arg);
        script_machine_note(machine, vm, "gameapp_not_ported", pair);
        return;
    }
    if (machine->gameapp_count >= SCRIPT_GAMEAPP_MAX) {
        script_machine_note(machine, vm, "too_many_gameapp_commands", verb);
        return;
    }
    machine->gameapp_verbs[machine->gameapp_count] = verb;
    machine->gameapp_args[machine->gameapp_count] = arg;
    machine->gameapp_count = machine->gameapp_count + 1;
}

static void send_from_vm(struct script_machine *machine, const struct script_vm *vm, const char *target, const char *verb, const char *arg)
{
    if (strcmp(target, GAME_APP_TARGET) == 0) {
        queue_gameapp(machine, vm, verb, arg);
        return;
    }
    script_machine_send(machine, target, verb, arg);
}

static int handler_key_matches(const struct script_handler *handler, const char *verb, const char *arg)
{
    return strcmp(handler->verb, verb) == 0 && (handler->arg[0] == '\0' || strcmp(handler->arg, arg) == 0);
}

int script_machine_handles(const struct script_machine *machine, const char *verb, const char *arg)
{
    int object;
    int handler;

    for (object = 0; object < machine->object_count; object++) {
        for (handler = 0; handler < machine->objects[object].handler_count; handler++) {
            if (handler_key_matches(&machine->objects[object].handlers[handler], verb, arg)) {
                return 1;
            }
        }
    }
    return 0;
}

static int deliver(struct script_handler *handler, const char *verb, const char *arg)
{
    if (!handler_key_matches(handler, verb, arg)) {
        return 0;
    }
    handler->is_busy = 1;
    return 1;
}

static int deliver_to_later_handlers(struct script_vm *vm, const char *verb, const char *arg)
{
    struct script_object *object;
    int delivered;
    int found_self;
    int index;

    object = vm->owner;
    delivered = 0;
    found_self = 0;
    for (index = 0; index < object->handler_count; index++) {
        if (found_self) {
            delivered = delivered | deliver(&object->handlers[index], verb, arg);
        }
        if (&object->handlers[index].vm == vm) {
            found_self = 1;
        }
    }
    return delivered;
}

static void run_send(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *target;
    const char *verb;
    const char *arg;
    int delivered;

    target = script_machine_string(machine, vm, words[0]);
    verb = script_machine_string(machine, vm, words[1]);
    arg = script_machine_string(machine, vm, words[2]);
    if (target == NULL || verb == NULL || arg == NULL) {
        return;
    }
    if (strcmp(verb, "demo_end") == 0 && machine->outputs->marked != NULL) {
        machine->outputs->marked("demo_end", target);
    }
    if (vm->kind == SCRIPT_VM_HANDLER) {
        delivered = 0;
        if (!text_is_all_objects(target) && strcmp(target, vm->owner->name) == 0) {
            delivered = deliver_to_later_handlers(vm, verb, arg[0] == '\0' ? "*" : arg);
        }
        if (!delivered) {
            send_from_vm(machine, vm, target, verb, arg);
        }
    } else if (vm->kind == SCRIPT_VM_RUNNER) {
        send_from_vm(machine, vm, target, verb, arg);
    }
}

static void create_object(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_object *object;
    const char *name;

    if (machine->object_count >= SCRIPT_OBJECT_MAX) {
        script_machine_note(machine, vm, "too_many_objects", "");
        return;
    }
    name = script_machine_string(machine, vm, words[3]);
    if (name == NULL) {
        return;
    }
    object = &machine->objects[machine->object_count];
    memset(object, 0, sizeof(*object));
    object->name = name;
    object->idle_handler = SCRIPT_NO_SELECTION;
    begin_vm(&object->main_vm, SCRIPT_VM_MAIN, vm->source, object, vm->pc + 5);
    begin_vm(&object->scanner, SCRIPT_VM_SCANNER, vm->source, object, vm->pc + 5);
    machine->object_count = machine->object_count + 1;
}

static void register_handler(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_object *object;
    struct script_handler *handler;
    const char *verb;
    const char *arg;

    object = vm->owner;
    if (object->handler_count >= SCRIPT_HANDLER_MAX) {
        script_machine_note(machine, vm, "too_many_handlers", object->name);
        return;
    }
    verb = script_machine_string(machine, vm, words[3]);
    arg = script_machine_string(machine, vm, words[4]);
    if (verb == NULL || arg == NULL) {
        return;
    }
    handler = &object->handlers[object->handler_count];
    memset(handler, 0, sizeof(*handler));
    begin_vm(&handler->vm, SCRIPT_VM_HANDLER, vm->source, object, vm->pc + 6);
    handler->first_pc = vm->pc + 6;
    handler->verb = verb;
    handler->arg = arg;
    if (strcmp(verb, "idle") == 0 && strcmp(arg, "*") == 0) {
        object->idle_handler = object->handler_count;
    }
    object->handler_count = object->handler_count + 1;
}

static int step_vm(struct script_machine *machine, struct script_vm *vm);

static void scan_act_body(struct script_machine *machine, struct script_object *object)
{
    unsigned int op;
    int steps;

    object->scanner.has_ended = 0;
    for (steps = 0; steps < SCRIPT_RUN_STEP_MAX; steps++) {
        if (script_file_read_word(&object->scanner.source->file, object->scanner.pc, &op) != SCRIPT_FILE_OK) {
            script_machine_note(machine, &object->scanner, "word_past_code", "");
            return;
        }
        if (op == 0x112 || object->scanner.has_ended) {
            return;
        }
        step_vm(machine, &object->scanner);
    }
    script_machine_note(machine, &object->scanner, "runaway", object->name);
}

static void start_act(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    struct script_object *object;
    const char *act_name;

    object = vm->owner;
    object->handler_count = 0;
    object->idle_handler = SCRIPT_NO_SELECTION;
    object->act_was_requested = 0;
    object->scanner.source = vm->source;
    object->scanner.pc = vm->pc + 5;
    scan_act_body(machine, object);
    script_machine_send(machine, object->name, "SYSTEM", "ACT_JUMP");
    act_name = script_machine_string(machine, vm, words[3]);
    object->current_act = act_name;
    if (act_name != NULL && machine->outputs->marked != NULL) {
        machine->outputs->marked("act_begun", act_name);
    }
    if (act_name != NULL) {
        script_soul_act_began(machine, object->name, act_name);
    }
    if (act_name != NULL && machine->stop_act != NULL && strcmp(act_name, machine->stop_act) == 0) {
        machine->stop_is_requested = 1;
    }
}

static int move_vm_to_offset(struct script_machine *machine, struct script_vm *vm, unsigned int file_offset)
{
    unsigned int word;

    if (file_offset == 0 || script_file_word_of_offset(&vm->source->file, file_offset, &word) != SCRIPT_FILE_OK) {
        script_machine_end_block(machine, vm, "bad_jump");
        return 0;
    }
    vm->pc = word;
    return 1;
}

static void begin_script_change(struct script_machine *machine, const char *path);

static void request_script_change(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *path;

    path = script_machine_string(machine, vm, words[0]);
    if (path == NULL) {
        return;
    }
    begin_script_change(machine, path);
}

static void read_flag_into_condition(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    int value;

    if (!script_flags_read_known(&machine->flags, script_machine_u32(words, 0), &value)) {
        script_machine_note(machine, vm, "flag_out_of_range", "");
        return;
    }
    vm->condition = value ? 1 : 0;
}

static void write_flag_byte(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    if (!script_flags_write_byte(&machine->flags, script_machine_u32(words, 0), words[2] & 0xffu)) {
        script_machine_note(machine, vm, "flag_out_of_range", "");
    }
}

static void read_flag_byte(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    vm->reg_a = script_flags_read_byte(&machine->flags, script_machine_u32(words, 0));
}

static void write_flag(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int value)
{
    if (!script_flags_write_known(&machine->flags, script_machine_u32(words, 0), value)) {
        script_machine_note(machine, vm, "flag_out_of_range", "");
    }
}

static void set_pause_class(struct script_vm *vm, int pause_class)
{
    if (vm->owner != NULL) {
        vm->owner->main_vm.pause_class = pause_class;
    } else {
        vm->pause_class = pause_class;
    }
}

int script_machine_name_matches_target(const char *name, const char *target)
{
    if (target[0] == '*') {
        return 1;
    }
    if (target[0] == '@') {
        return strstr(name, target + 1) != NULL;
    }
    return strcmp(name, target) == 0;
}

static void reset_handler(struct script_handler *handler)
{
    handler->vm.pc = handler->vm.start_pc;
    handler->vm.wait_ticks = 0;
    handler->vm.call_depth = 0;
    handler->vm.has_ended = 0;
    handler->vm.exit_is_requested = 0;
    handler->is_busy = 0;
}

static void cancel_on_object(struct script_object *object, const struct script_vm *vm, const char *verb, const char *arg)
{
    int handler;

    for (handler = 0; handler < object->handler_count; handler++) {
        struct script_handler *candidate;

        candidate = &object->handlers[handler];
        if ((handler_key_matches(candidate, verb, arg) || (verb[0] == '*' && handler != object->idle_handler)) && &candidate->vm != vm) {
            reset_handler(candidate);
        }
    }
}

static void cancel_handlers(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *target;
    const char *verb;
    const char *arg;
    int index;

    target = script_machine_string(machine, vm, words[0]);
    verb = script_machine_string(machine, vm, words[1]);
    arg = script_machine_string(machine, vm, words[2]);
    if (target == NULL || verb == NULL || arg == NULL) {
        return;
    }
    if (target[0] == '?') {
        if (vm->owner != NULL) {
            cancel_on_object(vm->owner, vm, verb, arg);
        }
        return;
    }
    for (index = 0; index < machine->object_count; index++) {
        if (script_machine_name_matches_target(machine->objects[index].name, target)) {
            cancel_on_object(&machine->objects[index], vm, verb, arg);
        }
    }
}

static void set_every_other_object_asleep(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int is_asleep)
{
    const char *name;
    int index;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    for (index = 0; index < machine->object_count; index++) {
        if (strcmp(machine->objects[index].name, name) != 0) {
            machine->objects[index].is_sleeping = is_asleep;
        }
    }
}

static void set_object_asleep(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int is_asleep)
{
    const char *name;
    struct script_object *object;

    name = script_machine_string(machine, vm, words[0]);
    if (name == NULL) {
        return;
    }
    object = script_machine_find_object(machine, name);
    if (object != NULL) {
        object->is_sleeping = is_asleep;
    }
}

#define ACTOR_REGISTER_FRAME 2u
#define ACTOR_REGISTER_FLIP 3u
#define COUNTER_REGISTER_FIRST 0x0cu
#define COUNTER_REGISTER_LAST 0x10u
#define REGISTER_A 0x1eu
#define CODE_PATCH_HALF_WORD 1
#define CODE_PATCH_FULL_WORD 2

static int read_actor_register(const struct script_machine *machine, const struct script_vm *vm, unsigned int reg)
{
    const struct script_object *object;
    const struct script_actor *actor;

    object = vm->selected_object != SCRIPT_NO_SELECTION ? &machine->objects[vm->selected_object] : vm->owner;
    actor = object != NULL ? &object->actor : NULL;
    if (actor == NULL || !actor->exists) {
        return 0;
    }
    if (reg == 0) {
        return (actor->x + actor->points[0][0]) * NATIVE_FIXED_ONE;
    }
    if (reg == 1) {
        return (actor->y + actor->points[0][1]) * NATIVE_FIXED_ONE;
    }
    if (reg == ACTOR_REGISTER_FRAME) {
        return actor->frame_word_index;
    }
    if (reg == ACTOR_REGISTER_FLIP) {
        return actor->is_flipped ? 1 : 0;
    }
    if (reg >= 4 && reg < 8) {
        return (actor->x + actor->points[reg - 4][0]) * NATIVE_FIXED_ONE;
    }
    if (reg >= 8 && reg < 12) {
        return (actor->y + actor->points[reg - 8][1]) * NATIVE_FIXED_ONE;
    }
    if (reg >= COUNTER_REGISTER_FIRST && reg <= COUNTER_REGISTER_LAST) {
        return actor->counters[reg - COUNTER_REGISTER_FIRST];
    }
    return 0;
}

#define TARGET_POSITION_WHEN_NONE (-0x400000)
#define LANGUAGE_ENGLISH 1

static int ghost_target_position(const struct script_machine *machine, int along_x)
{
    float core_x;
    float core_y;

    if (machine->soul.active_object < 0 || !machine->objects[machine->soul.active_object].actor.exists) {
        return along_x ? TARGET_POSITION_WHEN_NONE : 0;
    }
    script_machine_node_position(&machine->objects[machine->soul.active_object], SCRIPT_FIRST_CORE_NODE + machine->soul.active_slot, &core_x, &core_y);
    return (int)(along_x ? core_x : core_y) * NATIVE_FIXED_ONE;
}

static int read_register(const struct script_machine *machine, const struct script_vm *vm, unsigned int reg)
{
    switch (reg) {
    case 0x12:
        return ghost_target_position(machine, 1);
    case 0x13:
        return ghost_target_position(machine, 0);
    case 0x16:
        return machine->fate_level;
    case 0x1d:
        return LANGUAGE_ENGLISH;
    case 0x14:
        return machine->camera_x * NATIVE_FIXED_ONE;
    case 0x15:
        return machine->camera_y * NATIVE_FIXED_ONE;
    case 0x19:
        return machine->chapter_mark;
    case 0x1a:
        return machine->phone_book_place;
    case 0x1b:
        return machine->mode_byte;
    case 0x1c:
        return 1;
    case 0x1e:
        return vm->reg_a;
    default:
        return read_actor_register(machine, vm, reg);
    }
}

static int compare_values(int left, int right, unsigned int comparison)
{
    switch (comparison) {
    case 0x1f:
        return left == right;
    case 0x20:
        return left != right;
    case 0x21:
        return left < right;
    case 0x22:
        return left <= right;
    case 0x23:
        return left > right;
    case 0x24:
        return left >= right;
    default:
        return 0;
    }
}

static void compare_register(const struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    vm->condition = compare_values(read_register(machine, vm, words[0]), (int)script_machine_u32(words, 1), words[3]);
}

static struct script_actor *current_actor(struct script_machine *machine, struct script_vm *vm)
{
    struct script_object *object;

    object = vm->selected_object != SCRIPT_NO_SELECTION ? &machine->objects[vm->selected_object] : vm->owner;
    if (object == NULL || !object->actor.exists) {
        return NULL;
    }
    return &object->actor;
}

static void store_register(struct script_machine *machine, struct script_vm *vm, unsigned int reg, int value, int is_relative)
{
    struct script_actor *actor;

    actor = current_actor(machine, vm);
    if (actor == NULL) {
        return;
    }
    if (is_relative) {
        value = value + read_register(machine, vm, reg);
    }
    if (reg >= COUNTER_REGISTER_FIRST && reg <= COUNTER_REGISTER_LAST) {
        actor->counters[reg - COUNTER_REGISTER_FIRST] = (unsigned char)value;
    } else if (reg == REGISTER_A) {
        vm->reg_a = value;
    }
}

#define NAME_OF_CURRENT_ACTOR '?'
#define DISTANCE_WHEN_AN_ACTOR_IS_MISSING (-4096)
#define DISTANCE_ALONG_X 0

static const struct script_actor *named_actor(struct script_machine *machine, struct script_vm *vm, unsigned int name_word)
{
    const char *name;
    const struct script_object *object;

    name = script_machine_string(machine, vm, name_word);
    if (name == NULL) {
        return NULL;
    }
    if (name[0] == NAME_OF_CURRENT_ACTOR) {
        return current_actor(machine, vm);
    }
    object = script_machine_find_object(machine, name);
    return object != NULL && object->actor.exists ? &object->actor : NULL;
}

static int object_is_named_or_current(struct script_machine *machine, struct script_vm *vm, unsigned int name_word)
{
    const char *name;

    name = script_machine_string(machine, vm, name_word);
    if (name == NULL) {
        return 0;
    }
    if (name[0] == NAME_OF_CURRENT_ACTOR) {
        return current_actor(machine, vm) != NULL;
    }
    return script_machine_find_object(machine, name) != NULL;
}

static int point_position(const struct script_actor *actor, unsigned int point, int along_x)
{
    if (point >= SCRIPT_ACTOR_POINT_MAX) {
        point = 0;
    }
    return along_x ? actor->x + actor->points[point][0] : actor->y + actor->points[point][1];
}

static void store_distance_between_points(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int axis)
{
    const struct script_actor *first;
    const struct script_actor *second;
    int difference;

    vm->reg_a = DISTANCE_WHEN_AN_ACTOR_IS_MISSING;
    first = named_actor(machine, vm, words[0]);
    second = named_actor(machine, vm, words[2]);
    if (first == NULL || second == NULL) {
        return;
    }
    difference = (point_position(second, words[3], axis == DISTANCE_ALONG_X) - point_position(first, words[1], axis == DISTANCE_ALONG_X)) * NATIVE_FIXED_ONE;
    vm->reg_a = difference < 0 ? -difference : difference;
}

static void patch_own_code(struct script_machine *machine, struct script_vm *vm, unsigned int word_offset, unsigned int value, int word_count)
{
    struct script_source *source;
    int index;

    source = &machine->sources[vm->source - machine->sources];
    for (index = 0; index < word_count; index++) {
        if (script_file_write_word(&source->file, vm->pc + word_offset + (unsigned int)index, (value >> (16 * index)) & 0xffffu) != SCRIPT_FILE_OK) {
            script_machine_note(machine, vm, "code_patch_outside", "");
            return;
        }
    }
}

static enum script_step_result call_subroutine(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    if (vm->call_depth >= SCRIPT_CALL_DEPTH_MAX) {
        script_machine_note(machine, vm, "call_depth_overflow", "");
        vm->pc = vm->pc + 3;
        return SCRIPT_STEP_MOVED;
    }
    vm->call_stack[vm->call_depth] = vm->pc + 3;
    vm->call_depth = vm->call_depth + 1;
    if (!move_vm_to_offset(machine, vm, script_machine_u32(words, 0))) {
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    }
    return SCRIPT_STEP_MOVED;
}

static enum script_step_result return_from_subroutine(struct script_machine *machine, struct script_vm *vm)
{
    if (vm->call_depth == 0) {
        script_machine_end_block(machine, vm, "return_with_empty_stack");
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    }
    vm->call_depth = vm->call_depth - 1;
    vm->pc = vm->call_stack[vm->call_depth];
    return SCRIPT_STEP_MOVED;
}

static void note_entry(struct script_machine *machine, unsigned int list, unsigned int id, unsigned int level)
{
    if (list >= SCRIPT_NOTE_LIST_COUNT || id == 0 || id >= SCRIPT_NOTE_ID_COUNT) {
        return;
    }
    if (level + 1u > machine->note_levels[list][id]) {
        machine->note_levels[list][id] = (unsigned char)(level + 1u);
        machine->note_was_added = 1;
    }
}

static enum script_step_result run_control_op(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    switch (op) {
    case 0x0:
    case 0x1:
    case 0x112:
    case 0x113:
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x15:
        vm->pc = vm->pc + 4;
        return SCRIPT_STEP_MOVED;
    case 0x16:
        create_object(machine, vm, words);
        vm->pc = vm->pc + 1;
        return SCRIPT_STEP_MOVED;
    case 0x1c:
        if (vm->kind == SCRIPT_VM_HANDLER) {
            vm->pc = vm->start_pc;
            vm->call_depth = 0;
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        vm->pc = vm->pc + 1;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x19:
        if (vm->kind == SCRIPT_VM_MAIN) {
            start_act(machine, vm, words);
            vm->pc = vm->pc + 1;
            return SCRIPT_STEP_MOVED;
        }
        if (vm->kind == SCRIPT_VM_HANDLER) {
            vm->owner->main_vm.pc = vm->pc;
            vm->owner->main_vm.wait_ticks = 0;
            vm->owner->act_was_requested = 1;
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        vm->pc = vm->pc + 1;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x1a:
        if (vm->kind == SCRIPT_VM_SCANNER || vm->kind == SCRIPT_VM_HANDLER) {
            register_handler(machine, vm, words);
            vm->pc = vm->pc + 1;
            return SCRIPT_STEP_MOVED;
        }
        vm->pc = vm->pc + 1;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x1b:
        cancel_handlers(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x1d:
        run_send(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x1e:
        set_pause_class(vm, PAUSE_CLASS_TALK);
        return SCRIPT_STEP_NEXT;
    case 0x1f:
        set_pause_class(vm, PAUSE_CLASS_NORMAL);
        return SCRIPT_STEP_NEXT;
    case 0x20:
        set_pause_class(vm, PAUSE_CLASS_FREE);
        return SCRIPT_STEP_NEXT;
    case 0x21:
        set_pause_class(vm, PAUSE_CLASS_ONE);
        return SCRIPT_STEP_NEXT;
    case 0x22:
    case 0x23:
        if (!move_vm_to_offset(machine, vm, script_machine_u32(words, 0))) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        return SCRIPT_STEP_MOVED;
    case 0x24:
        request_script_change(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x26:
        set_object_asleep(machine, vm, words, 1);
        return SCRIPT_STEP_NEXT;
    case 0x27:
        set_object_asleep(machine, vm, words, 0);
        return SCRIPT_STEP_NEXT;
    case 0x2d:
        return call_subroutine(machine, vm, words);
    case 0x2e:
        return return_from_subroutine(machine, vm);
    case 0x31:
        vm->exit_is_requested = 1;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x65:
        compare_register(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x66:
        machine->random_state = machine->random_state * 1103515245u + 12345u;
        vm->condition = words[0] != 0 && (machine->random_state >> 16) % words[0] == 0;
        return SCRIPT_STEP_NEXT;
    case 0x67:
        machine->mode_byte = MODE_BYTE_ALSO_RUNS_IDLE;
        if (machine->soul.mark_age_ticks < 0) {
            script_soul_start_player_mark(machine);
        }
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x68:
        machine->mode_byte = MODE_BYTE_NORMAL;
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x69:
        vm->condition = machine->mode_byte == MODE_BYTE_NORMAL;
        return SCRIPT_STEP_NEXT;
    case 0x7e:
        script_soul_start_player_mark(machine);
        return SCRIPT_STEP_NEXT;
    case 0x6d:
        machine->soul.free_camera_is_forbidden = 0;
        return SCRIPT_STEP_NEXT;
    case 0x6e:
        machine->soul.free_camera_is_forbidden = 1;
        return SCRIPT_STEP_NEXT;
    case 0x77:
        machine->phone_view_is_on = 1;
        if (machine->outputs->soul_vision_changed != NULL) {
            machine->outputs->soul_vision_changed(1, 1);
        }
        return SCRIPT_STEP_NEXT;
    case 0x73:
        if (machine->outputs->film_scroll_paused != NULL) {
            machine->outputs->film_scroll_paused(words[0] != 0);
        }
        return SCRIPT_STEP_NEXT;
    case 0xdf:
    case 0xe0:
        if (machine->outputs->swirl_changed != NULL) {
            machine->outputs->swirl_changed(op == 0xe0);
        }
        return SCRIPT_STEP_NEXT;
    case 0xf3:
    case 0xf5:
        if (machine->outputs->screen_still_changed != NULL) {
            machine->outputs->screen_still_changed(op == 0xf3);
        }
        return op == 0xf3 ? SCRIPT_STEP_NEXT_THEN_YIELD : SCRIPT_STEP_NEXT;
    case 0xe1:
        if (vm->owner != NULL) {
            vm->owner->is_dead = 1;
        }
        return SCRIPT_STEP_NEXT;
    case 0xe5:
        machine->soul.ghost_pill_is_held_out = 0;
        return SCRIPT_STEP_NEXT;
    case 0xe6:
        machine->soul.ghost_pill_is_held_out = 1;
        return SCRIPT_STEP_NEXT;
    case 0xe7:
        machine->soul.trick_pill_is_held_out = 0;
        return SCRIPT_STEP_NEXT;
    case 0xe8:
        machine->soul.trick_pill_is_held_out = 1;
        return SCRIPT_STEP_NEXT;
    case 0xe9:
    case 0xea:
        if (vm->owner != NULL) {
            if (op == 0xea) {
                vm->owner->actor.busy_cores = vm->owner->actor.busy_cores | (1 << words[0]);
            } else {
                vm->owner->actor.busy_cores = vm->owner->actor.busy_cores & ~(1 << words[0]);
            }
        }
        return SCRIPT_STEP_NEXT;
    case 0xfe:
        machine->soul.rollback_is_on = 1;
        return SCRIPT_STEP_NEXT;
    case 0xff:
        machine->soul.rollback_is_on = 0;
        return SCRIPT_STEP_NEXT;
    case 0x74:
        if (words[0] == 0 || (int)words[0] > machine->chapter_mark) {
            machine->chapter_mark = (int)words[0];
        }
        return SCRIPT_STEP_NEXT;
    case 0xc4:
    case 0x12e:
    case 0x12f:
    case 0x130:
    case 0x131:
    case 0x132:
    case 0xf0:
        return SCRIPT_STEP_NEXT;
    case 0x76:
        note_entry(machine, words[0], words[1], words[2]);
        return SCRIPT_STEP_NEXT;
    case 0x79:
        machine->note_was_added = 0;
        return SCRIPT_STEP_NEXT;
    case 0x7a:
        vm->condition = machine->note_was_added;
        if (machine->note_was_added) {
            script_talk_open(machine, NOTE_ADDED_TALK_LABEL);
            return SCRIPT_STEP_NEXT_THEN_YIELD;
        }
        return SCRIPT_STEP_NEXT;
    case 0x7b:
        vm->condition = machine->note_was_added;
        return SCRIPT_STEP_NEXT;
    case 0xe2:
        machine->pause_bit = 1;
        set_pause_class(vm, PAUSE_CLASS_TALK);
        machine->demo_soul_is_on = 1;
        if (machine->outputs->soul_vision_changed != NULL) {
            machine->outputs->soul_vision_changed(1, 1);
        }
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0xe3:
        machine->mode_byte = MODE_BYTE_NORMAL;
        machine->pause_bit = 0;
        set_pause_class(vm, PAUSE_CLASS_NORMAL);
        machine->demo_soul_is_on = 0;
        if (machine->outputs->soul_vision_changed != NULL) {
            machine->outputs->soul_vision_changed(0, 1);
        }
        return SCRIPT_STEP_NEXT;
    case 0xec:
        if (machine->phone_view_is_on) {
            machine->phone_view_is_on = 0;
            if (machine->outputs->soul_vision_changed != NULL) {
                machine->outputs->soul_vision_changed(0, 0);
            }
            if (machine->outputs->phone_view_ended != NULL) {
                machine->outputs->phone_view_ended();
            }
        }
        if (machine->auto_press != SCRIPT_AUTO_FAST && machine->outputs->movie_of_class_started != NULL) {
            machine->movie_ticks_left = machine->outputs->movie_of_class_started(script_machine_u32(words, 0));
            if (machine->movie_ticks_left > 0 && machine->outputs->sound_played != NULL) {
                machine->outputs->sound_played(MOVIE_SOUND_KIND);
            }
        }
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0xed:
        if (machine->movie_ticks_left > 0) {
            return SCRIPT_STEP_MOVED_THEN_YIELD;
        }
        return SCRIPT_STEP_NEXT;
    case 0xef:
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x18:
        vm->condition = object_is_named_or_current(machine, vm, words[0]);
        return SCRIPT_STEP_NEXT;
    case 0x28:
        set_every_other_object_asleep(machine, vm, words, 1);
        return SCRIPT_STEP_NEXT;
    case 0x29:
        set_every_other_object_asleep(machine, vm, words, 0);
        return SCRIPT_STEP_NEXT;
    case 0x63:
        write_flag_byte(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x64:
        read_flag_byte(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x92:
    case 0x93:
        store_distance_between_points(machine, vm, words, (int)op - 0x92);
        return SCRIPT_STEP_NEXT;
    case 0xfb:
        script_flags_save_game_bank(&machine->flags);
        return SCRIPT_STEP_NEXT;
    case 0xfc:
        script_flags_restore_game_bank(&machine->flags);
        return SCRIPT_STEP_NEXT;
    case 0x127:
        vm->reg_a = read_register(machine, vm, words[0]);
        return SCRIPT_STEP_NEXT;
    case 0x128:
        store_register(machine, vm, words[0], (int)script_machine_u32(words, 1), 0);
        return SCRIPT_STEP_NEXT;
    case 0x129:
        store_register(machine, vm, words[0], 1, 1);
        return SCRIPT_STEP_NEXT;
    case 0x12a:
        store_register(machine, vm, words[0], -1, 1);
        return SCRIPT_STEP_NEXT;
    case 0x12b:
        store_register(machine, vm, words[0], 0, 0);
        return SCRIPT_STEP_NEXT;
    case 0x12c:
        patch_own_code(machine, vm, words[0], (unsigned int)vm->reg_a, CODE_PATCH_HALF_WORD);
        return SCRIPT_STEP_NEXT;
    case 0x12d:
        patch_own_code(machine, vm, words[0], (unsigned int)vm->reg_a, CODE_PATCH_FULL_WORD);
        return SCRIPT_STEP_NEXT;
    case 0x2f:
        vm->wait_ticks = words[0];
        return SCRIPT_STEP_NEXT_THEN_YIELD;
    case 0x30:
        machine->has_runner = 0;
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    case 0x32:
        read_flag_into_condition(machine, vm, words);
        return SCRIPT_STEP_NEXT;
    case 0x33:
        vm->pc = vm->pc + (vm->condition ? 4 : 1);
        return SCRIPT_STEP_MOVED;
    case 0x34:
        vm->pc = vm->pc + (vm->condition ? 1 : 4);
        return SCRIPT_STEP_MOVED;
    case 0x61:
        write_flag(machine, vm, words, 1);
        return SCRIPT_STEP_NEXT;
    case 0x62:
        write_flag(machine, vm, words, 0);
        return SCRIPT_STEP_NEXT;
    case 0x114:
        vm->condition = 1;
        return SCRIPT_STEP_NEXT;
    case 0x115:
        vm->condition = 0;
        return SCRIPT_STEP_NEXT;
    default:
        return SCRIPT_STEP_NEXT;
    }
}

static int vm_is_paused(const struct script_machine *machine, const struct script_vm *vm)
{
    int pause_class;

    if (vm->kind == SCRIPT_VM_RUNNER) {
        return 0;
    }
    pause_class = vm->owner->main_vm.pause_class;
    if (pause_class == PAUSE_CLASS_FREE) {
        return 0;
    }
    if (pause_class == PAUSE_CLASS_TALK) {
        return machine->talk_is_open;
    }
    return machine->pause_bit;
}

static void note_stored_op_once(struct script_machine *machine, unsigned int op)
{
    char text[16];

    if (machine->noted_ops[op / 8] & (1u << (op % 8))) {
        return;
    }
    machine->noted_ops[op / 8] = (unsigned char)(machine->noted_ops[op / 8] | (1u << (op % 8)));
    snprintf(text, sizeof(text), "0x%x", op);
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("stored_only", text);
    }
}

static int read_operands(struct script_machine *machine, struct script_vm *vm, unsigned int length, unsigned int *words)
{
    unsigned int index;

    for (index = 1; index < length; index++) {
        if (script_file_read_word(&vm->source->file, vm->pc + index, &words[index - 1]) != SCRIPT_FILE_OK) {
            script_machine_end_block(machine, vm, "word_past_code");
            return 0;
        }
    }
    return 1;
}

static void note_visited_site(struct script_machine *machine, const struct script_vm *vm)
{
    unsigned int site;

    site = ((unsigned int)(vm->source - machine->sources) * 2654435761u + vm->pc * 40503u) % (SCRIPT_VISITED_SITE_BYTES * 8u);
    if (machine->visited_sites[site / 8] & (1u << (site % 8))) {
        return;
    }
    machine->visited_sites[site / 8] = (unsigned char)(machine->visited_sites[site / 8] | (1u << (site % 8)));
    machine->last_new_site_tick = machine->tick;
}

static void remember_fetch(struct script_machine *machine, const struct script_vm *vm, unsigned int op, const unsigned int *words, unsigned int length)
{
    struct script_recent_fetch *recent;
    struct script_fetch fetch;
    unsigned int index;

    recent = &machine->recent_fetches[machine->fetch_total % SCRIPT_RECENT_FETCH_MAX];
    recent->tick = machine->tick;
    recent->offset = vm->pc * SCRIPT_FILE_BYTES_PER_WORD + SCRIPT_FILE_HEADER_SIZE;
    recent->op = op;
    recent->object_name = vm->owner != NULL ? vm->owner->name : "runner";
    machine->fetch_total = machine->fetch_total + 1;
    note_visited_site(machine, vm);
    if (machine->outputs->fetched == NULL) {
        return;
    }
    fetch.tick = machine->tick;
    fetch.vm_kind = vm_kind_name(vm->kind);
    fetch.file_name = vm->source->name;
    fetch.object_name = recent->object_name;
    fetch.offset = recent->offset;
    fetch.op = op;
    fetch.word_count = (int)length - 1;
    fetch.condition = vm->condition;
    for (index = 0; index + 1 < length; index++) {
        fetch.words[index] = words[index];
    }
    machine->outputs->fetched(&fetch);
}

static enum script_step_result run_op(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    switch (script_opcode_family(op)) {
    case SCRIPT_OP_CONTROL:
        return run_control_op(machine, vm, op, words);
    case SCRIPT_OP_ACTOR:
        return script_ops_actor_run(machine, vm, op, words);
    case SCRIPT_OP_VIEW:
        return script_ops_view_run(machine, vm, op, words);
    case SCRIPT_OP_MEDIA:
        return script_ops_media_run(machine, vm, op, words);
    case SCRIPT_OP_CHOICE:
        return script_choice_run_op(machine, vm, op, words);
    case SCRIPT_OP_STORES_ONLY:
        note_stored_op_once(machine, op);
        return SCRIPT_STEP_NEXT;
    default:
        script_machine_note(machine, vm, "op_not_ported", "");
        return SCRIPT_STEP_NEXT;
    }
}

static int step_vm(struct script_machine *machine, struct script_vm *vm)
{
    unsigned int words[SCRIPT_OPERAND_MAX];
    unsigned int length;
    unsigned int op;

    if (vm->has_ended) {
        return 0;
    }
    if (vm_is_paused(machine, vm)) {
        return 0;
    }
    if (vm->wait_ticks > 0) {
        vm->wait_ticks = vm->wait_ticks - 1;
        return 0;
    }
    if (script_file_read_word(&vm->source->file, vm->pc, &op) != SCRIPT_FILE_OK) {
        script_machine_end_block(machine, vm, "word_past_code");
        return 0;
    }
    length = script_opcode_length(op);
    if (length == 0) {
        script_machine_end_block(machine, vm, "op_with_no_known_length");
        return 0;
    }
    if (!read_operands(machine, vm, length, words)) {
        return 0;
    }
    remember_fetch(machine, vm, op, words, length);
    switch (run_op(machine, vm, op, words)) {
    case SCRIPT_STEP_NEXT:
        vm->pc = vm->pc + length;
        return 1;
    case SCRIPT_STEP_NEXT_THEN_YIELD:
        vm->pc = vm->pc + length;
        return 0;
    case SCRIPT_STEP_MOVED:
        return 1;
    default:
        return 0;
    }
}

static void run_vm(struct script_machine *machine, struct script_vm *vm)
{
    int steps;

    for (steps = 0; steps < SCRIPT_RUN_STEP_MAX; steps++) {
        if (!step_vm(machine, vm)) {
            return;
        }
    }
    script_machine_note(machine, vm, "runaway", "");
}

static int vm_sits_on_end_op(const struct script_vm *vm)
{
    unsigned int op;

    return script_file_read_word(&vm->source->file, vm->pc, &op) == SCRIPT_FILE_OK && op == 0x113 && vm->wait_ticks == 0;
}

static void run_handler(struct script_machine *machine, struct script_handler *handler)
{
    if (!handler->is_busy) {
        return;
    }
    run_vm(machine, &handler->vm);
    if (vm_sits_on_end_op(&handler->vm) || handler->vm.has_ended || handler->vm.exit_is_requested) {
        reset_handler(handler);
    }
}

static void deliver_queued_messages(struct script_object *object)
{
    int message;
    int handler;

    for (message = 0; message < object->message_count; message++) {
        if (strcmp(object->messages[message].verb, "command") == 0 && strcmp(object->messages[message].arg, "sleep") == 0) {
            object->is_sleeping = 1;
        } else if (strcmp(object->messages[message].verb, "command") == 0 && strcmp(object->messages[message].arg, "wake") == 0) {
            object->is_sleeping = 0;
        } else {
            for (handler = 0; handler < object->handler_count; handler++) {
                deliver(&object->handlers[handler], object->messages[message].verb, object->messages[message].arg);
            }
        }
    }
    object->message_count = 0;
}

static void update_object(struct script_machine *machine, struct script_object *object)
{
    int any_was_busy;
    int count_at_start;
    int index;

    if (object->is_sleeping) {
        int message;
        int kept;

        kept = 0;
        for (message = 0; message < object->message_count; message++) {
            if (strcmp(object->messages[message].verb, "command") == 0 && strcmp(object->messages[message].arg, "wake") == 0) {
                object->is_sleeping = 0;
            } else {
                object->messages[kept] = object->messages[message];
                kept = kept + 1;
            }
        }
        object->message_count = kept;
        return;
    }
    run_vm(machine, &object->main_vm);
    deliver_queued_messages(object);
    if (object->is_sleeping) {
        return;
    }
    any_was_busy = 0;
    count_at_start = object->handler_count;
    for (index = 0; index < count_at_start; index++) {
        if (object->handlers[index].is_busy) {
            any_was_busy = 1;
        }
        run_handler(machine, &object->handlers[index]);
        if (object->act_was_requested) {
            return;
        }
    }
    if (!any_was_busy && object->idle_handler != SCRIPT_NO_SELECTION && (machine->mode_byte == MODE_BYTE_NORMAL || machine->mode_byte == MODE_BYTE_ALSO_RUNS_IDLE)) {
        struct script_handler *idle;

        idle = &object->handlers[object->idle_handler];
        idle->is_busy = 1;
        run_handler(machine, idle);
        idle->is_busy = 0;
    }
}

static void tear_down_script(struct script_machine *machine)
{
    unsigned int pills_registered;
    int rollback_is_on;
    int trick_is_on;
    int countdown_is_kept;
    int countdown_flag;
    int countdown_is_running;
    int countdown_step;
    int countdown_ticks;
    int countdown_interval;

    machine->phone_view_is_on = 0;
    memset(machine->anchored_effects, 0, sizeof(machine->anchored_effects));
    machine->has_runner = 0;
    machine->object_count = 0;
    machine->actor_count = 0;
    machine->gameapp_count = 0;
    machine->game_start_at = NEVER;
    memset(machine->balloons, 0, sizeof(machine->balloons));
    memset(&machine->talk, 0, sizeof(machine->talk));
    machine->talk_is_open = 0;
    machine->talk_close_at = NEVER;
    machine->narration.is_running = 0;
    machine->pause_clear_at = NEVER;
    machine->fade_count = 0;
    machine->pan_is_active = 0;
    machine->camera_follow_object = SCRIPT_NO_SELECTION;
    pills_registered = machine->soul.pills_registered;
    rollback_is_on = machine->soul.rollback_is_on;
    trick_is_on = machine->soul.trick_is_on;
    countdown_is_kept = script_flags_read_known(&machine->flags, COUNTDOWN_KEPT_ACROSS_A_SCRIPT_CHANGE_FLAG, &countdown_flag) && countdown_flag != 0;
    countdown_is_running = machine->soul.countdown_is_running;
    countdown_step = machine->soul.countdown_step;
    countdown_ticks = machine->soul.countdown_ticks;
    countdown_interval = machine->countdown_interval;
    script_flags_write_known(&machine->flags, COUNTDOWN_KEPT_ACROSS_A_SCRIPT_CHANGE_FLAG, 0);
    script_soul_reset(machine);
    machine->soul.pills_registered = pills_registered;
    machine->soul.rollback_is_on = rollback_is_on;
    machine->soul.trick_is_on = trick_is_on;
    if (countdown_is_kept) {
        machine->soul.countdown_is_running = countdown_is_running;
        machine->soul.countdown_step = countdown_step;
        machine->soul.countdown_ticks = countdown_ticks;
        machine->countdown_interval = countdown_interval;
    }
    script_fades_end_mask(machine);
    machine->soul_state_keeps_pause = 0;
    machine->soul_vision_message_at = NEVER;
    machine->film_frame_is_on = 0;
    if (machine->outputs->stage_cleared != NULL) {
        machine->outputs->stage_cleared();
    }
}

static void load_pending_script(struct script_machine *machine)
{
    char name[SCRIPT_SOURCE_NAME_MAX];
    const struct script_source *source;

    script_machine_name_of_path(machine->pending_script, name, sizeof(name));
    source = find_source(machine, name);
    if (source == NULL) {
        machine->is_finished = 1;
        if (machine->outputs->marked != NULL) {
            machine->outputs->marked("script_change_past_baked_files", name);
        }
        return;
    }
    begin_script(machine, source);
}

void script_machine_note_press(struct script_machine *machine)
{
    machine->press_is_pending = 1;
}

static void run_native_steps_before_tasks(struct script_machine *machine)
{
    if (machine->pause_clear_at == (long long)machine->tick) {
        machine->pause_bit = 0;
        machine->mode_byte = MODE_BYTE_NORMAL;
        machine->pause_clear_at = NEVER;
    }
    script_balloons_count_down(machine);
    if (machine->teardown_at == (long long)machine->tick) {
        tear_down_script(machine);
        machine->teardown_at = NEVER;
    }
    if (machine->load_at == (long long)machine->tick) {
        machine->load_at = NEVER;
        load_pending_script(machine);
    }
    if (machine->game_start_at == (long long)machine->tick) {
        machine->game_start_at = NEVER;
        script_machine_send(machine, "*", "SYSTEM", "GAME START");
        machine->mode_byte = MODE_BYTE_NORMAL;
        machine->pause_bit = 0;
    }
    if (machine->soul_vision_message_at == (long long)machine->tick) {
        machine->soul_vision_message_at = NEVER;
        script_machine_send(machine, "*", "SVTUTO", "mes00");
    }
    if (machine->soul_sound_at == (long long)machine->tick) {
        machine->soul_sound_at = NEVER;
        if (machine->outputs->sound_played != NULL) {
            machine->outputs->sound_played(SOUL_SOUND_KIND);
        }
    }
    if (machine->movie_ticks_left > 0) {
        machine->movie_ticks_left = machine->movie_ticks_left - 1;
    }
    script_talk_close_when_due(machine);
    script_soul_step_before_tasks(machine);
}

static void step_narration(struct script_machine *machine)
{
    if (!machine->narration.is_running) {
        return;
    }
    if (machine->narration_started_this_tick) {
        machine->narration_started_this_tick = 0;
        return;
    }
    if (script_narration_step(&machine->narration, script_narration_report_to_outputs, machine)) {
        machine->pause_clear_at = (long long)machine->tick + NARRATION_OVERHEAD_TICKS;
    }
}

static void run_gameapp_queue(struct script_machine *machine)
{
    int index;

    for (index = 0; index < machine->gameapp_count; index++) {
        script_soul_run_command(machine, machine->gameapp_verbs[index], machine->gameapp_args[index]);
    }
    machine->gameapp_count = 0;
}

static int vm_has_a_running_timer(const struct script_vm *vm)
{
    return vm->wait_ticks > 0;
}

static int object_has_a_running_timer(const struct script_object *object)
{
    int index;

    if (vm_has_a_running_timer(&object->main_vm)) {
        return 1;
    }
    for (index = 0; index < object->handler_count; index++) {
        if (index != object->idle_handler && object->handlers[index].is_busy && vm_has_a_running_timer(&object->handlers[index].vm)) {
            return 1;
        }
    }
    return 0;
}

static int something_is_running_to_a_known_end(const struct script_machine *machine)
{
    int index;

    if (machine->narration.is_running || machine->fade_count > 0 || machine->pan_is_active) {
        return 1;
    }
    if (machine->movie_ticks_left > 0 || machine->eyecatch_ticks_left > 0 || machine->soul.view_change_ticks_left > 0 || machine->soul.phone_event.target != NULL || machine->soul.time_jump_event.target != NULL) {
        return 1;
    }
    if (machine->talk.phase != SCRIPT_TALK_CLOSED && machine->talk.phase != SCRIPT_TALK_WAITING) {
        return 1;
    }
    if (machine->teardown_at != NEVER || machine->load_at != NEVER || machine->game_start_at != NEVER || machine->pause_clear_at != NEVER || machine->soul_vision_message_at != NEVER || machine->talk_close_at != NEVER) {
        return 1;
    }
    for (index = 0; index < machine->object_count; index++) {
        if (object_has_a_running_timer(&machine->objects[index])) {
            return 1;
        }
    }
    return 0;
}

static int game_waits_on_a_press_that_works(const struct script_machine *machine)
{
    if (machine->auto_press == SCRIPT_AUTO_FAST) {
        return 0;
    }
    if (script_soul_waits_on_the_player(machine)) {
        return 1;
    }
    return machine->talk.phase != SCRIPT_TALK_CLOSED && script_talk_wants_a_press(machine);
}

static int bit_is_set_and_then_set(unsigned char *bits, unsigned int bytes, unsigned int value)
{
    unsigned int bit;
    int was_set;

    bit = value % (bytes * 8u);
    was_set = (bits[bit / 8u] >> (bit % 8u)) & 1;
    bits[bit / 8u] = (unsigned char)(bits[bit / 8u] | (1u << (bit % 8u)));
    return was_set;
}

static int verb_is_for_native_senders(const char *verb)
{
    return strcmp(verb, "EVENT") != 0;
}

static int handler_can_be_pushed(const struct script_object *object, int index, int native_only)
{
    const struct script_handler *handler;

    handler = &object->handlers[index];
    if (index == object->idle_handler || handler->is_busy || strcmp(handler->arg, "GAME START") == 0 || strcmp(handler->arg, "ACT_JUMP") == 0 || strncmp(handler->arg, DEBUG_ARG_PREFIX, sizeof(DEBUG_ARG_PREFIX) - 1) == 0) {
        return 0;
    }
    return !native_only || verb_is_for_native_senders(handler->verb);
}

static int push_first_handler_of_kind(struct script_machine *machine, int native_only)
{
    int object_index;
    int index;

    for (object_index = 0; object_index < machine->object_count; object_index++) {
        struct script_object *object;

        object = &machine->objects[object_index];
        for (index = 0; index < object->handler_count; index++) {
            char key[SCRIPT_SOURCE_NAME_MAX];

            if (!handler_can_be_pushed(object, index, native_only) || bit_is_set_and_then_set(machine->pushed_sites, sizeof(machine->pushed_sites), object->handlers[index].first_pc)) {
                continue;
            }
            object->handlers[index].is_busy = 1;
            snprintf(key, sizeof(key), "%s|%s", object->handlers[index].verb, object->handlers[index].arg);
            script_machine_note(machine, &object->main_vm, "stuck_pushed_handler", key);
            return 1;
        }
    }
    return 0;
}

static int push_a_waiting_handler(struct script_machine *machine)
{
    if (push_first_handler_of_kind(machine, 1)) {
        return 1;
    }
    return push_first_handler_of_kind(machine, 0);
}

static int word_after_op(const struct script_source *source, unsigned int pc, unsigned int *next)
{
    unsigned int op;
    unsigned int length;

    if (script_file_read_word(&source->file, pc, &op) != SCRIPT_FILE_OK) {
        return 0;
    }
    length = script_opcode_length(op);
    if (length == 0) {
        return 0;
    }
    *next = pc + length;
    return 1;
}

static int move_object_to_next_act(struct script_machine *machine, struct script_object *object)
{
    unsigned int pc;
    unsigned int op;
    int steps;

    pc = object->main_vm.pc;
    for (steps = 0; steps < SCRIPT_RUN_STEP_MAX; steps++) {
        if (script_file_read_word(&object->main_vm.source->file, pc, &op) != SCRIPT_FILE_OK) {
            return 0;
        }
        if (op == 0x19 && pc != object->main_vm.pc) {
            object->main_vm.pc = pc;
            object->main_vm.wait_ticks = 0;
            object->main_vm.has_ended = 0;
            script_machine_note(machine, &object->main_vm, "stuck_moved_to_next_act", object->name);
            return 1;
        }
        if (op == 0x0 && steps > 0) {
            return 0;
        }
        if (!word_after_op(object->main_vm.source, pc, &pc)) {
            return 0;
        }
    }
    return 0;
}

static int move_to_next_act(struct script_machine *machine)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        if (machine->objects[index].handler_count > 0 && move_object_to_next_act(machine, &machine->objects[index])) {
            return 1;
        }
    }
    return 0;
}

static int names_a_script(const char *path)
{
    unsigned int length;

    if (path == NULL) {
        return 0;
    }
    length = (unsigned int)strlen(path);
    return length > strlen(XML_SUFFIX) && strcmp(&path[length - strlen(XML_SUFFIX)], XML_SUFFIX) == 0;
}

static const char *first_script_change_of(const struct script_source *source)
{
    unsigned int pc;
    unsigned int op;
    unsigned int string_word;
    const char *path;

    for (pc = 0; script_file_read_word(&source->file, pc, &op) == SCRIPT_FILE_OK; pc++) {
        if (op != SCRIPT_CHANGE_OP || script_file_read_word(&source->file, pc + 1, &string_word) != SCRIPT_FILE_OK) {
            continue;
        }
        path = script_file_string(&source->file, string_word);
        if (names_a_script(path)) {
            return path;
        }
    }
    return NULL;
}

static void begin_script_change(struct script_machine *machine, const char *path)
{
    machine->pause_bit = 1;
    machine->pending_script = path;
    machine->teardown_at = (long long)machine->tick + 1;
    machine->load_at = machine->teardown_at + LOAD_AFTER_TEARDOWN_TICKS;
}

static const char *exit_of_this_scene(const struct script_machine *machine)
{
    const char *path;

    path = machine->has_runner ? first_script_change_of(machine->runner.source) : NULL;
    if (path == NULL && machine->object_count > 0) {
        path = first_script_change_of(machine->objects[0].main_vm.source);
    }
    return path;
}

static void force_script_change(struct script_machine *machine, const char *path)
{
    machine->is_finished = 0;
    if (machine->outputs->scene_change_was_forced != NULL) {
        machine->outputs->scene_change_was_forced();
    }
    begin_script_change(machine, path);
}

void script_machine_reload_scene(struct script_machine *machine)
{
    if (machine->scene_name != NULL && find_source(machine, machine->scene_name) != NULL) {
        force_script_change(machine, machine->scene_name);
    }
}

static void move_to_next_script(struct script_machine *machine)
{
    const char *path;

    path = exit_of_this_scene(machine);
    if (path == NULL) {
        machine->is_finished = 1;
        script_machine_note(machine, NULL, "stuck_with_nowhere_left_to_go", "");
        return;
    }
    script_machine_note(machine, NULL, "stuck_moved_to_next_script", path);
    force_script_change(machine, path);
}

void script_machine_skip_ahead(struct script_machine *machine)
{
    machine->last_new_site_tick = machine->tick;
    if (machine->teardown_at != NEVER || machine->load_at != NEVER) {
        return;
    }
    machine->talk_close_at = (long long)machine->tick + 1;
    if (!move_to_next_act(machine)) {
        move_to_next_script(machine);
    }
}

static unsigned int stuck_after_any_ticks(const struct script_machine *machine)
{
    return machine->route != NULL && machine->auto_press == SCRIPT_AUTO_PACED ? STUCK_AFTER_ANY_TICKS_WHEN_A_ROUTE_DRIVES : STUCK_AFTER_ANY_TICKS;
}

static void push_when_stuck(struct script_machine *machine)
{
    unsigned int quiet_ticks;

    quiet_ticks = machine->tick - machine->last_new_site_tick;
    if (machine->teardown_at != NEVER || machine->load_at != NEVER) {
        return;
    }
    if (game_waits_on_a_press_that_works(machine)) {
        if (machine->auto_press == SCRIPT_AUTO_OFF) {
            machine->last_new_site_tick = machine->tick;
            machine->last_timer_tick = machine->tick;
            return;
        }
        machine->last_timer_tick = machine->tick;
        if (quiet_ticks < stuck_after_any_ticks(machine)) {
            return;
        }
    }
    if (something_is_running_to_a_known_end(machine)) {
        machine->last_timer_tick = machine->tick;
    }
    if (machine->auto_press != SCRIPT_AUTO_FAST && script_soul_world_is_playable(machine) && quiet_ticks < (machine->auto_press == SCRIPT_AUTO_PACED ? STUCK_AFTER_TICKS_WHEN_PLAYING_AT_A_HUMAN_PACE : STUCK_AFTER_TICKS_WHEN_THE_PLAYER_CAN_ACT)) {
        return;
    }
    if (quiet_ticks < stuck_after_any_ticks(machine) && (quiet_ticks < STUCK_AFTER_QUIET_TICKS || machine->tick - machine->last_timer_tick < STUCK_AFTER_QUIET_TICKS)) {
        return;
    }
    machine->last_new_site_tick = machine->tick;
    machine->last_timer_tick = machine->tick;
    machine->push_count = machine->push_count + 1;
    machine->pushes_in_this_script = machine->pushes_in_this_script + 1;
    if (machine->narration.is_running) {
        script_narration_finish_soon(&machine->narration);
        script_machine_note(machine, NULL, "stuck_ended_narration", "");
        return;
    }
    if (machine->talk.phase != SCRIPT_TALK_CLOSED) {
        script_talk_close_now(machine);
        script_machine_note(machine, NULL, "stuck_closed_talk", "");
        return;
    }
    if (machine->pushes_in_this_script <= STUCK_PUSHES_BEFORE_LEAVING_THE_SCRIPT && push_a_waiting_handler(machine)) {
        return;
    }
    if (machine->pushes_in_this_script <= STUCK_PUSHES_BEFORE_LEAVING_THE_SCRIPT && move_to_next_act(machine)) {
        return;
    }
    move_to_next_script(machine);
}

static const char *scene_where_the_story_should_be(const struct script_machine *machine)
{
    int scene;

    scene = machine->scene_name != NULL ? script_route_scene_index(machine->route, machine->scene_name) : -1;
    if (scene < 0 || scene + 1 >= machine->route->scene_count) {
        return NULL;
    }
    if (scene < machine->furthest_scene) {
        scene = machine->furthest_scene + 1 < machine->route->scene_count ? machine->furthest_scene : machine->furthest_scene - 1;
    }
    return machine->route->scenes[scene + 1];
}

static int scene_has_run_too_long(const struct script_machine *machine)
{
    if (machine->route == NULL || machine->route->scene_most_ticks <= 0 || machine->auto_press == SCRIPT_AUTO_OFF) {
        return 0;
    }
    if (machine->teardown_at != NEVER || machine->load_at != NEVER || machine->stop_is_requested) {
        return 0;
    }
    return machine->tick - machine->last_new_scene_tick >= (unsigned int)machine->route->scene_most_ticks;
}

static void move_on_when_the_story_went_back(struct script_machine *machine)
{
    const char *next;

    if (!machine->scene_is_behind_the_story || machine->teardown_at != NEVER || machine->load_at != NEVER || machine->stop_is_requested) {
        return;
    }
    machine->scene_is_behind_the_story = 0;
    next = machine->furthest_scene + 1 < machine->route->scene_count ? machine->route->scenes[machine->furthest_scene + 1] : NULL;
    if (next == NULL || find_source(machine, next) == NULL) {
        script_machine_note(machine, NULL, "scene_behind_the_story_found_no_way_on", machine->scene_name != NULL ? machine->scene_name : "");
        return;
    }
    if (machine->outputs->marked != NULL) {
        machine->outputs->marked("scene_behind_the_story_left", machine->scene_name != NULL ? machine->scene_name : "");
        machine->outputs->marked("scene_behind_the_story_moved_to", next);
    }
    machine->last_new_scene_tick = machine->tick;
    force_script_change(machine, next);
}

int script_machine_move_on_in_story_order(struct script_machine *machine, const char *why)
{
    char mark[SCRIPT_MOVE_ON_MARK_MAX];
    const char *next;

    machine->last_new_scene_tick = machine->tick;
    next = machine->route != NULL && machine->route->scene_count > 0 ? scene_where_the_story_should_be(machine) : NULL;
    if (next == NULL || find_source(machine, next) == NULL) {
        next = exit_of_this_scene(machine);
    }
    if (next == NULL) {
        snprintf(mark, sizeof(mark), "%s_found_no_way_on", why);
        script_machine_note(machine, NULL, mark, machine->scene_name != NULL ? machine->scene_name : "");
        return 0;
    }
    if (machine->outputs->marked != NULL) {
        snprintf(mark, sizeof(mark), "%s_left", why);
        machine->outputs->marked(mark, machine->scene_name != NULL ? machine->scene_name : "");
        snprintf(mark, sizeof(mark), "%s_moved_to", why);
        machine->outputs->marked(mark, next);
    }
    force_script_change(machine, next);
    return 1;
}

static void move_on_when_the_scene_runs_too_long(struct script_machine *machine)
{
    if (scene_has_run_too_long(machine)) {
        script_machine_move_on_in_story_order(machine, "scene_timeout");
    }
}

void script_machine_tick(struct script_machine *machine)
{
    int index;

    if (script_soul_step_eyecatch(machine)) {
        return;
    }
    move_on_when_the_story_went_back(machine);
    move_on_when_the_scene_runs_too_long(machine);
    if (machine->is_finished) {
        machine->tick = machine->tick + 1;
        return;
    }
    run_native_steps_before_tasks(machine);
    if (machine->is_finished) {
        return;
    }
    if (machine->has_runner) {
        run_vm(machine, &machine->runner);
    }
    for (index = 0; index < machine->object_count; index++) {
        update_object(machine, &machine->objects[index]);
    }
    script_motion_update_all(machine);
    script_fades_update(machine);
    script_machine_move_anchored_effects(machine);
    script_machine_update_camera(machine);
    step_narration(machine);
    script_talk_step(machine);
    script_balloons_report_opened(machine);
    run_gameapp_queue(machine);
    push_when_stuck(machine);
    if (machine->stop_is_requested) {
        machine->is_finished = 1;
    }
    machine->tick = machine->tick + 1;
}
