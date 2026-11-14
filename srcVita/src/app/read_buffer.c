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

unsigned char *read_buffer_take(unsigned int size)
{
    if (size >= READ_BUFFER_LARGE_LEAST_BYTES && size <= READ_BUFFER_RESERVED_BYTES && reserved != NULL && __sync_bool_compare_and_swap(&reserved_is_taken, 0, 1)) {
        return reserved;
    }
    return mem_alloc(size);
}

void read_buffer_release(unsigned char *data)
{
    if (data != NULL && data == reserved) {
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
