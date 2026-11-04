struct State {
    unsigned int before[4];
    int data;
};

struct Entry {
    unsigned int index;
    unsigned short subs[4];
};

struct Sub {
    unsigned int index : 24;
    unsigned int type : 8;
};

extern struct State data_020b18a0;
extern int func_02000d9c();
extern void func_02001fac(int heap, int value, int size);
extern int func_02001020(int bank, struct Sub *sub, int data);
extern int func_02000dc4(unsigned int index);
extern int func_020010ac(struct Sub *sub, int seq, int wave, unsigned short slot, int pos);
extern unsigned int func_02000d64(int bank, unsigned int index, int pos, int size, int flag);
extern void func_0200128c(int seq, int wave, unsigned short slot);
extern void func_02007a0c(int seq, unsigned short slot, int wave);
extern void func_020013dc(void);

int func_02002564(int id, int base)
{
    struct Entry *entry;
    int seq;
    int wave;
    struct Sub *sub;
    unsigned short i;
    int start;
    int table;
    unsigned int count;
    int n;
    int r;
    unsigned int size;
    int obj;
    int e;
    unsigned int idx;

    start = base;
    entry = (struct Entry *)func_02000d9c(*(int *)(data_020b18a0.data + 0x1580), id, *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x98) + 0x10));
    table = *(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90);
    count = *(unsigned int *)(table + 8);
    if (entry->index >= count) {
        return -10;
    }
    base = (base + 0x1f) & ~0x1f;
    seq = *(int *)(table + entry->index * 16 + 0x14);
    if (*(unsigned int *)(data_020b18a0.data + 0x15a4) & 0x80000000) {
        if ((*(unsigned int *)(data_020b18a0.data + 0x15a4) & 0x40000000) == 0) {
            **(int **)(data_020b18a0.data + 0x1bac) = 0;
            func_02001fac(*(int *)(data_020b18a0.data + 0x1bac) + 4, -1, 0x2000);
        } else {
            **(int **)(data_020b18a0.data + 0x1bb0) = 0;
            func_02001fac(*(int *)(data_020b18a0.data + 0x1bb0) + 4, -1, 0x2000);
        }
    }
    for (i = 0; i < 4; i++) {
        if (entry->subs[i] == 0xffff) {
            continue;
        }
        sub = (struct Sub *)func_02000d9c(*(int *)(data_020b18a0.data + 0x1580), entry->subs[i], *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x98) + 0x14));
        if (sub->type & 1) {
            obj = *(int *)(data_020b18a0.data + 0x1580);
            e = *(int *)(obj + 0x90) + sub->index * 16;
            if (*(int *)(e + 0x10) == -1) {
                r = func_02001020(obj, sub, *(int *)(e + 0x14));
                if (r == -2) {
                    return r;
                }
                *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x10) = func_02000dc4(sub->index);
            }
            wave = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x14);
            n = func_020010ac(sub, seq, wave, i, base);
            if (n == -3 || n == -2) {
                return n;
            }
            base = (base + n + 0x1f) & ~0x1f;
        } else {
            obj = *(int *)(data_020b18a0.data + 0x1580);
            idx = sub->index;
            e = *(int *)(obj + 0x90) + idx * 16;
            if (*(int *)(e + 0x14) == 0) {
                size = func_02000d64(obj, idx, base, *(int *)(e + 0x10), 0);
                if (size < *(unsigned int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x10)) {
                    return -2;
                }
                *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x14) = base;
                base = base + ((size + 0x1f) & ~0x1f);
            }
            wave = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + sub->index * 16 + 0x14);
            if (*(unsigned int *)(data_020b18a0.data + 0x15a4) & 0x80000000) {
                func_0200128c(seq, wave, i);
            }
        }
        func_02007a0c(seq, i, wave);
    }
    if (*(unsigned int *)(data_020b18a0.data + 0x15a4) & 0x80000000) {
        func_020013dc();
    }
    *(int *)(data_020b18a0.data + 0x1ba0) = 0;
    *(unsigned char *)(data_020b18a0.data + 0x1b7d) = 0;
    return base - start;
}
