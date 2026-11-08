import struct
import zlib

PACKED_FILE_MAGIC = b"GPK1"
PACKED_FILE_HEADER_SIZE = 8
BEST_COMPRESSION = 9
RAW_DEFLATE_WINDOW_BITS = -15


def packed(data):
    compressor = zlib.compressobj(BEST_COMPRESSION, zlib.DEFLATED, RAW_DEFLATE_WINDOW_BITS)
    return PACKED_FILE_MAGIC + struct.pack("<I", len(data)) + compressor.compress(data) + compressor.flush()


def is_packed(data):
    return len(data) >= PACKED_FILE_HEADER_SIZE and data[:len(PACKED_FILE_MAGIC)] == PACKED_FILE_MAGIC


def unpacked(data):
    if not is_packed(data):
        return data
    size = struct.unpack_from("<I", data, len(PACKED_FILE_MAGIC))[0]
    plain = zlib.decompress(data[PACKED_FILE_HEADER_SIZE:], RAW_DEFLATE_WINDOW_BITS)
    if len(plain) != size:
        raise ValueError("packed file holds %d bytes, header says %d" % (len(plain), size))
    return plain
