#ifndef SCREEN_BURST_H
#define SCREEN_BURST_H

void screen_burst_reserve_memory(void);
void screen_burst_note_game_time(unsigned int tick, int route_step, unsigned int route_step_began_at);
void screen_burst_run_for_seconds(int seconds, int ms_between_pictures);
int screen_burst_note_frame_and_say_if_it_ended(void);

#endif
