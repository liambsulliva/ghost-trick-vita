#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "clock.h"
#include "music_engine.h"
#include "run_log.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_ui_view.h"
#include "script_bridge.h"
#include "script_player.h"
#include "script_view.h"
#include "sound_engine.h"

#define ALL_EFFECTS_NAME "*"
#define MASK_FADE_ALPHA_FULL 31.0f
#define CLASS_DRAWN_ONLY_IN_SOUL_VIEW 0x6309aa5au
#define CLASS_DRAWN_ONLY_IN_LIVING_VIEW 0x45427fc2u
#define BRIGHTNESS_SCALE 16.0f
#define DIM_LEVEL_WHEN_FULL (1.0f - (float)SCRIPT_NARRATION_DIM_TOP_LEVEL / (float)SCRIPT_NARRATION_DIM_SCALE)
#define CARD_FADE_IN_STARTS 22
#define CARD_FADE_IN_ENDS 31
#define CARD_FADE_OUT_STARTS 242
#define CARD_FADE_OUT_ENDS 273
#define PENDING_OVERLAY_MAX 16
#define OVERLAYS_PER_BALLOON 4
#define BALLOON_SHAKE_DS_PIXELS_PER_STRENGTH 2.0f
#define LAYER_PER_LEVEL 256
#define LEVEL_COUNT 15
#define CREATION_INDEX_TOP 255
#define DS_TICKS_PER_FRAME 4
#define LOGGED_ONCE_MAX 32
#define LOG_FOR_TOOLS_DISSOLVE "scene dissolve ticks=%d end_zoom=%.3f snapshot=%d snapshot_us=%llu"

struct pending_overlay {
    int in_use;
    unsigned int due_tick;
    int is_hide;
    int overlay_id;
    const struct script_balloon_part *part;
    float center_x;
    float center_y;
};

static const struct script_tables *tables;
static unsigned int tick;
#define PHONE_SPLIT_BASE_Y 128.0f

static int card_is_showing;
static unsigned int card_started_tick;
static int black_level;
static int white_level;
static int mask_fade_is_alive;
static int mask_fade_is_white;
static float mask_fade_alpha;
static int zoom_ticks_left;
#define NO_SOUL_TEXTURE "-"
#define SCREEN_FILTER_GRAYSCALE 1
#define HUMAN_CLASS_ID 0x5a390072u
#define PHONE_VIEW_PROP_CLASS_ID 0x789c5740u
#define ALL_SOUND_LOOPS "*"

static int soul_vision_is_on;
static int mask_is_on;
static int is_quiet;
static const char *current_music;
static const struct script_balloon_row *open_balloon_rows[SCRIPT_BALLOON_MAX];
static struct pending_overlay pending_overlays[PENDING_OVERLAY_MAX];
static char logged_once[LOGGED_ONCE_MAX][ASSET_NAME_MAX];
static int logged_once_count;

static void write_cue(const char *kind, const char *name)
{
    run_log_write("cue tick=%u kind=%s name=%s", tick, kind, name);
}

static void log_once(const char *what, const char *name)
{
    char key[ASSET_NAME_MAX];
    int index;

    snprintf(key, sizeof(key), "%s %s", what, name);
    for (index = 0; index < logged_once_count; index++) {
        if (strcmp(logged_once[index], key) == 0) {
            return;
        }
    }
    if (logged_once_count < LOGGED_ONCE_MAX) {
        snprintf(logged_once[logged_once_count], sizeof(logged_once[logged_once_count]), "%s", key);
        logged_once_count = logged_once_count + 1;
    }
    run_log_write("script %s name=%s", what, name);
}

static char grey_world_effect[ASSET_NAME_MAX];

