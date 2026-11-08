#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "crc32.h"
#include "file.h"
#include "file_path.h"
#include "memory.h"
#include "run_log.h"

#define BUNDLE_ANIMATION_PREFIX "animation "
#define NAME_BUCKET_COUNT 1024u
#define NO_SLOT (-1)

static char folder[FILE_PATH_MAX];
static struct animation *loaded_animations[ANIMATION_LIBRARY_MAX];
static unsigned long long last_used[ANIMATION_LIBRARY_MAX];
static int first_slot_in_bucket[NAME_BUCKET_COUNT];
static int next_slot_in_bucket[ANIMATION_LIBRARY_MAX];
static int is_unused_now[ANIMATION_LIBRARY_MAX];
static unsigned long long use_clock;
static int loaded_count;
static int evicted_count;
static animation_library_in_use_check in_use_check;

static void note_use(int index)
{
    use_clock = use_clock + 1;
    last_used[index] = use_clock;
}

static unsigned int name_bucket(const char *name)
{
    return crc32_add(0u, (const unsigned char *)name, (unsigned int)strlen(name)) % NAME_BUCKET_COUNT;
}

static void forget_all_names(void)
{
    unsigned int bucket;

    for (bucket = 0; bucket < NAME_BUCKET_COUNT; bucket++) {
        first_slot_in_bucket[bucket] = NO_SLOT;
    }
}

static void remember_name_of_slot(int slot)
{
    unsigned int bucket;

    bucket = name_bucket(loaded_animations[slot]->name);
    next_slot_in_bucket[slot] = first_slot_in_bucket[bucket];
    first_slot_in_bucket[bucket] = slot;
}

static void forget_name_of_slot(int slot)
{
    int *link;

    for (link = &first_slot_in_bucket[name_bucket(loaded_animations[slot]->name)]; *link != NO_SLOT; link = &next_slot_in_bucket[*link]) {
        if (*link == slot) {
            *link = next_slot_in_bucket[slot];
            return;
        }
    }
}

static const struct animation *find_loaded(const char *name)
{
    int slot;

    for (slot = first_slot_in_bucket[name_bucket(name)]; slot != NO_SLOT; slot = next_slot_in_bucket[slot]) {
        if (strcmp(loaded_animations[slot]->name, name) == 0) {
            note_use(slot);
            return loaded_animations[slot];
        }
    }
    return NULL;
}

static void evict_slot(int slot)
{
    int last;

    run_log_write("animation evicted name=%s", loaded_animations[slot]->name);
    forget_name_of_slot(slot);
    animation_free_frames(loaded_animations[slot]);
    mem_free(loaded_animations[slot]);
    last = loaded_count - 1;
    if (slot != last) {
        forget_name_of_slot(last);
        loaded_animations[slot] = loaded_animations[last];
        last_used[slot] = last_used[last];
        is_unused_now[slot] = is_unused_now[last];
        remember_name_of_slot(slot);
    }
    loaded_count = last;
    evicted_count = evicted_count + 1;
}

static int mark_unused_slots(void)
{
    int slot;
    int unused;

    unused = 0;
    for (slot = 0; slot < loaded_count; slot++) {
        is_unused_now[slot] = in_use_check != NULL && !in_use_check(loaded_animations[slot]);
        unused = unused + is_unused_now[slot];
    }
    return unused;
}

static int least_recent_unused_slot(void)
{
    int slot;
    int oldest;

    oldest = NO_SLOT;
    for (slot = 0; slot < loaded_count; slot++) {
        if (is_unused_now[slot] && (oldest == NO_SLOT || last_used[slot] < last_used[oldest])) {
            oldest = slot;
        }
    }
    return oldest;
}

static void evict_to_make_room_for(int animations_wanted)
{
    int to_evict;
    int slot;

    to_evict = loaded_count + animations_wanted - ANIMATION_LIBRARY_MAX;
    if (to_evict <= 0 || mark_unused_slots() == 0) {
        return;
    }
    for (; to_evict > 0; to_evict--) {
        slot = least_recent_unused_slot();
        if (slot == NO_SLOT) {
            return;
        }
        evict_slot(slot);
    }
}

static int evict_least_recent_unused(void)
{
    int slot;

    if (mark_unused_slots() == 0) {
        return 0;
    }
    slot = least_recent_unused_slot();
    if (slot == NO_SLOT) {
        return 0;
    }
    evict_slot(slot);
    return 1;
}

static void add_loaded(struct animation *animation)
{
    loaded_animations[loaded_count] = animation;
    remember_name_of_slot(loaded_count);
    note_use(loaded_count);
    loaded_count = loaded_count + 1;
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
    run_log_write("animation read_from_card name=%s", name);
    return animation;
}

void animation_library_init(const char *animation_folder)
{
    snprintf(folder, sizeof(folder), "%s", animation_folder);
    forget_all_names();
    loaded_count = 0;
    evicted_count = 0;
    use_clock = 0;
}

void animation_library_set_in_use_check(animation_library_in_use_check check)
{
    in_use_check = check;
}

int animation_library_evicted_count(void)
{
    return evicted_count;
}

const struct animation *animation_library_load(const char *name)
{
    const struct animation *existing;
    struct animation *animation;

    existing = find_loaded(name);
    if (existing != NULL) {
        return existing;
    }
    if (loaded_count >= ANIMATION_LIBRARY_MAX && !evict_least_recent_unused()) {
        run_log_write("animation library_full name=%s", name);
        return NULL;
    }
    animation = read_animation_file(name);
    if (animation == NULL) {
        return NULL;
    }
    add_loaded(animation);
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

static int read_bundled_name(const char *start, const char *end, char *name)
{
    const char *text;
    size_t length;

    text = start + sizeof(BUNDLE_ANIMATION_PREFIX) - 1u;
    while (text < end && (*text == ' ' || *text == '\t')) {
        text = text + 1;
    }
    length = 0;
    while (text + length < end && length + 1u < ASSET_NAME_MAX && text[length] != ' ' && text[length] != '\t' && text[length] != '\r' && text[length] != '\n') {
        name[length] = text[length];
        length = length + 1;
    }
    name[length] = '\0';
    return length > 0;
}

static int add_bundled_animation(const char *start, const char *end)
{
    char name[ASSET_NAME_MAX];
    struct animation *animation;

    if (!read_bundled_name(start, end, name) || find_loaded(name) != NULL) {
        return 0;
    }
    if (loaded_count >= ANIMATION_LIBRARY_MAX && !evict_least_recent_unused()) {
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
    add_loaded(animation);
    return 1;
}

static int bundled_animations_not_loaded(const char *text, const char *end)
{
    char name[ASSET_NAME_MAX];
    const char *start;
    int missing;

    missing = 0;
    for (start = next_animation_start(text, end); start < end; start = next_animation_start(start + sizeof(BUNDLE_ANIMATION_PREFIX) - 1u, end)) {
        if (read_bundled_name(start, end, name) && find_loaded(name) == NULL) {
            missing = missing + 1;
        }
    }
    return missing;
}

int animation_library_add_bundle(const char *text, unsigned int text_size)
{
    const char *end;
    const char *start;
    int added;

    end = text + text_size;
    added = 0;
    evict_to_make_room_for(bundled_animations_not_loaded(text, end));
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
    evicted_count = evicted_count + loaded_count;
    loaded_count = 0;
    forget_all_names();
}
