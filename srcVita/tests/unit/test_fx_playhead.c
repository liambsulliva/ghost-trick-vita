#include <string.h>
#include "fx_playhead.h"
#include "script_tables.h"
#include "test_support.h"

#define BLANK_FRAME "fxframe -1 0 0 0 0 0 0 0 0 0 0 0 0\n"
#define FOUR_FRAMES BLANK_FRAME BLANK_FRAME BLANK_FRAME BLANK_FRAME
#define TICKS_TO_RUN 400

static struct script_tables tables;

static const struct script_fx_movie *movie_of(const char *text, const char *name)
{
    script_tables_clear(&tables);
    CHECK(script_tables_read_effects(&tables, text, (unsigned int)strlen(text)));
    CHECK_INT(tables.rows_dropped, 0);
    return script_tables_find_fx_movie(&tables, name);
}

static int frame_after(const struct script_fx_movie *movie, int ticks, int *is_over)
{
    return fx_playhead_frame_at_age(movie, tables.fx_jumps, ticks, is_over);
}

static void test_a_movie_with_no_jumps_plays_as_its_age_says(void)
{
    const struct script_fx_movie *movie;
    int is_over;

    movie = movie_of("fxmovie loops 2 1 1 -1 0\n" FOUR_FRAMES, "loops");
    CHECK_INT(frame_after(movie, 0, &is_over), 0);
    CHECK_INT(frame_after(movie, 1, &is_over), 0);
    CHECK_INT(frame_after(movie, 2, &is_over), 1);
    CHECK_INT(frame_after(movie, 7, &is_over), 3);
    CHECK_INT(frame_after(movie, 8, &is_over), 1);
    CHECK_INT(frame_after(movie, 14, &is_over), 1);
    CHECK_INT(is_over, 0);
    movie = movie_of("fxmovie holds 2 2 0 -1 0\n" FOUR_FRAMES, "holds");
    CHECK_INT(frame_after(movie, 100, &is_over), 3);
    CHECK_INT(is_over, 0);
    movie = movie_of("fxmovie ends 2 0 0 -1 0\n" FOUR_FRAMES, "ends");
    CHECK_INT(frame_after(movie, 7, &is_over), 3);
    CHECK_INT(is_over, 0);
    (void)frame_after(movie, 8, &is_over);
    CHECK_INT(is_over, 1);
}

static void test_the_table_keeps_each_movie_s_jumps_and_status_frames(void)
{
    const struct script_fx_movie *movie;

    movie = movie_of("fxmovie first 1 1 0 -1 0\nfxjump 1 0 0 0\n" FOUR_FRAMES "fxmovie second 1 1 0 -1 0\nfxjump 0 0 1 0\nfxjump 0 -1 0 0\nfxstatus 1 2\n" FOUR_FRAMES, "second");
    CHECK(movie != NULL);
    CHECK_INT(movie->first_jump, 1);
    CHECK_INT(movie->jump_count, 2);
    CHECK_INT(tables.fx_jumps[1].condition, SCRIPT_FX_JUMP_WHEN_STATUS_IS);
    CHECK_INT(tables.fx_jumps[2].target, SCRIPT_FX_NO_FRAME);
    CHECK_INT(movie->status_frames[0], SCRIPT_FX_NO_FRAME);
    CHECK_INT(movie->status_frames[1], 2);
    CHECK_INT(movie->frame_count, 4);
    movie = script_tables_find_fx_movie(&tables, "first");
    CHECK_INT(movie->jump_count, 1);
    CHECK_INT(movie->status_frames[1], SCRIPT_FX_NO_FRAME);
}

static void test_a_movie_that_reads_status_loops_until_the_game_changes_it(void)
{
    const struct script_fx_movie *movie;
    struct fx_playhead playhead;
    int tick;
    int furthest;

    movie = movie_of("fxmovie fountain 1 0 0 -1 0\nfxjump 1 0 1 0\nfxjump 1 -1 0 0\n" FOUR_FRAMES, "fountain");
    fx_playhead_start(&playhead);
    furthest = 0;
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        fx_playhead_advance(&playhead, movie, tables.fx_jumps);
        furthest = playhead.frame > furthest ? playhead.frame : furthest;
    }
    CHECK_INT(furthest, 1);
    CHECK_INT(playhead.is_over, 0);
    fx_playhead_set_status(&playhead, movie, tables.fx_jumps, 1);
    CHECK_INT(playhead.status, 1);
    for (tick = 0; tick < 4 && !playhead.is_over; tick++) {
        fx_playhead_advance(&playhead, movie, tables.fx_jumps);
        furthest = playhead.frame > furthest ? playhead.frame : furthest;
    }
    CHECK_INT(furthest, 3);
    CHECK_INT(playhead.is_over, 1);
}

static void test_a_movie_with_status_labels_goes_to_the_label(void)
{
    const struct script_fx_movie *movie;
    struct fx_playhead playhead;
    int tick;

    movie = movie_of("fxmovie kettle 1 1 0 -1 0\nfxjump 1 1 0 0\nfxjump 3 3 0 0\nfxstatus 1 2\n" FOUR_FRAMES, "kettle");
    fx_playhead_start(&playhead);
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        fx_playhead_advance(&playhead, movie, tables.fx_jumps);
    }
    CHECK_INT(playhead.frame, 1);
    fx_playhead_set_status(&playhead, movie, tables.fx_jumps, 0);
    CHECK_INT(playhead.frame, 1);
    fx_playhead_set_status(&playhead, movie, tables.fx_jumps, 1);
    CHECK_INT(playhead.frame, 2);
    for (tick = 0; tick < TICKS_TO_RUN; tick++) {
        fx_playhead_advance(&playhead, movie, tables.fx_jumps);
    }
    CHECK_INT(playhead.frame, 3);
}

static void test_a_rolled_jump_is_taken_about_one_time_in_its_sides(void)
{
    const struct script_fx_movie *movie;
    struct fx_playhead playhead;
    int tick;
    int visits_to_the_rare_frame;

    movie = movie_of("fxmovie fire 1 1 0 -1 0\nfxjump 0 2 2 5\nfxjump 0 0 0 0\nfxjump 2 0 0 0\n" FOUR_FRAMES, "fire");
    fx_playhead_start(&playhead);
    visits_to_the_rare_frame = 0;
    for (tick = 0; tick < 1000; tick++) {
        fx_playhead_advance(&playhead, movie, tables.fx_jumps);
        visits_to_the_rare_frame = visits_to_the_rare_frame + (playhead.frame == 2);
    }
    CHECK(visits_to_the_rare_frame > 100);
    CHECK(visits_to_the_rare_frame < 250);
}

int main(void)
{
    test_a_movie_with_no_jumps_plays_as_its_age_says();
    test_the_table_keeps_each_movie_s_jumps_and_status_frames();
    test_a_movie_that_reads_status_loops_until_the_game_changes_it();
    test_a_movie_with_status_labels_goes_to_the_label();
    test_a_rolled_jump_is_taken_about_one_time_in_its_sides();
    return test_finish("unit/fx_playhead");
}
