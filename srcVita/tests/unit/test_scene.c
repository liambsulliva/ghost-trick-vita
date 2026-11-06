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
    CHECK_INT(scene.step_count, 11);
    CHECK_INT(scene.steps[2].kind, SCENE_STEP_OBJECT);
    CHECK_INT(scene.steps[5].kind, SCENE_STEP_CAMERA_PAN);
    CHECK_INT(scene.steps[5].ticks, 600);
    CHECK_INT(scene.steps[10].kind, SCENE_STEP_WAIT);
    CHECK_INT(scene.steps[10].ticks, 90);
}

static void test_scene_parse_reads_beam_and_mask_steps(void)
{
    const char text[] =
        "scene opening\n"
        "mask_on\n"
        "beam_create spot_a -16 64 108 542 60.5\n"
        "beam_animate spot_a 118 367 30 60\n"
        "beam_delete spot_a\n"
        "mask_off\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.step_count, 5);
    CHECK_INT(scene.steps[0].kind, SCENE_STEP_MASK_ON);
    CHECK_INT(scene.steps[1].kind, SCENE_STEP_BEAM_CREATE);
    CHECK_TEXT(scene.steps[1].beam_name, "spot_a");
    CHECK(scene.steps[1].apex_x == -16.0f);
    CHECK(scene.steps[1].apex_y == 64.0f);
    CHECK(scene.steps[1].x == 108.0f);
    CHECK(scene.steps[1].y == 542.0f);
    CHECK(scene.steps[1].half_width == 60.5f);
    CHECK_INT(scene.steps[2].kind, SCENE_STEP_BEAM_ANIMATE);
    CHECK_TEXT(scene.steps[2].beam_name, "spot_a");
    CHECK(scene.steps[2].x == 118.0f);
    CHECK(scene.steps[2].y == 367.0f);
    CHECK(scene.steps[2].half_width == 30.0f);
    CHECK_INT(scene.steps[2].ticks, 60);
    CHECK_INT(scene.steps[3].kind, SCENE_STEP_BEAM_DELETE);
    CHECK_TEXT(scene.steps[3].beam_name, "spot_a");
    CHECK_INT(scene.steps[4].kind, SCENE_STEP_MASK_OFF);
}

static void test_scene_parse_reads_iris_dim_and_flash_steps(void)
{
    const char text[] =
        "scene opening\n"
        "iris 144 359 384 120\n"
        "dim_to 0.49 10\n"
        "flash 0.5 8\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.steps[0].kind, SCENE_STEP_IRIS);
    CHECK(scene.steps[0].x == 144.0f);
    CHECK(scene.steps[0].y == 359.0f);
    CHECK(scene.steps[0].radius == 384.0f);
    CHECK_INT(scene.steps[0].ticks, 120);
    CHECK_INT(scene.steps[1].kind, SCENE_STEP_DIM_TO);
    CHECK(scene.steps[1].brightness == 0.49f);
    CHECK_INT(scene.steps[1].ticks, 10);
    CHECK_INT(scene.steps[2].kind, SCENE_STEP_FLASH);
    CHECK(scene.steps[2].brightness == 0.5f);
    CHECK_INT(scene.steps[2].ticks, 8);
}

static void test_scene_parse_lets_effects_change_at_once(void)
{
    CHECK_INT(parse_scene_text("scene s\nbeam_animate b 0 0 0 0\n"), SCENE_OK);
    CHECK_INT(parse_scene_text("scene s\niris 0 0 0 0\n"), SCENE_OK);
    CHECK_INT(parse_scene_text("scene s\ndim_to 1 0\n"), SCENE_OK);
    CHECK_INT(parse_scene_text("scene s\nflash 1 0\n"), SCENE_OK);
}

static void test_scene_parse_rejects_bad_effect_values(void)
{
    CHECK_INT(parse_scene_text("scene s\nbeam_create b 0 0 0 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nbeam_create b 0 0 0 0 -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nbeam_animate b 0 0 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nbeam_animate b 0 0 -1 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nbeam_animate b 0 0 0 -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nbeam_delete\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmask_on now\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmask_off now\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\niris 0 0 0\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\niris 0 0 -1 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\niris 0 0 1 -5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ndim_to 1.5 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ndim_to -0.5 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ndim_to 1 -5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nflash 1.5 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nflash -0.5 5\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nflash 1 -5\n"), SCENE_ERR_BAD_LINE);
}

