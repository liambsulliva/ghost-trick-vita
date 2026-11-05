#include <stddef.h>
#include <psp2/ctrl.h>
#include "input.h"
#include "memory.h"
#include "renderer.h"

#define CHECKER_SIZE 64
#define CHECKER_TILE 8

static void fill_checker(unsigned char *pixels)
{
    int x;
    int y;
    int index;
    int tile;

    for (y = 0; y < CHECKER_SIZE; y++) {
        for (x = 0; x < CHECKER_SIZE; x++) {
            index = (y * CHECKER_SIZE + x) * 4;
            tile = ((x / CHECKER_TILE) + (y / CHECKER_TILE)) % 2;
            if (tile) {
                pixels[index] = 255;
                pixels[index + 1] = 200;
                pixels[index + 2] = 80;
            } else {
                pixels[index] = 40;
                pixels[index + 1] = 60;
                pixels[index + 2] = 120;
            }
            pixels[index + 3] = 255;
        }
    }
}

int main(void)
{
    struct input_state input;
    unsigned char *pixels;
    GLuint texture;

    renderer_init();

    pixels = mem_alloc(CHECKER_SIZE * CHECKER_SIZE * 4);
    if (pixels == NULL) {
        renderer_shutdown();
        return 1;
    }
    fill_checker(pixels);
    texture = renderer_texture_create(pixels, CHECKER_SIZE, CHECKER_SIZE);
    mem_free(pixels);

    input.held = 0;
    input.pressed = 0;
    while (1) {
        input_poll(&input);
        if (input.pressed & SCE_CTRL_START) {
            break;
        }
        renderer_begin_frame(0.05f, 0.05f, 0.1f);
        renderer_draw_triangle();
        renderer_draw_textured_quad(texture);
        renderer_end_frame();
    }

    renderer_texture_destroy(texture);
    renderer_shutdown();
    return 0;
}
