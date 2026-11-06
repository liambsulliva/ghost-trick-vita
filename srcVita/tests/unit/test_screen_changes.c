#include "screen_changes.h"
#include "test_support.h"

#define WIDTH 64
#define HEIGHT 48

static const int first_buffer = 1;
static const int second_buffer = 2;

static void test_the_grid_sees_a_pixel_on_it_and_staggers_every_other_row(void)
{
    static unsigned int pixels[WIDTH * HEIGHT];
    unsigned int plain;

    memset(pixels, 0, sizeof(pixels));
    plain = screen_changes_hash_grid(pixels, WIDTH, WIDTH, HEIGHT);
    pixels[SCREEN_CHANGES_GRID_STEP] = 7;
    CHECK(screen_changes_hash_grid(pixels, WIDTH, WIDTH, HEIGHT) != plain);
    pixels[SCREEN_CHANGES_GRID_STEP] = 0;
    pixels[SCREEN_CHANGES_GRID_STEP * WIDTH + SCREEN_CHANGES_GRID_STEP / 2] = 7;
    CHECK(screen_changes_hash_grid(pixels, WIDTH, WIDTH, HEIGHT) != plain);
    pixels[SCREEN_CHANGES_GRID_STEP * WIDTH + SCREEN_CHANGES_GRID_STEP / 2] = 0;
    pixels[1] = 7;
    CHECK_INT(screen_changes_hash_grid(pixels, WIDTH, WIDTH, HEIGHT), plain);
}

static void test_the_grid_follows_the_row_stride_and_not_the_width(void)
{
    static unsigned int pixels[(WIDTH + 8) * HEIGHT];
    unsigned int plain;

    memset(pixels, 0, sizeof(pixels));
    plain = screen_changes_hash_grid(pixels, WIDTH + 8, WIDTH, HEIGHT);
    pixels[SCREEN_CHANGES_GRID_STEP * 2 * (WIDTH + 8)] = 7;
    CHECK(screen_changes_hash_grid(pixels, WIDTH + 8, WIDTH, HEIGHT) != plain);
}

static void test_the_first_picture_is_not_a_change(void)
{
    struct screen_changes changes;

    screen_changes_reset(&changes);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    CHECK_INT(changes.flips, 1);
    CHECK_INT(changes.changed, 0);
    CHECK_INT(changes.longest_still, 0);
}

static void test_a_new_hash_counts_as_a_change_and_ends_the_still_run(void)
{
    struct screen_changes changes;

    screen_changes_reset(&changes);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    screen_changes_add_flip(&changes, &second_buffer, 10);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    CHECK_INT(changes.changed, 0);
    CHECK_INT(changes.still_run, 2);
    CHECK_INT(changes.longest_still, 2);
    screen_changes_add_flip(&changes, &second_buffer, 11);
    CHECK_INT(changes.flips, 4);
    CHECK_INT(changes.changed, 1);
    CHECK_INT(changes.still_run, 0);
    CHECK_INT(changes.longest_still, 2);
}

static void test_the_same_buffer_twice_counts_as_still(void)
{
    struct screen_changes changes;

    screen_changes_reset(&changes);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    screen_changes_add_same_buffer(&changes);
    CHECK_INT(changes.same_buffer, 1);
    CHECK_INT(changes.flips, 1);
    CHECK_INT(changes.still_run, 1);
}

static void test_a_still_run_carries_across_windows(void)
{
    struct screen_changes changes;

    screen_changes_reset(&changes);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    screen_changes_add_flip(&changes, &second_buffer, 10);
    screen_changes_start_window(&changes);
    CHECK_INT(changes.flips, 0);
    CHECK_INT(changes.changed, 0);
    CHECK_INT(changes.same_buffer, 0);
    CHECK_INT(changes.longest_still, 1);
    screen_changes_add_flip(&changes, &first_buffer, 10);
    CHECK_INT(changes.longest_still, 2);
    screen_changes_add_flip(&changes, &second_buffer, 11);
    screen_changes_start_window(&changes);
    CHECK_INT(changes.longest_still, 0);
}

int main(void)
{
    test_the_grid_sees_a_pixel_on_it_and_staggers_every_other_row();
    test_the_grid_follows_the_row_stride_and_not_the_width();
    test_the_first_picture_is_not_a_change();
    test_a_new_hash_counts_as_a_change_and_ends_the_still_run();
    test_the_same_buffer_twice_counts_as_still();
    test_a_still_run_carries_across_windows();
    return test_finish("unit/screen_changes");
}