static void on_stage_cleared(void)
{
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    scene_effects_reset(scene_stage_effects());
    scene_effects_set_flash(scene_stage_effects(), (float)white_level / BRIGHTNESS_SCALE);
    mask_is_on = 0;
    scene_view_forget_held_poses();
    script_view_effect_deleted(ALL_EFFECTS_NAME);
    scene_stage_hide_card();
    card_is_showing = 0;
    memset(open_balloon_rows, 0, sizeof(open_balloon_rows));
    memset(pending_overlays, 0, sizeof(pending_overlays));
    script_view_talk_changed(NULL);
    script_view_film_frame_changed(0);
    script_view_swirl_changed(0);
    script_view_screen_still_changed(0);
    scene_stage_set_mono_mode(0);
    grey_world_effect[0] = '\0';
    mask_fade_alpha = 0.0f;
    mask_fade_is_alive = 0;
    mask_fade_is_white = 0;
    sound_request_stop_loop(ALL_SOUND_LOOPS);
}

static void on_ui_clip_started(const char *name, int level_from, int level_to)
{
    write_cue("ui_clip", name);
    script_ui_view_clip_started(name, level_from, level_to);
}

static void on_world_origin_changed(int x, int y)
{
    scene_stage_set_object_origin((float)x, (float)y);
}

static void on_camera_moved(int x, int y)
{
    scene_stage_set_camera((float)x, (float)y);
}

static void on_screen_filter_changed(int mode)
{
    scene_stage_set_mono_mode(mode == SCREEN_FILTER_GRAYSCALE);
}

static void on_mask_fade_color_changed(int is_white)
{
    mask_fade_is_white = is_white;
}

static void on_sounds_stopped(void)
{
    if (!is_quiet) {
        sound_request_stop();
    }
}

static void on_sound_loop_started(int kind, const char *loop_tag)
{
    const char *name;

    name = script_tables_sound_name(tables, kind);
    if (name != NULL && !is_quiet) {
        sound_request_loop(name, loop_tag);
    }
}

static void on_sound_loop_stopped(const char *loop_tag)
{
    if (!is_quiet) {
        sound_request_stop_loop(loop_tag);
    }
}

static void on_music_volume_changed(int volume, int ticks)
{
    if (!is_quiet) {
        music_request_volume(volume, ticks);
    }
}

static void on_talk_changed(const struct script_talk_view *view)
{
    script_view_talk_changed(view);
}

static void on_room_loaded(int room, int camera_x, int camera_y)
{
    char name[16];
    int found;
    int index;

    snprintf(name, sizeof(name), "%d", room);
    write_cue("room", name);
    scene_stage_clear_plates();
    found = 0;
    for (index = 0; index < tables->room_plate_count; index++) {
        const struct script_room_plate *plate;

        plate = &tables->room_plates[index];
        if (plate->room == room) {
            scene_stage_add_plate(plate->texture_name, plate->x, plate->y, plate->width, plate->height);
            if (strcmp(plate->soul_texture_name, NO_SOUL_TEXTURE) != 0) {
                scene_stage_set_last_plate_soul_texture(plate->soul_texture_name);
            }
            found = 1;
        }
    }
    if (!found) {
        log_once("no_room_row", name);
    }
    scene_stage_set_camera((float)camera_x, (float)camera_y);
}

static void on_camera_cut(int x, int y)
{
    write_cue("camera", "cut");
    scene_stage_set_camera((float)x, (float)y);
}

static int record_of(const struct script_actor_view *actor, int records_per_frame)
{
    if (actor->frame_word_index < 0) {
        return 0;
    }
    return actor->frame_word_index * records_per_frame + (actor->ticks_into_frame * records_per_frame) / DS_TICKS_PER_FRAME;
}

static void on_soul_object_styled(const char *object_name, unsigned int class_id, int has_core, int is_active, int is_dead, int is_phone_view)
{
    const struct script_soul_style *style;

    if (is_phone_view && class_id != HUMAN_CLASS_ID) {
        style = script_tables_soul_style(tables, PHONE_VIEW_PROP_CLASS_ID, 0, 0, 0, 0);
    } else {
        style = script_tables_soul_style(tables, class_id, class_id == HUMAN_CLASS_ID, has_core, is_active, is_dead);
    }
    if (style != NULL) {
        scene_stage_set_object_soul_tint(object_name, style->red, style->green, style->blue, style->alpha, style->is_flat);
    }
}

