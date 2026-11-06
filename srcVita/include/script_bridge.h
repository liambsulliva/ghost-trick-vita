#ifndef SCRIPT_BRIDGE_H
#define SCRIPT_BRIDGE_H

#include "script_outputs.h"
#include "script_tables.h"

void script_bridge_start(const struct script_tables *tables);
const struct script_outputs *script_bridge_outputs(void);
void script_bridge_finish_tick(void);
unsigned int script_bridge_tick(void);

#endif
