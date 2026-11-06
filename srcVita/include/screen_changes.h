#ifndef SCREEN_CHANGES_H
#define SCREEN_CHANGES_H

#define SCREEN_CHANGES_GRID_STEP 16

struct screen_changes {
    const void *last_buffer;
    unsigned int last_hash;
    unsigned int flips;
    unsigned int changed;
    unsigned int same_buffer;
    unsigned int still_run;
    unsigned int longest_still;
};

unsigned int screen_changes_hash_grid(const unsigned int *pixels, unsigned int pixels_per_row, unsigned int width, unsigned int height);
void screen_changes_reset(struct screen_changes *changes);
void screen_changes_start_window(struct screen_changes *changes);
void screen_changes_add_same_buffer(struct screen_changes *changes);
void screen_changes_add_flip(struct screen_changes *changes, const void *buffer, unsigned int hash);

#endif
