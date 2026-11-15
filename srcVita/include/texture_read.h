#ifndef TEXTURE_READ_H
#define TEXTURE_READ_H

#include "background_reader.h"

void texture_read_whole(const char *path, int wants_grayscale, struct finished_read *finished);

#endif