static void on_actor_changed(const struct script_actor_view *actor)
{
    const struct script_motion_row *row;

    if (!actor->is_visible) {
        scene_stage_remove_object(actor->name);
        return;
    }
    row = script_tables_find_motion(tables, actor->file_name, actor->table_offset);
    if (row == NULL) {
        log_once("no_motion_row", actor->name);
        return;
    }
    scene_stage_place_object(actor->name, row->animation_name, (float)actor->x, (float)actor->y, (LEVEL_COUNT - actor->level) * LAYER_PER_LEVEL + (CREATION_INDEX_TOP - actor->creation_index), record_of(actor, row->records_per_frame));
    scene_stage_set_object_flipped(actor->name, actor->is_flipped);
    scene_stage_set_object_views(actor->name, actor->class_id != CLASS_DRAWN_ONLY_IN_SOUL_VIEW, actor->class_id != CLASS_DRAWN_ONLY_IN_LIVING_VIEW);
}

static void on_brightness_changed(int signed_level)
{
    black_level = signed_level < 0 ? -signed_level : 0;
    white_level = signed_level > 0 ? signed_level : 0;
    scene_effects_set_flash(scene_stage_effects(), (float)white_level / BRIGHTNESS_SCALE);
}

static void on_mask_fade_changed(int is_alive, int alpha)
{
    mask_fade_alpha = (float)alpha / MASK_FADE_ALPHA_FULL;
    mask_fade_is_alive = is_alive;
    if (!is_alive) {
        mask_fade_is_white = 0;
        mask_fade_alpha = 0.0f;
    }
    if (!is_alive && card_is_showing) {
        card_is_showing = 0;
        scene_stage_hide_card();
        write_cue("card", "hide");
    }
}

static void on_beam_created(const char *name, float apex_x, float apex_y, float base_x, float base_y, float half_width, float cover_alpha)
{
    write_cue("beam", name);
    scene_effects_create_beam(scene_stage_effects(), name, apex_x, apex_y, base_x, base_y, half_width);
    scene_effects_ramp_beam_cover(scene_stage_effects(), name, cover_alpha, 0);
}

static void on_beam_cover_ramped(const char *name, float cover_alpha, int ticks)
{
    write_cue("beam", name);
    scene_effects_ramp_beam_cover(scene_stage_effects(), name, cover_alpha, ticks);
}

static void on_beam_width_ramped(const char *name, float half_width, int ticks)
{
    const struct scene_beam *beam;

    write_cue("beam", name);
    beam = scene_effects_find_beam(scene_stage_effects(), name);
    if (beam != NULL) {
        scene_effects_animate_beam(scene_stage_effects(), name, beam->base_x.value, beam->base_y.value, half_width, ticks);
    }
}

static void on_beam_base_ramped(const char *name, float base_x, float base_y, int ticks)
{
    const struct scene_beam *beam;

    write_cue("beam", name);
    beam = scene_effects_find_beam(scene_stage_effects(), name);
    if (beam != NULL) {
        scene_effects_animate_beam(scene_stage_effects(), name, base_x, base_y, beam->half_width.value, ticks);
    }
}

static void on_beam_deleted(const char *name)
{
    write_cue("beam", name);
    scene_effects_delete_beam(scene_stage_effects(), name);
}

static void on_iris_created(const char *name, float x, float y, float radius, float alpha)
{
    write_cue("iris_create", name);
    scene_effects_create_iris(scene_stage_effects(), x, y, radius, alpha);
}

static void on_iris_radius_ramped(const char *name, float radius, int ticks)
{
    write_cue("iris", name);
    scene_effects_ramp_iris_radius(scene_stage_effects(), radius, ticks);
}

