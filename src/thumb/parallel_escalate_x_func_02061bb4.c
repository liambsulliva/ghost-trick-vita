typedef struct Entry61bb4 {
    unsigned char flag : 1;
    char pad[7];
    int value;
} Entry61bb4;

typedef struct Obj61bb4 {
    char pad[0x1e4];
    Entry61bb4 entries[1];
} Obj61bb4;

int func_02061bb4(Obj61bb4 *object, int index)
{
    int value;
    if (object->entries[index].flag) {
        value = object->entries[index].value;
    } else {
        value = -1;
    }
    return value;
}
