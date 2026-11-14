#ifndef SCRIPT_PACK_H
#define SCRIPT_PACK_H

#define SCRIPT_PACK_FILE_NAME "scripts"
#define SCRIPT_PACK_EXTENSION ".pack"

struct script_pack {
    const unsigned char *bytes;
    unsigned int size;
    unsigned int count;
};

int script_pack_open(struct script_pack *pack, const unsigned char *bytes, unsigned int size);
int script_pack_find(const struct script_pack *pack, const char *name, const char *extension, const unsigned char **data, unsigned int *size);

#endif
