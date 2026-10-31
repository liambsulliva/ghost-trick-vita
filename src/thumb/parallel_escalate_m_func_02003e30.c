struct Entry02003e30 {
    char before[0xe];
    unsigned char steps;
    unsigned char pad0f;
    unsigned char active;
    char pad11[7];
    int value;
    unsigned int start;
    unsigned int rate;
    unsigned char index;
    char pad25[3];
    int flag;
    int state;
    unsigned char id;
};

extern unsigned int func_0207b424(unsigned int value);
extern unsigned int func_0207b3dc(unsigned int value);
extern unsigned int func_0207c1d8(unsigned int first, unsigned int second);
extern void func_02003a70(unsigned int id);
extern void func_0200799c(unsigned int id);
extern void func_02003b1c(unsigned int id);

void func_02003e30(struct Entry02003e30 *entry, int value, int flag)
{
    unsigned int bits;

    bits = *(unsigned int *)((int)entry + 0x3c) & ~8;
    *(unsigned int *)((int)entry + 0x3c) = bits;
    entry->state = 3;
    if (value != 0) {
        *(unsigned int *)((int)entry + 0x3c) = bits | 8;
        entry->value = value;
        entry->start = func_0207b424(entry->steps);
        entry->index = 0;
        entry->rate = func_0207c1d8(func_0207b3dc(entry->index - entry->steps), func_0207b424(entry->value));
        entry->flag = flag;
        entry->state = 1;
        if (flag == 0) {
            entry->active = 0;
        }
    } else if (flag == 0) {
        func_02003a70(entry->id);
        func_0200799c(entry->id);
    } else {
        func_02003b1c(entry->id);
    }
}
