#define renderer_init real_renderer_init
#define renderer_shutdown real_renderer_shutdown
#define renderer_begin_frame real_renderer_begin_frame
#define renderer_end_frame real_renderer_end_frame
#define renderer_texture_create real_renderer_texture_create
#define renderer_texture_create_compressed real_renderer_texture_create_compressed
#define renderer_texture_destroy real_renderer_texture_destroy
#define renderer_live_texture_count real_renderer_live_texture_count
#define renderer_draw_sprite real_renderer_draw_sprite
#define renderer_draw_flat_sprite real_renderer_draw_flat_sprite
#define renderer_draw_beam real_renderer_draw_beam
#define renderer_draw_filled_rect real_renderer_draw_filled_rect
#define renderer_draw_filled_rects real_renderer_draw_filled_rects
#define renderer_snapshot_begin real_renderer_snapshot_begin
#define renderer_snapshot_end real_renderer_snapshot_end
#define renderer_draw_snapshot real_renderer_draw_snapshot
#define renderer_draw_turned_snapshot real_renderer_draw_turned_snapshot
#define renderer_draw_triangles real_renderer_draw_triangles
#include "../../src/vita/renderer.c"
#include "fakes/fake_platform.h"
#include "test_support.h"

#define FAKE_RECORD_MAX 32
#define FAKE_TEXTURE_ID_MAX 64
#define FAKE_RENDER_TARGET_BYTES 1234
#define FREE_VIDEO_BYTES_BEFORE 100000000u
#define FREE_MAIN_BYTES_BEFORE 50000000u
#define VIDEO_BYTES_TAKEN 3133440u
#define MAIN_BYTES_TAKEN 4096u
#define THREE_MIB (3u * 1024u * 1024u)
#define BYTES_PER_PIXEL_OF_VITAGL_DEPTH_BUFFER 4u
#define BYTES_PER_PIXEL_OF_RGB565 2u
#define BYTES_PER_PIXEL_OF_ANYTHING_ELSE 4u

struct texture_request {
    GLuint texture;
    GLint internal_format;
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum type;
    const GLvoid *data;
};

struct draw_record {
    GLuint texture;
    GLuint framebuffer;
    GLsizei vertex_count;
    struct batch_vertex vertices[VERTICES_PER_QUAD];
};

struct clear_record {
    GLuint framebuffer;
    GLbitfield mask;
    GLclampf red;
    GLclampf green;
    GLclampf blue;
    GLclampf alpha;
};

static struct texture_request texture_requests[FAKE_RECORD_MAX];
static int texture_request_count;
static struct draw_record draws[FAKE_RECORD_MAX];
static int draw_count;
static struct clear_record clears[FAKE_RECORD_MAX];
static int clear_count;
static GLuint deleted_textures[FAKE_RECORD_MAX];
static int deleted_texture_count;
static const GLvoid *pixels_of_texture[FAKE_TEXTURE_ID_MAX];
static GLuint next_texture_id;
static GLuint bound_texture;
static GLuint bound_framebuffer;
static GLenum last_bind_target;
static int framebuffers_generated;
static int framebuffers_deleted;
static GLuint last_deleted_framebuffer;
static GLuint attached_texture;
static GLenum attached_to;
static GLenum attach_target;
static GLuint attached_on_framebuffer;
static int textures_with_no_pixels_fail;
static int device_stats_reads;
static SceGxmRenderTargetParams seen_target_params;
static const unsigned char some_pixels[4] = { 1, 2, 3, 4 };

void device_stats_read(struct device_stats *stats)
{
    memset(stats, 0, sizeof(*stats));
    stats->video_memory_free = FREE_VIDEO_BYTES_BEFORE - (device_stats_reads == 0 ? 0 : VIDEO_BYTES_TAKEN);
    stats->main_memory_free = FREE_MAIN_BYTES_BEFORE - (device_stats_reads == 0 ? 0 : MAIN_BYTES_TAKEN);
    device_stats_reads = device_stats_reads + 1;
}

int sceGxmGetRenderTargetMemSize(const SceGxmRenderTargetParams *params, unsigned int *driver_memory_size)
{
    seen_target_params = *params;
    *driver_memory_size = FAKE_RENDER_TARGET_BYTES;
    return 0;
}

GLboolean vglInitExtended(int pool_size, int width, int height, int ram_threshold, int msaa)
{
    (void)pool_size;
    (void)width;
    (void)height;
    (void)ram_threshold;
    (void)msaa;
    return GL_FALSE;
}

void vglSwapBuffers(GLboolean has_commondialog)
{
    (void)has_commondialog;
}

void *vglGetTexDataPointer(GLenum target)
{
    (void)target;
    if (textures_with_no_pixels_fail && pixels_of_texture[bound_texture] == NULL) {
        return NULL;
    }
    return (void *)some_pixels;
}

