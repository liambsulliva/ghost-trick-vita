#ifndef LITTLE_ENDIAN_IO_H
#define LITTLE_ENDIAN_IO_H

static inline unsigned int read_u16_little_endian(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8);
}

static inline unsigned int read_u32_little_endian(const unsigned char *bytes)
{
    return read_u16_little_endian(bytes) | (read_u16_little_endian(&bytes[2]) << 16);
}

static inline void write_u16_little_endian(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)(value >> 8);
}

static inline void write_u32_little_endian(unsigned char *bytes, unsigned int value)
{
    write_u16_little_endian(bytes, value & 0xffffu);
    write_u16_little_endian(&bytes[2], value >> 16);
}

#endif
