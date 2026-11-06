#include <stdio.h>
#include <string.h>
#include "scene.h"
#include "text_lines.h"

#define SCENE_LINE_MAX 320

static int brightness_is_valid(float brightness)
{
    return brightness >= 0.0f && brightness <= 1.0f;
}

static int read_effect_step(const char *line, struct scene_step *step)
{
    if (text_line_starts_with(line, "beam_create ")) {
        step->kind = SCENE_STEP_BEAM_CREATE;
        return sscanf(line, "beam_create " ASSET_NAME_SCAN " %f %f %f %f %f", step->beam_name, &step->apex_x, &step->apex_y, &step->x, &step->y, &step->half_width) == 6 && step->half_width >= 0.0f;
    }
    if (text_line_starts_with(line, "beam_animate ")) {
        step->kind = SCENE_STEP_BEAM_ANIMATE;
        return sscanf(line, "beam_animate " ASSET_NAME_SCAN " %f %f %f %d", step->beam_name, &step->x, &step->y, &step->half_width, &step->ticks) == 5 && step->half_width >= 0.0f && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "beam_delete ")) {
        step->kind = SCENE_STEP_BEAM_DELETE;
        return sscanf(line, "beam_delete " ASSET_NAME_SCAN, step->beam_name) == 1;
    }
    if (strcmp(line, "mask_on") == 0) {
        step->kind = SCENE_STEP_MASK_ON;
        return 1;
    }
    if (strcmp(line, "mask_off") == 0) {
        step->kind = SCENE_STEP_MASK_OFF;
        return 1;
    }
    if (text_line_starts_with(line, "iris ")) {
        step->kind = SCENE_STEP_IRIS;
        return sscanf(line, "iris %f %f %f %d", &step->x, &step->y, &step->radius, &step->ticks) == 4 && step->radius >= 0.0f && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "dim_to ")) {
        step->kind = SCENE_STEP_DIM_TO;
        return sscanf(line, "dim_to %f %d", &step->brightness, &step->ticks) == 2 && brightness_is_valid(step->brightness) && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "flash ")) {
        step->kind = SCENE_STEP_FLASH;
        return sscanf(line, "flash %f %d", &step->brightness, &step->ticks) == 2 && brightness_is_valid(step->brightness) && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "dissolve ")) {
        step->kind = SCENE_STEP_DISSOLVE;
        return sscanf(line, "dissolve %d %f", &step->ticks, &step->end_zoom) == 2 && step->ticks > 0 && step->end_zoom > 0.0f;
    }
    return 0;
}

