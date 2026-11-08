#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_HEAP_BYTES (112u * 1024u * 1024u)

void *mem_alloc(unsigned int size);
void mem_free(void *block);

#endif
