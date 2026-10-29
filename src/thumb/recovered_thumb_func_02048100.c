extern int func_02047120();

unsigned int * func_02048100(unsigned int *param_1)

{
    func_02047120();
    *param_1 = ((unsigned int)0x020a93f8u);
    param_1[0x2e] = 0;
    param_1[0x2f] = 0;
    param_1[0x30] = 0;
    param_1[0x31] = 0;
    param_1[0x32] = 0xffffffff;
    *(unsigned char *)(param_1 + 0x33) = 0;
    return param_1;
}