static void on_iris_alpha_ramped(const char *name, float alpha, int ticks)
{
    write_cue("iris_alpha", name);
    scene_effects_ramp_iris_alpha(scene_stage_effects(), alpha, ticks);
}

static void on_iris_moved(const char *name, float x, float y, int ticks)
{
    write_cue("iris_move", name);
    scene_effects_move_iris(scene_stage_effects(), x, y, ticks);
}

static void on_iris_deleted(const char *name)
{
    write_cue("iris_delete", name);
    scene_effects_stop_iris(scene_stage_effects());
}

static void start_dissolve(const struct script_effect_row *row, const char *name)
{
    unsigned long long started_us;
    int has_snapshot;

    write_cue("dissolve", name);
    if (is_quiet) {
        return;
    }
    started_us = clock_now_us();
    has_snapshot = scene_view_snapshot_stage(scene_stage_view());
    if (has_snapshot) {
        scene_effects_start_dissolve(scene_stage_effects(), row->end_zoom, row->ticks);
    }
    zoom_ticks_left = row->ticks;
    run_log_write(LOG_FOR_TOOLS_DISSOLVE, row->ticks, row->end_zoom, has_snapshot, clock_now_us() - started_us);
}

static void start_card(const char *name)
{
    write_cue("card", name);
    scene_stage_show_card(tables->card_texture);
    card_is_showing = 1;
    card_started_tick = tick;
}

static void on_sound_played(int kind);

static float phone_split_slide_px(float x, float y)
{
    const struct scene_stage *stage;

    stage = scene_stage_view();
    if (y == 0.0f) {
        return x * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
    }
    return ((stage->camera_x - stage->object_origin_x) - (y - PHONE_SPLIT_BASE_Y)) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;
}

static void on_effect_spawned(int id, const char *name, float x, float y)
{
    const struct script_effect_row *row;
    const struct script_fx_movie *fx_movie;
    char text[64];

    row = script_tables_find_effect(tables, id);
    if (row == NULL) {
        snprintf(text, sizeof(text), "%d %s", id, name);
        log_once("effect_not_drawn", text);
        return;
    }
    switch (row->kind) {
    case SCRIPT_EFFECT_CARD:
        start_card(name);
        break;
    case SCRIPT_EFFECT_UI_CLIP:
        script_ui_view_clip_started(row->texture_name, 0, 0);
        break;
    case SCRIPT_EFFECT_GREY_WORLD:
        snprintf(grey_world_effect, sizeof(grey_world_effect), "%s", name);
        scene_stage_set_mono_mode(1);
        break;
    case SCRIPT_EFFECT_DISSOLVE:
        start_dissolve(row, name);
        break;
    case SCRIPT_EFFECT_PHONE_SPLIT:
        script_view_phone_split_spawned(row, name, phone_split_slide_px(x, y));
        break;
    case SCRIPT_EFFECT_MOVIE:
        fx_movie = script_tables_find_fx_movie(tables, row->texture_name);
        if (fx_movie != NULL && fx_movie->sound_kind >= 0) {
            on_sound_played(fx_movie->sound_kind);
        }
        script_view_effect_spawned(row, name, x, y);
        break;
    default:
        script_view_effect_spawned(row, name, x, y);
        break;
    }
}

static void on_effect_deleted(const char *name)
{
    if (grey_world_effect[0] != '\0' && (strcmp(name, ALL_EFFECTS_NAME) == 0 || strcmp(name, grey_world_effect) == 0)) {
        grey_world_effect[0] = '\0';
        scene_stage_set_mono_mode(0);
    }
    script_view_effect_deleted(name);
}

