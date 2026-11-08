#ifndef MUSIC_SOURCE_H
#define MUSIC_SOURCE_H

#include "vorbis_stream.h"

struct music_source {
    struct vorbis_stream stream;
    unsigned int next_frame;
    unsigned int loop_start;
    unsigned int loop_last;
    unsigned int loop_count;
    int has_loop_points;
    int is_looping;
    int has_ended;
};

int music_source_open(struct music_source *source, const unsigned char *data, unsigned int size);
void music_source_set_loop_points(struct music_source *source, unsigned int loop_start, unsigned int loop_last);
int music_source_begin(struct music_source *source, int is_looping);
void music_source_fill(struct music_source *source, short *stereo_frames, unsigned int frame_count);
void music_source_skip(struct music_source *source, unsigned int frame_count);
void music_source_close(struct music_source *source);

#endif
