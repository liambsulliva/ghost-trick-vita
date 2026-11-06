#ifndef SCRIPT_MACHINE_H
#define SCRIPT_MACHINE_H

#include "script_file.h"
#include "script_flags.h"
#include "script_narration.h"
#include "script_opcodes.h"
#include "script_outputs.h"
#include "script_talk.h"

#define SCRIPT_SOURCE_MAX 32
#define SCRIPT_SOURCE_NAME_MAX 96
#define SCRIPT_OBJECT_MAX 64
#define SCRIPT_HANDLER_MAX 64
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
#define SCRIPT_NOTED_PROBLEM_BYTES 1024
#define SCRIPT_VISITED_SITE_BYTES 8192
#define SCRIPT_PUSHED_SITE_BYTES 1024
#define SCRIPT_CALL_DEPTH_MAX 4
#define SCRIPT_ROOM_ID_MAX 256
#define SCRIPT_CORE_SLOT_COUNT 2
#define SCRIPT_TAP_GROUP_LEFT 1
#define SCRIPT_TAP_GROUP_RIGHT 2

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
    int has_ended;
    int exit_is_requested;
    int reg_a;
    int call_depth;
    unsigned int call_stack[SCRIPT_CALL_DEPTH_MAX];
};

struct script_handler {
    struct script_vm vm;
    const char *verb;
    const char *arg;
    int is_busy;
    unsigned int first_pc;
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
    int is_flipped;
    int stepping_is_stopped;
    int is_frozen_and_hidden;
    int is_hard_hidden;
    int disabled_cores;
    int loop_carry_x;
    int loop_carry_y;
    int is_after_loop;
    int has_queued_table;
    unsigned int queued_table_word;
    unsigned int queued_table_offset;
    int queued_keeps_point_zero;
    int has_table;
    unsigned int table_word;
    unsigned int table_start_word;
    unsigned int table_offset;
    const struct script_source *table_source;
    int frame_word_index;
    int frame_hold_ticks;
    int ticks_left;
    unsigned int class_id;
    int has_core;
    int core_x;
    int core_y;
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
    int keeps_following;
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
    int is_sleeping;
    int is_dead;
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
    int tap_group;
    const char *tap_target;
    const char *tap_verb;
    const char *tap_arg;
};

struct script_room_bounds {
    int width;
    int height;
};

struct script_recent_fetch {
    unsigned int tick;
    unsigned int offset;
    unsigned int op;
    const char *object_name;
};

enum script_auto_play {
    SCRIPT_AUTO_OFF,
    SCRIPT_AUTO_FAST,
    SCRIPT_AUTO_PACED
};

struct script_timed_event {
    const char *target;
    const char *verb;
    const char *arg;
    long long at;
};

struct script_soul {
    int is_on;
    int player_opened_soul_view;
    int active_object;
    int active_slot;
    int trick_is_on;
    int input_is_off;
    int camera_is_locked;
    int camera_tutorial_is_on;
    int tutorial_mode;
    int tutorial_click_was_sent;
    int ghost_is_pending;
    int tap_group_is_pending;
    int direction_x;
    int direction_y;
    int held_x;
    int held_y;
    long long start_message_at;
    unsigned int next_auto_try_tick;
    unsigned int auto_reading_ticks;
    int action_is_due_in;
    int action_object;
    int action_slot;
    int countdown_is_running;
    int countdown_step;
    int countdown_ticks;
    struct script_timed_event phone_event;
    struct script_timed_event time_jump_event;
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
    long long soul_sound_at;
    long long talk_close_at;
    struct script_talk talk;
    const struct script_talk_book *talk_book;
    int soul_state_keeps_pause;
    int press_is_pending;
    struct script_soul soul;
    const char *active_core_name;
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
    int world_origin_x;
    int world_origin_y;
    int room;
    struct script_room_bounds room_bounds[SCRIPT_ROOM_ID_MAX];
    int pan_is_active;
    int pan_ticks_left;
    int pan_x_fixed;
    int pan_y_fixed;
    int pan_step_x_fixed;
    int pan_step_y_fixed;
    int pan_target_x;
    int pan_target_y;
    int pan_sends_hosei_end;
    int shake_level;
    int shake_ticks_left;
    int chapter_mark;
    unsigned char noted_ops[SCRIPT_NOTED_OP_BYTES];
    unsigned char noted_effects[SCRIPT_NOTED_EFFECT_BYTES];
    unsigned char noted_problems[SCRIPT_NOTED_PROBLEM_BYTES];
    unsigned char visited_sites[SCRIPT_VISITED_SITE_BYTES];
    unsigned char pushed_sites[SCRIPT_PUSHED_SITE_BYTES];
    int pushes_in_this_script;
    unsigned int last_new_site_tick;
    unsigned int last_timer_tick;
    unsigned int push_count;
    int auto_press;
    int demo_soul_is_on;
    unsigned int random_state;
};

void script_machine_init(struct script_machine *machine, const struct script_outputs *outputs, enum script_timing timing, const char *stop_act);
int script_machine_add_source(struct script_machine *machine, const char *name, const unsigned char *data, unsigned int size, const struct script_cue_list *cues);
int script_machine_start(struct script_machine *machine, const char *name);
void script_machine_tick(struct script_machine *machine);
void script_machine_name_of_path(const char *path, char *name, unsigned int name_size);

void script_machine_note(struct script_machine *machine, const struct script_vm *vm, const char *reason, const char *detail);
void script_machine_end_block(struct script_machine *machine, struct script_vm *vm, const char *reason);
void script_machine_skip_ahead(struct script_machine *machine);
void script_machine_update_camera(struct script_machine *machine);
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
void script_balloons_tap(struct script_machine *machine, int tap_group);
int script_balloons_can_be_tapped(const struct script_machine *machine, int tap_group);
void script_machine_note_press(struct script_machine *machine);
void script_machine_scroll_camera(struct script_machine *machine, int delta_x, int delta_y);
void script_soul_run_command(struct script_machine *machine, const char *verb, const char *arg);
void script_soul_step_before_tasks(struct script_machine *machine);
void script_soul_note_ghost(struct script_machine *machine);
void script_soul_note_tap(struct script_machine *machine, int tap_group);
void script_soul_note_direction(struct script_machine *machine, int direction_x, int direction_y);
void script_soul_set_held(struct script_machine *machine, int held_x, int held_y);
void script_soul_reset(struct script_machine *machine);
void script_soul_set_first_core(struct script_machine *machine, int object_index);
void script_soul_set_countdown(struct script_machine *machine, int is_on);
void script_soul_effect_spawned(struct script_machine *machine, int effect_id, const char *name);
void script_soul_effect_signalled(struct script_machine *machine, const char *name);
void script_soul_report_cursor(struct script_machine *machine);
int script_soul_waits_on_the_player(const struct script_machine *machine);
int script_soul_world_is_playable(const struct script_machine *machine);
void script_talk_open(struct script_machine *machine, const char *label);
void script_talk_step(struct script_machine *machine);
void script_talk_note_press(struct script_machine *machine);
void script_talk_note_forward(struct script_machine *machine);
void script_talk_close_when_due(struct script_machine *machine);
int script_talk_wants_a_press(const struct script_machine *machine);
void script_talk_close_now(struct script_machine *machine);
void script_narration_report_to_outputs(void *listener, const char *label, enum script_narration_event event, int group);

#endif
