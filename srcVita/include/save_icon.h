#ifndef SAVE_ICON_H
#define SAVE_ICON_H

#include "soul_chain.h"

#define SAVE_ICON_SCALE (1.0f / 3.0f)
#define SAVE_ICON_MARGIN 24.0f
#define SAVE_ICON_CORE_BLOBS 4
#define SAVE_ICON_STEP_US 125000ull

void save_icon_reset(void);
void save_icon_show(void);
void save_icon_update(unsigned long long elapsed_us, int is_writing);
int save_icon_is_showing(void);
float save_icon_alpha(void);
unsigned int save_icon_step(void);
int save_icon_body(struct soul_chain_blob *blobs);
void save_icon_core(struct soul_chain_blob *blobs);
void save_icon_draw(void);

#endif
