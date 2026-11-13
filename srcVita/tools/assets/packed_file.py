import struct
import zlib

PACKED_FILE_MAGIC = b"GPK1"
PACKED_FILE_HEADER_SIZE = 8
BEST_COMPRESSION = 9
RAW_DEFLATE_WINDOW_BITS = -15


def packed(data):
    compressor = zlib.compressobj(BEST_COMPRESSION, zlib.DEFLATED, RAW_DEFLATE_WINDOW_BITS)
    return PACKED_FILE_MAGIC + struct.pack("<I", len(data)) + compressor.compress(data) + compressor.flush()

