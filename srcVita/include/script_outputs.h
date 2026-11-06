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

struct script_outputs {
    void (*fetched)(const struct script_fetch *fetch);
    void (*stage_cleared)(void);
    void (*room_loaded)(int room, int camera_x, int camera_y);
    void (*camera_cut)(int x, int y);
    void (*camera_moved)(int x, int y);
    void (*shake_changed)(int level, int ticks);
    void (*screen_filter_changed)(int mode);
    void (*mask_fade_color_changed)(int is_white);
    void (*actor_changed)(const struct script_actor_view *actor);
    void (*brightness_changed)(int signed_level);
    void (*mask_fade_changed)(int is_alive, int alpha);
    void (*beam_created)(const char *name, float apex_x, float apex_y, float base_x, float base_y, float half_width);
    void (*beam_width_ramped)(const char *name, float half_width, int ticks);
    void (*beam_base_ramped)(const char *name, float base_x, float base_y, int ticks);
    void (*beam_deleted)(const char *name);
    void (*iris_created)(const char *name, float x, float y, float radius);
    void (*iris_radius_ramped)(const char *name, float radius, int ticks);
    void (*iris_deleted)(const char *name);
    void (*effect_spawned)(int id, const char *name, float x, float y);
    void (*narration_changed)(const char *label, enum script_narration_event event, int group);
    void (*balloon_opened)(int slot, const char *label, int kind, float anchor_x, float anchor_y);
    void (*balloon_closed)(int slot);
    void (*music_prepared)(int id);
    void (*music_started)(int id);
    void (*music_stopped)(int fade_ticks);
    void (*sound_played)(int kind);
    void (*sounds_stopped)(void);
    void (*sound_loop_started)(int kind, const char *loop_tag);
    void (*sound_loop_stopped)(const char *loop_tag);
    void (*music_volume_changed)(int volume, int ticks);
    void (*soul_vision_changed)(int is_on);
    void (*soul_cursor_moved)(int is_shown, float x, float y);
    void (*marked)(const char *what, const char *name);
    void (*talk_changed)(const struct script_talk_view *view);
    void (*effect_deleted)(const char *name);
};

#endif
