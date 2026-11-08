#include <stdlib.h>
#include "memory.h"

unsigned int _newlib_heap_size_user = MEMORY_HEAP_BYTES;

void *mem_alloc(unsigned int size)
{
    if (size == 0) {
        return NULL;
    }
    return malloc(size);
}

void mem_free(void *block)
{
    free(block);
}
