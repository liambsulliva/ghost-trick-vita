#include <string.h>
#include "fps_overlay.h"
#include "test_support.h"

static void test_steady_frames_read_as_sixty(void)
{
    char text[48];
    int index;

    fps_overlay_reset();
    for (index = 0; index < FPS_OVERLAY_FRAMES; index++) {
        fps_overlay_note_frame(16667);
    }
    fps_overlay_text(text, sizeof(text));
    CHECK_TEXT(text, "FPS 60.0 WORST 17MS");
}

static void test_one_slow_frame_shows_as_the_worst(void)
{
    char text[48];
    int index;

    fps_overlay_reset();
    for (index = 0; index < FPS_OVERLAY_FRAMES - 1; index++) {
        fps_overlay_note_frame(16667);
    }
    fps_overlay_note_frame(250000);
    fps_overlay_text(text, sizeof(text));
    CHECK(strstr(text, "WORST 250MS") != NULL);
    CHECK(strncmp(text, "FPS 48.6", 8) == 0);
}

static void test_no_frames_yet_reads_as_dashes(void)
{
    char text[48];

    fps_overlay_reset();
    fps_overlay_text(text, sizeof(text));
    CHECK_TEXT(text, "FPS --");
}

int main(void)
{
    test_steady_frames_read_as_sixty();
    test_one_slow_frame_shows_as_the_worst();
    test_no_frames_yet_reads_as_dashes();
    return test_finish("unit/fps_overlay");
}
