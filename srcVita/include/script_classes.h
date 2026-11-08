#ifndef SCRIPT_CLASSES_H
#define SCRIPT_CLASSES_H

#define SCRIPT_CLASS_HUM_COMMON 0x5a390072u
#define SCRIPT_CLASS_HUMAN 0x1dc76216u
#define SCRIPT_CLASS_HUM_PAST_DEAD 0x531e576eu
#define SCRIPT_CLASS_GMK_SOUL_VISION 0x6309aa5au
#define SCRIPT_CLASS_GMK_COMMON_DEAD_SOUL_VIEW 0x5f599f93u
#define SCRIPT_CLASS_GMK_STEALTH 0x44ffc5e2u
#define SCRIPT_CLASS_SCR_OBJ 0x45427fc2u
#define SCRIPT_CLASS_GMK_COMMON_DEAD 0x6743e879u

int script_class_is_human(unsigned int class_id);
unsigned int script_class_of_soul_style(unsigned int class_id);
int script_class_draws_in_living_view(unsigned int class_id);
int script_class_draws_in_soul_view(unsigned int class_id);
int script_class_default_level(unsigned int class_id);

#endif
