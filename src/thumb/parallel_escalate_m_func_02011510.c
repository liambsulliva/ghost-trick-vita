struct Entry {
    unsigned char data[0x80];
};
struct Owner {
    unsigned char pad[0x5c];
    unsigned char count;
    unsigned char index;
    unsigned char pad2[6];
    struct Entry current;
    struct Entry spare;
    struct Entry entries[1];
};
extern void func_020114d4(struct Entry *dest, struct Entry *src, int size);

void func_02011510(struct Owner *owner)
{
    unsigned int index;
    index = owner->index;
    if (index > owner->count && index != 0) {
        func_020114d4(&owner->current, &owner->entries[index], sizeof(struct Entry));
    }
}
