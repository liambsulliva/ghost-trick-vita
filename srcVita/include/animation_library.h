#ifndef ANIMATION_LIBRARY_H
#define ANIMATION_LIBRARY_H

#include "animation.h"

#define ANIMATION_LIBRARY_MAX 500

typedef int (*animation_library_in_use_check)(const struct animation *animation);

void animation_library_init(const char *animation_folder);
void animation_library_set_in_use_check(animation_library_in_use_check check);
int animation_library_evicted_count(void);
const struct animation *animation_library_load(const char *name);
void animation_library_clear(void);
int animation_library_is_full(void);
int animation_library_add_bundle(const char *text, unsigned int text_size);

#endif
