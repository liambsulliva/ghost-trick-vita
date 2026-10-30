extern unsigned int data_020c9e48;
unsigned int *func_02008444(void);
void func_ov004_020d6c8c(unsigned int *, int);

void func_ov004_020d6c60(unsigned char *param_1)
{
    func_ov004_020d6c8c(func_02008444(), 1);
    *(unsigned int *)(param_1 + 0x20) = data_020c9e48;
    data_020c9e48 = data_020c9e48 & 0xfffff9ff;
    param_1[6] = 0x1e;
}
