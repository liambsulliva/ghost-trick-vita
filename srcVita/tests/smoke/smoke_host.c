#define main app_main
#include "../../src/main.c"
#undef main
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "log_sink.h"
#include "smoke_audio.h"
#include "smoke_renderer.h"

#define FRAME_US 16667ULL
#define DISK_PATH_MAX 1024
#define EXIT_FLOW_OVER 0
#define EXIT_USAGE 2
#define EXIT_OUT_OF_FRAMES 3
#define EXIT_RELAUNCH 10
#define VIDEO_MEMORY_BYTES (112u << 20)
#define MAIN_MEMORY_BYTES (180u << 20)
#define DEVICE_PREFIX "ux0:data/ghosttrick"
#define SET_FOLDER_COUNT 2

static const char *const set_device_folders[SET_FOLDER_COUNT] = { ASSET_SETS_DS_FOLDER, ASSET_SETS_REMASTERED_FOLDER };
static const char *set_disk_folders[SET_FOLDER_COUNT];
static const char *sandbox_folder;
static unsigned long long now_us;
static unsigned long long frames_polled;
static unsigned long long frames_most = 400000ULL;
static FILE *log_output;
static unsigned int reported_dead_draws;
static unsigned int reported_unfilled_draws;
static int flow_was_over;

static int disk_path_of(const char *path, char *disk_path)
{
    unsigned int length;
    int index;

    for (index = 0; index < SET_FOLDER_COUNT; index++) {
        length = (unsigned int)strlen(set_device_folders[index]);
        if (strncmp(path, set_device_folders[index], length) == 0 && (path[length] == '/' || path[length] == '\0')) {
            if (set_disk_folders[index] == NULL) {
                return 0;
            }
            return snprintf(disk_path, DISK_PATH_MAX, "%s%s", set_disk_folders[index], &path[length]) < DISK_PATH_MAX;
        }
    }
    length = (unsigned int)strlen(DEVICE_PREFIX);
    if (strncmp(path, DEVICE_PREFIX, length) == 0) {
        return snprintf(disk_path, DISK_PATH_MAX, "%s%s", sandbox_folder, &path[length]) < DISK_PATH_MAX;
    }
    return snprintf(disk_path, DISK_PATH_MAX, "%s/other/%s", sandbox_folder, path) < DISK_PATH_MAX;
}

unsigned long long clock_now_us(void)
{
    return now_us;
}

void *mem_alloc(unsigned int size)
{
    return size == 0 ? NULL : malloc(size);
}

void mem_free(void *block)
{
    free(block);
}

void log_sink_start(const char *path)
{
    char disk_path[DISK_PATH_MAX];

    if (log_output == NULL && disk_path_of(path, disk_path)) {
        log_output = fopen(disk_path, "ab");
    }
}

void log_sink_write(const char *text, unsigned int size)
{
    if (log_output != NULL) {
        fwrite(text, 1, size, log_output);
    }
}

void log_sink_flush(void)
{
    if (log_output != NULL) {
        fflush(log_output);
    }
}

static unsigned char *make_with_mem_alloc(void *context, unsigned int size)
{
    (void)context;
    return mem_alloc(size);
}

static void give_back_with_mem_free(void *context, unsigned char *buffer)
{
    (void)context;
    mem_free(buffer);
}

int file_read_whole_into(const char *path, const struct file_buffers *buffers, unsigned char **data, unsigned int *size, unsigned long long *open_us)
{
    char disk_path[DISK_PATH_MAX];
    unsigned char *buffer;
    FILE *source;
    long length;

    *open_us = 0;
    if (!disk_path_of(path, disk_path)) {
        return FILE_ERR_OPEN;
    }
    source = fopen(disk_path, "rb");
    if (source == NULL) {
        return FILE_ERR_OPEN;
    }
    fseek(source, 0, SEEK_END);
    length = ftell(source);
    fseek(source, 0, SEEK_SET);
    if (length <= 0) {
        fclose(source);
        return FILE_ERR_SIZE;
    }
    buffer = buffers->make(buffers->context, (unsigned int)length);
    if (buffer == NULL) {
        fclose(source);
        return FILE_ERR_MEMORY;
    }
    if (fread(buffer, 1, (size_t)length, source) != (size_t)length) {
        fclose(source);
        buffers->give_back(buffers->context, buffer);
        return FILE_ERR_READ;
    }
    fclose(source);
    *data = buffer;
    *size = (unsigned int)length;
    return FILE_OK;
}

int file_read_whole(const char *path, unsigned char **data, unsigned int *size)
{
    struct file_buffers buffers;
    unsigned long long open_us;

    buffers.make = make_with_mem_alloc;
    buffers.give_back = give_back_with_mem_free;
    buffers.context = NULL;
    return file_read_whole_into(path, &buffers, data, size, &open_us);
}

static int write_bytes(const char *path, const unsigned char *data, unsigned int size, const char *mode)
{
    char disk_path[DISK_PATH_MAX];
    FILE *output;
    int is_written;

    if (!disk_path_of(path, disk_path)) {
        return FILE_ERR_OPEN;
    }
    output = fopen(disk_path, mode);
    if (output == NULL) {
        return FILE_ERR_OPEN;
    }
    is_written = fwrite(data, 1, size, output) == size;
    fclose(output);
    return is_written ? FILE_OK : FILE_ERR_WRITE;
}

int file_write_whole(const char *path, const unsigned char *data, unsigned int size)
{
    return write_bytes(path, data, size, "wb");
}

int file_append(const char *path, const unsigned char *data, unsigned int size)
{
    return write_bytes(path, data, size, "ab");
}

