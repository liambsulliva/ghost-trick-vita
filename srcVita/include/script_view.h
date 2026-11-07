#ifndef SCRIPT_VIEW_H
#define SCRIPT_VIEW_H

#include "script_outputs.h"
#include "script_tables.h"

void script_view_start(const struct script_tables *tables);
void script_view_talk_changed(const struct script_talk_view *view);
void script_view_soul_changed(int is_on, int with_effect);
void script_view_cursor_moved(int is_shown, float x, float y, float from_x, float from_y, int motion);
void script_view_core_marked(int object_index, float x, float y, int is_dead);
void script_view_soul_flash_changed(float alpha);
void script_view_film_frame_changed(int is_on);
void script_view_film_scroll_paused(int is_paused);
void script_view_screen_still_changed(int is_on);
void script_view_swirl_changed(int is_on);
void script_view_movie_started(const char *name);
void script_view_soul_sparks_changed(int elapsed_ticks, float x, float y);
void script_view_mark_changed(const char *object_name, int age_ticks, float x, float y);
void script_view_effect_spawned(const struct script_effect_row *row, const char *name, float x, float y);
void script_view_effect_deleted(const char *name);
void script_view_effect_moved(const char *name, float x, float y, float aim_degrees);
void script_view_advance(void);
void script_view_draw_on_world(void);
void script_view_draw_on_covers(void);
int script_view_wants_snapshot(void);
int script_view_movie_covers_the_screen(void);
void script_view_snapshot_taken(void);

#endif
