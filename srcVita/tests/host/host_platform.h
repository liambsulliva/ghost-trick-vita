#ifndef HOST_PLATFORM_H
#define HOST_PLATFORM_H

void host_clock_advance_us(unsigned long long elapsed_us);
void host_files_keep_device_files_in(const char *folder);

#endif
