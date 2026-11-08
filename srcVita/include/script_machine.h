#ifndef SCRIPT_MACHINE_H
#define SCRIPT_MACHINE_H

#include "script_choice.h"
#include "script_file.h"
#include "script_flags.h"
#include "script_narration.h"
#include "script_opcodes.h"
#include "script_outputs.h"
#include "script_route.h"
#include "script_talk.h"
#include "soul_chain.h"

#define SCRIPT_NO_OBJECT (-1)
#define SCRIPT_COLOR_LEVEL_NORMAL 256
#define SCRIPT_NOTE_LIST_COUNT 3
#define SCRIPT_NOTE_ID_COUNT 64
#define SCRIPT_SOURCE_MAX 256
#define SCRIPT_SOURCE_NAME_MAX 96
#define SCRIPT_OBJECT_MAX 128
#define SCRIPT_HANDLER_MAX 96
#define SCRIPT_MESSAGE_MAX 16
#define SCRIPT_GAMEAPP_MAX 32
#define SCRIPT_BALLOON_MAX 8
#define SCRIPT_TAP_BALLOON_SLOTS 2
#define SCRIPT_NO_TAP_GROUP 0
#define SCRIPT_BALLOON_ON_OWNER 0
#define SCRIPT_BALLOON_AT_WORLD_POINT 1
#define SCRIPT_BALLOON_AT_SCREEN_POINT 2
#define SCRIPT_FADE_MAX 8
#define SCRIPT_RECENT_FETCH_MAX 64
#define SCRIPT_RUN_STEP_MAX 4096
#define SCRIPT_ACTOR_POINT_MAX 4
#define SCRIPT_ACTOR_COUNTER_COUNT 5
#define SCRIPT_NO_SELECTION -1
#define SCRIPT_NOTED_OP_BYTES ((SCRIPT_OPCODE_COUNT / 8) + 1)
#define SCRIPT_NOTED_EFFECT_BYTES 128
#define SCRIPT_NOTED_PROBLEM_BYTES 1024
#define SCRIPT_VISITED_SITE_BYTES 8192
#define SCRIPT_PUSHED_SITE_BYTES 1024
#define SCRIPT_CALL_DEPTH_MAX 4
#define SCRIPT_ROOM_ID_MAX 256
#define SCRIPT_CAMERA_BAND_MAX 4
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
    unsigned char counters[SCRIPT_ACTOR_COUNTER_COUNT];
    int level;
    int is_visible;
    int color_level;
    int is_flipped;
    int stepping_is_stopped;
    int is_frozen_and_hidden;
    int is_hard_hidden;
    int disabled_cores;
    int busy_cores;
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
    int counts_loops;
    int loops_left;
    int motion_is_done;
    unsigned int class_id;
    int has_core;
    int core_x;
    int core_y;
    int has_second_core;
    int second_core_x;
    int second_core_y;
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
    const char *current_act;
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
    int placement;
    int open_is_pending;
    int tap_group;
    const char *tap_target;
    const char *tap_verb;
    const char *tap_arg;
};

