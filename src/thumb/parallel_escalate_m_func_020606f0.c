struct Flags020606f0 {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
};

extern void func_02060f54(unsigned char *object, int value);
extern void func_020083c0(void *dst, void *src, void *ptr);
extern void func_020349b0(void *dst, void *src, unsigned int value);
extern void func_02008378(void *dst, void *src, void *ptr);
extern void func_020083b0(void *dst, void *src);
extern int func_0201f0dc(void *v);
extern void func_0200f70c(void *v);
extern void func_02060800(void *dst, void *src, int n);
extern void func_0200f9cc(int *dst, void *src);

void func_020606f0(unsigned char *param_1)
{
    unsigned int i;
    unsigned char dir[12];
    unsigned char next[12];
    unsigned char seg[12];
    int vec[3];
    unsigned char base[12];
    unsigned char step[12];
    unsigned char last[12];
    unsigned char alt[12];
    unsigned char lim[12];

    if (((struct Flags020606f0 *)(param_1 + 0x7c))->bit1) {
        if (param_1[0x7e] == 0) {
            func_02060f54(param_1, 1);
        }
    }
    else if (param_1[0x80] < 0x1e) {
        func_020083c0(base, param_1 + 0x98, *(unsigned char **)(param_1 + 0xd0));
        func_020349b0(dir, base, *(unsigned int *)(param_1 + 0xac));
        for (i = 1; i < param_1[0x7f]; i = (i + 1) & 0xff) {
            func_020349b0(step, dir, i << 12);
            func_02008378(next, step, *(unsigned char **)(param_1 + 0xd0));
            func_020083c0(seg, next, *(unsigned char **)(param_1 + 0xd0) + i * 12);
            if (func_0201f0dc(seg) >= 4) {
                func_0200f70c(vec);
                if (((struct Flags020606f0 *)(param_1 + 0x7c))->bit2) {
                    func_020349b0(last, seg,
                                  *(unsigned int *)(*(unsigned char **)(param_1 + 0xe0) + param_1[0x7f] * 8 - 8));
                    func_020083b0(vec, last);
                }
                else {
                    func_020349b0(alt, seg, *(unsigned int *)(*(unsigned char **)(param_1 + 0xe0) + i * 8));
                    func_020083b0(vec, alt);
                }
                if (func_0201f0dc(vec) < 8) {
                    func_02060800(lim, vec, 8);
                    func_020083b0(vec, lim);
                }
                func_0200f9cc((int *)(*(unsigned char **)(param_1 + 0xd0) + i * 12), vec);
            }
        }
    }
    return;
}
