struct Entry {
    unsigned short id;
    unsigned short active;
    char pad[12];
};

struct Owner {
    char pad[6];
    unsigned short count;
    char pad8[8];
    struct Entry entries[1];
};

struct Entry *func_02002d44(struct Owner *owner, unsigned int key)
{
    struct Entry *entry = owner->entries;
    int i = 0;
    int count = owner->count;
    for (; i < count; i++) {
        if (entry->id == key) {
            break;
        }
        entry++;
    }
    if (i == count) {
        entry = 0;
    }
    return entry;
}
