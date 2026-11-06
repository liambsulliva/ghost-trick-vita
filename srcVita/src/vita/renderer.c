#include <string.h>
#include <psp2/gxm.h>
#include <vitaGL.h>
#include "device_stats.h"
#include "renderer.h"
#include "run_log.h"

#define RENDERER_POOL_SIZE 0x800000
#define RENDERER_MAIN_MEMORY_LEFT_FOR_APP 0x1000000
#define BATCH_QUAD_MAX 1024
#define VERTICES_PER_QUAD 6
#define LOG_FOR_TOOLS_SNAPSHOT_TARGET "renderer snapshot_target ok=%d video_bytes=%d main_bytes=%d render_target_bytes=%u"

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
static int batch_is_flat;
static renderer_texture snapshot_texture;
static GLuint snapshot_framebuffer;

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

static GLuint start_new_texture(void)
{
    GLuint texture;

    flush_batch();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}

static renderer_texture finish_new_texture(GLuint texture)
{
    if (glGetError() != GL_NO_ERROR || vglGetTexDataPointer(GL_TEXTURE_2D) == NULL) {
        glDeleteTextures(1, &texture);
        return RENDERER_NO_TEXTURE;
    }
    return texture;
}

static void clear_bound_target_to_black(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static unsigned int snapshot_render_target_bytes(void)
{
    SceGxmRenderTargetParams params;
    unsigned int bytes;

    memset(&params, 0, sizeof(params));
    params.width = RENDERER_WIDTH;
    params.height = RENDERER_HEIGHT;
    params.scenesPerFrame = 1;
    params.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;
    params.driverMemBlock = -1;
    bytes = 0;
    sceGxmGetRenderTargetMemSize(&params, &bytes);
    return bytes;
}

static void create_snapshot_target(void)
{
    struct device_stats before;
    struct device_stats after;
    GLuint texture;

    device_stats_read(&before);
    texture = start_new_texture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RENDERER_WIDTH, RENDERER_HEIGHT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    snapshot_texture = finish_new_texture(texture);
    if (snapshot_texture != RENDERER_NO_TEXTURE) {
        glGenFramebuffers(1, &snapshot_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, snapshot_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, snapshot_texture, 0);
        clear_bound_target_to_black();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    device_stats_read(&after);
    run_log_write(LOG_FOR_TOOLS_SNAPSHOT_TARGET, snapshot_texture != RENDERER_NO_TEXTURE, (int)(before.video_memory_free - after.video_memory_free), (int)(before.main_memory_free - after.main_memory_free), snapshot_render_target_bytes());
}

int renderer_init(void)
{
    static const unsigned char white_pixel[4] = { 255, 255, 255, 255 };
    GLboolean resolution_fell_back;

    resolution_fell_back = vglInitExtended(RENDERER_POOL_SIZE, RENDERER_WIDTH, RENDERER_HEIGHT, RENDERER_MAIN_MEMORY_LEFT_FOR_APP, SCE_GXM_MULTISAMPLE_NONE);
    set_up_screen_space_drawing();
    white_texture = renderer_texture_create(white_pixel, 1, 1);
    create_snapshot_target();
    return !resolution_fell_back;
}

void renderer_shutdown(void)
{
    if (snapshot_framebuffer != 0) {
        glDeleteFramebuffers(1, &snapshot_framebuffer);
        renderer_texture_destroy(snapshot_texture);
    }
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

    texture = start_new_texture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_pixels);
    return finish_new_texture(texture);
}

renderer_texture renderer_texture_create_compressed(const unsigned char *blocks, unsigned int block_bytes, int width, int height, enum renderer_compressed_format format)
{
    GLuint texture;
    GLenum gl_format;

    gl_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    if (format == RENDERER_COMPRESSED_DXT1) {
        gl_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    }
    texture = start_new_texture();
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, block_bytes, blocks);
    return finish_new_texture(texture);
}

void renderer_texture_destroy(renderer_texture texture)
{
    GLuint gl_texture;

    flush_batch();
    gl_texture = texture;
    glDeleteTextures(1, &gl_texture);
}

static void set_flat_drawing(int is_flat)
{
    if (batch_is_flat == is_flat) {
        return;
    }
    flush_batch();
    batch_is_flat = is_flat;
    if (!is_flat) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        return;
    }
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
}

static void queue_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
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

void renderer_draw_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color tint)
{
    set_flat_drawing(0);
    queue_sprite(texture, screen_area, texture_area, tint);
}

void renderer_draw_flat_sprite(renderer_texture texture, struct renderer_rect screen_area, struct renderer_rect texture_area, struct renderer_color color)
{
    set_flat_drawing(1);
    queue_sprite(texture, screen_area, texture_area, color);
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

void renderer_draw_filled_rects(const struct renderer_rect *screen_areas, int count, struct renderer_color color)
{
    int index;

    for (index = 0; index < count; index++) {
        renderer_draw_filled_rect(screen_areas[index], color);
    }
}

int renderer_snapshot_begin(void)
{
    if (snapshot_framebuffer == 0) {
        return 0;
    }
    flush_batch();
    glBindFramebuffer(GL_FRAMEBUFFER, snapshot_framebuffer);
    clear_bound_target_to_black();
    return 1;
}

void renderer_snapshot_end(void)
{
    flush_batch();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderer_draw_snapshot(struct renderer_rect screen_area, float alpha)
{
    struct renderer_rect texture_stored_bottom_row_first;
    struct renderer_color tint;

    texture_stored_bottom_row_first.x = 0.0f;
    texture_stored_bottom_row_first.y = 1.0f;
    texture_stored_bottom_row_first.width = 1.0f;
    texture_stored_bottom_row_first.height = -1.0f;
    tint.red = 1.0f;
    tint.green = 1.0f;
    tint.blue = 1.0f;
    tint.alpha = alpha;
    renderer_draw_sprite(snapshot_texture, screen_area, texture_stored_bottom_row_first, tint);
}