static void show_group(const char *label, int group)
{
    int shown;
    int index;

    shown = 0;
    for (index = 0; index < tables->text_row_count; index++) {
        const struct script_text_row *row;

        row = &tables->text_rows[index];
        if (row->group == group && strcmp(row->label, label) == 0) {
            scene_layers_show_text(scene_stage_layers(), row->id, row->texture_name, row->center_x, row->top_y, SCRIPT_NARRATION_GROUP_IN_TICKS);
            write_cue("text_show", row->texture_name);
            shown = 1;
        }
    }
    if (!shown) {
        log_once("no_text_lines", label);
    }
}

static void fade_out_message(const char *label)
{
    int index;

    for (index = 0; index < tables->text_row_count; index++) {
        const struct script_text_row *row;

        row = &tables->text_rows[index];
        if (strcmp(row->label, label) == 0) {
            scene_layers_hide_text(scene_stage_layers(), row->id, SCRIPT_NARRATION_TEXT_OUT_TICKS);
            write_cue("text_hide", row->texture_name);
        }
    }
}

static void on_narration_changed(const char *label, enum script_narration_event event, int group)
{
    switch (event) {
    case SCRIPT_NARRATION_DIM_IN:
        write_cue("dim", label);
        scene_effects_dim_to(scene_stage_effects(), DIM_LEVEL_WHEN_FULL, SCRIPT_NARRATION_DIM_TICKS);
        break;
    case SCRIPT_NARRATION_DIM_FULL:
        write_cue("dim", label);
        scene_effects_dim_to(scene_stage_effects(), DIM_LEVEL_WHEN_FULL, 0);
        break;
    case SCRIPT_NARRATION_GROUP_IN:
        show_group(label, group);
        break;
    case SCRIPT_NARRATION_TEXT_OUT:
        fade_out_message(label);
        break;
    case SCRIPT_NARRATION_CUT:
        write_cue("text_hide_all", label);
        write_cue("dim", label);
        scene_layers_hide_all_text(scene_stage_layers());
        scene_effects_dim_to(scene_stage_effects(), 1.0f, 0);
        break;
    default:
        write_cue("dim", label);
        scene_effects_dim_to(scene_stage_effects(), 1.0f, SCRIPT_NARRATION_DIM_TICKS);
        break;
    }
}

static void schedule_overlay(unsigned int due_tick, int is_hide, int overlay_id, const struct script_balloon_part *part, float center_x, float center_y)
{
    int index;

    for (index = 0; index < PENDING_OVERLAY_MAX; index++) {
        if (!pending_overlays[index].in_use) {
            pending_overlays[index].in_use = 1;
            pending_overlays[index].due_tick = due_tick;
            pending_overlays[index].is_hide = is_hide;
            pending_overlays[index].overlay_id = overlay_id;
            pending_overlays[index].part = part;
            pending_overlays[index].center_x = center_x;
            pending_overlays[index].center_y = center_y;
            return;
        }
    }
    run_log_write("script too_many_pending_overlays");
}

static void on_balloon_opened(int slot, const char *label, int kind, float anchor_x, float anchor_y)
{
    const struct script_balloon_row *row;
    char name[SCRIPT_CUE_LABEL_MAX];
    int index;

    if (kind == SCRIPT_BALLOON_KIND_TAP_MARKS || kind == SCRIPT_BALLOON_KIND_TAP_DOTS) {
        script_ui_view_tap_balloon_opened(slot, kind, anchor_x, anchor_y);
        open_balloon_rows[slot] = NULL;
        return;
    }
    row = script_tables_find_balloon(tables, label, kind);
    open_balloon_rows[slot] = row;
    if (row == NULL) {
        snprintf(name, sizeof(name), "%s", label != NULL ? label : "kind");
        log_once("no_balloon_row", name);
        return;
    }
    for (index = 0; index < row->part_count; index++) {
        const struct script_balloon_part *part;

        part = &row->parts[index];
        schedule_overlay(tick + (unsigned int)row->delay_ticks + (unsigned int)part->after_ticks, 0, slot * OVERLAYS_PER_BALLOON + index + 1, part, anchor_x + part->dx, anchor_y + part->dy);
    }
}

