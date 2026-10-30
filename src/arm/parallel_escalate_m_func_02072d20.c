extern int func_02075c98();
extern void func_02075cac(int enabled);

void func_02072d20(unsigned int dmaNo)
{
    int enabled;
    volatile unsigned int *cnt;
    volatile unsigned int *p;

    enabled = func_02075c98();
    cnt = (volatile unsigned int *)(0x040000b8 + dmaNo * 12);
    *cnt &= ~0x3a000000;
    *cnt &= ~0x80000000;
    {
        int dummy = cnt[0];
    }
    {
        int dummy = cnt[0];
    }
    if (dmaNo == 0) {
        p = (volatile unsigned int *)(0x040000b0 + dmaNo * 12);
        *p = 0;
        *(p + 1) = 0;
        *(p + 2) = 0x81400001;
    }
    func_02075cac(enabled);
}
