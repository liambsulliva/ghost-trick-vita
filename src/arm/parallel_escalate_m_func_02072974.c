extern void func_02072e3c(unsigned int dmaNo, unsigned int src, unsigned int size, unsigned int dir);
extern void func_01ff80f4(unsigned int dmaNo, unsigned int src, unsigned int dest, unsigned int ctrl, unsigned int mode);

void func_02072974(unsigned int dmaNo, unsigned int src, unsigned int dest, unsigned int size, int flag)
{
    volatile unsigned int *dmaCntp;

    if (size == 0) {
        return;
    }
    func_02072e3c(dmaNo, src, size, 0);
    dmaCntp = (volatile unsigned int *)((dmaNo * 3 + 2) * 4 + 0x040000b0);
    do {
    } while (*dmaCntp & 0x80000000);
    if (flag) {
        func_01ff80f4(dmaNo, src, dest, 0x84000000 | (size >> 2), 2);
    } else {
        func_01ff80f4(dmaNo, src, dest, 0x04000000 | (size >> 2), 6);
    }
    do {
    } while (*dmaCntp & 0x80000000);
}
