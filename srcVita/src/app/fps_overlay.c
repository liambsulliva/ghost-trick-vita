#include <stdio.h>
#include <string.h>
#include "debug_font.h"
#include "fps_overlay.h"
#include "renderer.h"

#define MICROSECONDS_PER_SECOND 1000000.0f
#define MICROSECONDS_PER_MILLISECOND 1000.0f
#define OVERLAY_TEXT_MAX 48
#define OVERLAY_X 6.0f
#define OVERLAY_Y 6.0f
#define OVERLAY_SCALE 2.0f
#define OVERLAY_PADDING 3.0f

static unsigned long long frame_times_us[FPS_OVERLAY_FRAMES];
static int frame_count;
static int next_slot;

void fps_overlay_reset(void)
{
    frame_count = 0;
    next_slot = 0;
}

void fps_overlay_note_frame(unsigned long long frame_us)
{
    frame_times_us[next_slot] = frame_us;
    next_slot = (next_slot + 1) % FPS_OVERLAY_FRAMES;
    if (frame_count < FPS_OVERLAY_FRAMES) {
        frame_count = frame_count + 1;
    }
}

void fps_overlay_text(char *text, size_t text_size)
{
    unsigned long long total_us;
    unsigned long long worst_us;
    int index;

    total_us = 0;
    worst_us = 0;
    for (index = 0; index < frame_count; index++) {
        total_us = total_us + frame_times_us[index];
        if (frame_times_us[index] > worst_us) {
            worst_us = frame_times_us[index];
        }
    }
    if (total_us == 0) {
        snprintf(text, text_size, "FPS --");
        return;
    }
    snprintf(text, text_size, "FPS %.1f WORST %.0fMS", (float)frame_count * MICROSECONDS_PER_SECOND / (float)total_us, (float)worst_us / MICROSECONDS_PER_MILLISECOND);
}

void fps_overlay_draw(void)
{
    static const struct renderer_color backing = { 0.0f, 0.0f, 0.0f, 0.7f };
    static const struct renderer_color lettering = { 1.0f, 1.0f, 0.2f, 1.0f };
    char text[OVERLAY_TEXT_MAX];
    struct renderer_rect area;
    size_t length;

    fps_overlay_text(text, sizeof(text));
    length = strlen(text);
    area.x = OVERLAY_X - OVERLAY_PADDING;
    area.y = OVERLAY_Y - OVERLAY_PADDING;
    area.width = (float)(length * DEBUG_FONT_CELL_WIDTH) * OVERLAY_SCALE + 2.0f * OVERLAY_PADDING;
    area.height = (float)DEBUG_FONT_CELL_HEIGHT * OVERLAY_SCALE + 2.0f * OVERLAY_PADDING;
    renderer_draw_filled_rect(area, backing);
    debug_font_draw_text(OVERLAY_X, OVERLAY_Y, OVERLAY_SCALE, lettering, text);
}
