#include <stdio.h>
#include <string.h>
#include "script_machine.h"
#include "script_opcodes.h"

#define MODE_BYTE_LOADING 1
#define MODE_BYTE_NORMAL 2
#define MODE_BYTE_ALSO_RUNS_IDLE 3
#define GAME_START_AFTER_LOAD_TICKS 3
#define LOAD_AFTER_TEARDOWN_TICKS 4
#define SOUL_VISION_MESSAGE_AFTER_TICKS 90
#define NARRATION_OVERHEAD_TICKS 3
#define NEVER -1
#define PAUSE_CLASS_NORMAL 0
#define PAUSE_CLASS_ONE 1
#define PAUSE_CLASS_TALK 2
#define PAUSE_CLASS_FREE 3
#define GAME_APP_TARGET "GameApp"
#define XML_SUFFIX ".xml"

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
    machine->game_start_at = NEVER;
    machine->pause_clear_at = NEVER;
    machine->soul_vision_message_at = NEVER;
    script_flags_clear_all(&machine->flags);
}

int script_machine_add_source(struct script_machine *machine, const char *name, const unsigned char *data, unsigned int size, const struct script_cue_list *cues)
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

static void begin_script(struct script_machine *machine, const struct script_source *source)
{
    begin_vm(&machine->runner, SCRIPT_VM_RUNNER, source, NULL, 0);
    machine->has_runner = 1;
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

void script_machine_halt(struct script_machine *machine, const struct script_vm *vm, const char *reason, const char *detail)
{
    struct script_halt *halt;

    halt = &machine->halt;
    if (halt->is_halted) {
        return;
    }
    halt->is_halted = 1;
    halt->reason = reason;
    halt->detail = detail;
    halt->tick = machine->tick;
    halt->op = 0;
    halt->offset = 0;
    halt->file_name = "";
    halt->object_name = "";
    if (vm != NULL) {
        unsigned int op;

        op = 0;
        script_file_read_word(&vm->source->file, vm->pc, &op);
        halt->op = op;
        halt->offset = vm->pc * SCRIPT_FILE_BYTES_PER_WORD + SCRIPT_FILE_HEADER_SIZE;
        halt->file_name = vm->source->name;
        halt->object_name = vm->owner != NULL ? vm->owner->name : "runner";
    }
}

const char *script_machine_string(struct script_machine *machine, const struct script_vm *vm, unsigned int string_offset)
{
    const char *text;

    text = script_file_string(&vm->source->file, string_offset);
    if (text == NULL) {
        script_machine_halt(machine, vm, "bad_string", "");
    }
    return text;
}

struct script_object *script_machine_find_object(struct script_machine *machine, const char *name)
{
    int index;

    for (index = 0; index < machine->object_count; index++) {
        if (strcmp(machine->objects[index].name, name) == 0) {
            return &machine->objects[index];
        }
    }
    return NULL;
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

static int gameapp_pair_is_known(const char *verb, const char *arg)
{
    if (strcmp(verb, "Button") == 0 && strcmp(arg, "All:Off") == 0) {
        return 1;
    }
    return strcmp(verb, "SoulVision") == 0 && strcmp(arg, "On") == 0;
}

static void queue_gameapp(struct script_machine *machine, const struct script_vm *vm, const char *verb, const char *arg)
{
    if (!gameapp_pair_is_known(verb, arg)) {
        script_machine_halt(machine, vm, "gameapp_not_ported", verb);
        return;
    }
    if (machine->gameapp_count >= SCRIPT_GAMEAPP_MAX) {
        script_machine_halt(machine, vm, "too_many_gameapp_commands", verb);
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
        script_machine_halt(machine, vm, "too_many_objects", "");
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
        script_machine_halt(machine, vm, "too_many_handlers", object->name);
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

    for (steps = 0; steps < SCRIPT_RUN_STEP_MAX; steps++) {
        if (script_file_read_word(&object->scanner.source->file, object->scanner.pc, &op) != SCRIPT_FILE_OK) {
            script_machine_halt(machine, &object->scanner, "word_past_code", "");
            return;
        }
        if (op == 0x112 || machine->halt.is_halted) {
            return;
        }
        step_vm(machine, &object->scanner);
    }
    script_machine_halt(machine, &object->scanner, "runaway", object->name);
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
    if (act_name != NULL && machine->stop_act != NULL && strcmp(act_name, machine->stop_act) == 0) {
        machine->stop_is_requested = 1;
    }
}

static int move_vm_to_offset(struct script_machine *machine, struct script_vm *vm, unsigned int file_offset)
{
    unsigned int word;

    if (file_offset == 0 || script_file_word_of_offset(&vm->source->file, file_offset, &word) != SCRIPT_FILE_OK) {
        script_machine_halt(machine, vm, "bad_jump", "");
        return 0;
    }
    vm->pc = word;
    return 1;
}

static void request_script_change(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    const char *path;

    path = script_machine_string(machine, vm, words[0]);
    if (path == NULL) {
        return;
    }
    machine->pause_bit = 1;
    machine->pending_script = path;
    machine->teardown_at = (long long)machine->tick + 1;
    machine->load_at = machine->teardown_at + LOAD_AFTER_TEARDOWN_TICKS;
}

static void read_flag_into_condition(struct script_machine *machine, struct script_vm *vm, const unsigned int *words)
{
    unsigned int flag;

    flag = script_machine_u32(words, 0);
    if (!script_flags_is_known(flag)) {
        script_machine_halt(machine, vm, "flag_out_of_range", "");
        return;
    }
    vm->condition = script_flags_read(&machine->flags, flag) ? 1 : 0;
}

static void write_flag(struct script_machine *machine, struct script_vm *vm, const unsigned int *words, int value)
{
    unsigned int flag;

    flag = script_machine_u32(words, 0);
    if (!script_flags_is_known(flag)) {
        script_machine_halt(machine, vm, "flag_out_of_range", "");
        return;
    }
    script_flags_write(&machine->flags, flag, value);
}

static void set_pause_class(struct script_vm *vm, int pause_class)
{
    if (vm->owner != NULL) {
        vm->owner->main_vm.pause_class = pause_class;
    } else {
        vm->pause_class = pause_class;
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
        script_machine_halt(machine, vm, "op_not_ported", "");
        return SCRIPT_STEP_MOVED_THEN_YIELD;
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

static int read_operands(struct script_machine *machine, const struct script_vm *vm, unsigned int length, unsigned int *words)
{
    unsigned int index;

    for (index = 1; index < length; index++) {
        if (script_file_read_word(&vm->source->file, vm->pc + index, &words[index - 1]) != SCRIPT_FILE_OK) {
            script_machine_halt(machine, vm, "word_past_code", "");
            return 0;
        }
    }
    return 1;
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
    case SCRIPT_OP_STORES_ONLY:
        note_stored_op_once(machine, op);
        return SCRIPT_STEP_NEXT;
    default:
        script_machine_halt(machine, vm, "op_not_ported", "");
        return SCRIPT_STEP_MOVED_THEN_YIELD;
    }
}

static int step_vm(struct script_machine *machine, struct script_vm *vm)
{
    unsigned int words[SCRIPT_OPERAND_MAX];
    unsigned int length;
    unsigned int op;

    if (machine->halt.is_halted) {
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
        script_machine_halt(machine, vm, "word_past_code", "");
        return 0;
    }
    length = script_opcode_length(op);
    if (length == 0) {
        script_machine_halt(machine, vm, "op_with_no_known_length", "");
        return 0;
    }
    if (!read_operands(machine, vm, length, words)) {
        return 0;
    }
    remember_fetch(machine, vm, op, words, length);
    switch (run_op(machine, vm, op, words)) {
    case SCRIPT_STEP_NEXT:
        vm->pc = vm->pc + length;
        return !machine->halt.is_halted;
    case SCRIPT_STEP_NEXT_THEN_YIELD:
        vm->pc = vm->pc + length;
        return 0;
    case SCRIPT_STEP_MOVED:
        return !machine->halt.is_halted;
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
    script_machine_halt(machine, vm, "runaway", "");
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
    if (vm_sits_on_end_op(&handler->vm)) {
        handler->vm.pc = handler->vm.start_pc;
        handler->vm.wait_ticks = 0;
        handler->is_busy = 0;
    }
}

static void deliver_queued_messages(struct script_object *object)
{
    int message;
    int handler;

    for (message = 0; message < object->message_count; message++) {
        for (handler = 0; handler < object->handler_count; handler++) {
            deliver(&object->handlers[handler], object->messages[message].verb, object->messages[message].arg);
        }
    }
    object->message_count = 0;
}

static void update_object(struct script_machine *machine, struct script_object *object)
{
    int any_was_busy;
    int count_at_start;
    int index;

    run_vm(machine, &object->main_vm);
    deliver_queued_messages(object);
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
    machine->has_runner = 0;
    machine->object_count = 0;
    machine->actor_count = 0;
    machine->gameapp_count = 0;
    machine->game_start_at = NEVER;
    memset(machine->balloons, 0, sizeof(machine->balloons));
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
        if (strcmp(machine->gameapp_verbs[index], "SoulVision") == 0) {
            machine->pause_bit = 1;
            machine->soul_vision_message_at = (long long)machine->tick + SOUL_VISION_MESSAGE_AFTER_TICKS;
            if (machine->outputs->soul_vision_changed != NULL) {
                machine->outputs->soul_vision_changed(1);
            }
        }
    }
    machine->gameapp_count = 0;
}

void script_machine_tick(struct script_machine *machine)
{
    int index;

    if (machine->halt.is_halted || machine->is_finished) {
        return;
    }
    run_native_steps_before_tasks(machine);
    if (machine->is_finished) {
        return;
    }
    if (machine->has_runner) {
        run_vm(machine, &machine->runner);
    }
    for (index = 0; index < machine->object_count && !machine->halt.is_halted; index++) {
        update_object(machine, &machine->objects[index]);
    }
    if (machine->halt.is_halted) {
        return;
    }
    script_motion_update_all(machine);
    script_fades_update(machine);
    step_narration(machine);
    script_balloons_report_opened(machine);
    run_gameapp_queue(machine);
    if (machine->stop_is_requested) {
        machine->is_finished = 1;
    }
    machine->tick = machine->tick + 1;
}
