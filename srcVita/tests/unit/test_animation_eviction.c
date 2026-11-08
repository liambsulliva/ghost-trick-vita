#include <stdio.h>
#include <string.h>
#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "test_support.h"

static const char frame_text[] = "frame a 1 0 0 1 1 0 0 1 1\n";
static const struct animation *kept_animation;

static int keeps_only_the_kept_animation(const struct animation *animation)
{
    return animation == kept_animation;
}

static void add_animation_files(int count)
{
    char path[64];
    int index;

    fake_files_reset();
    fake_memory_reset();
    for (index = 0; index < count; index++) {
        snprintf(path, sizeof(path), "anims/a%d.anim", index);
        fake_files_add(path, (const unsigned char *)frame_text, sizeof(frame_text) - 1);
    }
}

static const struct animation *load_numbered(int number)
{
    char name[32];

    snprintf(name, sizeof(name), "a%d", number);
    return animation_library_load(name);
}

static void fill_the_library(void)
{
    int index;

    animation_library_init("anims");
    for (index = 0; index < ANIMATION_LIBRARY_MAX; index++) {
        CHECK(load_numbered(index) != NULL);
    }
}

static void test_full_library_without_a_check_refuses_new_animations(void)
{
    add_animation_files(ANIMATION_LIBRARY_MAX + 1);
    animation_library_set_in_use_check(NULL);
    fill_the_library();
    CHECK(load_numbered(ANIMATION_LIBRARY_MAX) == NULL);
    CHECK_INT(animation_library_evicted_count(), 0);
    animation_library_clear();
}

static void test_full_library_evicts_the_least_recent_animation_nothing_plays(void)
{
    add_animation_files(ANIMATION_LIBRARY_MAX + 2);
    animation_library_set_in_use_check(keeps_only_the_kept_animation);
    fill_the_library();
    kept_animation = load_numbered(0);
    CHECK(load_numbered(1) != NULL);
    CHECK(load_numbered(ANIMATION_LIBRARY_MAX) != NULL);
    CHECK_INT(animation_library_evicted_count(), 1);
    CHECK(load_numbered(0) == kept_animation);
    CHECK(load_numbered(ANIMATION_LIBRARY_MAX + 1) != NULL);
    CHECK_INT(animation_library_evicted_count(), 2);
    CHECK(load_numbered(0) == kept_animation);
    animation_library_clear();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
    animation_library_set_in_use_check(NULL);
    kept_animation = NULL;
}

int main(void)
{
    test_full_library_without_a_check_refuses_new_animations();
    test_full_library_evicts_the_least_recent_animation_nothing_plays();
    fake_files_reset();
    return test_finish("unit/animation_eviction");
}
