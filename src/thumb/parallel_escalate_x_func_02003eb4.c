struct Entry02003eb4 {
    char before[0xe];
    unsigned char steps;
    unsigned char pad0f;
    unsigned char active;
    unsigned char slot;
    char pad12[6];
    int value;
    unsigned int start;
    unsigned int rate;
    unsigned char index;
    char pad25[3];
    int flag;
    volatile int state;
    unsigned char id;
    unsigned char target;
    char pad32[10];
    unsigned int bits;
    char pad40[8];
    volatile int timer;
};

extern char data_020b18a0;
extern void func_020079b4(unsigned int id, int value);
extern void func_020079bc(unsigned int id, int value);
extern unsigned int func_0207b424(unsigned int value);
extern unsigned int func_0207b3dc(unsigned int value);
extern unsigned int func_0207c1d8(unsigned int first, unsigned int second);
extern unsigned int func_0207bb44(unsigned int first, unsigned int second);
extern unsigned int func_0207bd24(unsigned int first, unsigned int second);
extern int func_0207ba84(unsigned int value);

void func_02003eb4(struct Entry02003eb4 *entry, int value)
{
    unsigned int level;
    unsigned int scale;

    func_020079b4(entry->id, 0);
    entry->timer = 0;
    *(unsigned int *)((int)entry + 0x3c) &= ~4;
    *(unsigned int *)((int)entry + 0x3c) &= ~8;
    entry->state = 3;
    if (value != 0) {
        entry->steps = 0;
        *(unsigned int *)((int)entry + 0x3c) |= 8;
        entry->value = value;
        entry->start = func_0207b424(entry->steps);
        entry->index = entry->target;
        entry->rate = func_0207c1d8(func_0207b3dc(entry->index - entry->steps), func_0207b424(entry->value));
        entry->flag = 1;
        entry->state = 0;
        return;
    }
    level = func_0207b424(entry->target);
    scale = func_0207bb44(level, func_0207b424(*(unsigned char *)(*(int *)(((int)&data_020b18a0) + 0x10) + entry->slot * 8 + ((int)0x00001a82u))));
    func_020079bc(entry->id, func_0207ba84(func_0207bd24(0x3f000000, func_0207c1d8(scale, 0x42fe0000))));
}
