#ifndef ANIMATION_LIBRARY_H
#define ANIMATION_LIBRARY_H

#include "animation.h"

#define ANIMATION_LIBRARY_MAX 500
#define ANIMATION_BUNDLE_MAX 512

struct animation_bundle {
    struct animation *animations[ANIMATION_BUNDLE_MAX];
    int count;
};

typedef int (*animation_library_in_use_check)(const struct animation *animation);

void animation_library_init(const char *animation_folder);
void animation_library_set_in_use_check(animation_library_in_use_check check);
int animation_library_evicted_count(void);
const struct animation *animation_library_load(const char *name);
void animation_library_clear(void);
int animation_library_is_full(void);
int animation_library_add_bundle(const char *text, unsigned int text_size);
int animation_bundle_prepare(struct animation_bundle *bundle, const char *text, unsigned int text_size);
void animation_bundle_free(struct animation_bundle *bundle);
int animation_library_add_prepared(struct animation_bundle *bundle);

#endif
