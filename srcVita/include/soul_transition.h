#ifndef SOUL_TRANSITION_H
#define SOUL_TRANSITION_H

void soul_transition_begin_entering(void);
void soul_transition_begin_leaving(void);
void soul_transition_stop(void);
void soul_transition_release_textures(void);
void soul_transition_restart_sweep(void);
void soul_transition_advance(void);
int soul_transition_wants_snapshot(void);
void soul_transition_snapshot_taken(void);
void soul_transition_draw(void);
void soul_transition_draw_ghost_lines(void);

#endif
