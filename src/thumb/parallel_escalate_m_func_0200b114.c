struct Target {
    int (**table)();
};
struct Entry {
    int first;
    int second;
    int key;
    unsigned char low : 3;
    unsigned char flag : 1;
    unsigned char kind : 4;
    unsigned char state;
    unsigned short id;
    int value;
    struct Target *target;
};
extern void func_0200af44(struct Entry *entry,unsigned int id,unsigned int kind);
unsigned char func_0200b114(struct Entry *entry)
{
    if (entry->state == 0xff) {
        if (entry->flag && entry->key == -1) {
            func_0200af44(entry,entry->id,entry->kind);
        }
        return (unsigned char)entry->target->table[2](entry->target,entry->key);
    }
    return entry->state;
}
