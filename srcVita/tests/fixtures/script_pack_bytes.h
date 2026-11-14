#ifndef SCRIPT_PACK_BYTES_H
#define SCRIPT_PACK_BYTES_H

#include <string.h>

static unsigned int script_pack_put_u32(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)value;
    at[1] = (unsigned char)(value >> 8);
    at[2] = (unsigned char)(value >> 16);
    at[3] = (unsigned char)(value >> 24);
    return 4;
}

static unsigned int make_script_pack(unsigned char *bytes, const char *const *names, const char *const *datas, int count)
{
    unsigned int at;
    unsigned int offset;
    int index;

    memcpy(bytes, "GTSP", 4);
    at = 4;
    at = at + script_pack_put_u32(&bytes[at], 1);
    at = at + script_pack_put_u32(&bytes[at], (unsigned int)count);
    offset = at;
    for (index = 0; index < count; index++) {
        offset = offset + 2 + (unsigned int)strlen(names[index]) + 8;
    }
    for (index = 0; index < count; index++) {
        bytes[at] = (unsigned char)strlen(names[index]);
        bytes[at + 1] = 0;
        memcpy(&bytes[at + 2], names[index], strlen(names[index]));
        at = at + 2 + (unsigned int)strlen(names[index]);
        at = at + script_pack_put_u32(&bytes[at], offset);
        at = at + script_pack_put_u32(&bytes[at], (unsigned int)strlen(datas[index]));
        offset = offset + (unsigned int)strlen(datas[index]);
    }
    for (index = 0; index < count; index++) {
        memcpy(&bytes[at], datas[index], strlen(datas[index]));
        at = at + (unsigned int)strlen(datas[index]);
    }
    return at;
}

#endif
