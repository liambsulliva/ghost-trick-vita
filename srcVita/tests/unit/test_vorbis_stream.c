#include <stdlib.h>
#include "crc32.h"
#include "fixtures/fixture_facts.h"
#include "fixtures/ogg_edit.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "test_support.h"
#include "vorbis_stream.h"

#define SEEK_COMPARE_FRAMES 2048
#define READ_CHUNK_FRAMES 1024

static struct vorbis_stream stream;
static short whole_track[(SINE_STEREO_FRAMES + READ_CHUNK_FRAMES) * 2];

static unsigned int read_whole_track(void)
{
    unsigned int total;
    unsigned int got;

    total = 0;
    do {
        got = vorbis_stream_read(&stream, &whole_track[total * 2], READ_CHUNK_FRAMES);
        total = total + got;
    } while (got > 0);
    return total;
}

static void test_a_stereo_stream_reports_its_facts_and_gives_every_frame(void)
{
    unsigned int total;

    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    CHECK_INT(stream.sample_rate, 44100);
    CHECK_INT(stream.channel_count, 2);
    CHECK_INT(stream.frame_count, SINE_STEREO_FRAMES);
    CHECK(stream.decoder_bytes > 0);
    CHECK(stream.decoder_bytes < VORBIS_STREAM_DECODER_BYTES);
    total = read_whole_track();
    CHECK_INT(total, SINE_STEREO_FRAMES);
    CHECK(crc32_add(0, (const unsigned char *)whole_track, total * 4) == SINE_STEREO_CRC32);
    vorbis_stream_close(&stream);
    CHECK(stream.decoder == NULL);
}

static void test_a_mono_stream_is_copied_to_both_channels(void)
{
    unsigned int total;
    unsigned int frame;
    int channels_differ;
    int has_sound;

    CHECK_INT(vorbis_stream_open(&stream, sine_mono_ogg, sizeof(sine_mono_ogg)), VORBIS_STREAM_OK);
    CHECK_INT(stream.channel_count, 1);
    CHECK_INT(stream.frame_count, SINE_MONO_FRAMES);
    total = read_whole_track();
    CHECK_INT(total, SINE_MONO_FRAMES);
    channels_differ = 0;
    has_sound = 0;
    for (frame = 0; frame < total; frame++) {
        channels_differ = channels_differ || whole_track[frame * 2] != whole_track[frame * 2 + 1];
        has_sound = has_sound || whole_track[frame * 2] != 0;
    }
    CHECK(!channels_differ);
    CHECK(has_sound);
    vorbis_stream_close(&stream);
}

static void check_a_seek_matches_the_linear_decode(unsigned int target)
{
    static short after_seek[SEEK_COMPARE_FRAMES * 2];
    unsigned int total;

    CHECK(vorbis_stream_seek(&stream, target));
    total = vorbis_stream_read(&stream, after_seek, SEEK_COMPARE_FRAMES);
    CHECK_INT(total, SEEK_COMPARE_FRAMES);
    CHECK(memcmp(after_seek, &whole_track[target * 2], SEEK_COMPARE_FRAMES * 4) == 0);
}

static void test_a_seek_gives_the_same_samples_as_the_linear_decode(void)
{
    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, sizeof(sine_stereo_ogg)), VORBIS_STREAM_OK);
    read_whole_track();
    check_a_seek_matches_the_linear_decode(11025);
    check_a_seek_matches_the_linear_decode(0);
    check_a_seek_matches_the_linear_decode(33000);
    check_a_seek_matches_the_linear_decode(1);
    vorbis_stream_close(&stream);
}

static void test_data_that_is_not_ogg_fails_cleanly(void)
{
    static const unsigned char not_ogg[64] = "this is not an ogg file";

    CHECK(vorbis_stream_open(&stream, not_ogg, sizeof(not_ogg)) != VORBIS_STREAM_OK);
}

static void test_a_cut_header_fails_cleanly(void)
{
    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, 200), VORBIS_STREAM_BAD_PAGE);
}

static void test_a_stream_cut_in_its_audio_fails_cleanly(void)
{
    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, sizeof(sine_stereo_ogg) / 2 + 1500), VORBIS_STREAM_BAD_PAGE);
}

static void test_a_flipped_bit_that_would_send_the_decoder_out_of_bounds_fails_cleanly(void)
{
    static unsigned char flipped[sizeof(sine_stereo_ogg)];

    memcpy(flipped, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    flipped[SINE_STEREO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    CHECK_INT(vorbis_stream_open(&stream, flipped, sizeof(flipped)), VORBIS_STREAM_BAD_PAGE);
}

static void test_a_stream_of_only_headers_opens_with_no_frames(void)
{
    CHECK_INT(vorbis_stream_open(&stream, sine_stereo_ogg, SINE_STEREO_HEADER_PAGES_BYTES), VORBIS_STREAM_OK);
    CHECK_INT(stream.frame_count, 0);
    CHECK_INT(read_whole_track(), 0);
    vorbis_stream_close(&stream);
}

static void test_audio_that_is_all_skipped_packets_reads_as_nothing_and_stops(void)
{
    static unsigned char undecodable[sizeof(sine_mono_ogg)];

    memcpy(undecodable, sine_mono_ogg, sizeof(sine_mono_ogg));
    memset(&undecodable[SINE_MONO_AUDIO_BODY_OFFSET], 0xFF, sizeof(undecodable) - SINE_MONO_AUDIO_BODY_OFFSET);
    ogg_edit_refresh_crcs(undecodable, sizeof(undecodable));
    CHECK_INT(vorbis_stream_open(&stream, undecodable, sizeof(undecodable)), VORBIS_STREAM_OK);
    CHECK_INT(stream.frame_count, SINE_MONO_FRAMES);
    CHECK_INT(read_whole_track(), 0);
    vorbis_stream_close(&stream);
}

int main(void)
{
    test_a_stereo_stream_reports_its_facts_and_gives_every_frame();
    test_a_mono_stream_is_copied_to_both_channels();
    test_a_seek_gives_the_same_samples_as_the_linear_decode();
    test_data_that_is_not_ogg_fails_cleanly();
    test_a_cut_header_fails_cleanly();
    test_a_stream_cut_in_its_audio_fails_cleanly();
    test_a_flipped_bit_that_would_send_the_decoder_out_of_bounds_fails_cleanly();
    test_a_stream_of_only_headers_opens_with_no_frames();
    test_audio_that_is_all_skipped_packets_reads_as_nothing_and_stops();
    return test_finish("unit/vorbis_stream");
}
