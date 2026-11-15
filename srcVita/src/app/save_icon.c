#include <math.h>
#include <stddef.h>
#include "renderer.h"
#include "run_log.h"
#include "save_icon.h"
#include "texture_cache.h"

#define FADE_IN_US 250000ull
#define HOLD_US 1500000ull
#define FADE_OUT_US 500000ull
#define BLOB_TEXTURE "soul_blob"
#define BLOB_DISC_FRACTION 0.59f
#define EDGE_WOBBLE 0.6f
#define CORE_RADIUS 14.0f
#define CORE_LUMP_RADIUS 9.0f
#define CORE_LUMP_REACH 6.0f
#define TWO_PI 6.2831853f

static unsigned long long age_us;
static unsigned long long clock_us;
static int is_showing;

void save_icon_reset(void)
{
    age_us = 0;
    clock_us = 0;
    is_showing = 0;
}

void save_icon_show(void)
{
    if (!is_showing) {
        age_us = 0;
        run_log_write("save icon shown");
    } else if (age_us >= FADE_IN_US + HOLD_US) {
        age_us = (unsigned long long)((float)FADE_IN_US * save_icon_alpha());
    } else if (age_us >= FADE_IN_US) {
        age_us = FADE_IN_US;
    }
    is_showing = 1;
}

void save_icon_update(unsigned long long elapsed_us, int is_writing)
{
    clock_us = clock_us + elapsed_us;
    if (!is_showing) {
        return;
    }
    age_us = age_us + elapsed_us;
    if (is_writing && age_us > FADE_IN_US + HOLD_US) {
        age_us = FADE_IN_US + HOLD_US;
    }
    if (age_us >= FADE_IN_US + HOLD_US + FADE_OUT_US) {
        is_showing = 0;
    }
}

int save_icon_is_showing(void)
{
    return is_showing;
}

float save_icon_alpha(void)
{
    if (!is_showing) {
        return 0.0f;
    }
    if (age_us < FADE_IN_US) {
        return (float)age_us / (float)FADE_IN_US;
    }
    if (age_us <= FADE_IN_US + HOLD_US) {
        return 1.0f;
    }
    return 1.0f - (float)(age_us - FADE_IN_US - HOLD_US) / (float)FADE_OUT_US;
}

unsigned int save_icon_step(void)
{
    return (unsigned int)(clock_us / SAVE_ICON_STEP_US);
}

static float centre_x(void)
{
    return (float)RENDERER_WIDTH - SAVE_ICON_MARGIN - EDGE_WOBBLE - SOUL_BODY_HALF_WIDTH * SAVE_ICON_SCALE;
}

static float centre_y(void)
{
    return (float)RENDERER_HEIGHT - SAVE_ICON_MARGIN - EDGE_WOBBLE - SOUL_BODY_DROP * SAVE_ICON_SCALE;
}

static void wobble(struct soul_chain_blob *blob, unsigned int seed)
{
    blob->x = blob->x + (soul_seeded_fraction(seed, 40u) * 2.0f - 1.0f) * EDGE_WOBBLE;
    blob->y = blob->y + (soul_seeded_fraction(seed, 41u) * 2.0f - 1.0f) * EDGE_WOBBLE;
}

int save_icon_body(struct soul_chain_blob *blobs)
{
    unsigned int step;
    int count;
    int index;

    step = save_icon_step();
    count = soul_body_blobs(centre_x(), centre_y(), SAVE_ICON_SCALE, step, blobs);
    for (index = 0; index < count; index++) {
        wobble(&blobs[index], step * 29u + (unsigned int)index);
    }
    return count;
}

void save_icon_core(struct soul_chain_blob *blobs)
{
    unsigned int step;
    int index;

    step = save_icon_step();
    blobs[0].x = centre_x();
    blobs[0].y = centre_y();
    blobs[0].radius = CORE_RADIUS * SAVE_ICON_SCALE;
    for (index = 1; index < SAVE_ICON_CORE_BLOBS; index++) {
        float angle;

        angle = ((float)index + soul_seeded_fraction(step, 50u + (unsigned int)index)) * TWO_PI / (float)(SAVE_ICON_CORE_BLOBS - 1);
        blobs[index].x = centre_x() + cosf(angle) * CORE_LUMP_REACH * SAVE_ICON_SCALE;
        blobs[index].y = centre_y() + sinf(angle) * CORE_LUMP_REACH * SAVE_ICON_SCALE;
        blobs[index].radius = CORE_LUMP_RADIUS * SAVE_ICON_SCALE;
    }
}

static struct renderer_rect blob_area(const struct soul_chain_blob *blob)
{
    struct renderer_rect area;
    float size;

    size = 2.0f * blob->radius / BLOB_DISC_FRACTION;
    area.x = blob->x - size / 2.0f;
    area.y = blob->y - size / 2.0f;
    area.width = size;
    area.height = size;
    return area;
}

void save_icon_draw(void)
{
    static const struct renderer_rect whole = { 0.0f, 0.0f, 1.0f, 1.0f };
    struct soul_chain_blob body[SOUL_BODY_BLOBS];
    struct soul_chain_blob core[SAVE_ICON_CORE_BLOBS];
    struct renderer_color flame = { 1.0f, 1.0f, 1.0f, 1.0f };
    struct renderer_color orb = { 0.73f, 0.74f, 0.94f, 1.0f };
    const struct cached_texture *texture;
    int count;
    int index;

    if (!is_showing) {
        return;
    }
    texture = texture_cache_use_or_start_loading(BLOB_TEXTURE);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    flame.alpha = save_icon_alpha();
    orb.alpha = flame.alpha;
    count = save_icon_body(body);
    for (index = 0; index < count; index++) {
        renderer_draw_sprite(texture->texture, blob_area(&body[index]), whole, flame);
    }
    save_icon_core(core);
    for (index = 0; index < SAVE_ICON_CORE_BLOBS; index++) {
        renderer_draw_flat_sprite(texture->texture, blob_area(&core[index]), whole, orb);
    }
}
