#ifndef ANIMATION_LIBRARY_H
#define ANIMATION_LIBRARY_H

#include "animation.h"

#define ANIMATION_LIBRARY_MAX 256

void animation_library_init(const char *animation_folder);
const struct animation *animation_library_load(const char *name);
void animation_library_clear(void);
int animation_library_is_full(void);

#endif