struct script_room_bounds {
    int width;
    int height;
    int band_keys[SCRIPT_CAMERA_BAND_MAX];
    int band_values[SCRIPT_CAMERA_BAND_MAX];
    int band_count;
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

#define SCRIPT_ANCHORED_EFFECT_MAX 12

struct script_anchored_effect {
    const char *name;
    int object_index;
    int node;
    int aim_node;
    float offset_x;
    float offset_y;
};

struct script_pill {
    int state;
    int step;
    int press_ticks;
    int hold_ticks;
};

struct script_soul {
    struct script_pill pills[SCRIPT_PILL_COUNT];
    unsigned int pills_registered;
    unsigned int pills_shown_by_effect;
    const char *pill_effect_names[SCRIPT_PILL_COUNT];
    int pill_slide_in_wait;
    unsigned int pill_pulse_ticks;
    int ghost_pill_is_held_out;
    int trick_pill_is_held_out;
    int touch_is_down;
    float touch_x;
    float touch_y;
    float camera_drag_x;
    float camera_drag_y;
    int free_camera_is_forbidden;
    int leaving_shows_the_mark;
    float camera_follow_x;
    float camera_carry_x;
    float camera_carry_y;
    int camera_was_saved;
    int saved_camera_core;
    int saved_camera_x;
    int saved_camera_y;
    int rollback_is_on;
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
    float held_x;
    float held_y;
    long long start_message_at;
    long long final_message_at;
    long long leap_spokes_at;
    long long leap_swirl_at;
    long long leap_message_at;
    int view_change_ticks_left;
    int is_leaving_view;
    int next_leaving_is_quiet;
    unsigned int auto_reading_ticks;
    int tip_is_out;
    float tip_x;
    float tip_y;
    int route_step;
    const char *route_script;
    unsigned int route_ready_at;
    unsigned int route_step_began_at;
    int route_action_has_begun;
    int route_acted_step;
    unsigned int route_action_began_at;
    unsigned int route_last_action_tick;
    int route_camera_is_scrolling;
    int tricks_fired;
    int route_tricks_when_step_began;
    int route_aimed_node;
    int tip_is_aimed;
    struct soul_chain chain;
    int chain_needs_reset;
    int chain_pick_object;
    int chain_pick_slot;
    int touch_was_down;
    int touch_drags_the_tip;
    float pen_x;
    float pen_y;
    int tip_input_is_held;
    float tip_aim_x;
    float tip_aim_y;
    int mark_age_ticks;
    int rewind_select_ticks;
    int rewind_press_ticks;
    int flash_ticks_left;
    int sparks_elapsed_ticks;
    int dead_light_ticks_left;
    int drag_ticks_left;
    int drag_stream_ticks;
    int drag_object;
    int drag_slot;
    float drag_from_x;
    float drag_from_y;
    int camera_follow_is_off;
    int view_demo_is_on;
    int view_follow_is_off;
    int view_back_is_offered;
    int action_is_due_in;
    int action_object;
    int action_slot;
    int countdown_is_running;
    int countdown_step;
    int countdown_ticks;
    int dead_talk_is_on;
    int dead_talk_object;
    struct script_timed_event phone_event;
    char phone_split_name[SCRIPT_SOURCE_NAME_MAX];
    struct script_timed_event time_jump_event;
};

#define SCRIPT_SCENES_SEEN_MAX 512
#define COUNTDOWN_KEPT_ACROSS_A_SCRIPT_CHANGE_FLAG 0x1008u

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
    int film_frame_is_on;
    int talk_is_open;
    struct script_choice choice;
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
    int eyecatch_marks_the_core;
    int press_is_pending;
    struct script_soul soul;
    const char *active_core_name;
    struct script_narration narration;
    int narration_started_this_tick;
    struct script_fade fades[SCRIPT_FADE_MAX];
    int fade_count;
    int brightness;
    int background_color_level;
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
    int camera_band_keys[SCRIPT_CAMERA_BAND_MAX];
    int camera_band_values[SCRIPT_CAMERA_BAND_MAX];
    int camera_snap_is_off;
    int camera_follow_object;
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
    int shake_offset_x;
    int shake_offset_y;
    unsigned int shake_random;
    int chapter_mark;
    unsigned char note_levels[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];
    int note_was_added;
    int time_jump_movie_ticks;
    int movie_ticks_left;
    int phone_view_is_on;
    int phone_book_place;
    int game_over_is_pending;
    unsigned int game_over_talk_ended_at;
    struct script_anchored_effect anchored_effects[SCRIPT_ANCHORED_EFFECT_MAX];
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
    int fate_level;
    int fate_previous_level;
    int eyecatch_ticks_left;
    int mode_before_eyecatch;
    int countdown_interval;
    const struct script_route *route;
    const char *scene_name;
    unsigned int scene_hashes_seen[SCRIPT_SCENES_SEEN_MAX];
    int scenes_seen;
    int furthest_scene;
    int route_furthest_acted_step;
    int route_step_after_retry;
    int game_over_retry_step;
    int game_over_retries_at_that_step;
    int scene_is_behind_the_story;
    unsigned int last_new_scene_tick;
    int demo_soul_is_on;
    unsigned int random_state;
};

void script_machine_init(struct script_machine *machine, const struct script_outputs *outputs, enum script_timing timing, const char *stop_act);
int script_machine_add_source(struct script_machine *machine, const char *name, unsigned char *data, unsigned int size, const struct script_cue_list *cues);
int script_machine_start(struct script_machine *machine, const char *name);
void script_machine_tick(struct script_machine *machine);
void script_machine_name_of_path(const char *path, char *name, unsigned int name_size);

void script_machine_pan_camera_back_to(struct script_machine *machine, int x, int y);
#define SCRIPT_FIRST_CORE_NODE 10

