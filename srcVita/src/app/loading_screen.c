#include <stddef.h>
#include "loading_screen.h"
#include "pause_widgets.h"
#include "renderer.h"
#include "texture_cache.h"

#define READING_SHARE 0.2f
#define ANIMATIONS_SHARE 0.3f
#define TEXTURES_SHARE 0.5f
#define BACKDROP "pause_backdrop"
#define FLOOR_HEIGHT PAUSE_BAR_HEIGHT
#define FLOOR_TOP PAUSE_BAR_TOP
#define TEXT_LEFT 28.0f
#define TRACK_LEFT 300.0f
#define TRACK_RIGHT_MARGIN 28.0f
#define TRACK_HEIGHT 16.0f
#define FILL_INSET 3.0f
#define FADE_IN_US 166666ull
#define FILL_CATCH_UP_PER_SECOND 1.5f
#define LOADING_TEXT "bar.loading"
#define TRACK_SLICE "loading_track"
#define FILL_SLICE "loading_fill"
#define WARM_UP_SIZE 1.0f

static float shown_progress;
static float target_progress;
static unsigned long long shown_us;

static float share_of(int done, int total)
{
    if (total <= 0) {
        return 0.0f;
    }
    if (done >= total) {
        return 1.0f;
    }
    return done <= 0 ? 0.0f : (float)done / (float)total;
}

float loading_screen_progress(const struct loading_counts *counts)
{
    switch (counts->stage) {
    case LOADING_READING:
        return READING_SHARE * share_of(counts->done, counts->total);
    case LOADING_ANIMATIONS:
        return READING_SHARE + ANIMATIONS_SHARE * share_of(counts->done, counts->total);
    case LOADING_TEXTURES:
        return READING_SHARE + ANIMATIONS_SHARE + TEXTURES_SHARE * share_of(counts->done, counts->total);
    default:
        return 1.0f;
    }
}

void loading_screen_begin(void)
{
    shown_progress = 0.0f;
    target_progress = 0.0f;
    shown_us = 0;
    texture_cache_use_or_start_loading(BACKDROP);
    texture_cache_use_or_start_loading(PAUSE_WIDGET_SHEET);
}

void loading_screen_update(const struct loading_counts *counts, unsigned long long elapsed_us)
{
    float progress;
    float step;

    shown_us = shown_us + elapsed_us;
    progress = loading_screen_progress(counts);
    if (progress > target_progress) {
        target_progress = progress;
    }
    step = FILL_CATCH_UP_PER_SECOND * (float)elapsed_us / 1000000.0f;
    shown_progress = shown_progress + step < target_progress ? shown_progress + step : target_progress;
}

float loading_screen_shown_progress(void)
{
    return shown_progress;
}

static struct renderer_rect area(float x, float y, float width, float height)
{
    struct renderer_rect rect;

    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    return rect;
}

static struct renderer_color color(float red, float green, float blue, float alpha)
{
    struct renderer_color tint;

    tint.red = red;
    tint.green = green;
    tint.blue = blue;
    tint.alpha = alpha;
    return tint;
}

void loading_screen_draw(void)
{
    const struct cached_texture *backdrop;
    float alpha;
    float track_width;
    float fill_width;
    float track_top;

    alpha = shown_us >= FADE_IN_US ? 1.0f : (float)shown_us / (float)FADE_IN_US;
    renderer_draw_filled_rect(area(0.0f, 0.0f, (float)RENDERER_WIDTH, (float)RENDERER_HEIGHT), color(0.0f, 0.0f, 0.0f, 1.0f));
    backdrop = texture_cache_use_or_start_loading(BACKDROP);
    if (backdrop != NULL) {
        pause_draw_from(BACKDROP, 0.0f, 0.0f, (float)backdrop->width, (float)backdrop->height, 0.0f, 0.0f, (float)RENDERER_WIDTH, (float)RENDERER_HEIGHT, alpha);
    }
    pause_draw_floor(alpha);
    pause_draw_text(LOADING_TEXT, TEXT_LEFT, FLOOR_TOP + FLOOR_HEIGHT / 2.0f, PAUSE_ANCHOR_LEFT, alpha);
    track_width = (float)RENDERER_WIDTH - TRACK_RIGHT_MARGIN - TRACK_LEFT;
    track_top = FLOOR_TOP + (FLOOR_HEIGHT - TRACK_HEIGHT) / 2.0f;
    pause_draw_slice(TRACK_SLICE, TRACK_LEFT, track_top, track_width, TRACK_HEIGHT, alpha);
    fill_width = (track_width - 2.0f * FILL_INSET) * shown_progress;
    if (fill_width >= TRACK_HEIGHT - 2.0f * FILL_INSET) {
        pause_draw_slice(FILL_SLICE, TRACK_LEFT + FILL_INSET, track_top + FILL_INSET, fill_width, TRACK_HEIGHT - 2.0f * FILL_INSET, alpha);
    }
}

void loading_screen_warm_up_the_renderer(void)
{
    const struct cached_texture *texture;
    struct renderer_vertex corners[3];
    struct renderer_color hidden;
    int index;

    texture = texture_cache_use_or_start_loading(BACKDROP);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    hidden = color(1.0f, 1.0f, 1.0f, 0.0f);
    for (index = 0; index < 3; index++) {
        corners[index].x = (float)index * WARM_UP_SIZE;
        corners[index].y = (float)(index % 2) * WARM_UP_SIZE;
        corners[index].u = 0.0f;
        corners[index].v = 0.0f;
    }
    renderer_draw_sprite(texture->texture, area(0.0f, 0.0f, WARM_UP_SIZE, WARM_UP_SIZE), area(0.0f, 0.0f, 1.0f, 1.0f), hidden);
    renderer_draw_flat_sprite(texture->texture, area(0.0f, 0.0f, WARM_UP_SIZE, WARM_UP_SIZE), area(0.0f, 0.0f, 1.0f, 1.0f), hidden);
    renderer_draw_beam(texture->texture, 0.0f, 0.0f, WARM_UP_SIZE, WARM_UP_SIZE, WARM_UP_SIZE, hidden);
    renderer_draw_triangles(texture->texture, corners, 1, hidden);
    renderer_draw_filled_rects(&(struct renderer_rect){ 0.0f, 0.0f, WARM_UP_SIZE, WARM_UP_SIZE }, 1, hidden);
}
