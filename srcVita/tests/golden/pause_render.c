#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "game_settings.h"
#include "input.h"
#include "pause_menu.h"
#include "run_log.h"
#include "texture_cache.h"
#include "texture_file.h"

#define SET "set"
#define PATH_MOST 512
#define SETTLE_STEPS 80
#define PAN_STEPS 40

static const char *const tables[] = { "pause/cat.txt", "pause/cat_track.txt", "pause/labels.txt", "pause/widgets.txt" };
static const char *const fixed_textures[] = { "pause_backdrop", "pause_spotlight", "pause_labels", "pause_widgets", "hud_sheet" };
static char texture_names[64][ASSET_NAME_MAX];
static int texture_name_count;

static void add_disk_file(const char *assets_folder, const char *relative_path)
{
    char disk_path[PATH_MOST];
    char fake_path[PATH_MOST];
    unsigned char *bytes;
    FILE *source;
    long length;

    snprintf(disk_path, sizeof(disk_path), "%s/%s", assets_folder, relative_path);
    snprintf(fake_path, sizeof(fake_path), "%s/%s", SET, relative_path);
    source = fopen(disk_path, "rb");
    if (source == NULL) {
        fprintf(stderr, "missing %s\n", disk_path);
        exit(1);
    }
    fseek(source, 0, SEEK_END);
    length = ftell(source);
    fseek(source, 0, SEEK_SET);
    bytes = malloc((size_t)length);
    if (bytes == NULL || fread(bytes, 1, (size_t)length, source) != (size_t)length) {
        exit(1);
    }
    fclose(source);
    if (texture_file_is_packed(bytes, (unsigned int)length)) {
        unsigned int unpacked_size = texture_file_unpacked_size(bytes);
        unsigned char *unpacked = malloc(unpacked_size);

        if (unpacked == NULL || !texture_file_unpack(bytes, (unsigned int)length, unpacked, unpacked_size)) {
            fprintf(stderr, "cannot unpack %s\n", disk_path);
            exit(1);
        }
        fake_files_add(fake_path, unpacked, unpacked_size);
        free(unpacked);
    } else {
        fake_files_add(fake_path, bytes, (unsigned int)length);
    }
    free(bytes);
}

static void add_texture(const char *assets_folder, const char *name)
{
    char relative_path[PATH_MOST];

    snprintf(relative_path, sizeof(relative_path), "textures/%s.gtex", name);
    add_disk_file(assets_folder, relative_path);
    snprintf(texture_names[texture_name_count], ASSET_NAME_MAX, "%s", name);
    texture_name_count = texture_name_count + 1;
}

static void press(unsigned int buttons)
{
    struct input_state input;

    memset(&input, 0, sizeof(input));
    input.pressed = buttons;
    texture_cache_upload_finished_reads(16);
    texture_cache_begin_frame();
    pause_menu_update(&input, PAUSE_TICK_US);
}

static void wait_steps(int count)
{
    int step;

    for (step = 0; step < count; step++) {
        press(0);
    }
}

static void press_each(const char *buttons)
{
    const char *letter;

    for (letter = buttons; *letter != '\0'; letter++) {
        switch (*letter) {
        case 'u':
            press(INPUT_UP);
            break;
        case 'd':
            press(INPUT_DOWN);
            break;
        case 'l':
            press(INPUT_LEFT);
            break;
        case 'r':
            press(INPUT_RIGHT);
            break;
        case 'x':
            press(INPUT_CROSS);
            break;
        case 'o':
            press(INPUT_CIRCLE);
            break;
        case '.':
            wait_steps(PAN_STEPS);
            break;
        default:
            break;
        }
    }
    wait_steps(PAN_STEPS);
}

static const char *name_of(renderer_texture texture)
{
    int index;
    const struct cached_texture *entry;

    for (index = 0; index < texture_name_count; index++) {
        entry = texture_cache_use_or_start_loading(texture_names[index]);
        if (entry != NULL && entry->texture == texture) {
            return texture_names[index];
        }
    }
    return "unknown";
}

static void print_draws(void)
{
    int index;
    const struct fake_draw *draw;

    fake_renderer_clear_draws();
    pause_menu_draw();
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        draw = fake_renderer_draw(index);
        if (draw->is_filled_rect) {
            printf("rect %g %g %g %g %g %g %g %g\n", draw->screen_area.x, draw->screen_area.y, draw->screen_area.width, draw->screen_area.height, draw->color.red, draw->color.green, draw->color.blue, draw->color.alpha);
        } else {
            printf("sprite %s %g %g %g %g %.6f %.6f %.6f %.6f %g\n", name_of(draw->texture), draw->screen_area.x, draw->screen_area.y, draw->screen_area.width, draw->screen_area.height, draw->texture_area.x, draw->texture_area.y, draw->texture_area.width, draw->texture_area.height, draw->color.alpha);
        }
    }
}

int main(int argc, char **argv)
{
    char name[ASSET_NAME_MAX];
    unsigned int index;
    int page;

    if (argc < 3) {
        fprintf(stderr, "usage: pause_render <assets folder> <buttons: u d l r x o, . waits>\n");
        return 2;
    }
    fake_files_reset();
    run_log_open("render.log");
    fake_renderer_reset();
    fake_reader_reset();
    for (index = 0; index < sizeof(tables) / sizeof(tables[0]); index++) {
        add_disk_file(argv[1], tables[index]);
    }
    for (index = 0; index < sizeof(fixed_textures) / sizeof(fixed_textures[0]); index++) {
        add_texture(argv[1], fixed_textures[index]);
    }
    texture_cache_init(SET "/textures", 256u << 20);
    game_settings_defaults(game_settings_now());
    if (!pause_menu_load(SET)) {
        fprintf(stderr, "pause art not ready\n");
        return 1;
    }
    for (page = 0; page < pause_cat_page_count(); page++) {
        snprintf(name, sizeof(name), "pause_cat_%02d", page);
        add_texture(argv[1], name);
    }
    pause_menu_open();
    wait_steps(SETTLE_STEPS);
    press_each(argv[2]);
    for (index = 0; index < (unsigned int)texture_name_count; index++) {
        const struct cached_texture *entry = texture_cache_use_or_start_loading(texture_names[index]);
        fprintf(stderr, "texture %s state=%d width=%u\n", texture_names[index], entry ? (int)entry->state : -1, entry ? entry->width : 0u);
    }
    print_draws();
    run_log_flush();
    {
        unsigned int log_size;
        const unsigned char *log_bytes = fake_files_contents("render.log", &log_size);
        if (log_bytes != NULL) {
            fwrite(log_bytes, 1, log_size, stderr);
        }
    }
    return 0;
}
