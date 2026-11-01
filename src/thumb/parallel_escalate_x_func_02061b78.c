typedef struct Entry61b78 {
    unsigned char flag : 1;
    char pad[11];
} Entry61b78;

typedef struct Obj61b78 {
    char pad[0x1e4];
    Entry61b78 entries[1];
} Obj61b78;

int func_02061b78(Obj61b78 *object, int index)
{
    return object->entries[index].flag;
}
