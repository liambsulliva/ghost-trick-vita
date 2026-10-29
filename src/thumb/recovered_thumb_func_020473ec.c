extern int func_02047120();

unsigned int * func_020473ec(unsigned int *param_1)

{
    func_02047120();
    *param_1 = ((unsigned int)0x020a90d4u);
    param_1[0x2e] = 0xffffffff;
    param_1[0x2f] = 0;
    *(unsigned short *)(param_1 + 0x30) = 0;
    *(unsigned char *)(param_1 + 0x32) = 0;
    *(unsigned char *)((int)param_1 + 0xc9) = 0;
    param_1[0x35] = 0;
    return param_1;
}
