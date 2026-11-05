typedef struct {
    int pad[4];
    char *work;
} Globals;

extern Globals data_020b18a0;
extern int func_02000d9c(int object, unsigned int index, int offset);
extern void func_02000f70(void *object, int value, int extra);
extern int func_02001b00(unsigned int param_1, unsigned int param_2);

int func_020018f0(unsigned int id, unsigned int flags, signed char *list)
{
    char *obj;
    int *entry;
    int rec;
    int size;
    int i;
    int q;
    int *bits;
    signed char v;
    int r;

    size = 0;
    obj = *(char **)(data_020b18a0.work + 0x1580);
    entry = (int *)func_02000d9c((int)obj, id, *(int *)(*(char **)(obj + 0x98) + 8));
    if (entry == 0) {
        return -5;
    }
    rec = *(int *)(*(int *)(data_020b18a0.work + 0x1580) + 0x90) + *entry * 0x10;
    if (*(int *)(rec + 0x14) == 0) {
        size = (*(int *)(rec + 0x10) + 0x1f) & ~0x1f;
    }
    if (flags & 2) {
        if (flags & 4) {
            i = 0;
            if (list[i] != -1) {
                func_02000f70((void *)(*(int *)((int)&data_020b18a0 + 0x10) + 0x13e0), i, 0x20);
                while (*(signed char *)((int)list + i) != -1) {
                    bits = (int *)(data_020b18a0.work + 0x13e0);
                    v = list[i];
                    q = v / 32;
                    bits += q;
                    *bits |= 1 << (*(signed char *)((char *)list + i) - q * 32);
                    i++;
                }
            }
        }
        r = func_02001b00(*(unsigned short *)(entry + 1), flags);
        if (r == -2) {
            return r;
        }
        if (r == -3) {
            return r;
        }
        size += r;
        func_02000f70((void *)(data_020b18a0.work + 0x13e0), -1, 0x20);
    }
    return size;
}
