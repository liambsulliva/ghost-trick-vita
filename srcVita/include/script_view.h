#ifndef SCRIPT_VIEW_H
#define SCRIPT_VIEW_H

#include "script_outputs.h"
#include "script_tables.h"

void script_view_start(const struct script_tables *tables);
void script_view_talk_changed(const struct script_talk_view *view);
void script_view_soul_changed(int is_on);
void script_view_cursor_moved(int is_shown, float x, float y, float from_x, float from_y, int motion);
void script_view_core_marked(int object_index, float x, float y);
void script_view_effect_spawned(const struct script_effect_row *row, const char *name, float x, float y);
void script_view_effect_deleted(const char *name);
void script_view_advance(void);
void script_view_draw(void);

#endif