static int read_step(const char *line, struct scene_step *step)
{
    if (text_line_starts_with(line, "room ")) {
        step->kind = SCENE_STEP_ROOM;
        return sscanf(line, "room %d", &step->room_id) == 1;
    }
    if (text_line_starts_with(line, "plate ")) {
        step->kind = SCENE_STEP_PLATE;
        return sscanf(line, "plate " ASSET_NAME_SCAN " %f %f %f %f", step->texture_name, &step->x, &step->y, &step->width, &step->height) == 5;
    }
    if (text_line_starts_with(line, "object ")) {
        step->kind = SCENE_STEP_OBJECT;
        return sscanf(line, "object " ASSET_NAME_SCAN " " ASSET_NAME_SCAN " %f %f %d %d", step->object_name, step->animation_name, &step->x, &step->y, &step->layer, &step->is_flipped) == 6;
    }
    if (text_line_starts_with(line, "remove_object ")) {
        step->kind = SCENE_STEP_REMOVE_OBJECT;
        return sscanf(line, "remove_object " ASSET_NAME_SCAN, step->object_name) == 1;
    }
    if (text_line_starts_with(line, "camera_pan ")) {
        step->kind = SCENE_STEP_CAMERA_PAN;
        return sscanf(line, "camera_pan %f %f %d", &step->x, &step->y, &step->ticks) == 3 && step->ticks > 0;
    }
    if (text_line_starts_with(line, "camera ")) {
        step->kind = SCENE_STEP_CAMERA;
        return sscanf(line, "camera %f %f", &step->x, &step->y) == 2;
    }
    if (text_line_starts_with(line, "card ")) {
        step->kind = SCENE_STEP_CARD;
        return sscanf(line, "card " ASSET_NAME_SCAN, step->texture_name) == 1;
    }
    if (strcmp(line, "hide_card") == 0) {
        step->kind = SCENE_STEP_HIDE_CARD;
        return 1;
    }
    if (text_line_starts_with(line, "text_show ")) {
        step->kind = SCENE_STEP_TEXT_SHOW;
        return sscanf(line, "text_show %d " ASSET_NAME_SCAN " %f %f %d", &step->layer_item_id, step->texture_name, &step->x, &step->y, &step->ticks) == 5 && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "text_hide ")) {
        step->kind = SCENE_STEP_TEXT_HIDE;
        return sscanf(line, "text_hide %d %d", &step->layer_item_id, &step->ticks) == 2 && step->ticks >= 0;
    }
    if (strcmp(line, "text_hide_all") == 0) {
        step->kind = SCENE_STEP_TEXT_HIDE_ALL;
        return 1;
    }
    if (text_line_starts_with(line, "overlay ")) {
        step->kind = SCENE_STEP_OVERLAY;
        return sscanf(line, "overlay %d " ASSET_NAME_SCAN " %f %f %f %f %d", &step->layer_item_id, step->texture_name, &step->x, &step->y, &step->width, &step->height, &step->ticks) == 7 && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "overlay_hide ")) {
        step->kind = SCENE_STEP_OVERLAY_HIDE;
        return sscanf(line, "overlay_hide %d", &step->layer_item_id) == 1;
    }
    if (text_line_starts_with(line, "fade_out ")) {
        step->kind = SCENE_STEP_FADE_OUT;
        return sscanf(line, "fade_out %d", &step->ticks) == 1 && step->ticks > 0;
    }
    if (text_line_starts_with(line, "fade_in ")) {
        step->kind = SCENE_STEP_FADE_IN;
        return sscanf(line, "fade_in %d", &step->ticks) == 1 && step->ticks > 0;
    }
    if (text_line_starts_with(line, "wait ")) {
        step->kind = SCENE_STEP_WAIT;
        return sscanf(line, "wait %d", &step->ticks) == 1 && step->ticks > 0;
    }
    if (text_line_starts_with(line, "music ")) {
        step->kind = SCENE_STEP_MUSIC;
        return sscanf(line, "music " ASSET_NAME_SCAN " %d", step->audio_name, &step->is_looping) == 2 && (step->is_looping == 0 || step->is_looping == 1);
    }
    if (text_line_starts_with(line, "music_stop ")) {
        step->kind = SCENE_STEP_MUSIC_STOP;
        return sscanf(line, "music_stop %d", &step->ticks) == 1 && step->ticks >= 0;
    }
    if (text_line_starts_with(line, "sound ")) {
        step->kind = SCENE_STEP_SOUND;
        return sscanf(line, "sound " ASSET_NAME_SCAN, step->audio_name) == 1;
    }
    return read_effect_step(line, step);
}

int scene_parse(const char *text, unsigned int text_size, struct scene *scene)
{
    char line[SCENE_LINE_MAX];
    struct scene_step *step;
    unsigned int offset;

    scene->name[0] = '\0';
    scene->step_count = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        if (line[0] == '\0') {
            continue;
        }
        if (scene->name[0] == '\0') {
            if (!text_line_starts_with(line, "scene ") || sscanf(line, "scene " ASSET_NAME_SCAN, scene->name) != 1) {
                return SCENE_ERR_NO_NAME;
            }
            continue;
        }
        if (scene->step_count >= SCENE_STEP_MAX) {
            return SCENE_ERR_TOO_MANY_STEPS;
        }
        step = &scene->steps[scene->step_count];
        memset(step, 0, sizeof(*step));
        if (!read_step(line, step)) {
            return SCENE_ERR_BAD_LINE;
        }
        scene->step_count = scene->step_count + 1;
    }
    if (scene->name[0] == '\0') {
        return SCENE_ERR_NO_NAME;
    }
    if (scene->step_count == 0) {
        return SCENE_ERR_EMPTY;
    }
    return SCENE_OK;
}
