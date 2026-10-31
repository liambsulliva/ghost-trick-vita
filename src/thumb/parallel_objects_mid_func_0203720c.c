extern char data_020cdd0c;
extern int func_02008444();
extern int func_02011454();

int func_0203720c(unsigned char *param_1)
{
    unsigned char *puVar1;

    func_02011454(func_02008444(param_1));
    ((unsigned int *)&data_020cdd0c)[2] = 0;
    param_1[0x55] = 0;
    param_1[0x56] = 0;
    param_1[0x57] = 0;
    param_1[0x58] = 0;
    param_1[0x59] = 0;
    param_1[0x5a] = 0;
    param_1[0x5b] = 0;
    param_1[0x5c] = 0;
    param_1[0x5d] = 0;
    puVar1 = param_1 + 0x5e;
    *(unsigned int *)(param_1 + 0x38) = 0;
    *(unsigned int *)(param_1 + 0x3c) = 0;
    *(unsigned int *)(param_1 + 0x40) = 0;
    *(unsigned int *)(param_1 + 0x44) = 0;
    *(unsigned int *)(param_1 + 0x48) = 0;
    *(unsigned int *)(param_1 + 0x4c) = 0;
    *(unsigned int *)(param_1 + 0x50) = 0;
    *puVar1 = 0;
    param_1[5] = 0;
    data_020cdd0c = 0;
    return 1;
}
