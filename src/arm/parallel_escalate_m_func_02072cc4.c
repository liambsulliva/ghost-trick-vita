extern int func_02075c98();
extern void func_02075cac(int enabled);

void func_02072cc4(unsigned int dmaNo)
{
    int enabled;
    volatile unsigned int *cnt;
    volatile unsigned int *p;

    enabled = func_02075c98();
    cnt = (volatile unsigned int *)(0x040000b8 + dmaNo * 12);
    while (*cnt & 0x80000000) {
    }
    if (dmaNo == 0) {
        p = (volatile unsigned int *)(0x040000b0 + dmaNo * 12);
        *p = 0;
        *(p + 1) = 0;
        *(p + 2) = 0x81400001;
    }
    func_02075cac(enabled);
}