void script_machine_node_position(const struct script_object *object, int node, float *x, float *y);
void script_machine_move_anchored_effects(struct script_machine *machine);
void script_machine_spawn_table_effect(struct script_machine *machine, struct script_object *object, int effect_id, float offset_x, float offset_y, const char *name);
int script_machine_camera_band_snap(const struct script_machine *machine, int object_y);
void script_machine_note(struct script_machine *machine, const struct script_vm *vm, const char *reason, const char *detail);
void script_machine_end_block(struct script_machine *machine, struct script_vm *vm, const char *reason);
void script_machine_skip_ahead(struct script_machine *machine);
void script_machine_update_camera(struct script_machine *machine);
const char *script_machine_string(struct script_machine *machine, const struct script_vm *vm, unsigned int string_offset);
int script_machine_index_of_object(const struct script_machine *machine, const char *name);
int script_machine_name_matches_target(const char *name, const char *target);
struct script_object *script_machine_find_object(struct script_machine *machine, const char *name);
struct script_object *script_machine_current_object(struct script_machine *machine, const struct script_vm *vm);
void script_machine_send(struct script_machine *machine, const char *target, const char *verb, const char *arg);
int script_machine_handles(const struct script_machine *machine, const char *verb, const char *arg);
void script_machine_start_music(struct script_machine *machine, int id);
void script_machine_stop_music(struct script_machine *machine, int fade_ticks);
unsigned int script_machine_u32(const unsigned int *words, int first);

enum script_step_result script_ops_actor_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);
enum script_step_result script_ops_view_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);
enum script_step_result script_ops_media_run(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);
enum script_step_result script_choice_run_op(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words);

void script_motion_update_all(struct script_machine *machine);
void script_fades_update(struct script_machine *machine);
void script_fades_add(struct script_machine *machine, const struct script_vm *vm, int kind, int frames);
void script_fades_set_mask(struct script_machine *machine, int target_alpha, int frames);
void script_fades_end_mask(struct script_machine *machine);
void script_balloons_count_down(struct script_machine *machine);
void script_balloons_report_opened(struct script_machine *machine);
void script_balloons_tap(struct script_machine *machine, int tap_group);
int script_balloons_can_be_tapped(const struct script_machine *machine, int tap_group);
void script_machine_note_press(struct script_machine *machine);
void script_machine_reload_scene(struct script_machine *machine);
int script_machine_move_on_in_story_order(struct script_machine *machine, const char *why);
void script_machine_scroll_camera(struct script_machine *machine, int delta_x, int delta_y);
void script_machine_set_world_origin(struct script_machine *machine, int x, int y);
void script_soul_run_command(struct script_machine *machine, const char *verb, const char *arg);
int script_soul_knows_command(const char *verb);
void script_soul_step_before_tasks(struct script_machine *machine);
void script_pills_reset(struct script_machine *machine);
void script_pills_step(struct script_machine *machine);
void script_pills_press(struct script_machine *machine, enum script_pill_id id, int ticks_after_press);
int script_pills_is_up(const struct script_machine *machine, enum script_pill_id id);
void script_pills_register(struct script_machine *machine, enum script_pill_id id, int is_on);
void script_pills_register_all(struct script_machine *machine);
void script_pills_unregister_all(struct script_machine *machine);
void script_pills_hide_at_once(struct script_machine *machine);
void script_pills_effect_spawned(struct script_machine *machine, int effect_id, const char *name);
void script_pills_effect_deleted(struct script_machine *machine, const char *name);
void script_pills_fill_view(const struct script_machine *machine, struct script_hud_view *view);
void script_soul_start_player_mark(struct script_machine *machine);
int script_soul_wants_a_tap(const struct script_machine *machine);
void script_soul_set_touch(struct script_machine *machine, int is_down, float x, float y);
void script_soul_drag_camera(struct script_machine *machine, float delta_x, float delta_y);
void script_soul_note_ghost(struct script_machine *machine);
void script_soul_note_pill(struct script_machine *machine, enum script_pill_id id);
void script_soul_note_tap(struct script_machine *machine, int tap_group);
void script_soul_note_direction(struct script_machine *machine, int direction_x, int direction_y);
void script_soul_set_held(struct script_machine *machine, float held_x, float held_y);
void script_soul_reset(struct script_machine *machine);
void script_soul_script_began(struct script_machine *machine, const char *script);
void script_soul_act_began(struct script_machine *machine, const char *object_name, const char *act_name);
void script_soul_set_first_core(struct script_machine *machine, int object_index, int slot);
void script_soul_set_countdown(struct script_machine *machine, int first_tick_count);
void script_soul_set_countdown_interval(struct script_machine *machine, int interval);
void script_soul_set_fate_level(struct script_machine *machine, int level);
int script_soul_step_eyecatch(struct script_machine *machine);
void script_soul_effect_spawned(struct script_machine *machine, int effect_id, const char *name);
void script_soul_effect_deleted(struct script_machine *machine, const char *name);
void script_soul_effect_signalled(struct script_machine *machine, const char *name);
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
