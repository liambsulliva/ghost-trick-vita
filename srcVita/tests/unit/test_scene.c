#include <stdio.h>
#include "scene.h"
#include "test_support.h"

static int parse_scene_text(const char *text)
{
    static struct scene scene;

    return scene_parse(text, (unsigned int)strlen(text), &scene);
}

static void test_scene_parse_reads_every_kind_of_step(void)
{
    const char text[] =
        "scene opening\n"
        "side_bars left right\n"
        "room 11\n"
        "plate st01 0 384 256 192\n"
        "object linne idle 281.5 353 900 1\n"
        "remove_object linne\n"
        "camera 0 192\n"
        "camera_pan 768 224 600\n"
        "card chapter\n"
        "hide_card\n"
        "fade_out 20\n"
        "fade_in 40\n"
        "wait 90\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.step_count, 12);
    CHECK_INT(scene.steps[3].kind, SCENE_STEP_OBJECT);
    CHECK_INT(scene.steps[6].kind, SCENE_STEP_CAMERA_PAN);
    CHECK_INT(scene.steps[6].ticks, 600);
    CHECK_INT(scene.steps[11].kind, SCENE_STEP_WAIT);
    CHECK_INT(scene.steps[11].ticks, 90);
}

static void test_scene_parse_rejects_durations_that_are_not_positive(void)
{
    CHECK_INT(parse_scene_text("scene s\ncamera_pan 0 0 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nfade_out 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nfade_in -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nwait 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nnot_a_step 1\n"), SCENE_ERR_BAD_LINE);
}

static void test_scene_parse_stops_at_the_step_limit(void)
{
    static char text[32 + (SCENE_STEP_MAX + 1) * 10];
    size_t used;
    int index;

    used = (size_t)sprintf(text, "scene s\n");
    for (index = 0; index < SCENE_STEP_MAX; index++) {
        used = used + (size_t)sprintf(&text[used], "hide_card\n");
    }
    CHECK_INT(parse_scene_text(text), SCENE_OK);
    sprintf(&text[used], "hide_card\n");
    CHECK_INT(parse_scene_text(text), SCENE_ERR_TOO_MANY_STEPS);
}

int main(void)
{
    test_scene_parse_reads_every_kind_of_step();
    test_scene_parse_rejects_durations_that_are_not_positive();
    test_scene_parse_stops_at_the_step_limit();
    return test_finish("unit/scene");
}
