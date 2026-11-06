#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "background_reader.h"
#include "clock.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "music_engine.h"
#include "run_log.h"
#include "scene.h"
#include "scene_player.h"
#include "scene_stage.h"
#include "sound_engine.h"
#include "texture_cache.h"

#define PRELOAD_PASS_MAX (TEXTURE_CACHE_ENTRY_MAX / BACKGROUND_READER_QUEUE_MAX)
#define LONGEST_UPDATE_US 500000ULL
#define LONGEST_AUDIO_PRELOAD_US 10000000ULL
#define LOG_FOR_TOOLS_SCENE_READY "scene ready name=%s preload_us=%llu texture_bytes=%u missing=%u"

struct script_progress {
    int next_step;
    unsigned long long hold_remaining_us;
    unsigned long long hold_total_us;
    float fade_start;
    float fade_target;
    float camera_start_x;
    float camera_start_y;
    float camera_target_x;
    float camera_target_y;
    int is_fading;
    int is_panning;
};

static char folder[FILE_PATH_MAX];
static struct scene *current_scene;
static enum scene_playback_state status;
static struct script_progress script;
static int is_paused;
static unsigned long long preload_started_us;
static int preload_passes;
static const char *preload_music_name;

static void reset_script_progress(void)
{
    memset(&script, 0, sizeof(script));
}

static void begin_hold(int ticks)
{
    script.hold_total_us = (unsigned long long)ticks * MICROSECONDS_PER_TICK;
    script.hold_remaining_us = script.hold_total_us;
}

static void begin_fade(float target, int ticks)
{
    script.fade_start = scene_stage_view()->fade_amount;
    script.fade_target = target;
    script.is_fading = 1;
    begin_hold(ticks);
}

static void begin_pan(float target_x, float target_y, int ticks)
{
    script.camera_start_x = scene_stage_view()->camera_x;
    script.camera_start_y = scene_stage_view()->camera_y;
    script.camera_target_x = target_x;
    script.camera_target_y = target_y;
    script.is_panning = 1;
    begin_hold(ticks);
}

static void run_step(const struct scene_step *step)
{
    switch (step->kind) {
    case SCENE_STEP_ROOM:
        run_log_write("scene room id=%d", step->room_id);
        scene_stage_clear_room();
        break;
    case SCENE_STEP_PLATE:
        scene_stage_add_plate(step->texture_name, step->x, step->y, step->width, step->height);
        break;
    case SCENE_STEP_OBJECT:
        scene_stage_set_object(step->object_name, step->animation_name, step->x, step->y, step->layer, step->is_flipped);
        break;
    case SCENE_STEP_REMOVE_OBJECT:
        scene_stage_remove_object(step->object_name);
        break;
    case SCENE_STEP_CAMERA:
        scene_stage_set_camera(step->x, step->y);
        script.is_panning = 0;
        break;
    case SCENE_STEP_CAMERA_PAN:
        begin_pan(step->x, step->y, step->ticks);
        break;
    case SCENE_STEP_CARD:
        scene_stage_show_card(step->texture_name);
        run_log_write("scene card texture=%s", step->texture_name);
        break;
    case SCENE_STEP_HIDE_CARD:
        scene_stage_hide_card();
        break;
    case SCENE_STEP_TEXT_SHOW:
        scene_layers_show_text(scene_stage_layers(), step->layer_item_id, step->texture_name, step->x, step->y, step->ticks);
        break;
    case SCENE_STEP_TEXT_HIDE:
        scene_layers_hide_text(scene_stage_layers(), step->layer_item_id, step->ticks);
        break;
    case SCENE_STEP_TEXT_HIDE_ALL:
        scene_layers_hide_all_text(scene_stage_layers());
        break;
    case SCENE_STEP_OVERLAY:
        scene_layers_show_overlay(scene_stage_layers(), step->layer_item_id, step->texture_name, step->x, step->y, step->width, step->height, step->ticks);
        break;
    case SCENE_STEP_OVERLAY_HIDE:
        scene_layers_hide_overlay(scene_stage_layers(), step->layer_item_id);
        break;
    case SCENE_STEP_FADE_OUT:
        begin_fade(1.0f, step->ticks);
        break;
    case SCENE_STEP_FADE_IN:
        begin_fade(0.0f, step->ticks);
        break;
    case SCENE_STEP_WAIT:
        begin_hold(step->ticks);
        break;
    case SCENE_STEP_BEAM_CREATE:
        scene_effects_create_beam(scene_stage_effects(), step->beam_name, step->apex_x, step->apex_y, step->x, step->y, step->half_width);
        run_log_write("scene beam_create name=%s", step->beam_name);
        break;
    case SCENE_STEP_BEAM_ANIMATE:
        scene_effects_animate_beam(scene_stage_effects(), step->beam_name, step->x, step->y, step->half_width, step->ticks);
        run_log_write("scene beam_animate name=%s ticks=%d", step->beam_name, step->ticks);
        break;
    case SCENE_STEP_BEAM_DELETE:
        scene_effects_delete_beam(scene_stage_effects(), step->beam_name);
        break;
    case SCENE_STEP_MASK_ON:
        scene_effects_set_mask(scene_stage_effects(), 1);
        run_log_write("scene mask on");
        break;
    case SCENE_STEP_MASK_OFF:
        scene_effects_set_mask(scene_stage_effects(), 0);
        run_log_write("scene mask off");
        break;
    case SCENE_STEP_IRIS:
        scene_effects_start_iris(scene_stage_effects(), step->x, step->y, step->radius, step->ticks);
        run_log_write("scene iris ticks=%d", step->ticks);
        break;
    case SCENE_STEP_DIM_TO:
        scene_effects_dim_to(scene_stage_effects(), step->brightness, step->ticks);
        run_log_write("scene dim ticks=%d", step->ticks);
        break;
    case SCENE_STEP_FLASH:
        scene_effects_flash(scene_stage_effects(), step->brightness, step->ticks);
        run_log_write("scene flash ticks=%d", step->ticks);
        break;
    case SCENE_STEP_MUSIC:
        music_request_play(step->audio_name, step->is_looping);
        break;
    case SCENE_STEP_MUSIC_STOP:
        music_request_stop(step->ticks);
        break;
    case SCENE_STEP_SOUND:
        sound_request_play(step->audio_name);
        break;
    }
}

