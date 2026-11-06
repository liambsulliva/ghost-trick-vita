#ifndef VORBIS_STREAM_H
#define VORBIS_STREAM_H

#define VORBIS_STREAM_OK 0
#define VORBIS_STREAM_BAD_PAGE (-1)
#define VORBIS_STREAM_DECODER_BYTES (256 * 1024)

struct stb_vorbis;

struct vorbis_stream {
    struct stb_vorbis *decoder;
    unsigned long long decoder_memory[VORBIS_STREAM_DECODER_BYTES / sizeof(unsigned long long)];
    unsigned int frame_count;
    unsigned int sample_rate;
    unsigned int channel_count;
    unsigned int decoder_bytes;
};

int vorbis_stream_open(struct vorbis_stream *stream, const unsigned char *data, unsigned int size);
unsigned int vorbis_stream_read(struct vorbis_stream *stream, short *stereo_frames, unsigned int frame_count);
int vorbis_stream_seek(struct vorbis_stream *stream, unsigned int frame);
void vorbis_stream_close(struct vorbis_stream *stream);

#endif
