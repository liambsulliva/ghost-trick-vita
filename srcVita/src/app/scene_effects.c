#include <stdio.h>
#include <string.h>
#include "animation.h"
#include "run_log.h"
#include "scene_effects.h"

static void ramp_begin(struct scene_ramp *ramp, float target, int ticks)
{
    ramp->start = ramp->value;
    ramp->target = target;
    ramp->elapsed_us = 0;
    ramp->total_us = (unsigned long long)ticks * MICROSECONDS_PER_TICK;
    if (ramp->total_us == 0) {
        ramp->value = target;
    }
}

static void ramp_advance(struct scene_ramp *ramp, unsigned long long elapsed_us)
{
    if (ramp->total_us == 0) {
        return;
    }
    ramp->elapsed_us = ramp->elapsed_us + elapsed_us;
    if (ramp->elapsed_us >= ramp->total_us) {
        ramp->value = ramp->target;
        ramp->total_us = 0;
        return;
    }
    ramp->value = ramp->start + (ramp->target - ramp->start) * ((float)ramp->elapsed_us / (float)ramp->total_us);
}

static void ramp_set(struct scene_ramp *ramp, float value)
{
    ramp->value = value;
    ramp->total_us = 0;
}

static struct scene_beam *find_beam(struct scene_effects *effects, const char *name)
{
    int index;

    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (effects->beams[index].in_use && strcmp(effects->beams[index].name, name) == 0) {
            return &effects->beams[index];
        }
    }
    return NULL;
}

static struct scene_beam *find_free_beam(struct scene_effects *effects)
{
    int index;

    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (!effects->beams[index].in_use) {
            return &effects->beams[index];
        }
    }
    return NULL;
}

void scene_effects_reset(struct scene_effects *effects)
{
    memset(effects, 0, sizeof(*effects));
    ramp_set(&effects->brightness, 1.0f);
}

void scene_effects_advance(struct scene_effects *effects, unsigned long long elapsed_us)
{
    int index;

    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (effects->beams[index].in_use) {
            ramp_advance(&effects->beams[index].base_x, elapsed_us);
            ramp_advance(&effects->beams[index].base_y, elapsed_us);
            ramp_advance(&effects->beams[index].half_width, elapsed_us);
            ramp_advance(&effects->beams[index].cover_alpha, elapsed_us);
        }
    }
    ramp_advance(&effects->iris_x, elapsed_us);
    ramp_advance(&effects->iris_y, elapsed_us);
    ramp_advance(&effects->iris_radius, elapsed_us);
    ramp_advance(&effects->iris_alpha, elapsed_us);
    ramp_advance(&effects->brightness, elapsed_us);
    ramp_advance(&effects->flash_level, elapsed_us);
    ramp_advance(&effects->dissolve_progress, elapsed_us);
    if (effects->dissolve_progress.value >= 1.0f) {
        effects->dissolve_is_on = 0;
    }
}

void scene_effects_create_beam(struct scene_effects *effects, const char *name, float apex_x, float apex_y, float base_x, float base_y, float half_width)
{
    struct scene_beam *beam;

    beam = find_beam(effects, name);
    if (beam == NULL) {
        beam = find_free_beam(effects);
    }
    if (beam == NULL) {
        run_log_write("scene too_many_beams name=%s", name);
        return;
    }
    snprintf(beam->name, sizeof(beam->name), "%s", name);
    beam->apex_x = apex_x;
    beam->apex_y = apex_y;
    ramp_set(&beam->base_x, base_x);
    ramp_set(&beam->base_y, base_y);
    ramp_set(&beam->cover_alpha, 1.0f);
    ramp_set(&beam->half_width, half_width);
    beam->in_use = 1;
}

void scene_effects_animate_beam(struct scene_effects *effects, const char *name, float base_x, float base_y, float half_width, int ticks)
{
    struct scene_beam *beam;

    beam = find_beam(effects, name);
    if (beam == NULL) {
        run_log_write("scene beam_missing name=%s", name);
        return;
    }
    ramp_begin(&beam->base_x, base_x, ticks);
    ramp_begin(&beam->base_y, base_y, ticks);
    ramp_begin(&beam->half_width, half_width, ticks);
}

