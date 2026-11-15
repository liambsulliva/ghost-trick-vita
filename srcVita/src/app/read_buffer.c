#include <stddef.h>
#include "memory.h"
#include "read_buffer.h"

static unsigned char *reserved;
static volatile int reserved_is_taken;

int read_buffer_reserve(void)
{
    if (reserved == NULL) {
        reserved = mem_alloc(READ_BUFFER_RESERVED_BYTES);
        reserved_is_taken = 0;
    }
    return reserved != NULL;
}

void read_buffer_forget_reserve(void)
{
    mem_free(reserved);
    reserved = NULL;
    reserved_is_taken = 0;
}

static int take_reserved(unsigned int size)
{
    return size <= READ_BUFFER_RESERVED_BYTES && reserved != NULL && __sync_bool_compare_and_swap(&reserved_is_taken, 0, 1);
}

unsigned char *read_buffer_take(unsigned int size)
{
    unsigned char *data;

    if (size >= READ_BUFFER_LARGE_LEAST_BYTES && take_reserved(size)) {
        return reserved;
    }
    data = mem_alloc(size);
    if (data == NULL && take_reserved(size)) {
        return reserved;
    }
    return data;
}

int read_buffer_holds(const unsigned char *data)
{
    return reserved != NULL && data >= reserved && data < reserved + READ_BUFFER_RESERVED_BYTES;
}

unsigned char *read_buffer_take_whole_reserve(void)
{
    return take_reserved(READ_BUFFER_RESERVED_BYTES) ? reserved : NULL;
}

void read_buffer_release(unsigned char *data)
{
    if (data != NULL && read_buffer_holds(data)) {
        __sync_synchronize();
        reserved_is_taken = 0;
        return;
    }
    mem_free(data);
}

int read_buffer_reserve_is_free(void)
{
    return reserved != NULL && !reserved_is_taken;
}
