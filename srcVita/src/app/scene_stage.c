#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "run_log.h"
#include "scene_stage.h"
#include "texture_cache.h"

static struct scene_stage stage;

static int frame_can_be_shown(const struct animation_frame *frame)
{
    const struct cached_texture *texture;

    if (animation_frame_is_blank(frame)) {
        return 1;
    }
    texture = texture_cache_use_or_start_loading(frame->texture_name);
    return texture != NULL && texture->state != CACHED_TEXTURE_LOADING;
}

static struct scene_object *find_object(const char *name)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (stage.objects[index].in_use && strcmp(stage.objects[index].name, name) == 0) {
            return &stage.objects[index];
        }
    }
    return NULL;
}

static struct scene_object *find_free_object(void)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (!stage.objects[index].in_use) {
            return &stage.objects[index];
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

static unsigned int frames_passed_over(unsigned int from, unsigned int to, unsigned int frame_count)
{
    return (to + frame_count - from - 1) % frame_count;
}

void scene_stage_reset(void)
{
    memset(&stage, 0, sizeof(stage));
    scene_effects_reset(&stage.effects);
}

const struct scene_stage *scene_stage_view(void)
{
    return &stage;
}

struct scene_effects *scene_stage_effects(void)
{
    return &stage.effects;
}

struct scene_layers *scene_stage_layers(void)
{
    return &stage.layers;
}

void scene_stage_request_animation_textures(const struct animation *animation)
{
    unsigned int frame;

    for (frame = 0; frame < animation->frame_count; frame++) {
        if (!animation_frame_is_blank(&animation->frames[frame])) {
            texture_cache_use_or_start_loading(animation->frames[frame].texture_name);
        }
    }
}

int scene_stage_set_object(const char *object_name, const char *animation_name, float x, float y, int layer, int is_flipped)
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
    scene_stage_request_animation_textures(animation);
    animation_player_start(&object->player, animation);
    object->x = x;
    object->y = y;
    object->layer = layer;
    object->is_flipped = is_flipped;
    object->in_use = 1;
    return 1;
}

static void show_object_frame(struct scene_object *object, int frame_index)
{
    unsigned int previous_frame;

    previous_frame = object->player.frame_index;
    animation_player_show_frame(&object->player, (unsigned int)frame_index);
    if (object->player.frame_index != previous_frame) {
        run_log_write("frame_change object=%s from=%u to=%u passed_over=%u", object->name, previous_frame, object->player.frame_index, frames_passed_over(previous_frame, object->player.frame_index, object->player.animation->frame_count));
    }
}

void scene_stage_place_object(const char *object_name, const char *animation_name, float x, float y, int layer, int frame_index)
{
    struct scene_object *object;

    object = find_object(object_name);
    if (object == NULL || object->player.animation == NULL || strcmp(object->player.animation->name, animation_name) != 0) {
        if (!scene_stage_set_object(object_name, animation_name, x, y, layer, 0)) {
            return;
        }
        object = find_object(object_name);
    }
    object->x = x;
    object->y = y;
    object->layer = layer;
    show_object_frame(object, frame_index);
}

void scene_stage_set_object_flipped(const char *object_name, int is_flipped)
{
    struct scene_object *object;

    object = find_object(object_name);
    if (object != NULL) {
        object->is_flipped = is_flipped;
    }
}

void scene_stage_set_soul_mode(int is_on)
{
    stage.soul_mode_is_on = is_on;
}

void scene_stage_set_mono_mode(int is_on)
{
    stage.mono_mode_is_on = is_on;
}

void scene_stage_set_object_soul_tint(const char *object_name, float red, float green, float blue, float alpha, int is_flat)
{
    struct scene_object *object;

    object = find_object(object_name);
    if (object != NULL) {
        object->has_soul_tint = 1;
        object->soul_red = red;
        object->soul_green = green;
        object->soul_blue = blue;
        object->soul_alpha = alpha;
        object->soul_is_flat = is_flat;
    }
}

void scene_stage_set_last_plate_soul_texture(const char *texture_name)
{
    if (stage.plate_count > 0) {
        snprintf(stage.plates[stage.plate_count - 1].soul_texture_name, sizeof(stage.plates[stage.plate_count - 1].soul_texture_name), "%s", texture_name);
    }
}

void scene_stage_remove_object(const char *object_name)
{
    struct scene_object *object;

    object = find_object(object_name);
    if (object != NULL) {
        log_object_coverage(object);
        object->in_use = 0;
    }
}

void scene_stage_clear_room(void)
{
    int index;

    stage.plate_count = 0;
    scene_layers_clear_overlays(&stage.layers);
    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (stage.objects[index].in_use) {
            log_object_coverage(&stage.objects[index]);
            stage.objects[index].in_use = 0;
        }
    }
}

void scene_stage_clear_plates(void)
{
    stage.plate_count = 0;
}

void scene_stage_add_plate(const char *texture_name, float x, float y, float width, float height)
{
    struct scene_plate *plate;

    if (stage.plate_count >= SCENE_PLATE_MAX) {
        run_log_write("scene too_many_plates name=%s", texture_name);
        return;
    }
    plate = &stage.plates[stage.plate_count];
    snprintf(plate->texture_name, sizeof(plate->texture_name), "%s", texture_name);
    plate->x = x;
    plate->y = y;
    plate->width = width;
    plate->height = height;
    stage.plate_count = stage.plate_count + 1;
    texture_cache_use_or_start_loading(texture_name);
}

void scene_stage_set_camera(float x, float y)
{
    stage.camera_x = x;
    stage.camera_y = y;
}

void scene_stage_show_card(const char *texture_name)
{
    snprintf(stage.card_texture, sizeof(stage.card_texture), "%s", texture_name);
}

void scene_stage_hide_card(void)
{
    stage.card_texture[0] = '\0';
}

void scene_stage_set_fade(float fade_amount)
{
    stage.fade_amount = fade_amount;
}

static void advance_object(struct scene_object *object, unsigned long long elapsed_us)
{
    unsigned int previous_frame;

    previous_frame = object->player.frame_index;
    animation_player_advance(&object->player, elapsed_us, frame_can_be_shown);
    if (object->player.frame_index != previous_frame) {
        run_log_write("frame_change object=%s from=%u to=%u passed_over=%u", object->name, previous_frame, object->player.frame_index, frames_passed_over(previous_frame, object->player.frame_index, object->player.animation->frame_count));
    }
}

void scene_stage_advance_objects(unsigned long long elapsed_us)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (stage.objects[index].in_use) {
            advance_object(&stage.objects[index], elapsed_us);
        }
    }
}

void scene_stage_log_coverage(void)
{
    int index;

    for (index = 0; index < SCENE_OBJECT_MAX; index++) {
        if (stage.objects[index].in_use) {
            log_object_coverage(&stage.objects[index]);
        }
    }
}
