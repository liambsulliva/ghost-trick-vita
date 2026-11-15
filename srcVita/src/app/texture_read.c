#include <stddef.h>
#include "clock.h"
#include "file.h"
#include "memory.h"
#include "read_buffer.h"
#include "texture_file.h"
#include "texture_read.h"

struct read_place {
    unsigned char *reserve;
};

static unsigned char *make_read_buffer(void *context, unsigned int size)
{
    struct read_place *place;
    unsigned char *buffer;

    place = context;
    buffer = mem_alloc(size);
    if (buffer != NULL || size > READ_BUFFER_RESERVED_BYTES) {
        return buffer;
    }
    place->reserve = read_buffer_take_whole_reserve();
    if (place->reserve == NULL) {
        return NULL;
    }
    return place->reserve + READ_BUFFER_RESERVED_BYTES - size;
}

static void give_back_read_buffer(void *context, unsigned char *buffer)
{
    struct read_place *place;

    place = context;
    read_buffer_release(buffer);
    place->reserve = NULL;
}

static unsigned char *unpack_target(const struct read_place *place, const struct finished_read *finished, unsigned int unpacked_size)
{
    if (place->reserve == NULL) {
        return read_buffer_take(unpacked_size);
    }
    if (unpacked_size + finished->size > READ_BUFFER_RESERVED_BYTES) {
        return NULL;
    }
    return place->reserve;
}

static int unpack_texture(struct read_place *place, struct finished_read *finished)
{
    unsigned char *unpacked;
    unsigned int unpacked_size;
    int was_unpacked;

    unpacked_size = texture_file_unpacked_size(finished->data);
    unpacked = unpack_target(place, finished, unpacked_size);
    if (unpacked == NULL) {
        read_buffer_release(finished->data);
        finished->data = NULL;
        return FILE_ERR_MEMORY;
    }
    was_unpacked = texture_file_unpack(finished->data, finished->size, unpacked, unpacked_size);
    if (place->reserve == NULL) {
        mem_free(finished->data);
    }
    if (!was_unpacked) {
        read_buffer_release(unpacked);
        finished->data = NULL;
        return FILE_ERR_READ;
    }
    finished->data = unpacked;
    finished->size = unpacked_size;
    return FILE_OK;
}

void texture_read_whole(const char *path, int wants_grayscale, struct finished_read *finished)
{
    struct read_place place;
    struct file_buffers buffers;
    unsigned long long started_us;

    place.reserve = NULL;
    buffers.make = make_read_buffer;
    buffers.give_back = give_back_read_buffer;
    buffers.context = &place;
    finished->data = NULL;
    finished->size = 0;
    finished->unpack_us = 0;
    started_us = clock_now_us();
    finished->result = file_read_whole_into(path, &buffers, &finished->data, &finished->size, &finished->open_us);
    finished->read_us = clock_now_us() - started_us - finished->open_us;
    if (finished->result == FILE_OK && texture_file_is_packed(finished->data, finished->size)) {
        started_us = clock_now_us();
        finished->result = unpack_texture(&place, finished);
        finished->unpack_us = clock_now_us() - started_us;
    }
    if (finished->result == FILE_OK && wants_grayscale) {
        started_us = clock_now_us();
        background_reader_make_grayscale(finished);
        finished->unpack_us = finished->unpack_us + clock_now_us() - started_us;
    }
}