static void update_hold_effects(void)
{
    float progress;

    progress = 1.0f;
    if (script.hold_total_us > 0) {
        progress = 1.0f - (float)script.hold_remaining_us / (float)script.hold_total_us;
    }
    if (script.is_fading) {
        scene_stage_set_fade(script.fade_start + (script.fade_target - script.fade_start) * progress);
    }
    if (script.is_panning) {
        scene_stage_set_camera(script.camera_start_x + (script.camera_target_x - script.camera_start_x) * progress, script.camera_start_y + (script.camera_target_y - script.camera_start_y) * progress);
    }
    if (script.hold_remaining_us == 0) {
        script.is_fading = 0;
        script.is_panning = 0;
    }
}

static void run_script(unsigned long long elapsed_us)
{
    if (script.next_step == 0) {
        elapsed_us = 0;
    }
    while (script.hold_remaining_us <= elapsed_us) {
        scene_effects_advance(scene_stage_effects(), script.hold_remaining_us);
        elapsed_us = elapsed_us - script.hold_remaining_us;
        script.hold_remaining_us = 0;
        update_hold_effects();
        while (script.hold_remaining_us == 0 && script.next_step < current_scene->step_count) {
            run_step(&current_scene->steps[script.next_step]);
            script.next_step = script.next_step + 1;
        }
        if (script.hold_remaining_us == 0) {
            status = SCENE_PLAYER_FINISHED;
            scene_effects_advance(scene_stage_effects(), elapsed_us);
            music_request_stop(MUSIC_QUICK_FADE_TICKS);
            run_log_write("scene finished name=%s", current_scene->name);
            return;
        }
    }
    script.hold_remaining_us = script.hold_remaining_us - elapsed_us;
    scene_effects_advance(scene_stage_effects(), elapsed_us);
    update_hold_effects();
}

static void preload_step_assets(const struct scene_step *step)
{
    const struct animation *animation;

    switch (step->kind) {
    case SCENE_STEP_PLATE:
    case SCENE_STEP_CARD:
    case SCENE_STEP_TEXT_SHOW:
    case SCENE_STEP_OVERLAY:
        texture_cache_use_or_start_loading(step->texture_name);
        break;
    case SCENE_STEP_OBJECT:
        animation = animation_library_load(step->animation_name);
        if (animation != NULL) {
            scene_stage_request_animation_textures(animation);
        }
        break;
    default:
        break;
    }
}

static void request_scene_assets(void)
{
    int step;

    for (step = 0; step < current_scene->step_count; step++) {
        preload_step_assets(&current_scene->steps[step]);
    }
}

static void stop_scene_audio(void)
{
    music_request_stop(MUSIC_QUICK_FADE_TICKS);
    sound_request_stop();
}

static void request_scene_audio(void)
{
    const struct scene_step *step;
    int index;

    preload_music_name = NULL;
    for (index = 0; index < current_scene->step_count; index++) {
        step = &current_scene->steps[index];
        if (step->kind == SCENE_STEP_MUSIC && preload_music_name == NULL) {
            preload_music_name = step->audio_name;
            music_request_prepare(preload_music_name);
        }
        if (step->kind == SCENE_STEP_SOUND) {
            sound_request_prepare(step->audio_name);
        }
    }
}

static int scene_audio_is_ready(void)
{
    int is_ready;

    is_ready = sound_prepare_is_done() && (preload_music_name == NULL || music_prepare_is_done(preload_music_name));
    if (!is_ready && clock_now_us() - preload_started_us > LONGEST_AUDIO_PRELOAD_US) {
        run_log_write("scene audio_wait_gave_up name=%s", current_scene->name);
        return 1;
    }
    return is_ready;
}

