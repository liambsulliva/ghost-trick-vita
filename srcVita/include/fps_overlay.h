#ifndef FPS_OVERLAY_H
#define FPS_OVERLAY_H

#include <stddef.h>

#define FPS_OVERLAY_FRAMES 60

void fps_overlay_reset(void);
void fps_overlay_note_frame(unsigned long long frame_us);
void fps_overlay_text(char *text, size_t text_size);
void fps_overlay_draw(void);

#endif
