#ifndef LOOP_FILE_H
#define LOOP_FILE_H

#define LOOP_FILE_NONE 0
#define LOOP_FILE_OK 1
#define LOOP_FILE_BAD 2

int loop_file_read(const char *folder, const char *name, unsigned int frame_count, unsigned int *loop_start, unsigned int *loop_last);

#endif