GLenum glGetError(void)
{
    return GL_NO_ERROR;
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}

void glMatrixMode(GLenum mode)
{
    (void)mode;
}

void glLoadIdentity(void)
{
}

void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_value, GLdouble far_value)
{
    (void)left;
    (void)right;
    (void)bottom;
    (void)top;
    (void)near_value;
    (void)far_value;
}

void glDisable(GLenum capability)
{
    (void)capability;
}

void glEnable(GLenum capability)
{
    (void)capability;
}

void glBlendFunc(GLenum source_factor, GLenum destination_factor)
{
    (void)source_factor;
    (void)destination_factor;
}

void glEnableClientState(GLenum array)
{
    (void)array;
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    clears[clear_count].red = red;
    clears[clear_count].green = green;
    clears[clear_count].blue = blue;
    clears[clear_count].alpha = alpha;
}

void glClear(GLbitfield mask)
{
    clears[clear_count].mask = mask;
    clears[clear_count].framebuffer = bound_framebuffer;
    clear_count = clear_count + 1;
}

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    (void)size;
    (void)type;
    (void)stride;
    (void)pointer;
}

void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    (void)size;
    (void)type;
    (void)stride;
    (void)pointer;
}

void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    (void)size;
    (void)type;
    (void)stride;
    (void)pointer;
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    (void)mode;
    (void)first;
    draws[draw_count].texture = bound_texture;
    draws[draw_count].framebuffer = bound_framebuffer;
    draws[draw_count].vertex_count = count;
    memcpy(draws[draw_count].vertices, batch_vertices, sizeof(draws[draw_count].vertices));
    draw_count = draw_count + 1;
}

void glGenTextures(GLsizei count, GLuint *textures)
{
    (void)count;
    next_texture_id = next_texture_id + 1;
    textures[0] = next_texture_id;
}

void glDeleteTextures(GLsizei count, const GLuint *textures)
{
    (void)count;
    deleted_textures[deleted_texture_count] = textures[0];
    deleted_texture_count = deleted_texture_count + 1;
}

void glBindTexture(GLenum target, GLuint texture)
{
    (void)target;
    bound_texture = texture;
}

void glTexParameteri(GLenum target, GLenum name, GLint value)
{
    (void)target;
    (void)name;
    (void)value;
}

void glTexImage2D(GLenum target, GLint level, GLint internal_format, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
    (void)target;
    (void)level;
    (void)border;
    texture_requests[texture_request_count].texture = bound_texture;
    texture_requests[texture_request_count].internal_format = internal_format;
    texture_requests[texture_request_count].width = width;
    texture_requests[texture_request_count].height = height;
    texture_requests[texture_request_count].format = format;
    texture_requests[texture_request_count].type = type;
    texture_requests[texture_request_count].data = data;
    texture_request_count = texture_request_count + 1;
    pixels_of_texture[bound_texture] = data;
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internal_format, GLsizei width, GLsizei height, GLint border, GLsizei size, const GLvoid *data)
{
    (void)target;
    (void)level;
    (void)internal_format;
    (void)width;
    (void)height;
    (void)border;
    (void)size;
    pixels_of_texture[bound_texture] = data;
}

void glGenFramebuffers(GLsizei count, GLuint *framebuffers)
{
    (void)count;
    framebuffers_generated = framebuffers_generated + 1;
    framebuffers[0] = 500 + (GLuint)framebuffers_generated;
}

void glDeleteFramebuffers(GLsizei count, const GLuint *framebuffers)
{
    (void)count;
    framebuffers_deleted = framebuffers_deleted + 1;
    last_deleted_framebuffer = framebuffers[0];
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    last_bind_target = target;
    bound_framebuffer = framebuffer;
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum texture_target, GLuint texture, GLint level)
{
    (void)texture_target;
    (void)level;
    attach_target = target;
    attached_to = attachment;
    attached_texture = texture;
    attached_on_framebuffer = bound_framebuffer;
}

static struct renderer_rect rect_of(float x, float y, float width, float height)
{
    struct renderer_rect rect;

    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    return rect;
}

static struct renderer_color white_at(float alpha)
{
    struct renderer_color color;

    color.red = 1.0f;
    color.green = 1.0f;
    color.blue = 1.0f;
    color.alpha = alpha;
    return color;
}

static const struct texture_request *request_of_the_target(void)
{
    int index;

    for (index = 0; index < texture_request_count; index++) {
        if (texture_requests[index].data == NULL) {
            return &texture_requests[index];
        }
    }
    return NULL;
}

