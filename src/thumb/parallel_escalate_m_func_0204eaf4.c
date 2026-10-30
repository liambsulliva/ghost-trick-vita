extern signed char data_0209bda0;
extern signed char data_0209bda2;
extern unsigned long long func_0207b694();

int func_0204eaf4(unsigned int param_1,int param_2,int param_3)
{
    int hi;
    int lo;
    unsigned long long q;

    lo = (int)*(short *)(((int)&data_0209bda0) + param_2 * 4);
    if (param_3 < lo) {
        return 0x1f;
    }
    hi = (int)*(short *)(((int)&data_0209bda2) + param_2 * 4);
    if (param_3 > hi) {
        return 0;
    }
    q = func_0207b694((hi - param_3) * 0x1f000,hi - lo);
    return (int)(signed char)((int)q >> 0xc);
}
