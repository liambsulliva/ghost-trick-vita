#include "renderer.h"

static const float triangle_positions[] = {
    -0.8f, -0.5f,
    -0.2f, -0.5f,
    -0.5f,  0.5f
};

static const float triangle_colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

static const float quad_positions[] = {
     0.2f,  0.5f,
     0.2f, -0.5f,
     0.8f,  0.5f,
     0.8f, -0.5f
};

static const float quad_uvs[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

void renderer_init(void)
{
    vglInit(RENDERER_POOL_SIZE);
    glViewport(0, 0, RENDERER_WIDTH, RENDERER_HEIGHT);
}

void renderer_shutdown(void)
{
    vglEnd();
}

void renderer_begin_frame(float red, float green, float blue)
{
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_end_frame(void)
{
    vglSwapBuffers(GL_TRUE);
}

void renderer_draw_triangle(void)
{
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, triangle_positions);
    glColorPointer(4, GL_FLOAT, 0, triangle_colors);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_COLOR_ARRAY);
}

GLuint renderer_texture_create(const unsigned char *rgba, int width, int height)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    return texture;
}

void renderer_texture_destroy(GLuint texture)
{
    glDeleteTextures(1, &texture);
}

void renderer_draw_textured_quad(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, quad_positions);
    glTexCoordPointer(2, GL_FLOAT, 0, quad_uvs);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
