extern unsigned short data_020d0e44[];
extern int data_020b1744[];
extern void func_02071e90(void);
extern int func_02073a04(void);
extern void func_02075f84(void);
extern void func_020728f0(int dma, void *dest, unsigned int data, unsigned int size, int flag);
extern void func_02072ec4(unsigned int data, void *dest, unsigned int size);

void func_0206f324(void)
{
    int lock;

    *(volatile unsigned short *)0x04000304 = *(volatile unsigned short *)0x04000304 | 0x8000;
    *(volatile unsigned short *)0x04000304 = (unsigned short)((*(volatile unsigned short *)0x04000304 & ~0x20e) | 0x20e);
    *(volatile unsigned short *)0x04000304 = *(volatile unsigned short *)0x04000304 | 1;
    func_02071e90();
    while (data_020d0e44[1] == 0) {
        lock = func_02073a04();
        if (lock == -3) {
            func_02075f84();
        }
        data_020d0e44[1] = (unsigned short)lock;
    }
    *(volatile unsigned short *)0x04000004 = 0;
    *(volatile unsigned int *)0x04000000 = 0;
    if (data_020b1744[1] != -1) {
        func_020728f0(data_020b1744[1], (void *)0x04000008, 0, 0x60, 1);
        *(volatile unsigned short *)0x0400006c = 0;
        func_020728f0(data_020b1744[1], (void *)0x04001000, 0, 0x70, 1);
    }
    else {
        func_02072ec4(0, (void *)0x04000008, 0x60);
        *(volatile unsigned short *)0x0400006c = 0;
        func_02072ec4(0, (void *)0x04001000, 0x70);
    }
    *(volatile unsigned short *)0x04000020 = 0x100;
    *(volatile unsigned short *)0x04000026 = 0x100;
    *(volatile unsigned short *)0x04000030 = 0x100;
    *(volatile unsigned short *)0x04000036 = 0x100;
    *(volatile unsigned short *)0x04001020 = 0x100;
    *(volatile unsigned short *)0x04001026 = 0x100;
    *(volatile unsigned short *)0x04001030 = 0x100;
    *(volatile unsigned short *)0x04001036 = 0x100;
}
