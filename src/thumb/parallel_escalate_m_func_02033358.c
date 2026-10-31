extern void *func_02008444(void);
extern int func_0200f5c8(void *p);
extern int func_0200f5e8(void *p);
extern void func_020330c0(void *p, int v);
extern void func_020330e0(void *p, int v);
extern void func_02061014(int *out, int obj);

void func_02033358(int param_1)
{
    int v[3];

    func_02061014(v, *(int *)(param_1 + 0x24));
    v[0] = v[0] - func_0200f5c8(func_02008444()) - 0x80000;
    v[1] = -v[1] + func_0200f5e8(func_02008444()) + 0x60000;
    if (v[1] > 0x40000) {
        func_020330c0(func_02008444(), -0x4000);
    } else if (v[1] < -0x40000) {
        func_020330c0(func_02008444(), 0x4000);
    }
    if (v[0] > 0x60000) {
        func_020330e0(func_02008444(), 0x4000);
    } else if (v[0] < -0x60000) {
        func_020330e0(func_02008444(), -0x4000);
    }
}
