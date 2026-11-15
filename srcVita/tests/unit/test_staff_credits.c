#include <stdio.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "music_engine.h"
#include "run_log.h"
#include "staff_credits.h"
#include "test_support.h"

#define TICK_LIMIT 200000
#define PAGE_COUNT 3
#define HOLD_TICKS 120
#define FADE_IN_TICKS 25
#define FADE_OUT_TICKS 45
#define PAGE_SLACK 3

static const char rows_out_of_order[] =
    "movie 3 fx_thing 0\n"
    "staffpage 2 1 25 0 45\n"
    "staffpage 0 1 15 120 45\n"
    "staffpage 1 2 25 0 45\n"
    "staffmusic bgm35_new\n";

static struct staff_credits credits;

static void read_rows(const char *rows)
{
    staff_credits_read(&credits, rows, (unsigned int)strlen(rows));
}

static void forget_music(void)
{
    music_request_stop(0);
}

static int ticks_to_play(struct staff_credits *player)
{
    int ticks;

    for (ticks = 0; ticks < TICK_LIMIT && staff_credits_is_playing(player); ticks++) {
        staff_credits_step(player, 1);
    }
    return ticks;
}

static void test_the_rows_are_read_in_page_order_with_their_pace_and_music(void)
{
    read_rows(rows_out_of_order);
    CHECK_INT(credits.page_count, PAGE_COUNT);
    CHECK_INT(credits.pages[0].page, 0);
    CHECK_INT(credits.pages[1].page, 1);
    CHECK_INT(credits.pages[2].page, 2);
    CHECK_INT(credits.pages[0].fade_in_ticks, 15);
    CHECK_INT(credits.pages[0].hold_ticks, HOLD_TICKS);
    CHECK_INT(credits.pages[1].screen_count, 2);
    CHECK_INT(credits.pages[2].fade_out_ticks, FADE_OUT_TICKS);
    CHECK_TEXT(credits.music_name, "bgm35_new");
    CHECK(staff_credits_can_play(&credits));
    read_rows("staffpage 4 3\n");
    CHECK_INT(credits.page_count, 1);
    CHECK_INT(credits.pages[0].screen_count, 3);
    CHECK_INT(credits.pages[0].fade_in_ticks, 0);
    CHECK_INT(credits.music_name[0], '\0');
    read_rows("effect 1 card x 0 1 0 0 0\n");
    CHECK(!staff_credits_can_play(&credits));
}

static void test_every_page_plays_once_in_order_and_the_player_ends(void)
{
    int seen[PAGE_COUNT];
    int seen_count;
    int ticks;
    const struct script_staff_page_row *row;
    int ended;

    read_rows(rows_out_of_order);
    staff_credits_start(&credits);
    seen_count = 0;
    ended = 0;
    for (ticks = 0; ticks < TICK_LIMIT && !ended; ticks++) {
        row = staff_credits_page(&credits);
        if (row != NULL && (seen_count == 0 || seen[seen_count - 1] != row->page) && seen_count < PAGE_COUNT) {
            seen[seen_count] = row->page;
            seen_count = seen_count + 1;
        }
        ended = staff_credits_step(&credits, 1);
    }
    CHECK_INT(ended, 1);
    CHECK_INT(seen_count, PAGE_COUNT);
    CHECK_INT(seen[0], 0);
    CHECK_INT(seen[1], 1);
    CHECK_INT(seen[2], 2);
    CHECK(!staff_credits_is_playing(&credits));
    CHECK(staff_credits_page(&credits) == NULL);
}

