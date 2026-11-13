#include "script_classes.h"

#define DEFAULT_LEVEL_FOR_HUMAN 6
#define DEFAULT_LEVEL_FOR_DEAD_GIMMICK 4
#define DEFAULT_LEVEL_FOR_GIMMICK 9

int script_class_is_human(unsigned int class_id)
{
    return class_id == SCRIPT_CLASS_HUM_COMMON || class_id == SCRIPT_CLASS_HUMAN;
}

static const unsigned int gimmick_classes[] = {
    0x789c5740u, 0x5984ef37u, 0x7321cb17u, 0x79aa254cu, 0x6743e879u, 0x5f599f93u, 0x4080f0cdu,
    0x42ffa903u, 0x4d5b0354u, 0x12bdf673u, 0x2981d97eu, 0x601aee47u, 0x019fc802u, 0x488e7fabu,
    0x07a51afeu, 0x2e49526bu, 0x6309aa5au, 0x44ffc5e2u, 0x239ff6e5u, 0x1d2e0ed8u,
};

int script_class_is_gimmick(unsigned int class_id)
{
    unsigned int index;

    for (index = 0; index < sizeof(gimmick_classes) / sizeof(gimmick_classes[0]); index++) {
        if (gimmick_classes[index] == class_id) {
            return 1;
        }
    }
    return 0;
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
