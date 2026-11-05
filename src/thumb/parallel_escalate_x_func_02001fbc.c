struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern unsigned int *func_02000d9c();
extern unsigned int func_02000d64();
extern void func_02000f70(void *object, int value, int extra);
extern int func_020021e8();

int func_02001fbc(unsigned int param_1, int param_2, unsigned char param_3, signed char *param_4)
{
    int start;
    int ctx;
    unsigned int *ent;
    unsigned int n;
    int node;
    unsigned int flags;
    int v;
    int *bits;
    int q;
    int r;

    start = param_2;
    ctx = *(int *)(data_020b18a0.data + 0x1580);
    ent = func_02000d9c(ctx, param_1, *(int *)(*(int *)(ctx + 0x98) + 8));
    if (ent == 0) {
        return -5;
    }
    if (*(int *)(*(int *)(*(int *)(*(int *)((int)&data_020b18a0 + 0x10) + 0x1580) + 0x90) + *(int *)ent * 0x10 + 0x14) == 0) {
        n = func_02000d64((int)*(int *)(data_020b18a0.data + 0x1580), *ent, param_2, *(unsigned int *)(*(int *)((int)*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x90) + (int)*(char **)ent * 0x10 + 0x10), 0);
        node = *(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + *ent * 0x10;
        if (n < *(unsigned int *)(node + 0x10)) {
            return -2;
        }
        *(int *)(node + 0x14) = param_2;
        param_2 = param_2 + n;
    }
    flags = *(unsigned int *)(data_020b18a0.data + 0x15a4);
    if ((flags & 0x80000000) != 0) {
        *(unsigned int *)(data_020b18a0.data + 0x15a4) = flags & 0xbfffffff;
    }
    if ((param_3 & 2) != 0) {
        if ((param_3 & 4) != 0) {
            param_1 = 0;
            if (param_4[param_1] != -1) {
                func_02000f70((void *)(data_020b18a0.data + 0x13e0), param_1, 0x20);
                while (*(signed char *)((int)param_4 + param_1) != -1) {
                    bits = (int *)(data_020b18a0.data + 0x13e0);
                    v = param_4[param_1];
                    q = v / 32;
                    bits += q;
                    *bits |= 1 << (*(signed char *)((char *)param_4 + param_1) - q * 32);
                    param_1++;
                }
            }
        }
        r = func_020021e8(*(unsigned short *)(ent + 1), param_2, param_3);
        if (r == -2) {
            return r;
        }
        if (r == -3) {
            return r;
        }
        param_2 = param_2 + r;
        func_02000f70((void *)(data_020b18a0.data + 0x13e0), -1, 0x20);
    }
    return param_2 - start;
}
