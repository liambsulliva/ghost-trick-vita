#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "run_log.h"
#include "scene_layers.h"

static unsigned long long ticks_to_us(int ticks)
{
    return (unsigned long long)ticks * MICROSECONDS_PER_TICK;
}

static struct scene_text_line *find_text_line(struct scene_layers *layers, int id)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (layers->text_lines[index].in_use && layers->text_lines[index].id == id) {
            return &layers->text_lines[index];
        }
    }
    return NULL;
}

static struct scene_text_line *find_free_text_line(struct scene_layers *layers)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (!layers->text_lines[index].in_use) {
            return &layers->text_lines[index];
        }
    }
    return NULL;
}

static struct scene_overlay *find_overlay(struct scene_layers *layers, int id)
{
    int index;

    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        if (layers->overlays[index].in_use && layers->overlays[index].id == id) {
            return &layers->overlays[index];
        }
    }
    return NULL;
}

static struct scene_overlay *find_free_overlay(struct scene_layers *layers)
{
    int index;

    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        if (!layers->overlays[index].in_use) {
            return &layers->overlays[index];
        }
    }
    return NULL;
}

void scene_layers_clear(struct scene_layers *layers)
{
    memset(layers, 0, sizeof(*layers));
}

void scene_layers_clear_overlays(struct scene_layers *layers)
{
    memset(layers->overlays, 0, sizeof(layers->overlays));
}

static void start_fade(struct scene_text_line *line, float target_alpha, int fade_ticks)
{
    line->fade_start_alpha = line->alpha;
    line->fade_target_alpha = target_alpha;
    line->fade_total_us = ticks_to_us(fade_ticks);
    line->fade_elapsed_us = 0;
}

void scene_layers_show_text(struct scene_layers *layers, int id, const char *texture_name, float center_x, float top_y, int fade_ticks)
{
    struct scene_text_line *line;

    line = find_text_line(layers, id);
    if (line == NULL) {
        line = find_free_text_line(layers);
    }
    if (line == NULL) {
        run_log_write("scene too_many_text_lines name=%s", texture_name);
        return;
    }
    snprintf(line->texture_name, sizeof(line->texture_name), "%s", texture_name);
    line->center_x = center_x;
    line->top_y = top_y;
    line->id = id;
    line->in_use = 1;
    line->alpha = fade_ticks > 0 ? 0.0f : 1.0f;
    start_fade(line, 1.0f, fade_ticks);
}

void scene_layers_hide_text(struct scene_layers *layers, int id, int fade_ticks)
{
    struct scene_text_line *line;

    line = find_text_line(layers, id);
    if (line == NULL) {
        return;
    }
    if (fade_ticks <= 0) {
        line->in_use = 0;
        return;
    }
    start_fade(line, 0.0f, fade_ticks);
}

void scene_layers_hide_all_text(struct scene_layers *layers)
{
    memset(layers->text_lines, 0, sizeof(layers->text_lines));
}

void scene_layers_show_overlay(struct scene_layers *layers, int id, const char *texture_name, float x, float y, float width, float height, int grow_ticks)
{
    struct scene_overlay *overlay;

    overlay = find_overlay(layers, id);
    if (overlay == NULL) {
        overlay = find_free_overlay(layers);
    }
    if (overlay == NULL) {
        run_log_write("scene too_many_overlays name=%s", texture_name);
        return;
    }
    snprintf(overlay->texture_name, sizeof(overlay->texture_name), "%s", texture_name);
    overlay->x = x;
    overlay->y = y;
    overlay->width = width;
    overlay->height = height;
    overlay->id = id;
    overlay->in_use = 1;
    overlay->grow_total_us = ticks_to_us(grow_ticks);
    overlay->grow_elapsed_us = 0;
    overlay->size_fraction = grow_ticks > 0 ? 0.0f : 1.0f;
}

void scene_layers_hide_overlay(struct scene_layers *layers, int id)
{
    struct scene_overlay *overlay;

    overlay = find_overlay(layers, id);
    if (overlay != NULL) {
        overlay->in_use = 0;
    }
}

static void advance_text_line(struct scene_text_line *line, unsigned long long elapsed_us)
{
    float progress;

    if (line->fade_elapsed_us >= line->fade_total_us) {
        return;
    }
    line->fade_elapsed_us = line->fade_elapsed_us + elapsed_us;
    if (line->fade_elapsed_us >= line->fade_total_us) {
        line->alpha = line->fade_target_alpha;
        line->in_use = line->fade_target_alpha > 0.0f;
        return;
    }
    progress = (float)line->fade_elapsed_us / (float)line->fade_total_us;
    line->alpha = line->fade_start_alpha + (line->fade_target_alpha - line->fade_start_alpha) * progress;
}

static void advance_overlay(struct scene_overlay *overlay, unsigned long long elapsed_us)
{
    if (overlay->grow_elapsed_us >= overlay->grow_total_us) {
        return;
    }
    overlay->grow_elapsed_us = overlay->grow_elapsed_us + elapsed_us;
    if (overlay->grow_elapsed_us >= overlay->grow_total_us) {
        overlay->size_fraction = 1.0f;
        return;
    }
    overlay->size_fraction = (float)overlay->grow_elapsed_us / (float)overlay->grow_total_us;
}

int scene_layers_text_is_fading_out(const struct scene_layers *layers)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (layers->text_lines[index].in_use && layers->text_lines[index].fade_target_alpha <= 0.0f) {
            return 1;
        }
    }
    return 0;
}

void scene_layers_advance(struct scene_layers *layers, unsigned long long elapsed_us)
{
    int index;

    for (index = 0; index < SCENE_TEXT_LINE_MAX; index++) {
        if (layers->text_lines[index].in_use) {
            advance_text_line(&layers->text_lines[index], elapsed_us);
        }
    }
    for (index = 0; index < SCENE_OVERLAY_MAX; index++) {
        if (layers->overlays[index].in_use) {
            advance_overlay(&layers->overlays[index], elapsed_us);
        }
    }
}
