#include <string.h>
#include <psp2/display.h>
#include "bmp_image.h"
#include "file.h"
#include "memory.h"
#include "screen_capture.h"

int screen_capture_save_bmp(const char *path)
{
    SceDisplayFrameBuf frame_buffer;
    const unsigned char *displayed_pixels;
    const unsigned char *source_row;
    unsigned char *image;
    unsigned char *destination_row;
    unsigned int width;
    unsigned int height;
    unsigned int row_size;
    unsigned int row;
    unsigned int source_stride;
    int write_result;

    memset(&frame_buffer, 0, sizeof(frame_buffer));
    frame_buffer.size = sizeof(frame_buffer);
    if (sceDisplayGetFrameBuf(&frame_buffer, SCE_DISPLAY_SETBUF_NEXTFRAME) < 0 || frame_buffer.base == NULL) {
        return SCREEN_CAPTURE_ERR_DISPLAY;
    }
    if (frame_buffer.pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return SCREEN_CAPTURE_ERR_FORMAT;
    }

    width = frame_buffer.width;
    height = frame_buffer.height;
    source_stride = frame_buffer.pitch * 4;
    row_size = bmp_row_size(width);
    image = mem_alloc(bmp_file_size(width, height));
    if (image == NULL) {
        return SCREEN_CAPTURE_ERR_MEMORY;
    }

    bmp_write_header(image, width, height);
    displayed_pixels = frame_buffer.base;
    for (row = 0; row < height; row++) {
        source_row = &displayed_pixels[(height - 1 - row) * source_stride];
        destination_row = &image[BMP_HEADER_SIZE + row * row_size];
        bmp_write_row_from_rgba(destination_row, source_row, width);
    }

    write_result = file_write_whole(path, image, bmp_file_size(width, height));
    mem_free(image);
    if (write_result != FILE_OK) {
        return SCREEN_CAPTURE_ERR_WRITE;
    }
    return SCREEN_CAPTURE_OK;
}
