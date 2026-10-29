extern int func_02031288();
extern int func_02047120();

unsigned int * func_020487f0(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02047120();
    *param_1 = ((unsigned int)0x020a9f7cu);
    *(unsigned char *)(param_1 + 0x30) = 0;
    *(unsigned char *)((int)param_1 + 0xc1) = 0;
    uVar1 = func_02031288(0x400);
    param_1[0x2e] = uVar1;
    return param_1;
}