static void on_balloon_closed(int slot)
{
    const struct script_balloon_row *row;
    int index;

    script_ui_view_tap_balloon_closed(slot);
    row = open_balloon_rows[slot];
    if (row == NULL) {
        return;
    }
    for (index = 0; index < row->part_count; index++) {
        schedule_overlay(tick + (unsigned int)row->delay_ticks, 1, slot * OVERLAYS_PER_BALLOON + index + 1, &row->parts[index], 0.0f, 0.0f);
    }
    open_balloon_rows[slot] = NULL;
}

struct balloon_shake {
    int strength;
    int ticks_left;
};

static struct balloon_shake balloon_shakes[SCRIPT_BALLOON_MAX];

static void run_due_overlays(void)
{
    int index;

    for (index = 0; index < PENDING_OVERLAY_MAX; index++) {
        struct pending_overlay *pending;

        pending = &pending_overlays[index];
        if (!pending->in_use || pending->due_tick > tick) {
            continue;
        }
        pending->in_use = 0;
        if (pending->is_hide) {
            write_cue("overlay_hide", pending->part->texture_name);
            scene_layers_hide_overlay(scene_stage_layers(), pending->overlay_id);
        } else {
            write_cue("overlay", pending->part->texture_name);
            scene_layers_show_overlay(scene_stage_layers(), pending->overlay_id, pending->part->texture_name, pending->center_x - pending->part->width / 2.0f, pending->center_y - pending->part->height / 2.0f, pending->part->width, pending->part->height, pending->part->grow_ticks);
        }
    }
}

static void on_balloon_shaken(int slot, int strength, int ticks)
{
    balloon_shakes[slot].strength = strength;
    balloon_shakes[slot].ticks_left = ticks;
}

static void set_balloon_shake(int slot, float x, float y)
{
    int part;

    for (part = 1; part <= OVERLAYS_PER_BALLOON; part++) {
        scene_layers_shake_overlay(scene_stage_layers(), slot * OVERLAYS_PER_BALLOON + part, x, y);
    }
}

static void shake_balloons(void)
{
    int slot;

    for (slot = 0; slot < SCRIPT_BALLOON_MAX; slot++) {
        struct balloon_shake *shake;

        shake = &balloon_shakes[slot];
        if (shake->ticks_left <= 0) {
            continue;
        }
        shake->ticks_left = shake->ticks_left - 1;
        if (shake->ticks_left == 0) {
            set_balloon_shake(slot, 0.0f, 0.0f);
        } else {
            float reach;

            reach = (float)shake->strength * BALLOON_SHAKE_DS_PIXELS_PER_STRENGTH;
            set_balloon_shake(slot, (tick & 1u) != 0u ? reach : -reach, ((tick >> 1) & 1u) != 0u ? reach : -reach);
        }
    }
}

static void on_music_prepared(int id)
{
    const char *name;
    char number[16];

    name = script_tables_music_name(tables, id);
    if (name == NULL) {
        snprintf(number, sizeof(number), "%d", id);
        log_once("no_music_row", number);
        return;
    }
    if (!is_quiet) {
        music_request_prepare(name);
    }
}

static void on_music_started(int id)
{
    const char *name;
    char number[16];

    name = script_tables_music_name(tables, id);
    if (name == NULL) {
        snprintf(number, sizeof(number), "%d", id);
        log_once("no_music_row", number);
        return;
    }
    write_cue("music", name);
    current_music = name;
    if (!is_quiet) {
        music_request_play(name, 1);
    }
}

static void on_music_stopped(int fade_ticks)
{
    write_cue("music_stop", "-");
    current_music = NULL;
    music_request_stop(is_quiet ? 0 : fade_ticks);
}

static void on_sound_played(int kind)
{
    const char *name;
    char number[16];

    name = script_tables_sound_name(tables, kind);
    if (name == NULL) {
        snprintf(number, sizeof(number), "%d", kind);
        log_once("no_sound_row", number);
        return;
    }
    write_cue("sound", name);
    if (!is_quiet) {
        sound_request_play(name);
    }
}

