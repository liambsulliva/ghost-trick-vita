#include <stddef.h>
#include "ogg_pages.h"
#include "stb_vorbis.h"
#include "vorbis_stream.h"

#define STEREO_CHANNELS 2

int vorbis_stream_open(struct vorbis_stream *stream, const unsigned char *data, unsigned int size)
{
    stb_vorbis_alloc memory;
    stb_vorbis_info info;
    int error;

    if (!ogg_pages_are_intact(data, size)) {
        return VORBIS_STREAM_BAD_PAGE;
    }
    memory.alloc_buffer = (char *)stream->decoder_memory;
    memory.alloc_buffer_length_in_bytes = (int)sizeof(stream->decoder_memory);
    error = VORBIS_STREAM_OK;
    stream->decoder = stb_vorbis_open_memory(data, (int)size, &error, &memory);
    if (stream->decoder == NULL) {
        return error;
    }
    info = stb_vorbis_get_info(stream->decoder);
    stream->frame_count = stb_vorbis_stream_length_in_samples(stream->decoder);
    stream->sample_rate = info.sample_rate;
    stream->channel_count = (unsigned int)info.channels;
    stream->decoder_bytes = info.setup_memory_required + info.temp_memory_required;
    return VORBIS_STREAM_OK;
}

unsigned int vorbis_stream_read(struct vorbis_stream *stream, short *stereo_frames, unsigned int frame_count)
{
    return (unsigned int)stb_vorbis_get_samples_short_interleaved(stream->decoder, STEREO_CHANNELS, stereo_frames, (int)(frame_count * STEREO_CHANNELS));
}

int vorbis_stream_seek(struct vorbis_stream *stream, unsigned int frame)
{
    return stb_vorbis_seek(stream->decoder, frame);
}

void vorbis_stream_close(struct vorbis_stream *stream)
{
    stb_vorbis_close(stream->decoder);
    stream->decoder = NULL;
}
