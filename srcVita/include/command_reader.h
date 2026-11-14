#ifndef COMMAND_READER_H
#define COMMAND_READER_H

int command_reader_start(const char *path);
int command_reader_take(unsigned char **text, unsigned int *size);

#endif
