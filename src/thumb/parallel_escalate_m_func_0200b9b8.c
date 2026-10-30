typedef struct {
    int first;
    int second;
} Pair;

typedef struct Entry {
    int name;
    unsigned short kind;
    unsigned short flags;
    int owner;
    Pair a;
    Pair c;
    Pair b;
    Pair d;
    int f2c;
    int f30;
    int f34;
} Entry;

void func_0200b9b8(Entry *entry, int owner, unsigned short kind, int name, Pair a, Pair b, unsigned short flags, Pair c, Pair d)
{
    entry->name = name;
    entry->kind = kind;
    entry->flags = flags | 0x80;
    entry->owner = owner;
    entry->a = a;
    entry->c = c;
    entry->b = b;
    entry->d = d;
    entry->f2c = 0;
    entry->f30 = 0;
    entry->f34 = 0;
}
