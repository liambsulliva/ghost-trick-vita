#include <stdio.h>
#include <stdlib.h>
#include "animation.h"
#include "test_support.h"

static void test_animation_parses_all_fields(void)
{
    const char text[] =
        "animation walk\n"
        "loops 1\n"
        "frame walk_00 2 10 20 100 200 0 0 0.25 0.5\n"
        "\n"
        "frame walk_01 3 -5.5 21 100 200 0.25 0.5 0.25 0.5\n"
        "frame - 1 0 0 0 0 0 0 0 0\n";
    static struct animation animation;

    CHECK_INT(animation_parse(text, sizeof(text) - 1, &animation), ANIMATION_OK);
    CHECK_TEXT(animation.name, "walk");
    CHECK_INT(animation.loops, 1);
    CHECK_INT(animation.frame_count, 3);
    CHECK(animation.frames[1].u == 0.25f);
    CHECK(animation.frames[1].v_size == 0.5f);
    CHECK(!animation_frame_is_blank(&animation.frames[1]));
    CHECK(animation_frame_is_blank(&animation.frames[2]));
    CHECK_TEXT(animation.frames[1].texture_name, "walk_01");
    CHECK_INT(animation.frames[1].ticks, 3);
    CHECK(animation.frames[1].x == -5.5f);
}

static void test_animation_rejects_bad_input(void)
{
    static struct animation animation;
    const char empty[] = "animation empty\n";
    const char bad_line[] = "dance\n";
    const char missing_field[] = "frame a 1 2 3 4 5\n";
    const char zero_ticks[] = "frame a 0 0 0 1 1 0 0 1 1\n";
    const char tick_rate_line[] = "ticks_per_second 2000000\n";
    char *too_many;
    unsigned int index;
    unsigned int used;

    CHECK_INT(animation_parse(empty, sizeof(empty) - 1, &animation), ANIMATION_ERR_EMPTY);
    CHECK_INT(animation_parse(bad_line, sizeof(bad_line) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(missing_field, sizeof(missing_field) - 1, &animation), ANIMATION_ERR_BAD_LINE);
    CHECK_INT(animation_parse(zero_ticks, sizeof(zero_ticks) - 1, &animation), ANIMATION_ERR_BAD_TICKS);
    CHECK_INT(animation_parse(tick_rate_line, sizeof(tick_rate_line) - 1, &animation), ANIMATION_ERR_BAD_LINE);

    too_many = malloc((ANIMATION_FRAME_MAX + 1) * 40);
    used = 0;
    for (index = 0; index < ANIMATION_FRAME_MAX + 1; index++) {
        used = used + (unsigned int)sprintf(&too_many[used], "frame a 1 0 0 1 1 0 0 1 1\n");
    }
    CHECK_INT(animation_parse(too_many, used, &animation), ANIMATION_ERR_TOO_MANY_FRAMES);
    free(too_many);
}

static void make_two_frame_animation(struct animation *animation, int loops)
{
    const char text[] = "frame a 6 0 0 1 1 0 0 1 1\nframe b 12 0 0 1 1 0 0 1 1\n";

    animation_parse(text, sizeof(text) - 1, animation);
    animation->loops = loops;
}

static int frame_is_never_ready(const struct animation_frame *frame)
{
    (void)frame;
    return 0;
}

static int frame_is_always_ready(const struct animation_frame *frame)
{
    (void)frame;
    return 1;
}

static void test_player_holds_each_frame_for_its_ticks(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 99999, NULL);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 1, NULL);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 199999, NULL);
    CHECK_INT(player.frame_index, 1);
    CHECK(!player.is_finished);
    animation_player_advance(&player, 1, NULL);
    CHECK(player.is_finished);
    CHECK_INT(player.frame_index, 1);
    animation_player_advance(&player, 1000000, NULL);
    CHECK_INT(player.frame_index, 1);
}

static void test_player_loops_and_catches_up_after_a_stall(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 1);
    animation_player_start(&player, &animation);
    animation_player_advance(&player, 350000, NULL);
    CHECK_INT(player.frame_index, 0);
    CHECK_INT(player.time_in_frame_us, 50000);
    CHECK(!player.is_finished);
}

static void test_player_tracks_which_frames_were_shown(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    CHECK_INT(animation_player_count_shown_frames(&player), 1);
    animation_player_advance(&player, 100000, NULL);
    CHECK_INT(animation_player_count_shown_frames(&player), 2);
}

static void test_player_without_animation_is_harmless(void)
{
    static struct animation_player player;

    player.animation = NULL;
    animation_player_advance(&player, 1000, NULL);
    CHECK(animation_player_current_frame(&player) == NULL);
    CHECK_INT(animation_player_count_shown_frames(&player), 0);
}

static void test_player_holds_the_current_frame_until_the_next_is_ready(void)
{
    static struct animation animation;
    static struct animation_player player;

    make_two_frame_animation(&animation, 0);
    animation_player_start(&player, &animation);
    animation_player_advance(&player, 500000, frame_is_never_ready);
    CHECK_INT(player.frame_index, 0);
    CHECK_INT(player.late_frame_count, 1);
    animation_player_advance(&player, 500000, frame_is_never_ready);
    CHECK_INT(player.late_frame_count, 1);
    CHECK_INT(player.frame_index, 0);
    animation_player_advance(&player, 0, frame_is_always_ready);
    CHECK_INT(player.frame_index, 1);
    CHECK_INT(player.time_in_frame_us, 0);
    CHECK_INT(player.late_frame_count, 1);
    CHECK_INT(animation_player_count_shown_frames(&player), 2);
}

int main(void)
{
    test_animation_parses_all_fields();
    test_animation_rejects_bad_input();
    test_player_holds_each_frame_for_its_ticks();
    test_player_loops_and_catches_up_after_a_stall();
    test_player_tracks_which_frames_were_shown();
    test_player_without_animation_is_harmless();
    test_player_holds_the_current_frame_until_the_next_is_ready();
    return test_finish("unit/animation");
}
