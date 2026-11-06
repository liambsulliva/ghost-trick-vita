#include <stddef.h>
#include <string.h>
#include "screen_changes.h"

#define HASH_START 2166136261u
#define HASH_MULTIPLIER 16777619u

unsigned int screen_changes_hash_grid(const unsigned int *pixels, unsigned int pixels_per_row, unsigned int width, unsigned int height)
{
    const unsigned int *row;
    unsigned int hash;
    unsigned int x;
    unsigned int y;
    unsigned int row_number;

    hash = HASH_START;
    row_number = 0;
    for (y = 0; y < height; y = y + SCREEN_CHANGES_GRID_STEP) {
        row = pixels + y * pixels_per_row;
        x = (row_number % 2 == 0) ? 0 : SCREEN_CHANGES_GRID_STEP / 2;
        for (; x < width; x = x + SCREEN_CHANGES_GRID_STEP) {
            hash = (hash ^ row[x]) * HASH_MULTIPLIER;
        }
        row_number = row_number + 1;
    }
    return hash;
}

void screen_changes_reset(struct screen_changes *changes)
{
    memset(changes, 0, sizeof(*changes));
}

void screen_changes_start_window(struct screen_changes *changes)
{
    changes->flips = 0;
    changes->changed = 0;
    changes->same_buffer = 0;
    changes->longest_still = changes->still_run;
}

static void count_still_frame(struct screen_changes *changes)
{
    changes->still_run = changes->still_run + 1;
    if (changes->still_run > changes->longest_still) {
        changes->longest_still = changes->still_run;
    }
}

void screen_changes_add_same_buffer(struct screen_changes *changes)
{
    changes->same_buffer = changes->same_buffer + 1;
    count_still_frame(changes);
}

void screen_changes_add_flip(struct screen_changes *changes, const void *buffer, unsigned int hash)
{
    int is_first_picture;

    is_first_picture = changes->last_buffer == NULL;
    changes->flips = changes->flips + 1;
    if (is_first_picture || hash != changes->last_hash) {
        if (!is_first_picture) {
            changes->changed = changes->changed + 1;
        }
        changes->still_run = 0;
    } else {
        count_still_frame(changes);
    }
    changes->last_buffer = buffer;
    changes->last_hash = hash;
}
