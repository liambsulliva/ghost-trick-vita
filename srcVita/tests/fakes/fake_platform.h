#ifndef FAKE_PLATFORM_H
#define FAKE_PLATFORM_H

void fake_clock_set_us(unsigned long long now_us);
void fake_clock_advance_us(unsigned long long elapsed_us);

void fake_files_reset(void);
void fake_files_add(const char *path, const unsigned char *data, unsigned int size);
const unsigned char *fake_files_contents(const char *path, unsigned int *size);
int fake_files_exists(const char *path);
void fake_files_fail_writes(int should_fail);

#endif
