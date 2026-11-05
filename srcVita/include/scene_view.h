#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include "scene_player.h"

#define SCENE_VIEW_PLAYFIELD_X 120.0f
#define SCENE_VIEW_PLAYFIELD_Y 2.0f
#define SCENE_VIEW_PLAYFIELD_WIDTH 720.0f
#define SCENE_VIEW_PLAYFIELD_HEIGHT 540.0f
#define SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL 2.8125f

void scene_view_draw(const struct scene_picture *picture);

#endif
