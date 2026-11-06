#ifndef SCRIPT_MACHINE_H
#define SCRIPT_MACHINE_H

#include "script_file.h"
#include "script_flags.h"
#include "script_narration.h"
#include "script_opcodes.h"
#include "script_outputs.h"

#define SCRIPT_SOURCE_MAX 16
#define SCRIPT_SOURCE_NAME_MAX 96
#define SCRIPT_OBJECT_MAX 48
#define SCRIPT_HANDLER_MAX 48
#define SCRIPT_MESSAGE_MAX 16
#define SCRIPT_GAMEAPP_MAX 32
#define SCRIPT_BALLOON_MAX 6
#define SCRIPT_FADE_MAX 8
#define SCRIPT_RECENT_FETCH_MAX 64
#define SCRIPT_RUN_STEP_MAX 4096
#define SCRIPT_ACTOR_POINT_MAX 4
#define SCRIPT_NO_SELECTION -1
#define SCRIPT_NOTED_OP_BYTES ((SCRIPT_OPCODE_COUNT / 8) + 1)
#define SCRIPT_NOTED_EFFECT_BYTES 128

enum script_vm_kind {
    SCRIPT_VM_RUNNER,
    SCRIPT_VM_MAIN,
    SCRIPT_VM_SCANNER,
    SCRIPT_VM_HANDLER
};

enum script_step_result {
    SCRIPT_STEP_NEXT,
    SCRIPT_STEP_NEXT_THEN_YIELD,
    SCRIPT_STEP_MOVED,
    SCRIPT_STEP_MOVED_THEN_YIELD
};

struct script_source {
    char name[SCRIPT_SOURCE_NAME_MAX];
    struct script_file file;
    const struct script_cue_list *cues;
};

struct script_object;

struct script_vm {
    enum script_vm_kind kind;
    const struct script_source *source;
    struct script_object *owner;
    unsigned int pc;
    unsigned int start_pc;
    unsigned int wait_ticks;
    int condition;
    int pause_class;
    int selected_object;
};

struct script_handler {
    struct script_vm vm;
    const char *verb;
    const char *arg;
    int is_busy;
};

struct script_message {
    const char *verb;
    const char *arg;
};

struct script_actor {
    int exists;
    int creation_index;
    int x;
    int y;
    int points[SCRIPT_ACTOR_POINT_MAX][2];
    int level;
    int is_visible;
    int has_queued_table;
    unsigned int queued_table_word;
    unsigned int queued_table_offset;
    int queued_keeps_point_zero;
    int has_table;
    unsigned int table_word;
    unsigned int table_offset;
    const struct script_source *table_source;
    int frame_word_index;
    int frame_hold_ticks;
    int ticks_left;
    int place_is_pending;
    int place_x;
    int place_y;
    int place_point;
    int shift_is_pending;
    int shift_x;
    int shift_y;
    int dock_is_pending;
    int dock_target;
    int dock_target_point;
    int dock_own_point;
    struct script_actor_view last_view;
    int has_last_view;
};

struct script_object {
    const char *name;
    struct script_vm main_vm;
    struct script_vm scanner;
    struct script_handler handlers[SCRIPT_HANDLER_MAX];
    int handler_count;
    int idle_handler;
    int act_was_requested;
    struct script_message messages[SCRIPT_MESSAGE_MAX];
    int message_count;
    struct script_actor actor;
};

struct script_fade {
    int kind;
    int level;
    int step;
};

struct script_balloon {
    int in_use;
    int owner_object;
    const char *label;
    int kind;
    int timer;
    float offset_x;
    float offset_y;
    int open_is_pending;
};

struct script_halt {
    int is_halted;
    const char *reason;
    const char *detail;
    unsigned int op;
    unsigned int offset;
    unsigned int tick;
    const char *file_name;
    const char *object_name;
};

struct script_recent_fetch {
    unsigned int tick;
    unsigned int offset;
    unsigned int op;
    const char *object_name;
};

struct script_machine {
    const struct script_outputs *outputs;
    enum script_timing timing;
    const char *stop_act;
    struct script_source sources[SCRIPT_SOURCE_MAX];
    int source_count;
    unsigned int tick;
    int is_finished;
    int stop_is_requested;
    struct script_halt halt;
    struct script_recent_fetch recent_fetches[SCRIPT_RECENT_FETCH_MAX];
    unsigned int fetch_total;
    struct script_flags flags;
    int pause_bit;
    int mode_byte;
    int talk_is_open;
    int has_runner;
    struct script_vm runner;
    struct script_object objects[SCRIPT_OBJECT_MAX];
    int object_count;
    int actor_count;
    const char *gameapp_verbs[SCRIPT_GAMEAPP_MAX];
    const char *gameapp_args[SCRIPT_GAMEAPP_MAX];
    int gameapp_count;
    const char *pending_script;
    long long teardown_at;
    long long load_at;
    long long game_start_at;
    long long pause_clear_at;
    long long soul_vision_message_at;
    struct script_narration narration;
    int narration_started_this_tick;
    struct script_fade fades[SCRIPT_FADE_MAX];
    int fade_count;
    int brightness;
    int reported_brightness;
    int mask_fade_is_alive;
    int mask_fade_alpha;
    int mask_fade_fixed;
    int mask_fade_frames;
    int mask_fade_target;
    int mask_fade_step;
    int reported_mask_fade_is_alive;
    int reported_mask_fade_alpha;
    struct script_balloon balloons[SCRIPT_BALLOON_MAX];
    int music_id;
    int camera_x;
    int camera_y;
    unsigned char noted_ops[SCRIPT_NOTED_OP_BYTES];
    unsigned char noted_effects[SCRIPT_NOTED_EFFECT_BYTES];
};

void script_machine_init(struct script_machine *machine, const struct script_outputs *outputs, enum script_timing timing, const char *stop_act);
int script_machine_add_source(struct script_machine *machine, const char *name, const unsigned char *data, unsigned int size, const struct script_cue_list *cues);
int script_machine_start(struct script_machine *machine, const char *name);
void script_machine_tick(struct script_machine *machine);
void script_machine_name_of_path(const char *path, char *name, unsigned int name_size);

void script_machine_halt(struct script_machine *machine, const struct script_vm *vm, const char *reason, const char *detail);
const char *script_machine_string(struct script_machine *machine, const struct script_vm *vm, unsigned int string_offset);
struct script_object *script_machine_find_object(struct script_machine *machine, const char *name);
struct script_object *script_machine_current_object(struct script_machine *machine, const struct script_vm *vm);
void script_machine_send(struct script_machine *machine, const char *target, const char *verb, const char *arg);
unsigned int script_machine_u32(const unsigned int *words, int first);

enum script_step_result script_ops_actor_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);
enum script_step_result script_ops_view_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);
enum script_step_result script_ops_media_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);

void script_motion_update_all(struct script_machine *machine);
void script_fades_update(struct script_machine *machine);
void script_fades_add(struct script_machine *machine, const struct script_vm *vm, int kind, int frames);
void script_fades_set_mask(struct script_machine *machine, int target_alpha, int frames);
void script_balloons_count_down(struct script_machine *machine);
void script_balloons_report_opened(struct script_machine *machine);
void script_narration_report_to_outputs(void *listener, const char *label, enum script_narration_event event, int group);

#endif
