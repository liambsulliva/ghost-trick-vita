extern char data_020d2f00[];
extern char data_020d2efc[];
extern void CpuSet(const void *src, void *dest, unsigned int control);
extern unsigned int func_02073a04(void);

void func_02079b6c(void)
{
    unsigned int zero;

    zero = 0;
    CpuSet(&zero, data_020d2f00, 0x05000001);
    *(unsigned short *)(data_020d2efc + 6) = func_02073a04();
}
