#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_GENERAL_HEAP_BYTES (112u * 1024u * 1024u)
#define MEMORY_HEAP_BYTES (MEMORY_GENERAL_HEAP_BYTES + 16u * 1024u * 1024u)

void *mem_alloc(unsigned int size);
void mem_free(void *block);

#endif
