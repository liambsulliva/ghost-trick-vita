#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "scene_player.h"
#include "timing.h"

#define MICROSECONDS_PER_TICK (1000000ULL / 60ULL)

static char folder[FILE_PATH_MAX];
static struct scene *current_scene;
static struct scene_picture picture;
static enum scene_player_status status;
static int next_step;
static int is_paused;
static unsigned long long preload_started_us;
static unsigned long long hold_remaining_us;
static unsigned long long hold_total_us;
static float fade_start;
static float fade_target;
static float camera_start_x;
static float camera_start_y;
static float camera_target_x;
static float camera_target_y;
static int is_fading;
static int is_panning;

static int frame_can_be_shown(const struct animation_frame *frame)
{
    const struct cached_texture *texture;

    if (animation_frame_is_blank(frame)) {
        return 1;
    }
    texture = texture_cache_request(frame->texture_name);
    return texture == NULL || texture->state != CACHED_TEXTURE_LOADING;
}

static struct scene_object *find_object(const char *name)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (picture.objects[index].in_use && strcmp(picture.objects[index].name, name) == 0) {
            return &picture.objects[index];
        }
    }
    return NULL;
}

static struct scene_object *find_free_object(void)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (!picture.objects[index].in_use) {
            return &picture.objects[index];
        }
    }
    return NULL;
}

static void log_object_coverage(const struct scene_object *object)
{
    const struct animation *animation;

    animation = object->player.animation;
    if (animation == NULL) {
        return;
    }
    run_log_write("animation coverage object=%s name=%s frames_shown=%u frames_total=%u late_frames=%u",
        object->name, animation->name, animation_player_count_shown_frames(&object->player), animation->frame_count, object->player.late_frame_count);
}

static void request_animation_textures(const struct animation *animation)
{
    unsigned int frame;

    for (frame = 0; frame < animation->frame_count; frame++) {
        if (!animation_frame_is_blank(&animation->frames[frame])) {
            texture_cache_request(animation->frames[frame].texture_name);
        }
    }
}

int scene_player_set_object(const char *object_name, const char *animation_name, float x, float y, int layer, int is_flipped)
{
    const struct animation *animation;
    struct scene_object *object;

    animation = animation_library_load(animation_name);
    if (animation == NULL) {
        return 0;
    }
    object = find_object(object_name);
    if (object != NULL) {
        log_object_coverage(object);
    } else {
        object = find_free_object();
        if (object == NULL) {
            run_log_write("scene too_many_objects name=%s", object_name);
            return 0;
        }
    }
    snprintf(object->name, sizeof(object->name), "%s", object_name);
    request_animation_textures(animation);
    animation_player_start(&object->player, animation);
    object->x = x;
    object->y = y;
    object->layer = layer;
    object->is_flipped = is_flipped;
    object->in_use = 1;
    return 1;
}

static void remove_object(const char *object_name)
{
    struct scene_object *object;

    object = find_object(object_name);
    if (object != NULL) {
        log_object_coverage(object);
        object->in_use = 0;
    }
}

void scene_player_clear_room(void)
{
    int index;

    picture.plate_count = 0;
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (picture.objects[index].in_use) {
            log_object_coverage(&picture.objects[index]);
            picture.objects[index].in_use = 0;
        }
    }
}

void scene_player_add_plate(const char *texture_name, float x, float y, float width, float height)
{
    struct scene_plate *plate;

    if (picture.plate_count >= SCENE_PLATE_MAX) {
        run_log_write("scene too_many_plates name=%s", texture_name);
        return;
    }
    plate = &picture.plates[picture.plate_count];
    snprintf(plate->texture_name, sizeof(plate->texture_name), "%s", texture_name);
    plate->x = x;
    plate->y = y;
    plate->width = width;
    plate->height = height;
    picture.plate_count = picture.plate_count + 1;
    texture_cache_request(texture_name);
}

void scene_player_set_camera(float x, float y)
{
    picture.camera_x = x;
    picture.camera_y = y;
    is_panning = 0;
}

static void begin_hold(int ticks)
{
    hold_total_us = (unsigned long long)ticks * MICROSECONDS_PER_TICK;
    hold_remaining_us = hold_total_us;
}

static void begin_fade(float target, int ticks)
{
    fade_start = picture.fade_amount;
    fade_target = target;
    is_fading = 1;
    begin_hold(ticks);
}

static void begin_pan(float target_x, float target_y, int ticks)
{
    camera_start_x = picture.camera_x;
    camera_start_y = picture.camera_y;
    camera_target_x = target_x;
    camera_target_y = target_y;
    is_panning = 1;
    begin_hold(ticks);
}

static void run_step(const struct scene_step *step)
{
    switch (step->kind) {
    case SCENE_STEP_SIDE_BARS:
        snprintf(picture.left_bar_texture, sizeof(picture.left_bar_texture), "%s", step->first_name);
        snprintf(picture.right_bar_texture, sizeof(picture.right_bar_texture), "%s", step->second_name);
        break;
    case SCENE_STEP_ROOM:
        run_log_write("scene room id=%d", step->number);
        scene_player_clear_room();
        break;
    case SCENE_STEP_PLATE:
        scene_player_add_plate(step->first_name, step->x, step->y, step->width, step->height);
        break;
    case SCENE_STEP_OBJECT:
        scene_player_set_object(step->first_name, step->second_name, step->x, step->y, step->number, step->flip);
        break;
    case SCENE_STEP_REMOVE_OBJECT:
        remove_object(step->first_name);
        break;
    case SCENE_STEP_CAMERA:
        scene_player_set_camera(step->x, step->y);
        break;
    case SCENE_STEP_CAMERA_PAN:
        begin_pan(step->x, step->y, step->number);
        break;
    case SCENE_STEP_CARD:
        snprintf(picture.card_texture, sizeof(picture.card_texture), "%s", step->first_name);
        run_log_write("scene card texture=%s", step->first_name);
        break;
    case SCENE_STEP_HIDE_CARD:
        picture.card_texture[0] = '\0';
        break;
    case SCENE_STEP_FADE_OUT:
        begin_fade(1.0f, step->number);
        break;
    case SCENE_STEP_FADE_IN:
        begin_fade(0.0f, step->number);
        break;
    case SCENE_STEP_WAIT:
        begin_hold(step->number);
        break;
    }
}

