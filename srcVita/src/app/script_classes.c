#include "script_classes.h"

#define DEFAULT_LEVEL_FOR_HUMAN 6
#define DEFAULT_LEVEL_FOR_DEAD_GIMMICK 4
#define DEFAULT_LEVEL_FOR_GIMMICK 9

int script_class_is_human(unsigned int class_id)
{
    return class_id == SCRIPT_CLASS_HUM_COMMON || class_id == SCRIPT_CLASS_HUMAN;
}

int script_class_is_past_dead(unsigned int class_id)
{
    return class_id == SCRIPT_CLASS_HUM_PAST_DEAD || class_id == SCRIPT_CLASS_HUM_PAST_DEAD_MISSILE;
}

int script_class_is_soul_start(unsigned int class_id)
{
    return class_id == SCRIPT_CLASS_GMK_SOUL_DEAD || class_id == SCRIPT_CLASS_GMK_SOUL_DEAD_MISSILE;
}

unsigned int script_class_of_soul_style(unsigned int class_id)
{
    return script_class_is_human(class_id) ? SCRIPT_CLASS_HUM_COMMON : class_id;
}

int script_class_draws_in_living_view(unsigned int class_id)
{
    return class_id != SCRIPT_CLASS_GMK_SOUL_VISION && class_id != SCRIPT_CLASS_GMK_COMMON_DEAD_SOUL_VIEW && !script_class_is_past_dead(class_id);
}

int script_class_draws_in_soul_view(unsigned int class_id)
{
    return class_id != SCRIPT_CLASS_SCR_OBJ && class_id != SCRIPT_CLASS_GMK_STEALTH && !script_class_is_past_dead(class_id);
}

int script_class_default_level(unsigned int class_id)
{
    if (script_class_is_human(class_id)) {
        return DEFAULT_LEVEL_FOR_HUMAN;
    }
    return class_id == SCRIPT_CLASS_GMK_COMMON_DEAD ? DEFAULT_LEVEL_FOR_DEAD_GIMMICK : DEFAULT_LEVEL_FOR_GIMMICK;
}
