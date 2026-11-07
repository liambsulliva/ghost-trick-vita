#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"

#define BUNDLE_ANIMATION_PREFIX "animation "

static char folder[FILE_PATH_MAX];
static struct animation *loaded_animations[ANIMATION_LIBRARY_MAX];
static int loaded_count;

static const struct animation *find_loaded(const char *name)
{
    int index;

    for (index = 0; index < loaded_count; index++) {
        if (strcmp(loaded_animations[index]->name, name) == 0) {
            return loaded_animations[index];
        }
    }
    return NULL;
}

static struct animation *read_animation_file(const char *name)
{
    char path[FILE_PATH_MAX];
    unsigned char *text;
    unsigned int text_size;
    struct animation *animation;
    int result;

    if (!file_path_join(path, folder, name, ".anim")) {
        run_log_write("animation path_too_long name=%s", name);
        return NULL;
    }
    result = file_read_whole(path, &text, &text_size);
    if (result != FILE_OK) {
        run_log_write("animation missing name=%s code=%d", name, result);
        return NULL;
    }
    animation = mem_alloc(sizeof(struct animation));
    if (animation == NULL) {
        mem_free(text);
        run_log_write("animation out_of_memory name=%s", name);
        return NULL;
    }
    result = animation_parse((const char *)text, text_size, animation);
    mem_free(text);
    if (result != ANIMATION_OK) {
        mem_free(animation);
        run_log_write("animation invalid name=%s code=%d", name, result);
        return NULL;
    }
    snprintf(animation->name, sizeof(animation->name), "%s", name);
    return animation;
}

void animation_library_init(const char *animation_folder)
{
    snprintf(folder, sizeof(folder), "%s", animation_folder);
    loaded_count = 0;
}

const struct animation *animation_library_load(const char *name)
{
    const struct animation *existing;
    struct animation *animation;

    existing = find_loaded(name);
    if (existing != NULL) {
        return existing;
    }
    if (loaded_count >= ANIMATION_LIBRARY_MAX) {
        run_log_write("animation library_full name=%s", name);
        return NULL;
    }
    animation = read_animation_file(name);
    if (animation == NULL) {
        return NULL;
    }
    loaded_animations[loaded_count] = animation;
    loaded_count = loaded_count + 1;
    return animation;
}

static const char *next_animation_start(const char *text, const char *end)
{
    const char *line;

    line = text;
    while (line < end) {
        const char *line_end;

        if ((unsigned int)(end - line) >= sizeof(BUNDLE_ANIMATION_PREFIX) - 1u && memcmp(line, BUNDLE_ANIMATION_PREFIX, sizeof(BUNDLE_ANIMATION_PREFIX) - 1u) == 0) {
            return line;
        }
        line_end = memchr(line, '\n', (size_t)(end - line));
        if (line_end == NULL) {
            return end;
        }
        line = line_end + 1;
    }
    return end;
}

static int add_bundled_animation(const char *start, const char *end)
{
    char name[ASSET_NAME_MAX];
    struct animation *animation;

    if (sscanf(start, BUNDLE_ANIMATION_PREFIX ASSET_NAME_SCAN, name) != 1 || find_loaded(name) != NULL || loaded_count >= ANIMATION_LIBRARY_MAX) {
        return 0;
    }
    animation = mem_alloc(sizeof(struct animation));
    if (animation == NULL) {
        return 0;
    }
    if (animation_parse(start, (unsigned int)(end - start), animation) != ANIMATION_OK) {
        mem_free(animation);
        run_log_write("animation invalid_in_bundle name=%s", name);
        return 0;
    }
    snprintf(animation->name, sizeof(animation->name), "%s", name);
    loaded_animations[loaded_count] = animation;
    loaded_count = loaded_count + 1;
    return 1;
}

int animation_library_add_bundle(const char *text, unsigned int text_size)
{
    const char *end;
    const char *start;
    int added;

    end = text + text_size;
    added = 0;
    start = next_animation_start(text, end);
    while (start < end) {
        const char *next;

        next = next_animation_start(start + sizeof(BUNDLE_ANIMATION_PREFIX) - 1u, end);
        added = added + add_bundled_animation(start, next);
        start = next;
    }
    return added;
}

int animation_library_is_full(void)
{
    return loaded_count >= ANIMATION_LIBRARY_MAX;
}

void animation_library_clear(void)
{
    int index;

    for (index = 0; index < loaded_count; index++) {
        animation_free_frames(loaded_animations[index]);
        mem_free(loaded_animations[index]);
    }
    loaded_count = 0;
}