static void update_hold_effects(void)
{
    float progress;

    progress = 1.0f;
    if (hold_total_us > 0) {
        progress = 1.0f - (float)hold_remaining_us / (float)hold_total_us;
    }
    if (is_fading) {
        picture.fade_amount = fade_start + (fade_target - fade_start) * progress;
    }
    if (is_panning) {
        picture.camera_x = camera_start_x + (camera_target_x - camera_start_x) * progress;
        picture.camera_y = camera_start_y + (camera_target_y - camera_start_y) * progress;
    }
    if (hold_remaining_us == 0) {
        is_fading = 0;
        is_panning = 0;
    }
}

static void run_script(unsigned long long elapsed_us)
{
    if (hold_remaining_us > elapsed_us) {
        hold_remaining_us = hold_remaining_us - elapsed_us;
        update_hold_effects();
        return;
    }
    hold_remaining_us = 0;
    update_hold_effects();
    while (hold_remaining_us == 0 && next_step < current_scene->step_count) {
        run_step(&current_scene->steps[next_step]);
        next_step = next_step + 1;
    }
    if (hold_remaining_us == 0 && next_step >= current_scene->step_count) {
        status = SCENE_PLAYER_FINISHED;
        run_log_write("scene finished name=%s", current_scene->name);
    }
}

static void preload_step_assets(const struct scene_step *step)
{
    const struct animation *animation;

    switch (step->kind) {
    case SCENE_STEP_SIDE_BARS:
        texture_cache_request(step->first_name);
        texture_cache_request(step->second_name);
        break;
    case SCENE_STEP_PLATE:
    case SCENE_STEP_CARD:
        texture_cache_request(step->first_name);
        break;
    case SCENE_STEP_OBJECT:
        animation = animation_library_load(step->second_name);
        if (animation != NULL) {
            request_animation_textures(animation);
        }
        break;
    default:
        break;
    }
}

static void finish_preloading_when_ready(void)
{
    struct texture_cache_totals totals;

    if (texture_cache_loading_count() > 0) {
        return;
    }
    texture_cache_read_totals(&totals);
    run_log_write("scene ready name=%s preload_us=%llu texture_bytes=%u missing=%u", current_scene->name, timing_now_us() - preload_started_us, totals.bytes_in_use, totals.missing);
    status = SCENE_PLAYER_RUNNING;
}

void scene_player_init(const char *scene_folder)
{
    snprintf(folder, sizeof(folder), "%s", scene_folder);
    current_scene = NULL;
    memset(&picture, 0, sizeof(picture));
    status = SCENE_PLAYER_IDLE;
    next_step = 0;
    is_paused = 0;
    hold_remaining_us = 0;
    hold_total_us = 0;
    is_fading = 0;
    is_panning = 0;
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
    int step;

    scene = read_scene_file(scene_name);
    if (scene == NULL) {
        return 0;
    }
    scene_player_clear_room();
    mem_free(current_scene);
    current_scene = scene;
    picture.card_texture[0] = '\0';
    picture.fade_amount = 0.0f;
    next_step = 0;
    hold_remaining_us = 0;
    hold_total_us = 0;
    is_fading = 0;
    is_panning = 0;
    preload_started_us = timing_now_us();
    for (step = 0; step < scene->step_count; step++) {
        preload_step_assets(&scene->steps[step]);
    }
    status = SCENE_PLAYER_PRELOADING;
    run_log_write("scene started name=%s steps=%d", scene->name, scene->step_count);
    return 1;
}

static void advance_objects(unsigned long long elapsed_us)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (picture.objects[index].in_use) {
            animation_player_advance(&picture.objects[index].player, elapsed_us, frame_can_be_shown);
        }
    }
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
    advance_objects(elapsed_us);
    if (status == SCENE_PLAYER_RUNNING) {
        run_script(elapsed_us);
    }
}

void scene_player_stop_script(void)
{
    status = SCENE_PLAYER_IDLE;
    picture.fade_amount = 0.0f;
    picture.card_texture[0] = '\0';
    hold_remaining_us = 0;
    is_fading = 0;
    is_panning = 0;
}

void scene_player_set_paused(int should_pause)
{
    is_paused = should_pause;
}

int scene_player_is_paused(void)
{
    return is_paused;
}

enum scene_player_status scene_player_status(void)
{
    return status;
}

int scene_player_current_step(void)
{
    return next_step;
}

const struct scene_picture *scene_player_picture(void)
{
    return &picture;
}

void scene_player_log_coverage(void)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (picture.objects[index].in_use) {
            log_object_coverage(&picture.objects[index]);
        }
    }
}

void scene_player_shutdown(void)
{
    scene_player_log_coverage();
    mem_free(current_scene);
    current_scene = NULL;
    memset(&picture, 0, sizeof(picture));
    status = SCENE_PLAYER_IDLE;
}
