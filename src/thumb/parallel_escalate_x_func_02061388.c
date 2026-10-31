extern unsigned int *func_02008444();
extern void func_0201013c(unsigned int *object, int value);
extern int func_02036f3c(unsigned char *object);
extern int func_02010194(unsigned int *object);
extern int func_020615e8(unsigned char *object);

void func_02061388(unsigned char *p)
{
    int best;
    unsigned int index;
    unsigned short i;
    int off;
    int *entry;
    int limit;
    int value;
    int *cur;

    if (p[0x7e] != 1) {
        func_0201013c(func_02008444(), 0);
        return;
    }
    best = 0x2000000;
    index = 0xffffffff;
    for (i = 0; i < *(unsigned short *)(p + 0xec); i++) {
        off = i * 0x18;
        if (func_02036f3c(*(unsigned char **)(*(int *)(p + 0xf0) + off + 4)) == 0) {
            entry = (int *)(*(int *)(p + 0xf0) + off);
            if (entry[1] != func_02010194(func_02008444()) && entry[0] < best) {
                limit = func_020615e8(p);
                cur = (int *)(*(int *)(p + 0xf0) + off);
                value = *cur;
                if (value < limit && cur[5] != *(int *)(p + 0xf4)) {
                    best = value;
                    index = i;
                }
            }
        }
    }
    if (index < 0xffffffff) {
        func_0201013c(func_02008444(), *(int *)(*(int *)(p + 0xf0) + index * 0x18 + 4));
        return;
    }
    func_0201013c(func_02008444(), 0);
}
