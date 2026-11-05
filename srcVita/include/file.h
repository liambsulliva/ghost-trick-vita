#ifndef FILE_H
#define FILE_H

#define FILE_APP_PREFIX "app0:/"
#define FILE_PATH_MAX 256

#define FILE_OK 0
#define FILE_ERR_PATH -1
#define FILE_ERR_OPEN -2
#define FILE_ERR_SIZE -3
#define FILE_ERR_MEMORY -4
#define FILE_ERR_READ -5

int file_read_whole(const char *path, unsigned char **data, unsigned int *size);
int file_read_app(const char *name, unsigned char **data, unsigned int *size);

#endif