void scene_effects_ramp_beam_cover(struct scene_effects *effects, const char *name, float cover_alpha, int ticks)
{
    struct scene_beam *beam;

    beam = find_beam(effects, name);
    if (beam == NULL) {
        run_log_write("scene beam_missing name=%s", name);
        return;
    }
    ramp_begin(&beam->cover_alpha, cover_alpha, ticks);
}

void scene_effects_set_cover_mask(struct scene_effects *effects, int should_be_on, int is_opaque, float fade_alpha, int fade_is_white)
{
    effects->mask_is_on = should_be_on;
    effects->mask_uses_cover_alpha = 1;
    effects->mask_is_opaque = is_opaque;
    effects->mask_fade_alpha = fade_alpha;
    effects->mask_fade_is_white = fade_is_white;
}

float scene_effects_strongest_beam_cover(const struct scene_effects *effects)
{
    float strongest;
    int index;

    strongest = 0.0f;
    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (effects->beams[index].in_use && effects->beams[index].cover_alpha.value > strongest) {
            strongest = effects->beams[index].cover_alpha.value;
        }
    }
    return strongest;
}

void scene_effects_delete_beam(struct scene_effects *effects, const char *name)
{
    struct scene_beam *beam;

    beam = find_beam(effects, name);
    if (beam != NULL) {
        beam->in_use = 0;
    }
}

void scene_effects_set_mask(struct scene_effects *effects, int should_be_on)
{
    effects->mask_is_on = should_be_on;
    effects->iris_is_on = 0;
}

void scene_effects_stop_iris(struct scene_effects *effects)
{
    effects->iris_is_on = 0;
}

void scene_effects_set_flash(struct scene_effects *effects, float level)
{
    ramp_set(&effects->flash_level, level);
}

const struct scene_beam *scene_effects_find_beam(const struct scene_effects *effects, const char *name)
{
    int index;

    for (index = 0; index < SCENE_BEAM_MAX; index++) {
        if (effects->beams[index].in_use && strcmp(effects->beams[index].name, name) == 0) {
            return &effects->beams[index];
        }
    }
    return NULL;
}

void scene_effects_start_iris(struct scene_effects *effects, float x, float y, float radius, int ticks)
{
    if (!effects->iris_is_on) {
        ramp_set(&effects->iris_radius, 0.0f);
        ramp_set(&effects->iris_alpha, 1.0f);
    }
    effects->iris_is_on = 1;
    ramp_set(&effects->iris_x, x);
    ramp_set(&effects->iris_y, y);
    ramp_begin(&effects->iris_radius, radius, ticks);
}

void scene_effects_create_iris(struct scene_effects *effects, float x, float y, float radius, float alpha)
{
    effects->iris_is_on = 1;
    ramp_set(&effects->iris_x, x);
    ramp_set(&effects->iris_y, y);
    ramp_set(&effects->iris_radius, radius);
    ramp_set(&effects->iris_alpha, alpha);
}

void scene_effects_ramp_iris_radius(struct scene_effects *effects, float radius, int ticks)
{
    ramp_begin(&effects->iris_radius, radius, ticks);
}

void scene_effects_ramp_iris_alpha(struct scene_effects *effects, float alpha, int ticks)
{
    ramp_begin(&effects->iris_alpha, alpha, ticks);
}

void scene_effects_move_iris(struct scene_effects *effects, float x, float y, int ticks)
{
    ramp_begin(&effects->iris_x, x, ticks);
    ramp_begin(&effects->iris_y, y, ticks);
}

void scene_effects_dim_to(struct scene_effects *effects, float brightness, int ticks)
{
    ramp_begin(&effects->brightness, brightness, ticks);
}

void scene_effects_flash(struct scene_effects *effects, float brightness, int ticks)
{
    ramp_set(&effects->flash_level, brightness);
    ramp_begin(&effects->flash_level, 0.0f, ticks);
}

void scene_effects_start_dissolve(struct scene_effects *effects, float end_zoom, int ticks)
{
    effects->dissolve_is_on = 1;
    effects->dissolve_end_zoom = end_zoom;
    ramp_set(&effects->dissolve_progress, 0.0f);
    ramp_begin(&effects->dissolve_progress, 1.0f, ticks);
}