int file_delete(const char *path)
{
    char disk_path[DISK_PATH_MAX];

    if (!disk_path_of(path, disk_path)) {
        return FILE_ERR_OPEN;
    }
    return remove(disk_path) == 0 ? FILE_OK : FILE_ERR_OPEN;
}

int file_make_folder(const char *path)
{
    char disk_path[DISK_PATH_MAX];

    if (!disk_path_of(path, disk_path)) {
        return FILE_ERR_OPEN;
    }
    return mkdir(disk_path, 0755) == 0 || errno == EEXIST ? FILE_OK : FILE_ERR_OPEN;
}

int file_list_folder(const char *path, file_name_visitor visit, void *context)
{
    char disk_path[DISK_PATH_MAX];
    struct dirent *entry;
    DIR *folder;

    if (!disk_path_of(path, disk_path)) {
        return FILE_ERR_OPEN;
    }
    folder = opendir(disk_path);
    if (folder == NULL) {
        return FILE_ERR_OPEN;
    }
    for (entry = readdir(folder); entry != NULL; entry = readdir(folder)) {
        if (entry->d_name[0] != '.') {
            visit(context, entry->d_name);
        }
    }
    closedir(folder);
    return FILE_OK;
}

void keep_awake_tick(void)
{
}

static void report_bad_draws(void)
{
    if (smoke_renderer_draws_of_dead_textures() != reported_dead_draws) {
        reported_dead_draws = smoke_renderer_draws_of_dead_textures();
        run_log_write("renderer drew_destroyed_texture count=%u", reported_dead_draws);
    }
    if (smoke_renderer_draws_of_unfilled_textures() != reported_unfilled_draws) {
        reported_unfilled_draws = smoke_renderer_draws_of_unfilled_textures();
        run_log_write("renderer drew_unfilled_texture count=%u", reported_unfilled_draws);
    }
}

static void leave(int code)
{
    run_log_write("smoke_host audio music_grains=%llu music_with_sound=%llu sound_grains=%llu sound_with_sound=%llu", smoke_audio_grains(AUDIO_PORT_MUSIC), smoke_audio_grains_with_sound(AUDIO_PORT_MUSIC), smoke_audio_grains(AUDIO_PORT_SOUND), smoke_audio_grains_with_sound(AUDIO_PORT_SOUND));
    run_log_write("smoke_host leave code=%d frames=%llu", code, frames_polled);
    run_log_flush();
    if (log_output != NULL) {
        fclose(log_output);
    }
    exit(code);
}

void input_poll(struct input_state *state)
{
    memset(state, 0, sizeof(*state));
    now_us = now_us + FRAME_US;
    frames_polled = frames_polled + 1;
    report_bad_draws();
    smoke_audio_pump(now_us, FRAME_US);
    if (smoke_run_flows_over() > 0 && !flow_was_over) {
        flow_was_over = 1;
        leave(EXIT_FLOW_OVER);
    }
    if (frames_polled > frames_most) {
        run_log_write("smoke fail name=%s reason=out_of_frames", smoke_run_name());
        leave(EXIT_OUT_OF_FRAMES);
    }
}

void device_stats_read(struct device_stats *stats)
{
    memset(stats, 0, sizeof(*stats));
    stats->video_memory_free = VIDEO_MEMORY_BYTES;
    stats->video_memory_total = VIDEO_MEMORY_BYTES;
    stats->main_memory_free = MAIN_MEMORY_BYTES;
    stats->main_memory_total = MAIN_MEMORY_BYTES;
}

const char *build_info_id(void)
{
    return "smoke-host";
}

int screen_capture_save_bmp(const char *path)
{
    char disk_path[DISK_PATH_MAX];

    if (!disk_path_of(path, disk_path)) {
        return SCREEN_CAPTURE_ERR_WRITE;
    }
    return smoke_renderer_save_bmp(disk_path) ? SCREEN_CAPTURE_OK : SCREEN_CAPTURE_ERR_WRITE;
}

void screen_watch_start(void)
{
}

void screen_watch_note_frame(void)
{
}

void screen_watch_publish(void)
{
}

void screen_burst_reserve_memory(void)
{
}

void screen_burst_note_game_time(unsigned int tick, int acted_route_step, unsigned int route_action_began_at)
{
    (void)tick;
    (void)acted_route_step;
    (void)route_action_began_at;
}

void screen_burst_run_for_seconds(int seconds, int ms_between_pictures)
{
    (void)seconds;
    (void)ms_between_pictures;
}

int screen_burst_note_frame_and_say_if_it_ended(void)
{
    return 0;
}

int sceKernelExitProcess(int result)
{
    return result;
}

int sceAppMgrLoadExec(const char *path, char * const arguments[], const void *options)
{
    (void)path;
    (void)arguments;
    (void)options;
    leave(EXIT_RELAUNCH);
    return 0;
}

int main(int argument_count, char **arguments)
{
    int index;

    for (index = 1; index + 1 < argument_count; index += 2) {
        if (strcmp(arguments[index], "--remastered") == 0) {
            set_disk_folders[1] = arguments[index + 1];
        } else if (strcmp(arguments[index], "--ds") == 0) {
            set_disk_folders[0] = arguments[index + 1];
        } else if (strcmp(arguments[index], "--sandbox") == 0) {
            sandbox_folder = arguments[index + 1];
        } else if (strcmp(arguments[index], "--frames-most") == 0) {
            frames_most = strtoull(arguments[index + 1], NULL, 10);
        } else {
            sandbox_folder = NULL;
            break;
        }
    }
    if (sandbox_folder == NULL) {
        fprintf(stderr, "usage: smoke_host --sandbox <folder> [--remastered <set folder>] [--ds <set folder>] [--frames-most <n>]\n");
        return EXIT_USAGE;
    }
    app_main();
    leave(EXIT_FLOW_OVER);
    return EXIT_FLOW_OVER;
}