static void test_each_page_fades_in_from_black_and_out_to_black(void)
{
    int ticks;
    int was_clear;

    read_rows("staffpage 0 1 25 0 45\nstaffpage 1 1 25 0 45\n");
    staff_credits_start(&credits);
    CHECK(staff_credits_black(&credits) == 1.0f);
    staff_credits_step(&credits, 1);
    CHECK(staff_credits_page_is_drawn(&credits));
    for (ticks = 0; ticks < FADE_IN_TICKS + PAGE_SLACK; ticks++) {
        staff_credits_step(&credits, 1);
    }
    CHECK(staff_credits_black(&credits) == 0.0f);
    was_clear = 0;
    for (ticks = 0; ticks < TICK_LIMIT && staff_credits_page(&credits)->page == 0; ticks++) {
        was_clear = was_clear || staff_credits_black(&credits) == 0.0f;
        staff_credits_step(&credits, 1);
    }
    CHECK(was_clear);
    CHECK(staff_credits_black(&credits) == 1.0f);
    staff_credits_stop(&credits);
}

static void test_a_page_waits_in_black_until_its_screens_are_loaded(void)
{
    int ticks;

    read_rows("staffpage 0 1 25 0 45\n");
    staff_credits_start(&credits);
    for (ticks = 0; ticks < 500; ticks++) {
        CHECK_INT(staff_credits_step(&credits, 0), 0);
    }
    CHECK_INT(credits.step, STAFF_CREDITS_LOADING);
    CHECK(!staff_credits_page_is_drawn(&credits));
    CHECK(staff_credits_black(&credits) == 1.0f);
    staff_credits_step(&credits, 1);
    CHECK_INT(credits.step, STAFF_CREDITS_FADING_IN);
    staff_credits_stop(&credits);
}

static void test_the_hold_after_a_page_adds_its_ticks(void)
{
    struct staff_credits held;
    int plain_ticks;
    int held_ticks;

    read_rows("staffpage 0 1 15 0 45\n");
    staff_credits_start(&credits);
    plain_ticks = ticks_to_play(&credits);
    staff_credits_clear(&held);
    staff_credits_read(&held, "staffpage 0 1 15 120 45\n", 24);
    staff_credits_start(&held);
    held_ticks = ticks_to_play(&held);
    CHECK(held_ticks - plain_ticks >= HOLD_TICKS);
    CHECK(held_ticks - plain_ticks <= HOLD_TICKS + 1);
}

static void test_the_credits_play_their_music_and_hand_back_silence(void)
{
    struct music_memory now;

    forget_music();
    read_rows(rows_out_of_order);
    staff_credits_start(&credits);
    music_remember(&now);
    CHECK_TEXT(now.track_name, "bgm35_new");
    CHECK_INT(now.should_play, 1);
    CHECK_INT(now.is_looping, 1);
    ticks_to_play(&credits);
    music_remember(&now);
    CHECK_INT(now.should_play, 0);
}

static void test_circle_mid_page_hands_back_the_music_that_was_playing(void)
{
    struct music_memory now;
    int ticks;

    music_request_play("bgm10_new", 1);
    read_rows(rows_out_of_order);
    staff_credits_start(&credits);
    for (ticks = 0; ticks < 300; ticks++) {
        staff_credits_step(&credits, 1);
    }
    CHECK(staff_credits_is_playing(&credits));
    staff_credits_stop(&credits);
    CHECK(!staff_credits_is_playing(&credits));
    CHECK(!staff_credits_page_is_drawn(&credits));
    music_remember(&now);
    CHECK_TEXT(now.track_name, "bgm10_new");
    CHECK_INT(now.should_play, 1);
    staff_credits_stop(&credits);
    music_remember(&now);
    CHECK_TEXT(now.track_name, "bgm10_new");
    forget_music();
}

int main(void)
{
    fake_files_reset();
    run_log_open("run.log");
    staff_credits_clear(&credits);
    test_the_rows_are_read_in_page_order_with_their_pace_and_music();
    test_every_page_plays_once_in_order_and_the_player_ends();
    test_each_page_fades_in_from_black_and_out_to_black();
    test_a_page_waits_in_black_until_its_screens_are_loaded();
    test_the_hold_after_a_page_adds_its_ticks();
    test_the_credits_play_their_music_and_hand_back_silence();
    test_circle_mid_page_hands_back_the_music_that_was_playing();
    return test_finish("unit/staff_credits");
}
