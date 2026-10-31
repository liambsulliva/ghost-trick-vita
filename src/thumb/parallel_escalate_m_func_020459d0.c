extern int data_020a86a4;
extern void func_02044ef4();

int *func_020459d0(int *param_1)
{
    unsigned char *pbVar1;

    func_02044ef4(param_1);
    *param_1 = (int)&data_020a86a4;
    pbVar1 = (unsigned char *)param_1;
    pbVar1[0x130] = pbVar1[0x130] & ~2;
    pbVar1[0x130] = pbVar1[0x130] & ~4;
    pbVar1[0x130] = pbVar1[0x130] & ~0x18;
    pbVar1[0x130] = pbVar1[0x130] & ~0xe0;
    param_1[0x56] = 0;
    param_1[0x57] = 0;
    param_1[0x58] = -1;
    param_1[0x59] = 0;
    param_1[0x5a] = 0;
    return param_1;
}
