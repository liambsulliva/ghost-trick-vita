typedef struct Handle00e00 {
    unsigned int id : 24;
    unsigned int flags : 8;
} Handle00e00;

typedef struct Header00e00 {
    char pad[0x38];
    int count;
} Header00e00;

extern int func_02000d64(void *archive, unsigned int index, void *dest, unsigned int length, unsigned int offset);

int func_02000e00(void *archive, Handle00e00 *handle)
{
    Header00e00 header;
    int result = func_02000d64(archive, handle->id, &header, sizeof(header), 0);
    if (result == -3 || result == -2) {
        return result;
    }
    return (int)((char *)(header.count * 8) + 0x3c) + 0x80;
}
