typedef struct Entry00d64 {
    unsigned int base;
    unsigned int size;
    int f8;
    int fc;
} Entry00d64;

typedef struct Table00d64 {
    int f0;
    int f4;
    unsigned int count;
    Entry00d64 entries[1];
} Table00d64;

typedef struct Obj00d64 {
    char pad[0x34];
    char stream[0x5c];
    Table00d64 *table;
} Obj00d64;

extern int func_02000ce8(void *stream, void *dest, unsigned int length, unsigned int address);

int func_02000d64(Obj00d64 *object, unsigned int index, void *dest, unsigned int length, unsigned int offset)
{
    Table00d64 *table = object->table;
    Entry00d64 *entry;
    unsigned int avail;
    if (index >= table->count) {
        return -10;
    }
    entry = &table->entries[index];
    avail = entry->size - offset;
    if (length > avail) {
        length = avail;
    }
    return func_02000ce8(object->stream, dest, length, entry->base + offset);
}
