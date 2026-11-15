#include "fakes/fake_renderer.h"
#include "ds_units.h"
#include "scene_effects.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "test_support.h"

#define LIGHT_RADIUS 20.0f
#define DS_SCREEN_MIDDLE_X 128.0f
#define DS_SCREEN_MIDDLE_Y 96.0f
#define A_CAMERA_X 300.0f
#define A_CAMERA_Y 150.0f
#define NOT_DRAWN (-10000.0f)

static float light_centre_x;
static float light_centre_y;

static void draw_and_find_the_light(void)
{
    int index;

    fake_renderer_clear_draws();
    scene_view_draw_covers(scene_stage_view());
    light_centre_x = NOT_DRAWN;
    light_centre_y = NOT_DRAWN;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && !draw->is_rect_batch && draw->screen_area.width == LIGHT_RADIUS * 2.0f * VITA_PIXELS_PER_DS_PIXEL) {
            light_centre_x = draw->screen_area.x + draw->screen_area.width / 2.0f;
            light_centre_y = draw->screen_area.y + draw->screen_area.height / 2.0f;
        }
    }
}

static void set_up_a_light_at_the_ds_screen_middle(void)
{
    fake_renderer_reset();
    scene_view_start();
    scene_stage_reset();
    scene_effects_create_iris(scene_stage_effects(), DS_SCREEN_MIDDLE_X, DS_SCREEN_MIDDLE_Y, LIGHT_RADIUS, 1.0f);
}

static void test_a_pinned_light_stays_put_when_the_camera_moves(void)
{
    set_up_a_light_at_the_ds_screen_middle();
    scene_effects_pin_iris(scene_stage_effects(), 1);
    scene_stage_set_camera(0.0f, 0.0f);
    draw_and_find_the_light();
    CHECK(light_centre_x == (float)RENDERER_WIDTH / 2.0f);
    CHECK(light_centre_y == (float)RENDERER_HEIGHT / 2.0f);
    scene_stage_set_camera(A_CAMERA_X, A_CAMERA_Y);
    draw_and_find_the_light();
    CHECK(light_centre_x == (float)RENDERER_WIDTH / 2.0f);
    CHECK(light_centre_y == (float)RENDERER_HEIGHT / 2.0f);
    scene_view_stop();
}

static void test_a_light_in_the_world_moves_with_the_camera(void)
{
    float first_x;

    set_up_a_light_at_the_ds_screen_middle();
    scene_stage_set_camera(0.0f, 0.0f);
    draw_and_find_the_light();
    first_x = light_centre_x;
    CHECK(first_x != NOT_DRAWN);
    scene_stage_set_camera(A_CAMERA_X, A_CAMERA_Y);
    draw_and_find_the_light();
    CHECK(light_centre_x != NOT_DRAWN);
    CHECK(light_centre_x != first_x);
    scene_view_stop();
}

int main(void)
{
    test_a_pinned_light_stays_put_when_the_camera_moves();
    test_a_light_in_the_world_moves_with_the_camera();
    return test_finish("unit/pinned_light_draw");
}
