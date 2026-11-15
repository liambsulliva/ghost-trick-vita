#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asset_language.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"
#include "text_lines.h"

#define TEXTURE_FOLDER "textures"
#define TEXTURE_EXTENSION ".gtex"

static const char *const codes[ASSET_LANGUAGE_COUNT] = { "en", "ja", "fr", "it", "de", "es", "zhtw", "zhcn", "ko" };
static const int system_languages[ASSET_LANGUAGE_COUNT] = { 1, 0, 2, 5, 4, 3, 10, 11, 9 };

static char assets_folder[FILE_PATH_MAX];
static char overrides[ASSET_LANGUAGE_OVERRIDE_MAX][ASSET_LANGUAGE_PATH_MAX];
static int override_count;
static int current_language;

static int compare_paths(const void *left, const void *right)
{
    return strcmp((const char *)left, (const char *)right);
}

void asset_language_init(const char *folder)
{
    snprintf(assets_folder, sizeof(assets_folder), "%s", folder);
    override_count = 0;
    current_language = ASSET_LANGUAGE_ENGLISH;
}

static int language_folder(char *path, const char *folder, int language)
{
    int written;

    written = snprintf(path, FILE_PATH_MAX, "%s/%s/%s", folder, ASSET_LANGUAGE_FOLDER, codes[language]);
    return written > 0 && written < FILE_PATH_MAX;
}

static void read_overrides(int language)
{
    char folder[FILE_PATH_MAX];
    char path[FILE_PATH_MAX];
    char line[ASSET_LANGUAGE_PATH_MAX];
    unsigned char *bytes;
    unsigned int size;
    unsigned int offset;

    override_count = 0;
    if (language == ASSET_LANGUAGE_ENGLISH || !language_folder(folder, assets_folder, language) || !file_path_join(path, folder, ASSET_LANGUAGE_NAMES_FILE, "") || file_read_whole(path, &bytes, &size) != FILE_OK) {
        return;
    }
    offset = 0;
    while (offset < size && override_count < ASSET_LANGUAGE_OVERRIDE_MAX) {
        offset = text_lines_read_next((const char *)bytes, size, offset, line, sizeof(line));
        if (line[0] != '\0') {
            snprintf(overrides[override_count], ASSET_LANGUAGE_PATH_MAX, "%s", line);
            override_count = override_count + 1;
        }
    }
    mem_free(bytes);
    qsort(overrides, (size_t)override_count, sizeof(overrides[0]), compare_paths);
}

int asset_language_use(int language)
{
    if (language < 0 || language >= ASSET_LANGUAGE_COUNT) {
        language = ASSET_LANGUAGE_ENGLISH;
    }
    current_language = language;
    read_overrides(language);
    run_log_write("language use=%s overrides=%d", codes[language], override_count);
    return override_count;
}

int asset_language_now(void)
{
    return current_language;
}

const char *asset_language_code(int language)
{
    if (language < 0 || language >= ASSET_LANGUAGE_COUNT) {
        return codes[ASSET_LANGUAGE_ENGLISH];
    }
    return codes[language];
}

int asset_language_overrides(const char *relative_path)
{
    return override_count > 0 && bsearch(relative_path, overrides, (size_t)override_count, sizeof(overrides[0]), compare_paths) != NULL;
}

int asset_language_join(char *path, const char *folder, const char *relative_path)
{
    char overlay[FILE_PATH_MAX];

    if (asset_language_overrides(relative_path) && language_folder(overlay, folder, current_language)) {
        return file_path_join(path, overlay, relative_path, "");
    }
    return file_path_join(path, folder, relative_path, "");
}

int asset_language_join_assets(char *path, const char *relative_path)
{
    return asset_language_join(path, assets_folder, relative_path);
}

static int texture_relative_path(char *relative_path, const char *texture_name)
{
    int written;

    written = snprintf(relative_path, ASSET_LANGUAGE_PATH_MAX, "%s/%s%s", TEXTURE_FOLDER, texture_name, TEXTURE_EXTENSION);
    return written > 0 && written < ASSET_LANGUAGE_PATH_MAX;
}

int asset_language_overrides_texture(const char *texture_name)
{
    char relative_path[ASSET_LANGUAGE_PATH_MAX];

    return override_count > 0 && texture_relative_path(relative_path, texture_name) && asset_language_overrides(relative_path);
}

int asset_language_texture_folder(char *folder)
{
    char overlay[FILE_PATH_MAX];

    if (override_count == 0 || !language_folder(overlay, assets_folder, current_language)) {
        folder[0] = '\0';
        return 0;
    }
    return file_path_join(folder, overlay, TEXTURE_FOLDER, "");
}

int asset_language_from_system(int system_language)
{
    int language;

    for (language = 0; language < ASSET_LANGUAGE_COUNT; language++) {
        if (system_languages[language] == system_language) {
            return language;
        }
    }
    return ASSET_LANGUAGE_ENGLISH;
}
