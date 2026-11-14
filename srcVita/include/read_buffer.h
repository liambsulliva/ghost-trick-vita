#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#define READ_BUFFER_LARGE_LEAST_BYTES (4u * 1024u * 1024u + 1u)
#define READ_BUFFER_RESERVED_BYTES (16u * 1024u * 1024u + 64u)

int read_buffer_reserve(void);
void read_buffer_forget_reserve(void);
unsigned char *read_buffer_take(unsigned int size);
void read_buffer_release(unsigned char *data);
int read_buffer_reserve_is_free(void);

#endif