static void start_movie_with_its_sound(const char *name)
{
    const struct script_movie *movie;

    script_view_movie_started(name);
    movie = script_tables_find_movie(tables, name);
    if (movie != NULL && movie->sound_kind >= 0) {
        on_sound_played(movie->sound_kind);
    }
}

static int on_movie_of_class_started(unsigned int class_id)
{
    const struct script_movie *movie;
    char text[32];

    movie = script_tables_find_movie_of_class(tables, class_id);
    if (movie == NULL) {
        snprintf(text, sizeof(text), "%08x", class_id);
        log_once("movie_of_class_not_baked", text);
        return 0;
    }
    start_movie_with_its_sound(movie->name);
    return movie->frame_count * movie->ticks_per_frame;
}

static void on_soul_vision_changed(int is_on, int with_effect)
{
    soul_vision_is_on = is_on;
    script_view_soul_changed(is_on, with_effect);
    run_log_write("script soul_vision on=%d tick=%u", is_on, tick);
}

static void on_marked(const char *what, const char *name)
{
    if (strcmp(what, "demo_end") == 0) {
        write_cue("demo_end", name);
        return;
    }
    script_player_note_mark(what, name);
    run_log_write("script %s name=%s", what, name);
}

static const struct script_outputs bridge_outputs = {
    .stage_cleared = on_stage_cleared,
    .room_loaded = on_room_loaded,
    .camera_cut = on_camera_cut,
    .camera_moved = on_camera_moved,
    .world_origin_changed = on_world_origin_changed,
    .ui_clip_started = on_ui_clip_started,
    .hud_changed = script_ui_view_hud_changed,
    .screen_filter_changed = on_screen_filter_changed,
    .mask_fade_color_changed = on_mask_fade_color_changed,
    .actor_changed = on_actor_changed,
    .brightness_changed = on_brightness_changed,
    .mask_fade_changed = on_mask_fade_changed,
    .beam_created = on_beam_created,
    .beam_cover_ramped = on_beam_cover_ramped,
    .beam_width_ramped = on_beam_width_ramped,
    .beam_base_ramped = on_beam_base_ramped,
    .beam_deleted = on_beam_deleted,
    .iris_created = on_iris_created,
    .iris_radius_ramped = on_iris_radius_ramped,
    .iris_alpha_ramped = on_iris_alpha_ramped,
    .iris_moved = on_iris_moved,
    .iris_deleted = on_iris_deleted,
    .effect_spawned = on_effect_spawned,
    .effect_deleted = on_effect_deleted,
    .effect_signalled = script_view_effect_signalled,
    .effect_moved = script_view_effect_moved,
    .narration_changed = on_narration_changed,
    .balloon_opened = on_balloon_opened,
    .balloon_closed = on_balloon_closed,
    .balloon_shaken = on_balloon_shaken,
    .music_prepared = on_music_prepared,
    .music_started = on_music_started,
    .music_stopped = on_music_stopped,
    .music_volume_changed = on_music_volume_changed,
    .sound_loop_started = on_sound_loop_started,
    .sound_loop_stopped = on_sound_loop_stopped,
    .sound_played = on_sound_played,
    .sounds_stopped = on_sounds_stopped,
    .soul_vision_changed = on_soul_vision_changed,
    .phone_view_ended = scene_stage_clear_room,
    .soul_cursor_moved = script_view_cursor_moved,
    .soul_chain_moved = script_view_chain_moved,
    .soul_core_marked = script_view_core_marked,
    .soul_mark_changed = script_view_mark_changed,
    .soul_flash_changed = script_view_soul_flash_changed,
    .film_frame_changed = script_view_film_frame_changed,
    .swirl_changed = script_view_swirl_changed,
    .film_scroll_paused = script_view_film_scroll_paused,
    .screen_still_changed = script_view_screen_still_changed,
    .movie_started = start_movie_with_its_sound,
    .movie_of_class_started = on_movie_of_class_started,
    .soul_sparks_changed = script_view_soul_sparks_changed,
    .soul_object_styled = on_soul_object_styled,
    .marked = on_marked,
    .talk_changed = on_talk_changed
};

