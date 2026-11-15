#ifndef SMOKE_RENDERER_H
#define SMOKE_RENDERER_H

int smoke_renderer_save_bmp(const char *disk_path);
unsigned int smoke_renderer_draws_of_dead_textures(void);
unsigned int smoke_renderer_draws_of_unfilled_textures(void);
unsigned int smoke_renderer_frames_drawn(void);
unsigned int smoke_renderer_shapes_in_last_frame(void);

#endif
