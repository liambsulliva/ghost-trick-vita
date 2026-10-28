import sys
import struct


def crc16(data):
    crc = 0xFFFF
    for b in data:
        crc = crc ^ b
        bit = 0
        while bit < 8:
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc = crc >> 1
            bit = bit + 1
    return crc


def main():
    base_path = sys.argv[1]
    built_path = sys.argv[2]
    f = open(base_path, "rb")
    base_header = f.read(0x200)
    f.close()

    stored = struct.unpack_from("<H", base_header, 0x15E)[0]
    if crc16(base_header[0:0x15E]) != stored:
        print("header CRC check failed on the base ROM, refusing to patch")
        sys.exit(1)

    f = open(built_path, "r+b")
    header = bytearray(f.read(0x200))
    header[0x6C] = base_header[0x6C]
    header[0x6D] = base_header[0x6D]
    struct.pack_into("<H", header, 0x15E, crc16(header[0:0x15E]))
    f.seek(0)
    f.write(header)
    f.close()
    print("secure area checksum copied and header CRC set to " + hex(crc16(header[0:0x15E])))


main()
