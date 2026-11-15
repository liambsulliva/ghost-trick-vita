import struct

WRAPPER_SIZE = 86
WRAPPER_MAGIC = b"srcd"
WRAPPER_FORMAT_TAG = b"ogg "
OGG_MAGIC = b"OggS"
OFFSET_OGG_SIZE = 8
OFFSET_FORMAT_TAG = 12
OFFSET_CHANNELS = 28
OFFSET_RATE = 36
OFFSET_LOOP_FLAG = 52
OFFSET_LOOP_START = 53
OFFSET_LOOP_LAST = 57
OFFSET_AUDIO_START = 78
OFFSET_PAGE_FRAMES = 6
PAGE_FRAMES_SIZE = 8
PLAYBACK_RATE = 44100


def read_stream(data, source_name):
    if len(data) < WRAPPER_SIZE + len(OGG_MAGIC):
        raise ValueError(source_name + " is too short to hold an srcd wrapper")
    if data[:len(WRAPPER_MAGIC)] != WRAPPER_MAGIC:
        raise ValueError(source_name + " does not start with srcd; use the path under natives/android/streaming")
    if data[OFFSET_FORMAT_TAG:OFFSET_FORMAT_TAG + len(WRAPPER_FORMAT_TAG)] != WRAPPER_FORMAT_TAG:
        raise ValueError(source_name + " does not say it holds Ogg audio")
    if struct.unpack_from("<I", data, OFFSET_AUDIO_START)[0] != WRAPPER_SIZE:
        raise ValueError(source_name + " does not have its audio right after the 86-byte wrapper")
    if struct.unpack_from("<I", data, OFFSET_OGG_SIZE)[0] != len(data) - WRAPPER_SIZE:
        raise ValueError(source_name + " has the wrong Ogg size in its wrapper")
    if data[WRAPPER_SIZE:WRAPPER_SIZE + len(OGG_MAGIC)] != OGG_MAGIC:
        raise ValueError(source_name + " has no Ogg file after its wrapper")
    loop_points = None
    if data[OFFSET_LOOP_FLAG] != 0:
        loop_points = (struct.unpack_from("<I", data, OFFSET_LOOP_START)[0], struct.unpack_from("<I", data, OFFSET_LOOP_LAST)[0])
    return {
        "ogg": data[WRAPPER_SIZE:],
        "channels": struct.unpack_from("<I", data, OFFSET_CHANNELS)[0],
        "rate": struct.unpack_from("<I", data, OFFSET_RATE)[0],
        "loop_points": loop_points,
    }


def check_playback_rate(stream, source_name):
    if stream["rate"] != PLAYBACK_RATE:
        raise ValueError("%s is %d Hz, and the app only plays %d Hz audio; choose another file or add a resampler" % (source_name, stream["rate"], PLAYBACK_RATE))


def loop_points_at_playback_rate(loop_points, rate):
    if loop_points is None:
        return None
    return tuple(int(round(point * PLAYBACK_RATE / rate)) for point in loop_points)


def ogg_frame_count(ogg):
    page = ogg.rfind(OGG_MAGIC)
    if page < 0 or page + OFFSET_PAGE_FRAMES + PAGE_FRAMES_SIZE > len(ogg):
        raise ValueError("the sound has no Ogg page to count its frames from")
    return int.from_bytes(ogg[page + OFFSET_PAGE_FRAMES:page + OFFSET_PAGE_FRAMES + PAGE_FRAMES_SIZE], "little")


def loop_points_inside(loop_points, frame_count):
    start, last = loop_points
    last = min(last, frame_count - 1)
    if start > last:
        raise ValueError("a loop from frame %d to %d does not fit a sound of %d frames" % (loop_points[0], loop_points[1], frame_count))
    return (start, last)


def loop_text(loop_points, ogg):
    return "%d %d\n" % loop_points_inside(loop_points, ogg_frame_count(ogg))
