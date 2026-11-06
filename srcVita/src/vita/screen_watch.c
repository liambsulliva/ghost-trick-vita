#include <string.h>
#include <psp2/display.h>
#include "clock.h"
#include "run_log.h"
#include "screen_changes.h"
#include "screen_watch.h"

static struct screen_changes changes;
static int vcount_at_window_start;
static unsigned long long hash_us_total;
static unsigned long long hash_us_longest;

static void start_window(void)
{
    screen_changes_start_window(&changes);
    vcount_at_window_start = sceDisplayGetVcount();
    hash_us_total = 0;
    hash_us_longest = 0;
}

void screen_watch_start(void)
{
    screen_changes_reset(&changes);
    start_window();
}

void screen_watch_note_frame(void)
{
    SceDisplayFrameBuf frame_buffer;
    unsigned long long started_us;
    unsigned long long hash_us;
    unsigned int hash;

    memset(&frame_buffer, 0, sizeof(frame_buffer));
    frame_buffer.size = sizeof(frame_buffer);
    if (sceDisplayGetFrameBuf(&frame_buffer, SCE_DISPLAY_SETBUF_NEXTFRAME) < 0 || frame_buffer.base == NULL) {
        return;
    }
    if (frame_buffer.base == changes.last_buffer) {
        screen_changes_add_same_buffer(&changes);
        return;
    }
    started_us = clock_now_us();
    hash = screen_changes_hash_grid(frame_buffer.base, frame_buffer.pitch, frame_buffer.width, frame_buffer.height);
    hash_us = clock_now_us() - started_us;
    hash_us_total = hash_us_total + hash_us;
    if (hash_us > hash_us_longest) {
        hash_us_longest = hash_us;
    }
    screen_changes_add_flip(&changes, frame_buffer.base, hash);
}

void screen_watch_publish(void)
{
    unsigned long long hash_us_average;

    hash_us_average = 0;
    if (changes.flips > 0) {
        hash_us_average = hash_us_total / changes.flips;
    }
    run_log_write("screen flips=%u changed=%u same_buffer=%u longest_still=%u vblanks=%d hash_us_avg=%llu hash_us_max=%llu",
        changes.flips, changes.changed, changes.same_buffer, changes.longest_still, sceDisplayGetVcount() - vcount_at_window_start, hash_us_average, hash_us_longest);
    start_window();
}
