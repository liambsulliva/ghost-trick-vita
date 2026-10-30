extern int data_020b1748;
extern void func_02072ac0(int dmaNo, void *dest, unsigned int data, unsigned int size, int a, int b, int c);
extern void func_020728f0(int dmaNo, void *dest, unsigned int data, unsigned int size, int c);
extern void func_02072ec4(unsigned int data, void *destp, unsigned int size);

void func_0206fb08(void)
{
    int i;

    if (data_020b1748 != -1) {
        func_02072ac0(data_020b1748, (char *)0x04000330, 0, 0x10, 0, 0, 1);
        func_020728f0(data_020b1748, (char *)0x04000330 + 0x30, 0, 0x60, 1);
    } else {
        func_02072ec4(0, (char *)0x04000330, 0x10);
        func_02072ec4(0, (char *)0x04000330 + 0x30, 0x60);
    }
    for (i = 0; i < 0x20; i++) {
        *(volatile unsigned int *)0x040004d0 = 0;
    }
}