void script_bridge_set_quiet(int should_be_quiet)
{
    if (is_quiet && !should_be_quiet && current_music != NULL) {
        music_request_prepare(current_music);
        music_request_play(current_music, 1);
    }
    if (!is_quiet && should_be_quiet) {
        music_request_stop(0);
        sound_request_stop();
    }
    is_quiet = should_be_quiet;
}

const struct script_outputs *script_bridge_outputs(void)
{
    return &bridge_outputs;
}

void script_bridge_start(const struct script_tables *new_tables)
{
    tables = new_tables;
    tick = 0;
    card_is_showing = 0;
    card_started_tick = 0;
    black_level = 0;
    white_level = 0;
    mask_fade_is_alive = 0;
    zoom_ticks_left = 0;
    soul_vision_is_on = 0;
    mask_is_on = 0;
    logged_once_count = 0;
    current_music = NULL;
    script_view_start(new_tables);
    script_ui_view_start(new_tables);
    memset(open_balloon_rows, 0, sizeof(open_balloon_rows));
    memset(pending_overlays, 0, sizeof(pending_overlays));
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    scene_effects_reset(scene_stage_effects());
}

static float card_fade_amount(void)
{
    int age;

    if (!card_is_showing) {
        return 0.0f;
    }
    age = (int)(tick - card_started_tick);
    if (age < CARD_FADE_IN_STARTS) {
        return 1.0f;
    }
    if (age < CARD_FADE_IN_ENDS) {
        return 1.0f - (float)(age - CARD_FADE_IN_STARTS) / (float)(CARD_FADE_IN_ENDS - CARD_FADE_IN_STARTS);
    }
    if (age < CARD_FADE_OUT_STARTS) {
        return 0.0f;
    }
    if (age < CARD_FADE_OUT_ENDS) {
        return (float)(age - CARD_FADE_OUT_STARTS) / (float)(CARD_FADE_OUT_ENDS - CARD_FADE_OUT_STARTS);
    }
    return 1.0f;
}

static void apply_fade(void)
{
    float brightness_fade;
    float card_fade;

    brightness_fade = (float)black_level / BRIGHTNESS_SCALE;
    card_fade = card_fade_amount();
    scene_stage_set_fade(brightness_fade > card_fade ? brightness_fade : card_fade);
}

static int any_beam_exists(const struct scene_effects *effects)
{
    int index;

    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (effects->beams[index].in_use) {
            return 1;
        }
    }
    return 0;
}

static void apply_mask_rule(void)
{
    struct scene_effects *effects;
    int should_be_on;

    effects = scene_stage_effects();
    should_be_on = any_beam_exists(effects) || effects->iris_is_on || mask_fade_is_alive || zoom_ticks_left > 0;
    if (should_be_on != mask_is_on) {
        mask_is_on = should_be_on;
        write_cue("mask", should_be_on ? "on" : "off");
    }
    scene_effects_set_cover_mask(effects, should_be_on, zoom_ticks_left > 0, mask_fade_alpha, mask_fade_is_white);
}

void script_bridge_finish_tick(void)
{
    run_due_overlays();
    shake_balloons();
    apply_fade();
    apply_mask_rule();
    if (zoom_ticks_left > 0) {
        zoom_ticks_left = zoom_ticks_left - 1;
    }
    scene_effects_advance(scene_stage_effects(), MICROSECONDS_PER_TICK);
    scene_layers_advance(scene_stage_layers(), MICROSECONDS_PER_TICK);
    script_view_advance();
    tick = tick + 1;
}

unsigned int script_bridge_tick(void)
{
    return tick;
}
