#include <math.h>
#include <stdlib.h>
#include "renderer.h"
#include "soul_transition.h"
#include "texture_cache.h"

#define SPOKE_TEXTURE "soul_spokes"
#define SPOKE_COUNT 32
#define SPOKE_RING_RADIUS 569.5f
#define SPOKE_ROW_STRETCH 1.25f
#define SPOKE_RAMP_ROWS 64.0f
#define SPOKE_FIRST_ROW 0.5f
#define SPOKE_TEXTURE_MIDDLE_U 0.5f
#define DEGREES_PER_SPOKE 11.25f
#define DEGREES_TO_RADIANS 0.017453293f
#define SCREEN_CENTER_X 480.0f
#define SCREEN_CENTER_Y 272.0f
#define LAST_SPOKE_TICK 12
#define SNAPSHOT_TICK 11
#define TICK_BEFORE_FIRST_SPUN_LAYER 13
#define LAST_SPUN_LAYER_TICK 32
#define SPUN_LAYER_COUNT 4
#define SPUN_LAYER_START_ALPHA 124
#define SPUN_LAYER_ALPHA_PER_STEP 6
#define SPUN_LAYER_OUTER_ALPHA_MOST 8
#define SPUN_LAYER_ALPHA_SCALE 31.0f
#define SPUN_LAYER_GROWTH_PER_STEP (3.0f / 128.0f)
#define SPUN_LAYER_DEGREES_PER_STEP 0.824f
#define LEAVING_BLACK_LAST_TICK 2
#define LEAVING_FIRST_SPOKE_TICK 3
#define SWEEP_NATIVE_Y_START 212
#define SWEEP_NATIVE_Y_PER_TICK 2
#define SWEEP_NATIVE_Y_LOWEST -159
#define SWEEP_NATIVE_Y_WRAP 159
#define VITA_PIXELS_PER_DS_PIXEL 2.8125f
#define SWEEP_FOOTAGE_NUDGE 1.875f
#define SWEEP_HEIGHT 2.8f
#define FINE_LINE_BRIGHT_CENTER 0.3125f
#define FINE_LINE_SAMPLES_PER_ROW 16
#define FINE_LINE_ALPHA 41
#define FINE_LINE_DIM_CHANNEL 24
#define FINE_LINE_BRIGHT_RED 255

enum transition_kind {
    TRANSITION_NONE,
    TRANSITION_ENTERING,
    TRANSITION_LEAVING
};

static enum transition_kind kind;
static int tick;
static int snapshot_is_ready;
static int sweep_native_y = SWEEP_NATIVE_Y_START;
static renderer_texture fine_lines_texture;

void soul_transition_begin_entering(void)
{
    kind = TRANSITION_ENTERING;
    tick = 0;
    snapshot_is_ready = 0;
    soul_transition_restart_sweep();
}

void soul_transition_begin_leaving(void)
{
    kind = TRANSITION_LEAVING;
    tick = 0;
    snapshot_is_ready = 0;
}

void soul_transition_stop(void)
{
    kind = TRANSITION_NONE;
    tick = 0;
}

void soul_transition_restart_sweep(void)
{
    sweep_native_y = SWEEP_NATIVE_Y_START;
}

void soul_transition_advance(void)
{
    (void)texture_cache_use_or_start_loading(SPOKE_TEXTURE);
    sweep_native_y = sweep_native_y - SWEEP_NATIVE_Y_PER_TICK;
    if (sweep_native_y < SWEEP_NATIVE_Y_LOWEST) {
        sweep_native_y = SWEEP_NATIVE_Y_WRAP;
    }
    if (kind == TRANSITION_NONE) {
        return;
    }
    tick = tick + 1;
    if ((kind == TRANSITION_ENTERING && tick > LAST_SPUN_LAYER_TICK) || (kind == TRANSITION_LEAVING && tick > LAST_SPOKE_TICK)) {
        soul_transition_stop();
    }
}

int soul_transition_wants_snapshot(void)
{
    return kind == TRANSITION_ENTERING && tick >= SNAPSHOT_TICK && !snapshot_is_ready;
}

void soul_transition_snapshot_taken(void)
{
    snapshot_is_ready = 1;
}

static struct renderer_vertex spoke_vertex(float x, float y, float row)
{
    struct renderer_vertex vertex;

    vertex.x = x;
    vertex.y = y;
    vertex.u = SPOKE_TEXTURE_MIDDLE_U;
    vertex.v = row / SPOKE_RAMP_ROWS;
    return vertex;
}

static struct renderer_vertex ring_vertex(int spoke, float row)
{
    float radians;

    radians = (float)spoke * DEGREES_PER_SPOKE * DEGREES_TO_RADIANS;
    return spoke_vertex(SCREEN_CENTER_X + SPOKE_RING_RADIUS * cosf(radians), SCREEN_CENTER_Y - SPOKE_RING_RADIUS * sinf(radians), row);
}

