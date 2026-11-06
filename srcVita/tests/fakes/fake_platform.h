#ifndef FAKE_PLATFORM_H
#define FAKE_PLATFORM_H

void fake_clock_set_us(unsigned long long now_us);
void fake_clock_advance_us(unsigned long long elapsed_us);
void fake_clock_set_tick_per_read_us(unsigned long long tick_us);
unsigned long long fake_clock_peek_us(void);

void fake_files_reset(void);
void fake_files_add(const char *path, const unsigned char *data, unsigned int size);
const unsigned char *fake_files_contents(const char *path, unsigned int *size);
int fake_files_exists(const char *path);
void fake_files_fail_writes(int should_fail);

void fake_memory_reset(void);
void fake_memory_set_largest_block(unsigned int bytes);
int fake_memory_blocks_in_use(void);

#endif
