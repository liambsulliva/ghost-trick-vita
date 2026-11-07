#include <string.h>
#include "test_support.h"
#include "ui_clips.h"

static struct ui_clips clips;

static int read_text(const char *text)
{
    return ui_clips_read(&clips, text, (unsigned int)strlen(text));
}

static void test_reads_clips_elements_and_tracks(void)
{
    const struct ui_clip *clip;
    const struct ui_element *element;

    CHECK(read_text("clip A 10\nelement sprite pic 1 0.5 0\ntrack x 0 100 10 200\nelement rect - 1 1 1\nclip B 5\nelement band - 1 0 0\ntrack h 0 1\n"));
    CHECK_INT(clips.clip_count, 2);
    clip = ui_clips_find(&clips, "A");
    CHECK(clip != NULL);
    CHECK_INT(clip->frame_count, 10);
    CHECK_INT(clip->element_count, 2);
    element = &clips.elements[clip->first_element];
    CHECK_INT(element->kind, UI_ELEMENT_SPRITE);
    CHECK_TEXT(element->texture_name, "pic");
    CHECK(element->green == 0.5f);
    CHECK_INT(clips.elements[clip->first_element + 1].kind, UI_ELEMENT_RECT);
    CHECK(ui_clips_find(&clips, "C") == NULL);
}

static void test_values_interpolate_and_clamp(void)
{
    const struct ui_element *element;

    CHECK(read_text("clip A 10\nelement sprite pic 1 1 1\ntrack x 2 100 4 200 6 0\n"));
    element = &clips.elements[0];
    CHECK(ui_element_value(&clips, element, UI_PROP_X, 0.0f, -1.0f) == 100.0f);
    CHECK(ui_element_value(&clips, element, UI_PROP_X, 3.0f, -1.0f) == 150.0f);
    CHECK(ui_element_value(&clips, element, UI_PROP_X, 5.0f, -1.0f) == 100.0f);
    CHECK(ui_element_value(&clips, element, UI_PROP_X, 9.0f, -1.0f) == 0.0f);
    CHECK(ui_element_value(&clips, element, UI_PROP_ALPHA, 3.0f, 0.75f) == 0.75f);
}

static void test_rejects_bad_lines(void)
{
    CHECK(!read_text("element sprite pic 1 1 1\n"));
    CHECK(!read_text("clip A 10\nelement wobble pic 1 1 1\n"));
    CHECK(!read_text("clip A 10\nelement sprite pic 1 1 1\ntrack spin 0 1\n"));
    CHECK(!read_text("clip A 10\nelement sprite pic 1 1 1\ntrack x 0\n"));
    CHECK(!read_text("nonsense\n"));
}

int main(void)
{
    test_reads_clips_elements_and_tracks();
    test_values_interpolate_and_clamp();
    test_rejects_bad_lines();
    return test_finish("unit/ui_clips");
}