static void draw_spokes(int step, struct renderer_color tint)
{
    const struct cached_texture *texture;
    struct renderer_vertex corners[SPOKE_COUNT * 3];
    float outer_row;
    float inner_row;
    int spoke;

    texture = texture_cache_use_or_start_loading(SPOKE_TEXTURE);
    if (texture == NULL || texture->state != CACHED_TEXTURE_READY) {
        return;
    }
    outer_row = (float)(2 * (step - 1)) - 0.5f;
    if (outer_row < SPOKE_FIRST_ROW) {
        outer_row = SPOKE_FIRST_ROW;
    }
    inner_row = (float)(3 * step) - 1.5f;
    for (spoke = 0; spoke < SPOKE_COUNT; spoke++) {
        float near_row;
        float far_row;
        struct renderer_vertex *triangle;

        near_row = spoke % 2 == 0 ? inner_row : outer_row;
        far_row = spoke % 2 == 0 ? outer_row : inner_row;
        triangle = &corners[spoke * 3];
        triangle[0] = ring_vertex(spoke, near_row + SPOKE_ROW_STRETCH * (far_row - near_row));
        triangle[1] = spoke_vertex(SCREEN_CENTER_X, SCREEN_CENTER_Y, near_row);
        triangle[2] = ring_vertex(spoke + 1, near_row);
    }
    renderer_draw_triangles(texture->texture, corners, SPOKE_COUNT, tint);
}

static void draw_spun_layers(int step)
{
    int whole_alpha;
    int layer;

    whole_alpha = (SPUN_LAYER_START_ALPHA - SPUN_LAYER_ALPHA_PER_STEP * step) >> 2;
    for (layer = SPUN_LAYER_COUNT - 1; layer >= 0; layer--) {
        float growth;
        int alpha;

        alpha = whole_alpha;
        if (layer > 0 && alpha > SPUN_LAYER_OUTER_ALPHA_MOST) {
            alpha = SPUN_LAYER_OUTER_ALPHA_MOST;
        }
        growth = 1.0f + (float)((layer + 1) * step) * SPUN_LAYER_GROWTH_PER_STEP;
        renderer_draw_turned_snapshot(SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_CENTER_X * growth, SCREEN_CENTER_Y * growth, (float)((layer + 1) * step) * SPUN_LAYER_DEGREES_PER_STEP, (float)alpha / SPUN_LAYER_ALPHA_SCALE);
    }
}

void soul_transition_draw(void)
{
    static const struct renderer_color red = { 1.0f, 0.0f, 0.0f, 1.0f };
    static const struct renderer_color black = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (kind == TRANSITION_ENTERING) {
        if (tick >= 1 && tick <= LAST_SPOKE_TICK) {
            draw_spokes(tick, red);
        } else if (tick > TICK_BEFORE_FIRST_SPUN_LAYER && snapshot_is_ready) {
            draw_spun_layers(tick - TICK_BEFORE_FIRST_SPUN_LAYER);
        }
    } else if (kind == TRANSITION_LEAVING) {
        if (tick >= 1 && tick <= LEAVING_BLACK_LAST_TICK) {
            struct renderer_rect whole_screen;

            whole_screen.x = 0.0f;
            whole_screen.y = 0.0f;
            whole_screen.width = (float)RENDERER_WIDTH;
            whole_screen.height = (float)RENDERER_HEIGHT;
            renderer_draw_filled_rect(whole_screen, black);
        } else if (tick >= LEAVING_FIRST_SPOKE_TICK && tick <= LAST_SPOKE_TICK) {
            draw_spokes(tick, black);
        }
    }
}

static float bright_share_of_row(int row)
{
    int sample;
    int bright_samples;

    bright_samples = 0;
    for (sample = 0; sample < FINE_LINE_SAMPLES_PER_ROW; sample++) {
        float offset;

        offset = fmodf((float)row + ((float)sample + 0.5f) / (float)FINE_LINE_SAMPLES_PER_ROW - FINE_LINE_BRIGHT_CENTER + VITA_PIXELS_PER_DS_PIXEL / 4.0f + VITA_PIXELS_PER_DS_PIXEL, VITA_PIXELS_PER_DS_PIXEL);
        if (offset < VITA_PIXELS_PER_DS_PIXEL / 2.0f) {
            bright_samples = bright_samples + 1;
        }
    }
    return (float)bright_samples / (float)FINE_LINE_SAMPLES_PER_ROW;
}

static void make_fine_lines_texture(void)
{
    static unsigned char rows[RENDERER_HEIGHT * 4];
    int row;

    for (row = 0; row < RENDERER_HEIGHT; row++) {
        rows[row * 4] = (unsigned char)(bright_share_of_row(row) * (float)FINE_LINE_BRIGHT_RED);
        rows[row * 4 + 1] = FINE_LINE_DIM_CHANNEL;
        rows[row * 4 + 2] = FINE_LINE_DIM_CHANNEL;
        rows[row * 4 + 3] = FINE_LINE_ALPHA;
    }
    fine_lines_texture = renderer_texture_create(rows, 1, RENDERER_HEIGHT);
}

void soul_transition_draw_ghost_lines(void)
{
    static const struct renderer_color white = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const struct renderer_color pure_red = { 1.0f, 0.0f, 0.0f, 1.0f };
    struct renderer_rect area;
    struct renderer_rect whole_texture;

    if (fine_lines_texture == RENDERER_NO_TEXTURE) {
        make_fine_lines_texture();
    }
    area.x = 0.0f;
    area.y = 0.0f;
    area.width = (float)RENDERER_WIDTH;
    area.height = (float)RENDERER_HEIGHT;
    whole_texture.x = 0.0f;
    whole_texture.y = 0.0f;
    whole_texture.width = 1.0f;
    whole_texture.height = 1.0f;
    renderer_draw_sprite(fine_lines_texture, area, whole_texture, white);
    area.y = SCREEN_CENTER_Y - VITA_PIXELS_PER_DS_PIXEL * (float)sweep_native_y - SWEEP_FOOTAGE_NUDGE - SWEEP_HEIGHT / 2.0f;
    area.height = SWEEP_HEIGHT;
    renderer_draw_filled_rect(area, pure_red);
}
