extern unsigned short data_020b1744;
extern unsigned short data_020d0e44;
void func_02078e68(void);

void func_0206f4dc(void)
{
    unsigned int *reg;
    unsigned int value;

    reg = (unsigned int *)0x04000000;
    value = *reg;
    data_020b1744 = 0;
    data_020d0e44 = (value & 0x30000) >> 16;
    *reg = value & 0xfffcffff;
    func_02078e68();
}
