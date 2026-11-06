#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc32.h"
#include "music_engine.h"
#include "music_source.h"
#include "pcm_ramp.h"

#define INPUT_LINE_MAX 128
#define SAMPLES_PER_FRAME 2
#define GRAIN_BYTES (MUSIC_GRAIN_FRAMES * SAMPLES_PER_FRAME * (unsigned int)sizeof(short))
#define PENDING_RAMP_MAX 64
#define NO_LOOP_POINT -1

struct pending_ramp {
    unsigned long long at;
    unsigned int frames;
    int start_gain;
    int target_gain;
};

static struct music_source source;
static struct pcm_ramp ramp;
static struct pending_ramp pending_ramps[PENDING_RAMP_MAX];
static int pending_ramp_count;
static short grain[MUSIC_GRAIN_FRAMES * SAMPLES_PER_FRAME];
static unsigned long long frames_done;
static unsigned int crc_total;

static unsigned char *read_whole_file(const char *path, unsigned int *size)
{
    unsigned char *data;
    FILE *input;
    long length;

    input = fopen(path, "rb");
    if (input == NULL) {
        return NULL;
    }
    fseek(input, 0, SEEK_END);
    length = ftell(input);
    fseek(input, 0, SEEK_SET);
    data = malloc((size_t)length);
    if (data == NULL || fread(data, 1, (size_t)length, input) != (size_t)length) {
        fclose(input);
        return NULL;
    }
    fclose(input);
    *size = (unsigned int)length;
    return data;
}

static void begin_ramps_due_now(void)
{
    int index;
    int kept;

    kept = 0;
    for (index = 0; index < pending_ramp_count; index++) {
        if (pending_ramps[index].at == frames_done) {
            pcm_ramp_begin(&ramp, pending_ramps[index].start_gain, pending_ramps[index].target_gain, pending_ramps[index].frames);
        } else {
            pending_ramps[kept] = pending_ramps[index];
            kept = kept + 1;
        }
    }
    pending_ramp_count = kept;
}

static void make_next_grain(void)
{
    begin_ramps_due_now();
    if (frames_done == 0 || source.has_ended || pcm_ramp_has_faded_out(&ramp)) {
        memset(grain, 0, sizeof(grain));
    } else {
        music_source_fill(&source, grain, MUSIC_GRAIN_FRAMES);
        pcm_ramp_apply(&ramp, grain, MUSIC_GRAIN_FRAMES);
    }
    frames_done = frames_done + MUSIC_GRAIN_FRAMES;
}

static void print_window(unsigned int frame_count)
{
    unsigned int window_crc;
    unsigned int first_source_frame;
    unsigned int done;

    window_crc = 0;
    first_source_frame = source.next_frame;
    for (done = 0; done < frame_count; done = done + MUSIC_GRAIN_FRAMES) {
        make_next_grain();
        window_crc = crc32_add(window_crc, (const unsigned char *)grain, GRAIN_BYTES);
        crc_total = crc32_add(crc_total, (const unsigned char *)grain, GRAIN_BYTES);
    }
    printf("%08x %08x %u %u\n", window_crc, crc_total, first_source_frame, source.loop_count);
}

static int read_script(void)
{
    char line[INPUT_LINE_MAX];
    struct pending_ramp next;
    unsigned int frame_count;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "ramp %llu %u %d %d", &next.at, &next.frames, &next.start_gain, &next.target_gain) == 4) {
            if (pending_ramp_count >= PENDING_RAMP_MAX) {
                fprintf(stderr, "too many ramps in one window\n");
                return 0;
            }
            pending_ramps[pending_ramp_count] = next;
            pending_ramp_count = pending_ramp_count + 1;
        } else if (sscanf(line, "window %u", &frame_count) == 1) {
            print_window(frame_count);
        } else {
            fprintf(stderr, "bad line: %s", line);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    unsigned char *data;
    unsigned int size;
    int loop_start;
    int loop_last;
    int error;

    if (argc != 5) {
        fprintf(stderr, "usage: music_reference <ogg path> <looping> <loop start or -1> <loop last or -1>\n");
        return 2;
    }
    data = read_whole_file(argv[1], &size);
    if (data == NULL) {
        fprintf(stderr, "cannot read %s\n", argv[1]);
        return 2;
    }
    error = music_source_open(&source, data, size);
    if (error != VORBIS_STREAM_OK) {
        fprintf(stderr, "cannot decode %s: error %d\n", argv[1], error);
        return 2;
    }
    loop_start = atoi(argv[3]);
    loop_last = atoi(argv[4]);
    if (loop_start != NO_LOOP_POINT && loop_last != NO_LOOP_POINT) {
        music_source_set_loop_points(&source, (unsigned int)loop_start, (unsigned int)loop_last);
    }
    music_source_begin(&source, atoi(argv[2]));
    return read_script() ? 0 : 1;
}
