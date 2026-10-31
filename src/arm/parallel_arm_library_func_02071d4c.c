extern unsigned short data_020d0e92;
extern int func_02071c64(unsigned short *mask);

void func_02071d4c(void)
{
    unsigned int *reg;
    unsigned int value;

    reg = (unsigned int *)0x04000000;
    value = *reg;
    value = value & 0xbfffffff;
    *reg = value;
    func_02071c64(&data_020d0e92);
}
