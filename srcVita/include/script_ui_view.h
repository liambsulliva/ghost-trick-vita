#ifndef SCRIPT_UI_VIEW_H
#define SCRIPT_UI_VIEW_H

#include "script_outputs.h"
#include "script_tables.h"

void script_ui_view_start(const struct script_tables *tables);
void script_ui_view_clear(void);
void script_ui_view_clip_started(const char *name, int level_from, int level_to);
void script_ui_view_hud_changed(const struct script_hud_view *view);
void script_ui_view_tap_balloon_opened(int slot, int kind, float anchor_x, float anchor_y);
void script_ui_view_tap_balloon_closed(int slot);
int script_ui_view_pill_at(float x, float y);
int script_ui_view_tap_balloon_kind_at(float x, float y);
int script_ui_view_rewind_button_is_at(float x, float y);
void script_ui_view_advance(void);
void script_ui_view_draw(void);

#endif
