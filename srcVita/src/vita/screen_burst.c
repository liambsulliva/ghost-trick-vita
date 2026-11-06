#include <stdio.h>
#include <string.h>
#include <psp2/display.h>
#include <psp2/kernel/threadmgr.h>
#include "clock.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "screen_burst.h"

#define BURST_FOLDER_NAME FILE_DATA_FOLDER "/captures/burst"
#define BURST_MAGIC "GTB1"
#define BURST_HEADER_SIZE 20u
#define BURST_LARGEST_WIDTH 480u
#define BURST_LARGEST_HEIGHT 272u
#define BURST_BYTES_PER_PIXEL 2u
#define BURST_LARGEST_FILE (BURST_HEADER_SIZE + BURST_LARGEST_WIDTH * BURST_LARGEST_HEIGHT * BURST_BYTES_PER_PIXEL)
#define BURST_US_BETWEEN_FRAMES 200000ull
#define BURST_QUEUE_LENGTH 4u
#define BURST_SLOTS 1000u
#define BURST_WRITER_PRIORITY 0x10000100
#define BURST_WRITER_STACK 0x4000
#define BURST_SOURCE_BYTES_PER_PIXEL 4u
#define BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL 2u

static unsigned char *pictures[BURST_QUEUE_LENGTH];
static unsigned int picture_sizes[BURST_QUEUE_LENGTH];
static SceUID pictures_waiting;
static volatile unsigned int pictures_copied;
static volatile unsigned int pictures_written;
static int is_ready;
static int is_on;
static unsigned int session;
static unsigned int dropped;
static unsigned long long started_us;
static unsigned long long last_copy_us;
static unsigned long long longest_copy_us;

static void write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)((value >> 8) & 0xffu);
    bytes[2] = (unsigned char)((value >> 16) & 0xffu);
    bytes[3] = (unsigned char)(value >> 24);
}

static unsigned int read_sequence(const unsigned char *picture)
{
    return (unsigned int)picture[8] | ((unsigned int)picture[9] << 8) | ((unsigned int)picture[10] << 16) | ((unsigned int)picture[11] << 24);
}

static int write_waiting_pictures(SceSize argument_size, void *arguments)
{
    char path[FILE_PATH_MAX];
    unsigned int queue_index;

    (void)argument_size;
    (void)arguments;
    for (;;) {
        sceKernelWaitSema(pictures_waiting, 1, NULL);
        queue_index = pictures_written % BURST_QUEUE_LENGTH;
        snprintf(path, sizeof(path), BURST_FOLDER_NAME "/f%03u.rgb", read_sequence(pictures[queue_index]) % BURST_SLOTS);
        file_write_whole(path, pictures[queue_index], picture_sizes[queue_index]);
        pictures_written = pictures_written + 1;
    }
    return 0;
}

static int get_ready(void)
{
    SceUID writer;
    unsigned int index;

    if (is_ready) {
        return 1;
    }
    file_make_folder(BURST_FOLDER_NAME);
    for (index = 0; index < BURST_QUEUE_LENGTH; index++) {
        pictures[index] = mem_alloc(BURST_LARGEST_FILE);
        if (pictures[index] == NULL) {
            return 0;
        }
    }
    pictures_waiting = sceKernelCreateSema("burst_waiting", 0, 0, BURST_QUEUE_LENGTH, NULL);
    writer = sceKernelCreateThread("burst_writer", write_waiting_pictures, BURST_WRITER_PRIORITY, BURST_WRITER_STACK, 0, SCE_KERNEL_CPU_MASK_USER_1, NULL);
    if (pictures_waiting < 0 || writer < 0 || sceKernelStartThread(writer, 0, NULL) < 0) {
        return 0;
    }
    is_ready = 1;
    return 1;
}

void screen_burst_set_on(int is_turning_on)
{
    if (!is_turning_on) {
        if (is_on) {
            run_log_write("burst off pictures=%u dropped=%u longest_copy_us=%llu", pictures_copied, dropped, longest_copy_us);
        }
        is_on = 0;
        return;
    }
    if (is_on) {
        return;
    }
    if (!get_ready()) {
        run_log_write("burst refused");
        return;
    }
    while (pictures_written != pictures_copied) {
        sceKernelDelayThread(1000);
    }
    pictures_copied = 0;
    pictures_written = 0;
    dropped = 0;
    longest_copy_us = 0;
    started_us = clock_now_us();
    last_copy_us = 0;
    session = (unsigned int)started_us;
    is_on = 1;
    run_log_write("burst on session=%u", session);
}

static void copy_half_size(unsigned char *destination, const unsigned char *source, unsigned int source_stride, unsigned int width, unsigned int height)
{
    unsigned int row;
    unsigned int column;

    for (row = 0; row < height; row++) {
        const unsigned char *pixel;

        pixel = &source[row * BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL * source_stride];
        for (column = 0; column < width; column++) {
            unsigned int color;

            color = ((unsigned int)(pixel[0] >> 3) << 11) | ((unsigned int)(pixel[1] >> 2) << 5) | (unsigned int)(pixel[2] >> 3);
            destination[0] = (unsigned char)(color & 0xffu);
            destination[1] = (unsigned char)(color >> 8);
            destination = &destination[BURST_BYTES_PER_PIXEL];
            pixel = &pixel[BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL * BURST_SOURCE_BYTES_PER_PIXEL];
        }
    }
}

void screen_burst_note_frame(void)
{
    SceDisplayFrameBuf frame_buffer;
    unsigned long long now_us;
    unsigned long long copy_us;
    unsigned char *picture;
    unsigned int queue_index;
    unsigned int width;
    unsigned int height;

    if (!is_on) {
        return;
    }
    now_us = clock_now_us();
    if (last_copy_us != 0 && now_us - last_copy_us < BURST_US_BETWEEN_FRAMES) {
        return;
    }
    if (pictures_copied - pictures_written >= BURST_QUEUE_LENGTH) {
        dropped = dropped + 1;
        return;
    }
    memset(&frame_buffer, 0, sizeof(frame_buffer));
    frame_buffer.size = sizeof(frame_buffer);
    if (sceDisplayGetFrameBuf(&frame_buffer, SCE_DISPLAY_SETBUF_NEXTFRAME) < 0 || frame_buffer.base == NULL || frame_buffer.pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return;
    }
    width = (unsigned int)frame_buffer.width / BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL;
    height = (unsigned int)frame_buffer.height / BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL;
    if (width > BURST_LARGEST_WIDTH || height > BURST_LARGEST_HEIGHT) {
        return;
    }
    last_copy_us = now_us;
    queue_index = pictures_copied % BURST_QUEUE_LENGTH;
    picture = pictures[queue_index];
    memcpy(picture, BURST_MAGIC, 4);
    write_u32(&picture[4], session);
    write_u32(&picture[8], pictures_copied);
    write_u32(&picture[12], (unsigned int)((now_us - started_us) / 1000ull));
    write_u32(&picture[16], width | (height << 16));
    copy_half_size(&picture[BURST_HEADER_SIZE], frame_buffer.base, (unsigned int)frame_buffer.pitch * BURST_SOURCE_BYTES_PER_PIXEL, width, height);
    picture_sizes[queue_index] = BURST_HEADER_SIZE + width * height * BURST_BYTES_PER_PIXEL;
    pictures_copied = pictures_copied + 1;
    sceKernelSignalSema(pictures_waiting, 1);
    copy_us = clock_now_us() - now_us;
    if (copy_us > longest_copy_us) {
        longest_copy_us = copy_us;
    }
}