static void forget_everything(void)
{
    texture_request_count = 0;
    draw_count = 0;
    clear_count = 0;
    deleted_texture_count = 0;
    next_texture_id = 0;
    bound_texture = 0;
    bound_framebuffer = 0;
    last_bind_target = 0;
    framebuffers_generated = 0;
    framebuffers_deleted = 0;
    last_deleted_framebuffer = 0;
    attached_texture = 0;
    attached_to = 0;
    attach_target = 0;
    attached_on_framebuffer = 0;
    textures_with_no_pixels_fail = 0;
    device_stats_reads = 0;
    memset(&seen_target_params, 0, sizeof(seen_target_params));
    memset(pixels_of_texture, 0, sizeof(pixels_of_texture));
    snapshot_framebuffer = 0;
    snapshot_texture = RENDERER_NO_TEXTURE;
    batch_quad_count = 0;
    batch_texture = RENDERER_NO_TEXTURE;
    fake_files_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
}

static void start_renderer(void)
{
    forget_everything();
    real_renderer_init();
}

static void test_the_target_is_one_rgb565_texture_the_size_of_the_screen(void)
{
    const struct texture_request *request;

    start_renderer();
    CHECK_INT(framebuffers_generated, 1);
    request = request_of_the_target();
    CHECK(request != NULL);
    CHECK_INT(request->width, RENDERER_WIDTH);
    CHECK_INT(request->height, RENDERER_HEIGHT);
    CHECK_INT(request->internal_format, GL_RGB);
    CHECK_INT(request->format, GL_RGB);
    CHECK_INT(request->type, GL_UNSIGNED_SHORT_5_6_5);
    CHECK_INT(attach_target, GL_FRAMEBUFFER);
    CHECK_INT(attached_to, GL_COLOR_ATTACHMENT0);
    CHECK_INT(attached_texture, request->texture);
    CHECK_INT(attached_on_framebuffer, 501);
}

static void test_the_target_costs_under_three_mib_of_video_memory(void)
{
    const struct texture_request *request;
    unsigned int pixels;
    unsigned int color_bytes;
    unsigned int depth_bytes;

    start_renderer();
    request = request_of_the_target();
    CHECK(request != NULL);
    pixels = (unsigned int)request->width * (unsigned int)request->height;
    color_bytes = pixels * (request->type == GL_UNSIGNED_SHORT_5_6_5 ? BYTES_PER_PIXEL_OF_RGB565 : BYTES_PER_PIXEL_OF_ANYTHING_ELSE);
    depth_bytes = pixels * BYTES_PER_PIXEL_OF_VITAGL_DEPTH_BUFFER;
    CHECK_INT(color_bytes + depth_bytes, VIDEO_BYTES_TAKEN);
    CHECK(color_bytes + depth_bytes < THREE_MIB);
}

static void test_starting_clears_the_target_so_its_first_use_holds_no_old_memory(void)
{
    start_renderer();
    CHECK_INT(clear_count, 1);
    CHECK_INT(clears[0].framebuffer, 501);
    CHECK_INT(clears[0].mask, GL_COLOR_BUFFER_BIT);
    CHECK(clears[0].red == 0.0f && clears[0].green == 0.0f && clears[0].blue == 0.0f && clears[0].alpha == 1.0f);
    CHECK_INT(bound_framebuffer, 0);
}

static void test_the_log_says_what_the_target_took(void)
{
    unsigned int size;

    start_renderer();
    CHECK_HAS_TEXT((const char *)fake_files_contents("run.log", &size), "renderer snapshot_target ok=1 video_bytes=3133440 main_bytes=4096 render_target_bytes=1234");
    CHECK_INT(seen_target_params.width, RENDERER_WIDTH);
    CHECK_INT(seen_target_params.height, RENDERER_HEIGHT);
    CHECK_INT(seen_target_params.scenesPerFrame, 1);
    CHECK_INT(seen_target_params.multisampleMode, SCE_GXM_MULTISAMPLE_NONE);
    CHECK_INT(seen_target_params.driverMemBlock, -1);
}

static void test_a_snapshot_goes_to_the_target_and_drawing_then_returns_to_the_screen(void)
{
    start_renderer();
    real_renderer_begin_frame(white_at(1.0f));
    real_renderer_draw_filled_rect(rect_of(1.0f, 2.0f, 3.0f, 4.0f), white_at(1.0f));
    CHECK_INT(draw_count, 0);
    CHECK(real_renderer_snapshot_begin());
    CHECK_INT(draw_count, 1);
    CHECK_INT(draws[0].framebuffer, 0);
    CHECK_INT(bound_framebuffer, 501);
    CHECK_INT(last_bind_target, GL_FRAMEBUFFER);
    real_renderer_draw_filled_rect(rect_of(5.0f, 6.0f, 7.0f, 8.0f), white_at(1.0f));
    CHECK_INT(draw_count, 1);
    real_renderer_snapshot_end();
    CHECK_INT(draw_count, 2);
    CHECK_INT(draws[1].framebuffer, 501);
    CHECK(draws[1].vertices[0].x == 5.0f);
    CHECK_INT(bound_framebuffer, 0);
    CHECK_INT(last_bind_target, GL_FRAMEBUFFER);
}

