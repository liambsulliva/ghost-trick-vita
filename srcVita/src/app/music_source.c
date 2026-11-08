#include <string.h>
#include "music_source.h"

#define SAMPLES_PER_FRAME 2

int music_source_open(struct music_source *source, const unsigned char *data, unsigned int size)
{
    source->has_loop_points = 0;
    source->next_frame = 0;
    return vorbis_stream_open(&source->stream, data, size);
}

void music_source_set_loop_points(struct music_source *source, unsigned int loop_start, unsigned int loop_last)
{
    source->loop_start = loop_start;
    source->loop_last = loop_last;
    source->has_loop_points = 1;
}

int music_source_begin(struct music_source *source, int is_looping)
{
    source->is_looping = is_looping;
    source->has_ended = 0;
    source->loop_count = 0;
    if (source->next_frame != 0 && !vorbis_stream_seek(&source->stream, 0)) {
        return 0;
    }
    source->next_frame = 0;
    return 1;
}

static unsigned int frames_before_loop_point(const struct music_source *source, unsigned int wanted)
{
    unsigned int frames_left;

    if (!source->is_looping || !source->has_loop_points) {
        return wanted;
    }
    if (source->next_frame > source->loop_last) {
        return 0;
    }
    frames_left = source->loop_last + 1 - source->next_frame;
    return wanted < frames_left ? wanted : frames_left;
}

static int go_back_to_the_loop_start(struct music_source *source)
{
    unsigned int target;

    target = source->has_loop_points ? source->loop_start : 0;
    if (!source->is_looping || !vorbis_stream_seek(&source->stream, target)) {
        return 0;
    }
    source->next_frame = target;
    source->loop_count = source->loop_count + 1;
    return 1;
}

void music_source_fill(struct music_source *source, short *stereo_frames, unsigned int frame_count)
{
    unsigned int filled;
    unsigned int wanted;
    unsigned int got;
    int just_went_back;

    filled = 0;
    just_went_back = 0;
    while (filled < frame_count && !source->has_ended) {
        wanted = frames_before_loop_point(source, frame_count - filled);
        got = 0;
        if (wanted > 0) {
            got = vorbis_stream_read(&source->stream, &stereo_frames[filled * SAMPLES_PER_FRAME], wanted);
        }
        source->next_frame = source->next_frame + got;
        filled = filled + got;
        if (got > 0) {
            just_went_back = 0;
        } else if (just_went_back || !go_back_to_the_loop_start(source)) {
            source->has_ended = 1;
        } else {
            just_went_back = 1;
        }
    }
    memset(&stereo_frames[filled * SAMPLES_PER_FRAME], 0, (frame_count - filled) * SAMPLES_PER_FRAME * sizeof(short));
}

void music_source_skip(struct music_source *source, unsigned int frame_count)
{
    unsigned int target;
    unsigned int loop_length;

    if (frame_count == 0 || source->has_ended) {
        return;
    }
    target = source->next_frame + frame_count;
    if (source->is_looping && source->has_loop_points && target > source->loop_last) {
        loop_length = source->loop_last + 1 - source->loop_start;
        target = source->loop_start + (target - source->loop_start) % loop_length;
        source->loop_count = source->loop_count + 1;
    }
    if (vorbis_stream_seek(&source->stream, target)) {
        source->next_frame = target;
        return;
    }
    if (!go_back_to_the_loop_start(source)) {
        source->has_ended = 1;
    }
}

void music_source_close(struct music_source *source)
{
    vorbis_stream_close(&source->stream);
}
