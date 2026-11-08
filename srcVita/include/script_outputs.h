#ifndef SCRIPT_OUTPUTS_H
#define SCRIPT_OUTPUTS_H

#include "script_narration.h"
#include "script_talk.h"

#define SCRIPT_OPERAND_MAX 18

struct script_fetch {
    unsigned int tick;
    const char *vm_kind;
    const char *file_name;
    const char *object_name;
    unsigned int offset;
    unsigned int op;
    unsigned int words[SCRIPT_OPERAND_MAX];
    int word_count;
    int condition;
};

struct script_actor_view {
    const char *name;
    const char *file_name;
    unsigned int table_offset;
    int frame_word_index;
    int ticks_into_frame;
    int x;
    int y;
    int level;
    int creation_index;
    int is_visible;
    int is_flipped;
    unsigned int class_id;
    int has_core;
    int core_x;
    int core_y;
};

enum script_soul_motion {
    SCRIPT_SOUL_RESTING,
    SCRIPT_SOUL_BEAM,
    SCRIPT_SOUL_STREAM,
    SCRIPT_SOUL_LIGHTNING
};

#define SCRIPT_BALLOON_KIND_TAP_MARKS 100
#define SCRIPT_BALLOON_KIND_TAP_DOTS 101

enum script_pill_id {
    SCRIPT_PILL_GHOST,
    SCRIPT_PILL_HOURGLASS,
    SCRIPT_PILL_BACK,
    SCRIPT_PILL_NOTEBOOK,
    SCRIPT_PILL_TRICK,
    SCRIPT_PILL_COUNT
};

struct script_pill_view {
    int is_on_screen;
    int slide_step;
    int is_pressed;
};

struct script_hud_view {
    int side_is_on;
    int fate_level;
    int world_is_running;
    int is_hidden_for_movie;
    int narration_arrow_is_on;
    int soul_view_is_on;
    int rewind_select_ticks;
    int rewind_press_ticks;
    int pulse_ticks;
    struct script_pill_view pills[SCRIPT_PILL_COUNT];
};

struct script_outputs {
    void (*fetched)(const struct script_fetch *fetch);
    void (*stage_cleared)(void);
    void (*scene_change_was_forced)(void);
    void (*room_loaded)(int room, int camera_x, int camera_y);
    void (*camera_cut)(int x, int y);
    void (*camera_moved)(int x, int y);
    void (*world_origin_changed)(int x, int y);
    void (*ui_clip_started)(const char *name, int level_from, int level_to);
    void (*hud_changed)(const struct script_hud_view *view);
    void (*screen_filter_changed)(int mode);
    void (*mask_fade_color_changed)(int is_white);
    void (*actor_changed)(const struct script_actor_view *actor);
    void (*brightness_changed)(int signed_level);
    void (*mask_fade_changed)(int is_alive, int alpha);
    void (*beam_created)(const char *name, float apex_x, float apex_y, float base_x, float base_y, float half_width, float cover_alpha);
    void (*beam_width_ramped)(const char *name, float half_width, int ticks);
    void (*beam_cover_ramped)(const char *name, float cover_alpha, int ticks);
    void (*beam_base_ramped)(const char *name, float base_x, float base_y, int ticks);
    void (*beam_deleted)(const char *name);
    void (*iris_created)(const char *name, float x, float y, float radius, float alpha);
    void (*iris_radius_ramped)(const char *name, float radius, int ticks);
    void (*iris_alpha_ramped)(const char *name, float alpha, int ticks);
    void (*iris_moved)(const char *name, float x, float y, int ticks);
    void (*iris_deleted)(const char *name);
    void (*effect_spawned)(int id, const char *name, float x, float y);
    void (*narration_changed)(const char *label, enum script_narration_event event, int group);
    void (*balloon_opened)(int slot, const char *label, int kind, float anchor_x, float anchor_y);
    void (*balloon_closed)(int slot);
    void (*balloon_shaken)(int slot, int strength, int ticks);
    void (*music_prepared)(int id);
    void (*music_started)(int id);
    void (*music_faded_in)(int id, int fade_in_ticks);
    void (*music_stopped)(int fade_ticks);
    void (*sound_played)(int kind);
    void (*sounds_stopped)(void);
    void (*sound_loop_started)(int kind, const char *loop_tag);
    void (*sound_loop_stopped)(const char *loop_tag);
    void (*music_volume_changed)(int volume, int ticks);
    void (*soul_vision_changed)(int is_on, int with_effect);
    void (*phone_view_ended)(void);
    void (*soul_chain_moved)(const float *x, const float *y, int is_out);
    void (*soul_cursor_moved)(int is_shown, float x, float y, float from_x, float from_y, int motion);
    void (*soul_core_marked)(int object_index, int slot, float x, float y, int is_dead);
    void (*soul_mark_changed)(const char *object_name, int age_ticks, float x, float y);
    void (*soul_object_styled)(const char *object_name, unsigned int class_id, int has_core, int is_active, int is_dead, int is_phone_view);
    void (*soul_flash_changed)(float alpha);
    void (*film_frame_changed)(int is_on);
    void (*film_scroll_paused)(int is_paused);
    void (*screen_still_changed)(int is_on);
    void (*swirl_changed)(int is_on);
    void (*movie_started)(const char *name);
    int (*movie_of_class_started)(unsigned int class_id);
    void (*soul_sparks_changed)(int elapsed_ticks, float x, float y);
    void (*marked)(const char *what, const char *name);
    void (*talk_changed)(const struct script_talk_view *view);
    void (*effect_deleted)(const char *name);
    void (*effect_signalled)(const char *name);
    void (*effect_moved)(const char *name, float x, float y, float aim_degrees);
};

#endif
