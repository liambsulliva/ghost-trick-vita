#ifndef SCRIPT_BRIDGE_H
#define SCRIPT_BRIDGE_H

#include "script_outputs.h"
#include "script_tables.h"

void script_bridge_start(const struct script_tables *tables);
const struct script_outputs *script_bridge_outputs(void);
void script_bridge_finish_tick(void);
void script_bridge_take_output_times(char *text, unsigned int size);
void script_bridge_forget_output_times(void);
int script_bridge_balloon_art_is_loading(void);

#endif
