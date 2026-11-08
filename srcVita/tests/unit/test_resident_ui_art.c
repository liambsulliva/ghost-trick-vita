#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "script_ui_view.h"
#include "ui_clips.h"
#include "test_support.h"

static const char *const screen_art[] = { "ui_hourglass", "side_label", "ui_select_button", "rewind_rocks_far", "rewind_rocks_mid", "rewind_rocks_near", "ui_tap_balloon", "narration_arrow", "hud_button_art", "hud_sheet" };

static void test_every_interface_texture_is_requested_before_its_screen_opens(void)
{
    static struct script_tables tables;
    size_t index;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    texture_cache_init("tex", 64u * 1024u * 1024u);
    fake_reader_hold_reads(1);
    memset(&tables, 0, sizeof(tables));
    script_ui_view_start(&tables);
    script_ui_view_advance();
    CHECK_INT(texture_cache_drawn_loading_count(), (int)(sizeof(screen_art) / sizeof(screen_art[0])));
    for (index = 0; index < sizeof(screen_art) / sizeof(screen_art[0]); index++) {
        const struct cached_texture *entry;

        entry = texture_cache_use_or_start_loading(screen_art[index]);
        CHECK(entry != NULL && entry->state == CACHED_TEXTURE_LOADING);
    }
    CHECK_INT(texture_cache_drawn_loading_count(), (int)(sizeof(screen_art) / sizeof(screen_art[0])));
    fake_reader_hold_reads(0);
}

static void test_every_clip_texture_is_requested_before_its_clip_plays(void)
{
    static struct script_tables tables;
    static const char clips[] = "clip CARD 10\nelement sprite card_clock 1 1 1\ntrack alpha 0 1\nelement rect - 1 1 1\ntrack alpha 0 1\n";
    const struct cached_texture *entry;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    texture_cache_init("tex", 64u * 1024u * 1024u);
    fake_reader_hold_reads(1);
    memset(&tables, 0, sizeof(tables));
    CHECK(ui_clips_read(&tables.ui_clips, clips, (unsigned int)strlen(clips)));
    script_ui_view_start(&tables);
    script_ui_view_advance();
    CHECK_INT(texture_cache_drawn_loading_count(), (int)(sizeof(screen_art) / sizeof(screen_art[0])) + 1);
    entry = texture_cache_use_or_start_loading("card_clock");
    CHECK(entry != NULL && entry->state == CACHED_TEXTURE_LOADING);
    fake_reader_hold_reads(0);
}

int main(void)
{
    test_every_interface_texture_is_requested_before_its_screen_opens();
    test_every_clip_texture_is_requested_before_its_clip_plays();
    fake_files_reset();
    return test_finish("unit/resident_ui_art");
}
