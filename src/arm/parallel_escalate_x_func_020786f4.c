extern unsigned int data_020d2edc[];
extern void func_02072cc4(unsigned int dmaNo);
extern void func_02072d20(unsigned int dmaNo);

void func_020786f4(void)
{
    unsigned int dmaNo;

    for (dmaNo = 0; dmaNo <= 3; dmaNo++) {
        volatile unsigned int *cnt;
        unsigned int timing;

        cnt = (volatile unsigned int *)(0x040000b8 + dmaNo * 12);
        data_020d2edc[dmaNo] = *cnt;
        timing = *cnt & 0x38000000;
        switch (timing) {
        case 0x28000000:
            while (*(volatile unsigned int *)0x040001a4 & 0x80000000) {
            }
            break;
        case 0x18000000:
        case 0x20000000:
            func_02072cc4(dmaNo);
            break;
        default:
            if ((*cnt & 0x02000000) == 0) {
                func_02072cc4(dmaNo);
            } else {
                func_02072d20(dmaNo);
            }
            break;
        }
    }
}