static void test_scene_parse_reads_text_and_overlay_steps(void)
{
    const char text[] =
        "scene opening\n"
        "text_show 3 line_a 480 72.5 60\n"
        "text_hide 3 45\n"
        "text_hide_all\n"
        "overlay 2 bubble 98.5 242.25 118 38.5 6\n"
        "overlay_hide 2\n"
        "text_show 4 line_b 480 90 0\n"
        "text_hide 4 0\n"
        "overlay 5 bubble 1 2 3 4 0\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.step_count, 8);
    CHECK_INT(scene.steps[0].kind, SCENE_STEP_TEXT_SHOW);
    CHECK_INT(scene.steps[0].layer_item_id, 3);
    CHECK_TEXT(scene.steps[0].texture_name, "line_a");
    CHECK(scene.steps[0].x == 480.0f);
    CHECK(scene.steps[0].y == 72.5f);
    CHECK_INT(scene.steps[0].ticks, 60);
    CHECK_INT(scene.steps[1].kind, SCENE_STEP_TEXT_HIDE);
    CHECK_INT(scene.steps[1].layer_item_id, 3);
    CHECK_INT(scene.steps[1].ticks, 45);
    CHECK_INT(scene.steps[2].kind, SCENE_STEP_TEXT_HIDE_ALL);
    CHECK_INT(scene.steps[3].kind, SCENE_STEP_OVERLAY);
    CHECK_INT(scene.steps[3].layer_item_id, 2);
    CHECK_TEXT(scene.steps[3].texture_name, "bubble");
    CHECK(scene.steps[3].x == 98.5f);
    CHECK(scene.steps[3].y == 242.25f);
    CHECK(scene.steps[3].width == 118.0f);
    CHECK(scene.steps[3].height == 38.5f);
    CHECK_INT(scene.steps[3].ticks, 6);
    CHECK_INT(scene.steps[4].kind, SCENE_STEP_OVERLAY_HIDE);
    CHECK_INT(scene.steps[4].layer_item_id, 2);
    CHECK_INT(scene.steps[5].ticks, 0);
    CHECK_INT(scene.steps[6].ticks, 0);
    CHECK_INT(scene.steps[7].ticks, 0);
}

static void test_scene_parse_rejects_bad_text_and_overlay_steps(void)
{
    CHECK_INT(parse_scene_text("scene s\ntext_show 1 line 480 72\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ntext_show 1 line 480 72 -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ntext_hide 1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ntext_hide 1 -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\ntext_hide_all now\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\noverlay 1 art 0 0 8 8\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\noverlay 1 art 0 0 8 8 -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\noverlay_hide\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\noverlay_hide x\n"), SCENE_ERR_BAD_LINE);
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

static void test_scene_parse_reads_audio_steps(void)
{
    const char text[] =
        "scene opening\n"
        "music song_a 1\n"
        "music other 0\n"
        "music_stop 4\n"
        "music_stop 0\n"
        "sound ping_a\n";
    static struct scene scene;

    CHECK_INT(scene_parse(text, sizeof(text) - 1, &scene), SCENE_OK);
    CHECK_INT(scene.step_count, 5);
    CHECK_INT(scene.steps[0].kind, SCENE_STEP_MUSIC);
    CHECK_TEXT(scene.steps[0].audio_name, "song_a");
    CHECK_INT(scene.steps[0].is_looping, 1);
    CHECK_INT(scene.steps[1].is_looping, 0);
    CHECK_INT(scene.steps[2].kind, SCENE_STEP_MUSIC_STOP);
    CHECK_INT(scene.steps[2].ticks, 4);
    CHECK_INT(scene.steps[3].ticks, 0);
    CHECK_INT(scene.steps[4].kind, SCENE_STEP_SOUND);
    CHECK_TEXT(scene.steps[4].audio_name, "ping_a");
}

static void test_scene_parse_rejects_bad_audio_steps(void)
{
    CHECK_INT(parse_scene_text("scene s\nmusic song\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmusic song 2\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmusic song -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmusic song yes\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmusic_stop -1\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nmusic_stop\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nsound\n"), SCENE_ERR_BAD_LINE);
    CHECK_INT(parse_scene_text("scene s\nsound \n"), SCENE_ERR_BAD_LINE);
}

int main(void)
{
    test_scene_parse_reads_every_kind_of_step();
    test_scene_parse_reads_beam_and_mask_steps();
    test_scene_parse_reads_iris_dim_and_flash_steps();
    test_scene_parse_lets_effects_change_at_once();
    test_scene_parse_rejects_bad_effect_values();
    test_scene_parse_reads_audio_steps();
    test_scene_parse_rejects_bad_audio_steps();
    test_scene_parse_reads_text_and_overlay_steps();
    test_scene_parse_rejects_bad_text_and_overlay_steps();
    test_scene_parse_rejects_durations_that_are_not_positive();
    test_scene_parse_stops_at_the_step_limit();
    return test_finish("unit/scene");
}
