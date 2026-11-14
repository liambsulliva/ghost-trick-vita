#include <string.h>
#include "script_pack.h"

#define PACK_MAGIC "GTSP"
#define PACK_MAGIC_SIZE 4u
#define PACK_VERSION 1u
#define PACK_HEADER_SIZE 12u
#define ENTRY_TAIL_SIZE 8u

static unsigned int read_u16(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8);
}

static unsigned int read_u32(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8) | ((unsigned int)bytes[2] << 16) | ((unsigned int)bytes[3] << 24);
}

static int walk_entry(const struct script_pack *pack, unsigned int *at, const unsigned char **name, unsigned int *name_size, unsigned int *offset, unsigned int *size)
{
    if (*at + 2u > pack->size) {
        return 0;
    }
    *name_size = read_u16(&pack->bytes[*at]);
    if (*at + 2u + *name_size + ENTRY_TAIL_SIZE > pack->size) {
        return 0;
    }
    *name = &pack->bytes[*at + 2u];
    *offset = read_u32(&pack->bytes[*at + 2u + *name_size]);
    *size = read_u32(&pack->bytes[*at + 2u + *name_size + 4u]);
    *at = *at + 2u + *name_size + ENTRY_TAIL_SIZE;
    return *offset <= pack->size && *size <= pack->size - *offset;
}

int script_pack_open(struct script_pack *pack, const unsigned char *bytes, unsigned int size)
{
    unsigned int at;
    unsigned int index;
    const unsigned char *name;
    unsigned int name_size;
    unsigned int offset;
    unsigned int entry_size;

    pack->bytes = bytes;
    pack->size = size;
    pack->count = 0;
    if (size < PACK_HEADER_SIZE || memcmp(bytes, PACK_MAGIC, PACK_MAGIC_SIZE) != 0 || read_u32(&bytes[4]) != PACK_VERSION) {
        return 0;
    }
    at = PACK_HEADER_SIZE;
    for (index = 0; index < read_u32(&bytes[8]); index++) {
        if (!walk_entry(pack, &at, &name, &name_size, &offset, &entry_size)) {
            return 0;
        }
    }
    pack->count = read_u32(&bytes[8]);
    return 1;
}

int script_pack_find(const struct script_pack *pack, const char *name, const char *extension, const unsigned char **data, unsigned int *size)
{
    unsigned int at;
    unsigned int index;
    unsigned int name_length;
    unsigned int extension_length;
    const unsigned char *entry_name;
    unsigned int entry_name_size;
    unsigned int offset;
    unsigned int entry_size;

    name_length = (unsigned int)strlen(name);
    extension_length = (unsigned int)strlen(extension);
    at = PACK_HEADER_SIZE;
    for (index = 0; index < pack->count; index++) {
        if (!walk_entry(pack, &at, &entry_name, &entry_name_size, &offset, &entry_size)) {
            return 0;
        }
        if (entry_name_size == name_length + extension_length && memcmp(entry_name, name, name_length) == 0 && memcmp(entry_name + name_length, extension, extension_length) == 0) {
            *data = &pack->bytes[offset];
            *size = entry_size;
            return 1;
        }
    }
    return 0;
}