static void test_every_snapshot_starts_from_black(void)
{
    start_renderer();
    clear_count = 0;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    CHECK(real_renderer_snapshot_begin());
    CHECK_INT(clear_count, 1);
    CHECK_INT(clears[0].framebuffer, 501);
    CHECK(clears[0].red == 0.0f && clears[0].green == 0.0f && clears[0].blue == 0.0f && clears[0].alpha == 1.0f);
    real_renderer_snapshot_end();
}

static void test_taking_snapshots_never_makes_another_texture_or_target(void)
{
    int textures_made;
    int round;

    start_renderer();
    textures_made = (int)next_texture_id;
    for (round = 0; round < 5; round++) {
        CHECK(real_renderer_snapshot_begin());
        real_renderer_snapshot_end();
    }
    CHECK_INT(next_texture_id, textures_made);
    CHECK_INT(texture_request_count, 2);
    CHECK_INT(framebuffers_generated, 1);
}

static void test_the_snapshot_is_drawn_from_its_texture_with_the_rows_turned_over(void)
{
    const struct draw_record *draw;

    start_renderer();
    real_renderer_begin_frame(white_at(1.0f));
    real_renderer_draw_snapshot(rect_of(-10.0f, -20.0f, 1000.0f, 600.0f), 0.25f);
    real_renderer_end_frame();
    CHECK_INT(draw_count, 1);
    draw = &draws[0];
    CHECK_INT(draw->framebuffer, 0);
    CHECK_INT(draw->texture, request_of_the_target()->texture);
    CHECK(draw->vertices[0].x == -10.0f && draw->vertices[0].y == -20.0f);
    CHECK(draw->vertices[0].u == 0.0f && draw->vertices[0].v == 1.0f);
    CHECK(draw->vertices[4].x == 990.0f && draw->vertices[4].y == 580.0f);
    CHECK(draw->vertices[4].u == 1.0f && draw->vertices[4].v == 0.0f);
    CHECK(draw->vertices[0].red == 1.0f && draw->vertices[0].green == 1.0f && draw->vertices[0].blue == 1.0f);
    CHECK(draw->vertices[0].alpha == 0.25f);
}

static void test_with_no_texture_there_is_no_target_and_a_snapshot_is_refused(void)
{
    unsigned int size;

    forget_everything();
    textures_with_no_pixels_fail = 1;
    real_renderer_init();
    CHECK_INT(framebuffers_generated, 0);
    CHECK(!real_renderer_snapshot_begin());
    CHECK_INT(bound_framebuffer, 0);
    CHECK_INT(clear_count, 0);
    CHECK_HAS_TEXT((const char *)fake_files_contents("run.log", &size), "renderer snapshot_target ok=0");
}

static void test_shutting_down_deletes_the_target_and_its_texture(void)
{
    GLuint target_texture;

    start_renderer();
    target_texture = request_of_the_target()->texture;
    real_renderer_shutdown();
    CHECK_INT(framebuffers_deleted, 1);
    CHECK_INT(last_deleted_framebuffer, 501);
    CHECK_INT(deleted_texture_count, 2);
    CHECK_INT(deleted_textures[0], target_texture);
}

static void test_shutting_down_with_no_target_deletes_only_the_white_texture(void)
{
    forget_everything();
    textures_with_no_pixels_fail = 1;
    real_renderer_init();
    deleted_texture_count = 0;
    real_renderer_shutdown();
    CHECK_INT(framebuffers_deleted, 0);
    CHECK_INT(deleted_texture_count, 1);
}

int main(void)
{
    test_the_target_is_one_rgb565_texture_the_size_of_the_screen();
    test_the_target_costs_under_three_mib_of_video_memory();
    test_starting_clears_the_target_so_its_first_use_holds_no_old_memory();
    test_the_log_says_what_the_target_took();
    test_a_snapshot_goes_to_the_target_and_drawing_then_returns_to_the_screen();
    test_every_snapshot_starts_from_black();
    test_taking_snapshots_never_makes_another_texture_or_target();
    test_the_snapshot_is_drawn_from_its_texture_with_the_rows_turned_over();
    test_with_no_texture_there_is_no_target_and_a_snapshot_is_refused();
    test_shutting_down_deletes_the_target_and_its_texture();
    test_shutting_down_with_no_target_deletes_only_the_white_texture();
    fake_files_reset();
    return test_finish("unit/vita_renderer");
}

void glTexEnvi(GLenum target, GLenum name, GLint value)
{
    (void)target;
    (void)name;
    (void)value;
}
