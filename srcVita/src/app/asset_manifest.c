#include <stdio.h>
#include <string.h>
#include "asset_manifest.h"
#include "text_lines.h"

#define MANIFEST_LINE_MAX 160
#define SCENE_LINE_START "scene "
#define BACKGROUND_LINE_START "background "
#define ANIMATION_LINE_START "animation "

static int read_asset_name(const char *name_text, char names[][ASSET_NAME_MAX], int *count, int limit)
{
    if (*count >= limit) {
        return ASSET_MANIFEST_ERR_TOO_MANY;
    }
    if (sscanf(name_text, ASSET_NAME_SCAN, names[*count]) != 1) {
        return ASSET_MANIFEST_ERR_BAD_LINE;
    }
    *count = *count + 1;
    return ASSET_MANIFEST_OK;
}

static int read_one_line(const char *line, struct asset_manifest *manifest)
{
    if (line[0] == '\0') {
        return ASSET_MANIFEST_OK;
    }
    if (text_line_starts_with(line, SCENE_LINE_START)) {
        return read_asset_name(line + strlen(SCENE_LINE_START), manifest->scene_names, &manifest->scene_count, ASSET_MANIFEST_SCENE_MAX);
    }
    if (text_line_starts_with(line, BACKGROUND_LINE_START)) {
        return read_asset_name(line + strlen(BACKGROUND_LINE_START), manifest->background_names, &manifest->background_count, ASSET_MANIFEST_BACKGROUND_MAX);
    }
    if (text_line_starts_with(line, ANIMATION_LINE_START)) {
        return read_asset_name(line + strlen(ANIMATION_LINE_START), manifest->animation_names, &manifest->animation_count, ASSET_MANIFEST_ANIMATION_MAX);
    }
    return ASSET_MANIFEST_ERR_BAD_LINE;
}

int asset_manifest_parse(const char *text, unsigned int text_size, struct asset_manifest *manifest)
{
    char line[MANIFEST_LINE_MAX];
    unsigned int offset;
    int result;

    manifest->scene_count = 0;
    manifest->background_count = 0;
    manifest->animation_count = 0;
    offset = 0;
    while (offset < text_size) {
        offset = text_lines_read_next(text, text_size, offset, line, sizeof(line));
        result = read_one_line(line, manifest);
        if (result != ASSET_MANIFEST_OK) {
            return result;
        }
    }
    return ASSET_MANIFEST_OK;
}

int asset_manifest_find_background(const struct asset_manifest *manifest, const char *name)
{
    int index;

    for (index = 0; index < manifest->background_count; index++) {
        if (strcmp(manifest->background_names[index], name) == 0) {
            return index;
        }
    }
    return -1;
}

int asset_manifest_find_animation(const struct asset_manifest *manifest, const char *name)
{
    int index;

    for (index = 0; index < manifest->animation_count; index++) {
        if (strcmp(manifest->animation_names[index], name) == 0) {
            return index;
        }
    }
    return -1;
}
