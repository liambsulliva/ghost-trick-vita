struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern unsigned int *func_02000d9c();
extern unsigned int func_02000d64();
extern unsigned int func_02001b00();
extern void func_02000f70();

struct Bank019d4 {
    char pad[0x1400];
    unsigned int bits[32];
};


unsigned int func_020019d4(unsigned int param_1, unsigned char param_2)
{
    unsigned int total;
    int base;
    unsigned int *puVar3;
    int arc;
    int entry;
    unsigned int i;
    unsigned int r;
    int n;
    int d;
    int w;
    int c;
    unsigned int *p;
    struct Bank019d4 *st;
    unsigned int hdr[8];
    unsigned short ent[6];

    base = (int)&data_020b18a0;
    arc = *(int *)(*(int *)((int)&data_020b18a0 + 0x10) + 0x1580);
    total = 0;
    puVar3 = func_02000d9c(arc, param_1, *(int *)(*(int *)(arc + 0x98) + 0xc));
    if (puVar3 == 0) {
        return 0xfffffffb;
    }
    entry = *(int *)(*(int *)(*(int *)(base + 0x10) + 0x1580) + 0x90) + *(int *)puVar3 * 0x10;
    if (*(int *)(entry + 0x14) == 0) {
        total = (*(int *)(entry + 0x10) + 0x1f) & 0xffffffe0;
    }
    if ((param_2 & 2) != 0) {
        i = 0;
        r = func_02000d64(*(int *)(data_020b18a0.data + 0x1580), *puVar3, hdr, 0x20, i);
        if (r != 0x20) {
            return 0xfffffffe;
        }
        for (; i < hdr[7]; i = i + 1) {
            r = func_02000d64(*(int *)(data_020b18a0.data + 0x1580), *puVar3, ent, 0xc, i * 0xc + 0x20);
            if (r != 0xc) {
                return 0xfffffffe;
            }
            p = (unsigned int *)data_020b18a0.data;
            n = ent[2];
            w = n / 32;
            p += w;
            if (((p[0x500] >> (n - w * 32)) & 1) == 0) {
                r = func_02001b00(n, param_2);
                if (r == 0xfffffffe) {
                    return r;
                }
                if (r == 0xfffffffd) {
                    return r;
                }
                total = total + r;
                p = (unsigned int *)(data_020b18a0.data + 0x1400);
                n = *(ent + 2);
                w = (int)(long long)n / 32;
                p += w;
                *p |= 1 << (*(unsigned short *)((char *)ent + 4) - w * 32);
            }
        }
    }
    func_02000f70(data_020b18a0.data + 0x1400, 0, 0x80);
    return total;
}
