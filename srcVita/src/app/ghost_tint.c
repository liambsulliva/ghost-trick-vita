#include "ghost_tint.h"
#include "scene_stage.h"

struct renderer_color ghost_tint_turned(struct renderer_color sissel_color, int ghost)
{
    struct renderer_color turned;

    turned = sissel_color;
    if (ghost == SCENE_GHOST_MISSILE) {
        turned.red = sissel_color.blue;
        turned.green = sissel_color.red;
        turned.blue = sissel_color.green;
    } else if (ghost == SCENE_GHOST_BOSS) {
        turned.red = sissel_color.green;
        turned.green = sissel_color.blue;
        turned.blue = sissel_color.red;
    }
    return turned;
}
