#ifndef FAKE_VITAGL_H
#define FAKE_VITAGL_H

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef void GLvoid;

#define GL_FALSE 0
#define GL_NO_ERROR 0
#define GL_INVALID_OPERATION 0x0502
#define GL_TRIANGLES 0x0004
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_BLEND 0x0BE2
#define GL_TEXTURE_2D 0x0DE1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_FLOAT 0x1406
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_ONE 1
#define GL_DST_COLOR 0x0306
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_VERTEX_ARRAY 0x8074
#define GL_COLOR_ARRAY 0x8076
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_FRAMEBUFFER 0x8D40

GLboolean vglInitExtended(int pool_size, int width, int height, int ram_threshold, int msaa);
void vglSwapBuffers(GLboolean has_commondialog);
void *vglGetTexDataPointer(GLenum target);
GLenum glGetError(void);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glMatrixMode(GLenum mode);
void glLoadIdentity(void);
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_value, GLdouble far_value);
void glDisable(GLenum capability);
void glEnable(GLenum capability);
void glBlendFunc(GLenum source_factor, GLenum destination_factor);
void glBlendEquation(GLenum mode);
void glEnableClientState(GLenum array);
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glClear(GLbitfield mask);
void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void glDrawArrays(GLenum mode, GLint first, GLsizei count);
void glGenTextures(GLsizei count, GLuint *textures);
void glDeleteTextures(GLsizei count, const GLuint *textures);
void glBindTexture(GLenum target, GLuint texture);
void glTexParameteri(GLenum target, GLenum name, GLint value);
void glTexImage2D(GLenum target, GLint level, GLint internal_format, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
void glCompressedTexImage2D(GLenum target, GLint level, GLenum internal_format, GLsizei width, GLsizei height, GLint border, GLsizei size, const GLvoid *data);
void glGenFramebuffers(GLsizei count, GLuint *framebuffers);
void glDeleteFramebuffers(GLsizei count, const GLuint *framebuffers);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum texture_target, GLuint texture, GLint level);

#define GL_TEXTURE_ENV 0x2300
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_MODULATE 0x2100
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_REPLACE 0x1E01
#define GL_PRIMARY_COLOR 0x8577
#define GL_TEXTURE 0x1702
#define GL_SRC0_RGB 0x8580
#define GL_SRC0_ALPHA 0x8588
#define GL_SRC1_ALPHA 0x8589
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_SRC_COLOR 0x0300
void glTexEnvi(GLenum target, GLenum name, GLint value);
#endif
