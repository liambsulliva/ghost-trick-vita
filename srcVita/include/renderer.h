#ifndef RENDERER_H
#define RENDERER_H

#include <vitaGL.h>

#define RENDERER_WIDTH 960
#define RENDERER_HEIGHT 544
#define RENDERER_POOL_SIZE 0x800000
#define RENDERER_RESERVED_MAIN_MEMORY 0x1000000

int renderer_init(void);
void renderer_shutdown(void);
void renderer_begin_frame(float red, float green, float blue);
void renderer_end_frame(void);
void renderer_draw_triangle(void);
GLuint renderer_texture_create(const unsigned char *rgba, int width, int height);
void renderer_texture_destroy(GLuint texture);
void renderer_draw_textured_quad(GLuint texture);

#endif
