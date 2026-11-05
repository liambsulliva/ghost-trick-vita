#include <vitaGL.h>
#include "renderer.h"

#define RENDERER_POOL_SIZE 0x800000
#define RENDERER_RESERVED_MAIN_MEMORY 0x1000000
#define BATCH_QUAD_MAX 1024
#define VERTICES_PER_QUAD 6

struct batch_vertex {
    float x;
    float y;
    float u;
    float v;
    float red;
    float green;
    float blue;
    float alpha;
};

static struct batch_vertex batch_vertices[BATCH_QUAD_MAX * VERTICES_PER_QUAD];
static int batch_quad_count;
static renderer_texture batch_texture;
static renderer_texture white_texture;

static void flush_batch(void)
{
    if (batch_quad_count == 0) {
        return;
    }
    glBindTexture(GL_TEXTURE_2D, batch_texture);
    glVertexPointer(2, GL_FLOAT, sizeof(struct batch_vertex), &batch_vertices[0].x);
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct batch_vertex), &batch_vertices[0].u);
    glColorPointer(4, GL_FLOAT, sizeof(struct batch_vertex), &batch_vertices[0].red);
    glDrawArrays(GL_TRIANGLES, 0, batch_quad_count * VERTICES_PER_QUAD);
    batch_quad_count = 0;
}

static void set_vertex(struct batch_vertex *vertex, float x, float y, float u, float v, struct renderer_color tint)
{
    vertex->x = x;
    vertex->y = y;
    vertex->u = u;
    vertex->v = v;
    vertex->red = tint.red;
    vertex->green = tint.green;
    vertex->blue = tint.blue;
    vertex->alpha = tint.alpha;
}

static void set_up_screen_space_drawing(void)
{
    glViewport(0, 0, RENDERER_WIDTH, RENDERER_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, RENDERER_WIDTH, RENDERER_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

int renderer_init(void)
{
    static const unsigned char white_pixel[4] = { 255, 255, 255, 255 };
    GLboolean resolution_fell_back;

    resolution_fell_back = vglInitExtended(RENDERER_POOL_SIZE, RENDERER_WIDTH, RENDERER_HEIGHT, RENDERER_RESERVED_MAIN_MEMORY, SCE_GXM_MULTISAMPLE_NONE);
    set_up_screen_space_drawing();
    white_texture = renderer_texture_create(white_pixel, 1, 1);
    return !resolution_fell_back;
}

void renderer_shutdown(void)
{
    renderer_texture_destroy(white_texture);
}

void renderer_begin_frame(struct renderer_color clear_color)
{
    glClearColor(clear_color.red, clear_color.green, clear_color.blue, clear_color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    batch_quad_count = 0;
    batch_texture = RENDERER_NO_TEXTURE;
}

void renderer_end_frame(void)
{
    flush_batch();
    vglSwapBuffers(GL_FALSE);
}

renderer_texture renderer_texture_create(const unsigned char *rgba_pixels, int width, int height)
{
    GLuint texture;

    flush_batch();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_pixels);
    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &texture);
        return RENDERER_NO_TEXTURE;
    }
    return texture;
}

void renderer_texture_destroy(renderer_texture texture)
{
    GLuint gl_texture;

    flush_batch();
    gl_texture = texture;
    glDeleteTextures(1, &gl_texture);
}

void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    struct batch_vertex *quad;
    float left;
    float right;
    float top;
    float bottom;
    float u_left;
    float u_right;
    float v_top;
    float v_bottom;

    if (texture != batch_texture || batch_quad_count == BATCH_QUAD_MAX) {
        flush_batch();
        batch_texture = texture;
    }
    left = screen_area.x;
    right = screen_area.x + screen_area.width;
    top = screen_area.y;
    bottom = screen_area.y + screen_area.height;
    u_left = texture_area.x;
    u_right = texture_area.x + texture_area.width;
    v_top = texture_area.y;
    v_bottom = texture_area.y + texture_area.height;

    quad = &batch_vertices[batch_quad_count * VERTICES_PER_QUAD];
    set_vertex(&quad[0], left, top, u_left, v_top, tint);
    set_vertex(&quad[1], right, top, u_right, v_top, tint);
    set_vertex(&quad[2], left, bottom, u_left, v_bottom, tint);
    set_vertex(&quad[3], right, top, u_right, v_top, tint);
    set_vertex(&quad[4], right, bottom, u_right, v_bottom, tint);
    set_vertex(&quad[5], left, bottom, u_left, v_bottom, tint);
    batch_quad_count = batch_quad_count + 1;
}

void renderer_draw_filled_rect(struct renderer_rect screen_area, struct renderer_color color)
{
    struct renderer_rect whole_texture;

    whole_texture.x = 0.0f;
    whole_texture.y = 0.0f;
    whole_texture.width = 1.0f;
    whole_texture.height = 1.0f;
    renderer_draw_sprite(white_texture, screen_area, whole_texture, color);
}