static void finish_preloading_when_ready(void)
{
    struct texture_cache_totals totals;

    if (texture_cache_loading_count() > 0) {
        return;
    }
    if (!scene_audio_is_ready()) {
        return;
    }
    if (preload_passes < PRELOAD_PASS_MAX) {
        preload_passes = preload_passes + 1;
        request_scene_assets();
        if (texture_cache_loading_count() > 0) {
            return;
        }
    } else {
        run_log_write("scene does_not_fit_in_cache name=%s", current_scene->name);
    }
    texture_cache_read_totals(&totals);
    run_log_write(LOG_FOR_TOOLS_SCENE_READY, current_scene->name, clock_now_us() - preload_started_us, totals.bytes_in_use, totals.missing);
    status = SCENE_PLAYER_RUNNING;
}

void scene_player_init(const char *scene_folder)
{
    snprintf(folder, sizeof(folder), "%s", scene_folder);
    current_scene = NULL;
    scene_stage_reset();
    status = SCENE_PLAYER_IDLE;
    is_paused = 0;
    reset_script_progress();
}

static struct scene *read_scene_file(const char *scene_name)
{
    char path[FILE_PATH_MAX];
    unsigned char *text;
    unsigned int text_size;
    struct scene *scene;
    int result;

    if (!file_path_join(path, folder, scene_name, ".scene")) {
        run_log_write("scene path_too_long name=%s", scene_name);
        return NULL;
    }
    result = file_read_whole(path, &text, &text_size);
    if (result != FILE_OK) {
        run_log_write("scene missing name=%s code=%d", scene_name, result);
        return NULL;
    }
    scene = mem_alloc(sizeof(struct scene));
    if (scene == NULL) {
        mem_free(text);
        run_log_write("scene out_of_memory name=%s", scene_name);
        return NULL;
    }
    result = scene_parse((const char *)text, text_size, scene);
    mem_free(text);
    if (result != SCENE_OK) {
        mem_free(scene);
        run_log_write("scene invalid name=%s code=%d", scene_name, result);
        return NULL;
    }
    return scene;
}

int scene_player_play(const char *scene_name)
{
    struct scene *scene;

    scene = read_scene_file(scene_name);
    if (scene == NULL) {
        return 0;
    }
    scene_stage_clear_room();
    scene_layers_clear(scene_stage_layers());
    animation_library_clear();
    mem_free(current_scene);
    current_scene = scene;
    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    scene_effects_reset(scene_stage_effects());
    reset_script_progress();
    is_paused = 0;
    preload_started_us = clock_now_us();
    preload_passes = 0;
    stop_scene_audio();
    request_scene_audio();
    request_scene_assets();
    status = SCENE_PLAYER_PRELOADING;
    run_log_write("scene started name=%s steps=%d", scene->name, scene->step_count);
    return 1;
}

void scene_player_update(unsigned long long elapsed_us)
{
    if (status == SCENE_PLAYER_PRELOADING) {
        finish_preloading_when_ready();
        return;
    }
    if (is_paused) {
        return;
    }
    if (elapsed_us > LONGEST_UPDATE_US) {
        elapsed_us = LONGEST_UPDATE_US;
    }
    scene_stage_advance_objects(elapsed_us);
    scene_layers_advance(scene_stage_layers(), elapsed_us);
    if (status == SCENE_PLAYER_RUNNING) {
        run_script(elapsed_us);
    } else {
        scene_effects_advance(scene_stage_effects(), elapsed_us);
    }
}

void scene_player_stop(void)
{
    stop_scene_audio();
    status = SCENE_PLAYER_IDLE;
    scene_stage_set_fade(0.0f);
    scene_stage_hide_card();
    scene_effects_reset(scene_stage_effects());
    scene_layers_clear(scene_stage_layers());
    reset_script_progress();
}

void scene_player_skip_hold(void)
{
    if (status != SCENE_PLAYER_RUNNING) {
        return;
    }
    script.hold_remaining_us = 0;
    run_log_write("scene hold_skipped next_step=%d", script.next_step);
}

void scene_player_set_paused(int should_pause)
{
    if (should_pause != is_paused) {
        run_log_write("scene %s next_step=%d", should_pause ? "paused" : "resumed", script.next_step);
    }
    is_paused = should_pause;
}

int scene_player_is_paused(void)
{
    return is_paused;
}

enum scene_playback_state scene_player_status(void)
{
    return status;
}

int scene_player_next_step(void)
{
    return script.next_step;
}

void scene_player_shutdown(void)
{
    scene_stage_log_coverage();
    mem_free(current_scene);
    current_scene = NULL;
    scene_stage_reset();
    status = SCENE_PLAYER_IDLE;
    reset_script_progress();
}
