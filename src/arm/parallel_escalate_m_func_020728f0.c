extern void func_01ff80f4(unsigned int dmaNo, unsigned int src, unsigned int dest, unsigned int ctrl, unsigned int mode);

void func_020728f0(unsigned int dmaNo, unsigned int dest, unsigned int src, unsigned int size, int flag)
{
    volatile unsigned int *dmaCntp;

    if (size == 0) {
        return;
    }
    dmaCntp = (volatile unsigned int *)((dmaNo * 3 + 2) * 4 + 0x040000b0);
    do {
    } while (*dmaCntp & 0x80000000);
    if (flag) {
        func_01ff80f4(dmaNo, src, dest, 0x85000000 | (size >> 2), 0x12);
    } else {
        func_01ff80f4(dmaNo, src, dest, 0x05000000 | (size >> 2), 0x16);
    }
    do {
    } while (*dmaCntp & 0x80000000);
}
