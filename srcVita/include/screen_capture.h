#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#define SCREEN_CAPTURE_OK 0
#define SCREEN_CAPTURE_ERR_DISPLAY -1
#define SCREEN_CAPTURE_ERR_FORMAT -2
#define SCREEN_CAPTURE_ERR_MEMORY -3
#define SCREEN_CAPTURE_ERR_WRITE -4

int screen_capture_save_bmp(const char *path);

#endif
