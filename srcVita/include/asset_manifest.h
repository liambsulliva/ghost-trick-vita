#ifndef ASSET_MANIFEST_H
#define ASSET_MANIFEST_H

#include "asset_name.h"

#define ASSET_MANIFEST_SCENE_MAX 16
#define ASSET_MANIFEST_BACKGROUND_MAX 512
#define ASSET_MANIFEST_ANIMATION_MAX 512

#define ASSET_MANIFEST_OK 0
#define ASSET_MANIFEST_ERR_BAD_LINE -1
#define ASSET_MANIFEST_ERR_TOO_MANY -2

struct asset_manifest {
    int scene_count;
    int background_count;
    int animation_count;
    char scene_names[ASSET_MANIFEST_SCENE_MAX][ASSET_NAME_MAX];
    char background_names[ASSET_MANIFEST_BACKGROUND_MAX][ASSET_NAME_MAX];
    char animation_names[ASSET_MANIFEST_ANIMATION_MAX][ASSET_NAME_MAX];
};

int asset_manifest_parse(const char *text, unsigned int text_size, struct asset_manifest *manifest);
int asset_manifest_find_background(const struct asset_manifest *manifest, const char *name);
int asset_manifest_find_animation(const struct asset_manifest *manifest, const char *name);

#endif
