#include <stdio.h>
#include <string.h>
#include <psp2/display.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/threadmgr.h>
#include "clock.h"
#include "file.h"
#include "memory.h"
#include "run_log.h"
#include "screen_burst.h"

#define BURST_FOLDER_NAME FILE_DATA_FOLDER "/captures/burst"
#define BURST_WINDOW_FILE BURST_FOLDER_NAME "/window.bin"
#define BURST_MAGIC "GTB1"
#define BURST_HEADER_SIZE 20u
#define BURST_LARGEST_WIDTH 480u
#define BURST_LARGEST_HEIGHT 272u
#define BURST_BYTES_PER_PIXEL 2u
#define BURST_LARGEST_PICTURE (BURST_HEADER_SIZE + BURST_LARGEST_WIDTH * BURST_LARGEST_HEIGHT * BURST_BYTES_PER_PIXEL)
#define BURST_US_BETWEEN_FRAMES 200000ull
#define BURST_QUEUE_LENGTH 4u
#define BURST_WRITER_PRIORITY 0x10000100
#define BURST_WRITER_STACK 0x4000
#define BURST_SOURCE_BYTES_PER_PIXEL 4u
#define BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL 2u
#define BURST_FILE_MODE 0777
#define US_PER_SECOND 1000000ull

static unsigned char *pictures[BURST_QUEUE_LENGTH];
static unsigned int picture_sizes[BURST_QUEUE_LENGTH];
static SceUID pictures_waiting;
static SceUID window_file;
static volatile unsigned int pictures_copied;
static volatile unsigned int pictures_written;
static volatile int window_should_close;
static int is_ready;
static int is_on;
static unsigned int session;
static unsigned int dropped;
static unsigned long long started_us;
static unsigned long long stops_at_us;
static unsigned long long last_copy_us;
static unsigned long long longest_copy_us;

static void write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)((value >> 8) & 0xffu);
    bytes[2] = (unsigned char)((value >> 16) & 0xffu);
    bytes[3] = (unsigned char)(value >> 24);
}

static int write_waiting_pictures(SceSize argument_size, void *arguments)
{
    unsigned int queue_index;

    (void)argument_size;
    (void)arguments;
    for (;;) {
        sceKernelWaitSema(pictures_waiting, 1, NULL);
        if (pictures_written != pictures_copied) {
            queue_index = pictures_written % BURST_QUEUE_LENGTH;
            sceIoWrite(window_file, pictures[queue_index], picture_sizes[queue_index]);
            pictures_written = pictures_written + 1;
        } else if (window_should_close) {
            sceIoClose(window_file);
            window_file = -1;
            window_should_close = 0;
        }
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
        pictures[index] = mem_alloc(BURST_LARGEST_PICTURE);
        if (pictures[index] == NULL) {
            return 0;
        }
    }
    pictures_waiting = sceKernelCreateSema("burst_waiting", 0, 0, BURST_QUEUE_LENGTH + 1, NULL);
    writer = sceKernelCreateThread("burst_writer", write_waiting_pictures, BURST_WRITER_PRIORITY, BURST_WRITER_STACK, 0, SCE_KERNEL_CPU_MASK_USER_1, NULL);
    if (pictures_waiting < 0 || writer < 0 || sceKernelStartThread(writer, 0, NULL) < 0) {
        return 0;
    }
    is_ready = 1;
    return 1;
}

static void stop_burst(void)
{
    if (is_on) {
        run_log_write("burst off pictures=%u dropped=%u longest_copy_us=%llu", pictures_copied, dropped, longest_copy_us);
        window_should_close = 1;
        sceKernelSignalSema(pictures_waiting, 1);
    }
    is_on = 0;
}

void screen_burst_run_for_seconds(int seconds)
{
    if (seconds <= 0) {
        stop_burst();
        return;
    }
    if (is_on) {
        stops_at_us = clock_now_us() + (unsigned long long)seconds * US_PER_SECOND;
        return;
    }
    if (!get_ready()) {
        run_log_write("burst refused");
        return;
    }
    while (window_should_close || pictures_written != pictures_copied) {
        sceKernelDelayThread(1000);
    }
    window_file = sceIoOpen(BURST_WINDOW_FILE, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, BURST_FILE_MODE);
    if (window_file < 0) {
        run_log_write("burst refused open=%d", window_file);
        return;
    }
    pictures_copied = 0;
    pictures_written = 0;
    dropped = 0;
    longest_copy_us = 0;
    started_us = clock_now_us();
    stops_at_us = started_us + (unsigned long long)seconds * US_PER_SECOND;
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

int screen_burst_note_frame_and_say_if_it_ended(void)
{
    SceDisplayFrameBuf frame_buffer;
    unsigned long long now_us;
    unsigned long long copy_us;
    unsigned char *picture;
    unsigned int queue_index;
    unsigned int width;
    unsigned int height;

    if (!is_on) {
        return 0;
    }
    now_us = clock_now_us();
    if (now_us >= stops_at_us) {
        stop_burst();
        return 1;
    }
    if (last_copy_us != 0 && now_us - last_copy_us < BURST_US_BETWEEN_FRAMES) {
        return 0;
    }
    if (pictures_copied - pictures_written >= BURST_QUEUE_LENGTH) {
        dropped = dropped + 1;
        return 0;
    }
    memset(&frame_buffer, 0, sizeof(frame_buffer));
    frame_buffer.size = sizeof(frame_buffer);
    if (sceDisplayGetFrameBuf(&frame_buffer, SCE_DISPLAY_SETBUF_NEXTFRAME) < 0 || frame_buffer.base == NULL || frame_buffer.pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return 0;
    }
    width = (unsigned int)frame_buffer.width / BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL;
    height = (unsigned int)frame_buffer.height / BURST_SOURCE_PIXELS_PER_PICTURE_PIXEL;
    if (width > BURST_LARGEST_WIDTH || height > BURST_LARGEST_HEIGHT) {
        return 0;
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
    return 0;
}
