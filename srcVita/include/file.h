#ifndef FILE_H
#define FILE_H

#define FILE_DATA_FOLDER "ux0:data/ghosttrick"
#define FILE_PATH_MAX 256

#define FILE_OK 0
#define FILE_ERR_OPEN -2
#define FILE_ERR_SIZE -3
#define FILE_ERR_MEMORY -4
#define FILE_ERR_READ -5
#define FILE_ERR_WRITE -6

int file_read_whole(const char *path, unsigned char **data, unsigned int *size);
struct file_buffers {
    unsigned char *(*make)(void *context, unsigned int size);
    void (*give_back)(void *context, unsigned char *buffer);
    void *context;
};

int file_read_whole_into(const char *path, const struct file_buffers *buffers, unsigned char **data, unsigned int *size, unsigned long long *open_us);
int file_read_whole_timing_open(const char *path, unsigned char **data, unsigned int *size, unsigned long long *open_us);
int file_write_whole(const char *path, const unsigned char *data, unsigned int size);
int file_append(const char *path, const unsigned char *data, unsigned int size);
int file_delete(const char *path);
int file_make_folder(const char *path);

typedef void (*file_name_visitor)(void *context, const char *name);

int file_list_folder(const char *path, file_name_visitor visit, void *context);

#endif
