extern unsigned short data_020d0e9c;
extern int func_02071c64(unsigned short *mask);

void func_02071df8(void)
{
    unsigned int *reg;
    unsigned int value;

    reg = (unsigned int *)0x04001000;
    value = *reg;
    value = value & 0x7fffffff;
    *reg = value;
    func_02071c64(&data_020d0e9c);
}
