#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

enum loading_stage {
    LOADING_READING,
    LOADING_ANIMATIONS,
    LOADING_TEXTURES,
    LOADING_DONE
};

struct loading_counts {
    enum loading_stage stage;
    int done;
    int total;
};

float loading_screen_progress(const struct loading_counts *counts);
void loading_screen_begin(void);
void loading_screen_update(const struct loading_counts *counts, unsigned long long elapsed_us);
float loading_screen_shown_progress(void);
void loading_screen_draw(void);
void loading_screen_warm_up_the_renderer(void);

#endif
