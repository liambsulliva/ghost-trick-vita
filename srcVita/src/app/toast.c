#include <stdio.h>
#include "pause_widgets.h"
#include "renderer.h"
#include "run_log.h"
#include "toast.h"

#define KEY_MOST 64
#define SLIDE_US 250000ull
#define HOLD_US 3000000ull
#define TOP_SHOWN 12.0f
#define HEIGHT 48.0f
#define TEXT_PAD 28.0f

static char shown_key[KEY_MOST];
static unsigned long long age_us;
static int is_showing;

void toast_show(const char *key)
{
    snprintf(shown_key, sizeof(shown_key), "%s", key);
    age_us = 0;
    is_showing = 1;
    run_log_write("toast %s", key);
}

void toast_update(unsigned long long elapsed_us)
{
    if (!is_showing) {
        return;
    }
    age_us = age_us + elapsed_us;
    if (age_us >= 2ull * SLIDE_US + HOLD_US) {
        is_showing = 0;
    }
}

int toast_is_showing(void)
{
    return is_showing;
}

float toast_top(void)
{
    float hidden;
    float shown;

    hidden = -HEIGHT - TOP_SHOWN;
    if (!is_showing) {
        return hidden;
    }
    if (age_us < SLIDE_US) {
        shown = (float)age_us / (float)SLIDE_US;
    } else if (age_us < SLIDE_US + HOLD_US) {
        shown = 1.0f;
    } else {
        shown = 1.0f - (float)(age_us - SLIDE_US - HOLD_US) / (float)SLIDE_US;
    }
    shown = shown < 0.0f ? 0.0f : shown;
    return hidden + (TOP_SHOWN - hidden) * shown;
}

void toast_draw(void)
{
    float width;
    float left;
    float top;

    if (!is_showing || !pause_widgets_has(shown_key)) {
        return;
    }
    width = pause_text_width(shown_key) + 2.0f * TEXT_PAD;
    left = ((float)RENDERER_WIDTH - width) / 2.0f;
    top = toast_top();
    pause_draw_slice("tab_selected", left, top, width, HEIGHT, 1.0f);
    pause_draw_text(shown_key, (float)RENDERER_WIDTH / 2.0f, top + HEIGHT / 2.0f, PAUSE_ANCHOR_CENTRE, 1.0f);
}
