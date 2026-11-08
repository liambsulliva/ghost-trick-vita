#include "playback_speed.h"
#include "test_support.h"

#define TWICE (2 * PLAYBACK_SPEED_NORMAL)
#define FOUR_TIMES (4 * PLAYBACK_SPEED_NORMAL)

static void test_normal_speed_skips_nothing_and_plays_everything(void)
{
    CHECK_INT((int)playback_speed_frames_to_skip(4096, PLAYBACK_SPEED_NORMAL), 0);
    CHECK_INT((int)playback_speed_frames_to_play(11025, 4800, PLAYBACK_SPEED_NORMAL), 11025);
}

static void test_a_faster_speed_skips_what_the_game_ran_past(void)
{
    CHECK_INT((int)playback_speed_frames_to_skip(4096, TWICE), 4096);
    CHECK_INT((int)playback_speed_frames_to_skip(4096, FOUR_TIMES), 3 * 4096);
}

static void test_a_sped_up_sound_keeps_its_share_but_never_less_than_the_floor(void)
{
    CHECK_INT((int)playback_speed_frames_to_play(11025, 4800, TWICE), 5512);
    CHECK_INT((int)playback_speed_frames_to_play(11025, 4800, FOUR_TIMES), 4800);
    CHECK_INT((int)playback_speed_frames_to_play(3000, 4800, FOUR_TIMES), 3000);
}

static void test_speeds_below_normal_are_held_at_normal(void)
{
    playback_speed_set(PLAYBACK_SPEED_NORMAL / 2);
    CHECK_INT(playback_speed_get(), PLAYBACK_SPEED_NORMAL);
    playback_speed_set(FOUR_TIMES);
    CHECK_INT(playback_speed_get(), FOUR_TIMES);
    playback_speed_set(PLAYBACK_SPEED_NORMAL);
}

int main(void)
{
    test_normal_speed_skips_nothing_and_plays_everything();
    test_a_faster_speed_skips_what_the_game_ran_past();
    test_a_sped_up_sound_keeps_its_share_but_never_less_than_the_floor();
    test_speeds_below_normal_are_held_at_normal();
    return test_finish("unit/playback_speed");
}
