typedef struct {
    int pad[4];
    char *work;
} Globals;

extern Globals data_020b18a0;
extern int func_02000d9c(int object, unsigned int index, int offset);
extern void func_02000f70(void *object, int value, int extra);
extern int func_02002a88(unsigned int param_1, unsigned int param_2);

int func_02002900(unsigned int id, unsigned int flags, signed char *list)
{
    char *obj;
    int *entry;
    int i;
    int c;
    int q;
    unsigned int word;
    unsigned int *bits;
    int r;

    obj = *(char **)(data_020b18a0.work + 0x1580);
    entry = (int *)func_02000d9c((int)obj, id, *(int *)(*(char **)(obj + 0x98) + 8));
    if (entry == 0) {
        return -5;
    }
    if (flags & 2) {
        if (flags & 4) {
            i = 0;
            if (list[i] != -1) {
                func_02000f70(data_020b18a0.work + 0x13e0, i, 0x20);
                while (list[i] != -1) {
                    bits = (unsigned int *)(data_020b18a0.work + 0x13e0);
                    c = *(signed char *)((int)list + i);
                    q = c / 32;
                    bits += q;
                    *bits |= 1 << (*(signed char *)((char *)list + i) - q * 32);
                    i++;
                }
            }
        }
        r = func_02002a88(*(unsigned short *)(entry + 1), 4);
        if (r == -2) {
            return r;
        }
        if (r == -3) {
            return r;
        }
        func_02000f70(data_020b18a0.work + 0x13e0, -1, 0x20);
    }
    *(int *)(*(char **)(*(char **)(data_020b18a0.work + 0x1580) + 0x90) + entry[0] * 16 + 0x14) = 0;
    bits = (unsigned int *)(data_020b18a0.work + 0x1500);
    r = id / 32;
    bits += r;
    *bits &= ~(1 << (id - r * 32));
    return 0;
}
